//
//  SGStoryBattleLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-7.
//
//

#ifndef __GameSanGuo__SGStoryBattleLayer__
#define __GameSanGuo__SGStoryBattleLayer__

#include "SGBaseBox.h"
#include "SGBoxDelegate.h"
#include "SGBaseLayer.h"

class SGStoryBattleLayer : public SGBaseLayer
{
public:
    SGStoryBattleLayer(void);
    ~SGStoryBattleLayer(void);
    
    static SGStoryBattleLayer* creat();
    
private:
    void initView();
    void fighting(CCPoint position);
    void afterFighting(CCNode *sender);
    void gameOver(CCNode *sender);
    void afterGameOver(CCObject *sender);
};

#endif /* defined(__GameSanGuo__SGStoryBattleLayer__) */
