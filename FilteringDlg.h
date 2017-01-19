
// FilteringDlg.h : header file
//

#pragma once
#include "Mask.h"


// CFilteringDlg dialog
class CFilteringDlg : public CDialogEx
{
// Construction
public:
	CFilteringDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FILTERING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoad();
	CString m_strPathName;
	int m_intRadioSelect;
	afx_msg void OnBnClickedRadioFilter();
	afx_msg void OnBnClickedRadioEdge();
	afx_msg void OnBnClickedRadioSegmentation();
	afx_msg void OnBnClickedRadioColor();
	afx_msg void OnBnClickedButtonConversion();
	void showFilterFalse(void);
	void showFilterTrue(void);
	afx_msg void OnBnClickedButtonCustomfilter();
	CComboBox m_cbMenu;
	void ImageConversion(int Radio, int Combo);
	int m_Width;
	int m_Height;
	Color Negative(Color color);
	Color** pic;	
	int nR;
	int nG;
	int nB;
	Color GrayScale(Color color);
	Bitmap& SetMask(double mask[3][3], int k, Bitmap& filter);
	Color** fpic;
	void BubbleSort(int mask[], int n);
	afx_msg void OnBnClickedButtonApply();
	double m_Hist[256];
	Color Otsu(Color color);
	double** label;
	void GrassFire_8direc(double** value, int label, int width, int height, double** L);
	double** value;
};
