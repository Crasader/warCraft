//
//  SGHeroSkill28.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill28.h"
#include "SGSkillManager.h"

SGHeroSkill28::SGHeroSkill28()
{
    
}

SGHeroSkill28::~SGHeroSkill28()
{
    
}

//发动后，以我方所有散兵（不包括武将）的防御力总和形成一个攻击球，玩家选择列发射出去。
void SGHeroSkill28::activateSkill_20065(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid)
{
    int indexJ = value2;
    CCLOG("选择的列 %d",indexJ);
    CCLOG("打列%d",abs(mapList-1-indexJ));
    
    
    float time = 1.5;
    float sbTime = time/12;
    
    bool isme = true;
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() != roleid )
    {
        isme = false;
        if(!SGAIManager::shareManager()->isPVE)
            heroLayer->setBulletSp(1);
    }
    
    heroLayer->m_skills_Bullet->setPosition(GameConfig::getGridPoint(isme ? 5 : -5,   abs(isme ? - indexJ :  (mapList - 1 - indexJ)),true));

    if (value1 <= 0) {
        //处理攻击力为0
        CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                   callfuncND_selector(SGHeroSkill28::removeBullet),
                                                   (void*)(SKILLS_BULLET_TAG));
        GameSbIndex *sbin = new GameSbIndex();
        sbin->i = isme ? mapRow : -mapRow;
        sbin->j = indexJ;
        CCCallFuncND *call3 = CCCallFuncND::create(SGSkillManager::shareSkill(),
                                                   callfuncND_selector(SGSkillManager::playBulletBlast),
                                                   (void*)sbin);

        heroLayer->runAction(CCSequence::create( call1, call3, NULL));
    }else{
        CCMoveTo *move = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow : mapRow, abs(isme ? -indexJ : (mapList - 1 - indexJ)) ,true));
        heroLayer->m_skills_Bullet->runAction(CCSequence::create(move, NULL));
    }
    
    int ap = value1;
    for (int i = 0; i <= mapRow; i++) {
        if (ap > 0) {
            float b = sbTime * (i+5.5);
            if (i == mapRow) {
                //播放打到底线动作
                CCDelayTime* delay1 = CCDelayTime::create(b + sbTime);
                CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                           callfuncND_selector(SGHeroSkill28::removeBullet),
                                                           (void*)(SKILLS_BULLET_TAG));
                GameSbIndex *sbin = new GameSbIndex();
                sbin->i = mapRow;
                sbin->j = indexJ;
                CCCallFuncND *call3 = CCCallFuncND::create(SGSkillManager::shareSkill(),
                                                           callfuncND_selector(SGSkillManager::playBulletBlast),
                                                           (void*)sbin);
                heroLayer->runAction(CCSequence::create(delay1, call1, call3, NULL));
                
                CCDelayTime* delay2 = CCDelayTime::create(b + sbTime);
                CCCallFuncND* call2 = CCCallFuncND::create(SGSkillManager::shareSkill(),
                                                           callfuncND_selector(SGSkillManager::changBloodNum),
                                                           (void*)ap);
                heroLayer1->runAction(CCSequence::create(delay2, call2, NULL));
                
                
            }else{
                SGGridBase *grid = heroLayer1->battleMap->myGrids[i][abs(mapList-1-indexJ)];
                
                if (grid) {
                    if (grid->getStype() == knormal) {
                        
                        ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                        if(sb)
                        {
                            CCDelayTime* delay = CCDelayTime::create(b);
                            CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                            sb->runAction(CCSequence::create(delay, call, NULL));
                            ap = ap - sb->getDef();
                            {
                                heroLayer->showBattleLabel( sb->getDef() , "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,b);
                            }
                            if (sb->getSbType() == ksbtwo || sb->getSbType() == ksbfour) {
                                i++;
                            }
                        }
                        
                    }else if(grid->getStype() == kdefend)
                    {
                        ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                        if(sb)
                        {
                            CCDelayTime* delay = CCDelayTime::create(b);
                            CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                            sb->runAction(CCSequence::create(delay, call, NULL));
                            int originAp = ap;
                            ap = ap - sb->getDef();

                            //if (ap < 0)
                            {
                                heroLayer->showBattleLabel( (ap > 0 ?  sb->getDef(): originAp), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,b);
                            }
                            sb->setDef(-ap);
                        }
                        
                    }else
                    {
                        SGAttackList* attack = heroLayer1->battleMap->getAttackListByGrid(grid);
                        if(attack)
                        {
                            int originAp = ap;
                            
                            ap = ap - attack->getAp();
                            

                            
                            attack->setAp(-ap);
                            // 打到武将身上
                            if ((grid->getStype() == kattacktwo || grid->getStype() == kattackfour) && ap > 0)
                            {
                                attack->playEffectWithType(kAttackEffect);
                            }
                            
                            
                            CCDelayTime* delay = CCDelayTime::create(b);
                            CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                            ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                            sb->runAction(CCSequence::create( delay, call, NULL));
                            
                            //if (ap < 0)
                            {
                                heroLayer->showBattleLabel( (ap > 0 ?  attack->getAp(): originAp), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,b);
                            }
                            
                            
                            if (attack->getAttackType() == kattacknormal) {
                                i += 2;
                            }else{
                                i += 1;
                            }
                        }
                    }
                    
                    if (ap <= 0) {
                        CCDelayTime* delay1 = CCDelayTime::create(b);
                        CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                                   callfuncND_selector(SGHeroSkill28::removeBullet),
                                                                   (void*)(SKILLS_BULLET_TAG));
                        GameSbIndex *sbin = new GameSbIndex();
                        sbin->i = i;
                        sbin->j = indexJ;
                        CCCallFuncND *call3 = CCCallFuncND::create(SGSkillManager::shareSkill(),
                                                                   callfuncND_selector(SGSkillManager::playBulletBlast),
                                                                   (void*)sbin);
                        
                        heroLayer->runAction(CCSequence::create(delay1, call1, call3, NULL));
                    }
                }
            }
        }
        else{
            
            break;
        }
    }
    if (!SGAIManager::shareManager()->isPVE)
    {
        heroLayer->battleMap->refreshMapGrid(true);
    }
    
}

void SGHeroSkill28::removeBullet(cocos2d::CCObject *obj, void* tag)
{
    CCLOG("removeBullet");
    
    ((SGHeroLayer*)obj)->removeChildByTag( (long)tag, true);
    if (SGAIManager::shareManager()->isPVE) {
        ((SGHeroLayer*)obj)->battleMap->refreshMapGrid(true);
        SGAIManager::shareManager()->isAi = true;
        if (((SGHeroLayer*)obj)->battleMap->_isRefreshMap ||
            ((SGHeroLayer*)obj)->battleMap->m_touchState != eTouchNull)
        {
        }else{
            if (SGSkillManager::shareSkill()->isBeginAi)
                SGSkillManager::shareSkill()->beginAi();
        }
        if (SGSkillManager::shareSkill()->isBeginAi)
            SGSkillManager::shareSkill()->beginUpTime();
    }
    
}