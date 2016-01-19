//
//  SGHeroSkill14.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill14.h"

SGHeroSkill14::SGHeroSkill14()
{
    
}

SGHeroSkill14::~SGHeroSkill14()
{
    
}

//增加所有敌方攻击阵列蓄力回合2，攻击力上限不变。
void SGHeroSkill14::activateSkill_20020(SGHeroLayer *heroLayer, int addValue)
{
    CCObject *obj = NULL;

    //
    CCArray *attList = heroLayer->getBattleMap()->getAttackList();
    if (attList && attList->data && attList->data->arr && attList->count() > 0)
    {
		CCARRAY_FOREACH(heroLayer->getBattleMap()->getAttackList(), obj)
		{
			SGAttackList *attackList = (SGAttackList *)obj;
			attackList->changeRound(addValue, true);
		}
    }
}
