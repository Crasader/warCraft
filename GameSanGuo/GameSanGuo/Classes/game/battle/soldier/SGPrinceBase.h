//
//  SGPrinceBase.h
//  GameSanGuo
//  二格英雄
//  Created by wenlong w on 12-11-23.
//
//

#ifndef __GameSanGuo__SGPrinceBase__
#define __GameSanGuo__SGPrinceBase__

#include "ShiBing.h"

//格子位置信息
enum sgGridPsnType
{
    //用于两格
    kup,//上
    kdown,//下
    //用于4格
    kleftup,//左上
    krightup,//右上
    kleftdown,//左下
    krightdown,//右下
};

class SGPrinceBase : public ShiBing
{
protected:
    //初始士兵所占用的格子
    virtual void initGrids(GameIndex index,sgShiBingType sbType);
    //将士兵移动到指定的下标
    virtual void moveToIndex(GameIndex index);
    //将士兵移动到指定的格子，移到动完成后回调
    virtual void moveToIndexCall(CCObject *obj,int speed, GameIndex index, SEL_CallFuncN selector);
public:
    SGPrinceBase();
    ~SGPrinceBase();
    
    static SGPrinceBase *createWithData(SGSBObj *data, SGBattleMap *bm, bool isme, bool isSpine = false);
    //根据格子位置 得到士兵的格子
    //1:左上的格子；2：右上的格子；3：左下的格子；4：右下的格子
    virtual SGGridBase* getNumGrid(sgGridPsnType index);
    
    
    //向场景派兵
    virtual void moveToSceneCall(CCObject *obj, SEL_CallFuncN selector);
    virtual void swapGridMove(SGBattleMap *battleMap,GameIndex index, SEL_CallFunc selector);
    
    virtual void attackAnimation(CCObject *obj, SEL_CallFunc selector);

};
#endif /* defined(__GameSanGuo__SGPrinceBase__) */
