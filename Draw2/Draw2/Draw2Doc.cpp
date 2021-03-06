
// Draw2Doc.cpp : implementation of the CDraw2Doc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Draw2.h"
#endif

#include "Draw2Doc.h"

#include "DrawDialog.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDraw2Doc

IMPLEMENT_DYNCREATE(CDraw2Doc, CDocument)

BEGIN_MESSAGE_MAP(CDraw2Doc, CDocument)
	ON_COMMAND(ID_EDIT_SETATTRIBUTES, &CDraw2Doc::OnEditSetattributes)
END_MESSAGE_MAP()


// CDraw2Doc construction/destruction

CDraw2Doc::CDraw2Doc()
{
	// TODO: add one-time construction code here
	currentShape = square;
	color = D2D1::ColorF(D2D1::ColorF::White);
	in_color = D2D1::ColorF(200.0/255.0, 0, 0);
	currentThickness = 1;
	currentSize = 5;
}

CDraw2Doc::~CDraw2Doc()
{
}

BOOL CDraw2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CDraw2Doc serialization

void CDraw2Doc::Serialize(CArchive& ar)
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
void CDraw2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CDraw2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CDraw2Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDraw2Doc diagnostics

#ifdef _DEBUG
void CDraw2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDraw2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
CArray<DrawElement>* const CDraw2Doc::GetPoints()
{
	return &points;
}
D2D1_COLOR_F CDraw2Doc::GetCurrentColor()
{
	return color;
}
D2D1_COLOR_F CDraw2Doc::GetCurrentInnerColor()
{
	return in_color;
}
shape CDraw2Doc::GetCurrentShape()
{
	return currentShape;
}
int CDraw2Doc::GetCurrentSize()
{
	return currentSize;
}
int CDraw2Doc::GetCurrentThickness()
{
	return currentThickness;
}
#endif //_DEBUG


// CDraw2Doc commands


void CDraw2Doc::OnEditSetattributes()
{
	// TODO: Add your command handler code here

	DrawDialog dd(currentShape,&color,&in_color,currentThickness,currentSize);

	// Call DoModal on the dialog. If Okay is returned, the user want to update the attributes

	if (dd.DoModal() == IDOK)
	{
		currentShape = dd.GetShape();

		currentThickness = dd.GetThickness();
		currentSize = dd.GetSize();
	}
}
