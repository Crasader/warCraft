//
//  SGHeroSkill3.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill3.h"


SGHeroSkill3::SGHeroSkill3()
{
    
}

SGHeroSkill3::~SGHeroSkill3()
{
    
}

void SGHeroSkill3::activateSkill_20002(SGHeroLayer *heroLayer)
{
    
    CCArray *attackLists = heroLayer->getBattleMap()->getAttackList();
    if (attackLists == NULL) {
        return;
    }
    CCObject *obj = NULL;
    
    CCArray *remove = CCArray::create();
    CCARRAY_FOREACH(attackLists, obj)
    {
        SGAttackList *attackList = (SGAttackList *)obj;
        
        if (attackList->getAttackType() == kattacknormal)
        {
            attackList->m_effectsLayer->removeAllChildren();
            
            if (attackList->roundNum->getChildByTag(ATTACK_ROUND_EFF_TAG)) {
                CCSpriterX *s = (CCSpriterX*)attackList->roundNum->getChildByTag(ATTACK_ROUND_EFF_TAG);
//                s->removeAllChildrenWithCleanup(true);
                s->removeFromParentAndCleanup(true);
                s = NULL;
            }
            
            CCObject *sbObj = NULL;
            CCARRAY_FOREACH(attackList->getAttackSbs(), sbObj)
            {
                ShiBing *sb = (ShiBing *)sbObj;
                sb->removeChildByTag(EFFECT_LAYER_TAG, true);
                sb->removeChildByTag(EFFECT_LAYER_TAG * 2, true);
                sb->removeChildByTag(EFFECT_LAYER_TAG * 3, true);
                sb->removeChildByTag(EFFECT_LAYER_TAG * 4, true);
                sb->removeChildByTag(ATTACK_ROUND_EFF_TAG, true);
                sb->resetSbData();
                heroLayer->showEffectLayer(12, false, sb->getSbindex());
                
                //暂时还是加在这里吧
                if (sb) {
                    heroLayer->battleMap->oneList->addObject(sb);
                }
                
            }
            remove->addObject(attackList);
            SGNormalAttackList *nAttackList = (SGNormalAttackList *)attackList;
            heroLayer->getBattleMap()->removeAttackedSB(nAttackList->getFirstSB(), 0, heroLayer);
            
        }
    }
    
    CCARRAY_FOREACH(remove, obj)
    {
        SGAttackList *attackList = (SGAttackList *)obj;
        attackLists->removeObject(attackList);
    }
    if (remove) {
        remove->removeAllObjects();
    }
    
}
