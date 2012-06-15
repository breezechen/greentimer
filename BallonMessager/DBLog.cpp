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

std::string DBLog::ReadLog( int offset,int count )
{
	string strRes = "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=GBK\"></head><body>";

	//�����ݿ�
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateLogTable(dbTask);
	CppSQLite3Buffer strSql;

	try{
		int total = 0;
		{

			strSql.format("select count(*) from T_Log where type=%d",1);
			CppSQLite3Query q = dbTask.execQuery(strSql);
			total = q.getIntField(0);
			strRes += (string)"(" + GlobeFuns::Int2Str(total) + ")";
			int pages = total/count + 1;
			for(int i=0;i<pages;i++){
				strRes += (string)"<a href='/api/readlog?offset=" + GlobeFuns::Int2Str(i*count) + "&count=" + GlobeFuns::Int2Str(count) + "'"  + ">��" + GlobeFuns::Int2Str(i+1) + "ҳ</a> ";
			}
		}


		strSql.format("select * from T_Log where type=%d order by time_log desc limit %d,%d;",1,offset,count);
		//strSql.format("select * from T_Log order by time_log desc limit 100;");
		CppSQLite3Query q = dbTask.execQuery(strSql);
		if (q.eof())
		{
			strRes += "<br/>(None)";
		}

		while (!q.eof())
		{
			//time,modal,type,level,param2,text);

			//"modal char[128],"
			//	"time_log integer, "		//ʱ��
			//	"type integer, "		//״̬
			//	"value1 integer, "		//ֵ1
			//	"value2 integer, "		//ֵ2
			//	"message char[1024]);"	//��ʾ���

			int type = q.getIntField("type");
			CTime time = GlobeFuns::IntToTime(q.getIntField("time_log"));
			string modal = q.getStringField("modal");
			string level = GlobeFuns::Int2Str(q.getIntField("value1"));
			string text = q.getStringField("message");

			strRes += (string)"<br/>" + GlobeFuns::TimeToString(time).GetBuffer(0) + " " + modal + " " + level + " " + text;

			q.nextRow();
		}

	}
	catch(CppSQLite3Exception &exp)
	{
		exp;
		strRes += (string)"(" + exp.errorCodeAsString(exp.errorCode()) + ")" + exp.errorMessage();
		strRes += (string)"<body></html>";
		return strRes;
	}
	strRes += (string)"<body></html>";
	return strRes;
}

const char *LOG_CONST::MODAL_MAIN = "MAIN";
const char *LOG_CONST::MODAL_NOTIFY = "NOTIFY";
const char *LOG_CONST::MODAL_TODO = "TODO";
