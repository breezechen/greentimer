#include "stdafx.h"
#include ".\itask.h"

ITask::ITask(void)
{
	LastRunTime = 0;
}

ITask::~ITask(void)
{
}

bool ITask::IsTimeNow()
{

	CTime tmNow = CTime::GetCurrentTime();
	switch(Type)
	{
	case TT_DAILY:
		{
			//ÿ����������ֻҪʱ��ͷ�����ȷ���ɡ�
			if (tmNow.GetHour()==TaskTime.GetHour() 
				&& tmNow.GetMinute()==TaskTime.GetMinute())
			{
				//�����ظ����С�
				if (tmNow.GetYear()==LastRunTime.GetYear() 
					&& tmNow.GetMonth()==LastRunTime.GetMonth()
					&& tmNow.GetDay()==LastRunTime.GetDay()
					&& tmNow.GetHour()==LastRunTime.GetHour()
					&& tmNow.GetMinute()==LastRunTime.GetMinute()
					)
				{
					return false;
				}

				//OK,������ʱ��
				return true;
			}
		}
		break;
	case TT_ONCE:
		{

			//һ������������
			if (LastRunTime!=0)	//�Ѿ����й���
			{
				return false;
			}
			if (tmNow>TaskTime)	//������ʱ�仹û��ʾ���Ǿ͵���ʾ
			{
				return true;
			}
		}
	case TT_WEEKLY:
		{
			//ÿ����������
			if (tmNow.GetDayOfWeek()==GetMonthWeek(TaskTime)
				&& tmNow.GetHour()==TaskTime.GetHour() 
				&& tmNow.GetMinute()==TaskTime.GetMinute())
			{
				//�����ظ����С�
				if (tmNow.GetYear()==LastRunTime.GetYear() 
					&& tmNow.GetMonth()==LastRunTime.GetMonth()
					&& tmNow.GetDay()==LastRunTime.GetDay()
					&& tmNow.GetHour()==LastRunTime.GetHour()
					&& tmNow.GetMinute()==LastRunTime.GetMinute()
					)
				{
					return false;
				}

				//OK,������ʱ��
				return true;
			}
		}
	case TT_MONTHLY:
		{
			//ÿ����������
			if (tmNow.GetDay()==GetMonthWeek(TaskTime)
				&& tmNow.GetHour()==TaskTime.GetHour() 
				&& tmNow.GetMinute()==TaskTime.GetMinute())
			{
				//�����ظ����С�
				if (tmNow.GetYear()==LastRunTime.GetYear() 
					&& tmNow.GetMonth()==LastRunTime.GetMonth()
					&& tmNow.GetDay()==LastRunTime.GetDay()
					&& tmNow.GetHour()==LastRunTime.GetHour()
					&& tmNow.GetMinute()==LastRunTime.GetMinute()
					)
				{
					return false;
				}

				//OK,������ʱ��
				return true;
			}

		}
	}
	return false;
}

TCHAR * ITask::GetTaskTypeStr( TaskType type )
{
	switch(type)
	{
	case TT_ONCE:
		return "һ������";
	case TT_DAILY:
		return "ÿ������";
		//case TT_DATE: 
		//	return "��������";
	case TT_WEEKLY: 
		return "ÿ������";
	case TT_MONTHLY: 
		return "ÿ������";
		//case TT_YEARLY:
		//	return "ÿ������";
	default:
		return "δ֪����";
	}
	return "δ֪����";
}

ITask::TaskType ITask::GetTaskType( const string &strName )
{
	if (strName.compare("һ������")==0)
	{
		return TT_ONCE;
	}
	if (strName.compare("ÿ������")==0)
	{
		return TT_DAILY;
	}
	if (strName.compare("ÿ������")==0)
	{
		return TT_WEEKLY;
	}
	if (strName.compare("ÿ������")==0)
	{
		return TT_MONTHLY;
	}
	return (TaskType)-1;
}

void ITask::GetTaskTypes( vector<string> &vecTypes )
{
	vecTypes.clear();
	vecTypes.push_back("һ������");
	vecTypes.push_back("ÿ������");
	vecTypes.push_back("ÿ������");
	vecTypes.push_back("ÿ������");
}

bool ITask::operator == (const ITask &task)
{
	return  Id==task.Id
		&& Type==task.Type
		&& TaskTime == task.TaskTime
		&& LastRunTime == task.LastRunTime
		&& CreateTime == task.CreateTime
		&& Tip == task.Tip;	//��ʾ����
}

int ITask::GetMonthWeek(const CTime &tm)
{
	return tm.GetYear() - CTime(0).GetYear();
}

ATL::CString ITask::GetWeekDayName( int iDay )
{
	static UINT DayOfWeek[] = {
		LOCALE_SDAYNAME7,   // Sunday
		LOCALE_SDAYNAME1,   
		LOCALE_SDAYNAME2,
		LOCALE_SDAYNAME3,
		LOCALE_SDAYNAME4, 
		LOCALE_SDAYNAME5, 
		LOCALE_SDAYNAME6,   // Saturday
	};
	TCHAR strWeekday[256];

	::GetLocaleInfo(LOCALE_USER_DEFAULT,   // Get string for day of the week from system
		DayOfWeek[iDay-1],   // Get day of week from CTime
		strWeekday, sizeof(strWeekday));
	return strWeekday;
}