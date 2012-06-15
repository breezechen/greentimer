#pragma once
#include <atltime.h>

//��������
class ITask
{
public:
	ITask(void);
	virtual ~ITask(void);

	enum TaskType{
		TT_ONCE = 0,		//������һ��
		TT_DAILY = 1,		//ÿ�����У���ȷ��Ϊ���ӣ�
		//TT_DATE = 2,		//���ض�����������һ�Σ���ȷ��Ϊ�죩
		TT_WEEKLY = 3,		//ÿ��һ��
		TT_MONTHLY = 4,		//ÿ��һ��
		//TT_YEARLY = 5,		//ÿ��һ��
	};
	static TCHAR *GetTaskTypeStr(TaskType type);
	static TaskType GetTaskType(const string &strName);
	static void GetTaskTypes(vector<string> &vecTypes);
	static ATL::CString GetWeekDayName(int iDay);
	static int GetMonthWeek(const CTime &tm);	//����ÿ�ܡ�ÿ���������������������

	ATL::CString GetTimeStr() const
	{
		if (Type==TT_ONCE)
		{
			return TaskTime.Format("%y-%m-%d %H:%M:%S");
		}
		else if (Type==TT_DAILY)
		{
			return TaskTime.Format("%H:%M");
		}
		else if (Type==TT_WEEKLY)
		{
			ATL::CString str;
			str.Format("%s %d:%d",
				GetWeekDayName(GetMonthWeek(TaskTime)),TaskTime.GetHour(),TaskTime.GetMinute());
			return str;
		}
		else if (Type==TT_MONTHLY)
		{
			ATL::CString str;
			str.Format("��%d�� %d:%d",
				GetMonthWeek(TaskTime),TaskTime.GetHour(),TaskTime.GetMinute());
			return str;
		}
		else
		{
			ATLASSERT(FALSE);
			return "����";
		}
		
	}
	ATL::CString GetIDStr() const
	{
		ATL::CString strID;
		strID.Format("%d",Id);
		return strID;
	}

	int Id;
	TaskType Type;	
	CTime TaskTime;	    //����ִ��ʱ��
	CTime LastRunTime;  //�ϴ�ִ��ʱ�䣨Ϊ0�����δִ�й���
	CTime CreateTime;	//����ʱ��
	ATL::CString Tip;	//��ʾ����

	bool IsTimeNow();
	bool operator == (const ITask &task);
};