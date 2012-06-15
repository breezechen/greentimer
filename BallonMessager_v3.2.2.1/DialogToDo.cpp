#include "StdAfx.h"
#include "resource.h"
#include ".\dialogtodo.h"
#include "DialogTodoDetail.h"
#include "GlobeFuns.h"
#include "DialogToDoHistory.h"
#include "Globe.h"

using namespace std;


std::vector<BOOL> DialogToDo::m_vecPriorityShow;
std::vector<BOOL> DialogToDo::m_vecStateShow;

DialogToDo::DialogToDo(void)
{
	m_bHideFinished = true;
}

DialogToDo::~DialogToDo(void)
{
}

LRESULT DialogToDo::OnBnClickedOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//if (SaveData()==S_OK)
	{
		Globe::SaveDlgSizeToDB(*this,IDD);
		EndDialog(wID);
	}

	return 0;
}

LRESULT DialogToDo::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Globe::SaveDlgSizeToDB(*this,IDD);
	EndDialog(0);

	return 0;
}

LRESULT DialogToDo::OnUserDataSelected( LPNMHDR lpNMHDR )
{
	CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR );
	ATLTRACE("EDIT:%d,%d\n",pListNotify->m_nItem,pListNotify->m_nSubItem);
	return S_OK;
}

LRESULT DialogToDo::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
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

	m_listTodo.SubclassWindow( GetDlgItem( IDC_LIST_TODO ) );

	int iCol = 0;
	int iUsedLen = 0;
	m_listTodo.AddColumn(_T("���"),0);
	m_iColID = iCol++;
	iUsedLen += 0;
	m_listTodo.AddColumn(_T("����"),60);
	m_iColCreateTime = iCol++;
	iUsedLen += 60;
	m_listTodo.AddColumn(_T("����"),260,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_EDIT,ITEM_FLAGS_NONE);
	m_iColTitle = iCol++;
	iUsedLen += 260;
	m_listTodo.AddColumn(_T("��Ҫ"),50,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE);
	m_iColPriority = iCol++;
	iUsedLen += 50;
	m_listTodo.AddColumn(_T("״̬"),50,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE);
	m_iColState = iCol++;
	iUsedLen += 50;
	CRect rt;
	m_listTodo.GetClientRect(&rt);
	m_listTodo.AddColumn(_T("��ע"),rt.Width()-iUsedLen-15);//,ITEM_IMAGE_NONE,FALSE,ITEM_FORMAT_EDIT,ITEM_FLAGS_NONE);
	m_iColRemark = iCol++;

	m_aListPriority.Add(_T("����"));	//��˳���ʼ����˳��ž���priority
	m_aListPriority.Add(_T("��Ҫ"));
	m_aListPriority.Add(_T("��ͨ"));
	m_aListPriority.Add(_T("����Ҫ"));
	if (m_vecPriorityShow.empty())	//��ֻ֤��ʼ��һ��
	{
		m_vecPriorityShow.push_back(TRUE);
		m_vecPriorityShow.push_back(TRUE);
		m_vecPriorityShow.push_back(TRUE);
		m_vecPriorityShow.push_back(TRUE);
	}

	m_aListState.Add(_T("�ƻ���"));//��˳���ʼ����˳��ž���state
	m_aListState.Add(_T("������"));
	m_aListState.Add(_T("��ͣ"));
	m_aListState.Add(_T("�����"));
	m_aListState.Add(_T("��ȡ��"));
	if (m_vecStateShow.empty())	//��ֻ֤��ʼ��һ��
	{
		m_vecStateShow.push_back(TRUE);
		m_vecStateShow.push_back(TRUE);
		m_vecStateShow.push_back(TRUE);
		m_vecStateShow.push_back(TRUE);
		m_vecStateShow.push_back(TRUE);
	}

	CButton btn(GetDlgItem(IDC_CHK_HIDEOUTTIME));
	//m_bHideTimeOut = btn.GetCheck();
	btn.SetCheck(m_bHideFinished);

	ReloadTodos();

	return S_OK;
}
LRESULT DialogToDo::OnBnClickedAddTodo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int id = g_todoSet.AddToDo();
	ATLASSERT(id!=ToDoTask::ERROR_TASKID);

	//g_todoSet.Save();
	//ReloadTodos();

	int iItem = AddTodoItem(g_todoSet.GetToDo(id));

	m_listTodo.SetFocus();
	m_listTodo.SetFocusItem(iItem,m_iColTitle);
	m_listTodo.SelectItem(iItem);
	m_listTodo.EditItem(iItem,m_iColTitle);

	UpdateCounts();
	return 0;
}

//LRESULT DialogToDo::OnBnClickedSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//{
//	return SaveData();
//}

LRESULT DialogToDo::SaveData(int iItem)
{
	//for(int i=0;i<m_listTodo.GetItemCount();i++)
	//{
	DWORD id;
	m_listTodo.GetItemData(iItem,id);
	ToDoTask todo = g_todoSet.GetToDo(id);
	if (todo.id!=id)
	{
		ATLASSERT(ToDoTask::ERROR_TASKID==todo.id);
		return 0;
	}

	todo.strTask = m_listTodo.GetItemText(iItem,m_iColTitle).GetBuffer(0);

	todo.priority = (ToDoTask::TaskPriority)m_aListPriority.Find(m_listTodo.GetItemText(iItem,m_iColPriority));
	todo.state = (ToDoTask::TaskState)m_aListState.Find(m_listTodo.GetItemText(iItem,m_iColState));
	//todo.strRemark = m_listTodo.GetItemText(i,3).GetBuffer(0);

	if(!g_todoSet.UpdateToDo(todo))
	{
		MessageBox("��������ʱ�����δ֪����");
		return 0;
	}

	UpdateCounts();

	return 0;
}
LRESULT DialogToDo::OnBnClickedChkHideouttime(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//if (SaveData()!=S_OK)
	//{
	//	return S_FALSE;
	//}
	CButton btn(GetDlgItem(IDC_CHK_HIDEOUTTIME));
	m_bHideFinished = btn.GetCheck();

	if(m_bHideFinished)
	{
		//ȫ����ʾ
		for (int i=0;i<(int)m_vecStateShow.size();i++)
		{
			m_vecStateShow[i] = TRUE;
		}
		for (int j=0;j<(int)m_vecPriorityShow.size();j++)
		{
			m_vecPriorityShow[j] = TRUE;
		}
	}
	else
	{
		//����ȡ���������������Ϊ����
		m_vecStateShow[3] = FALSE;
		m_vecStateShow[4] = FALSE;
	}


	ReloadTodos();

	return 0;
}

LRESULT DialogToDo::ReloadTodos()
{
	m_listTodo.SetRedraw(FALSE);
	m_listTodo.DeleteAllItems();

	int iHiddenItem = 0;

	set<int> setID;
	g_todoSet.GetTodoList(setID);
	set<int>::iterator it = setID.begin();
	for (;it!=setID.end();it++)
	{
		ToDoTask todo = g_todoSet.GetToDo(*it);

		if (!m_vecPriorityShow[todo.priority])
		{
			iHiddenItem++;
			continue;
		}
		if (!m_vecStateShow[todo.state])
		{
			iHiddenItem++;
			continue;
		}
		AddTodoItem(todo);
	}
	m_listTodo.SortItems(m_iColID,true);
	m_listTodo.SortItems(m_iColState,true);

	m_listTodo.SetRedraw(TRUE);

	if (iHiddenItem>0)
	{
		//CButton btn(GetDlgItem(IDC_CHK_HIDEOUTTIME));
		//btn.SetCheck(TRUE);
		//btn.EnableWindow(TRUE);

		CString strHint;
		strHint.Format(_T("������ %d ����ȡ����"),iHiddenItem);
		SetDlgItemText(IDC_CHK_HIDEOUTTIME,strHint.GetBuffer(0));
	}
	else
	{
		//CButton btn(GetDlgItem(IDC_CHK_HIDEOUTTIME));
		//btn.SetCheck(FALSE);
		//btn.EnableWindow(FALSE);

		CString strHint;
		strHint.Format(_T("�� %d ���ȫ����ʾ"),m_listTodo.GetItemCount());
		SetDlgItemText(IDC_CHK_HIDEOUTTIME,strHint.GetBuffer(0));
	}

	UpdateCounts();

	return S_OK;
}
LRESULT DialogToDo::OnLvnItemchangedListTodo(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	return 0;
}

LRESULT DialogToDo::OnBnClickedAddDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CListArray < int > aSelectedItems;
	m_listTodo.GetSelectedItems(aSelectedItems);
	for (int i=0;i<aSelectedItems.GetSize();i++)
	{
		DWORD id;
		if(!m_listTodo.GetItemData(aSelectedItems[i],id))
		{
			MessageBox("ɾ���б�ʧ�ܣ�");
			return S_FALSE;
		}
		if(!g_todoSet.DeleteToDo(id))
		{
			MessageBox("ɾ���б�ʧ�ܣ�");
			return S_FALSE;
		}
	}
	ReloadTodos();
	return 0;
}

LRESULT DialogToDo::OnLvnKeydownListTodo(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	return 0;
}

LRESULT DialogToDo::OnLvnItemModified(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	//������ĵ�������״̬�����������ȼ���ˢ��һ�¡�
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iItem<0)
	{
		return 0;
	}

	//��������
	SaveData(pNMLV->iItem);

	//ˢ�½���
	if (m_iColPriority==pNMLV->iSubItem || m_iColState==pNMLV->iSubItem)
	{
		UpdateItem(pNMLV->iItem);
	}
	return 0;
}
LRESULT DialogToDo::OnBnClickedEditTodo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return EditTodo();
}

int DialogToDo::FindItem( int todo_id )
{
	int i=0;
	for(;i<m_listTodo.GetItemCount();i++)
	{
		DWORD dwItemId;
		m_listTodo.GetItemData(i,dwItemId);
		if (dwItemId==todo_id)
		{
			return i;
		}
	}
	return -1;
}

int DialogToDo::AddTodoItem( ToDoTask &todo )
{
	//����һ��
	//int iItem = m_listTodo.AddItem(GlobeFuns::TimeToFriendlyString(todo.tmCreateTime).GetBuffer(0));
	char buf[128];
	int iItem = m_listTodo.AddItem(itoa(todo.id,buf,10));
	m_listTodo.SetItemData(iItem,(DWORD)todo.id);
	m_listTodo.SetSubItemData(iItem,0,(DWORD)todo.id);
	ATLASSERT(todo.id!=ToDoTask::ERROR_TASKID);

	//ˢ�´�����ʾ
	UpdateItem(iItem);
	return iItem;
}

void DialogToDo::UpdateItem( int iItem )
{
	DWORD dwId;
	m_listTodo.GetItemData(iItem,dwId);
	ToDoTask todo = g_todoSet.GetToDo(dwId);
	ATLASSERT(todo.id!=ToDoTask::ERROR_TASKID);

	//m_listTodo.SetItemText(iItem,m_iColID,itoa(todo.id,buf,10));

	COLORREF clrBgn,clrText;
	m_listTodo.GetItemColours(iItem,m_iColCreateTime,clrBgn,clrText);
	m_listTodo.SetItemColours(iItem,m_iColCreateTime,clrBgn,RGB(0X66,0X66,0X66));
	m_listTodo.SetSubItemData(iItem,m_iColCreateTime,GlobeFuns::TimeToInt(todo.tmCreateTime));
	m_listTodo.SetItemText(iItem,m_iColCreateTime,GlobeFuns::TimeToFriendlyString(todo.tmCreateTime).GetBuffer(0));

	m_listTodo.SetItemText(iItem,m_iColTitle,todo.strTask.c_str());

	m_listTodo.SetItemFormat(iItem,m_iColPriority,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,m_aListPriority);
	m_listTodo.SetItemComboIndex(iItem,m_iColPriority,todo.priority);
	m_listTodo.SetItemColours(iItem,m_iColPriority,RGB(10,170-todo.priority*25,10),RGB(0,0,0));
	m_listTodo.SetSubItemData(iItem,m_iColPriority,todo.priority);

	m_listTodo.SetItemFormat(iItem,m_iColState,ITEM_FORMAT_COMBO,ITEM_FLAGS_NONE,m_aListState);
	m_listTodo.SetItemComboIndex(iItem,m_iColState,todo.state);
	m_listTodo.SetItemColours(iItem,m_iColState,ToDoTask::GetStateColor(todo.state),RGB(0,0,0));
	m_listTodo.SetSubItemData(iItem,m_iColState,todo.state);

	m_listTodo.SetItemText(iItem,m_iColRemark,todo.strRemark.c_str());
}

LRESULT DialogToDo::OnLvnDbClick( int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/ )
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iItem<0)
	{
		return 0;
	}
	return EditTodo();
}

LRESULT DialogToDo::EditTodo()
{
	CListArray < int > aSelectedItems;
	m_listTodo.GetSelectedItems(aSelectedItems);
	if (aSelectedItems.IsEmpty())
	{
		MessageBox("��ѡ����Ҫ�༭�Ĵ������");
		return S_OK;
	}

	DWORD id;
	ATLVERIFY(m_listTodo.GetItemData(aSelectedItems[0],id));
	CDialogTodoDetail dlg(id);
	dlg.DoModal();
	//ReloadTodos();
	UpdateItem(aSelectedItems[0]);
	return S_OK;
}

LRESULT DialogToDo::OnLvnRClick( int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/ )
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->iItem==-1)	//���б�ͷ
	{
		//////////////////////////////////////////////////////////////////////////
		//ȷ����ǰ���������һ��
		CPoint pt;
		GetCursorPos(&pt);
		CPoint ptClient = pt;
		m_listTodo.ScreenToClient(&ptClient);
		int nSubItem;
		UINT nColFlags;
		if(!m_listTodo.HitTestHeader(ptClient,nSubItem,nColFlags))
		{
			return S_OK;
		}

		//////////////////////////////////////////////////////////////////////////
		//���ݲ�ͬ���У���ʾ��ͬ�Ĳ˵�����������Ӧ�˵�
		//////////////////////////////////////////////////////////////////////////
		CListArray<CString> listData;
		if (nSubItem==m_iColState)
		{
			if(ShowColMenu(pt,m_aListState,m_vecStateShow))
			{
				ReloadTodos();
			}
		}
		else if (nSubItem==m_iColPriority)
		{
			if(ShowColMenu(pt,m_aListPriority,m_vecPriorityShow))
			{
				ReloadTodos();
			}
		}
		else
		{
			return S_OK;
		}
	}

	
	return S_OK;
}

bool DialogToDo::ShowColMenu(const CPoint &pt, const CListArray<CString> &colData, std::vector<BOOL> &vecSelect )
{
	CMenu menu;
	menu.CreatePopupMenu();
	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON;
	const UINT BASE_ID_COLUMN_MENU = 15000;
	for(int i=0;i<colData.GetSize();i++)
	{
		menu.AppendMenu(TPM_LEFTALIGN,BASE_ID_COLUMN_MENU+i,colData[i]);
		if(vecSelect[i])
		{
			menu.CheckMenuItem(BASE_ID_COLUMN_MENU+i,MF_BYCOMMAND | MF_CHECKED);
		}
		else
		{
			menu.CheckMenuItem(BASE_ID_COLUMN_MENU+i,MF_BYCOMMAND | MF_UNCHECKED);
		}

	}
	//menu.AppendMenu(TPM_LEFTALIGN,6,"bbb");
	int iMenuId = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY,pt.x,pt.y,m_hWnd);
	if (iMenuId<=0)
	{
		return false;
	}
	int index = iMenuId-BASE_ID_COLUMN_MENU;
	ATLASSERT(index>=0);
	vecSelect[index] = !vecSelect[index];
	return true;
}
LRESULT DialogToDo::OnBnClickedAddRefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ReloadTodos();
	return 0;
}

LRESULT DialogToDo::OnBnClickedAddShowrecycle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//MessageBox("�����ܽ�����һ�汾ʵ�֣������ע��\r\n\r\n�����Ը��������Ŀ�����¼ http://code.google.com/p/greentimer/����������ϵ","��Ǹ");
	CDialogToDoHistory dlg;
	dlg.DoModal();
	return 0;
}

LRESULT DialogToDo::OnBnClickedAddShowstatics(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MessageBox("�����ܽ�����һ�汾ʵ�֣������ע��\r\n\r\n�����Ը��������Ŀ�����¼ http://code.google.com/p/greentimer/����������ϵ","��Ǹ");

	return 0;
}

void DialogToDo::UpdateCounts()
{
	CWindow wndCount = GetDlgItem(IDC_STA_TASKCOUNT);
	int iFinished,iPlaned, iWorking;
	if(g_todoSet.GetStatic(iFinished,iPlaned, iWorking))
	{
		CString strTasks;
		strTasks.Format("%d ����������ɣ�%d ��������Ҫ����",iFinished,iPlaned+iWorking);
		wndCount.SetWindowText(strTasks);
	}
}