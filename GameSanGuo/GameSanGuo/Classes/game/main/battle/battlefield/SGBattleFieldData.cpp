//
//  SGBattleFieldData.cpp
//  GameSanGuo
//
//  Created by wwl on 13-10-8.
//
//

#include "SGBattleFieldData.h"

SGBattleFieldData::SGBattleFieldData()
:m_battleFieldType(0),
m_battleFieldName(NULL),
m_battleFieldTip(NULL)
{
    
}


SGBattleFieldData::~SGBattleFieldData()
{
    CC_SAFE_RELEASE(m_battleFieldName);
    CC_SAFE_RELEASE(m_battleFieldTip);
}