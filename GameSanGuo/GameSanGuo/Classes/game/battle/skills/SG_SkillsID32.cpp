//
//  SG_SkillsID32.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#include "SG_SkillsID32.h"


SG_SkillsID32::SG_SkillsID32()
{
    setHeroLayer();
}

SG_SkillsID32::~SG_SkillsID32()
{
    
}
//攻杀 蓄力阶段，每回合都对阵列前方敌人进行一次攻击。伤害为当前回合攻击力的2%
void SG_SkillsID32::activateSkills9(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    attackList1->drawSkillsEffect(7);
    int ap = attackList1->attackSubAp((attackList1->getMax_Ap() * num));
    if (ap > 0) {//如果没死,就改变其攻击力
        attackList1->changeAP(ap, false, true);
    }else{//死了就移除掉
        SGBattleMap *map = NULL;
        if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
        {
            map = getenemyHero()->getBattleMap();
        }else{
            map = getmyHero()->getBattleMap();
        }
        map->removeAttackList(attackList1);
    }
}

