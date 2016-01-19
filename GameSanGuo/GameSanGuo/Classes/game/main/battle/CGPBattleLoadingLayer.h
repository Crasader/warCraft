//
//  CGPBattleLoadingLayer.h
//  GameSanGuo
//
//  Created by caoguoping on 15/12/17.
//
//

#ifndef __GameSanGuo__CGPBattleLoadingLayer__
#define __GameSanGuo__CGPBattleLoadingLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SGUIHelper.h"
#include "SGCCLabelTTF.h"
#include "SGLayout.h"
#include "SGBaseLayer.h"
USING_NS_CC_EXT;
USING_NS_CC;
using namespace cocos2d::ui;

#endif /* defined(__GameSanGuo__CGPBattleLoadingLayer__) */

class CGPBattleLoadingLayer : public SGBaseLayer
{
public:
    int displayOfficerMax  ;    //loading界面最大的介绍主将
    int updateTimes;
    int fileId;
    LoadingBar*   barLoading;
    CCSkeletonAnimation*   spineHero;
    CCSkeletonAnimation*  lightEff;
    CC_SYNTHESIZE(float, allDelayTime, AllDelayTime)
    
    
public:
    CGPBattleLoadingLayer(void);
    ~CGPBattleLoadingLayer(void);
    
    static CGPBattleLoadingLayer* create();
    void initView();
    void loadingBarUpdate(float times);
    void updateSplit(float times);
    void delayCallSplit();
    void restartUpdate();
    void reupdateLoading(float times);
    
public:
   
    void showSplitLight();   //刀光
    void splitScreen();      //屏幕裂开
    void delayFight();    //继续战斗
    void gotoFightLayer();   //进入战斗


};
