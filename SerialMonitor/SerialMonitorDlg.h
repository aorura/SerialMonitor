
// SerialMonitorDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "Mycomm.h"


// CSerialMonitorDlg ��ȭ ����
class CSerialMonitorDlg : public CDialog
{
// �����Դϴ�.
public:
	CMycomm* m_comm;
	LRESULT      OnThreadClosed(WPARAM length, LPARAM lpara);
	LRESULT      OnReceive(WPARAM length, LPARAM lpara);
	CSerialMonitorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	BOOL comport_state;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SERIALMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
