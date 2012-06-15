#pragma once
#include <string>

/**
* ��־��¼��.
*
* �����־����д�뵽task.db��ȥ
* 
*
*/
class DBLog
{
public:
	DBLog(void);
	~DBLog(void);

	/// ��¼��־.
	/// 
	/// �����ֱ��ǣ�ģ���������š��Զ���ֵ1���Զ���ֵ2����־���ݡ�
	/// �Զ���ֵ��Ϊ�˷�����������ͳ��
	void Log(const char *pModel,int code,int value1,int value2, const char *pMessage);
private:
	std::string m_strDB;
};

extern DBLog g_DBLog;
 
/*

������־��
һ�������ػ���
ģ����Ϊ"MAIN",code = 0������value1=0,�ػ�value1=1;

�������ѣ�
���ӡ�ɾ�����޸ġ���ʾ����
ģ����Ϊ"NOTIFY",code = 1��value1��ֵ�����ӡ�ɾ�����޸ġ���ʾ�ֱ�Ϊ0��1��2��3

��������
���ӡ�ɾ�����޸�����
ģ����Ϊ"TODO",code = 2��value1��ֵ�����ӡ�ɾ�����޸ķֱ�Ϊ0��1��2
*/
class LOG_CONST
{
public:
	static const char *MODAL_MAIN ;
	static const char *MODAL_NOTIFY;
	static const char *MODAL_TODO;

	enum MODEL_CODE
	{
		MC_MAIN = 0,
		MC_NOTIFY = 1,
		MC_TODO = 2,
	};
	enum MODEL_MAIN_VALUE
	{
		MMV_START = 0,
		MMV_EXIT = 1,
	};

	//value1Ϊ����ֵʱ��value2Ϊ����ID
	enum MODEL_NOFIFY_VALUE
	{
		MNV_ADD = 0,
		MNV_DEL = 1,
		MNV_MODIFY = 2,
		MNV_DISPLAY = 3,
		MNV_CREATE_TABLE = 4,
	};
	//value1Ϊ����ֵʱ��value2Ϊ�ƻ�ID
	enum MODEL_TODO_VALUE
	{
		MTV_ADD = 0,
		MTV_DEL = 1,
		MTV_MODIFY = 2,
		MTV_DISPLAY_LIST = 3,
		MTV_DISPLAY_ITEM = 4,
		MTV_CREATE_TABLE = 5,
		MTV_SETFINISH = 6,
		MTV_REACTIVE = 7,
		MTV_DELETEFOREVER = 8,
	};
};

#define LOG_MAIN(v1,v2,remark) g_DBLog.Log(LOG_CONST::MODAL_MAIN,LOG_CONST::MC_MAIN,v1,v2,remark)
#define LOG_NOTIFY(v1,v2,remark) g_DBLog.Log(LOG_CONST::MODAL_NOTIFY,LOG_CONST::MC_NOTIFY,v1,v2,remark)
#define LOG_TODO(v1,v2,remark) g_DBLog.Log(LOG_CONST::MODAL_TODO,LOG_CONST::MC_TODO,v1,v2,remark)
