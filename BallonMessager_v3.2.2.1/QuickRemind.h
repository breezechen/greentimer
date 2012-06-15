// QuickRemind.h : CQuickRemind ������

#pragma once

#include "resource.h"       // ������
#include <atlhost.h>
#include "TaskDB.h"

// CQuickRemind

class CQuickRemind : 
	public CAxDialogImpl<CQuickRemind>
{
public:
	CQuickRemind(int iMinute)
	{
		m_iMinute = iMinute;
		ATLASSERT(m_iMinute>0);
	}

	~CQuickRemind()
	{
	}

	enum { IDD = IDD_QUICKREMIND };

BEGIN_MSG_MAP(CQuickRemind)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<CQuickRemind>)
END_MSG_MAP()

// �������ԭ��: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CQuickRemind>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		char buf[64];
		::SetWindowText(GetDlgItem(IDC_EDT_MINITE),itoa(m_iMinute,buf,10));
		::SetWindowText(GetDlgItem(IDC_EDT_TIPS),
			ATL::CString("�����ˣ�����Ҫ����\r\n\r\n������")
			+CTime::GetCurrentTime().Format("%y-%m-%d %H:%M:%S")
			+"�趨�����ѣ�");

		CUpDownCtrl spnMinute = GetDlgItem(IDC_SPIN_MINUTE);
		spnMinute.SetRange(1,365*24*60*10);

		return 1;  // ʹϵͳ���ý���
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		TCHAR strMin[1024],strTip[1024];
		::GetWindowText(GetDlgItem(IDC_EDT_MINITE),strMin,1024);
		::GetWindowText(GetDlgItem(IDC_EDT_TIPS),strTip,1024);

		if (atoi(strMin)<=0)
		{
			MessageBox("�����õ�ʱ��ֵ���Ϸ���");
			return 0;
		}
		CTime tmNextTime = CTime::GetCurrentTime() + CTimeSpan(0,0,atoi(strMin),0);
		if(!g_TaskDB.AddTimeTask(tmNextTime,strTip))
		{
			return 0;
		}
		if(!g_TaskDB.SaveToDB())
		{
			return 0;
		}
		ATL::CString strMsg = ATL::CString("�Ѿ�����һ�����ѣ�����")
			+ strMin 
			+ "���Ӻ�" 
			+ tmNextTime.Format("%y-%m-%d %H:%M:%S") 
			+ " ��ִ��";
		MessageBox(strMsg);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
private:
	int m_iMinute;
};


