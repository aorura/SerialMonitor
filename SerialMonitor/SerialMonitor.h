
// SerialMonitor.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSerialMonitorApp:
// �� Ŭ������ ������ ���ؼ��� SerialMonitor.cpp�� �����Ͻʽÿ�.
//

class CSerialMonitorApp : public CWinAppEx
{
public:
	CSerialMonitorApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CSerialMonitorApp theApp;