#pragma once
#include "ITask.h"
#include <atlapp.h>
#include <atlmisc.h>
using namespace ATL;
#include <vector>

#include "ThreadLock.h"

//�������ݿ�
//�˶������̰߳�ȫ��
//��ͨ��g_TaskDBʹ�ô��ࡣ
class TaskDB
{
public:
	TaskDB(void);
	~TaskDB(void);

	//��Ĭ�ϵ�ַ�ָ���������
	bool ReadFromDB();
	//����������д��Ĭ���ļ���
	bool SaveToDB();
	
	//����������
	bool AddTask(const ITask &task);
	//����ÿ������
	bool AddDailyTask(int iHour, int iMinute, ATL::CString strMessage);
	//����һ��������
	bool AddTimeTask(const CTime &tmRuntime, ATL::CString strMessage);
	//ɾ������
	bool RemoveTask(int idTask);
	//����������Ϣ������task�е�ID��ȷ������
	bool UpdateTask(const ITask &task);

	//��ȡϵͳ�е������б�
	void GetTaskList(std::vector<int> &vecIdOut);
	//��ȡ�����������Ϣ
	bool GetTask(int idTask,ITask &task);
	// ���������ͼ�ִ��ʱ���ѯ����
	bool GetTask(CTime taskTime, int taskType, ITask &task);
	// ͨ��ʱ������Ͳ�ѯ����,ʱ�������ַ�����ʾ
	bool GetTask(ATL::CString strHour, ATL::CString strMin, int taskType, ITask & task);
	// ͨ��ʱ���ѯ����ʱ������int��ʽ��ʾ
	bool GetTask(int iHour, int iMin, int taskType, ITask & task);


	//��ѯ���������ʱ�̣��Ƿ���������Ҫִ�У�
	bool FindTaskRunNow(ITask &task);
private:
	CThreadLock m_cs;
	std::vector<ITask> m_vecTask;
	bool ReadFromDB(const char *strDB);
	bool SaveToDB(const char *strDB);
};

extern TaskDB g_TaskDB;
