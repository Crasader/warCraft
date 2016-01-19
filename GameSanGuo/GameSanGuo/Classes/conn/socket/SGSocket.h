//
//  SGSocket.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-29.
//
//

#ifndef __GameSanGuo__SGSocket__
#define __GameSanGuo__SGSocket__

//#ifdef DEBUG
//#define PRINTF_LOG(format, ...) (printf(format, ##__VA_ARGS__))
//#else
//#define PRINTF_LOG(_STR_)
//#endif
#define PRINTF_LOG(format, ...) (printf(format, ##__VA_ARGS__))
#define IF_ERROR_BREAK(_P_,format, ...) ({if(_P_){PRINTF_LOG(format, ##__VA_ARGS__);break;}})

struct PkgHead
{
    int msgId;
    int zoneId;
};
typedef int SOCKET;

/*
 *  socket连接事件处理
 */
class SGSocketConnectionHandler {
public:    
    //获取接收到的数据
    virtual void on_RecvResponse_Handler(const char* _response, unsigned short _length) = 0;//{ return; }
};

class SGSocket 
{
public:
    //无参构造
	SGSocket();
    //释放类 释放连接
	~SGSocket();
    //连接到SOCKET服务器，成功返回0，否则返回错误码
    int startConnect(const char* ip, unsigned short port);
    //关闭连接
    int closeConn();
    //发送消息，如果出错返回-1，否则返回发送的字符长度
	//sendBuff：要发送的字符串 sendLen：发送字符长度
    int sendMsg(unsigned char *pBuff, unsigned long buffSize,bool isShowCircle);
    //采用事件转发处理方式接收数据，数据流以二进制形式传输
    //成功返回接收到的数据长度，失败返回错误码(负数)
    int receiveMsg();
    int receive();
    //设置handler处理类
    void setHandlerListen(SGSocketConnectionHandler* handler_);
    //检测网络是否可读
    int checkConn();
    
    //新增接口。
    int Receive(char* buf, int len, int flags = 0);
    
protected:
    //设置连接服务器，端口
    void setServerAddress(const char* serverAddress, unsigned short serverPort);
    //检测缓冲区空间是否足够放得下数据，不够重新构建
    void resetBufferSize(int curLength, int maxLength);
private:
    int startConnect();
private:
    SOCKET socketHandle;
    SOCKET sockID;
    char *serverIP;
    unsigned short _serverPort;
    
    //转发处理事件机制类
    SGSocketConnectionHandler* _handler;
    
    //接收数据缓冲区
    char *responseBuf;
    char *lengthBuf;
    int responseBufLength;
    
public:
    bool isConnect;

};
#endif /* defined(__GameSanGuo__SGSocket__) */
