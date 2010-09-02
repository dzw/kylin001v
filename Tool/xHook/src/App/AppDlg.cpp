// AppDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "App.h"
#include "AppDlg.h"
#include "inject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAppDlg �Ի���




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


// CAppDlg ��Ϣ�������

BOOL CAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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