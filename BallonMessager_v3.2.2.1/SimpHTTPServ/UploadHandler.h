#pragma once
#ifndef WIN32
#define WIN32
#endif

#include "./spserver/spporting.hpp"
#include "./spserver/spwin32iocp.hpp"
#include "./spserver/spiocpserver.hpp"
#include "./spserver/sphttp.hpp"
#include "./spserver/sphttpmsg.hpp"
#include "./spserver/spserver.hpp"

#pragma comment(lib,"ws2_32")
#pragma comment(lib,"mswsock")

//����ÿ�����ӵĻ�������С
//��ÿ�ζ����̵Ĵ�С������ֵ̫С���ܵ��¶����̴������ࡣ̫�����˷��ڴ档
const int c_iBUFFER_SIZE = 8*1024;

class UploadHandler :
	public SP_HttpHandler
{
public:
	UploadHandler();
	~UploadHandler(void);

	virtual void handle( SP_HttpRequest * request, SP_HttpResponse * response );
private:
	char m_pBuffer[c_iBUFFER_SIZE];
private:
	void ReturnNotFound(SP_HttpResponse * response);
};
