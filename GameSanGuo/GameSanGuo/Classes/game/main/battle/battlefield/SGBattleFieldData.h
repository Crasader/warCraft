//
//  SGBattleFieldData.h
//  GameSanGuo
//
//  Created by wwl on 13-10-8.
//
//

#ifndef __GameSanGuo__SGBattleFieldData__
#define __GameSanGuo__SGBattleFieldData__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class SGBattleFieldData : public CCObject {
    
    
public:
    SGBattleFieldData();
    ~SGBattleFieldData();

public:
CC_SYNTHESIZE(int , m_battleFieldType, BattleFieldType);//战场类型1是新手2是高手
CC_SYNTHESIZE_RETAIN(CCString*, m_battleFieldName, BattleFieldName);//战场名字
CC_SYNTHESIZE_RETAIN(CCString*, m_battleFieldTip, BattleFieldTip);//战场提示


};

#endif /* defined(__GameSanGuo__SGBattleFieldData__) */
