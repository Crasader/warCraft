//
//  SGHeroSkill29.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill29.h"

SGHeroSkill29::SGHeroSkill29()
{
    
}

SGHeroSkill29::~SGHeroSkill29()
{
    
}

void SGHeroSkill29::activateSkill_20066(SGHeroLayer *heroLayer, float value)
{
    SGBattleMap *battleMap = heroLayer->getBattleMap();
    CCArray *soldiers = battleMap->getSoldiers();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(soldiers, obj)
    {
        ShiBing *sb = (ShiBing *)obj;
        SGGridBase*grid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
        if ( grid->getStype() == knormal )
        {
            sb->setDef(sb->getDef() + sb->getDef() * value);
            sb->showDef(sb->getDef());
            sb->showEffect(1);
            heroLayer->showBattleLabel(sb->getDef() * value, "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, gameIndex(grid->getIndex().i, grid->getIndex().j), true);
        }
    }
}