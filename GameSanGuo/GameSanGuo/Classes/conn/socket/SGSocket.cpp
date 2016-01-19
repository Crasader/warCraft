//
//  SGSocket.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-29.
//
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "signal.h"
#include <fcntl.h>

#include "SGSocket.h"
#include "SGMainManager.h"


#define RES_DEFAULT_LENGTH (2048*10) //recv接收字符长度

#define TIME_OUT_TIME (10*1000) //超时时间


SGSocket::SGSocket()
:sockID(0),
_serverPort(0),
serverIP(NULL),
isConnect(false),
_handler(NULL),
responseBuf(NULL),
lengthBuf(NULL),
responseBufLength(0),
socketHandle(0)
{
    
}
SGSocket::~SGSocket()
{
    closeConn();
	
	if (serverIP) {
		free(serverIP);
		serverIP = NULL;
	}
    
    if ( responseBuf )
    {
        free(responseBuf);
        responseBuf = NULL;
    }
}
void SGSocket::setServerAddress(const char* serverAddress, unsigned short serverPort)
{
    if (NULL == this) {
        return;
    }
    //初始化服务器地址信息
    if ( serverIP ) {
        free(serverIP);
        serverIP = NULL;
        _serverPort = 0;
    }
    
    int len = strlen(serverAddress)+1;
	serverIP = (char*)malloc(sizeof(char)*len);
	memset(serverIP, 0, len);
	strcpy(serverIP, serverAddress);
	_serverPort = serverPort;
    
    //构建默认缓冲区
    resetBufferSize(0, RES_DEFAULT_LENGTH);
}
void SGSocket::resetBufferSize(int curLength, int maxLength)
{
    //空间够了
    if ( responseBufLength > maxLength ) {
        return;
    }
    
    char* pTmpBuf = responseBuf;
    
    //重新构造空间
    responseBufLength = maxLength;
    responseBuf = NULL;
    responseBuf = (char*)malloc(sizeof(char)*responseBufLength);
    
    //将旧数据拷贝
    if ( pTmpBuf && curLength > 0 ) {
        memcpy(responseBuf, pTmpBuf, curLength);
    }
    
    //释放旧空间
    if ( pTmpBuf )
    {
        free(pTmpBuf);
        pTmpBuf = NULL;
    }
}

//空函数
void dumpSocket(int sig)
{
    PRINTF_LOG("catch it! SGSocket!\n");
}

int SGSocket::startConnect(const char* ip, unsigned short port)
{
    setServerAddress(ip, port);
    return startConnect();
}

int SGSocket::startConnect()
{
    struct sockaddr_in addr;
    struct hostent *phost;
    int error = -1;
	
    do
    {
        //初始化地址
        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(_serverPort);
        addr.sin_addr.s_addr = inet_addr(serverIP);//按IP初始化
        if( addr.sin_addr.s_addr == INADDR_NONE )
        {
            //如果输入的是域名 需要解析出地址
            phost = (struct hostent*)gethostbyname(serverIP);
            IF_ERROR_BREAK(phost==NULL, "Init socket s_addr error!\n");
            addr.sin_addr.s_addr =((struct in_addr*)phost->h_addr)->s_addr;
        }
        
        //创建socket AF_INET表示使用IPv4协议 SOCK_STREAM表示使用TCP协议
        sockID = socket(addr.sin_family, SOCK_STREAM, IPPROTO_TCP);
        IF_ERROR_BREAK(sockID<0, "ERROR%d: Init socket error!\n", sockID);
        
        //链接服务器 成功后返回socket
        error = connect(sockID, (struct sockaddr*)&addr, sizeof(struct sockaddr));
        IF_ERROR_BREAK(0 != error, "ERROR%d: Socket not connect on server !\n", error);
        
        // 设置非阻塞模式
        int flags = fcntl(sockID, F_GETFL, 0);
        fcntl(sockID, F_SETFL, flags | O_NONBLOCK);
        
        //用来探测服务器是否可写
        fd_set          fdwrite;
        struct timeval  tvSelect;
        FD_ZERO(&fdwrite);
        FD_SET(sockID, &fdwrite);
        tvSelect.tv_sec = TIME_OUT_TIME;
        tvSelect.tv_usec = 0;
        int retval = select(sockID + 1, NULL, &fdwrite, NULL, &tvSelect);
        if(retval < 0) {
            if ( errno == EINTR ) {
                PRINTF_LOG("SGSocket select error\n");
            }
            else
            {
                PRINTF_LOG("SGSocket error %d\n", retval);
            }
            close(sockID);
            error = -1;
            break;
        }
        else if( 0 == retval ) {
            PRINTF_LOG("SGSocket select timeout........\n");
            close(sockID);
            error = -2;
            break;
        }
        
        // 设置阻塞模式
        flags = fcntl(sockID, F_GETFL, 0);
        flags &= ~ O_NONBLOCK;
        fcntl(sockID, F_SETFL, flags);
        
        /****************************************/
        //设置不被SIGPIPE信号中断，物理链路损坏时才不会导致程序直接被Terminate
        //在网络异常的时候如果程序收到SIGPIRE是会直接被退出的。

        struct sigaction sig;
        memset(&sig, 0, sizeof(sig));
        sig.sa_handler = dumpSocket;
//        sig.sa_handler = SIG_IGN;
//        sig.sa_flags = SA_RESTART;
        int r = sigaction( SIGPIPE, &sig, NULL );
        CCLOG("SIGNAL PROCESS RESULT = %d!", r);
        /****************************************/
        
        //成功连接
//        error = 0;
        isConnect = true;
//        m_isOpen = true;
        
    }
    while (0);
    
    return error;
}

int SGSocket::sendMsg(unsigned char *pBuff, unsigned long buffSize,bool isShowCircle)
{
    if(isShowCircle)
    {
        SGMainManager::shareMain()->getMainScene()->plat->showConnection();
    }
    
//    PRINTF_LOG("buffSize:%ld\n",buffSize);
	int sendSize = 0;
    
    sendSize = send(sockID, pBuff, buffSize, 0);
    
    if(sendSize <= 0){
        PRINTF_LOG("sendSize:%d  Socket send msg error!\n", sendSize);
    }
//    PRINTF_LOG("sendSize:%d\n",sendSize);
    return sendSize;
}
int SGSocket::receive()
{
    int curLenth = 0;
    short maxLength = 0;
    int recvResult = 0;
    while ( true )
    {
    	//android 需要在接收之前清空一下缓冲
    	memset(responseBuf, 0, responseBufLength);
        recvResult = recv(sockID, responseBuf, 2, 0);

        if( recvResult < 0  )
        {
			PRINTF_LOG("SGSocket::receive() || socket recv msg error!! result=%d\n", recvResult);
//            memset(responseBuf, 0, responseBufLength);
			return recvResult;
		}
        else if ( 0 == recvResult )
        {
            break;
		}
        else
        {
            //计算数据总长度
            short recVMax = 0;
            memset(&recVMax, 0, 2);
            memcpy(&recVMax, responseBuf+maxLength, 2);
            recVMax = htons(recVMax);

            int bagLen = recVMax;

            //此步不需要，直接开始循环接收即可
            //recvResult = recv(sockID, responseBuf+2, bagLen, 0);
            //curLenth += recvResult;
            while (curLenth < bagLen)
            {
                recvResult = recv(sockID, responseBuf+2+curLenth, bagLen-curLenth, 0);
                curLenth += recvResult;
            }
            //导出数据
            if ( _handler )
            {
                _handler->on_RecvResponse_Handler(responseBuf, curLenth);
                memset(responseBuf, 0, responseBufLength);
                break;
            }
        }
    }
    return curLenth;
}

//新增接口。
int SGSocket::Receive(char* buf, int len, int flags)
{
    return recv(sockID, buf, len, flags);
}

void SGSocket::setHandlerListen(SGSocketConnectionHandler* handler_)
{
    _handler = handler_;
}

int SGSocket::checkConn()
{
    fd_set          fdRead;
    struct timeval  tvSelect;
    FD_ZERO(&fdRead);
    FD_SET(sockID, &fdRead);
    tvSelect.tv_sec = TIME_OUT_TIME;
    tvSelect.tv_usec = 0;
    int iResult = select(0, &fdRead, NULL, NULL, &tvSelect);
    if(iResult < 0)
    {
        //连接出错
    }
    else if(iResult == 0)
    {
        //等待超时
    }
    else
    {
        //iResult返回可读socket数，客户端一般都1
    }
    
    return iResult;
}
int SGSocket::closeConn()
{
    if ( isConnect )
    {
        CCLOG("SGSocket::closeConn() || serverIP:%s port:%d", serverIP, _serverPort);
        shutdown(sockID, SHUT_RDWR);
		close(sockID);
		isConnect = false;
	}
    return 0;
}
