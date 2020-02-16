#pragma once
#include <afx.h>
#include <d2d1.h>
class CColor :	public CObject
{
	friend class CColor;      // Makes copying easier
	DECLARE_SERIAL(CColor);   // Needed to Support serialization
public:
	CColor();
	CColor(const CColor& c);  // Copy Constructor
	~CColor();

	virtual void Serialize(CArchive& ar) override; // Make sure our serialization method is called
	void SetColor(D2D1_COLOR_F c);
	void SetColor(COLORREF c);
	D2D1_COLOR_F  GetColor();
	COLORREF GetColorInRGB();

	void operator=(const CColor& c);

private:
	D2D1_COLOR_F color;           // The underlying color
};

