//
//  SGHeroSkill1.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill1.h"



SGHeroSkill1::SGHeroSkill1()
{
    
}

SGHeroSkill1::~SGHeroSkill1()
{
    
}

void SGHeroSkill1::activateSkill_20000(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid)
{
    CCObject *attack = NULL;
    CCARRAY_FOREACH(heroLayer->battleMap->getAttackList(), attack)
    {
        SGAttackList *attackObj = (SGAttackList*)attack;
        attackObj->attackAddAp(value1);
        
    }
    
    CCObject* objsb = NULL;
    CCARRAY_FOREACH(heroLayer->battleMap->getSoldiers(), objsb)
    {
        ShiBing *fSb = (ShiBing*) objsb;
        if (fSb)
        {
            SGGridBase *gg = (SGGridBase*) fSb->getPrinceGridArray()->objectAtIndex(0);
            if (gg)
            {
                if (gg->getStype() == knormal) {
                    fSb->setDef(fSb->getDef() + fSb->getDef() * value2);
                    fSb->showDef(fSb->getDef());
                    fSb->showEffect(1);
                }
            }
        }
    }
    
}