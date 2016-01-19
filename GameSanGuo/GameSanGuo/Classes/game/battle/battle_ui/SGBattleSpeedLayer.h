//
//  SGBattleSpeedLayer.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-19.
//
//

/**
 
    计算双方玩家速度，判断谁先出手
 
 **/

#ifndef __GameSanGuo__SGBattleSpeedLayer__
#define __GameSanGuo__SGBattleSpeedLayer__

#include <iostream>
#include "SGBaseLayer.h"
#include "SGCCLabelTTF.h"
#include "cocos2d.h"
using namespace cocos2d;

class SGBattleSpeedLayer : public SGBaseLayer
{
private:
    /*  *   敌方速度  * 我方速度  */
    int m_enemySpeed;
    int m_ourSpeed;
    bool m_bFirst;
    
    //定时计数器
    int m_tick;
    
    SGCCLabelTTF *m_pEnemyLabel;
    SGCCLabelTTF *m_pOurLabel;
    
    /* 先手 */
    CCSprite *m_pFirstSprite;
    
    /* 四条金线 两个黑框 */
    CCSprite *m_pGoldenLineUp_1;
    CCSprite *m_pGoldenLineUp_2;
    CCSprite *m_pGoldenLineDown_1;
    CCSprite *m_pGoldenLineDown_2;
    CCSprite *m_pEnemyBGSprite;
    CCSprite *m_pOurBGSprite;
    
    /* 速度计数器 */
    void updateSpeedCounter();
    /* 启动定时器 */
    void startTimer();
    
    void removeSelf();
    
    void  viewDidLoad();
    
    void showGoldenLine();
    
    void showBlackBG();
    
    void showSpeedLabel();
    
    void beginAI();
public:
    SGBattleSpeedLayer();
    ~SGBattleSpeedLayer();
    static SGBattleSpeedLayer *create(int ourSpeed,int enemySpeed,bool first);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

    
};


#endif /* defined(__GameSanGuo__SGBattleSpeedLayer__) */
