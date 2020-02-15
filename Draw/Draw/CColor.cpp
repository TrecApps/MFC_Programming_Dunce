#include "stdafx.h"
#include "CColor.h"

IMPLEMENT_SERIAL(CColor, CObject, 100);  // Needed to support serialization

CColor::CColor()
{
	color = RGB(0, 0, 0);                // Start with Black color
}

CColor::CColor(const CColor & c)
{
	color = c.color;
}


CColor::~CColor()
{
}

void CColor::Serialize(CArchive & ar)
{
	if (ar.IsStoring()) // We are Saving the file
	{
		BYTE red = GetRValue(color);     // Get individual Color components
		BYTE green = GetGValue(color);
		BYTE blue = GetBValue(color);
		ar << red << green << blue;      // Write the colors to file
	}
	else // We are loading the file
	{
		BYTE red = 0, green = 0, blue = 0;
		ar >> red >> green >> blue;      // Read colors from file
		color = RGB(red, green, blue);   // Set colors
	}
}

void CColor::SetColor(COLORREF c)
{
	color = c;
}

COLORREF CColor::GetColor()
{
	return color;
}

void CColor::operator=(const CColor & c)
{
	color = c.color;
}
