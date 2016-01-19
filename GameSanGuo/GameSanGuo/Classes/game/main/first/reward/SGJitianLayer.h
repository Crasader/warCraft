//
//  SGJitianLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-16.
//
//

#ifndef __GameSanGuo__SGJitianLayer__
#define __GameSanGuo__SGJitianLayer__

#include "SGBaseLayer.h"
#include "SGCCLabelTTF.h"
class SGJitianLayer : public SGBaseLayer
{
private:
    int curr;
    int maxx;
    int iscan;
    CCLabelTTF *label1;
    SGButton *jitianBtn;
    SGButton *backBtn;
    CCSprite * redBg;
    //保存奖励列表，以备查询。
    CCArray* _rewardList;
private:
    void initView(CCArray*array);
    void backHandler();
    void confirmHandler();
    void godListener(cocos2d::CCObject *obj);
    void showequipinfo(CCNode*node);
    void initReward(CCArray*array);
public:
    SGJitianLayer();
    ~SGJitianLayer();
    static SGJitianLayer *create(int current, int max,int cando,CCArray*array);
};


#endif /* defined(__GameSanGuo__SGJitianLayer__) */
