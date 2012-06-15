#include "StdAfx.h"
#include ".\tselfstart.h"

using namespace std;

TSelfStart::TSelfStart(const std::string &strName,const std::string &strPath)
{
	m_strPath = strPath;
	m_strName = strName;
}

TSelfStart::~TSelfStart(void)
{
}

bool TSelfStart::SetSelfStart()
{
	HKEY   hRegKey;
	string str="Software\\Microsoft\\Windows\\CurrentVersion\\Run";  
	if(RegOpenKey(HKEY_CURRENT_USER,   str.c_str(),   &hRegKey)   ==   ERROR_SUCCESS)
	{
		if(::RegSetValueEx(   hRegKey,  
			m_strName.c_str(),  
			0,  
			REG_SZ,  
			(const BYTE *)m_strPath.data(),  
			m_strPath.size()   )   ==   ERROR_SUCCESS)
		{
			RegCloseKey(hRegKey);
			return true;
		}
		RegCloseKey(hRegKey);
	}
    MessageBox(NULL,"������ͼΪ���趨������ע�����Ŀ��û�гɹ���\n�����������Ȩ�޲�����ɵġ�","�趨������ʧ��!",MB_OK);
	return false;
}

bool TSelfStart::IsSelfStart()
{
	HKEY   hRegKey;
	HKEY   hRegKey1;
	string str="Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	if(RegOpenKey(HKEY_LOCAL_MACHINE,   str.c_str(),   &hRegKey1)  ==   ERROR_SUCCESS)
	{
		//�����ù��ɰ汾,Ҫ�Ѿɵ�ע�����Ŀɾ��,�������������ʵ��
		char buf1[1024];
		DWORD ibufSize1 = sizeof(buf1);
		DWORD iType1 = REG_SZ;
		if(ERROR_SUCCESS==
			::RegQueryValueEx(hRegKey1, m_strName.c_str(), NULL,&iType1,(LPBYTE)buf1,(LPDWORD)&ibufSize1)
			)
		{
			if (m_strPath==buf1)
			{
				if(::RegDeleteValue(hRegKey1,m_strName.c_str())!=ERROR_SUCCESS)
				{
					DWORD iErr = GetLastError();
					MessageBox(NULL,"���ƺ��ù��������ľɰ汾����������ͼΪ��ɾ���ɵ���������ֵ��û�гɹ���\n������HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n��GreenTimer��ֵɾ����","�ɵ���������ֵ����ɾ��!",MB_OK);
				}
			}
		}
		RegCloseKey(hRegKey1);
	}
	if(RegOpenKey(HKEY_CURRENT_USER,   str.c_str(),   &hRegKey)   !=   ERROR_SUCCESS)
	{
		return false;
	}
	char buf[1024];
	DWORD ibufSize = sizeof(buf);
	DWORD iType = REG_SZ;
	if(ERROR_SUCCESS!=
		::RegQueryValueEx(hRegKey, m_strName.c_str(), NULL,&iType,(LPBYTE)buf,(LPDWORD)&ibufSize)
		)
	{
		RegCloseKey(hRegKey);
		return false;
	}
	RegCloseKey(hRegKey);

	//2008-12-22�գ�����ȡ�õı���·���̷�ʱ����дʱ��Сд
	//return m_strPath==buf;
	return 0==CString(buf).CompareNoCase(m_strPath.c_str());
}

bool TSelfStart::RemoveSelfStart()
{
	HKEY   hRegKey;
	string str="Software\\Microsoft\\Windows\\CurrentVersion\\Run";  
	if(::RegOpenKeyEx(HKEY_CURRENT_USER,   str.c_str(), 0, KEY_ALL_ACCESS,  &hRegKey)   !=   ERROR_SUCCESS)
	{
		return false;
	}
	if(::RegDeleteValue(hRegKey,m_strName.c_str())!=ERROR_SUCCESS)
	{
		DWORD iErr = GetLastError();
		RegCloseKey(hRegKey);
		return false;
	}
	RegCloseKey(hRegKey);
	return true;
}