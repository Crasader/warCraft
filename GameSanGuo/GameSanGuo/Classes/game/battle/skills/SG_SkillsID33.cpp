//
//  SG_SkillsID33.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#include "SG_SkillsID33.h"
#include "SGStringConfig.h"
SG_SkillsID33::SG_SkillsID33()
:attackAp(0)
,attAp(0)
,hero(NULL)
,enemyBattleMap(NULL)
{
    setHeroLayer();
}


SG_SkillsID33::~SG_SkillsID33()
{
    
}

void SG_SkillsID33::activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
    attackAp = attackList->getMax_Ap() * num;
    attAp = attackList->getAp();
    bool a = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    bool isme = false;
    if (isHero && a) {
        enemyBattleMap = getenemyHero()->getBattleMap();
        isme = true;
        hero = getmyHero();

    }else{
        enemyBattleMap = getmyHero()->getBattleMap();
        isme = false;
        hero = getenemyHero();
    }
    
    
    GameIndex index = attackList->getAttackIndex();
    
    int indexJ = index.j;
    
    float sbTime = 0.2;
    float time = (index.i + 2) * sbTime;
    
    float delay_time = 0.5;
    float delay_time1 = 0.0;
    
    
    
    float att_time = 0.;
    float att_time1 = 0.;
    
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackList->attackSbs, obj)
    {
        ShiBing *sb = (ShiBing*)obj;
        int act = ACT_atkUp;
//        int act1 = aAction_9;
        int act1 = ACT_atkUp;

        
        if (!isme) {
            act = ACT_atkDown;
//            act1 = aAction_5;
            act1 = ACT_atkDown;
        }
        //att_time = sb->getAnimaTimeLength(0, act) * ANIMA_TIME_NUM;
        att_time = spineAtkTimeTest * ANIMA_TIME_NUM;
        //att_time1 = sb->getAnimaTimeLength(0, act1) * ANIMA_TIME_NUM;
        att_time1 = 0;
        
        CCLOG("sb->getSbindex().i==%d",sb->getSbindex().i);
        CCLOG("sb->getSbindex().j==%d",sb->getSbindex().j);
        
        
        sb->setanimaID(act, false);

        
    }
    
    
    CCLOG("加载了弓箭！！");
    
    delay_time = delay_time + att_time;
    
    CCString *str = CCString::createWithFormat("animationFile/jian_%d.plist", attackList->getAtkModeSkill());
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(str->getCString());
    ResourceManager::sharedInstance()->bindTexture(str->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    
    CCString *str_ = CCString::createWithFormat("animationFile/jian_%d.scml", attackList->getAtkModeSkill());
    CCSpriterX *jianEffect = CCSpriterX::create(str_->getCString(), true, true);
    jianEffect->setanimaID(ACT_wait);
    jianEffect->setisloop(true);
    jianEffect->play();
    
    if (false == isme) {
        //MMDEBUG: FLIPY
        jianEffect->setScaleY(-1);
//        jianEffect->setCCSpriterXFlipY(true);
    }
    
    jianEffect->setPosition(GameConfig::getGridPoint(isme ? index.i : -index.i + OLD_EFFECT_FLIPY_FIX,   abs(isme ? -indexJ :  (mapList - 1 - indexJ)),true));
    
    CCHide *hide = CCHide::create();
    CCDelayTime *delay = CCDelayTime::create(delay_time);
    CCShow *show = CCShow::create();
    CCMoveTo *move = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -2 : 2 + OLD_EFFECT_FLIPY_FIX, abs(isme ? -indexJ : (mapList - 1 - indexJ)) ,true));
    CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SG_SkillsID33::removeJian));
    jianEffect->runAction(CCSequence::create(hide, delay, show, move, call, NULL));
    hero->addChild(jianEffect, SKILL_JIAN_EFFECT_TAG, SKILL_JIAN_EFFECT_TAG);
    hero->getBattleMap()->addSkillDelayCount();
    
    for (int i = 0; i <= 1; i++) {
        
            if (attackAp > 0) {
                float j = 0.0;
                if (i+index.i == 0) {
                    j = 1.0;
                }else{
                    j = (i + index.i + 1);
                }
                float b = sbTime * j;
                b = b + (delay_time + delay_time1);
                    
                SGGridBase *grid = enemyBattleMap->myGrids[i][abs(mapList-1-indexJ)];
                if (grid) {

                    ShiBing* sb = enemyBattleMap->getShiBingWithGrid(grid);
                    if(sb)
                    {
                        CCDelayTime* delay = CCDelayTime::create(b);
                        CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SG_SkillsID33::removesb), (void*)(abs(mapList-1-indexJ)));
                        sb->runAction(CCSequence::create(delay, call, NULL));
                    }
                    
                    
                }
            }
        
    }
}

void SG_SkillsID33::removesb(cocos2d::CCObject *obj, int rand)
{
    if (attackAp <= 0) {
        return;
    }
    ShiBing* sb = (ShiBing*)obj;
    SGGridBase* grid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
    
    
    SGGridBase *attackGrid0 = enemyBattleMap->myGrids[grid->getIndex().i][rand];
    
    if (attackGrid0 ) {
        attackAp = attackGrid(enemyBattleMap, attackGrid0,attackAp);
    }

}

void SG_SkillsID33::removeJian(cocos2d::CCObject *obj)
{
    if (hero) {
        CCSpriterX *sp = (CCSpriterX*)hero->getChildByTag(SKILL_JIAN_EFFECT_TAG);
        if (sp)
        {
//            sp->removeAllChildrenWithCleanup(true);
            sp->removeFromParentAndCleanup(true);
            sp = NULL;
        }
//        hero->removeChildByTag(SKILL_JIAN_EFFECT_TAG, true);
        hero->getBattleMap()->reduceSkillDelayCount();
    }
}

int SG_SkillsID33::attackGrid(SGBattleMap *map, SGGridBase *grid, int num)
{
    SGSoldier *soldier = NULL;
    
    int rand = 0;
    
    switch (grid->getStype()) {
        case knormal:
        {
            soldier = map->getShiBingWithGrid(grid);
            if(soldier)
            {
                map->getHero_Obj()->showBattleLabel(((ShiBing*)soldier)->getDef(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, ((ShiBing*)soldier)->getSbindex());
                map->removeAttackedSB((ShiBing*)soldier, 0);
                rand = num - soldier->getDef();
                CCString *str = CCString::createWithFormat(str_Format_duanhunzhan_shanbing_damage,num);
                SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
            }
            
        }
            break;
        case kdefend:
        {
            soldier = map->getShiBingWithGrid(grid);
            if(soldier)
            {
                rand = num - soldier->getDef();
                
                CCString *str = CCString::createWithFormat(str_Format_duanhunzhan_defense_damage,num);
                SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
                
                if (rand > 0) {
                    map->getHero_Obj()->showBattleLabel(((ShiBing*)soldier)->getDef(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, ((ShiBing*)soldier)->getSbindex());
                    map->removeAttackedSB((ShiBing*)soldier, 0);
                }else{
                    map->getHero_Obj()->showBattleLabel(num, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, ((ShiBing*)soldier)->getSbindex());
                    soldier->setDef(-rand);
                    ((ShiBing*)soldier)->setDefImage();
                    ((ShiBing*)soldier)->showDef(soldier->getDef());
                }
            }
        }
            break;
        case kattack:
        {
            soldier = map->getAttackListByGrid(grid);
            if(soldier)
            {
                SGAttackList * soldierattack = (SGAttackList *)soldier;
                
                CCString *str = CCString::createWithFormat("断魂斩_攻击阵列伤害： %d",num);
                SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
                
                if (!soldierattack->getIsDeath()) {
                    
    //                soldierattack->damageNum += num;
                    
    //                if (soldierattack->getSkillsDelegate()) {
    //                    num = soldierattack->getSkillsDelegate()->assistPassivitySkills(num, soldierattack, soldierattack->getAfftributeNum());
    //                    soldierattack->getSkillsDelegate()->activatePassivitySkills1(NULL, soldierattack, attAp);
    //                    soldierattack->attackSubAp(num);
    //                }
                    
    //                rand = num - soldierattack->getAp();
                    
                    rand = soldierattack->attackSubAp(num);
                    map->getHero_Obj()->showBattleLabel(num, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, soldierattack->getAttackIndex());
                    if (rand > 0) {
                        soldierattack->changeAP(rand, false, true);
                        rand = -1;
                    }else{
                        rand *= -1;
                        map->removeAttackList(soldierattack, getenemyHero(), grid);
                    }
                    
                }
            }
            
            
        }
            break;
            
        default:
            break;
    }
    
    return rand;
    
}
