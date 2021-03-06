
// Draw2View.h : interface of the CDraw2View class
//
#include <d2d1.h>
#pragma once


class CDraw2View : public CView
{
protected: // create from serialization only
	CDraw2View();
	DECLARE_DYNCREATE(CDraw2View)

// Attributes
public:
	CDraw2Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CDraw2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	ID2D1Factory * d_factory;
	ID2D1DCRenderTarget* d_renderer;
	ID2D1SolidColorBrush* d_border, *d_content;

	bool initialized, bound, drawClear;
	HWND windowHandle;

// Generated message map functions
protected:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Draw2View.cpp
inline CDraw2Doc* CDraw2View::GetDocument() const
   { return reinterpret_cast<CDraw2Doc*>(m_pDocument); }
#endif

