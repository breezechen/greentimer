#include "StdAfx.h"
#include "MainDlg.h"
#include <atltime.h>
#include <string>

// ȡ�ó������е�Ŀ¼���Է�б��\��β��
WTL::CString GetAppDirectory(bool bEndWithBackSlash=true)
{
	//ȡϵͳ·��
	char buf[_MAX_PATH];
	::GetModuleFileName(NULL,buf,_MAX_PATH);
	int iIndex = (int)std::string(buf).rfind('\\');
	if(bEndWithBackSlash)
	{
		buf[iIndex+1]='\0';	//�ضϣ����·����(��������'\')
	}
	else
	{
		buf[iIndex]='\0';	//�ضϣ����·����(ȥ������'\')
	}
	return (char *)buf;
}

LRESULT CMainDlg::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//GetPrivateProfileString(strSection,strKey,"COM3",buffer,sizeof(buffer),strFileName);  
	//���㱨ʱ���ܡ�
	CTime tm = CTime::GetCurrentTime();
	int iNowHour = tm.GetHour();
	if (iNowHour!=_iLastRemindHour)
	{
		WTL::CString strFileName = GetAppDirectory()+"tips.ini";
		char *strDefaultTips = "��������ʱ�䣬������Ч���ˡ�";
		char buffer[2*1024];
		char key[16];
		itoa(iNowHour,key,10);
		GetPrivateProfileString("tips",key,strDefaultTips,buffer,sizeof(buffer),strFileName);

		WTL::CString strNow;
			strNow.Format("����ʱ�䣺%d�㡣\r\n\r\n",iNowHour);
		BalloonToolTips(strNow + (char *)buffer);
		_iLastRemindHour = iNowHour;
	}
	
	return 0;
}