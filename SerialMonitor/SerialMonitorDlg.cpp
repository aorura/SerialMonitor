
// SerialMonitorDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "SerialMonitor.h"
#include "SerialMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSerialMonitorDlg ��ȭ ����




CSerialMonitorDlg::CSerialMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialMonitorDlg::IDD, pParent)
	, m_str_comport(_T(""))
	, m_str_baudrate(_T(""))
	, m_edit_filter_word(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMPORT, m_combo_comport_list);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_combo_baudrate_list);
	DDX_CBString(pDX, IDC_COMBO_COMPORT, m_str_comport);
	DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_str_baudrate);
	DDX_Control(pDX, IDC_EDIT_RCV_VIEW, m_edit_rcv_view);
	DDX_Control(pDX, IDC_EDIT_SEND_DATA, m_edit_send_data);
	DDX_Control(pDX, IDC_EDIT_FILTER, m_edit_filter);
	DDX_Text(pDX, IDC_EDIT_FILTER, m_edit_filter_word);
}

BEGIN_MESSAGE_MAP(CSerialMonitorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_MYCLOSE, &CSerialMonitorDlg::OnThreadClosed)
	ON_MESSAGE(WM_MYRECEIVE,&CSerialMonitorDlg::OnReceive)

	ON_BN_CLICKED(IDC_BT_CONNECT, &CSerialMonitorDlg::OnBnClickedBtConnect)
	ON_BN_CLICKED(IDC_BT_CLEAR, &CSerialMonitorDlg::OnBnClickedBtClear)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT, &CSerialMonitorDlg::OnCbnSelchangeComboComport)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CSerialMonitorDlg::OnCbnSelchangeComboBaudrate)

	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT, &CSerialMonitorDlg::OnCbnSelchangeComboComport)
	ON_BN_CLICKED(IDC_BT_CONNECT, &CSerialMonitorDlg::OnBnClickedBtConnect)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CSerialMonitorDlg::OnCbnSelchangeComboBaudrate)
	ON_BN_CLICKED(IDC_BT_CLEAR, &CSerialMonitorDlg::OnBnClickedBtClear)
	ON_BN_CLICKED(IDC_BT_SEND, &CSerialMonitorDlg::OnBnClickedBtSend)
//	ON_EN_CHANGE(IDC_EDIT_RCV_VIEW, &CSerialMonitorDlg::OnEnChangeEditRcvView)
//	ON_EN_CHANGE(IDC_EDIT_FILTER, &CSerialMonitorDlg::OnEnChangeEditFilter)
	ON_EN_UPDATE(IDC_EDIT_FILTER, &CSerialMonitorDlg::OnEnUpdateEditFilter)
END_MESSAGE_MAP()


// CSerialMonitorDlg �޽��� ó����
LRESULT CSerialMonitorDlg::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	//overlapped i/o �ڵ����ݴ´�.
	((CMycomm*)lpara)->HandleClose();
	delete ((CMycomm*)lpara);

	return 0;
}
LRESULT CSerialMonitorDlg::OnReceive(WPARAM length, LPARAM lpara)
{
	CString str;
	char data[20000];
	if(m_comm)
	{
		m_comm->Receive(data,length); //length ���̸�ŭ�����͸��޴´�.
		data[length]=_T('\0');
		str+=_T("\r\n");
		for(WPARAM i = 0; i < length; i++)
		{
			str += data[i];
		}

		//CString lowerStr = str;
		//CString lowerFilter = m_edit_filter_word;
		/*if (m_edit_filter_word.GetLength() > 0 && lowerStr.MakeLower().Find(lowerFilter.MakeLower()) == -1) {
			return 0;
		}*/

		if (m_edit_filter_word.GetLength() > 0 && str.Find(m_edit_filter_word) == -1) {
			TRACE("length: %d, find: %d\n", m_edit_filter_word.GetLength(), str.Find(m_edit_filter_word));
			return 0;
		}

		//int end = m_edit_rcv_view.GetWindowTextLength();
		//m_edit_rcv_view.SetSel(end, end);
		TRACE("m_edit_rcv_view.ReplaceSel(str)\n");
		m_edit_rcv_view.ReplaceSel(str);
	}
	return 0;
} 


BOOL CSerialMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_edit_rcv_view.SetLimitText(0);

	getSerialPort(m_combo_comport_list);

	m_combo_baudrate_list.AddString(_T("256000"));
	m_combo_baudrate_list.AddString(_T("128000"));
	m_combo_baudrate_list.AddString(_T("115200"));
	m_combo_baudrate_list.AddString(_T("57600"));
	m_combo_baudrate_list.AddString(_T("56000"));
	m_combo_baudrate_list.AddString(_T("38400"));
	m_combo_baudrate_list.AddString(_T("19200"));
	m_combo_baudrate_list.AddString(_T("14400"));
	m_combo_baudrate_list.AddString(_T("9600"));
	m_combo_baudrate_list.AddString(_T("4800"));
	m_combo_baudrate_list.AddString(_T("2400"));
	m_combo_baudrate_list.AddString(_T("1200"));
	m_combo_baudrate_list.AddString(_T("600"));
	m_combo_baudrate_list.AddString(_T("300"));
	m_combo_baudrate_list.AddString(_T("110"));

	comport_state=false;
	GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
	m_str_comport = _T("COM131");
	m_str_baudrate = _T("115200");
	UpdateData(FALSE); 

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CSerialMonitorDlg::getSerialPort(CComboBox &serialPort)  
{
	HKEY hKey;
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);
	TCHAR szData[20], szName[100];
	DWORD index = 0, dwSize=100, dwSize2 = 20, dwType = REG_SZ; 
	serialPort.ResetContent();
	memset(szData, 0x00, sizeof(szData));
	memset(szName, 0x00, sizeof(szName));

	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL))
	{
		index++;
		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
		serialPort.AddString(CString(szData)); 
		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 100;
		dwSize2 = 20;
	}
	RegCloseKey(hKey); 
}

void CSerialMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSerialMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSerialMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSerialMonitorDlg::OnCbnSelchangeComboComport()
{
	UpdateData();
}

void CSerialMonitorDlg::OnBnClickedBtConnect()
{
	if(comport_state)
	{
		if(m_comm)        //����Ʈ�������ϸ�
		{
			m_comm->Close();
			m_comm = NULL;
			//AfxMessageBox(_T("COM ��Ʈ����"));
			comport_state=false;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
			GetDlgItem(IDC_BT_SEND)->EnableWindow(false);
		}
	}
	else
	{
		m_comm= new CMycomm(_T("\\\\.\\")+m_str_comport,m_str_baudrate,_T("None"),_T("8 Bit"),_T("1 Bit"));// initial Comm port
		BOOL isConnected = m_comm->Create(GetSafeHwnd());
		if(isConnected != 0) //�����Ʈ���������������ڵ����ѱ��.
		{
			//AfxMessageBox(_T("COM ��Ʈ����"));
			comport_state=true;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("CLOSE"));
			GetDlgItem(IDC_BT_SEND)->EnableWindow(true);
		}
		else
		{
			
			AfxMessageBox(_T("ERROR!"));
		}
	}
}

void CSerialMonitorDlg::OnCbnSelchangeComboBaudrate()
{
	UpdateData();
}

void CSerialMonitorDlg::OnBnClickedBtClear()
{
	GetDlgItem(IDC_EDIT_RCV_VIEW)->SetWindowText(_T(" "));
}

void CSerialMonitorDlg::OnBnClickedBtSend()
{
	CString str;
	GetDlgItem(IDC_EDIT_SEND_DATA)->GetWindowText(str);
	str+= "\r\n";
	m_comm->Send(str,str.GetLength());
}

void CSerialMonitorDlg::OnEnUpdateEditFilter()
{
	m_edit_filter.GetWindowTextW(m_edit_filter_word);
}
