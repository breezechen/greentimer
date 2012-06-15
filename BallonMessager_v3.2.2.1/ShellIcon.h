#pragma once
#define WM_ICON WM_USER + 180
#define NM_ICON_INFO WM_ICON + 1
#ifndef NIIF_USER
#define NIIF_USER 0x4
#endif
static ATL::CString g_childInfo;
template <class T, class MenuT = CIconMenu<T>, int MenuID = IDR_ICONMENU>
class CShellIcon : public MenuT
{
private:
       NOTIFYICONDATA m_data;
       UINT m_msgTaskbarRestart;
	   ATL::CString m_appName;
public:
       CShellIcon()
       {
              //m_appName.LoadString(IDS_APPNAME);
		   m_appName = _T("��ɫʱ�����");
            m_msgTaskbarRestart = RegisterWindowMessage(TEXT("TaskbarCreated"));
       }

       ~CShellIcon()
       {
              Shell_NotifyIcon(NIM_DELETE, &m_data);
       }
       BOOL CreateShellIcon()
       {
              T* pT = static_cast<T*>(this);
              SecureZeroMemory(&m_data, sizeof(m_data));
              m_data.cbSize = sizeof(m_data);
              m_data.hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
              IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
              m_data.hWnd = pT->m_hWnd;
              m_data.uID = IDR_MAINFRAME;
              m_data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_INFO | NIF_TIP;
              m_data.uCallbackMessage = WM_ICON;
              m_data.dwInfoFlags = NIIF_USER;
			  m_data.uTimeout = 10*1000;	//������ʾ�����ʱ�䣬10�롣����ϵͳ�涨��̵�ʱ��10�롣
              //strcpy_s(m_data.szInfoTitle,64, m_appName);
              //strcpy_s(m_data.szTip, m_appName);
			  strcpy(m_data.szInfoTitle, m_appName);
			  strcpy(m_data.szTip, m_appName);
              return Shell_NotifyIcon(NIM_ADD, &m_data);
       }
 
       void ModifyToolTips(LPCTSTR info)
       {
              //strcpy_s(m_data.szInfo, info);
		   strcpy(m_data.szInfo, info);
       }
 
       BOOL DispalyToolTips()
       {
              return Shell_NotifyIcon(NIM_MODIFY, &m_data);
       }

       BOOL BalloonToolTips(LPCTSTR info, DWORD dwInfoFlags = NIIF_USER)
       {
              ModifyToolTips(info);
              m_data.dwInfoFlags = dwInfoFlags;
              return DispalyToolTips();
       }


       BEGIN_MSG_MAP(CShellIcon)
              MESSAGE_HANDLER(WM_ICON, OnIcon)
              MESSAGE_HANDLER(m_msgTaskbarRestart, OnRestart)
              MESSAGE_HANDLER(WM_SIZE, OnSize)
              NOTIFY_CODE_HANDLER(NM_ICON_INFO, OnIconInfo)
              CHAIN_MSG_MAP(MenuT)
       END_MSG_MAP()

       LRESULT OnIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
       {
              T* pT = static_cast<T*>(this); char t;
              if (wParam != IDR_MAINFRAME) return 1;
              switch(lParam)
              {
              case WM_RBUTTONUP:

                     t = *m_data.szInfo;

                     *m_data.szInfo = '\0';

                     Shell_NotifyIcon(NIM_MODIFY, &m_data);

                     pT->CreateContextMenu(MenuID);

                     *m_data.szInfo = t;

                     break;

              case WM_LBUTTONUP:

                     //pT->ShowWindow(SW_SHOW);
					 pT->OpenTodo();

                     OpenIcon(pT->m_hWnd);

                     break;

//ȥ�������ע�Ϳ���ʹ�����ͣ��ͼ����ʱ����������ʾ�����˲�̫ϲ��

              case WM_MOUSEMOVE:

       //            DispalyToolTips(); break;

              default:

                     ;

              }

              return 0;

       }

       LRESULT OnIconInfo(int wParam, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
       {
              BalloonToolTips(g_childInfo, wParam);
              g_childInfo.Empty();
              return 0;

       }

 

//����Explorer��Ǳ������������ؽ�����ᷢ�ֺܶ����û�����������Explorerһ����

//ͼ����Ҳ����ˣ������С�������������ң�����������������رգ����������û������ԣ�ϣ��������

 

       LRESULT OnRestart(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)

       {

              T* pT = static_cast<T*>(this);

              SecureZeroMemory(&m_data, sizeof(m_data));

              m_data.cbSize = sizeof(m_data);

              m_data.hWnd = pT->m_hWnd;

              m_data.uID = IDR_MAINFRAME;

              Shell_NotifyIcon(NIM_DELETE, &m_data);

              CreateShellIcon();

              return 0;

       }

 

       LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)

       {

              T* pT = static_cast<T*>(this);

              if (pT->IsIconic()) pT->ShowWindow(SW_HIDE);

              return 0;

       }

 

};
