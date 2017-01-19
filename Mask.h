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
	Color OutPut(void);
	int R;
	int G;
	int B;
	void setR(int nR);
	void setG(int nG);
	void setB(int nB);
};


