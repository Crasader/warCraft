//
//  SG_SkillsBase.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-11.
//
//

#ifndef __GameSanGuo__SG_SkillsBase__
#define __GameSanGuo__SG_SkillsBase__

#include <iostream>
#include "cocos2d.h"
#include "SGBattleMap.h"
#include "SGBattleLayer.h"
#include "SGHeroLayer.h"
#include "SGBattleManager.h"
#include "SGSoldier.h"
#include "SGGridBase.h"
#include "SGAttackList.h"
#include "SG_BuffManager.h"
#include "SG_BuffBase.h"


using namespace cocos2d;


class SG_SkillsBase: public CCNode {
    
    
public:
    
    SG_SkillsBase();
    ~SG_SkillsBase();
    
    void setHeroLayer();
    
    virtual void activateSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num);           //回合时
    virtual void activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num);
    virtual bool activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, float damage);          //攻击到时
    virtual bool activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    virtual void activateSkills1_1(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index);  //
    virtual bool activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num);          //atfm gc jf  trgq -- ngk cn fpgn
    virtual void activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num);          //攻击前
    virtual void activateSkills4(SGBattleMap *battleMap, SGAttackList *attackList, float num);          //加行动力
    virtual void activatePassivitySkills(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);  // 被动技能
    virtual int assistPassivitySkills(int ap, SGAttackList *attackList, float num);
    virtual void activatePassivitySkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    virtual int activateSkills5(SGBattleMap *battleMap, SGAttackList *attackList, float num, ShiBing *sb);          //秒杀散兵费血
    virtual int activateSkills5_1(SGBattleMap *battleMap, SGAttackList *attackList, float num);         //无损通过
    virtual void activateSkills6(SGBattleMap *battleMap, SGAttackList *attackList, float num);          //给自己加成
    virtual void activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num);          //开始蓄力
    virtual void activateSkills8(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1,int damage);    //远程武将
    virtual void activateSkills9(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);          //死亡时
    virtual void activateSkills10(SGBattleMap *battleMap, SGAttackList *attackList, float num);         //bqtq yuqfrfc lkwg ovt
    virtual bool activateSkills11(SGBattleMap *battleMap, SGAttackList *attackList, ShiBing *sb);         // 践踏
    virtual void activateSkills12(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1);
    virtual void detectionReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num);    //复活技能
    virtual void chackReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, SGAttackList *attackList1);        // 检测复活技能
    virtual void removeReviveAttack(SGBattleMap *battleMap, SGAttackList *attackList);
    
    #pragma mark - buff
    virtual void activateBuffSkills(SGBattleMap *battleMap, SGAttackList *attackList, GameIndex index, GameIndex index1, int row = 0);
    virtual void activateBuffSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
    virtual bool assistSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
    
    
public:
    CC_SYNTHESIZE(SGHeroLayer*, m_myHeroLayer, myHero);
    CC_SYNTHESIZE(SGHeroLayer*, m_enemyHeroLayer, enemyHero);
    
    
};





#endif /* defined(__GameSanGuo__SG_SkillsBase__) */
