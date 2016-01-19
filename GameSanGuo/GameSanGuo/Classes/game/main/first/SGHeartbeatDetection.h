//
//  SGHeartbeatDetection.h
//  GameSanGuo
//
//  Created by wenlong w on 13-4-27.
//
//

#ifndef __GameSanGuo__SGHeartbeatDetection__
#define __GameSanGuo__SGHeartbeatDetection__

#include <iostream>
#include "cocos2d.h"
#include "native/CCAlertViewDelegate.h"
#include "native/CCNative.h"
#include "SGSocketClient.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d;

/**
 * 心跳检测相关，每10秒发送一次108消息，并负责监控server的回复状况，超过3次未回复，则判定网络失联。
 */
class SGHeartbeatDetection : public CCLayer, CCAlertViewDelegate
{
public:
    SGHeartbeatDetection();
    ~SGHeartbeatDetection();
    static SGHeartbeatDetection *shareHeartbeatDetection(void);
    void initHeartbeat();
    
    //108心跳的回调，注意历史原因，如果因别人登录而被踢掉线也会走108，这种情况下返回消息的state会为1。
    void heartbeatSucceedCallback(CCObject *obj);
    
    //停止心跳，与断开连接配合使用。
    void stopHeartbeat(SGNetworkErrorType tp);
    
    //暂时无用，是调用平台本地的Dialog对话框。
    virtual void alertViewClickedButtonAtIndex(int buttonIndex);

public:
    //心跳检测。
    void heartbeatDetection();
    
    //设置心跳检测开关。
    void setIsHbdWorking(bool bl) {this->_isHbdWorking = bl; if (!bl) {this->_checkedNum = 0;} }

private:
    //开关，是否进行心跳检测。
    bool _isHbdWorking;
    
    //已进行心跳检测的次数，
    short _checkedNum;
};

#endif /* defined(__GameSanGuo__SGHeartbeatDetection__) */
