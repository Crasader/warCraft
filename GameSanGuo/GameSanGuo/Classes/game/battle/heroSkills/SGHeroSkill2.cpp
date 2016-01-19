//
//  SGHeroSkill2.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill2.h"
#include "SGSkillManager.h"
#include "SGStringConfig.h"

SGHeroSkill2::SGHeroSkill2()
{
    
}

SGHeroSkill2::~SGHeroSkill2()
{
    CCLOG("~SGHeroSkill2");
}

float SGHeroSkill2::activateSkill_20001(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid)
{
    int indexJ = value2;
    
    CCLOG("选择的列 %d",indexJ);
    CCLOG("打列%d",abs(mapList-1-indexJ));
    
    float time = 1.0;
    float sbTime = time/12;
    
//    业务逻辑无关
    bool isme = true;
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() != roleid)
    {
        isme = false;
        if (!SGAIManager::shareManager()->isPVE) {
            heroLayer->setBulletSp(3);
        }
        if (heroLayer->m_skills_Bullet)
        {
            //MMDEBUG: FLIPY
            heroLayer->m_skills_Bullet->setScaleY(-1);
//            heroLayer->m_skills_Bullet->setCCSpriterXFlipY(true);
        }
        if (heroLayer->m_skills_Bullet1)
        {
            //MMDEBUG: FLIPY
            heroLayer->m_skills_Bullet1->setScaleY(-1);
//            heroLayer->m_skills_Bullet1->setCCSpriterXFlipY(true);
        }
        if (heroLayer->m_skills_Bullet2)
        {
            //MMDEBUG: FLIPY
            heroLayer->m_skills_Bullet2->setScaleY(-1);
//            heroLayer->m_skills_Bullet2->setCCSpriterXFlipY(true);
        }
    }
    
    if (heroLayer->m_skills_Bullet)
    heroLayer->m_skills_Bullet->setPosition(GameConfig::getGridPoint(isme ? 5 : -5 + OLD_EFFECT_FLIPY_FIX,   abs(isme ? - indexJ :  (mapList - 1 - indexJ)),true));
    if (heroLayer->m_skills_Bullet1)
    heroLayer->m_skills_Bullet1->setPosition(GameConfig::getGridPoint(isme ? 5 : -5 + OLD_EFFECT_FLIPY_FIX,   abs(isme ? - (indexJ - 1) :  (mapList - 1 - (indexJ - 1))),true));
    if (heroLayer->m_skills_Bullet2)
    heroLayer->m_skills_Bullet2->setPosition(GameConfig::getGridPoint(isme ? 5 : -5 + OLD_EFFECT_FLIPY_FIX,   abs(isme ? - (indexJ + 1) :  (mapList - 1 - (indexJ + 1))),true));
    
    CCMoveTo *move = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow : mapRow + OLD_EFFECT_FLIPY_FIX, abs(isme ? -indexJ : (mapList - 1 - indexJ)) ,true));
    if (heroLayer->m_skills_Bullet)
    heroLayer->m_skills_Bullet->runAction(CCSequence::create(move, NULL));
    CCMoveTo *move1 = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow : mapRow + OLD_EFFECT_FLIPY_FIX, abs(isme ? -(indexJ - 1) : (mapList - 1 - (indexJ - 1))) ,true));
    if (heroLayer->m_skills_Bullet1)
    heroLayer->m_skills_Bullet1->runAction(CCSequence::create(move1, NULL));
    CCMoveTo *move2 = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow : mapRow + OLD_EFFECT_FLIPY_FIX, abs(isme ? -(indexJ + 1) : (mapList - 1 - (indexJ + 1))) ,true));
    if (heroLayer->m_skills_Bullet2)
    heroLayer->m_skills_Bullet2->runAction(CCSequence::create(move2, NULL));
    
    
    int delaychangeTime = 0;
    float  skillDeleayTime = 0;
//    逻辑部分
    for (int j = indexJ - 1; j <= indexJ + 1; j++) {
        int ap = value1;
        
        if (isme) {
            CCString *str = CCString::createWithFormat(str_Format_attack_column, ap, j);
            SGBattleManager::sharedBattle()->fightLogRequest(str->getCString());
        }
        
        for (int i = 0; i <= mapRow; i++) {
            if (ap > 0) {
                float b = sbTime * (i+5.5);
                if (skillDeleayTime < b)
                {
                    skillDeleayTime = b;//ai使用时的延时
                }
                
                if (i == mapRow) {
                    //播放打到底线动作
                    GameSbIndex *sbin = new GameSbIndex();
                    sbin->i = mapRow;
                    sbin->j = indexJ;
                    CCDelayTime* delay1 = CCDelayTime::create(b + sbTime);
                    CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGHeroSkill2::removeBullet),
                                                               (void*)(SKILLS_BULLET_TAG + j - indexJ));
                    CCCallFuncND *call3 = CCCallFuncND::create(SGSkillManager::shareSkill(),
                                                               callfuncND_selector(SGSkillManager::playBulletBlast),
                                                               (void*)sbin);
                    heroLayer->runAction(CCSequence::create(delay1, call1, call3, NULL));
                    heroLayer->getBattleMap()->addSkillDelayCount();
                    
                    CCDelayTime* delay2 = CCDelayTime::create(b + sbTime + delaychangeTime * 0.1);
                    CCCallFuncND* call2 = CCCallFuncND::create(SGSkillManager::shareSkill(),
                                                               callfuncND_selector(SGSkillManager::changBloodNum),
                                                               (void*)ap);
                    heroLayer1->runAction(CCSequence::create(delay2, call2, NULL));
                    delaychangeTime++;
                }else{
                    SGGridBase *grid = heroLayer1->battleMap->myGrids[i][abs(mapList-1-j)];
                    
                    if (grid) {
                        if (grid->getStype() == knormal) {
                            
                            ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                            if(sb)
                            {
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create(delay, call, NULL));
                                ap = ap - sb->getDef();
                                sb->setDef(0);
                                
                                {
                                    heroLayer->showBattleLabel( sb->getDef(), "battle/sub_hp_label.png",
                                                               SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,b);
                                }
                                sb->setDef(0);
                                if (sb->getSbType() == ksbtwo || sb->getSbType() == ksbfour) {
                                    i++;
                                }
                                if (isme) {
                                    CCString *str = CCString::createWithFormat(str_Format_behit_defense_residual, sb->getFileId(), sb->getDef(), ap);
                                    SGBattleManager::sharedBattle()->fightLogRequest(str->getCString());
                                }
                            }
                            
                        }
                        else if(grid->getStype() == kdefend)
                        {
                            ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                            if(sb)
                            {
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create(delay, call, NULL));
                                ap = ap - sb->getDef();
                                
                                {
                                    heroLayer->showBattleLabel( sb->getDef() + ap , "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,b);
                                }
                                
                                sb->setDef(-ap);
                                
                                if (isme) {
                                    CCString *str = CCString::createWithFormat(str_Format_behit_defense_residual, sb->getFileId(), sb->getDef(), ap);
                                    SGBattleManager::sharedBattle()->fightLogRequest(str->getCString());
                                }
                            }
                        }
                        else
                        {
                            SGAttackList* attack = heroLayer1->battleMap->getAttackListByGrid(grid);
                            if(attack)
                            {
                                ap = ap - attack->getAp();
                                int a = 0;
                                if (ap >= 0) {
                                    a = attack->getAp();
                                }else{
                                    a = attack->getAp() + ap;
                                }
                                
                                ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                                //if (ap < 0)
                                {
                                    heroLayer->showBattleLabel(a , "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,b);
                                }
                                
                                
                                
                                attack->heroSkillSubAp(a);
                                attack->setAp(-ap);
                                
                                if ((grid->getStype() == kattackfour || grid->getStype() == kattacktwo) && ap > 0)
                                {
                                    attack->playEffectWithType(kAttackEffect);
                                }
                                
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                
                                sb->runAction(CCSequence::create( delay, call, NULL));
                                
                                if (attack->getAttackType() == kattacknormal) {
                                    i += 2;
                                }else{
                                    i += 1;
                                }
                                if (isme) {
                                    CCString *str = CCString::createWithFormat(str_Format_behit_defense_residual, sb->getFileId(), attack->getAp(), ap);
                                    SGBattleManager::sharedBattle()->fightLogRequest(str->getCString());
                                }
                            }
                        }
                        
                        if (ap <= 0) {
                            GameSbIndex *sbin = new GameSbIndex();
                            sbin->i = i;
                            sbin->j = indexJ;
                            CCDelayTime* delay1 = CCDelayTime::create(b);
                            CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                                       callfuncND_selector(SGHeroSkill2::removeBullet),
                                                                       (void*)(SKILLS_BULLET_TAG + j - indexJ));
                            
                            CCCallFuncND *call3 = CCCallFuncND::create(SGSkillManager::shareSkill(),
                                                                       callfuncND_selector(SGSkillManager::playBulletBlast),
                                                                       (void*)sbin);
                            heroLayer->runAction(CCSequence::create(delay1, call1, call3, NULL));
                            heroLayer->getBattleMap()->addSkillDelayCount();
                        }
                    }
                }
            }
            else{
                
                break;
            }
        }
    }
    return (skillDeleayTime + sbTime)*3;
}


void SGHeroSkill2::removeBullet(cocos2d::CCObject *obj, void* tag)
{
    ((SGHeroLayer*)obj)->removeChildByTag( (long)tag, true);
    if (SGAIManager::shareManager()->isPVE) {
        SGAIManager::shareManager()->isAi = true;
        if (((SGHeroLayer*)obj)->battleMap->_isRefreshMap ||
            ((SGHeroLayer*)obj)->battleMap->m_touchState != eTouchNull)
        {
        }else{
            if (SGSkillManager::shareSkill()->isBeginAi){
                 SGSkillManager::shareSkill()->beginAi();
            }
            
        }
        if (SGSkillManager::shareSkill()->isBeginAi)
            SGSkillManager::shareSkill()->beginUpTime();
    }
    
    ((SGHeroLayer*)obj)->getBattleMap()->reduceSkillDelayCount();
}





