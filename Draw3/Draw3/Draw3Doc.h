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

// Draw3Doc.h : interface of the CDraw3Doc class
//


#pragma once

#include "CColor.h"
#include <dwrite.h>
#include <atltypes.h>
#include "MainFrm.h"
typedef enum shape
{
	star,     // From previous Project
	square,
	circle
}shape;

typedef struct DrawElement
{
	CPoint point;
	CColor c1, c2;
	shape sh;
	int size;
	int thickness;
	
}DrawElement; // Replaces PointColor from previous Project

typedef struct DrawTextElement
{
	CRect location;
	CColor textColor;
	CString text;
	int size;
	CString locale, font;
	IDWriteTextFormat* format;
}DrawTextElement;


class CDraw3Doc : public CDocument
{
protected: // create from serialization only
	CDraw3Doc() noexcept;
	DECLARE_DYNCREATE(CDraw3Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CDraw3Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Methods introduced in Draw2
	CArray<DrawElement>* const GetPoints();
	D2D1_COLOR_F GetCurrentColor();
	D2D1_COLOR_F GetCurrentInnerColor();
	shape GetCurrentShape();
	int GetCurrentSize();
	int GetCurrentThickness();

	// Text Specific Methods
	CArray<DrawTextElement>* const GetText();
	D2D1_COLOR_F GetCurrentTextColor();
	int GetCurrentFontSize();
	CString GetCurrentFont();
	CString GetCurrentLocale();
	CPoint GetDimensions();

protected:
	// Attributes drom Draw2
	CArray<DrawElement> points;
	D2D1_COLOR_F color, in_color;
	shape currentShape;
	int currentSize;
	int currentThickness;

	// Text Specific Attributes
	CArray<DrawTextElement> text;
	D2D1_COLOR_F text_color;
	int fontSize;
	CString font, locale;
	CPoint textSize;

	// Access to the main Window
	CMainFrame* window;
	void InitializeWindow();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnButtonStar();
	afx_msg void OnButtonSquare();
	afx_msg void OnButton4();
	afx_msg void OnSpinSize();
	afx_msg void OnSpinThickness();
	afx_msg void OnButtonInnerColor();
	afx_msg void OnButtonOuterColor();
	afx_msg void OnComboFont();
	afx_msg void OnComboFontSize();
	afx_msg void OnButtonFontColor();
	afx_msg void OnSpinTextWidth();
	afx_msg void OnSpinTextHeight();
};

D2D1_COLOR_F Get2DColorFromMFCColor(COLORREF c);