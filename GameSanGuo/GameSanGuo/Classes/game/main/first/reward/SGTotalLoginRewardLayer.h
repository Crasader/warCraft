//
//  SGTotalLoginRewardLayer.h
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-29.
//
//

#ifndef __GameSanGuo__SGTotalLoginRewardLayer__
#define __GameSanGuo__SGTotalLoginRewardLayer__

  #include "cocos2d.h"
#include "SGBaseLayer.h"
#include "cocos-ext.h"
#include "RoleModule.pb.h"
USING_NS_CC_EXT;

extern int SUBLAYRTAG;
class SGTotalLoginRewardLayer:public SGBaseLayer {
public:
    bool  getReward;
    SGButton *qd_btn;
    CCScrollView  *m_scrollview ;
    int day1;
    int length;
    int leftLq;
protected:
    SGBoxDelegate *deletage;
    void initView(main::LoginAwardListResponse &response);
//    virtual void onEnter();
//    virtual void onExit();
public:
    void getCoinRewardHandlel();
    void getCoinsRewardListener(cocos2d::CCObject *obj);
    
    void getRewardListener(CCObject *obj);
    void getRewardHandle(CCObject *obj);
    static SGTotalLoginRewardLayer *create(main::LoginAwardListResponse &response,SGBoxDelegate *dg);
     
//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void showBoxCall(CCObject *obj);
    SGTotalLoginRewardLayer();
    ~SGTotalLoginRewardLayer();
    
     
};

class LoginRewardObj:public cocos2d::CCObject {
    
    
public:
    CC_SYNTHESIZE(int, needDay, NeedDay);//需要的天数
    CC_SYNTHESIZE(int, itemId, ItemId);//物品ID
    CC_SYNTHESIZE(int, itemType, ItemType);//类型0武将，1小兵，2道具，3装备
    CC_SYNTHESIZE(int , isFinish, IsFinish);//是否完成1完成
    CC_SYNTHESIZE(int , currentDay, CurrentDay);//当前是第几天
    CC_SYNTHESIZE(int , reward_count, RewardCount);//奖励个数
    
};

#endif /* defined(__GameSanGuo__SGTotalLoginRewardLayer__) */
