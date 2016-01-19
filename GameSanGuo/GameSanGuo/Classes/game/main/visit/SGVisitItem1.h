//
//  SGVisitItem1.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-18.
//
//

#ifndef __GameSanGuo__SGVisitItem1__
#define __GameSanGuo__SGVisitItem1__

#include "SNSTableViewCellItem.h"
#include "SGButton.h"
class SGVisitDelegate;
class SGVisitItem1 : public SNSTableViewCellItem
{
private:
    CCSize s;
    CCLayer *layer;
    CCLayer *zbLayer;
    CCSprite * zik;    
    SGCCLabelTTF *time;
    SGCCLabelTTF *labelTime;
    SGCCLabelTTF *labelDesc;
//    CCLabelTTF *time1;    
    int index;
    int zbIndex;    
    
    SGVisitDelegate *delegate;
    std::string lableDes;
public:
    int officerTime;
    int choose;
    SGButton *visit1;
    CCProgressTimer *left;
private:
    void startMove();
    void moveEnd();
    void zbStartMove();
    void zbMoveEnd();
    void updateTime();
    void confirmHandler(CCNode *sender);
    void test(CCNode *psender);
    void refreshTimer();
    int limit_time; // 剩余时间, 秒
    bool isStartActivity;
public:
    SGVisitItem1();
    ~SGVisitItem1();
	virtual bool initWithSize(int num,int time,int per,bool huoVisitOpen);
	static SGVisitItem1* create(SGVisitDelegate *delegate,int num,int time,int per,bool huoVisitOpen);
    
    void setIsCanTouch(bool touch);
    void setActivityMsg(const time_t& currentTime,const time_t& startTime,const time_t& endTime,const std::string& activityMsg);
public:
    //有免费 元宝、铜钱探访机会
    CCSprite * m_noticeGoldFree;
};

class SGVisitDelegate
{
public:
    virtual void buyLegHandler(SGVisitItem1 *visitItem){};
    
};
#endif /* defined(__GameSanGuo__SGVisitItem1__) */
