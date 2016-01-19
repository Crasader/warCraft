//
//  SGGridBase.h
//  GameSanGuo
//  格子类
//  Created by gengshengcai on 12-11-19.
//
//

#ifndef __GameSanGuo__SGGridBase__
#define __GameSanGuo__SGGridBase__

#include "GameConfig.h"
#include "cocos2d.h"

using namespace cocos2d;
class SGBattleMap;
typedef enum {
    /// 普通
    knormal = 1,
    //攻击
    kattack,
    // 防御
    kdefend,
} sgGridType;

typedef enum {
    /// 散兵
    ksbone,
    /// 两格武将
    ksbtwo,
    
    /// 4格武将
    ksbfour,
    
} sgShiBingType;


class SG_BuffBase;
class SGGridBase : public CCObject
{
public:
    CC_SYNTHESIZE(GameIndex, m_pIndex, Index);
    CC_SYNTHESIZE(sgShiBingType, m_sbType, SbType);
    CC_SYNTHESIZE(int , m_saveID, SaveID);//当前格子上的兵种ID
    CC_SYNTHESIZE(int , m_saveColorID, SaveColorID);//当前格子的兵种艳色ID
    
    CCArray *m_baffObj;
    

public:
    static SGGridBase* createWithType(sgGridType type);
    bool initWithType(sgGridType type);

    SGGridBase();
    ~SGGridBase();

    void setStype(sgGridType type);
    sgGridType getStype();
    virtual void moveToIndex(GameIndex index);
    virtual void moveToBottom();
    virtual void moveToPos(SGBattleMap *battleMap,CCPoint moveToPos);
//    CCPoint getGridPoint(int i, int j);
    
    void  *memmove(void *dest, const void *src, size_t count);
    
    void initBaffObj();
    
    CCArray *getBuffList();
    SG_BuffBase* getIndexBuff(int rand);
    bool setBuffListObj(SG_BuffBase* buff);
    
private:
    sgGridType sType;
};
#endif /* defined(__GameSanGuo__SGGridBase__) */
