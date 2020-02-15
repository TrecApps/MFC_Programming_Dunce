#pragma once
#include "stdafx.h"
#include "Draw2Doc.h"
#include <d2d1.h>
class DrawDialog : public CDialogEx
{
public:
	DrawDialog(shape s, D2D1_COLOR_F* bC, D2D1_COLOR_F* cC, float t, float si);
	~DrawDialog();

	D2D1_COLOR_F* GetBorderColor();
	D2D1_COLOR_F* GetContentColor();
	float GetThickness();
	float GetSize();
	shape GetShape();

	virtual BOOL OnInitDialog() override;

	INT_PTR DoModal() override;
private:
	D2D1_COLOR_F * bColor, *cColor;
	float thickness, size;
	shape sh;
	CComboBox* typeBox;
	CEdit* thicknessBox;
	CEdit* sizeBox;
	CString numbers;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBorderColor();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOkay();
};

