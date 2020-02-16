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

// Draw3View.h : interface of the CDraw3View class
//

#pragma once


class CDraw3View : public CView
{
protected: // create from serialization only
	CDraw3View() noexcept;
	DECLARE_DYNCREATE(CDraw3View)

// Attributes
public:
	CDraw3Doc* GetDocument() const;

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
	virtual ~CDraw3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// Resources used in Draw2
	ID2D1Factory* d_factory;
	ID2D1DCRenderTarget* d_renderer;
	ID2D1SolidColorBrush* d_border, * d_content;

	bool initialized, bound, drawClear;
	HWND windowHandle;

	// Writing specific resources
	IDWriteFactory* dw_factory;
	ID2D1SolidColorBrush* d_text;
	DrawTextElement* currentTextElement;


// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Draw3View.cpp
inline CDraw3Doc* CDraw3View::GetDocument() const
   { return reinterpret_cast<CDraw3Doc*>(m_pDocument); }
#endif

