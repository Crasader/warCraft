//
//  ShiBing.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-22.
//
//

#ifndef __GameSanGuo__ShiBing__
#define __GameSanGuo__ShiBing__

#include "SGGridBase.h"
#include "SGBattleMap.h"
#include "SGSoldier.h"

//cgpSpine
#include "cocos-ext.h"
USING_NS_CC_EXT;



#define     ANIMA_TIME_NUM  1.0   //几倍速
#define     SpineSpeed     1    //spine动画快慢系数

typedef enum
{
    ACT_wait = 0,      //待机
    ACT_xuli = 1,          //准备
    ACT_moveDown = 3,          //向下移动
    ACT_atkDown = 4,          //向下攻击
    ACT_moveUp = 7,          //向上移动
    ACT_atkUp = 8,          //向上攻击
    ACT_null = 10,         //空
    
}Action_Sequnce;

static char acActionNames[11][30] =
{
    "idle",
    "chuli",
    "",    //2
    "run_down",
    "atk_down",
    "",
    "", //6
    "run_up",
    "atk_up",
    "",   //9
    ""  //10
};


//typedef enum
//{
//    ACT_wait = 0,      //待机
//    ACT_xuli ,          //准备 1
//    ACT_moveDown,          //向下移动2
//    ACT_atkDown,          //向下攻击3
//    ACT_moveUp,          //向上移动
//    ACT_atkUp,          //向上攻击
//    ACT_null,         //空
//    
//}Action_Sequnce;
//
//static char acActionNames[11][30] =
//{
//    "idle",
//    "chuli",
//    "run_down",
//    "atk_down",
//    "run_up",
//    "atk_up",
//};


class ShiBing :public SGSoldier
{
public:
    CC_SYNTHESIZE(int, sbId, SbId);
    
    CC_SYNTHESIZE(sgShiBingType, sbType, SbType);//士兵的类型
    CC_SYNTHESIZE_RETAIN(CCArray*, m_princeGridArray, PrinceGridArray);//存储当前英雄占用的格子
    CC_SYNTHESIZE(GameIndex, sg_Sbindex, Sbindex);
    CCDictionary *sbInitData;
    

protected:
    //初始士兵所占用的格子
    virtual void initGrids(GameIndex index,sgShiBingType sbType);
    //初始士兵的位置
    virtual void initSoldierPsn();
    
    void setSbReceive();
    
public://用于继承
    ShiBing();
    ~ShiBing();
    static ShiBing *createWithData(SGSBObj *data,SGBattleMap *bm, bool isme, bool isSpine = false);   //cgpSpine
    virtual bool initWithData(SGSBObj *data, SGBattleMap *bm, bool isme, bool isSpine);  //cgpSpine
    void setanimaID(int _id , bool isLoop = false);  //cgpSpine
    //判断这个格子是否为此士兵的
    bool isGridIn(SGGridBase *grid);
    
    //将士兵移动到指定的下标
    virtual void moveToIndex(GameIndex index);
    //将士兵移动到指定的格子，移到动完成后回调
    virtual void moveToIndexCall(CCObject *obj,int speed, GameIndex index, SEL_CallFuncN selector);
    
    //将士兵移动到准备区域
    virtual void moveToBottom(CCObject *obj ,SEL_CallFunc selector);
    //向场景派兵
    virtual void moveToSceneCall(CCObject *obj, SEL_CallFuncN selector);
    void runFade();
    
    void changSBState(sgGridType gridtype);

    
    virtual void swapGridMove(SGBattleMap *battleMap,GameIndex index, SEL_CallFunc selector);

    void setBingZOrder();
    virtual void attackAnimation(CCObject *obj, SEL_CallFunc selector);
    
    void drawSynthesisDefend(int defendLimit);
    void setDefImage();
    
    void showDef(int def);
    void hideDef();
    
    void showXuLiEffect(sgShiBingType type);
    void hideXuLiEffect();
    void showAddDefEffect(sgShiBingType type);
    void hideAddDefEffect();
    void showEffect(int num, bool isRelease = true);
    
    float removeSbEffect();
    
    void setShadowDraw(bool isdraw);
    
    
    void resetSbData();//重置兵数据
    virtual void resetSbGrid(); //以后供其它子类分别实现
    
    void removeSBGrid();
    
    
    void removePlist(CCObject *obj, CCString *str);
    void show_Buff_Effect();
    void removeBuffEffect();
    
public:
    SGGridBase *getOnlyGrid();
    GameIndex getGridIndex();

protected:
    SGBattleMap *battleMap;
    bool ismy;
    CCLabelTTF * defLabel;
    
    CCSprite * shadow_sp;
    
//    CCSpriterX * fermEffect;
//    CCSpriterX * fermEffect1;
    Action_Sequnce sequnceNum;
//    CCSpriterX *colorFlame;
public:
    void setLoopAction(CCObject *obj ,Action_Sequnce sequnce);
    void setMoveAction();
    void setAttAction( CCObject *obj, Action_Sequnce sequnce);
    void cleanSbFile();
    void removeColorFlame();
    void effPlay(CCObject *obj);
    
    bool isMaxDef();

};
#endif /* defined(__GameSanGuo__ShiBing__) */
