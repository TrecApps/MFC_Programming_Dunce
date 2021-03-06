
// DrawView.cpp : implementation of the CDrawView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Draw.h"
#endif

#include "DrawDoc.h"
#include "DrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawView

IMPLEMENT_DYNCREATE(CDrawView, CView)

BEGIN_MESSAGE_MAP(CDrawView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDrawView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CDrawView construction/destruction

CDrawView::CDrawView()
{
	// TODO: add construction code here

}

CDrawView::~CDrawView()
{
}

BOOL CDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDrawView drawing

void CDrawView::OnDraw(CDC* pDC)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc || !pDC)
		return;

	// TODO: add draw code for native data here

	CArray<PointColor>* points = pDoc->GetPoints();
	if (!points)
		return;

	for (UINT c = 0; c < points->GetCount(); c++)
	{
		PointColor p = points->ElementAt(c); // Retrieve the current Point

		// Draw them in Crosses, otherwise, they'll be too small to see
		if(p.point.x > 0)
			pDC->SetPixel(p.point.x-1, p.point.y, p.color.GetColor());
		if(p.point.y > 0)
			pDC->SetPixel(p.point.x, p.point.y-1, p.color.GetColor());
		pDC->SetPixel(p.point.x, p.point.y, p.color.GetColor());
		pDC->SetPixel(p.point.x, p.point.y + 1, p.color.GetColor());
		pDC->SetPixel(p.point.x + 1, p.point.y, p.color.GetColor());
	}
}


// CDrawView printing


void CDrawView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDrawView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDrawView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CDrawView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDrawDoc* drawDoc = GetDocument();

	// Should not be null but just to make sure
	if (!drawDoc) 
		return;

	// Get list of points from Document
	CArray<PointColor>* points = drawDoc->GetPoints();
	if (!points)
		return;
	PointColor p;

	// Set the Color of the PointColor structure, by getting the current
	// One from the Document
	p.color.SetColor(drawDoc->GetCurrentColor());

	// Set the points
	p.point = point;

	// Add the point and color
	points->Add(p);
	CView::OnLButtonDown(nFlags, point);
	drawDoc->UpdateAllViews(nullptr);
}


// CDrawView diagnostics

#ifdef _DEBUG
void CDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDrawDoc* CDrawView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawDoc)));
	return (CDrawDoc*)m_pDocument;
}
#endif //_DEBUG


// CDrawView message handlers
