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

// Draw3Doc.cpp : implementation of the CDraw3Doc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Draw3.h"
#endif

#include "Draw3Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDraw3Doc

IMPLEMENT_DYNCREATE(CDraw3Doc, CDocument)

BEGIN_MESSAGE_MAP(CDraw3Doc, CDocument)
	ON_COMMAND(ID_BUTTON2, &CDraw3Doc::OnButtonStar)
	ON_COMMAND(ID_BUTTON3, &CDraw3Doc::OnButtonSquare)
	ON_COMMAND(ID_BUTTON4, &CDraw3Doc::OnButton4)
	ON_COMMAND(ID_SPIN5, &CDraw3Doc::OnSpinSize)
	ON_COMMAND(ID_SPIN6, &CDraw3Doc::OnSpinThickness)
	ON_COMMAND(ID_COMBO3, &CDraw3Doc::OnComboFont)
	ON_COMMAND(ID_COMBO4, &CDraw3Doc::OnComboFontSize)
	ON_COMMAND(ID_BUTTON7, &CDraw3Doc::OnButtonFontColor)
	ON_COMMAND(ID_SPIN3, &CDraw3Doc::OnSpinTextWidth)
	ON_COMMAND(ID_SPIN4, &CDraw3Doc::OnSpinTextHeight)
	ON_COMMAND(ID_BUTTON8, &CDraw3Doc::OnButtonInnerColor)
	ON_COMMAND(ID_BUTTON9, &CDraw3Doc::OnButtonOuterColor)
END_MESSAGE_MAP()


// CDraw3Doc construction/destruction

CDraw3Doc::CDraw3Doc() noexcept
{
	// TODO: add one-time construction code here
	window = nullptr;

	currentShape = square;
	color = D2D1::ColorF(D2D1::ColorF::White);
	in_color = D2D1::ColorF(200.0 / 255.0, 0, 0);
	currentThickness = 1;
	currentSize = 5;

	font = L"Ariel";
	locale = L"en-us";
	fontSize = 12;
	text_color = D2D1::ColorF(D2D1::ColorF::Black);
	textSize.x = textSize.y = 100;

}

CDraw3Doc::~CDraw3Doc()
{
}

BOOL CDraw3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CDraw3Doc serialization

void CDraw3Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CDraw3Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CDraw3Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}



void CDraw3Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS


void CDraw3Doc::InitializeWindow()
{
	if(!window)
		window = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
}
// CDraw3Doc diagnostics

#ifdef _DEBUG
void CDraw3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDraw3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


CArray<DrawElement>* const CDraw3Doc::GetPoints()
{
	return &points;
}
CArray<DrawTextElement>* const CDraw3Doc::GetText()
{
	return &text;
}
D2D1_COLOR_F CDraw3Doc::GetCurrentTextColor()
{
	return text_color;
}
int CDraw3Doc::GetCurrentFontSize()
{
	return fontSize;
}
CString CDraw3Doc::GetCurrentFont()
{
	return font;
}
CString CDraw3Doc::GetCurrentLocale()
{
	return locale;
}
CPoint CDraw3Doc::GetDimensions()
{
	return textSize;
}
D2D1_COLOR_F CDraw3Doc::GetCurrentColor()
{
	return color;
}
D2D1_COLOR_F CDraw3Doc::GetCurrentInnerColor()
{
	return in_color;
}
shape CDraw3Doc::GetCurrentShape()
{
	return currentShape;
}
int CDraw3Doc::GetCurrentSize()
{
	return currentSize;
}
int CDraw3Doc::GetCurrentThickness()
{
	return currentThickness;
}

// CDraw3Doc commands


void CDraw3Doc::OnButtonStar()
{
	InitializeWindow();

	currentShape = star;
}


void CDraw3Doc::OnButtonSquare()
{
	InitializeWindow();

	currentShape = square;
}


void CDraw3Doc::OnButton4()
{
	InitializeWindow();

	currentShape = circle;
}


void CDraw3Doc::OnSpinSize()
{
	InitializeWindow();

	if (!window)
		return;

	auto ribbon = window->GetRibbon();

	auto cat = ribbon->GetActiveCategory();

	auto pan = cat->GetPanel(1);

	auto button = pan->GetElement(0);

	auto edit = dynamic_cast<CMFCRibbonEdit*>(button);

	if (!edit)
		return;

	CString num(edit->GetEditText());

	currentSize = _wtoi(num);

}


void CDraw3Doc::OnSpinThickness()
{
	InitializeWindow();

	if (!window)
		return;

	auto ribbon = window->GetRibbon();

	auto cat = ribbon->GetActiveCategory();

	auto pan = cat->GetPanel(1);

	auto button = pan->GetElement(1);

	auto edit = dynamic_cast<CMFCRibbonEdit*>(button);

	if (!edit)
		return;

	CString num(edit->GetEditText());

	currentThickness = _wtoi(num);
}





void CDraw3Doc::OnComboFont()
{
	InitializeWindow();

	if (!window)
		return;

	auto ribbon = window->GetRibbon();

	auto cat = ribbon->GetActiveCategory();

	auto pan = cat->GetPanel(0);

	auto button = pan->GetElement(0);

	auto edit = dynamic_cast<CMFCRibbonComboBox*>(button);

	if (!edit)
		return;

	font = edit->GetItem(edit->GetCurSel());
}


void CDraw3Doc::OnComboFontSize()
{
	InitializeWindow();

	if (!window)
		return;

	auto ribbon = window->GetRibbon();

	auto cat = ribbon->GetActiveCategory();

	auto pan = cat->GetPanel(0);

	auto button = pan->GetElement(1);

	auto edit = dynamic_cast<CMFCRibbonComboBox*>(button);

	if (!edit)
		return;

	fontSize = _wtoi(edit->GetItem(edit->GetCurSel()));
}


void CDraw3Doc::OnButtonFontColor()
{
	InitializeWindow();

	if (!window)
		return;

	auto ribbon = window->GetRibbon();

	auto cat = ribbon->GetActiveCategory();

	auto pan = cat->GetPanel(0);

	auto button = pan->GetElement(2);

	auto edit = dynamic_cast<CMFCRibbonColorButton*>(button);

	if (!edit)
		return;

	text_color = Get2DColorFromMFCColor(edit->GetColor());

}


void CDraw3Doc::OnSpinTextWidth()
{
	InitializeWindow();

	if (!window)
		return;

	auto ribbon = window->GetRibbon();

	auto cat = ribbon->GetActiveCategory();

	auto pan = cat->GetPanel(1);

	auto button = pan->GetElement(0);

	auto edit = dynamic_cast<CMFCRibbonEdit*>(button);

	if (!edit)
		return;

	textSize.x = _wtoi(edit->GetEditText());
}


void CDraw3Doc::OnSpinTextHeight()
{
	InitializeWindow();

	if (!window)
		return;

	auto ribbon = window->GetRibbon();

	auto cat = ribbon->GetActiveCategory();

	auto pan = cat->GetPanel(1);

	auto button = pan->GetElement(1);

	auto edit = dynamic_cast<CMFCRibbonEdit*>(button);

	if (!edit)
		return;

	textSize.y = _wtoi(edit->GetEditText());
}

D2D1_COLOR_F Get2DColorFromMFCColor(COLORREF c)
{
	return D2D1_COLOR_F{
		static_cast<float>(GetRValue(c)) / 255.0f,
		static_cast<float>(GetGValue(c)) / 255.0f,
		static_cast<float>(GetBValue(c)) / 255.0f,
		1.0f};
}


void CDraw3Doc::OnButtonInnerColor()
{
	InitializeWindow();

	if (!window)
		return;

	auto ribbon = window->GetRibbon();

	auto cat = ribbon->GetActiveCategory();

	auto pan = cat->GetPanel(2);

	auto button = pan->GetElement(0);

	auto edit = dynamic_cast<CMFCRibbonColorButton*>(button);

	if (!edit)
		return;

	in_color = Get2DColorFromMFCColor(edit->GetColor());
}


void CDraw3Doc::OnButtonOuterColor()
{
	InitializeWindow();

	if (!window)
		return;

	auto ribbon = window->GetRibbon();

	auto cat = ribbon->GetActiveCategory();

	auto pan = cat->GetPanel(2);

	auto button = pan->GetElement(1);

	auto edit = dynamic_cast<CMFCRibbonColorButton*>(button);

	if (!edit)
		return;

	color = Get2DColorFromMFCColor(edit->GetColor());
}
