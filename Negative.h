#pragma once


// Mask

class Mask : public CWnd
{
	DECLARE_DYNAMIC(Mask)

public:
	Mask();
	virtual ~Mask();

protected:
	DECLARE_MESSAGE_MAP()
public:
	int R;
};


