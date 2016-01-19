//
//  SGPlatFormIos.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-7.
//
//

#ifndef __GameSanGuo__SGPlatFormIos__
#define __GameSanGuo__SGPlatFormIos__

#include <iostream>
#include "cocos2d.h"
#include "PlatformAdp.h"

USING_NS_CC;


class SGPlatFormIos :public CCLayer
{
public:
    
    static SGPlatFormIos *create();
    
    SGPlatFormIos();
    
    ~SGPlatFormIos();

    //获得当前发送转圈的状态。
    bool getIsShowCircle();
    
#if (PLATFORM == ANDROID)
	bool init();
#endif
    void showConnection(bool isshow = true,bool startCount = true);
    void removeConnection();
    //推送好友消息
    void pushFriendMessage(const char* name);
    //推送体力回满
    void pushPowerFull(int timeLeft);
    //推送活动
    void pushActivities();
private:
    void connetDetection(float dt);
    int m_connetCount;
    
    //是否正在转圈。
    bool _isShowCircle;
};

#endif /* defined(__GameSanGuo__SGPlatFormIos__) */
