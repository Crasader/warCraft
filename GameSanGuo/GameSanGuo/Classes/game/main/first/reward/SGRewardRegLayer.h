//
//  SGRewardRegLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-20.
//
//

#ifndef __GameSanGuo__SGRewardRegLayer__
#define __GameSanGuo__SGRewardRegLayer__

#include "SGBaseLayer.h"
#include "SGCCLabelTTF.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
class SGRewardRegLayer : public SGBaseLayer
{
private:
    SGCCLabelTTF *lab_iscanget;
    SGCCLabelTTF *lab_notice;
    SGCCLabelTTF *lab_sec;
    /*迎财神快速点击，能把自己的元宝变成负的，加一字段以限制*/
    bool isCanClicked;
    
    SGButton *btn;
    int nowTime;
    int chance;
    int cost;
    int addvalue;
    int scrollHeight;
    CCLayer *scrollLayer;
    CCScrollView *scroll;
    CCArray *scrollArray;
    SGCCLabelTTF *lab_reward_notice;
    SGCCLabelTTF *lab_once_notice;
private:
    void initView(int a,int b,int c);
    void backHandler();
    void getCoins();
    void updateTime();
    void regListener(cocos2d::CCObject *obj);
    void labanimation();
    void animation1();
    void animation2();
    void animation3();
    void animation4();
    void animation5();
    void updatePlayers();
    void updatecount();
    void resumeAction();
    int leftcount;
    
    CCSprite *title_bg;
    void jinbiAction();
    void jinbiAction1();
    void jinbiAction2();
    void removejinb(CCNode *node);
    void stopjinbiAct();
public:
    virtual void setIsCanTouch(bool isTouch);
    SGRewardRegLayer();
    ~SGRewardRegLayer();
    static SGRewardRegLayer *create(int a,int b,int c,CCArray *bscArr);
    
};


#endif /* defined(__GameSanGuo__SGRewardRegLayer__) */
