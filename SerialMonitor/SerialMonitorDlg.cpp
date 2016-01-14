
// SerialMonitorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "SerialMonitor.h"
#include "SerialMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CSerialMonitorDlg 대화 상자




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


// CSerialMonitorDlg 메시지 처리기
LRESULT CSerialMonitorDlg::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	//overlapped i/o 핸들을닫는다.
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
		m_comm->Receive(data,length); //length 길이만큼데이터를받는다.
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

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSerialMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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
		if(m_comm)        //컴포트가존재하면
		{
			m_comm->Close();
			m_comm = NULL;
			//AfxMessageBox(_T("COM 포트닫힘"));
			comport_state=false;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
			GetDlgItem(IDC_BT_SEND)->EnableWindow(false);
		}
	}
	else
	{
		m_comm= new CMycomm(_T("\\\\.\\")+m_str_comport,m_str_baudrate,_T("None"),_T("8 Bit"),_T("1 Bit"));// initial Comm port
		BOOL isConnected = m_comm->Create(GetSafeHwnd());
		if(isConnected != 0) //통신포트를열고윈도우의핸들을넘긴다.
		{
			//AfxMessageBox(_T("COM 포트열림"));
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
