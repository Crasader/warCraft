//
//  SGHeroSkill21.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill21.h"
#include "SGSkillManager.h"

SGHeroSkill21::SGHeroSkill21()
{
    
}

SGHeroSkill21::~SGHeroSkill21()
{
    
}

//将所有己方墙转化为火球进行攻击（墙当前的列不变）攻击力等同于当前墙的防御值。
void SGHeroSkill21::activateSkill_20038(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid, int jianid)
{
//        业务逻辑无关
	seconds = 0;
	//如果没有array数据
	if (SGSkillManager::shareSkill()->array->count())
	{
	
		ShiBing* sb = (ShiBing*)SGSkillManager::shareSkill()->array->objectAtIndex(0);
		int ap = /*sb->getDef() * */value1;
		SGGridBase* sbgrid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
		heroLayer->battleMap->removeAttackedSB(sb, 0);
		heroLayer->showEffectLayer(7, true, sb->getSbindex());
		float a = 0.5 * ANIMA_TIME_NUM;
		CCDelayTime *dtime = CCDelayTime::create(a);
        seconds += a;
		CCCallFuncND *call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::huoQiu), (void*)sb);
		heroLayer->runAction(CCSequence::create(dtime, call, NULL));
		float b = 0.6 * ANIMA_TIME_NUM;
		float c = a + b;
		float time = 0.5;
		float sbTime = time/ (mapRow + sbgrid->getIndex().i);
		bool isme = true;
		if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() != roleid)
		{
			isme = false;
		}
		int num = jianid;
		num = 1;
	//        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(CCString::createWithFormat("animationFile/jian_%d.plist", num)->getCString());
		ResourceManager::sharedInstance()->bindTexture(CCString::createWithFormat("animationFile/jian_%d.plist", num)->getCString(),
													   RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
		CCSpriterX *fermEffect = CCSpriterX::create(CCString::createWithFormat("animationFile/jian_%d.scml",num)->getCString(), true, true);
		fermEffect->setanimaID(0);
		fermEffect->setisloop(true);
		fermEffect->play();
		heroLayer->addChild(fermEffect, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG + num);
		
		if (false == isme) {
            //MMDEBUG: FLIPY
            fermEffect->setScaleY(-1);
            fermEffect->setPosition(ccp(0, GameConfig::getGridSize().height * 1.5));
//			fermEffect->setCCSpriterXFlipY(true);
		}
		
		fermEffect->setPosition(GameConfig::getGridPoint(isme ? sbgrid->getIndex().i : -sbgrid->getIndex().i + OLD_EFFECT_FLIPY_FIX,   abs(isme ? -sbgrid->getIndex().j :  (mapList - 1 - sbgrid->getIndex().j)),true));
		CCHide *hide = CCHide::create();
		CCDelayTime *time_d = CCDelayTime::create(c);
		CCShow *show = CCShow::create();
		CCMoveTo *move = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow : mapRow + OLD_EFFECT_FLIPY_FIX, abs(isme ? -sbgrid->getIndex().j : (mapList - 1 - sbgrid->getIndex().j)) ,true));
		fermEffect->runAction(CCSequence::create(hide, time_d, show, move, NULL));
		seconds += c;
        
	//        逻辑开始
		for (int i = 0; i <= mapRow; i++) {
			
			if (ap > 0) {
				float b = sbTime * (i+sbgrid->getIndex().i);
				b = b + c;
				if (i == mapRow) {
					//播放打到底线动作
					GameSbIndex *sbin = new GameSbIndex();
					sbin->i = mapRow;
					sbin->j = sbgrid->getIndex().j;
					CCDelayTime* delay1 = CCDelayTime::create(b + sbTime);
                    seconds += (b + sbTime);
					CCCallFuncND* call1 = CCCallFuncND::create(this,
															   callfuncND_selector(SGHeroSkill21::removeBullet),
															   (void*)(SKILLS_BULLET_TAG + num));
					CCCallFuncND* call12 = CCCallFuncND::create(SGSkillManager::shareSkill(),
																callfuncND_selector(SGSkillManager::playBulletBlast),
																(void*)sbin);
					
					heroLayer->runAction(CCSequence::create( delay1, call1, call12, NULL));
					
					CCDelayTime* delay11 = CCDelayTime::create(b);
                    seconds += b;
					CCCallFuncND* call11 = CCCallFuncND::create(SGSkillManager::shareSkill(),
																callfuncND_selector(SGSkillManager::changBloodNum),
																(void*)ap);
					CCDelayTime* delay13 = CCDelayTime::create(0.4);
                    seconds += 0.4;
					CCCallFunc* call3 = CCCallFunc::create(this, callfunc_selector(SGHeroSkill21::callback));
					heroLayer1->runAction(CCSequence::create( delay11, call11, delay13, call3, NULL));
					
					
				}else{
					SGGridBase *grid = heroLayer1->battleMap->myGrids[i][abs(mapList-1-sbgrid->getIndex().j)];
					if (grid) {
						if (grid->getStype() == knormal) {
							
							ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                            if(sb)
                            {
                                CCDelayTime* delay = CCDelayTime::create(b);
                                seconds += b;
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create( delay, call, NULL));
                                ap = ap - sb->getDef();
                                {
                                    heroLayer1->showBattleLabel( sb->getDef()  , "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,b);
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
                                ap = ap - sb->getDef();
                                //if (ap < 0)
                                {
                                    heroLayer1->showBattleLabel( sb->getDef() + ap , "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,b);
                                }
                                sb->setDef(-ap);
                                CCDelayTime* delay = CCDelayTime::create(b);
                                seconds += b;
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create( delay, call, NULL));
                            }
						}else
						{
							SGAttackList* attack = heroLayer1->battleMap->getAttackListByGrid(grid);
                            if(attack)
                            {
                                ap = ap - attack->getAp();
                                int a = 0;
                                if (ap >= 0) {
                                    a = attack->getAp();
                                }else{
                                    a = ap;
                                }
                                attack->heroSkillSubAp(a);
                                attack->setAp(-ap);
                                
                                if ((grid->getStype() == kattackfour || grid->getStype() == kattacktwo) && ap > 0)
                                {
                                    attack->playEffectWithType(kAttackEffect);
                                }
                                
                                
                                CCDelayTime* delay = CCDelayTime::create(b);
                                seconds += b;
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                                sb->runAction(CCSequence::create( delay, call, NULL));
                                //if (ap < 0)
                                {
                                    heroLayer1->showBattleLabel( sb->getDef() + ap , "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,b);
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
							sbin->j = sbgrid->getIndex().j;
							CCDelayTime* delay12 = CCDelayTime::create(b);
                            seconds += b;
							CCCallFuncND* call12 = CCCallFuncND::create(this,
																		callfuncND_selector(SGHeroSkill21::removeBullet),
																		(void*)(SKILLS_BULLET_TAG + num));
							CCCallFuncND* call14 = CCCallFuncND::create(SGSkillManager::shareSkill(),
																		callfuncND_selector(SGSkillManager::playBulletBlast),
																		(void*)sbin);
							CCDelayTime* delay13 = CCDelayTime::create(0.4);
                            seconds += 0.4;
							CCCallFunc* call3 = CCCallFunc::create(this, callfunc_selector(SGHeroSkill21::callback));
							heroLayer->runAction(CCSequence::create( delay12, call12, call14, delay13, call3, NULL));
							
						}
					}
				}
			}
			else{
				break;
			}
		}
	}
	else
	{
		SGSkillManager::shareSkill()->array->removeAllObjects();
        SGSkillManager::shareSkill()->num = 0;
        SGSkillManager::shareSkill()->valueNum = 0;
        SGSkillManager::shareSkill()->roleidNum = 0;
        SGSkillManager::shareSkill()->jianNum = 0;
        
        SGSkillManager::shareSkill()->heroMe->battleMap->refreshMapGrid(true);
	}
}

void SGHeroSkill21::callback()
{
    SGSkillManager::shareSkill()->array->removeObjectAtIndex(0);
    if (SGSkillManager::shareSkill()->array->count()) {
        
        activateSkill_20038(SGSkillManager::shareSkill()->heroMe, SGSkillManager::shareSkill()->heroEnemy, SGSkillManager::shareSkill()->valueNum, SGSkillManager::shareSkill()->roleidNum, SGSkillManager::shareSkill()->jianNum);
    }else{
        CCLOG("所有火球攻击完毕！！");
        SGSkillManager::shareSkill()->array->removeAllObjects();
        SGSkillManager::shareSkill()->num = 0;
        SGSkillManager::shareSkill()->valueNum = 0;
        SGSkillManager::shareSkill()->roleidNum = 0;
        SGSkillManager::shareSkill()->jianNum = 0;
        
        SGSkillManager::shareSkill()->heroMe->battleMap->refreshMapGrid(true);
    }
}

void SGHeroSkill21::removeBullet(cocos2d::CCObject *obj, void* tag)
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
                CCLog("******，开始ai,SGHeroSkill21::removeBullet*********");
                SGSkillManager::shareSkill()->beginAi();
            }
            
        }
        if (SGSkillManager::shareSkill()->isBeginAi)
            SGSkillManager::shareSkill()->beginUpTime();
    }
    
}