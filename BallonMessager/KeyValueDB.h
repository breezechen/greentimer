#pragma once

//#include "sphttpmsg.hpp"
#include "HttpServ.h"

//========================================
//��ϵͳ��Ϣ
void handlerReadSys(TReq * request, TResp * response);
//д��ֵ
void handlerWrite(TReq * request, TResp * response);
//����ֵ
void handlerRead(TReq * request, TResp * response);
//��־
void handlerLog(TReq * request, TResp * response);
//��־
void handlerShowLog(TReq * request, TResp * response);
//gnugo
void handlerGnugo(TReq * request, TResp * response);
//�ļ�������
void handlerFile(TReq * request, TResp * response);
//�ļ�������
void handlerDir(TReq * request, TResp * response);