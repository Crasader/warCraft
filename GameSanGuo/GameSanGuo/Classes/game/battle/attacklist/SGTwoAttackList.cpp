//
//  SGGamePrinceAttack.cpp
//  GameSanGuo
//  两格攻击阵列
//  Created by wenlong w on 12-11-28.
//
//

#include "SGTwoAttackList.h"


SGTwoAttackList::SGTwoAttackList()
{
    
}

SGTwoAttackList::~SGTwoAttackList()
{
    
}

//二格武将的攻击阵列
SGTwoAttackList *SGTwoAttackList::createWithSoldier(ShiBing *sb, bool ismy)
{
    SGTwoAttackList *al = new SGTwoAttackList();
    if (al && al->initWithSoldier(sb, ismy, kattacktwo))   //调用SGAttackList.cpp中的initWithSoldier
    {
        al->showFormEffect();
        al->setAttackType(kattacktwo);
        al->autorelease();
        return al;
    }
    CC_SAFE_DELETE(al);
    return NULL;
}
//显示二格武将的组成阵列动画
void SGTwoAttackList::showFormEffect()
{

    if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsRepairMap())
    {
        return;
    }

    wjhbEffect = CCSkeletonAnimation::createWithFile("effSpine/wjhb2.json", "effSpine/wjhb2.atlas", 1);
    wjhbEffect->setAnimation("animation", false);
    m_effectsLayer->addChild(wjhbEffect, FERM_EFFECT_TAG, FERM_EFFECT_TAG);
    
    bool me = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    if (!me)
    {
        wjhbEffect->setScaleY(-1);
        wjhbEffect->setPosition(ccp(0, GameConfig::getGridSize().height * 1.5));
    }

}



