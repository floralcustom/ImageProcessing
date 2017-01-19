
// FilteringDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Filtering.h"
#include "FilteringDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFilteringDlg dialog




CFilteringDlg::CFilteringDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilteringDlg::IDD, pParent)
	, m_strPathName(_T(""))
	, m_intRadioSelect(0)
	, m_Width(0)
	, m_Height(0)
	, pic(NULL)
	, nR(0)
	, nG(0)
	, nB(0)
	, fpic(NULL)
	, label(NULL)
	, value(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFilteringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MENU, m_cbMenu);
}

BEGIN_MESSAGE_MAP(CFilteringDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CFilteringDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_RADIO_FILTER, &CFilteringDlg::OnBnClickedRadioFilter)
	ON_BN_CLICKED(IDC_RADIO_Edge, &CFilteringDlg::OnBnClickedRadioEdge)
	ON_BN_CLICKED(IDC_RADIO_SEGMENTATION, &CFilteringDlg::OnBnClickedRadioSegmentation)
	ON_BN_CLICKED(IDC_RADIO_COLOR, &CFilteringDlg::OnBnClickedRadioColor)
	ON_BN_CLICKED(IDC_BUTTON_CONVERSION, &CFilteringDlg::OnBnClickedButtonConversion)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOMFILTER, &CFilteringDlg::OnBnClickedButtonCustomfilter)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CFilteringDlg::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CFilteringDlg message handlers

BOOL CFilteringDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFilteringDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFilteringDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFilteringDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFilteringDlg::OnBnClickedButtonLoad()
{
	// TODO: Add your control notification handler code here
	CClientDC dc(this);
	HDC hDC = dc.GetSafeHdc();
	Graphics G(hDC);

	CFileDialog Load(TRUE,NULL,NULL,OFN_FILEMUSTEXIST); // 이미지파일 로드오류 확인 
	if(IDOK == Load.DoModal())
	{
        CString strPathName = Load.GetPathName(); 
	}
	CFile file;
	file.Open(Load.GetPathName(), CFile::modeRead | CFile::typeBinary); // 이미지파일 로드
	m_strPathName = file.GetFilePath(); // 경로에따른 이미지를 넣어줌
	file.Close();

	Bitmap dlg(m_strPathName);

	m_Width = dlg.GetWidth();
	m_Height = dlg.GetHeight();

	pic = (Color**)calloc(dlg.GetWidth(),sizeof(Color*)); // 불러온 이미지에 대한 동적배열 할당
	fpic = (Color**)calloc(dlg.GetWidth(),sizeof(Color*)); // 변환된 이미지에 대한 동적배열 할당
	label = (double**)calloc(dlg.GetWidth(),sizeof(double*)); // 라벨링 이미지에 대한 동적배열 할당
	value = (double**)calloc(dlg.GetWidth(),sizeof(double*)); // 

	for(int i = 0; i < m_Width; i++)
	{
		pic[i] = (Color*)calloc(sizeof(Color),dlg.GetHeight());
		fpic[i] = (Color*)calloc(sizeof(Color),dlg.GetHeight());
		label[i] = (double*)calloc(sizeof(double),dlg.GetHeight());
		value[i] = (double*)calloc(sizeof(double),dlg.GetHeight());
	}


	//원본이미지 픽셀 추출
	for(int i = 0; i < m_Width; i++){
		for(int j = 0; j < m_Height; j++){
			dlg.GetPixel(i,j,&pic[i][j]); //픽셀 값을 추출
			label[i][j]=0;
			value[i][j]=0;
		}
	}

	G.DrawImage(&dlg,28,37,400,250); // 이미지 출력
}

void CFilteringDlg::OnBnClickedButtonConversion()
{
	// TODO: Add your control notification handler code here
	ImageConversion(m_intRadioSelect,m_cbMenu.GetCurSel());
}

void CFilteringDlg::OnBnClickedRadioFilter()
{
	// TODO: Add your control notification handler code here
	m_intRadioSelect = 0; // 필터 선택
	(GetDlgItem(IDC_BUTTON_CUSTOMFILTER))->ShowWindow(TRUE);
	(GetDlgItem(IDC_COMBO_MENU))->ShowWindow(TRUE);

	m_cbMenu.ResetContent();
	m_cbMenu.AddString(_T("Average"));
	m_cbMenu.AddString(_T("Gaussian"));
	m_cbMenu.AddString(_T("Median"));
	m_cbMenu.AddString(_T("Sharpn"));
	m_cbMenu.AddString(_T("Emboss"));
	m_cbMenu.SetCurSel(0);
}


void CFilteringDlg::OnBnClickedRadioEdge()
{
	// TODO: Add your control notification handler code here
	m_intRadioSelect = 1; //에지 선택
	showFilterFalse();
	(GetDlgItem(IDC_COMBO_MENU))->ShowWindow(TRUE);

	m_cbMenu.ResetContent();
	m_cbMenu.AddString(_T("Canny"));
	m_cbMenu.AddString(_T("Laplacian"));
	m_cbMenu.AddString(_T("Prewitt(Gx)"));
	m_cbMenu.AddString(_T("Prewitt(Gy)"));
	m_cbMenu.AddString(_T("Roberts(Gx)"));
	m_cbMenu.AddString(_T("Roberts(Gy)"));
	m_cbMenu.AddString(_T("Sobel(Gx)"));
	m_cbMenu.AddString(_T("Sobel(Gy)"));
	m_cbMenu.SetCurSel(0);
}


void CFilteringDlg::OnBnClickedRadioSegmentation()
{
	// TODO: Add your control notification handler code here
	m_intRadioSelect = 2; //분할 선택
	showFilterFalse();
	(GetDlgItem(IDC_COMBO_MENU))->ShowWindow(TRUE);

	m_cbMenu.ResetContent();
	m_cbMenu.AddString(_T("Binary-Otsu"));
	m_cbMenu.AddString(_T("Label-GrassFire"));
	m_cbMenu.SetCurSel(0);
}


void CFilteringDlg::OnBnClickedRadioColor()
{
	// TODO: Add your control notification handler code here
	m_intRadioSelect = 3; //컬러 선택
	showFilterFalse();
	(GetDlgItem(IDC_COMBO_MENU))->ShowWindow(TRUE);

	m_cbMenu.ResetContent();
	m_cbMenu.AddString(_T("Gray Scale"));
	m_cbMenu.AddString(_T("Negative"));
	m_cbMenu.SetCurSel(0);
}





void CFilteringDlg::showFilterFalse(void)
{
	(GetDlgItem(IDC_BUTTON_CUSTOMFILTER))->ShowWindow(FALSE);
	(GetDlgItem(IDC_STATIC_MASK))->ShowWindow(FALSE);
	(GetDlgItem(IDC_BUTTON_APPLY))->ShowWindow(FALSE);

	(GetDlgItem(IDC_EDIT_1))->ShowWindow(FALSE);
	(GetDlgItem(IDC_EDIT_2))->ShowWindow(FALSE);
	(GetDlgItem(IDC_EDIT_3))->ShowWindow(FALSE);
	(GetDlgItem(IDC_EDIT_4))->ShowWindow(FALSE);
	(GetDlgItem(IDC_EDIT_5))->ShowWindow(FALSE);
	(GetDlgItem(IDC_EDIT_6))->ShowWindow(FALSE);
	(GetDlgItem(IDC_EDIT_7))->ShowWindow(FALSE);
	(GetDlgItem(IDC_EDIT_8))->ShowWindow(FALSE);
	(GetDlgItem(IDC_EDIT_9))->ShowWindow(FALSE);
}


void CFilteringDlg::showFilterTrue(void)
{
	(GetDlgItem(IDC_STATIC_MASK))->ShowWindow(TRUE);
	(GetDlgItem(IDC_BUTTON_APPLY))->ShowWindow(TRUE);

	(GetDlgItem(IDC_EDIT_1))->ShowWindow(TRUE);
	(GetDlgItem(IDC_EDIT_2))->ShowWindow(TRUE);
	(GetDlgItem(IDC_EDIT_3))->ShowWindow(TRUE);
	(GetDlgItem(IDC_EDIT_4))->ShowWindow(TRUE);
	(GetDlgItem(IDC_EDIT_5))->ShowWindow(TRUE);
	(GetDlgItem(IDC_EDIT_6))->ShowWindow(TRUE);
	(GetDlgItem(IDC_EDIT_7))->ShowWindow(TRUE);
	(GetDlgItem(IDC_EDIT_8))->ShowWindow(TRUE);
	(GetDlgItem(IDC_EDIT_9))->ShowWindow(TRUE);
}


void CFilteringDlg::OnBnClickedButtonCustomfilter()
{
	// TODO: Add your control notification handler code here
	showFilterTrue();
}


void CFilteringDlg::ImageConversion(int Radio, int Combo)
{
	CClientDC dc(this);
	HDC hDC = dc.GetSafeHdc();
	Graphics G(hDC);

	if(Radio == 0 && Combo == 0) //Average filter
	{
		Bitmap pAverage(m_Width,m_Height);
		double  mask[3][3] = {1,1,1
							 ,1,1,1
							 ,1,1,1};

		G.DrawImage(&SetMask(mask,9,pAverage), 28, 330, 400, 250);
	}
	else if(Radio == 0 && Combo == 1) // Embossing filter
	{
		Bitmap pEmboss(m_Width,m_Height);
		double  mask[3][3] = {-1,0,0
							  ,0,0,0
							  ,0,0,1};

		G.DrawImage(&SetMask(mask,1,pEmboss), 28, 330, 400, 250);
	}
	else if(Radio == 0 && Combo == 2) // Gaussian filter
	{
		Bitmap pGaussian(m_Width,m_Height);
		double  mask[3][3] = {1,2,1
							 ,2,4,2
							 ,1,2,1};

		G.DrawImage(&SetMask(mask,16,pGaussian), 28, 330, 400, 250);
	}
	else if(Radio == 0 && Combo == 3) // Median filter
	{
		Bitmap pMedian(m_Width,m_Height);
		
		int r[9], g[9], b[9];

		for(int i = 1; i < m_Width-1; i++)
		{
			for(int j = 1; j < m_Height-1; j ++)
			{
				r[0] = pic[i-1][j-1].GetR();
				r[1] = pic[i-1][j].GetR();
				r[2] = pic[i-1][j+1].GetR();
				r[3] = pic[i][j-1].GetR();
				r[4] = pic[i][j].GetR();
				r[5] = pic[i][j+1].GetR();
				r[6] = pic[i+1][j-1].GetR();
				r[7] = pic[i+1][j].GetR();
				r[8] = pic[i+1][j+1].GetR();

				g[0] = pic[i-1][j-1].GetG();
				g[1] = pic[i-1][j].GetG();
				g[2] = pic[i-1][j+1].GetG();
				g[3] = pic[i][j-1].GetG();
				g[4] = pic[i][j].GetG();
				g[5] = pic[i][j+1].GetG();
				g[6] = pic[i+1][j-1].GetG();
				g[7] = pic[i+1][j].GetG();
				g[8] = pic[i+1][j+1].GetG();
			
				b[0] = pic[i-1][j-1].GetB();
				b[1] = pic[i-1][j].GetB();
				b[2] = pic[i-1][j+1].GetB();
				b[3] = pic[i][j-1].GetB();
				b[4] = pic[i][j].GetB();
				b[5] = pic[i][j+1].GetB();
				b[6] = pic[i+1][j-1].GetB();
				b[7] = pic[i+1][j].GetB();
				b[8] = pic[i+1][j+1].GetB();

				

				BubbleSort(r, 9);
				BubbleSort(g, 9);
				BubbleSort(b, 9);

				nR = r[5];
				nG = g[5];
				nB = b[5];

				nR = min(255,max(0,nR));
				nG = min(255,max(0,nG));
				nB = min(255,max(0,nB));

				fpic[i-1][j-1].SetFromCOLORREF(RGB(nR,nG,nB));
				pMedian.SetPixel(i-1,j-1,fpic[i-1][j-1]);
			}

		}

		G.DrawImage(&pMedian, 28, 330, 400, 250);
	}

	else if(Radio == 0 && Combo == 4) // Sharpning filter
	{
		Bitmap pSharpn(m_Width,m_Height);
		double  mask[3][3] = {0,-1,0
							 ,-1 ,5,-1
							 ,0,-1,0};

		G.DrawImage(&SetMask(mask,1,pSharpn), 28, 330, 400, 250);
	}

	else if(Radio == 1 && Combo == 0) // Canny
	{
		Bitmap pCanny(m_Width,m_Height);
		double  mask[3][3] = {1,2,1
							 ,2,4,2
							 ,1,2,1};
		SetMask(mask,16,pCanny); // 가우시안 필터 적용

		/*double  mask[3][3] = {-1,-2,-1
							 ,0,0,0
							 ,1,2,1};

		SetMask(mask,1,pCanny);*/


		G.DrawImage(&pCanny, 28, 330, 400, 250);
	}
	else if(Radio == 1 && Combo == 1) // Laplacian
	{
		Bitmap pLaplacian(m_Width,m_Height);
		double  mask[3][3] = {0,1,0
							 ,1,-4,1
							 ,0,1,0};

		G.DrawImage(&SetMask(mask,1,pLaplacian), 28, 330, 400, 250);
	}
	else if(Radio == 1 && Combo == 2) // Prewitt(Gx)
	{
		Bitmap pPrewitt(m_Width,m_Height);
		double  mask[3][3] = {-1,-1,-1
							 ,0,0,0
							 ,1,1,1};

		G.DrawImage(&SetMask(mask,1,pPrewitt), 28, 330, 400, 250);
	}
	else if(Radio == 1 && Combo == 3) // Prewitt(Gy)
	{
		Bitmap pPrewitt(m_Width,m_Height);
		double  mask[3][3] = {-1,0,1
							 ,-1,0,1
							 ,-1,0,1};

		G.DrawImage(&SetMask(mask,1,pPrewitt), 28, 330, 400, 250);
	}
	else if(Radio == 1 && Combo == 4) // Roberts(Gx)
	{
		Bitmap pRoberts(m_Width,m_Height);
		double  mask[3][3] = {0,0,-1
							 ,0,1,0
							 ,0,0,0};

		G.DrawImage(&SetMask(mask,1,pRoberts), 28, 330, 400, 250);
	}
	else if(Radio == 1 && Combo == 5) // Roberts(Gy)
	{
		Bitmap pRoberts(m_Width,m_Height);
		double  mask[3][3] = {-1,0,0
							 ,0,1,0
							 ,0,0,0};

		G.DrawImage(&SetMask(mask,1,pRoberts), 28, 330, 400, 250);
	}
	else if(Radio == 1 && Combo == 6) // Sobel(Gx)
	{
		Bitmap pSobel(m_Width,m_Height);
		double  mask[3][3] = {-1,-2,-1
							 ,0,0,0
							 ,1,2,1};

		G.DrawImage(&SetMask(mask,1,pSobel), 28, 330, 400, 250);
	}
	else if(Radio == 1 && Combo == 7) // Sobel(Gy)
	{
		Bitmap pSobel(m_Width,m_Height);
		double  mask[3][3] = {-1,0,1
							 ,-2,0,2
							 ,-1,0,1};

		G.DrawImage(&SetMask(mask,1,pSobel), 28, 330, 400, 250);
	}

	else if(Radio == 2 && Combo == 0) // Otsu
	{
		Bitmap pOtsu(m_Width,m_Height);
		int i, j, t;
		int Y, temp, threshold, tmp;
		double result, max=0.0, wb, wf, ub, uf;

		for(i = 0; i < 256; i++) // 명암도 빈도수 초기화
		 m_Hist[i] = 0.0;

		for(i = 0; i < m_Width; i++)
		{
			for(j = 0; j < m_Height; j ++)
			{
				
				Y = (int)(pic[i][j].GetR() * 0.30 + pic[i][j].GetG() * 0.59 + pic[i][j].GetB() * 0.11);
				fpic[i][j].SetFromCOLORREF(RGB(Y,Y,Y)); // Gray Image	
				m_Hist[Y]++; // Gray Histogram

			}
		}

		for(t = 0; t < 256; t++)
		{
			result = 0.0;
			wb = 0.0, wf=0.0, ub=0.0, uf=0.0; // 초기화

			for(i=0; i<t+1; i++)
			{
				wb = wb + m_Hist[i]; // weight
				ub = (double)(ub + i * m_Hist[i] / wb); // mean
			} // background
			for(i=t+1; i<256; i++)
			{
				wf = wf + m_Hist[i]; // weight
				uf = (double)(uf + i * m_Hist[i] / wf); // mean
			} // foreground

			result = wb * wf * (uf - ub) * (uf - ub); // between variance(클래스 간 분산)

			if(result>max)
			{
				max = result;
				temp = t ;
			} // between variance가 최대가 되는 값을 찾음
		}
		threshold = temp;

		for(i = 0; i < m_Width; i++)
		{
			for(j = 0; j < m_Height; j ++)
			{
				
				Y = (int)(pic[i][j].GetR() * 0.30 + pic[i][j].GetG() * 0.59 + pic[i][j].GetB() * 0.11);
				if(Y>threshold)
					tmp=255;
				else
					tmp=0;
				
				fpic[i][j].SetFromCOLORREF(RGB(tmp,tmp,tmp));
				pOtsu.SetPixel(i, j, fpic[i][j]);
			}
		}

		G.DrawImage(&pOtsu, 28, 330, 400, 250);
	}
	else if(Radio == 2 && Combo == 1) //Grass-Fire
	{
		Bitmap pGrassFire(m_Width,m_Height);

		int nR, nG, nB, i, j, t, check, nlabel=0;
		int Y, temp, threshold, tmp;
		double result, max=0.0, wb, wf, ub, uf;

		for(i = 0; i < 256; i++) // 명암도 빈도수 초기화
		 m_Hist[i] = 0.0;

		for(i = 0; i < m_Width; i++)
		{
			for(j = 0; j < m_Height; j ++)
			{
				
				Y = (int)(pic[i][j].GetR() * 0.30 + pic[i][j].GetG() * 0.59 + pic[i][j].GetB() * 0.11);
				fpic[i][j].SetFromCOLORREF(RGB(Y,Y,Y)); // Gray Image	
				m_Hist[Y]++; // Gray Histogram

			}
		}

		for(t = 0; t < 256; t++)
		{
			result = 0.0;
			wb = 0.0, wf=0.0, ub=0.0, uf=0.0; // 초기화

			for(i=0; i<t+1; i++)
			{
				wb = wb + m_Hist[i]; // weight(가중치)
				ub = (double)(ub + i * m_Hist[i] / wb); // mean
			} // background
			for(i=t+1; i<256; i++)
			{
				wf = wf + m_Hist[i]; // weight(가중치)
				uf = (double)(uf + i * m_Hist[i] / wf); // mean
			} // foreground

			result = wb * wf * (uf - ub) * (uf - ub); // between variance(클래스 간 분산)

			if(result>max)
			{
				max = result;
				temp = t ;
			} // between variance가 최대가 되는 값을 찾음
		}
		threshold = temp;// 임계값 설정

		for(i = 0; i < m_Width; i++)
		{
			for(j = 0; j < m_Height; j ++)
			{
				
				Y = (int)(pic[i][j].GetR() * 0.30 + pic[i][j].GetG() * 0.59 + pic[i][j].GetB() * 0.11);
				if(Y>threshold)
					tmp=255;
				else
					tmp=0;
				
				fpic[i][j].SetFromCOLORREF(RGB(tmp,tmp,tmp));
				//pGrassFire.SetPixel(i, j, fpic[i][j]);
			}
		}

		for(i = 0; i < m_Width; i++)
		{
			for(j = 0; j < m_Height; j ++)
			{
				
				nR = fpic[i][j].GetR();
				nG = fpic[i][j].GetG();
				nB = fpic[i][j].GetB();
				value[i][j]=(nR+nG+nB)/3;

			}
		}

		for(i = 1; i<m_Width-1; i++)
		{
			for(j = 1; j<m_Height-1; j++)
			{
				if(value[i][j]==255 && label[i][j]==0) // 객체(255)인가? && 라벨링이 없는가(지나간 적 없는가)?
				{
					nlabel++; // 증가시킨 새로울 라벨링 값
					label[i][j] = nlabel; // 갱신한 라벨값으로 라벨링
					GrassFire_8direc(value, nlabel, i, j, label); // 주변 조사&라벨링(재귀 함수 호출)
				}
			}
		}

		for(i=0; i<m_Width; i++)
		{
			for(j = 0; j<m_Height; j++)
			{
				check = (int)label[i][j]%4+1;
				switch(check)
				{
				case 1:
					fpic[i][j].SetFromCOLORREF(RGB(252,115,153));
					pGrassFire.SetPixel(i,j,fpic[i][j]);
					break;
				case 2:
					fpic[i][j].SetFromCOLORREF(RGB(255,184,100));
					pGrassFire.SetPixel(i,j,fpic[i][j]);
					break;
				case 3:
					fpic[i][j].SetFromCOLORREF(RGB(253,255,190));
					pGrassFire.SetPixel(i,j,fpic[i][j]);
					break;
				case 4:
					fpic[i][j].SetFromCOLORREF(RGB(138,180,129));
					pGrassFire.SetPixel(i,j,fpic[i][j]);
					break;
				}
				if(value[i][j]==0)
				{
					fpic[i][j].SetFromCOLORREF(RGB(0,0,0));
					pGrassFire.SetPixel(i,j,fpic[i][j]);
				}
			}
		}
		
		
		G.DrawImage(&pGrassFire, 28, 330, 400, 250);
	}
	else if(Radio == 3 && Combo == 0) //Gray Scale
	{
		Bitmap pGrayscale(m_Width,m_Height);
		for(int i = 0; i < m_Width; i++)
		{
			for(int j = 0; j < m_Height; j ++)
			{
				pGrayscale.SetPixel(i,j,GrayScale(pic[i][j]));
			}
		}
		G.DrawImage(&pGrayscale, 28, 330, 400, 250);
	}
	else if(Radio == 3 && Combo == 1) //Negative
	{
		Bitmap pNegative(m_Width,m_Height);
		for(int i = 0; i < m_Width; i++)
		{
			for(int j = 0; j < m_Height; j ++)
			{
				pNegative.SetPixel(i,j,Negative(pic[i][j]));
			}
		}
		G.DrawImage(&pNegative, 28, 330, 400, 250);
	}
}


Color CFilteringDlg::Negative(Color color)
{
	Color a;

	nR = 255 - color.GetR();
	nG = 255 - color.GetG();
	nB = 255 - color.GetB();

	nR = min(255,max(0,nR));
	nG = min(255,max(0,nG));
	nB = min(255,max(0,nB));

	a.SetFromCOLORREF(RGB(nR,nG,nB));

	return a;
}


Color CFilteringDlg::GrayScale(Color color)
{
	Color a;

	double Y;
	Y = color.GetR() * 0.2126 + color.GetG() * 0.7152 + color.GetB() * 0.0722;
	//Y = color.GetR() * 0.29900 + color.GetG() * 0.58700 + color.GetB() * 0.11400;

	a.SetFromCOLORREF(RGB(Y,Y,Y));

	return a;
}

void CFilteringDlg::OnBnClickedButtonApply()
{
	// TODO: Add your control notification handler code here
	CClientDC dc(this);
	HDC hDC = dc.GetSafeHdc();
	Graphics G(hDC);

	Bitmap Custom(m_Width,m_Height);
	int sum = 0;
	double mask[3][3] = 
	{
		GetDlgItemInt(IDC_EDIT_1), GetDlgItemInt(IDC_EDIT_2), GetDlgItemInt(IDC_EDIT_3),
		GetDlgItemInt(IDC_EDIT_4), GetDlgItemInt(IDC_EDIT_5), GetDlgItemInt(IDC_EDIT_6),
		GetDlgItemInt(IDC_EDIT_7), GetDlgItemInt(IDC_EDIT_8), GetDlgItemInt(IDC_EDIT_9)
	};

	/*for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			sum += (int)mask[i][j];

	if(sum == 0)
		sum = 1;*/

	G.DrawImage(&SetMask(mask,1,Custom), 28, 330, 400, 250);

	SetDlgItemInt(IDC_EDIT_1,0);
	SetDlgItemInt(IDC_EDIT_2,0);
	SetDlgItemInt(IDC_EDIT_3,0);
	SetDlgItemInt(IDC_EDIT_4,0);
	SetDlgItemInt(IDC_EDIT_5,0);
	SetDlgItemInt(IDC_EDIT_6,0);
	SetDlgItemInt(IDC_EDIT_7,0);
	SetDlgItemInt(IDC_EDIT_8,0);
	SetDlgItemInt(IDC_EDIT_9,0);
}

Bitmap& CFilteringDlg::SetMask(double mask[3][3], int k, Bitmap& filter)
{
	//TODO: insert return statement here
	double R, G, B;
	for(int i = 1; i < m_Width-1; i++)
	{
		for(int j = 1; j < m_Height-1; j ++)
		{
			R =	mask[0][0] * pic[i-1][j-1].GetR()/k + mask[0][1] * pic[i-1][j].GetR()/k +
				mask[0][2] * pic[i-1][j+1].GetR()/k + mask[1][0] * pic[i][j-1].GetR()/k +
				mask[1][1] * pic[i][j].GetR()/k + mask[1][2] * pic[i][j+1].GetR()/k +
				mask[2][0] * pic[i+1][j-1].GetR()/k + mask[2][1] * pic[i+1][j].GetR()/k +
				mask[2][2] * pic[i+1][j+1].GetR()/k;

			G =	mask[0][0] * pic[i-1][j-1].GetG()/k + mask[0][1] * pic[i-1][j].GetG()/k +
				mask[0][2] * pic[i-1][j+1].GetG()/k + mask[1][0] * pic[i][j-1].GetG()/k +
				mask[1][1] * pic[i][j].GetG()/k + mask[1][2] * pic[i][j+1].GetG()/k +
				mask[2][0] * pic[i+1][j-1].GetG()/k + mask[2][1] * pic[i+1][j].GetG()/k +
				mask[2][2] * pic[i+1][j+1].GetG()/k;
			
			B = mask[0][0] * pic[i-1][j-1].GetB()/k + mask[0][1] * pic[i-1][j].GetB()/k +
				mask[0][2] * pic[i-1][j+1].GetB()/k + mask[1][0] * pic[i][j-1].GetB()/k +
				mask[1][1] * pic[i][j].GetB()/k + mask[1][2] * pic[i][j+1].GetB()/k +
				mask[2][0] * pic[i+1][j-1].GetB()/k + mask[2][1] * pic[i+1][j].GetB()/k +
				mask[2][2] * pic[i+1][j+1].GetB()/k;

			R = min(255,max(0,R));
			G = min(255,max(0,G));
			B = min(255,max(0,B));

			fpic[i-1][j-1].SetFromCOLORREF(RGB(R,G,B));
			filter.SetPixel(i-1,j-1,fpic[i-1][j-1]);

		}
	}

	
	return filter;
}

void CFilteringDlg::BubbleSort(int mask[], int n)
{
	int a, scan, tmp;
	for(scan=0; scan<n-1; scan++)
	{
		for(a=0; a<n-1; a++)
		{
			if(mask[a] > mask[a+1])
			{
				tmp = mask[a];
				mask[a] = mask[a+1];
				mask[a+1] = tmp;
			}
		}
	}
}

Color CFilteringDlg::Otsu(Color color)
{
	return Color();
}


void CFilteringDlg::GrassFire_8direc(double** value, int label, int width, int height, double** L)
{
	for(int i=-1; i<=1; i++)
	{
		for(int j=-1; j<=1; j++)
		{
			if(value[width+i][height+j]==255 && L[width+i][height+j]==0)
			{
				L[width+i][height+j] = label;
				GrassFire_8direc(value, label, width+i, height+j, L);
			}
		}
	}

}
