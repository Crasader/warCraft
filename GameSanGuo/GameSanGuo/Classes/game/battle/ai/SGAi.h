//
//  SGAi.h
//  GameSanGuo
//
//  Created by geng on 13-3-19.
//
//

#ifndef GameSanGuo_SGAi_h
#define GameSanGuo_SGAi_h
#include "GameConfig.h"

enum ReturnType
{
    rt_null,//不符合条件
    rt_notNull,
    rt_del4attack,
    rt_del1,//删除一个
    rt_del2,//删除两个
    rt_from1,//一个可以移动的
    rt_from2,//两个可以移动的
    rt_from3,//两个可以移动的
    rt_from4,//两个可以移动的
    rt_to1,//可以移动到的
    rt_mov1del1,//先移动一个，后删除一个//10
    rt_to3del0,
    rt_to2del0,
    rt_to1del1,//选移动到一个点，后删除一个点
    rt_to2del1,
    rt_to1del2,
    rt_del1to1,//先删除后移动
    rt_from1to1,//17 从一个地方移动到另一个地方
    rt_from2to2,//从两个地方移动到另两个地方
    rt_from3to3,
    rt_from4to4,
    rt_bu //补兵

};

struct ReturnIndex
{
    ReturnIndex() : type(rt_null), colorId(0), noIndexj1(0), noIndexj2(0), noIndexj3(0), needAction(0)
    {};
    
    ReturnType type;
    int colorId;
    GameIndex delIndex1;
    GameIndex delIndex2;
    
    
    GameIndex from1;
    GameIndex from2;
    GameIndex from4;
    GameIndex from3;
    
    GameIndex to1;
    GameIndex to2;
    GameIndex to3;
    GameIndex to4;
    
    int noIndexj1;
    int noIndexj2;
    int noIndexj3;
    
    GameIndex selectIndex;
    
    int needAction;//需要的回合数
    
    
};

#endif
