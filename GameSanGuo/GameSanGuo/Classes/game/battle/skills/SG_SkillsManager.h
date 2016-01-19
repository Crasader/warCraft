//
//  SG_SkillsManager.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-11.
//
//

#ifndef __GameSanGuo__SG_SkillsManager__
#define __GameSanGuo__SG_SkillsManager__

#include <iostream>
#include "cocos2d.h"
//#include "SG_SkillsBase.h"
#include "GameConfig.h"
#include "ShiBing.h"


using namespace cocos2d;

//以前FlipY的展现是错误的，为了兼容以前的问题，现在需要修正2个格子的距离。
const static int OLD_EFFECT_FLIPY_FIX = -2;

typedef enum
{
    SKILLS_ID1 = 10000,
    SKILLS_ID2,
    SKILLS_ID3,
    SKILLS_ID4,
    SKILLS_ID5,
    SKILLS_ID6,
    SKILLS_ID7,
    SKILLS_ID8,
    SKILLS_ID9,
    SKILLS_ID10,
    SKILLS_ID11,
    SKILLS_ID12,
    SKILLS_ID13,
    SKILLS_ID14,
    SKILLS_ID15,
    SKILLS_ID16,
    SKILLS_ID17,
    SKILLS_ID18,
    SKILLS_ID19,
    SKILLS_ID20,
    SKILLS_ID21,
    SKILLS_ID22,
    SKILLS_ID23,
    SKILLS_ID24,
    SKILLS_ID25,
    SKILLS_ID26,
    SKILLS_ID27,
    SKILLS_ID28,
    SKILLS_ID29,
    SKILLS_ID30,
    SKILLS_ID31,
    SKILLS_ID32,
    SKILLS_ID33,
    SKILLS_ID34,
    SKILLS_ID35,
    SKILLS_ID36,
    SKILLS_ID37,
    SKILLS_ID38,
    SKILLS_ID39,
    SKILLS_ID40,
    SKILLS_ID41,
    SKILLS_ID42,
    SKILLS_ID43,
    SKILLS_ID44,
    SKILLS_ID45,
    SKILLS_ID46,
    SKILLS_ID47,
    SKILLS_ID48,
    SKILLS_ID49,
    SKILLS_ID50,
    SKILLS_ID51,
    SKILLS_ID52,
    SKILLS_ID53,
    SKILLS_ID54,
    SKILLS_ID55,
    SKILLS_ID56,
    SKILLS_ID57,
    SKILLS_ID58,
    SKILLS_ID59,
    SKILLS_ID60,
    SKILLS_ID61,
    SKILLS_ID62,
    SKILLS_ID63,
    SKILLS_ID64,
    SKILLS_ID65,
    SKILLS_ID66,
    SKILLS_ID67,
    SKILLS_ID68,
    SKILLS_ID69,
    SKILLS_ID70,
    SKILLS_ID71,
    SKILLS_ID72,
    SKILLS_ID73,
    SKILLS_ID74,
    SKILLS_ID75,
    SKILLS_ID76,
    SKILLS_ID77,
    SKILLS_ID78,
    SKILLS_ID79,
    SKILLS_ID80,
    SKILLS_ID81,
    SKILLS_ID82,
    SKILLS_ID83,
    SKILLS_ID84,
    SKILLS_ID85,
    SKILLS_ID86,
    SKILLS_ID87,
    SKILLS_ID88,
    SKILLS_ID89,
    SKILLS_ID90,
    SKILLS_ID91,
    SKILLS_ID92,
    SKILLS_ID93,
    SKILLS_ID94,
    SKILLS_ID95,
    SKILLS_ID96,
    SKILLS_ID97,
    SKILLS_ID98,
    SKILLS_ID99,
    SKILLS_ID100,
    SKILLS_ID30000 = 30000,
    SKILLS_ID30001,
    
}SG_SkillsType;

class SG_SkillsBase;
class SGBattleMap;
class SGAttackList;
class SG_SkillsManager: public CCLayer {
    
public:
    SG_SkillsManager();
    ~SG_SkillsManager();
    
    static SG_SkillsManager* initskManager(int skid);
    SG_SkillsBase* setSkillsType(int skid);
    void activateSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num);
    bool activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, float damage);
    bool activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activateSkills1_1(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index);
    bool activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activateSkills4(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activatePassivitySkills(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
    int assistPassivitySkills(int ap, SGAttackList *attackList, float num);
    void activatePassivitySkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
    int activateSkills5(SGBattleMap *battleMap, SGAttackList *attackList, float num, ShiBing *sb);
    int activateSkills5_1(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activateSkills6(SGBattleMap *battleMap, SGAttackList *attackList, float num);//遇强则强技能
    void activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void activateSkills8(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1,int damage);//左右溅射类技能触发及后溅射
    void activateSkills9(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
    void activateSkills10(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    bool activateSkills11(SGBattleMap *battleMap, SGAttackList *attackList, ShiBing *sb);
    void activateSkills12(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1);
    void detectionReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void chackReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, SGAttackList *attackList1);
    void removeReviveAttack(SGBattleMap *battleMap, SGAttackList *attackList);
    
#pragma mark - buff rfce
    void activateBuffSkills(SGBattleMap *battleMap, SGAttackList *attackList, GameIndex index, GameIndex index1, int row = 0);
    void activateBuffSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
    
    
    
    
    
    bool assistSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
    
    
    void pushSkillData(SGAttackList *attackList, bool isbsk = false);
    
public:
    CC_SYNTHESIZE(SG_SkillsBase*, m_skBase, skBase);
    
};



#endif /* defined(__GameSanGuo__SG_SkillsManager__) */
