//
//  SG_SkillsID2.cpp
//  GameSanGuo
//  
//  Created by wenlong w on 13-1-11.
//
//

#define INTERVERTIME 0.06

#include "SG_SkillsID2.h"

SG_SkillsID2::SG_SkillsID2()
:skillsId(-1)
,attackAp(0)
,attAp(0)
,enemyBattleMap(NULL)
,hero(NULL)
{
    setHeroLayer();
}

SG_SkillsID2::~SG_SkillsID2()
{
    
}

int SG_SkillsID2::attackGrid(SGBattleMap *map, SGGridBase *grid, int num)
{
    SGSoldier *soldier = NULL;
    
    int rand = 0;
    
    switch (grid->getStype()) {
        case knormal:
        {
            if (grid->getIndex().i<2)
            {
                
                    soldier = map->getShiBingWithGrid(grid);
                if(soldier){
                    map->getHero_Obj()->showBattleLabel(soldier->getDef(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());
                    map->removeAttackedSB((ShiBing*)soldier, 0, map->getHero_Obj());
                    rand = num - soldier->getDef();
                }
            }
        }
            break;
        case kdefend:
        {
            if (grid->getIndex().i<2)
            {
                soldier = map->getShiBingWithGrid(grid);
                if(soldier)
                {
                    rand = num - soldier->getDef();
                    
                    if (rand > 0) {
                        map->getHero_Obj()->showBattleLabel(soldier->getDef(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());
                        map->removeAttackedSB((ShiBing*)soldier, 0,  map->getHero_Obj());
                    }
                    else
                    {
                        map->getHero_Obj()->showBattleLabel(soldier->getDef() + rand, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());
                        soldier->setDef(-rand);
                        ((ShiBing*)soldier)->showDef(soldier->getDef());
                }
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
                if (soldierattack && soldierattack->getAttackIndex().i < 2) {
                    if (!soldierattack->getIsDeath()) {
                        
    //                    soldierattack->damageNum += num;
    //                    
    //                    if (soldierattack->getSkillsDelegate()) {
    //                        num = soldierattack->getSkillsDelegate()->assistPassivitySkills(num, soldierattack, soldierattack->getAfftributeNum());
    //                        soldierattack->getSkillsDelegate()->activatePassivitySkills1(NULL, soldierattack, attAp);
    ////                        soldierattack->attackSubAp(num);
    //                    }
                        
    //                    rand = num - soldierattack->getAp();
                        rand = soldierattack->attackSubAp(num);
                        map->getHero_Obj()->showBattleLabel(num, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());
                        
                        if (rand > 0) {
                            
                            soldierattack->changeAP(rand, false, true);
                            rand = -1;
                        }else{
                            rand *= -1;
                            map->removeAttackList(soldierattack,  map->getHero_Obj(), grid);
                        }
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

//刺杀 蓄力阶段，每回合都对阵列前方敌人进行一次攻击。伤害为当前回合攻击力的2%
void SG_SkillsID2::activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    attackAp = attackList->getMax_Ap()*num;
    CCLOG("num ===%f",num);
    CCLOG("attackList->getAp() ===%d",attackList->getAp());
    CCLOG("attackAp ===%d",attackAp);
    attAp = attackList->getAp();
    bool a = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    bool isme = false;
    hero = NULL;
    if (isHero && a) {
        enemyBattleMap = getenemyHero()->getBattleMap();
        isme = true;
        hero = getmyHero();
    }else{
        enemyBattleMap = getmyHero()->getBattleMap();
        isme = false;
        hero = getenemyHero();
    }
    
    index = attackList->getAttackIndex();
    
    int indexJ = index.j;
    CCLOG("选择的列 %d",indexJ);
    CCLOG("打列%d",abs(mapList-1-indexJ));
    int indexJ1 = index.j + 1;
    CCLOG("选择的列 %d",indexJ1);
    CCLOG("打列%d",abs(mapList-1-indexJ1));
    
    float time = (index.i + 2) * 0.2;
    
    float delay_time = 0.5;


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
    ResourceManager::sharedInstance()->bindTexture(str->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(str->getCString());
    
    CCString *str_ = CCString::createWithFormat("animationFile/jian_%d.scml", attackList->getAtkModeSkill());
    CCSpriterX *jianEffect = CCSpriterX::create(str_->getCString(), true, true);
    jianEffect->setanimaID(ACT_wait);
    jianEffect->setisloop(true);
    jianEffect->play();
    
    CCSpriterX *jianEffect1 = CCSpriterX::create(str_->getCString(), true, true);
    jianEffect1->setanimaID(ACT_wait);
    jianEffect1->setisloop(true);
    jianEffect1->play();
    
    if (false == isme) {
        //MMDEBUG: FLIPY
        jianEffect->setScaleY(-1);
        jianEffect1->setScaleY(-1);
//        jianEffect->setCCSpriterXFlipY(true);
//        jianEffect1->setCCSpriterXFlipY(true);
    }
    
    jianEffect->setPosition(GameConfig::getGridPoint(isme ? index.i : -index.i + OLD_EFFECT_FLIPY_FIX,   abs(isme ? -indexJ :  (mapList - 2 - indexJ)),true));
    jianEffect1->setPosition(GameConfig::getGridPoint(isme ? index.i : -index.i + OLD_EFFECT_FLIPY_FIX,   abs(isme ? -indexJ - 1 :  (mapList - 2 - (indexJ - 1))),true));
    
    
    CCPoint toPos = GameConfig::getGridPoint(isme ? -2 : 2 + OLD_EFFECT_FLIPY_FIX, abs(isme ? -indexJ : (mapList - 2 - indexJ)) ,true);
    CCPoint toPos1 = GameConfig::getGridPoint(isme ? -2 : 2 + OLD_EFFECT_FLIPY_FIX, abs(isme ? -indexJ - 1 : (mapList - 2 - (indexJ - 1))) ,true);
   
    
    CCHide *hide = CCHide::create();
    CCDelayTime *delay = CCDelayTime::create(delay_time);
    CCShow *show = CCShow::create();
    CCMoveTo *move = CCMoveTo::create(time, toPos);
    CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SG_SkillsID2::removeJian));
    jianEffect->runAction(CCSequence::create(hide, delay, show, move, call, NULL));
    hero->getBattleMap()->addSkillDelayCount();
    
    CCHide *hide1 = CCHide::create();
    CCDelayTime *delay1 = CCDelayTime::create(delay_time);
    CCShow *show1 = CCShow::create();
    CCMoveTo *move1 = CCMoveTo::create(time, toPos1);
    CCCallFuncN *call1 = CCCallFuncN::create(this, callfuncN_selector(SG_SkillsID2::removeJian));
    jianEffect1->runAction(CCSequence::create(hide1, delay1, show1, move1, call1, NULL));
    hero->getBattleMap()->addSkillDelayCount();
    
    hero->addChild(jianEffect, SKILL_JIAN_EFFECT_TAG, SKILL_JIAN_EFFECT_TAG);
    hero->addChild(jianEffect1, SKILL_JIAN_EFFECT_TAG, SKILL_JIAN_EFFECT_TAG * 11);
    
    
    CCDelayTime* delayActivate = CCDelayTime::create(INTERVERTIME);
    CCCallFunc* delayCall = CCCallFunc::create(this, callfunc_selector(SG_SkillsID2::startRemoveSb));
    
    hero->runAction(CCSequence::create(delayActivate, delayCall, NULL));//让瞬发技能先打

}

void SG_SkillsID2::startRemoveSb()
{
    float sbTime = 0.4;
    
    float delay_time = 0.5;
    float delay_time1 = 0.0;
    
    int indexJ = index.j;
    CCLOG("选择的列 %d",indexJ);
    CCLOG("打列%d",abs(mapList-1-indexJ));
    int indexJ1 = index.j + 1;
    CCLOG("选择的列 %d",indexJ1);
    CCLOG("打列%d",abs(mapList-1-indexJ1));
    
    if (attackAp > 0) {
        for (int i = 0; i <= 1; i++) {
            
            float b = sbTime * i;
            b = b + (delay_time + delay_time1);
            
            
            SGGridBase *grid = enemyBattleMap->myGrids[i][abs(mapList-1-indexJ)];
            SGGridBase *grid1 = enemyBattleMap->myGrids[i][abs(mapList-1-indexJ1)];
            if (grid || grid1) {
                SGGridBase *randGrid = NULL;
                if (grid) {
                    randGrid = grid;
                }else{
                    randGrid = grid1;
                }
                
                CCDelayTime* delay = CCDelayTime::create(b);
                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SG_SkillsID2::removesb), (void*)(i));
                hero->runAction(CCSequence::create(delay, call, NULL));
                
            }
        }
    }
}


void SG_SkillsID2::removesb(cocos2d::CCObject *obj, int rand)
{
    CCLOG("选择一次%d",rand);
    if (attackAp <= 0) {
        return;
    }
    
    int indexJ = index.j;
    CCLOG("选择的列 %d",indexJ);
    CCLOG("打列%d",abs(mapList-1-indexJ));
    int indexJ1 = index.j + 1;
    CCLOG("选择的列 %d",indexJ1);
    CCLOG("打列%d",abs(mapList-1-indexJ1));
    //ShiBing* sb = (ShiBing*)obj;
    //SGGridBase* grid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
    
    
    SGGridBase *attackGrid0 = enemyBattleMap->myGrids[rand][abs(mapList-1-indexJ)];
    SGGridBase *attackGrid1 = enemyBattleMap->myGrids[rand][abs(mapList-1-indexJ1)];
    
//    CCLOG("行==%d  列===%d", grid->getIndex().i, rand);//左
//    CCLOG("行==%d  列===%d", grid->getIndex().i, rand - 1);//右
    
    if (attackGrid0 &&
        attackGrid1) {
        
        
        
        SGSoldier *soldier = NULL;
        SGSoldier *rightSoldier = NULL;
        
        bool isSoldierSklls = false;
        bool isRightSoldierSkills = false;
        
        int defNum = 0;
        int defNum1 = 0;
        
        switch (attackGrid0->getStype()) {
            case knormal:
            case kdefend:
            {
                soldier = enemyBattleMap->getShiBingWithGrid(attackGrid0);
                if(soldier)
                {
                    defNum = soldier->getDef();
                }
                
            }
                break;
            case kattack:
            {
                soldier = enemyBattleMap->getAttackListByGrid(attackGrid0);
                if(soldier)
                {
                    SGAttackList * soldierattack = (SGAttackList *)soldier;
                    if (!soldierattack->getIsDeath()) {
                        
                        defNum = soldier->getAp();
                        if (soldierattack->getSkillsDelegate()) {
                            isSoldierSklls = true;
                        }
                    }
                }
                
                
            }
                break;
                
            default:
                break;
        }
        
        switch (attackGrid1->getStype()) {
            case knormal:
            {
                rightSoldier = enemyBattleMap->getShiBingWithGrid(attackGrid1);
                if(rightSoldier)
                {
                    if (soldier != rightSoldier) {
                        defNum1 = rightSoldier->getDef();
                    }
                }
            }
                break;
            case kdefend:
            {
                
                rightSoldier = enemyBattleMap->getShiBingWithGrid(attackGrid1);
                if(rightSoldier)
                {
                     defNum1 = rightSoldier->getDef();
                }
                
            }
                break;
            case kattack:
            {
                rightSoldier = enemyBattleMap->getAttackListByGrid(attackGrid1);
                if(rightSoldier)
                {
                    SGAttackList * soldierattack = (SGAttackList *)rightSoldier;
                    if (!soldierattack->getIsDeath()) {
                        if (soldier != rightSoldier)
                        {
                            defNum1 = rightSoldier->getAp();
                            if (soldierattack->getSkillsDelegate()) {
                                
                                isRightSoldierSkills = true;
                                
                            }
                            
                        }
                    }
                }
            }
                break;
                
            default:
                break;
        }
        
        
        if (soldier == rightSoldier)
        {
            //四格武将
            
            attackAp = attackGrid(enemyBattleMap, attackGrid0,attackAp);
            
        }
        else
        {
            int attackAp0 = 0;
            int attackAp1 = 0; 
            if(defNum >= defNum1 )
            {
                attackAp0 = floor(attackAp*(float)defNum/(float)(defNum+defNum1));
                attackAp1= attackAp- attackAp0;
            }
            else
            {
                attackAp1 =  floor(attackAp*(float)defNum1/(float)(defNum+defNum1));
                attackAp0 = attackAp - attackAp1;
            }
            if(attackAp0+attackAp1!=attackAp)
            {
               
            }
            CCLog("att0^^^^^^^^^^%d",attackAp0);
            CCLog("att1^^^^^^^^^^%d",attackAp1);
            CCLog("att^^^^^^^^^^^%d",attackAp);
            attackAp0 = attackGrid(enemyBattleMap, attackGrid0,attackAp0);
            attackAp1 = attackGrid(enemyBattleMap, attackGrid1,attackAp1);
            CCLog("att0@^^^^^^^^^^%d",attackAp0);
            CCLog("att1@^^^^^^^^^^%d",attackAp1);
            if(attackAp0>0&&attackAp1>0)
            {
                attackAp = attackAp0+attackAp1;
            }
            else if(attackAp0>0&&attackAp1<=0)
            {
                attackAp = attackAp0;
            }
            else if(attackAp0<=0&&attackAp1>0)
            {
                attackAp = attackAp1;
            }
            else
            {
                attackAp = 0;
            }
        }

    
    }
    else if(attackGrid0 && !attackGrid1)
    {
        if (attackGrid0)
        {
           
            attackAp = attackGrid(enemyBattleMap, attackGrid0,attackAp);
            CCLog("att0^^^^^^^^^^%d",attackAp);
            if(attackAp<=0)
                attackAp=0;
        }
        
    }
    else if(!attackGrid0 && attackGrid1)
    {
       
        if(attackGrid1)
        {
            
            attackAp = attackGrid(enemyBattleMap, attackGrid1,attackAp);
            CCLog("att1^^^^^^^^^^%d",attackAp);
            if(attackAp<=0)
                attackAp=0;

        }
    }
}

void SG_SkillsID2::removeJian(cocos2d::CCObject *obj)
{
    if (hero) {
        CCSpriterX *sp = (CCSpriterX*)hero->getChildByTag(SKILL_JIAN_EFFECT_TAG);
        if (sp) {
//            sp->removeAllChildrenWithCleanup(true);
            sp->removeFromParentAndCleanup(true);
            sp = NULL;
        }

        CCSpriterX *sp1 = (CCSpriterX*)hero->getChildByTag(SKILL_JIAN_EFFECT_TAG * 11);
        if (sp1) {
//            sp1->removeAllChildrenWithCleanup(true);
            sp1->removeFromParentAndCleanup(true);
            sp1 = NULL;
        }
        hero->removeChildByTag(SKILL_JIAN_EFFECT_TAG, true);
        hero->removeChildByTag(SKILL_JIAN_EFFECT_TAG * 11, true);
        hero->getBattleMap()->reduceSkillDelayCount();
    }

}
