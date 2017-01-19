// Mask.cpp : implementation file
//

#include "stdafx.h"
#include "Filtering.h"
#include "Mask.h"


// Mask

IMPLEMENT_DYNAMIC(Mask, CWnd)

Mask::Mask()
: R(0)
, G(0)
, B(0)
{

}

Mask::~Mask()
{
}


BEGIN_MESSAGE_MAP(Mask, CWnd)
END_MESSAGE_MAP()



// Mask message handlers




Color Mask::OutPut(void)
{
	Color a;
	a.SetFromCOLORREF(RGB(R,G,B));
	return a;
}


void Mask::setR(int nR)
{
	R = nR;
	R = min(255,max(0,R));
}


void Mask::setG(int nG)
{
	G = nG;
	G = min(255,max(0,G));
}


void Mask::setB(int nB)
{
	B = nB;
	B = min(255,max(0,B));
}
