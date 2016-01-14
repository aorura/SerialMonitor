
// SerialMonitorDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "Mycomm.h"


// CSerialMonitorDlg 대화 상자
class CSerialMonitorDlg : public CDialog
{
// 생성입니다.
public:
	CMycomm* m_comm;
	LRESULT      OnThreadClosed(WPARAM length, LPARAM lpara);
	LRESULT      OnReceive(WPARAM length, LPARAM lpara);
	CSerialMonitorDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	BOOL comport_state;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SERIALMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo_comport_list;
	CComboBox m_combo_baudrate_list;
	afx_msg void OnCbnSelchangeComboComport();
	CString m_str_comport;
	CString m_str_baudrate;
	CEdit m_edit_rcv_view;
	CEdit m_edit_send_data;
	CEdit m_edit_filter;
	afx_msg void OnBnClickedBtConnect();
	afx_msg void OnCbnSelchangeComboBaudrate();
	afx_msg void OnBnClickedBtClear();
	afx_msg void OnBnClickedBtSend();
//	afx_msg void OnEnChangeEditRcvView();
//	afx_msg void OnEnChangeEditFilter();
	afx_msg void OnEnUpdateEditFilter();
	void getSerialPort(CComboBox &serialPort);
	CString m_edit_filter_word;
};
