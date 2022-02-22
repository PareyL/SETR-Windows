
// MySecondProjectMFCDlg.cpp : fichier d'implémentation
//

#include "pch.h"
#include "framework.h"
#include "MySecondProjectMFC.h"
#include "MySecondProjectMFCDlg.h"
#include "afxdialogex.h"
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HANDLE Sema, Thread_Inc, Thread_Dec;
CSemaphore Sync(0, 1, NULL, NULL);

static UINT Inc(int* Cpt)
{
	for (;;)
	{
		WaitForSingleObject(Sema, INFINITE);
		(*Cpt)++;
	}
	return 0;
}

static UINT Dec(int* Cpt)
{
	for (;;)
	{
		Sync.Lock();
		(*Cpt)--;
	}
	return 0;
}


// boîte de dialogue CAboutDlg utilisée pour la boîte de dialogue 'À propos de' pour votre application

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Données de boîte de dialogue
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

// Implémentation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// boîte de dialogue de CMySecondProjectMFCDlg



CMySecondProjectMFCDlg::CMySecondProjectMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYSECONDPROJECTMFC_DIALOG, pParent)
	, Cpt(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMySecondProjectMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Cpt);
}

BEGIN_MESSAGE_MAP(CMySecondProjectMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CMySecondProjectMFCDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMySecondProjectMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMySecondProjectMFCDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CMySecondProjectMFCDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// gestionnaires de messages de CMySecondProjectMFCDlg

BOOL CMySecondProjectMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Ajouter l'élément de menu "À propos de..." au menu Système.

	// IDM_ABOUTBOX doit se trouver dans la plage des commandes système.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Définir l'icône de cette boîte de dialogue.  L'infrastructure effectue cela automatiquement
	//  lorsque la fenêtre principale de l'application n'est pas une boîte de dialogue
	SetIcon(m_hIcon, TRUE);			// Définir une grande icône
	SetIcon(m_hIcon, FALSE);		// Définir une petite icône

	ShowWindow(SW_MAXIMIZE);

	ShowWindow(SW_MINIMIZE);

	Cpt = rand();
	UpdateData(false);
	SetTimer(1, 500, NULL);
	Sema = CreateSemaphore(NULL, 0, 1, NULL);
	Thread_Inc = AfxBeginThread((AFX_THREADPROC)Inc, &Cpt);
	Thread_Dec = AfxBeginThread((AFX_THREADPROC)Dec, &Cpt);


	return TRUE;  // retourne TRUE, sauf si vous avez défini le focus sur un contrôle
}

void CMySecondProjectMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// Si vous ajoutez un bouton Réduire à votre boîte de dialogue, vous devez utiliser le code ci-dessous
//  pour dessiner l'icône.  Pour les applications MFC utilisant le modèle Document/Vue,
//  cela est fait automatiquement par l'infrastructure.

void CMySecondProjectMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // contexte de périphérique pour la peinture

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrer l'icône dans le rectangle client
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dessiner l'icône
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Le système appelle cette fonction pour obtenir le curseur à afficher lorsque l'utilisateur fait glisser
//  la fenêtre réduite.
HCURSOR CMySecondProjectMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMySecondProjectMFCDlg::OnEnChangeEdit1()
{
	// TODO:  S'il s'agit d'un contrôle RICHEDIT, le contrôle ne
	// envoyez cette notification sauf si vous substituez CDialogEx::OnInitDialog()
	// fonction et appelle CRichEditCtrl().SetEventMask()
	// avec l'indicateur ENM_CHANGE ajouté au masque grâce à l'opérateur OR.

	// TODO:  Ajoutez ici le code de votre gestionnaire de notification de contrôle
}


void CMySecondProjectMFCDlg::OnBnClickedButton1()
{
	ReleaseSemaphore(Sema, 1, NULL);
}


void CMySecondProjectMFCDlg::OnBnClickedButton2()
{
	Sync.Unlock(1, NULL);
}


void CMySecondProjectMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
	UpdateData(false);
	CDialogEx::OnTimer(nIDEvent);
}


void CMySecondProjectMFCDlg::OnBnClickedButton3()
{
	Cpt = rand();
}
