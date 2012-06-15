#pragma once
#include <string>
#include <set>
#include <atltime.h>

//�����¼
class ToDoTask
{
public:
	//��������ID
	enum {
		ERROR_TASKID = 0,
	};
	//�������ȼ�
	enum TaskPriority
	{
		TP_MOST_IMPORTANT = 0,
		TP_IMPORTANT = 1,
		TP_NORMAL = 2,
		TP_NOT_IMPORTANT = 3,
	};
	//����״̬
	enum TaskState
	{
		TS_NOT_START = 0,
		TS_WORKING = 1,
		TS_STOP = 2,
		TS_FINISHED = 3,
		TS_CANCEL = 4,

		//"ɾ��"״̬
		TS_DELETED = 5,
	};
	static TCHAR *PriorityText(TaskPriority priority);
	static TCHAR *StateText(TaskState state);
	static COLORREF GetStateColor(TaskState state);

	ToDoTask(void);
	ToDoTask(int taskid);
	~ToDoTask(void);
	bool operator < (const ToDoTask &task) const;

	//����id
	int id;
	//�������
	std::string strTask;
	//���ȼ�
	TaskPriority priority;
	//״̬
	TaskState state;
	//����
	std::string strRemark;

	//���񴴽�ʱ��
	CTime tmCreateTime;
	//����ʼ������ʱ��
	CTime tmStartWorkTime;

	CTime tmPlanFinshTime;

	//���������ʷ
	//std::map<CTime,std::string> mapHistory;
};

//�������ݿ�
class TodoSet
{
public:
	TodoSet();

	//bool Load();
	//bool Save();

	//bool Load(const TCHAR *strDB);
	//bool Save(const TCHAR *strDB);
	void GetTodoList(std::set<int> &taskIDs);
	void GetHistoryTodoList(std::set<int> &taskIDs);
	ToDoTask GetToDo(int id);
	bool UpdateToDo(const ToDoTask &task);
	int AddToDo();
	bool DeleteToDo(int id);
	bool ReactiveToDo(int id);
	bool DeleteToDoForever(int id);
	//��ȡͳ����Ϣ
	bool GetStatic(int &iFinished,int &iPlaned, int &iWorking);
	int GetTaskCount(int iState);	//��״̬������������iState=-1����ȡ�������������������򷵻�-1
private:
	std::string m_strDB;
private:
	bool CheckDBValid();
	void UpdateTaskFinishTime(int id,const CTime &tm);

	//��t_todo�е�����Ǩ�Ƶ�t_todo2�У�Ȼ��ɾ��t_todo��
	//����t_todo�汾��û���������Բ���Ҫ�������������
	bool RecoverDataFromV1();
};

extern TodoSet g_todoSet;

