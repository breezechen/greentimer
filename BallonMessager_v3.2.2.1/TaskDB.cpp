#include "stdafx.h"
#include ".\taskdb.h"
#include <ATLComTime.h>
#include <algorithm>

#include "SQLite/CppSQLite3.h"
#include "GlobeFuns.h"
#include "Globe.h"
#include "DBLog.h"

TaskDB g_TaskDB;


bool WriteTaskToDB(CppSQLite3DB &dbTask, const ITask &task)
{
	ATL::CString strTime = GlobeFuns::TimeToString(CTime::GetCurrentTime());
	CppSQLite3Buffer strSql;

	//"id integer PRIMARY KEY AUTOINCREMENT, "	//����Ψһid
	//	"type integer, "			//���е�type�ֶ���ITask::TaskType���塣
	//	"time char[32], "			//����ʱ�䡣ʱ����"��-��-�� ʱ:��:��"�����ĸ�ʽ���棬��ͬ
	//	"last_run_time char[32], "	//���ִ��ʱ��
	//	"task_create_time char[32], "	//���񴴽�ʱ��
	//	"tip char[2048] );"			//��ʾ���
	strSql.format("insert into T_task values(NULL,%d, '%q','%q','%q','%q');",
		task.Type,
		GlobeFuns::TimeToString(task.TaskTime),
		GlobeFuns::TimeToString(task.LastRunTime),
		GlobeFuns::TimeToString(task.CreateTime),
		task.Tip
		);
	try{
		dbTask.execDML(strSql);
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

TaskDB::TaskDB(void)
{
}

TaskDB::~TaskDB(void)
{
}

bool CompTaskID(const ITask &taskLeft,const ITask &taskRight)
{
	return taskLeft.Id<taskRight.Id;
}
bool TaskDB::ReadFromDB( const char *strDB )
{
	LOCK_THIS_RANGE(m_cs);

	//������
	CppSQLite3DB dbTask;
	dbTask.open(strDB);

	//�����񲻴��ڣ���˵�������ݿ��ʽ����
	if (!dbTask.tableExists("T_task"))                //�����¼���־��
	{
		//////////////////////////////////////////////////////////////////////////
		//����Ĭ�ϵĳ�ʼ����
		//0-55,"ע�����壬����Ϣ��
		//1-55,"ע�����壬����Ϣ��
		//2-55  = ע�����壬����Ϣ��
		//3-55  = ע�����壬����Ϣ��
		//4-55  = ע�����壬����Ϣ��
		//5-55  = ���Ϻã��������
		//6-55  = ���Ϻá�
		//7-55  = ���Ϻá�
		//8-55  = �ϰ��ˣ��ȶ��������ƻ�
		//9-55  = �г���Ϣһ����ɡ�
		//10-55 = ��������Сʱ�ˣ�����Щ�˶���!
		//11-55 = �°��ˣ��ú���Ϣ��
		//12-55 = ������Ϣһ�£������Ǹ����ı�Ǯ
		//13-55 = �����ƻ���������ʲô��
		//14-55 = �����ȿ�ˮ����������������һ��
		//15-55 = ��Ϣһ�¡�
		//16-55 = ��Ϣһ�¡�
		//17-55 = �°�ʱ�䵽������һ����ͷ�Ĺ���������������ؼҰɡ�
		//18-55 = �ڹ������������֣���������Ŷ��
		//19-55 = �ڹ������������֣�����ڼҾͳ�ȥɢɢ���ɡ�
		//20-55 = ��Щ���飬�ǿ�����������������
		//21-55 = �ܽ�һ�½���Ĺ�����дһ���ռǣ�׼����Ϣ����
		//22-55 = ҹ�ˣ����ƣ�˯����
		//23-55 = ע�����壬����Ϣ��
		AddDailyTask(0,55,"ע�����壬����Ϣ��");
		AddDailyTask(1,55,"ע�����壬����Ϣ��");
		AddDailyTask(2,55,"ע�����壬����Ϣ��");
		AddDailyTask(3,55,"ע�����壬����Ϣ��");
		AddDailyTask(4,55,"ע�����壬����Ϣ��");
		AddDailyTask(5,55,"���Ϻã��������");
		AddDailyTask(6,55,"���Ϻã��������");
		AddDailyTask(7,55,"���Ϻã��������");
		AddDailyTask(8,55,"�ϰ��ˣ��ȶ��������ƻ�");
		AddDailyTask(9,55,"�г���Ϣһ����ɡ�");
		AddDailyTask(10,55,"��������Сʱ�ˣ�����Щ�˶���!");
		AddDailyTask(11,55,"�°��ˣ��ú���Ϣ��");
		AddDailyTask(12,55,"������Ϣһ�£������Ǹ����ı�Ǯ");
		AddDailyTask(13,55,"�����ƻ���������ʲô��");
		AddDailyTask(14,55,"�����ȿ�ˮ����������������һ��");
		AddDailyTask(15,55,"��Ϣһ�¡�");
		AddDailyTask(16,55,"��Ϣһ�¡�");
		AddDailyTask(17,55,"�°�ʱ�䵽������һ����ͷ�Ĺ���������������ؼҰɡ�");
		AddDailyTask(18,55,"�ڹ������������֣���������Ŷ��");
		AddDailyTask(19,55,"�ڹ������������֣�����ڼҾͳ�ȥɢɢ���ɡ�");
		AddDailyTask(20,55,"��Щ���飬�ǿ�����������������");
		AddDailyTask(21,55,"�ܽ�һ�½���Ĺ�����дһ���ռǣ�׼����Ϣ����");
		AddDailyTask(22,55,"ҹ�ˣ����ƣ�˯����");
		AddDailyTask(23,55,"ע�����壬����Ϣ��");

		CTime tmTime = CTime::GetCurrentTime();
		tmTime += CTimeSpan(100,0,0,0);
		AddTimeTask(tmTime,"��ʹ�á���ɫʱ������Ѿ�100���ˣ���л�������ǵ�֧�֣�"
			"���ڿ������°汾������ http://code.google.com/p/greentimer/ ������");
		//////////////////////////////////////////////////////////////////////////
		return true;
	}
	CppSQLite3Query q = dbTask.execQuery("select id, type, time, last_run_time, task_create_time, tip from T_task");
	while (!q.eof())
	{
		ITask task;
		task.Id = q.getIntField("id");
		task.Type = (ITask::TaskType)q.getIntField("type");
		task.TaskTime = GlobeFuns::StringToTime(q.getStringField("time"));
		task.LastRunTime = GlobeFuns::StringToTime(q.getStringField("last_run_time"));
		task.CreateTime = GlobeFuns::StringToTime(q.getStringField("task_create_time"));
		task.Tip = q.getStringField("tip");

		m_vecTask.push_back(task);

		q.nextRow();
	}
	//���ݱ��밴ID�ź�˳�򣬷����������������ʱ��ID�ſ��ܲ���ȷ��
	sort(m_vecTask.begin(),m_vecTask.end(),CompTaskID);
	return true;
}

bool TaskDB::ReadFromDB()
{
	return g_TaskDB.ReadFromDB(Globe::GetDBPath().c_str());
}
bool TaskDB::SaveToDB( const char *strDB )
{
	LOCK_THIS_RANGE(m_cs);
	//������
	CppSQLite3DB dbTask;
	dbTask.open(strDB);

	dbTask.execDML("begin;");                                 //��ʼ��������

	//�����񲻴��ڣ��򴴽����
	if (!dbTask.tableExists("T_task"))                //�����¼���־��
	{
		//���ݿ��ֶΣ�����id���������ͣ�����ʱ�䣬�ϴ���ʾʱ�䣬��ʾ���ȡ�
		//last_run_time������������ȷ����ʾ�Ƿ��Ѿ�ִ�У������ظ�
		dbTask.execDML("Create table T_task("
			"id integer PRIMARY KEY AUTOINCREMENT, "	//����Ψһid
			"type integer, "			//���е�type�ֶ���ITask::TaskType���塣
			"time char[32], "			//����ʱ�䡣ʱ����"��-��-�� ʱ:��:��"�����ĸ�ʽ���棬��ͬ
			"last_run_time char[32], "	//���ִ��ʱ��
			"task_create_time char[32], "	//���񴴽�ʱ��
			"tip char[2048] );"			//��ʾ���
			);

		////Ϊ�����ֶν�������
		//dbTask.execDML("create index idx_type on T_task(type);");
		//LOG_NOTIFY(LOG_CONST::MNV_CREATE_TABLE,-1,"�������ѱ�");
	}
	else
	{
		//��վ�����
		dbTask.execDML("delete from T_task;");
	}

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(!WriteTaskToDB(dbTask,*it))
		{
			dbTask.execDML("rollback transaction;");
			return false;
		}
	}

	dbTask.execDML("end;");                                    //�������������ύ����
	return true;
}

bool TaskDB::SaveToDB()
{
	return g_TaskDB.SaveToDB(Globe::GetDBPath().c_str());
}
bool TaskDB::AddTask(const ITask &task )
{
	LOCK_THIS_RANGE(m_cs);

	int id = 0;
	if (!m_vecTask.empty())
	{
		id = m_vecTask.back().Id + 1;
	}
	m_vecTask.push_back(task);
	m_vecTask.back().Id = id;

	LOG_NOTIFY(LOG_CONST::MNV_ADD,id,"�ɹ�:���Ӷ�ʱ����");
	return true;
}

bool TaskDB::RemoveTask( int idTask )
{
	LOCK_THIS_RANGE(m_cs);
	LOG_NOTIFY(LOG_CONST::MNV_DEL,idTask,"ɾ����ʱ����");

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(idTask==(*it).Id)
		{
			m_vecTask.erase(it);
			return true;
		}
	}
	return false;
}

void TaskDB::GetTaskList( std::vector<int> &vecIdOut )
{
	LOCK_THIS_RANGE(m_cs);

	vecIdOut.clear();
	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		vecIdOut.push_back((*it).Id);
	}
}

bool TaskDB::GetTask(int idTask,ITask &task)
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(idTask==(*it).Id)
		{
			task = *it;
			return true;
		}
	}
	return false;
}

bool TaskDB::FindTaskRunNow(ITask &task)
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if ((*it).IsTimeNow())
		{
			task = *it;
			return true;
		}
	}
	return false;
}

bool TaskDB::AddDailyTask( int iHour, int iMinute, ATL::CString strMessage )
{
	ITask task;
	task.Type = ITask::TT_DAILY;
	task.CreateTime = CTime::GetCurrentTime();
	task.TaskTime = CTime(2000,1,1,iHour,iMinute,0);
	task.LastRunTime = 0;
	task.Tip = strMessage;
	return AddTask(task);
}

bool TaskDB::AddTimeTask( const CTime &tmRuntime, ATL::CString strMessage )
{
	ITask task;
	task.Type = ITask::TT_ONCE;
	task.CreateTime = CTime::GetCurrentTime();
	task.TaskTime = tmRuntime;
	task.LastRunTime = 0;
	task.Tip = strMessage;
	return AddTask(task);
}
bool TaskDB::UpdateTask( const ITask &task )
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(task.Id==(*it).Id)
		{
			*it = task;
			return true;
		}
	}
	return false;
}
// ���������ͼ�ִ��ʱ���ѯ����
bool TaskDB::GetTask(CTime taskTime, int taskType, ITask &task)
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		if(taskTime == (*it).TaskTime &&
			taskType == (*it).Type
			)
		{
			task = *it;
			return true;
		}
	}
	return false;
}

// ͨ��ʱ������Ͳ�ѯ����,ʱ�������ַ�����ʾ
bool TaskDB::GetTask(ATL::CString strHour, ATL::CString strMin, int taskType, ITask & task)
{
	int iHour = atoi(strHour);
	int iMin = atoi(strMin);

	return GetTask(iHour, iMin, taskType, task);
}

// ͨ��ʱ���ѯ����ʱ������int��ʽ��ʾ
bool TaskDB::GetTask(int iHour, int iMin, int taskType, ITask & task)
{
	LOCK_THIS_RANGE(m_cs);

	std::vector<ITask>::iterator it = m_vecTask.begin();
	for (;it!=m_vecTask.end();it++)
	{
		ITask theTask;
		theTask = *it;

		CTime taskTime = theTask.TaskTime;

		if(iHour == taskTime.GetHour() &&
			iMin == taskTime.GetMinute() &&
			taskType == theTask.Type
			)
		{
			task = *it;
			return true;
		}
	}
	return false;
}
