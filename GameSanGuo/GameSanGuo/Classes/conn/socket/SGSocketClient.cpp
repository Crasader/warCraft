//
//  MessageManager.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-30.
//
//

#include "SGSocketClient.h"
#include "FightBaseModule.pb.h"
#include "SGProtocolToMsg.h"
#include "SGNotificationCenter.h"
#include "SGHeartbeatDetection.h"
#include "SGConnectLoading.h"
#include "PlatformAdp.h"
#include "EasyBuffer.h"

#if (PLATFORM == ANDROID)
#include <netinet/in.h>
#endif

#define FREE_MEM(_P_) ({if(_P_){delete _P_;_P_=NULL;}})

//发送缓冲区大小
const static unsigned int SEND_BUFFER_SIZE = 102400;

//接受缓冲区大小
const static unsigned int RECEIVE_BUFFER_SIZE = 102400;


static SGSocketClient* shareInte = NULL;
SGSocketClient::SGSocketClient()
:_socket(NULL)
,_isConn(false)
,_thread(NULL)
,_requestQueue(NULL)
,_serverIP(NULL)
,_serverPort(0)
,_isReconnecting(false)
,_curNetworkErrorState(SGNET_MR_BITE)
{
    pthread_mutex_init(&m_mutex, NULL);
    _receiveBuffer = new EasyBuffer(RECEIVE_BUFFER_SIZE);
    _sendBuffer = new EasyBuffer(SEND_BUFFER_SIZE);
}
SGSocketClient::~SGSocketClient()
{
    CCLOG("~SGSocketClient");
    
    pthread_mutex_destroy(&m_mutex);
    if ( _requestQueue ) {
        _requestQueue->removeAllRequest();
        FREE_MEM(_requestQueue);
    }
    
    if ( _thread )
    {
        _thread->exit();
        FREE_MEM(_thread);
    }
    FREE_MEM(_socket);
}
SGSocketClient* SGSocketClient::sharedSocketClient()
{
    if (NULL == shareInte)
    {
        shareInte = new SGSocketClient();
    }
    return shareInte;
}
void SGSocketClient::release()
{
    FREE_MEM(shareInte);
}
void SGSocketClient::setServerAddress(const char* address_, unsigned short port)
{
    //初始化服务器地址信息
    if ( _serverIP ) {
        free(_serverIP);
        _serverIP = NULL;
        _serverPort = 0;
    }
    
    int len = strlen(address_)+1;
	_serverIP = (char*)malloc(sizeof(char)*len);
	memset(_serverIP, 0, len);
	strcpy(_serverIP, address_);
	_serverPort = port;
}

void SGSocketClient::startConn(const char* address_, unsigned short port)
{
    _isConn = false;
    _isReconnecting = true;
    _curNetworkErrorState = SGNET_MR_BITE;
    CCLOG("SGSocketClient::startConn() || server=%s:%d, _isConn=%d", address_, port, _isConn);
    if (address_)
    {
        setServerAddress(address_, port);
        if (_socket)
        {
            stopConn();
        }
    }
    
    if (NULL == _socket)
    {
        _socket = new SGSocket();
        _socket->setHandlerListen(this);
        //初始化
        _thread = new SGThread(*this);
        
        _requestQueue = new SGSocketRequestQueue();
        CCLOG("开启线程");
        _thread->start();
    }
    else
    {
        //解锁这里 才能重新启动服务器
        pthread_mutex_unlock(&m_mutex);
        _thread->start();
    }
    
}

void SGSocketClient::stopConn()
{
    if (_socket)
    {
        _socket->closeConn();
    }
    SGHeartbeatDetection::shareHeartbeatDetection()->setIsHbdWorking(false);
}

void SGSocketClient::resetConn()
{
    CCLOG("SGSocketClient::resetConn() || begin! server=%s:%d, _isConn=%d",_serverIP, _serverPort, _isConn);
    //已经连接返回
    if (_isConn)
    {
        return;
    }
    
    pthread_mutex_lock(&m_mutex);
    int result = -1;
    int tryCount = 3;
    //嘗試tryCount次連接。
    do
    {
        result = _socket->startConnect(_serverIP, _serverPort);
        if (0 == result) //success
        {
            break;
        }
        else //try again
        {
            tryCount--;
        }
    }
    while (tryCount > 0);
    pthread_mutex_unlock(&m_mutex);
    //连接到服务器
    if ( 0 == result)
    {
        CCLOG("SGSocketClient::resetConn() || connection succeeded! server=%s:%d",_serverIP, _serverPort);
        _isConn = true;

        //XXX: 这里以前安卓会延时下发“连接成功”消息，为何这么吊？
        SGNotificationCenter::sharedNotificationCenter()->postNotification(NC_NETWORK_LINK_SUCCESS);
    }
    //网络状态有问题
    else
    {
        CCLOG("SGSocketClient::resetConn() || connection failed! server=%s:%d",_serverIP, _serverPort);
        _isConn = false;
        SGNotificationCenter::sharedNotificationCenter()->postNotification(NC_NETWORK_LINK_FAIL);
    }
    _isReconnecting = false;
}

void SGSocketClient::run()
{
    //new
    resetConn();
    
    bool isRecvMsgNow = false; //是否正在读取有效消息
    unsigned short msgLength = 0; //当前需要获得的消息总长度
    char* msgPack = NULL; //有效消息流的指针
    bool shallRecv = true; //是否开始统一接受数据
    
    do
    {
        /* 处理消息 */
        if (isRecvMsgNow) //收取包
        {
            if (_receiveBuffer->readBuffer(msgPack, msgLength))
            {
//                CCLOG("SGSocketClient::run() || on_RecvResponse_Handler Begin");
                on_RecvResponse_Handler(msgPack, msgLength);
//                CCLOG("SGSocketClient::run() || on_RecvResponse_Handler End");
                
                CC_SAFE_DELETE_ARRAY(msgPack); isRecvMsgNow = false; msgLength = 0;
//                msgPack = NULL; isRecvMsgNow = false; msgLength = 0; //msgPack交给SGPacketHeader去删除，省去复制的过程。
                
                shallRecv = false;
            }
            else
            {
                shallRecv = true;
            }
        }
        else //校验包
        {
            if (_receiveBuffer->readBuffer( (char*) &msgLength, USHORT_SIZE))
            {
                msgLength = htons(msgLength);
                CCLOG("SGSocketClient::run() || RECV. buffer length=%d", msgLength);
                if (msgLength >= 2)
                {
                    msgPack = new char[msgLength];
                    memset(msgPack, 0, msgLength);
                    isRecvMsgNow = true;
                    shallRecv = false;
                }
                else
                {
                    //XXX: 长度不合法的话丢弃处理？
                    isRecvMsgNow = false; msgLength = 0;
                }
            }
            else
            {
                shallRecv = true;
            }
        }
        
        if (shallRecv)
        {
            unsigned int len = 0;
            char* wBuf = _receiveBuffer->getFreeWriteBuffer(len);
            int realRecv = _socket->Receive(wBuf, len, 0);
            
            /* 收取消息 */
            if (realRecv > 0) //recv ok
            {
                _receiveBuffer->addWritePos(realRecv);
            }
            else if (realRecv < 0) //recv error
            {
                _isConn = false;
                //连接错误处理。
                if (!_isReconnecting)
                {
                    CCLOG("SGSocketClient::run() || ERROR! handle nega!");
                    SGNotificationCenter::sharedNotificationCenter()->postNotification(NC_NETWORK_RECEIVE_ERROR);
                }
                else
                {
                    CCLOG("SGSocketClient::run() || NOTICE! handle nega!");
                }
            }
            else //recv nothing, break.
            {
                _isConn = false;
                //可以做关闭连接处理
                if (!_isReconnecting)
                {
                    CCLOG("SGSocketClient::run() || ERROR! disconnected!");
                    SGNotificationCenter::sharedNotificationCenter()->postNotification(NC_NETWORK_RECEIVE_BREAK);
                }
                else
                {
                    CCLOG("SGSocketClient::run() || NOTICE! disconnected!");
                }
            }
        }
    }
    while (_isConn);
    
    return;
    
//    //obsolete code 0.22
//    CCLOG("SGSocketClient::run() || looper begins!");
//    do {
//        
//        resetConn();
//        if (_isConn)
//        {
//            do {
//                int receive = _socket->receive();
//                if (receive < 0)
//                {
//					//                    _socket->closeConn();
//					//                    _isConn = false;
//                    
//					//                    CCLOG("网络错误，已经关闭连接！");
//					//                    _connError = true;
//					//                    _isConn = false;
//                    //so what about samsung?
//                    CCLOG("SGSocketClient::run() || recv neg! bad idea!");
//                    SGNotificationCenter::sharedNotificationCenter()->postNotification(NC_NETWORK_LINK_FAIL);
//                    return;
//                }
//                _thread->sleepMillisecond(100);
//                
//                
//            } while (true);
//            
//        }
//    } while (!_connError);
//    CCLOG("SGSocketClient::run() || looper ends!");
}

#include "GameConfig.h"

void SGSocketClient::send(short _netcmd, google::protobuf::MessageLite *_msg,bool isShowCircle, bool forceMatch, float delaySecs /*= 0*/)
{
    do
    {
        if (_isConn)
        {
            if ( !_msg )
            {
                CCLOG(">>>>SendMsg || mId=%d, state=%s", _netcmd, "BAD! empty msg!");
                break;
            }
            
            if (!GameConfig::chackExistenceNetwork())
            {
                stopSocketConn(SGNET_NETWORK_NOT_AVAILABLE);
            }
            else
            {
                //发送的包数据
                SGPacketHeader* pack = new SGPacketHeader();
                pack->writePacketBytes(_netcmd, _msg);
                
                int status = _socket->sendMsg(pack->getPackData()->data, pack->getPackData()->length, isShowCircle);
                if (isShowCircle)
                {
                    SGConnectLoading::setDelayShowTime(delaySecs);
                }
                if ( status < 0 ) //error
                {
                    CCLOG(">>>>SendMsg || mId=%d, state=%s", _netcmd, "ERROR! problem of sending!");
                    stopSocketConn(SGNET_SEND_ERROR);
                }
                else if (status == 0) //break
                {
                    CCLOG(">>>>SendMsg || mId=%d, state=%s", _netcmd, "BAD! handle zero!");
                    stopSocketConn(SGNET_SEND_BREAK);
                }
                else
                {
                    //处理强制配对。
                    if (forceMatch)
                    {
                        char msgId[10] = { 0 };
                        sprintf(msgId, "%d", _netcmd);
                        SGNotificationCenter::sharedNotificationCenter()->setForceMatchMsgId(msgId);
                    }
                    CCLOG(">>>>SendMsg || mId=%d, state=%s, length=%d", _netcmd, "GOOD", pack->getPackData()->length);
                }
                FREE_MEM(pack);
            }
        }
        else
        {
            CCLOG(">>>>SendMsg || mId=%d, state=%s", _netcmd, "BAD! conn error!");
        }
    }
    while (0);
}

void SGSocketClient::stopSocketConn(SGNetworkErrorType tp)
{
    //有时候会同时出现多个掉线错误，只处理第一个。
    if (_curNetworkErrorState != SGNET_MR_BITE)
    {
        return;
    }
    _isConn = false;
    stopConn();
    SGHeartbeatDetection *sghd = SGHeartbeatDetection::shareHeartbeatDetection();
    sghd->stopHeartbeat(tp);
    _curNetworkErrorState = tp;
}

//获取接收到的数据
void SGSocketClient::on_RecvResponse_Handler(const char* _rspPack, unsigned short _length) {
    
    //数据包有问题
    if ( !_rspPack || _length < 2 )
    {
        CCLOG("SGSocketClient::on_RecvResponse_Handler() || ERROR! on_RecvResponse_Handler！length=%d", _length);
        return;
    }
    
    //构造数据包
    SGPacketHeader* pack = new SGPacketHeader();
    pack->readPacketBytes(_rspPack, _length);
    CCLOG("<<<<ReceiveMsg || mId=%d, mLength=%d, state=%s", pack->getMsgId(), pack->getPackData()->length, "GOOD.");
    
    //放到数据对象队列
    _requestQueue->requestPackToNotificationCenter(pack->getMsgId(), pack);
}

void SGSocketClient::onNotificationCenterResponseQueue(SGSocketRequest* request)
{
    if ( request ) {
        
        if ( request->getPackerHeader())
        {
            //转到负责类处理
            request->m_msg = parseMessage(request->m_cmd, request->getPackerHeader());
            memset(m_tmpChar, 0, 32);
            sprintf(m_tmpChar, "%s%d", m_tmpChar, request->m_cmd);
            SGNotificationCenter::sharedNotificationCenter()->postNotification(m_tmpChar, request, false);
        }
    }
}
#pragma mark - DDSocketRequestQueue

void SGSocketRequestQueue::pushRequest(SGSocketRequest* request)
{
    pthread_mutex_lock(&mutex);
	mVector.push_back(request);
    pthread_mutex_unlock(&mutex);
}

SGSocketRequest* SGSocketRequestQueue::popResponseRequest()
{
    pthread_mutex_lock(&mutex);
    
	if ( mVector.size() > 0 ) {
        
        mIter = mVector.begin();
        for (; mIter!=mVector.end(); mIter++) {
            if ( (*mIter)->m_isResponse ) {
                
                SGSocketRequest* request = (*mIter);
                mVector.erase(mIter);
                
                pthread_mutex_unlock(&mutex);
                return request;
            }
        }
	}
    
    pthread_mutex_unlock(&mutex);
	return NULL;
}

SGSocketRequest* SGSocketRequestQueue::getCmdRequest(unsigned int cmd)
{
    pthread_mutex_lock(&mutex);
    
	if ( mVector.size() > 0 ) {
        
        mIter = mVector.begin();
        for (; mIter!=mVector.end(); mIter++) {
            if ( cmd == (*mIter)->m_cmd ) {
                
                SGSocketRequest* request = (*mIter);
                pthread_mutex_unlock(&mutex);
                return request;
            }
        }
	}
    
    pthread_mutex_unlock(&mutex);
	return NULL;
}

void SGSocketRequestQueue::setCmdRequestPack(unsigned int cmd, SGPacketHeader* pack)
{
    pthread_mutex_lock(&mutex);
    
	if ( mVector.size() > 0 ) {
        
        mIter = mVector.begin();
        for (; mIter!=mVector.end(); mIter++) {
            if ( cmd == (*mIter)->m_cmd ) {
                
                (*mIter)->setPackerHeader(pack);
                (*mIter)->m_isResponse = true;
                break;
            }
        }
        
        //都没找到丢掉这个包
        if ( mIter == mVector.end() ) {
            FREE_MEM(pack);
        }
	}
    
    pthread_mutex_unlock(&mutex);
}

void SGSocketRequestQueue::requestPackToNotificationCenter(unsigned short cmd, SGPacketHeader* pack)
{
    //MMDEBUG:
//    pthread_mutex_lock(&mutex);
    
	//	if ( mVector.size() > 0 )
	//    {
	//
	//        mIter = mVector.begin();
	//        for (; mIter!=mVector.end(); mIter++)
	//        {
	//            CCLOG("%d<><>%i",cmd,(*mIter)->m_cmd);
	//            if ( cmd == (*mIter)->m_cmd )
	//            {
	//
	//                (*mIter)->m_packerHeader = pack;
	//                (*mIter)->m_isResponse = true;
	//
	//                //把包用通知发出去
	//                SGSocketClient::sharedSocketClient()->onNotificationCenterResponseQueue(*mIter);
	//
	//                break;
	//            }
	//        }
	//    }
	//都没找到丢掉这个包
	//        if ( mIter == mVector.end() )
	//        {
	//            CCLOG("都没找到丢掉这个包");
    
    
    
	SGSocketRequest *request = new SGSocketRequest();
	request->setPackerHeader(pack);
	request->m_cmd = cmd;
	request->m_isResponse = true;
	
	SGSocketClient::sharedSocketClient()->onNotificationCenterResponseQueue(request);

	
    //MMDEBUG:
//    pthread_mutex_unlock(&mutex);
}

void SGSocketRequestQueue::removeAllRequest()
{
    pthread_mutex_lock(&mutex);
    
	if ( mVector.size() > 0 ) {
        
        mIter = mVector.begin();
        for (; mIter!=mVector.end(); mIter++) {
            FREE_MEM(*mIter);
        }
        
        mVector.clear();
	}
    
    pthread_mutex_unlock(&mutex);
}
