//
//  AIFirstPriority.h
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-12.
//
//

#ifndef __GameSanGuo__AIFirstPriority__
#define __GameSanGuo__AIFirstPriority__

#include "cocos2d.h"
#include "SGBattleMap.h"
#include "SGAi.h"
#include "SGGridBase.h"
#include "SGSuperPrinceBase.h"
#define ARRAY_ROW 6
#define ARRAR_COLOM 4

USING_NS_CC;
extern SGGridBase *SBMOVEGRID; //防止重复移动

struct SBInfo {
    
    bool canMove;
    sgShiBingType sbtype;
    GameIndex sbIndex;//右下角的位置
      
};
class AIFirstPriority:public CCObject {
public:
    static ReturnIndex del1Get2(SGBattleMap  *bm);//删除一个得到两个攻击阵列的，返回删除的index.
    static ReturnIndex del1Get1(SGBattleMap *bm);//删除一个得到一个阵列的情况
    static ReturnIndex move1Get2(SGBattleMap *bm);//移动一个得到两个阵列的情况
    
    static ReturnIndex round1GetFour(SGBattleMap *bm);//本回合内得到四格武将
    
    static ReturnIndex  need1GetFour(SGBattleMap *bm);//需要一个行动力得到一个四格的攻击阵列
    static ReturnIndex need2GetFour(SGBattleMap *bm);//需要两个行动力得到一个四格武将的情况
    static ReturnIndex need3GetFour(SGBattleMap *bm);//需要三个行动力得到一个四格武将的情况
    
    static ReturnIndex round1GetTwo(SGBattleMap *bm); //本回合内得到二格武将
    
    static ReturnIndex  need1GetTwo(SGBattleMap *bm);//需要一个行动力得到一个二格的攻击阵列
    static ReturnIndex need2GetTwo(SGBattleMap *bm);//需要两个行动力得到一个二格武将的情况
    static ReturnIndex need3GetTwo(SGBattleMap *bm);//需要三个行动力得到一个二格武将的情况
  
    static ReturnIndex move1Get1(SGBattleMap *bm);// 移动一个得到一个小兵攻击阵列
    static ReturnIndex makeFastForOne(SGBattleMap *bm);// 移动一个得到一个小兵攻击阵列
 
    static int del1Get2List[][ARRAY_ROW][ARRAR_COLOM];//所有的一格四格二格删1得2的情况列表
    static int del1Get1List[][ARRAY_ROW][ARRAR_COLOM];//所有的一格四格二格删1得1的情况列表
    
    static int move1Get2List[][ARRAY_ROW][ARRAR_COLOM];//移动一个得到两个阵列的的情况列表，两格和四格
    static int move1Get2F1List[][ARRAY_ROW][ARRAR_COLOM];//移动一个得到两个攻击阵列中的一格的情况
    //有一个行动力情况下的
    static int n1C1F4[][ARRAY_ROW][ARRAR_COLOM];//消耗一个得到一个四格攻击阵列列表
    static int n1C1F2[][ARRAY_ROW][ARRAR_COLOM];//消耗一个得到一个二格攻击阵列列表
    //有两个行动力的情况
    
    ///1.移动
    static int n2C1F4M[][ARRAY_ROW][ARRAR_COLOM];//消耗一个行动力得到一个四格的情况，
    static int n2C2F4M[][ARRAY_ROW][ARRAR_COLOM];//消耗两个行动力得到一个四格的情况
    static int n2C1F2M[][ARRAY_ROW][ARRAR_COLOM];//消耗一个行动力得到一个二个武将的情况
    static int n2C2F2M[][ARRAY_ROW][ARRAR_COLOM];//消耗两个行动力得到一个二格武将的情况
    
    //2.删除
    static int n2C1F4D[][ARRAY_ROW][ARRAR_COLOM];//消耗一个行动力得到一个四格的情况，
    static int n2C1F2D[][ARRAY_ROW][ARRAR_COLOM];//消耗一个行动力得到一个二个武将的情况
    static int n2C2F2D[][ARRAY_ROW][ARRAR_COLOM];//消耗两个行动力得到一个二格武将的情况
    
    //有三个行动力的情况
    
    //1.移动
    static int n3C2F4M[][ARRAY_ROW][ARRAR_COLOM];//消耗两个行动力得到一个四格武将的情况
    static int n3C3F4M[][ARRAY_ROW][ARRAR_COLOM];//消耗三个行动力得到一个四格武将的情况
    static int n3C2F2M[][ARRAY_ROW][ARRAR_COLOM];//消耗两个行动力得到一个二格武将的情况
    static int n3C3F2M[][ARRAY_ROW][ARRAR_COLOM];//消耗三格行动力得到一个二格武将的情况
    
    //2.删除
    static int n3C2F4D[][ARRAY_ROW][ARRAR_COLOM];//消耗两个行动力得到一个四格武将的情况
    static int n3C2F2D[][ARRAY_ROW][ARRAR_COLOM];//消耗两个行动力得到一个二格武将的情况
    static int n3C3F2D[][ARRAY_ROW][ARRAR_COLOM];//消耗三格行动力得到一个二格武将的情况
    
    
    //其他的也有包含，下面两个不绝对
    static int n4C4F4M[][ARRAY_ROW][ARRAR_COLOM];//消耗四格行动力得到一个四格攻击阵列
    static int n4C3F4M[][ARRAY_ROW][ARRAR_COLOM];//消耗三个行动力得到一个四格攻击阵列
    
    //组小兵
    static int n1C1F1M[][ARRAY_ROW][ARRAR_COLOM]; //消耗一个行动力得到小兵阵列
    static int makeFastF1[][ARRAY_ROW][ARRAR_COLOM]; //消耗行动力得到小兵阵列
    
   
    /*
    static int list4[][ARRAY_ROW][ARRAR_COLOM];//完成四格消耗一个行动力完成的情况列表
    static int list5[][ARRAY_ROW][ARRAR_COLOM];//完成四格消耗两个行动力的情况列表
    static int list6[][ARRAY_ROW][ARRAR_COLOM];//完成四格消耗三格行动力的情况
    
    //本回合内完成二格武将的情况
    static int list7[][ARRAY_ROW][ARRAR_COLOM];//本回合内完成二格武将的情况
    */
    //枚举类型的逻辑
    static ReturnIndex checkForSB(SGBattleMap *bm,int list[][4],ShiBing *SB,sgShiBingType sbType);
   // static ReturnIndex checkWithList(SGBattleMap *bm,int SBType);
    static ReturnIndex checkWithSBTypeAndList(SGBattleMap *bm,sgShiBingType SBType,int list[][ARRAY_ROW][ARRAR_COLOM],int listCount);
    static ReturnIndex checkAllWithList(SGBattleMap *bm,int list[][ARRAY_ROW][ARRAR_COLOM],int listCount);
   
    //最快组成的逻辑
    static ReturnIndex fastMakeAttackForSB(SGBattleMap *bm,sgShiBingType SBType);
    static ReturnIndex checkForDeletePreSB(SGBattleMap *bm,GameIndex SBIndex,sgShiBingType sbType);//删除武将前面的可以得到可以组武将的条件
    static ReturnIndex checkForDeleteLastSB(SGBattleMap *bm,GameIndex SBIndex,sgShiBingType sbType,int colorID);//删除武将后面的不同颜色的
    static ReturnIndex checkForMoveLastSB(SGBattleMap *bm,GameIndex SBIndex,sgShiBingType SBType,int colorID);//移走武将后面不同颜色的
    static GameIndex getToIndexWithSB(SGBattleMap *bm,int colorID,GameIndex SBIndex,sgShiBingType SBType);//移动武将后面的到其他列
    static GameIndex getFromIndexWithSB(SGBattleMap *bm,int colorID,GameIndex SBIndex,sgShiBingType SBType,int nMove=-1,int nMove1=-1); //从其他列找到可移动的
    static bool checkIsHasAttack(SGBattleMap *bm,int indexJ,int endI);//检查某行是否有攻击阵列
    static GameIndex getDeleteIndex(SGBattleMap *bm,GameIndex sbIndex,int colorID);//得到删除武将后面的不同颜色的Index
    static ReturnIndex getSBByDig(SGBattleMap *bm ,int colorID,GameIndex sbIndex,sgShiBingType sbType);
    
    
    //移动武将
    
    static ReturnIndex moveWJ(SGBattleMap *bm);
    static SBInfo getNextWJ(SGBattleMap *bm,SBInfo sbInfo);//得到武将后面的武将的武将的index
    static GameIndex getToIndexForWJ(SGBattleMap *bm,SBInfo preWJInfo,SBInfo nextWJInfo);//移动武将需要到的位置
    static bool checkWJNextISNULL(SGBattleMap *bm,GameIndex sbIndex);//判断四格武将后面是不是都是空得格子
    static GameIndex getBaseIndex(SGBattleMap *bm,GameIndex sbIndex);//由一个最后面的格子判断武将右下角的格子
    
    //防止重复移动
    static bool isRepeatMove(GameIndex fromIndex,GameIndex toIndex,SGBattleMap *bm);
    //组三格
    static  ReturnIndex fastMake3(SGBattleMap *bm);
};

#endif /* defined(__GameSanGuo__AIFirstPriority__) */




















