//
//  SGHeartbeatDetection.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-4-27.
//
//

#include "SGHeartbeatDetection.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "LoginModule.pb.h"
#include "SGSocketClient.h"
#include "SGSocket.h"
#include "SGMainManager.h"

//最大检查心跳次数。
const static short MAX_CHECK_NUM = 6;

static SGHeartbeatDetection *_heartbeatDetection = NULL;

SGHeartbeatDetection::SGHeartbeatDetection()
:_isHbdWorking(false)
,_checkedNum(0)
{
    
}


SGHeartbeatDetection::~SGHeartbeatDetection()
{
    
}

SGHeartbeatDetection* SGHeartbeatDetection::shareHeartbeatDetection()
{
    if (NULL == _heartbeatDetection) {
        _heartbeatDetection = new SGHeartbeatDetection();
        _heartbeatDetection->initHeartbeat();
        return _heartbeatDetection;
    }
    return _heartbeatDetection;
}

void SGHeartbeatDetection::initHeartbeat()
{
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->addObserver(MSG_HEARTBEAD, this, callfuncO_selector(SGHeartbeatDetection::heartbeatSucceedCallback));
    schedule(schedule_selector(SGHeartbeatDetection::heartbeatDetection), 10.);
}

void SGHeartbeatDetection::heartbeatDetection()
{
    if (_isHbdWorking)
    {
        if (_checkedNum < MAX_CHECK_NUM)
        {
            main::HeartBeatRequest *msg = new main::HeartBeatRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_HEARTBEAD, msg, false);
            _checkedNum++;
            CCLOG("HBD || seq=%d", _checkedNum);
        }
        else
        {
            //心跳失败，与伺服器连接中断！
            SGSocketClient::sharedSocketClient()->stopSocketConn(SGNET_HEART_BEAT_FAILURE);
        }
    }
}

void SGHeartbeatDetection::heartbeatSucceedCallback(CCObject *obj)
{
    SGSocketRequest *socketRequest = (SGSocketRequest *)obj;
    if (socketRequest)
    {
        main::HeartBeatResponse *sr = (main::HeartBeatResponse *)socketRequest->m_msg;
        if (sr->state() == 1)
        {
            CCLOG("HBD || kicked.");
            //被别人登录提掉线。
            SGSocketClient::sharedSocketClient()->stopSocketConn(SGNET_KICKED_BY_OTHER_DEVICE);
            return;
        }
        else
        {
             CCLOG("HBD || pass.");
            this->_checkedNum = 0;
        }
    }
}

void SGHeartbeatDetection::alertViewClickedButtonAtIndex(int buttonIndex)
{
//    const char *serverIp = SGMainManager::shareMain()->getServerIp()->getCString();
//    short serverPost = SGMainManager::shareMain()->getServerPost();
//    SGSocketClient::sharedSocketClient()->startConn(serverIp, serverPost);
    MUSIC_STOP;
    MUSIC_PLAY(MUSIC_SCENE);
    SGMainManager::shareMain()->showwelcomelayer();
    
}

void SGHeartbeatDetection::stopHeartbeat(SGNetworkErrorType tp)
{
    this->setIsHbdWorking(false);
    SGMainManager* sgmm = SGMainManager::shareMain();
    sgmm->closeBox();
    sgmm->getMainScene()->plat->removeConnection();
    sgmm->showConnectTip(tp);

//    if (a == 1) {
////        CCNative::createAlert("网络失败", "服务忙请稍后继续！！", "Cancel");
////        CCNative::addAlertButton("OK");
////        CCNative::showAlert(this);
//////        SGMainManager::shareMain()->showMessage("。。。。000000");
//////        SGMainManager::shareMain()->showwelcomelayer();
//        SGMainManager::shareMain()->showConnectTip(a);
//
//    }else{
//        SGMainManager::shareMain()->showConnectTip(a);
//    }
}


