//
//  SGHeroSkill18.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill18.h"
#include "SGSkillManager.h"

SGHeroSkill18::SGHeroSkill18()
{
    
}

SGHeroSkill18::~SGHeroSkill18()
{
    
}

//解除敌方当前融合效果
void SGHeroSkill18::activateSkill_20030(SGHeroLayer *heroLayer, int skillsId)
{    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(heroLayer->battleMap->getAttackList(), obj)
    {
        SGAttackList *al = (SGAttackList *)obj;
        if (al->getLRType() == lr_r)
        {
            heroLayer->showEffectLayer(15, false, gameIndex(al->getAttackIndex().i + 1, al->getAttackIndex().j));
            al->setFuseNum(0);
            if (al->m_effectsLayer->getChildByTag(RONGHE_EFFECT_TAG)) {
                CCSpriterX *s = (CCSpriterX*)al->m_effectsLayer->getChildByTag(RONGHE_EFFECT_TAG);
//                s->removeAllChildrenWithCleanup(true);
                s->removeFromParentAndCleanup(true);
                s = NULL;
            }
            al->m_effectsLayer->removeChildByTag(RONGHE_EFFECT_TAG, true);
        }
        if (skillsId == skill_id_20030) {
            if (al->getLRType() == lr_l) {
                heroLayer->showEffectLayer(14, false, gameIndex(al->getAttackIndex().i + 1, al->getAttackIndex().j));
                al->setAddChain(0);
                if (al->m_effectsLayer->getChildByTag(LIANSUO_EFFECT_TAG)) {
                    CCSpriterX *s = (CCSpriterX*)al->m_effectsLayer->getChildByTag(LIANSUO_EFFECT_TAG);
//                    s->removeAllChildrenWithCleanup(true);
                    s->removeFromParentAndCleanup(true);
                    s = NULL;
                }
                al->m_effectsLayer->removeChildByTag(LIANSUO_EFFECT_TAG, true);
            }
        }
    }
}
