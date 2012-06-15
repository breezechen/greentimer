#include ".\uploadhandler.h"
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;


// ȡ�ó������е�Ŀ¼���Է�б��\��β��
static std::string GetAppDirectory( bool bEndWithBackSlash/*=true*/ )
{
	//ȡϵͳ·��
	char buf[_MAX_PATH];
	::GetModuleFileName(NULL,buf,_MAX_PATH);
	int iIndex = (int)std::string(buf).rfind('\\');
	if(bEndWithBackSlash)
	{
		buf[iIndex+1]='\0';	//�ضϣ����·����(��������'\')
	}
	else
	{
		buf[iIndex]='\0';	//�ضϣ����·����(ȥ������'\')
	}
	return (char *)buf;
}


UploadHandler::UploadHandler()
{
}

UploadHandler::~UploadHandler(void)
{
}

void UploadHandler::handle( SP_HttpRequest * request, SP_HttpResponse * response )
{
	string url = request->getURL();
	if(url=="/")
	{
		url = "/index.htm";
	}
	std::replace(url.begin(),url.end(),'/','\\');
	
	//const char *full_path = request->getParamValue("path");
	string full_path = GetAppDirectory(false) + url;
	
	//////////////////////////////////////////////////////////////////////////
	//���ļ�
	//////////////////////////////////////////////////////////////////////////
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if (!is)
	{
		ReturnNotFound(response);
		return;
	}

	is.seekg(0);
	if(is.eof() || is.fail())  //fuck,seekg��Ȼ��������false������
	{
		ReturnNotFound(response);
		return;
	}

	//����������
	//unsigned int iLeftSize = iSize;
	while (true)
	{
		int iReadSize = is.read(m_pBuffer, c_iBUFFER_SIZE).gcount();
		//iLeftSize -= iReadSize;
		if(iReadSize <= 0)
		{
			break;
		}
		response->appendContent(m_pBuffer, iReadSize);
	}
}

void UploadHandler::ReturnNotFound( SP_HttpResponse * response)
{
	const char *strEmptyMsg = "Empty!";
	//�������ݰ�
	response->appendContent(strEmptyMsg,strlen(strEmptyMsg));
}