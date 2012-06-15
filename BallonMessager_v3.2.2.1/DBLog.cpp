#include "stdafx.h"
#include ".\dblog.h"
#include "SQLite/CppSQLite3.h"
#include "GlobeFuns.h"
#include "Globe.h"

DBLog g_DBLog;

bool CheckCreateLogTable(CppSQLite3DB &dbTask)
{
	try{
		//�����񲻴��ڣ��򴴽����
		if (!dbTask.tableExists("T_log"))                //�����¼���־��
		{
			//���ݿ��ֶΣ�����id���������ͣ�����ʱ�䣬�ϴ���ʾʱ�䣬��ʾ���ȡ�
			//last_run_time������������ȷ����ʾ�Ƿ��Ѿ�ִ�У������ظ�
			dbTask.execDML("Create table T_log("
				"id integer PRIMARY KEY AUTOINCREMENT, "	//Ψһid
				"modal char[128],"
				"time_log integer, "		//ʱ��
				"type integer, "		//״̬
				"value1 integer, "		//ֵ1
				"value2 integer, "		//ֵ2
				"message char[1024]);"	//��ʾ���
				);

			//Ϊ�����ֶν�������
			dbTask.execDML("create index idx_time on T_log(time_log);");
		}
		return true;
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return false;
	}
}

DBLog::DBLog(void)
{
	m_strDB = Globe::GetDBPath();
}

DBLog::~DBLog(void)
{
}

void DBLog::Log( const char *pModel,int code,int value1,int value2, const char *pMessage )
{
	//�����ݿ�
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateLogTable(dbTask);

	CppSQLite3Buffer strSql;
	strSql.format("insert into T_Log values(NULL,'%q',%d,%d,%d,%d,'%q');",
		pModel,GlobeFuns::TimeToInt(CTime::GetCurrentTime()),code,value1,value2,pMessage
		);

	try{
		if(1!=dbTask.execDML(strSql))
		{
			ATLASSERT(FALSE);
			return;
		}
	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		ATLTRACE("error:%s\n",exp.errorMessage());
		ATLASSERT(FALSE);
		return;
	}
}


const char *LOG_CONST::MODAL_MAIN = "MAIN";
const char *LOG_CONST::MODAL_NOTIFY = "NOTIFY";
const char *LOG_CONST::MODAL_TODO = "TODO";
