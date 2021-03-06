
// Draw2Doc.h : interface of the CDraw2Doc class
//


#pragma once
#include "CColor.h"
#include <d2d1.h>
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

class CDraw2Doc : public CDocument
{
protected: // create from serialization only
	CDraw2Doc();
	DECLARE_DYNCREATE(CDraw2Doc)

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
	virtual ~CDraw2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CArray<DrawElement>* const GetPoints();
	D2D1_COLOR_F GetCurrentColor();
	D2D1_COLOR_F GetCurrentInnerColor();
	shape GetCurrentShape();
	int GetCurrentSize();
	int GetCurrentThickness();

protected:
	CArray<DrawElement> points;
	D2D1_COLOR_F color, in_color;
	shape currentShape;
	int currentSize;
	int currentThickness;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:
	afx_msg void OnEditSetattributes();
};
