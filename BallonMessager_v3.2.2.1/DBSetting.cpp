#include "stdafx.h"
#include "DBSetting.h"
#include "SQLite/CppSQLite3.h"
#include "GlobeFuns.h"

using namespace std;

bool CheckCreateSettingTable(CppSQLite3DB &dbTask)
{
	try{
		//�����񲻴��ڣ��򴴽����
		if (!dbTask.tableExists("T_Setting"))                //�����¼���־��
		{
			//���ݿ��ֶΣ�����id���������ͣ�����ʱ�䣬�ϴ���ʾʱ�䣬��ʾ���ȡ�
			//last_run_time������������ȷ����ʾ�Ƿ��Ѿ�ִ�У������ظ�
			dbTask.execDML("Create table T_Setting("
				"key integer , "		//key
				"value char[1024]);"	//value
				);

			//Ϊ�����ֶν�������
			dbTask.execDML("create index idx_id on T_Setting(key);");
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


DBSetting::DBSetting(const std::string &strDBName,const std::string &strTableName)
{
	m_strDB = strDBName;
	m_strTableName = strTableName;
}

DBSetting::~DBSetting(void)
{
}
bool DBSetting::SetValue(long lKey,long lValue)
{
	char buf[16];
	return SetValue(lKey,itoa(lValue,buf,10));
}
bool DBSetting::GetValue(long lKey,long &lValue)
{
	string strVal;
	if(!GetValue(lKey,strVal))
	{
		return false;
	}
	lValue = atoi(strVal.c_str());
	return true;
}
bool DBSetting::SetValue(long lKey,const std::string &strValue)
{
	//�����ݿ�
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateSettingTable(dbTask);

	CppSQLite3Buffer strSql;

	try{
		strSql.format("select count(*) from T_Setting where key=%d;",lKey);
		CppSQLite3Query q = dbTask.execQuery(strSql);
		if (0==q.getIntField(0))
		{
			strSql.format("insert into T_Setting(key,value) values(%d,'%q');",lKey,strValue.c_str());
			if(1!=dbTask.execDML(strSql))
			{
				ATLASSERT(FALSE);
				return false;
			}
		}
		else
		{
			strSql.format("update T_Setting set value = '%q' where key=%d;",strValue.c_str(),lKey);
			if(1!=dbTask.execDML(strSql))
			{
				ATLASSERT(FALSE);
				return false;
			}
		}

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
bool DBSetting::GetValue(long lKey,std::string &strValue)
{
	//�����ݿ�
	CppSQLite3DB dbTask;
	dbTask.open(m_strDB.c_str());
	CheckCreateSettingTable(dbTask);
	CppSQLite3Buffer strSql;

	try{
		strSql.format("select value from T_Setting where key=%d;",lKey);
		CppSQLite3Query q = dbTask.execQuery(strSql);
		if (q.eof())
		{
			return false;
		}
		strValue = q.getStringField(0);
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