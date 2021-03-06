
// Draw2View.cpp : implementation of the CDraw2View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Draw2.h"
#endif

#include "Draw2Doc.h"
#include "Draw2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDraw2View

IMPLEMENT_DYNCREATE(CDraw2View, CView)

BEGIN_MESSAGE_MAP(CDraw2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CDraw2View construction/destruction

CDraw2View::CDraw2View()
{
	// TODO: add construction code here
	d_factory = nullptr;
	d_renderer = nullptr;
	d_border = d_content = nullptr;
	windowHandle = 0;

	initialized = bound = false;

	D2D1_FACTORY_OPTIONS ops;

	ops.debugLevel = D2D1_DEBUG_LEVEL_WARNING;

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, ops,&d_factory);
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

	initialized = drawClear = true;
}

CDraw2View::~CDraw2View()
{
	if (d_border)
		d_border->Release();
	if (d_content)
		d_content->Release();
	if (d_renderer)
		d_renderer->Release();
	if (d_factory)
		d_factory->Release();
	d_factory = nullptr;
	d_renderer = nullptr;
	d_border = d_content = nullptr;
}

BOOL CDraw2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDraw2View drawing

void CDraw2View::OnDraw(CDC* pDC)
{
	if (!initialized || !drawClear)
		return;

	// Bind the DC Render Target to our Context Handle if not already done

		CWnd* win = AfxGetMainWnd();
		if (win)
		{
			windowHandle = win->GetSafeHwnd();
			if (!d_content || !d_border)
			{
				d_renderer->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &d_content);
				d_renderer->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &d_border);
			}
			initialized = d_content && d_border;

			// Get the Area of the window and bind it to the Render Target
			RECT area;
			UINT boundResult = pDC->GetBoundsRect(&area, 0);
			::GetClientRect(WindowFromDC(pDC->m_hDC), &area);
			HRESULT hr = d_renderer->BindDC(pDC->GetSafeHdc(), &area);

			// Assign bool to ensure that if this fails, we don't proceed
			bound = SUCCEEDED(hr);
		}


	if (!bound)
		return;

	// MFC-based procedure to retrieve our data
	CDraw2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc || !pDC)
		return;

	// Get the data
	CArray<DrawElement >* points = pDoc->GetPoints();
	if (!points)
		return;

	// All Draw operations must begin with a call to BeginDraw()
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
				D2D1::Point2F(p.point.x + 1, p.point.y), d_content ,p.thickness);
			d_renderer->DrawLine(D2D1::Point2F(p.point.x, p.point.y - 1),
				D2D1::Point2F(p.point.x, p.point.y + 1), d_content, p.thickness);
			break;
		case square:
			// Fill in the Rectangle
			d_renderer->FillRectangle(location, d_content);
			d_renderer->DrawRectangle(location, d_border,p.thickness);
			break;
		case circle:

			// Ellipse will draw ellipses. It draws both content and border at the same time, so 
			// Prepare both prior to draw
			d_renderer->FillEllipse(ell, d_content);
			d_renderer->DrawEllipse(ell, d_border, p.thickness);
			break;
		}
	}
	// Done Drawing, time to call EndDraw so that it shows
	HRESULT hr = d_renderer->EndDraw();
	drawClear = true;
	if (FAILED(hr))
	{
		int er = GetLastError();
		int e = 3;
	}
}


// CDraw2View printing

BOOL CDraw2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDraw2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDraw2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CDraw2View diagnostics

#ifdef _DEBUG
void CDraw2View::AssertValid() const
{
	CView::AssertValid();
}

void CDraw2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

void CDraw2View::OnLButtonUp(UINT nFlags, CPoint point)
{

	//CView::OnLButtonUp(nFlags, point);
	CDraw2Doc* drawDoc = GetDocument();

	// Should not be null but just to make sure
	if (!drawDoc)
		return;
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
	

	// Ensures that the updated data will be drawn immediately
	drawDoc->UpdateAllViews(nullptr);

}

CDraw2Doc* CDraw2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDraw2Doc)));
	return (CDraw2Doc*)m_pDocument;
}
#endif //_DEBUG


// CDraw2View message handlers
