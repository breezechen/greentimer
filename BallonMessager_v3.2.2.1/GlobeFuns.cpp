#include "stdafx.h"
#include ".\globefuns.h"

#include <ATLComTime.h>

using namespace ATL;

// ȡ�ó������е�Ŀ¼���Է�б��\��β��
std::string GlobeFuns::GetAppDirectory( bool bEndWithBackSlash/*=true*/ )
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

//////////////////////////////////////////////////////////////////////////
//ʱ�����ַ�������ת������
ATL::CString GlobeFuns::TimeToString(const CTime &t)
{
	ATL::CString strDateTime;
	strDateTime.Format("%d-%02d-%02d %02d:%02d:%02d", 
		t.GetYear(), t.GetMonth(), t.GetDay(),
		t.GetHour(), t.GetMinute(), t.GetSecond());
	return strDateTime;
}

ATL::CString GlobeFuns::TimeToFriendlyString(const CTime &t)
{
	CTimeSpan tmSpan = CTime::GetCurrentTime() - t;
	CString strRet;
	if (tmSpan.GetTotalMinutes()<1)
	{
		return "����";
	}
	//if (tmSpan.GetTotalMinutes()<10)
	//{
	//	strRet.Format("%d����ǰ",tmSpan.GetTotalMinutes());
	//	return strRet;
	//}
	if (tmSpan.GetTotalMinutes()<60)
	{
		strRet.Format("%d����ǰ",tmSpan.GetTotalMinutes());
		return strRet;
	}
	if (tmSpan.GetTotalHours()<24)
	{
		strRet.Format("%dСʱǰ",tmSpan.GetTotalHours());
		return strRet;
	}

	strRet.Format("%d��ǰ",tmSpan.GetTotalHours()/24);
	return strRet;
}
ATL::CTime GlobeFuns::StringToTime(const ATL::CString &strTime)
{
	COleDateTime   tm;  
	tm.ParseDateTime(strTime);  
	SYSTEMTIME   st;  
	tm.GetAsSystemTime(st);  
	return CTime(st);   
}

UINT GlobeFuns::TimeToInt( const CTime &t )
{
	CTimeSpan ts = t-CTime(0);
	return (UINT)ts.GetTotalSeconds();
}

CTime GlobeFuns::IntToTime(UINT uiTime)
{
	return CTime(0)+CTimeSpan(0,0,0,uiTime);
}
//////////////////////////////////////////////////////////////////////////
