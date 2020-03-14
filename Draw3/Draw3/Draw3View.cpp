// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// Draw3View.cpp : implementation of the CDraw3View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Draw3.h"
#endif

#include "Draw3Doc.h"
#include "Draw3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDraw3View

IMPLEMENT_DYNCREATE(CDraw3View, CView)

BEGIN_MESSAGE_MAP(CDraw3View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDraw3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CDraw3View construction/destruction

CDraw3View::CDraw3View() noexcept
{
	// Null these Direct2D objects so that if creation fails,
	// we don't accidentaly use them
	d_factory = nullptr;
	d_renderer = nullptr;
	d_border = d_content = d_text = nullptr;
	windowHandle = 0;

	initialized = bound = false;

	currentTextElement = nullptr;

	D2D1_FACTORY_OPTIONS ops;

	ops.debugLevel = D2D1_DEBUG_LEVEL_WARNING;

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, ops, &d_factory);
	if (FAILED(hr))
		return;
	D2D1_RENDER_TARGET_PROPERTIES renderProp;
	ZeroMemory(&renderProp, sizeof(renderProp));
	renderProp.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
	renderProp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE);
	renderProp.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
	renderProp.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;

	hr = d_factory->CreateDCRenderTarget(&renderProp, &d_renderer);

	if (FAILED(hr))
		return;

	// Introduced in Draw3 for Text Support
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&dw_factory));

	if (FAILED(hr))
		return;

	initialized = drawClear = true;
}

CDraw3View::~CDraw3View()
{
	if (d_border)
		d_border->Release();
	if (d_content)
		d_content->Release();
	if (d_text)
		d_text->Release();
	if (d_renderer)
		d_renderer->Release();
	if (d_factory)
		d_factory->Release();
	d_factory = nullptr;
	d_renderer = nullptr;
	d_border = d_content = d_text = nullptr;
}

BOOL CDraw3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDraw3View drawing

void CDraw3View::OnDraw(CDC* pDC)
{
	if (!initialized || !drawClear)
		return;

	// Bind the DC Render Target to our Context Handle if not already done

	CWnd* win = AfxGetMainWnd();
	if (win)
	{
		windowHandle = win->GetSafeHwnd();
		if (!d_content || !d_border || !d_text)
		{
			d_renderer->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &d_content);
			d_renderer->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &d_border);
			d_renderer->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &d_text);
		}
		initialized = d_content && d_border;
		RECT area;
		UINT boundResult = pDC->GetBoundsRect(&area, 0);
		::GetClientRect(WindowFromDC(pDC->m_hDC), &area);
		HRESULT hr = d_renderer->BindDC(pDC->GetSafeHdc(), &area);
		bound = SUCCEEDED(hr);
	}


	if (!bound)
		return;

	// MFC-based procedure to retrieve our data
	CDraw3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc || !pDC)
		return;

	// Get the data
	CArray<DrawElement >* points = pDoc->GetPoints();
	if (!points)
		return;

	d_renderer->BeginDraw();
	drawClear = false;
	d_renderer->Clear(D2D1::ColorF(D2D1::ColorF::White));

	for (UINT c = 0; c < points->GetCount(); c++)
	{
		DrawElement p = points->ElementAt(c); // Retrieve the current Point

		d_border->SetColor(p.c1.GetColor());  // The solid color brush for borders
		d_content->SetColor(p.c2.GetColor()); // The solid color brush for content

		// Do this here since we'll be using the RECT for both Rectangles and ellipses
		D2D1_RECT_F location;
		location.bottom = p.point.y + p.size / 2;
		location.top = p.point.y - p.size / 2;
		location.left = p.point.x - p.size / 2;
		location.right = p.point.x + p.size / 2;
		D2D1_ELLIPSE ell;
		ell.point.x = p.point.x;
		ell.point.y = p.point.y;
		ell.radiusX = ell.radiusY = location.right - location.left;

		switch (p.sh)
		{
		case star:
			// Draw them in Crosses, otherwise, they'll be too small to see
			d_renderer->DrawLine(D2D1::Point2F(p.point.x - 1, p.point.y),
				D2D1::Point2F(p.point.x + 1, p.point.y), d_content, p.thickness);
			d_renderer->DrawLine(D2D1::Point2F(p.point.x, p.point.y - 1),
				D2D1::Point2F(p.point.x, p.point.y + 1), d_content, p.thickness);
			break;
		case square:
			// Fill in the Rectangle
			d_renderer->FillRectangle(location, d_content);
			d_renderer->DrawRectangle(location, d_border, p.thickness);
			break;
		case circle:

			// Ellipse will draw ellipses. It draws both content and border at the same time, so 
			// Prepare both prior to draw
			d_renderer->FillEllipse(ell, d_content);
			d_renderer->DrawEllipse(ell, d_border, p.thickness);
			break;
		}
	}


	// Text Specific Code
	CArray<DrawTextElement>* tPoints = pDoc->GetText();
	if (!tPoints)
		return;
	for (UINT rust = 0; rust < tPoints->GetCount(); rust++)
	{
		DrawTextElement* curTextEle = &tPoints->ElementAt(rust);

		// Make sure we have an object and that object has an active format object
		if (curTextEle && curTextEle->format)
		{
			D2D1_RECT_F location;
			location.bottom = curTextEle->location.bottom;
			location.left = curTextEle->location.left;
			location.right = curTextEle->location.right;
			location.top = curTextEle->location.top;

			d_text->SetColor(curTextEle->textColor.GetColor());
			
			d_renderer->DrawTextW(curTextEle->text.GetBuffer(),
				curTextEle->text.GetLength(),
				curTextEle->format,
				location,
				d_text);

		}
	}


	// End Text-Specific Code
	HRESULT hr = d_renderer->EndDraw();
	drawClear = true;
	if (FAILED(hr))
	{
		int er = GetLastError();
		int e = 3;
	}

	// TODO: add draw code for native data here
}


// CDraw3View printing


void CDraw3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDraw3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDraw3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDraw3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDraw3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDraw3View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Since we use the active category in the ribbon to decide
	// Whether we are adding an object or setting up a text object

	// Regardless, tell the app that we aren't setting a text element up. That way,
	// if we are, we can reset it after this method
	currentTextElement = nullptr;

	// Get the Window (should do null check)
	CMainFrame* window = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	// Get the ribbon from the window
	auto ribbon = window->GetRibbon();

	// Get the category, which will determine the mode our click will go in
	auto cat = ribbon->GetActiveCategory();

	CString category(cat->GetName());
	CDraw3Doc* drawDoc = GetDocument();

	// Should not be null but just to make sure
	if (!drawDoc)
		return;

	if (!category.CompareNoCase(L"Shapes"))
	{
		// Get list of points from Document
		CArray<DrawElement >* points = drawDoc->GetPoints();
		if (!points)
			return;
		DrawElement  p;

		// Set the Attributes of the DrawElement  structure, by getting the current
		// One from the Document
		p.c1.SetColor(drawDoc->GetCurrentColor());
		p.c2.SetColor(drawDoc->GetCurrentInnerColor());
		p.sh = drawDoc->GetCurrentShape();
		p.size = drawDoc->GetCurrentSize();
		p.thickness = drawDoc->GetCurrentThickness();

		// Set the points
		p.point = point;

		// Add the point and color
		points->Add(p);
	}
	else
	{
		// Get list of points from Document
		CArray<DrawTextElement >* points = drawDoc->GetText();
		if (!points)
			return;
		DrawTextElement  p;

		p.font = drawDoc->GetCurrentFont();
		p.locale = drawDoc->GetCurrentLocale();
		p.location.top = point.y;
		p.location.left = point.x;
		p.location.bottom = p.location.top + drawDoc->GetDimensions().y;
		p.location.right = p.location.left + drawDoc->GetDimensions().x;

		p.size = drawDoc->GetCurrentFontSize();
		p.textColor.SetColor(drawDoc->GetCurrentTextColor());

		p.format = nullptr;
		if (dw_factory)
			dw_factory->CreateTextFormat(
				p.font,
				nullptr,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				static_cast<float>(p.size),
				p.locale,
				&p.format);

		points->Add(p);

		currentTextElement = &points->ElementAt(points->GetCount() - 1);
	}

	drawDoc->UpdateAllViews(nullptr);
}

void CDraw3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CDraw3View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (currentTextElement)
	{
		for (UINT Rust = 0; Rust < nRepCnt; Rust++)
			currentTextElement->text.AppendChar(static_cast<WCHAR>(nChar));
	}
	CDraw3Doc* drawDoc = GetDocument();

	// Should not be null but just to make sure
	if (!drawDoc)
		return;
	drawDoc->UpdateAllViews(nullptr);
}


// CDraw3View diagnostics

#ifdef _DEBUG
void CDraw3View::AssertValid() const
{
	CView::AssertValid();
}

void CDraw3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDraw3Doc* CDraw3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDraw3Doc)));
	return (CDraw3Doc*)m_pDocument;
}
#endif //_DEBUG


// CDraw3View message handlers
