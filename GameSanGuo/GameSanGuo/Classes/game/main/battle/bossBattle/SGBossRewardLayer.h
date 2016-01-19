//
//  SGBossRewardLayer.h
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-28.
//
//

#ifndef __GameSanGuo__SGBossRewardLayer__
#define __GameSanGuo__SGBossRewardLayer__

#include <iostream>
#include "SGBaseLayer.h"

struct BossReward {
    int damage;
    int coinNumber;
    int MedalNumber;
    
    bool isKillBoss;
};
class SGBossRewardLayer:public SGBaseLayer {
    
public:
    SGBossRewardLayer();
    ~SGBossRewardLayer();
    static SGBossRewardLayer *create(BossReward *data);
private:
     void initView(BossReward *data);
    bool iskillBoss;
protected:
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void    showBattleDetailLayer();
};

#endif /* defined(__GameSanGuo__SGBossRewardLayer__) */
