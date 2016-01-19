//
//  SGBattleChangeRoundLayer.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-27.
//
//

//----------------------------------------------
//  战斗中切换回合的动画
//----------------------------------------------


#define ANIMATION_STEP_WIDTH SGLayout::getSkewing(75)

#ifndef __GameSanGuo__SGBattleChangeRoundLayer__
#define __GameSanGuo__SGBattleChangeRoundLayer__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
class SGBattleChangeRoundLayer : public CCLayer
{
private:
    //是不是我的回合
    bool m_bIsmyRound;
    
    //上下边角
    CCSprite *m_pUpBorderSprite;
    CCSprite *m_pDownBorderSprite;
    //黑色背景
    CCSprite *m_pBackgroundSprite;
    //谁的回合
    CCSprite *m_pIsmyRoundSprite;
    
    bool init();
    
    //动画步骤
    void animation_1();
    void animation_2();
    void animation_3();
    void animation_4();
    void animation_5();
    void animation_6();
    void animation_7();
    void animation_8();
    //移除界面
    void removeSelf();
    //改变背景黑条
    void changeBG_1();
    void changeBG_2();

    
public:
    SGBattleChangeRoundLayer();
    ~SGBattleChangeRoundLayer();
    static SGBattleChangeRoundLayer *create(bool ismyRound);
    void removeRoundEff(CCObject *obj);

};


#endif /* defined(__GameSanGuo__SGBattleChangeRoundLayer__) */
