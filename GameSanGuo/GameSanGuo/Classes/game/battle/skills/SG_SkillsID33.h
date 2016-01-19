//
//  SG_SkillsID33.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#ifndef __GameSanGuo__SG_SkillsID33__
#define __GameSanGuo__SG_SkillsID33__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID33: public SG_SkillsBase {
    
    
public:
    SG_SkillsID33();
    ~SG_SkillsID33();
    
//    virtual void activateSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    virtual void activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
    int attackGrid(SGBattleMap* map, SGGridBase * grid, int num);
    
    void removesb(cocos2d::CCObject *obj, int rand);
    void removeJian(CCObject *obj);
    
private:
    int attackAp;
    int attAp;
    SGBattleMap *enemyBattleMap;
    SGHeroLayer *hero;
    
};


#endif /* defined(__GameSanGuo__SG_SkillsID33__) */
