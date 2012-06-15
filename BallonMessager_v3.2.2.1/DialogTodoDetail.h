// DialogTodoDetail.h : CDialogTodoDetail ������

#pragma once

#include "resource.h"       // ������
#include <atlhost.h>


/**
* \brief ��ʾ�ͱ༭������ϸ��Ϣ
* ��Ҫ����ʾ�����ע�ͣ��������û���дע�͵�ʱ���Զ���������
*/
class CDialogTodoDetail : 
	public CAxDialogImpl<CDialogTodoDetail>,
	public CDialogResize<CDialogTodoDetail>	
{
public:
	CDialogTodoDetail(int iTodoID,bool bReadOnly=false);

	~CDialogTodoDetail();

	enum { IDD = IDD_DIALOGTODODETAIL };

BEGIN_MSG_MAP(CDialogTodoDetail)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	COMMAND_HANDLER(ID_SET_NOTIFY, BN_CLICKED, OnBnClickedSetNotify)
	COMMAND_HANDLER(ID_SAVE, BN_CLICKED, OnBnClickedSave)
	COMMAND_HANDLER(IDC_CHK_AUTOSAVE, BN_CLICKED, OnBnClickedChkAutosave)
	COMMAND_HANDLER(IDC_EDT_REMARK, EN_CHANGE, OnEnChangeEdtRemark)
	CHAIN_MSG_MAP(CAxDialogImpl<CDialogTodoDetail>)
	CHAIN_MSG_MAP(CDialogResize<CDialogTodoDetail>)
END_MSG_MAP()

BEGIN_DLGRESIZE_MAP(CDialogTodoDetail)
	DLGRESIZE_CONTROL(IDC_EDT_REMARK,DLSZ_SIZE_X|DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(IDOK,DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDCANCEL,DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(ID_SAVE,DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(ID_SET_NOTIFY,DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_CHK_AUTOSAVE,DLSZ_MOVE_X|DLSZ_MOVE_Y)
END_DLGRESIZE_MAP()
private:
	int m_id;

	CEdit m_edtName;
	CEdit m_edtDetail;
	bool m_bModified;
	bool m_bReadOnly;
public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedSetNotify(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedChkAutosave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEdtRemark(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
private:
	void SaveToDB();
	void UpdateTitle();
};


