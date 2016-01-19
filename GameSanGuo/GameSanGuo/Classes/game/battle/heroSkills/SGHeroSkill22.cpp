//
//  SGHeroSkill22.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill22.h"

SGHeroSkill22::SGHeroSkill22()
{
    
}

SGHeroSkill22::~SGHeroSkill22()
{
    
}

//增加散兵状态防御力10%
void SGHeroSkill22::activateSkill_20046(SGHeroLayer *heroLayer, float value)
{
    SGBattleMap *battleMap = heroLayer->getBattleMap();
    CCArray *soldiers = battleMap->getSoldiers();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(soldiers, obj)
    {
        ShiBing *sb = (ShiBing *)obj;
        CCArray *grids = sb->getPrinceGridArray();
        SGGridBase *grid = (SGGridBase *)grids->objectAtIndex(0);
        if (grid->getStype() == knormal)
        {
            sb->showEffect(1);
            int addValue = sb->getDef() * value;
            sb->setDef(sb->getDef() + addValue);
            sb->showDef(sb->getDef());
        }
    }
}
