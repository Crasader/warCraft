//
//  SGHeroSkill15.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill15.h"
#include "SGSkillManager.h"

SGHeroSkill15::SGHeroSkill15()
{
    
}

SGHeroSkill15::~SGHeroSkill15()
{
    
}

//对敌方全列，进行武将攻击力总额75%的伤害
float SGHeroSkill15::activateSkill_20022(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid)
{
    float time = 1.2;
    float sbTime = time/12;
    
    bool isme = true;
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() != roleid)
    {
        isme = false;
    }
    
    float skillDelayTime = 0;
    for (int i = 0; i < mapList; i++) {
        //和业务逻辑无关
        CCString *name = NULL;
        CCString *name1 = NULL;
        name = CCString::createWithFormat("animationFile/jian_%d.plist", heroLayer->m_jianId);
        name1 = CCString::createWithFormat("animationFile/jian_%d.scml", heroLayer->m_jianId);

        ResourceManager::sharedInstance()->bindTexture(name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
        CCSpriterX *fermEffect = CCSpriterX::create(name1->getCString(), true, true);
        fermEffect->setanimaID(0);
        fermEffect->setisloop(true);
        fermEffect->setPosition(GameConfig::getGridPoint(isme ? 5 : -5 + OLD_EFFECT_FLIPY_FIX,   abs(isme ? -i :  (mapList - 1 - i)),true));
        fermEffect->play();
        heroLayer->addChild(fermEffect, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG + i);
        
        if (!isme) {
            //MMDEBUG: FLIPY
            fermEffect->setScaleY(-1);
//            fermEffect->setPosition(ccp(0, GameConfig::getGridSize().height * 1.5));
//            fermEffect->setCCSpriterXFlipY(true);
        }
        
        CCDelayTime *delaytime = CCDelayTime::create(sbTime*0.5*i);
        CCMoveTo *move = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow : mapRow + OLD_EFFECT_FLIPY_FIX, abs(isme ? -i : (mapList - 1 - i)) ,true));
        fermEffect->runAction(CCSequence::create(delaytime, move, NULL));
        
        //        int a = (fermEffect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM) * 10000;
        //
        //
        //        CCDelayTime *time = CCDelayTime::create(a);
        //        CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::sbSkill_20005), (void*)a);
        //        sb->runAction(CCSequence::create(time, call, NULL));
        
        
        
    }
    
    
    for (int j = 0; j < mapList; j++) {
        int ap = value1;
        for (int i = 0; i <= mapRow; i++) {
            if (ap > 0) {
                float a = sbTime * 0.5 * j;
                float b = sbTime * (i+5.5);
                
                skillDelayTime = a + b ;
                if (i == mapRow) {
                    //播放打到底线动作
                    GameSbIndex *sbin = new GameSbIndex();
                    sbin->i = mapRow;
                    sbin->j = j;
                    CCDelayTime* deal0 = CCDelayTime::create(a);
                    CCDelayTime* delay1 = CCDelayTime::create(b + sbTime);
                    CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGHeroSkill15::removeBullet),
                                                               (void*)(SKILLS_BULLET_TAG + j));
                    CCCallFuncND* call3 = CCCallFuncND::create(SGSkillManager::shareSkill(),
                                                               callfuncND_selector(SGSkillManager::playBulletBlast),
                                                               (void*)sbin);
                    heroLayer->runAction(CCSequence::create( deal0, delay1, call1, call3, NULL));
                    
                    CCDelayTime* deal01 = CCDelayTime::create(a);
                    CCDelayTime* delay11 = CCDelayTime::create(b + sbTime);
                    CCCallFuncND* call11 = CCCallFuncND::create(SGSkillManager::shareSkill(),
                                                                callfuncND_selector(SGSkillManager::changBloodNum),
                                                                (void*)ap);
                    heroLayer1->runAction(CCSequence::create( deal01, delay11, call11, NULL));
                    
                    
                }else{
                    SGGridBase *grid = heroLayer1->battleMap->myGrids[i][abs(mapList-1-j)];
                    if (grid) {
                        if (grid->getStype() == knormal)
                        {
                            
                            ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                            if(sb)
                            {
                                ap = ap - sb->getDef();
                            
                                heroLayer1->showBattleLabel( sb->getDef() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,a+b);
                                if (sb->getSbType() == ksbtwo || sb->getSbType() == ksbfour) {
                                    //                                sb->removeSBGrid();
                                    i++;
                                }
                                sb->setDef(0);
                                CCDelayTime* deal = CCDelayTime::create(a);
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create( deal, delay, call, NULL));
                            }
                            
                            
                            
                        }
                        else if(grid->getStype() == kdefend)
                        {
                            ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                            if(sb)
                            {
                                ap = ap - sb->getDef();
                                //if (ap < 0)
                                {
                                    heroLayer1->showBattleLabel( sb->getDef() + ap, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,a+b);
                                }
                                
                                sb->setDef(-ap);
                                CCDelayTime* deal = CCDelayTime::create(a);
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create( deal, delay, call, NULL));
                            }
                            
                        }
                        else
                        {
                            SGAttackList* attack = heroLayer1->battleMap->getAttackListByGrid(grid);
                            if(attack)
                            {
                                
                                ap = ap - attack->getAp();
                                int aa = 0;
                                if (ap >= 0) {
                                    aa = attack->getAp();
                                }else{
                                    aa = ap;
                                }
                                attack->heroSkillSubAp(aa);
                                attack->setAp(-ap);
                                
                                if ((grid->getStype() == kattackfour || grid->getStype() == kattacktwo) && ap > 0)
                                {
                                    attack->playEffectWithType(kAttackEffect);
                                }
                                
                                CCDelayTime* deal = CCDelayTime::create(a);
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                                sb->runAction(CCSequence::create( deal, delay, call, NULL));
                                
                                //if (ap < 0)
                                {
                                    heroLayer1->showBattleLabel( sb->getDef() + ap, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,a+b);
                                }
                                
                                if (attack->getAttackType() == kattacknormal) {
                                    i += 2;
                                }else{
                                    i += 1;
                                }
                            }
                            
                        }
                        
                        
                        if (ap <= 0) {
                            GameSbIndex *sbin = new GameSbIndex();
                            sbin->i = i;
                            sbin->j = j;
							
							//george//10.23
							sbin->autorelease();
                            CCDelayTime* deal0 = CCDelayTime::create(a);
                            CCDelayTime* delay1 = CCDelayTime::create(b);
                            CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                                       callfuncND_selector(SGHeroSkill15::removeBullet),
                                                                       (void*)(SKILLS_BULLET_TAG + j));
                            heroLayer->runAction(CCSequence::create( deal0, delay1, call1, NULL));
                        }
                        
                    }
                }
            }
            else{
                break;
            }
        }
    }
    return (skillDelayTime + sbTime)*3;
}

void SGHeroSkill15::removeBullet(cocos2d::CCObject *obj, void* tag)
{
    CCLOG("removeBullet");
    
    ((SGHeroLayer*)obj)->removeChildByTag( (long)tag, true);
    if (SGAIManager::shareManager()->isPVE) {
        SGAIManager::shareManager()->isAi = true;
        if (((SGHeroLayer*)obj)->battleMap->_isRefreshMap ||
            ((SGHeroLayer*)obj)->battleMap->m_touchState != eTouchNull)
        {
        }else{
            if (SGSkillManager::shareSkill()->isBeginAi){
                CCLog("**********SGSkillManager::beginAi()**********");
                SGSkillManager::shareSkill()->beginAi();
               

            }
        }
        if (SGSkillManager::shareSkill()->isBeginAi)
            SGSkillManager::shareSkill()->beginUpTime();
    }
    
}
