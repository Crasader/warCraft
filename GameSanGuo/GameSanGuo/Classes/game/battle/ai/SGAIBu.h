//
//  SGAIBu.h
//  GameSanGuo
//
//  Created by geng on 13-3-28.
//
//

#ifndef __GameSanGuo__SGAIBu__
#define __GameSanGuo__SGAIBu__

#include "cocos2d.h"
#include "SGBattleMap.h"
#include "SGAi.h"
USING_NS_CC;
class SGAIBu : public CCObject
{
private:
    static int findNum(SGBattleMap *battleMap, int color, int noj);
public:
    static  bool bu(SGBattleMap *battleMap);
};

#endif /* defined(__GameSanGuo__SGAIBu__) */
