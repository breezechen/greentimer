#pragma once

class GlobeFuns
{
public:
	// ȡ�ó������е�Ŀ¼���Է�б��\��β��
	static std::string GetAppDirectory(bool bEndWithBackSlash=true);

	//////////////////////////////////////////////////////////////////////////
	//ʱ�����ַ�������ת������
	static ATL::CString TimeToString(const CTime &t);
	static ATL::CString TimeToFriendlyString(const CTime &t);
	static CTime StringToTime(const ATL::CString &strTime);
	//////////////////////////////////////////////////////////////////////////

	static UINT TimeToInt(const CTime &t);
	static CTime IntToTime(UINT uiTime);
};
