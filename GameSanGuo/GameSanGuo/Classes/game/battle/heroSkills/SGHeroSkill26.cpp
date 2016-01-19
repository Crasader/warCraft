//
//  SGHeroSkill26.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill26.h"
#include "SGSkillManager.h"

SGHeroSkill26::SGHeroSkill26()
{
    
}

SGHeroSkill26::~SGHeroSkill26()
{
    
}

//发动后，所有我方散兵状态2格武将直接形成攻击阵列。
void SGHeroSkill26::activateSkill_20060(SGHeroLayer *heroLayer)
{
    SGBattleMap *battleMap = heroLayer->getBattleMap();
    CCArray *soldiers = battleMap->getSoldiers();
    CCArray *sbArray = CCArray::create();
    CCObject *obj = NULL;
    bool isR = false;
    CCARRAY_FOREACH(soldiers, obj)
    {
        ShiBing *sb = (ShiBing *)obj;
        SGGridBase *grid = (SGGridBase*) sb->getPrinceGridArray()->objectAtIndex(0);
        if (sb->getSbType() == ksbtwo &&
            grid->getStype() == knormal)
        {
            sbArray->removeAllObjects();
            SGTwoAttackList *al = SGTwoAttackList::createWithSoldier(sb, true);
            battleMap->getAttackList()->addObject(al);
            sbArray->addObject(sb);
            al->drawSkillsEffect(4);
            sb->showEffect(2, false);
            isR = true;
        }
        battleMap->spgongJiSwapGridObj(sbArray);
    }
    
    if (!isR) {
        SGSkillManager::shareSkill()->beginAi();
        SGSkillManager::shareSkill()->beginUpTime();
    }else{
        CCDelayTime *time = CCDelayTime::create(1.5);
        CCCallFuncN *call = CCCallFuncN::create(SGSkillManager::shareSkill(), callfuncN_selector(SGSkillManager::ccc));
        heroLayer->runAction(CCSequence::create(time, call, NULL));
    }
}