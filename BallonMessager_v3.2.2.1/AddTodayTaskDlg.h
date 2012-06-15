// AddTodayTaskDlg.h : CAddTodayTaskDlg ������

#pragma once

#include "resource.h"       // ������
#include <atlhost.h>
#include <atlctrls.h>
#include <atltime.h>
#include <string>

/**
* ��ӡ��������񡱶Ի�����δʹ�ã�.
*
* �˹��ܵ����Ŀ�ģ������û�ÿ�쿪ʼ��ʱ�򣬰��Լ��Ĺ����ƻ�д������
* �˹�����δ���
* \todo ��ɡ��������񡱹���
*/
class CAddTodayTaskDlg : 
	public CAxDialogImpl<CAddTodayTaskDlg>
{
public:
	CAddTodayTaskDlg()
	{
	}

	~CAddTodayTaskDlg()
	{
	}

	enum { IDD = IDD_ADDTODAYTASKDLG };

BEGIN_MSG_MAP(CAddTodayTaskDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<CAddTodayTaskDlg>)
END_MSG_MAP()

// �������ԭ��: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CAddTodayTaskDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);

		m_edtTodayTask.Attach(GetDlgItem(IDC_EDT_TODAYTASK));
		m_edtTodayTask.SetWindowText("10:00 ר�Ĺ���\r\n"
			"12:00 ���\r\n"
			"13:00 ��Ϣ\r\n"
			);
		return 1;  // ʹϵͳ���ý���
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		TCHAR strText[1024*10];
		m_edtTodayTask.GetWindowText(strText,1024*10);
		std::string strTasks(strText);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
private:
	CEdit m_edtTodayTask;
};


