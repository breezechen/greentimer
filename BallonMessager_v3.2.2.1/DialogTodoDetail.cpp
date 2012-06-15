// DialogTodoDetail.cpp : CDialogTodoDetail ��ʵ��

#include "stdafx.h"
#include "DialogTodoDetail.h"
#include "ToDoTask.h"
#include ".\dialogtododetail.h"
#include "Globe.h"

// CDialogTodoDetail

CDialogTodoDetail::CDialogTodoDetail( int iTodoID,bool bReadOnly )
{
	m_id = iTodoID;
	m_bReadOnly = bReadOnly;
}

CDialogTodoDetail::~CDialogTodoDetail()
{

}
LRESULT CDialogTodoDetail::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CAxDialogImpl<CDialogTodoDetail>::OnInitDialog(uMsg, wParam, lParam, bHandled);

	DlgResize_Init();
	//�Ƿ񱣴��������ڵĴ�С������ǣ��ı����С
	Globe::ReadDlgSizeFromDB(*this,IDD);

	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	ToDoTask todo = g_todoSet.GetToDo(m_id);
	if (todo.id==ToDoTask::ERROR_TASKID)
	{
		MessageBox("���񲻴��ڣ�","�����ˣ�");
		return 1;
	}
	SetWindowText((todo.strTask + _T(" - ��ע")).c_str());
	SetDlgItemText(IDC_EDT_NAME,todo.strTask.c_str());
	SetDlgItemText(IDC_EDT_REMARK,todo.strRemark.c_str());
	CEdit edtRemark = GetDlgItem(IDC_EDT_REMARK);
	edtRemark.SetReadOnly((BOOL)m_bReadOnly);
	//wnd.EnableWindow((BOOL)m_bReadOnly);	//���ֻ����ʽ̫�ѿ��ˡ�

	m_bModified = false;

	//��ʱ����
	SetTimer(0,1*60*1000,NULL);	//һ���ӱ���һ��

	CButton btn(GetDlgItem(IDC_CHK_AUTOSAVE));
	btn.SetCheck(TRUE);
	
	return 1;  // ʹϵͳ���ý���
}

LRESULT CDialogTodoDetail::OnClickedOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	SaveToDB();
	Globe::SaveDlgSizeToDB(*this,IDD);
	EndDialog(wID);
	return 0;
}

LRESULT CDialogTodoDetail::OnClickedCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	if (m_bModified)
	{
		int iRes = MessageBox(_T("�����Ѹ��ģ�Ҫ������"),"ע��",MB_YESNOCANCEL);
		if(IDOK==iRes)
		{
			SaveToDB();
		}
		else if (IDCANCEL==iRes)
		{
			return 0;
		}
		else
		{
			//������񡱼������棬��Ȼ�رա�
		}
	}
	Globe::SaveDlgSizeToDB(*this,IDD);
	EndDialog(wID);
	return 0;
}
LRESULT CDialogTodoDetail::OnBnClickedSetNotify(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	return 0;
}

LRESULT CDialogTodoDetail::OnBnClickedSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SaveToDB();

	return 0;
}

LRESULT CDialogTodoDetail::OnBnClickedChkAutosave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CDialogTodoDetail::OnTimer( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	if (!m_bModified)
	{
		return 0;
	}
	SaveToDB();
	return 0;
}

void CDialogTodoDetail::SaveToDB()
{
	ToDoTask todo = g_todoSet.GetToDo(m_id);
	CString strTmp;
	GetDlgItemText(IDC_EDT_NAME,strTmp);
	todo.strTask = strTmp.GetBuffer(0);
	GetDlgItemText(IDC_EDT_REMARK,strTmp);
	todo.strRemark = strTmp.GetBuffer(0);

	g_todoSet.UpdateToDo(todo);

	if (m_bModified)
	{
		m_bModified = false;
		UpdateTitle();
	}
}
LRESULT CDialogTodoDetail::OnEnChangeEdtRemark(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (!m_bModified)
	{
		m_bModified = true;
		UpdateTitle();
	}

	return 0;
}

void CDialogTodoDetail::UpdateTitle()
{
	ToDoTask todo = g_todoSet.GetToDo(m_id);
	if (todo.id==ToDoTask::ERROR_TASKID)
	{
		return ;
	}
	if (m_bModified)
	{
		SetWindowText((todo.strTask + _T(" - ��ע *")).c_str());
	}
	else
	{
		SetWindowText((todo.strTask + _T(" - ��ע")).c_str());
	}
}
