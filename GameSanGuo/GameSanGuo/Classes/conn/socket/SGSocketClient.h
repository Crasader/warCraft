//
//  MessageManager.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-30.
//
//

#ifndef __GameSanGuo__MessageManager__
#define __GameSanGuo__MessageManager__
#include <pthread.h>
#include "cocos2d.h"
#include "SGSocket.h"
#include "SGThread.h"

#include <google/protobuf/message_lite.h>
#include "SGPacketHeader.h"

class EasyBuffer;

using namespace cocos2d;

typedef enum
{
    SGNET_MR_BITE = 0, //无错误
    SGNET_NETWORK_NOT_AVAILABLE, //手机未打开网络连接
    SGNET_KICKED_BY_OTHER_DEVICE, //被其他设备
    SGNET_HEART_BEAT_FAILURE, //心跳检测失败
    SGNET_CANNOT_REACH_SERVER, //连接不上服务器
    SGNET_NO_REPLY_ON_REQUEST, //发消息没返回
    SGNET_SEND_ERROR, //发送失败
    SGNET_RECEIVE_ERROR, //接收失败（若为重连状态下，则不处理此项）
    SGNET_SEND_BREAK, //发送断链
    SGNET_RECEIVE_BREAK, //接收断链（若为重连状态下，则不处理此项）
} SGNetworkErrorType;


class SGSocketRequestQueue;
class SGSocketRequest;

//socket客户端管理
class SGSocketClient : public SGRunnable, public SGSocketConnectionHandler
{
public:
    SGSocket *_socket;
    //是否连接
	bool _isConn;
    //连接错误
    int _connError;
private:
    
    //服务器地址
	char *_serverIP;
    
    //服务器端口
	unsigned short _serverPort;
protected:
    
    
    SGThread *_thread;
    //请求队列
    SGSocketRequestQueue *_requestQueue;
    //暂停线程用
    pthread_mutex_t m_mutex;
    //转换char
    char m_tmpChar[32];
private:
    SGSocketClient();
    void setServerAddress(const char* address_, unsigned short port);
public:
    
    ~SGSocketClient();
    static SGSocketClient *sharedSocketClient();
    static void release();
    //连接到任意地址。
    void startConn(const char* address_, unsigned short port);
    //关闭当前socket。
    void stopConn();
    //发送消息。isShowCircle=是否显示锁屏；forceMatch=是否强制配对，当为true时候，则必须收到与_netcmd完全一样的消息号才允许接触锁屏，因为登陆消息的锁定而引发；delaySecs=延迟展示时间(秒)，可以在指定时间后再显示loading，用以提高PVP过程中的体验，对网速慢的旅客朋友们有一定的效果提升。
    void send(short _netcmd, google::protobuf::MessageLite* _msg,bool isShowCircle=true, bool forceMatch = false, float delaySecs = 0);
    //通过通知中心获取到需要处理的包，再转发到真正的类去。
    void onNotificationCenterResponseQueue(SGSocketRequest* request);
    //@重要：用于外部业务逻辑断线的调用，所有断线统一调用这里，由SGNetworkErrorType指示错误类型。
    void stopSocketConn(SGNetworkErrorType tp);
    
protected:
    //检测与连接到服务器
    void resetConn();
    //获取接收到的数据
    virtual void on_RecvResponse_Handler(const char* _rspPack, unsigned short _length);
    
    //运行网络线程
    virtual void run();
    
private:
    //接收缓冲区
    EasyBuffer* _receiveBuffer;
    
    //发送缓冲区
    EasyBuffer* _sendBuffer;
    
    //重连标识。若此标识为true，则忽略下一次recv带来的错误，因为即将更换连接。比如pve和pvp服务器的互相切换。
    bool _isReconnecting;
    
    //本次网络出错的类型。
    SGNetworkErrorType _curNetworkErrorState;
};

/*
 *  请求数据
 */
#pragma mark - SGSocketRequest
class SGSocketRequest : public cocos2d::CCObject {
public:
    CC_SYNTHESIZE(SGPacketHeader*, m_packerHeader, PackerHeader)
    bool m_isResponse;
    unsigned short m_cmd;
    google::protobuf::MessageLite* m_msg;
    
public:
    SGSocketRequest()
    :m_isResponse(false)
    ,m_packerHeader(NULL)
    ,m_cmd(0)
    ,m_msg(NULL)
    {
        
    }
    ~SGSocketRequest()
    {
//        CCLOG("~SGSocketRequest");
        CC_SAFE_DELETE(m_packerHeader);
        CC_SAFE_DELETE(m_msg);
    }
};
/*
 *  请求队列
 */
#pragma mark - SGSocketRequestQueue

class SGSocketRequestQueue {
private:
    std::vector<SGSocketRequest*>::iterator mIter;
    std::vector<SGSocketRequest*> mVector;
    pthread_mutex_t mutex;
    
    
public:
    SGSocketRequestQueue(){pthread_mutex_init(&mutex, NULL);}
    ~SGSocketRequestQueue()
    {
        CCLOG("~SGSocketRequestQueue");
        pthread_mutex_destroy(&mutex);
    }
    
    //压入新对象
	void pushRequest(SGSocketRequest* request);
    
    //获取有数据的对象
	SGSocketRequest* popResponseRequest();
    
    //获取指定命令的对象
    SGSocketRequest* getCmdRequest(unsigned int cmd);
    
    //释放所有对象
    void removeAllRequest();
    
    //两种发包方式，只能选一种
public:
    //设置指定命令的对象的包数据
    void setCmdRequestPack(unsigned int cmd, SGPacketHeader* pack);
    
    //向通知中心抛出包数据
    void requestPackToNotificationCenter(unsigned short cmd, SGPacketHeader* pack);
};


#pragma mark - SGSocketResponse

#endif /* defined(__GameSanGuo__MessageManager__) */
