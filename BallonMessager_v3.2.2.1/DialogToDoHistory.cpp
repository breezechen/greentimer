// DialogToDoHistory.cpp : CDialogToDoHistory ��ʵ��

#include "stdafx.h"
#include "DialogToDoHistory.h"

#include "GlobeFuns.h"
#include ".\dialogtodohistory.h"
#include ".\DialogTodoDetail.h"
#include "Globe.h"
using namespace std;

// CDialogToDoHistory

LRESULT CDialogToDoHistory::OnInitDialog( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CAxDialogImpl<CDialogToDoHistory>::OnInitDialog(uMsg, wParam, lParam, bHandled);
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

	m_listTodoHistory.SubclassWindow( GetDlgItem( IDC_LIST_TODO ) );
	////����id
	//int id;
	////�������
	//std::string strTask;
	////���ȼ�
	//TaskPriority priority;
	////״̬
	//TaskState state;
	////����
	//std::string strRemark;

	////���񴴽�ʱ��
	//CTime tmCreateTime;
	////����ʼ������ʱ��
	//CTime tmStartWorkTime;

	//CTime tmPlanFinshTime;
	int iCol = 0;
	int iUsedLen = 0;
	m_listTodoHistory.AddColumn(_T("����"),70);
	m_iColCreateTime = iCol++;
	iUsedLen += 70;
	m_listTodoHistory.AddColumn(_T("�ܺ�ʱ"),80);//,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE);
	m_iColTotleHours = iCol++;
	iUsedLen += 70;
	m_listTodoHistory.AddColumn(_T("����"),200);//,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_EDIT,ITEM_FLAGS_NONE);
	m_iColTitle = iCol++;
	iUsedLen += 200;
	m_listTodoHistory.AddColumn(_T("��Ҫ"),50);//,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE);
	m_iColPriority = iCol++;
	iUsedLen += 50;
	//m_listTodoHistory.AddColumn(_T("״̬"),50);//,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE);
	//m_iColState = iCol++;
	//iUsedLen += 50;
	CRect rt;
	m_listTodoHistory.GetClientRect(&rt);
	m_listTodoHistory.AddColumn(_T("��ע"),rt.Width()-iUsedLen);//,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_EDIT,ITEM_FLAGS_NONE);
	m_iColRemark = iCol++;

	Refresh();

	return 1;  // ʹϵͳ���ý���
}

LRESULT CDialogToDoHistory::OnClickedOK( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	Globe::SaveDlgSizeToDB(*this,IDD);
	EndDialog(wID);
	return 0;
}

LRESULT CDialogToDoHistory::OnClickedCancel( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	Globe::SaveDlgSizeToDB(*this,IDD);
	EndDialog(wID);
	return 0;
}

void CDialogToDoHistory::Refresh()
{
	m_listTodoHistory.SetRedraw(FALSE);
	m_listTodoHistory.DeleteAllItems();

	int iHiddenItem = 0;

	set<int> setID;
	g_todoSet.GetHistoryTodoList(setID);
	set<int>::iterator it = setID.begin();
	for (;it!=setID.end();it++)
	{
		ToDoTask todo = g_todoSet.GetToDo(*it);
		AddTodoItem(todo);
	}
	m_listTodoHistory.SortItems(0,true);
	m_listTodoHistory.SetRedraw(TRUE);
}

int CDialogToDoHistory::AddTodoItem( ToDoTask &todo )
{
	//����һ��
	int iItem = m_listTodoHistory.AddItem(GlobeFuns::TimeToFriendlyString(todo.tmCreateTime).GetBuffer(0));
	m_listTodoHistory.SetItemData(iItem,(DWORD)todo.id);
	ATLASSERT(todo.id!=ToDoTask::ERROR_TASKID);

	//ˢ�´�����ʾ
	UpdateItem(iItem);
	return iItem;
}

void CDialogToDoHistory::UpdateItem( int iItem )
{
	DWORD dwId;
	m_listTodoHistory.GetItemData(iItem,dwId);
	ToDoTask todo = g_todoSet.GetToDo(dwId);
	ATLASSERT(todo.id!=ToDoTask::ERROR_TASKID);

	//�ı��������ɫ
	COLORREF clrBgn,clrText;
	m_listTodoHistory.GetItemColours(iItem,m_iColCreateTime,clrBgn,clrText);
	m_listTodoHistory.SetItemColours(iItem,m_iColCreateTime,clrBgn,RGB(0X66,0X66,0X66));
	m_listTodoHistory.SetSubItemData(iItem,m_iColCreateTime,GlobeFuns::TimeToInt(todo.tmCreateTime));

	m_listTodoHistory.SetItemText(iItem,m_iColTitle,todo.strTask.c_str());

	m_listTodoHistory.SetItemText(iItem,m_iColPriority,ToDoTask::PriorityText(todo.priority));
	m_listTodoHistory.SetItemColours(iItem,m_iColPriority,RGB(10,170-todo.priority*25,10),RGB(0,0,0));
	m_listTodoHistory.SetSubItemData(iItem,m_iColPriority,todo.priority);

	CString strTime;
	if (todo.tmPlanFinshTime>=todo.tmCreateTime)
	{
		CTimeSpan ts = todo.tmPlanFinshTime-todo.tmCreateTime;

		CString strTmp;
		if (ts.GetDays()>0)
		{
			strTmp.Format("%d��",ts.GetDays());
			strTime += strTmp;
		}
		if (ts.GetHours()>0)
		{
			strTmp.Format("%dʱ",ts.GetHours());
			strTime += strTmp;
		}
		if (ts.GetDays()==0		//�������һ�����ϵģ��Ͳ�Ҫ��ȷ�������ˡ�
			&& ts.GetMinutes()>0)
		{
			strTmp.Format("%d��",ts.GetMinutes());
			strTime += strTmp;
		}
		if (strTime.IsEmpty())
		{
			strTmp.Format("%d��",ts.GetSeconds());
			strTime += strTmp;
		}
		m_listTodoHistory.SetSubItemData(iItem,m_iColTotleHours,GlobeFuns::TimeToInt(CTime(0)+ts));
	}
	else
	{
		strTime = "�������ݣ�";
	}
	m_listTodoHistory.SetItemText(iItem,m_iColTotleHours,strTime);

	m_listTodoHistory.SetItemText(iItem,m_iColRemark,todo.strRemark.c_str());
}
LRESULT CDialogToDoHistory::OnNMDblclkListTodo(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	ShowRemark();
	return 0;
}

LRESULT CDialogToDoHistory::OnBnClickedEditSetlive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CListArray < int > aSelectedItems;
	m_listTodoHistory.GetSelectedItems(aSelectedItems);
	for (int i=0;i<aSelectedItems.GetSize();i++)
	{
		DWORD id;
		if(!m_listTodoHistory.GetItemData(aSelectedItems[i],id))
		{
			MessageBox("�ؼ�������ʧ�ܣ�");
			return S_FALSE;
		}
		if(!g_todoSet.ReactiveToDo(id))
		{
			MessageBox("�ؼ�������ʧ�ܣ�");
			return S_FALSE;
		}
	}
	Refresh();
	return 0;
}

LRESULT CDialogToDoHistory::OnBnClickedEditTodo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowRemark();



	return 0;
}

LRESULT CDialogToDoHistory::OnBnClickedEditDeleteforever(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CListArray < int > aSelectedItems;
	m_listTodoHistory.GetSelectedItems(aSelectedItems);
	for (int i=0;i<aSelectedItems.GetSize();i++)
	{
		DWORD id;
		if(!m_listTodoHistory.GetItemData(aSelectedItems[i],id))
		{
			MessageBox("����ɾ������ʧ�ܣ�");
			return S_FALSE;
		}
		if(!g_todoSet.DeleteToDoForever(id))
		{
			MessageBox("����ɾ������ʧ�ܣ�");
			return S_FALSE;
		}
	}
	Refresh();
	return 0;
}

void CDialogToDoHistory::ShowRemark()
{
	CListArray < int > aSelectedItems;
	m_listTodoHistory.GetSelectedItems(aSelectedItems);
	if (aSelectedItems.IsEmpty())
	{
		MessageBox("��ѡ����Ҫ�༭�Ĵ������");
		return ;
	}

	DWORD id;
	ATLVERIFY(m_listTodoHistory.GetItemData(aSelectedItems[0],id));
	CDialogTodoDetail dlg(id);//,true);	//��ע�������б༭��һЩ����ʱһЩ������ɺ󣬻��ǿ���дһЩ�������ֵġ�
	dlg.DoModal();
}