// TaskModifyDialog.h : CTaskModifyDialog ������

#pragma once

#include "resource.h"       // ������
#include <atlhost.h>
#include "ITask.h"

//δ���������ID
#define TASKID_NOT_DEFINED 0

// CTaskModifyDialog

class CTaskModifyDialog : 
	public CAxDialogImpl<CTaskModifyDialog>
{
public:
	//��taskid==TASKID_NOT_DEFINED��������µ�����
	CTaskModifyDialog(int taskid);

	~CTaskModifyDialog()
	{
	}

	enum { IDD = IDD_TASKMODIFYDIALOG };

BEGIN_MSG_MAP(CTaskModifyDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	COMMAND_HANDLER(IDC_CMB_TASKTYPE, CBN_SELCHANGE, OnCbnSelchangeCmbTasktype)
	CHAIN_MSG_MAP(CAxDialogImpl<CTaskModifyDialog>)
END_MSG_MAP()

// �������ԭ��: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
private:
	int m_taskid;

	CEdit m_edtID;
	CComboBox m_edtType;
	//CEdit m_edtTime;
	CComboBox m_cmbMonthWeek;
	CDateTimePickerCtrl m_datePicker;
	CDateTimePickerCtrl m_timePicker;
	CEdit m_edtTips;

	//�����������ڼ���ÿ�µڼ�����������Դ
	vector<string> m_vecWeekDay;
	vector<string> m_vecMonthDay;
public:
	void ComboRollTo(int idCtrl, const string &strTitle);
	void ShowToCombox( int idCtrl, vector<string> &vecClass );
	LRESULT OnCbnSelchangeCmbTasktype(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void EnableControls();
	ITask::TaskType GetSelTaskType();
	CTime GetSelTime();
};


