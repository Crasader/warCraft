//
//  SGVisitItem2.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-18.
//
//

#ifndef __GameSanGuo__SGVisitItem2__
#define __GameSanGuo__SGVisitItem2__

#include "SNSTableViewCellItem.h"
#include "SGButton.h"
class SGVisitDelegate2;
class SGVisitItem2 : public SNSTableViewCellItem
{
private:

    CCLayer *wjlayer;
    CCLayer *zbLayer;
    CCSprite * zik;
    SGCCLabelTTF *time;
    SGCCLabelTTF *time1;
    int index;
    int zbIndex;
    SGCCLabelTTF *labelTime;
    SGVisitDelegate2 *delegate;
     std::string lableDes;
    SGCCLabelTTF *labelDesc;
    bool isStartActivity;
public:
    int officerTime;
    int equipTime;
    int type;
    SGButton *visit1;
    CCProgressTimer *left;
private:
    void startMove();
    void moveEnd();
    void zbStartMove();
    void zbMoveEnd();
    void updateTime1();
    void updateTime();
    void confirmHandler(CCNode *sender);
    void refreshTimer();
    int limit_time; // 剩余时间, 秒
public:
    SGVisitItem2();
    ~SGVisitItem2();
	virtual bool initWithSize(int num,int a,int b ,int c,int d);
	static SGVisitItem2* create(SGVisitDelegate2 *dele_,int num,int a,int b ,int c,int d);
    
    void setIsCanTouch(bool touch);
    void setActivityMsg(const time_t& currentTime,const time_t& startTime,const time_t& endTime,const std::string& activityMsg);
    
public:
    //有免费 元宝、铜钱探访机会
    CCSprite * m_noticeCoinFree;
};
class SGVisitDelegate2
{
public:
    virtual void buyHandler(SGVisitItem2 *visitItem){};
};


#endif /* defined(__GameSanGuo__SGVisitItem2__) */
