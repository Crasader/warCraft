//
//  SGCardAttribute.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-5-22.
//
//

#ifndef __GameSanGuo__SGCardAttribute__
#define __GameSanGuo__SGCardAttribute__

#include "cocos2d.h"
#include "SGCCLabelTTF.h"
#include "SGPlayerInfo.h"

#define WU_RATE 1.1
#define OTHER_RATE 1.1
USING_NS_CC;

//enum AttributeType
//{
//    card_atk,
//    card_def,
//    card_round,
//    card_speed,
//    card_mor,
//    card_gov
//};
class SGCardAttribute : public CCObject
{
public:
    static CCDictionary * getValue(int currlvl,int itemid);
    static CCArray *spliceStr(const char*str);
};

#endif /* defined(__GameSanGuo__SGCardAttribute__) */
