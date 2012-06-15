#include "stdafx.h"
#include "HttpServ.h"

#include "spporting.hpp"

#include "Globe.h"
#include "KeyValueDB.h"
#include "DBLog.h"
#include "DBSetting.h"
#include "GlobeFuns.h"


#pragma comment(lib,"ws2_32")
#pragma comment(lib,"mswsock")

/*
void SP_HttpEchoHandler::handle( SP_HttpRequest * request, SP_HttpResponse * response ) {
	response->setStatusCode( 200 );

	string uri = request->getURI();

	if(uri=="/api/log"){  //д��־
		handlerLog(request,response);
		return;
	}
	else if(uri=="/api/readlog"){  //д��־
		handlerShowLog(request,response);
		return;
	}
	else if('/'==*uri.rbegin()){ //Ŀ¼
		handlerDir(request,response);
		return;
	}
	else{						//�ļ�
		handlerFile(request,response);
		return;
	}

	response->appendContent( "<html><head>"
		"<title>Welcome to simple http</title>"
		"</head><body><p>hello,world</p></body></html>" );
	return;
};


SP_HttpHandler * SP_HttpEchoHandlerFactory::create() const {
	return new SP_HttpEchoHandler();
};


HttpServ::HttpServ(void)
{
}

HttpServ::~HttpServ(void)
{
}

*/

void TServ::httpserv()
{
	/////////////��ʼ��winsock����
	WSADATA wsadata;
	WORD wVersion=MAKEWORD(2,0);/////////winsock 2.0
	if(WSAStartup(wVersion,&wsadata)!=0)
	{
		printf("initalize failed!/n");
		WSACleanup();
		exit(1);
	}

	int sock,csock,length;//sock�Ƿ������������׽��֣�csock�ǿͻ��������׽���
	length=sizeof(struct sockaddr);


	struct sockaddr_in server_ipaddr,client_ipaddr;//������÷������Ϳͻ��˵ĵ�ַ��Ϣ
	memset(&server_ipaddr,0,length);
	server_ipaddr.sin_family=AF_INET;
	server_ipaddr.sin_port=htons(80);
	server_ipaddr.sin_addr.s_addr=inet_addr("0.0.0.0");



	sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	///��
	if(bind(sock,(struct sockaddr *)&server_ipaddr,length)==SOCKET_ERROR)
	{
		printf("Bind error/n");
		WSACleanup();
		exit(1);
	}
	////����
	if(listen(sock,5)==SOCKET_ERROR)
	{
		printf("Listen error/n");
		WSACleanup();
		exit(1);
	}

	//////////
	while(1)
	{
		//////////���������Ӻ󣬲����µ��׽���
		///////////�����Ϳͻ��˴�����Ϣ
		csock=accept(sock,(struct sockaddr *)&client_ipaddr,&length);
		if(csock==SOCKET_ERROR)
		{
			printf("Listen error/n");
			WSACleanup();
			exit(1);
		}
		string strRecv;
		char buff[4096];
		int nbuff;
		while(true){
			nbuff=recv(csock,buff,4096,0);
			if (nbuff<=0)
			{
				break;
			}

			strRecv.append(buff,nbuff);
			if (strstr(strRecv.c_str(),"\r\n\r\n"))
			{
				break;
			}
		}
		TReq req(strRecv);
		TResp resp(csock);


		resp.setStatusCode( 200 );

		string uri = req.getURI();

		if(uri=="/api/log"){  //д��־
			handlerLog(&req,&resp);
			//return;
		}
		else if(uri=="/api/readlog"){  //д��־
			handlerShowLog(&req,&resp);
			//return;
		}
		else if('/'==*uri.rbegin()){ //Ŀ¼
			handlerDir(&req,&resp);
			///return;
		}
		else{						//�ļ�
			handlerFile(&req,&resp);
			//return;
		}

		resp.appendContent( "<html><head>"
			"<title>Welcome to simple http</title>"
			"</head><body><p>hello,world</p></body></html>" );
		resp.finish();
		//resp.appendContent("hello,world!\r\n");
		//resp.appendContent(req.getURI().c_str());
	}
}

TReq::TReq( const string &req )
{
	m_strReq = req;
}

std::string TReq::getURI()
{
	const char *pBgn = strstr(m_strReq.c_str(),"GET ");
	if(!pBgn){
		pBgn = strstr(m_strReq.c_str(),"get ");
	}
	if (!pBgn)
	{
		return "/";
	}
	pBgn += 4;
	const char *pEnd = strstr(pBgn," ");
	if (!pEnd)
	{
		return "/";
	}

	string strGet;
	strGet.append(pBgn,pEnd-pBgn);
	return strGet;
}

const char *TReq::getParamValue( const string &strName )
{
	string strURI = getURI();
	vector<string> strRes;
	StrFuns::Tokenize(strURI,strRes,"?&=");
	vector<string>::iterator it = strRes.begin();
	for (;it!=strRes.end();it++)
	{
		ATLTRACE("TOKEN:%s\n",(*it).c_str());
		if (strName==(*it))
		{
			it++;
			if (it==strRes.end())
			{
				return NULL;
			}
			return (*it).c_str();
		}
	}
	return NULL;
}

TResp::TResp( int socket )
{
	m_socket = socket;
	addHeader("Server","TS Server");
	addHeader("Content-Type","text/html");
	addHeader("Accept-Ranges","bytes");
	addHeader("Accept-Ranges","bytes");
	time_t lt = time(NULL);
	string strGmtNow = (string)ctime(&lt) + " GMT";
	addHeader("Date",strGmtNow.c_str());
}

void TResp::addHeader( const char *key,const char *value )
{
	m_mapHeader[key] = value;
}

void TResp::appendContent( const char *resp,int ilen /*= 0*/ )
{
	if (ilen<=0)
	{
		ilen = strlen(resp);
	}
	m_strResp.append(resp,ilen);
}

void TResp::finish()
{
	//////////Ҫ���͵���ҳ����
	//string CustomHtml = (string)"hello,world!\r\n" + req.getURI();
	//ATLTRACE("name:%s",req.getParamValue("name").c_str());

	//char headers[1000];//////////����ͷ��
	/////////ͷ����ʽ
	char hdrFmt[]=
		"HTTP/1.0 200 OK\r\n"
		"\r\n";

	addHeader("Content-Length",GlobeFuns::Int2Str(m_strResp.size()).c_str());

	/////������Ӧͷ��
	//send(m_socket,headers,strlen(headers),0);
	string strHead = "HTTP/1.0 200 OK\r\n";
	map<string,string>::iterator it = m_mapHeader.begin();
	for (;it!=m_mapHeader.end();it++)
	{
		strHead += (*it).first + ":" + (*it).second + "\r\n";
	}
	strHead += "\r\n";
	send(m_socket,strHead.data(),strHead.size(),0);


	/////�������� 
	send(m_socket,m_strResp.data(),m_strResp.size(),0);

	/////////�رձ�������
	closesocket(m_socket);
}