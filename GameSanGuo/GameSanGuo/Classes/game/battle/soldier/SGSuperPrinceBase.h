//
//  SGSuperPrinceBase.h
//  GameSanGuo
//  4格英雄
//  Created by wenlong w on 12-11-23.
//
//

#ifndef __GameSanGuo__SGSuperPrinceBase__
#define __GameSanGuo__SGSuperPrinceBase__

#include "SGPrinceBase.h"
#include "SGBattleManager.h"

class SGSuperPrinceBase :public SGPrinceBase
{
protected:
    //初始士兵所占用的格子
    virtual void initGrids(GameIndex index,sgShiBingType sbType);
    //将士兵移动到指定的下标
    virtual void moveToIndex(GameIndex index);
    //将士兵移动到指定的格子，移到动完成后回调
    virtual void moveToIndexCall(CCObject *obj,int speed, GameIndex index, SEL_CallFuncN selector);
public:
    SGSuperPrinceBase();
    ~SGSuperPrinceBase();
    static SGSuperPrinceBase *createWithData(SGSBObj *data, SGBattleMap *bm, bool isme, bool isSpine = false);
    virtual void initSoldierPsn();
    virtual bool isAtLeft(int indexj);
    
    
    //向场景派兵
    virtual void moveToSceneCall(CCObject *obj, SEL_CallFuncN selector);
    
    virtual void swapGridMove(SGBattleMap *battleMap,GameIndex index, SEL_CallFunc selector);
    
    virtual void attackAnimation(CCObject *obj, SEL_CallFunc selector);
private:
    CCPoint getSuperSoldierPS();
};

#endif /* defined(__GameSanGuo__SGSuperPrinceBase__) */
