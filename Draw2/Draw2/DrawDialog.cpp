#include "stdafx.h"
#include "DrawDialog.h"
#include "resource.h"

DrawDialog::DrawDialog(shape s, D2D1_COLOR_F* bC, D2D1_COLOR_F* cC, float t, float si) :
	CDialogEx(IDD_FORMVIEW)
{
	sh = s;
	bColor = bC;
	cColor = cC;
	thickness = t;
	size = si;
}


DrawDialog::~DrawDialog()
{
}

D2D1_COLOR_F* DrawDialog::GetBorderColor()
{
	return bColor;
}

D2D1_COLOR_F* DrawDialog::GetContentColor()
{
	return cColor;
}

float DrawDialog::GetThickness()
{
	return thickness;
}

float DrawDialog::GetSize()
{
	return size;
}

shape DrawDialog::GetShape()
{
	return sh;
}

BOOL DrawDialog::OnInitDialog()
{
	if (!CDialogEx::OnInitDialog())
		return FALSE;
	typeBox = (CComboBox*)GetDlgItem(IDC_COMBO1);
	if (!typeBox)
		return FALSE;
	switch (sh)
	{
		// Use the SetTopIndex(int) to set the active text
	case star:
		typeBox->SetTopIndex(0);
		break;
	case square:
		typeBox->SetTopIndex(1);
		break;
	case circle:
		typeBox->SetTopIndex(2);
	}


	// Handling Edit Controls
	thicknessBox = (CEdit*)GetDlgItem(IDC_EDIT2);
	sizeBox = (CEdit*)GetDlgItem(IDC_EDIT1);
	if (!thicknessBox || !sizeBox)
		return FALSE;
	
	numbers.Format(L"%f", thickness);     // Can use to get the CString to resemble numbers in string form
	thicknessBox->SetWindowText(numbers); // Use this method (from CWnd) to set the text
	numbers.Format(L"%f", size);
	sizeBox->SetWindowText(numbers);
	return TRUE;
}

INT_PTR DrawDialog::DoModal()
{
	// Handling CComboBox

	// Use GetDlgItem(int) to get a specific control
	// Include "resources.h" to get access to the proper numbers
	// Use the Properties Panel (and mockup) to get the proper Macro for your control

	

	INT_PTR returnable = CDialogEx::DoModal();  // The dialog will run and freeze the parent window



	return returnable;
}
BEGIN_MESSAGE_MAP(DrawDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &DrawDialog::OnBnClickedBorderColor)
	ON_BN_CLICKED(IDC_BUTTON2, &DrawDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &DrawDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &DrawDialog::OnBnClickedOkay)
END_MESSAGE_MAP()


void DrawDialog::OnBnClickedBorderColor()
{
	CColorDialog col;
	if (col.DoModal() == IDOK)
	{
		COLORREF c = col.GetColor();
		bColor->a = 1.0f;
		bColor->b = static_cast<float>(GetBValue(c)) / 255.0f;
		bColor->r = static_cast<float>(GetRValue(c)) / 255.0f;
		bColor->g = static_cast<float>(GetGValue(c)) / 255.0f;
	}
}


void DrawDialog::OnBnClickedButton2()
{
	CColorDialog col;
	if (col.DoModal() == IDOK)
	{
		COLORREF c = col.GetColor();
		cColor->a = 1.0f;
		cColor->b = static_cast<float>(GetBValue(c)) / 255.0f;
		cColor->r = static_cast<float>(GetRValue(c)) / 255.0f;
		cColor->g = static_cast<float>(GetGValue(c)) / 255.0f;
	}
}

// Set the class where event is handle BEFORE renaming the function
// I just learned this the hard way

void DrawDialog::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


void DrawDialog::OnBnClickedOkay()
{
		// Use CComboBox::GetTopIndex() to get active index 
	typeBox->GetWindowTextW(numbers);

	if (!numbers.Compare(L"Star"))
		sh = star;
	else if (!numbers.Compare(L"Square"))
		sh = square;
	else
		sh = circle;

	// Use CWnd::GetWindowText(CString&) to get the font
	thicknessBox->GetWindowText(numbers);

	// These edit controls are configured to only accept text that resembles numbers
	thickness = _ttof(numbers);
	sizeBox->GetWindowText(numbers);
	size = _ttof(numbers);
	// Note Colors are handled by the Event Handlers so
	//	no need to worry about them

	// TODO: Add your control notification handler code here
	OnOK();
}
