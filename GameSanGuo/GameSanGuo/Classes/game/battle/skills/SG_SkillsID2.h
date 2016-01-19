//
//  SG_SkillsID2.h
//  GameSanGuo
//  SkillsID SKILLS_ID2
//  Created by wenlong w on 13-1-11.
//
//

#ifndef __GameSanGuo__SG_SkillsID2__
#define __GameSanGuo__SG_SkillsID2__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID2: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID2();
    ~SG_SkillsID2();
    
    virtual void activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
    int attackGrid(SGBattleMap* map, SGGridBase * grid, int num);
    void removesb(cocos2d::CCObject *obj, int rand);
    void removeJian(CCObject *obj);
    
public:
    
    int skillsId;
    int attackAp;
    int attAp;
    SGBattleMap *enemyBattleMap;
    SGHeroLayer *hero;
private:
    GameIndex index;
    void startRemoveSb();
};



#endif /* defined(__GameSanGuo__SG_SkillsID2__) */
