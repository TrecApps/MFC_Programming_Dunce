#pragma once
#include "stdafx.h"           // Gives you access to MFC Resources

class CColor : public CObject // Serialization only through 
{
	friend class CColor;      // Makes copying easier
	DECLARE_SERIAL(CColor);   // Needed to Support serialization
public:
	CColor();
	CColor(const CColor& c);  // Copy Constructor
	~CColor();

	virtual void Serialize(CArchive& ar) override; // Make sure our serialization method is called
	void SetColor(COLORREF c);
	COLORREF  GetColor();

	void operator=(const CColor& c);

private:
	COLORREF color;           // The underlying color
};

