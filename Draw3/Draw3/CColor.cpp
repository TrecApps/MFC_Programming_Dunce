#include "pch.h"
#include "CColor.h"


IMPLEMENT_SERIAL(CColor, CObject, 110);  // Needed to support serialization

CColor::CColor()
{
	color = D2D1::ColorF(D2D1::ColorF::White);                // Start with Black color
}

CColor::CColor(const CColor& c)
{
	color = c.color;
}


CColor::~CColor()
{
}

void CColor::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) // We are Saving the file
	{
		ar << color.r << color.g << color.b;      // Write the colors to file
	}
	else // We are loading the file
	{
		ar >> color.r >> color.g >> color.b;      // Read colors from file
		color.a = 1.0f;
	}
}

void CColor::SetColor(D2D1_COLOR_F c)
{
	color = c;
}

void CColor::SetColor(COLORREF c)
{
	color.r = static_cast<float>(GetRValue(c)) / 255.0f;
	color.g = static_cast<float>(GetGValue(c)) / 255.0f;
	color.b = static_cast<float>(GetBValue(c)) / 255.0f;
}

D2D1_COLOR_F CColor::GetColor()
{
	return color;
}

COLORREF CColor::GetColorInRGB()
{
	return RGB(static_cast<BYTE>(color.r * 255.0f),
		static_cast<BYTE>(color.g * 255.0f),
		static_cast<BYTE>(color.b * 255.0f));
}

void CColor::operator=(const CColor& c)
{
	color = c.color;
}
