//
//  SGSjzData.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 11/27/13.
//
//

#include "SGSjzData.h"

//构造
SGSjzData::SGSjzData() : _levelCur(0), _countCur(0), _countMax(0), _rankCur(0), _damageMax(100)
{
    _soulAmount = 0;
    _soulCurOfA = 0;
    _soulCurOfB = 0;
    _soulCurOfS = 0;
    _soulMaxOfA = 500;
    _soulMaxOfB = 200;
    _soulMaxOfS = 1000;
    _isShowBonus = false;
    _bonusDamage = 0;
    _bonusSoul = 0;
    _dailyIngot=0;
}

//析构
SGSjzData::~SGSjzData()
{
    ;
}

//创建
SGSjzData* SGSjzData::create()
{
    SGSjzData* obj = new SGSjzData();
    if (obj)
    {
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

//根据索引获得某箱子的最大军魂值
int SGSjzData::getSoulMaxByIndex(SoulTag idx)
{
    int ret = 0;
    if (idx == ST_SSS || idx == ST_AAA || idx == ST_BBB)
    {
        switch (idx) {
            case ST_SSS:
                ret = getSoulMaxOfS();
                break;
            case ST_AAA:
                ret = getSoulMaxOfA();
                break;
            case ST_BBB:
                ret = getSoulMaxOfB();
                break;
            default:
                break;
        }
    }
    else
        CCLOG(">>>>error: SGSjzData::getSoulMaxByIndex(SoulTag idx)");
    return ret;
}

//根据索引获得某箱子的当前军魂值
int SGSjzData::getSoulCurByIndex(SoulTag idx)
{
    int ret = 0;
    if (idx == ST_SSS || idx == ST_AAA || idx == ST_BBB)
    {
        switch (idx) {
            case ST_SSS:
                ret = getSoulCurOfS();
                break;
            case ST_AAA:
                ret = getSoulCurOfA();
                break;
            case ST_BBB:
                ret = getSoulCurOfB();
                break;
            default:
                break;
        }
    }
    else
        CCLOG(">>>>error: SGSjzData::getSoulCurByIndex(SoulTag idx)");
    return ret;
}

//根据索引设置某箱子的当前军魂值
void SGSjzData::setSoulCurByIndex(SoulTag idx, int num)
{
    if (idx == ST_SSS || idx == ST_AAA || idx == ST_BBB)
    {
        switch (idx)
        {
            case ST_SSS:
                setSoulCurOfS(num);
                break;
            case ST_AAA:
                setSoulCurOfA(num);
                break;
            case ST_BBB:
                setSoulCurOfB(num);
                break;
            default:
                break;
        }
    }
    else
        CCLOG(">>>>error: SGSjzData::setSoulCurByIndex(SoulTag idx, int num)");
}
