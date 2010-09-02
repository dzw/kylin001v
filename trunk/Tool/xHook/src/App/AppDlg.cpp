// AppDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "App.h"
#include "AppDlg.h"
#include "inject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAppDlg 对话框




CAppDlg::CAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAppDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BTN_RUN, OnStartBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CAppDlg 消息处理程序

BOOL CAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	HANDLE hSnapshot = NULL;
	PROCESSENTRY32 pe;

	SetPrivilege();

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
	pe.dwSize = sizeof(PROCESSENTRY32);

	Process32First(hSnapshot,&pe);
	do
	{
		if (strcmp(pe.szExeFile,"TestClToSr.exe") == 0)
		{
#ifdef _DEBUG
			inject(pe.th32ProcessID,"xHook_d.dll");
#else
			inject(pe.th32ProcessID);
#endif		
			break;
		}
	}
	while(Process32Next(hSnapshot,&pe)==TRUE);

	CloseHandle (hSnapshot);

	//////////////////////////////////////////////////////////////////////////
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAppDlg::OnStartBtn()
{
	
}

void CAppDlg::OnDestroy()
{
	unload();

	CDialog::OnDestroy();
}