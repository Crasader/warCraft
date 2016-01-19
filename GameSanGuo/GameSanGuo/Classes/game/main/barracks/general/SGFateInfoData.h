//
//  SGFateInfoData.h
//  GameSanGuo
//
//  Created by Fygo Woo on 11/17/13.
//
//

#ifndef GameSanGuo_SGFateInfoData_h
#define GameSanGuo_SGFateInfoData_h

#include "cocos2d.h"
#include "SGOfficerCard.h"
#include "SGFateBase.h"

class SGFateInfoData : public CCObject
{
public:
    //武将卡数据
    CC_SYNTHESIZE(SGOfficerCard*, m_card, Card);
    
    //缘分数据
    CC_SYNTHESIZE(int, m_fateId, Id);
    
    //缘分状态
    CC_SYNTHESIZE(int, m_fateState, State);
};

#endif
