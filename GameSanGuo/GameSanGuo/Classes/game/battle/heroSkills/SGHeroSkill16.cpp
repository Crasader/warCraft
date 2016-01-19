//
//  SGHeroSkill16.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill16.h"
SGHeroSkill16::SGHeroSkill16()
{
    
}

SGHeroSkill16::~SGHeroSkill16()
{
    
}

//使我方四格武将攻击阵列蓄力回合立刻降低3。（如果已经是1的不变）
void SGHeroSkill16::activateSkill_20024(SGHeroLayer *heroLayer, int value)
{
    CCArray *attacklist = heroLayer->getBattleMap()->getAttackList();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attacklist, obj)
    {
        SGAttackList *al = (SGAttackList *)obj;
        if (al->getAttackType() == kattackfour)
        {
            if ((al->getRoundV()- value) >0)
            {
                al->changeRound(-value, true);
                int a = al->getAddAp() * value;
                al->changeAP(al->getAp() + a);
            }
            else
            {
                int a = al->getAddAp() * (al->getRoundV() - 1);
                al->changeAP(al->getAp() + a);
                al->changeRound(1-al->getRoundV(), true);
                
            }
        }
    }
}
