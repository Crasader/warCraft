//
//  SGHeroSkill13.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill13.h"
#include "SGSkillManager.h"

SGHeroSkill13::SGHeroSkill13()
{
    
}

SGHeroSkill13::~SGHeroSkill13()
{
    
}

//火烧连营
void SGHeroSkill13::activateSkill_20018(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, GameIndex index, cocos2d::CCArray *array, float value2, int num, bool isend)
{
    
    SGBattleMap *battleMap = heroLayer->getBattleMap();
    SGGridBase *grid = battleMap->myGrids[index.i][index.j];
    ShiBing* sb = battleMap->getShiBingWithGrid(grid);
    if(sb)
    {
        heroLayer->showEffectLayer(7, false, sb->getSbindex());
        float t = 0;
        float a = t + 0.5 * ANIMA_TIME_NUM;
        CCDelayTime *time = CCDelayTime::create(a);
        CCCallFuncND *call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::baoZa), (void*)sb);
        heroLayer->runAction(CCSequence::create(time, call, NULL));
        battleMap->removeAttackedSB(sb, 0);
        
        if (isend) {
            float time = 2.0;
            for (int i = 0; i < array->count(); i++) {
                EndObjData *endData = (EndObjData*) array->objectAtIndex(i);
                int x = endData->x;
                int y = endData->y;
                int ap = endData->num;
                SGGridBase *grid = battleMap->myGrids[y][x];
                if (grid) {
                    switch (grid->getStype()) {
                        case knormal:
                        {
                            ShiBing* sb = battleMap->getShiBingWithGrid(grid);
                            if(sb)
                            {
                                CCDelayTime* delay = CCDelayTime::create(time);
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer);
                                sb->runAction(CCSequence::create( delay, call, NULL));
                            }
                        }
                            break;
                        case kdefend:
                        {
                            ShiBing* sb = battleMap->getShiBingWithGrid(grid);
                            if(sb)
                            {
                                heroLayer->showBattleLabel( sb->getDef() - ap, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                                sb->setDef(ap);
                                
                                CCDelayTime* delay = CCDelayTime::create(time);
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer);
                                sb->runAction(CCSequence::create( delay, call, NULL));
                            }
                            
                        }
                            break;
                        case kattack:
                        {
                            SGAttackList* attack = battleMap->getAttackListByGrid(grid);
                            if(attack)
                            {
                                heroLayer->showBattleLabel(attack->getAp() - ap, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attack->getAttackIndex());
                                //int a = 0;
                                if (ap <= 0) {
                                    a = attack->getAp();
                                }else{
                                    a = attack->getAp() - ap;
                                }
                                //attack->heroSkillSubAp(a);
                                attack->setAp(ap);
                                if ((grid->getStype() == kattackfour || grid->getStype() == kattacktwo) && ap > 0)
                                {
                                    attack->playEffectWithType(kAttackEffect);
                                }
                                
                                CCDelayTime* delay = CCDelayTime::create(time);
                                CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer);
                                ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                                sb->runAction(CCSequence::create( delay, call, NULL));
                            }
                        }
                            break;
                            
                        default:
                            break;
                    }
                }
                
                //CC_SAFE_DELETE(endData);
            }
            
            
            //array->removeAllObjects();
            //array->release();
        
        }
    }
}

//将敌方所有的墙转化为炸弹，伤害周围所有相邻单位（极限情况是5个）
//无处调用，已改为上面的方法！神经么？
void SGHeroSkill13::activateSkill_20018(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, GameIndex index, cocos2d::CCArray *array, float value2, int num)
{
    return;
    SGBattleMap *battleMap = heroLayer->getBattleMap();
    SGGridBase *grid = battleMap->myGrids[index.i][index.j];
    ShiBing* sb = battleMap->getShiBingWithGrid(grid);
    if(sb)
    {
        heroLayer->showEffectLayer(7, false, sb->getSbindex());
        float t = num * 0.2;
        float a = t + 0.5 * ANIMA_TIME_NUM;
        CCDelayTime *time = CCDelayTime::create(a);
        CCCallFuncND *call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::baoZa), (void*)sb);
        heroLayer->runAction(CCSequence::create(time, call, NULL));
        battleMap->removeAttackedSB(sb, 0);
        
        float b = 1.6 * ANIMA_TIME_NUM;
        
        float c = a + b;
        int ap = /*sb->getDef() * */value2;
        
        CCLOG("开始攻击了-------------------------------------> ap：%d",ap);
        
        for (int i = 0; i < 8; i++) {
            if (ap > 0) {
                SGGridBase* grid1 = NULL;
                GameIndex index1 = gameIndex(-1, -1);
                switch (i) {
                    case 0:
                    {
                        grid1 = battleMap->myGrids[index.i - 1][index.j - 1];
                        index1 = gameIndex(index.i - 1, index.j - 1);
                    }
                        break;
                    case 1:
                    {
                        grid1 = battleMap->myGrids[index.i][index.j - 1];
                        index1 = gameIndex(index.i, index.j - 1);
                    }
                        break;
                    case 2:
                    {
                        grid1 = battleMap->myGrids[index.i + 1][index.j - 1];
                        index1 = gameIndex(index.i + 1, index.j - 1);
                    }
                        break;
                    case 3:
                    {
                        grid1 = battleMap->myGrids[index.i - 1][index.j];
                        index1 = gameIndex(index.i - 1, index.j);
                    }
                        break;
                    case 4:
                    {
                        grid1 = battleMap->myGrids[index.i + 1][index.j];
                        index1 = gameIndex(index.i + 1, index.j);
                    }
                        break;
                        
                    case 5:
                    {
                        grid1 = battleMap->myGrids[index.i - 1][index.j + 1];
                        index1 = gameIndex(index.i - 1, index.j + 1);
                    }
                        break;
                    case 6:
                    {
                        grid1 = battleMap->myGrids[index.i][index.j + 1];
                        index1 = gameIndex(index.i, index.j + 1);
                    }
                        break;
                    case 7:
                    {
                        grid1 = battleMap->myGrids[index.i + 1][index.j + 1];
                        index1 = gameIndex(index.i + 1, index.j + 1);
                    }
                        break;
                        
                        
                    default:
                        break;
                }
                
                if (grid1) {
                    
                    if (index1.i < 0 || index1.i >= mapRow ||
                        index1.j < 0 || index1.j >= mapList) {
                        CCLOG("应该为空！！");
                        continue;
                    }
                    
                    bool isAttack = true;
                    for (int j = 0; j < array->count(); j++) {
                        SGGridBase* grid2 = (SGGridBase*)array->objectAtIndex(j);
                        if (grid1 == grid2) {
                            isAttack = false;
                        }
                    }
                    if (isAttack) {
                        switch (grid1->getStype()) {
                            case knormal:
                            {
                                ShiBing* sb = battleMap->getShiBingWithGrid(grid1);
                                if (sb&&sb->getDef() > 0) {
                                    ap = ap - sb->getDef();
                                    CCLOG("i=%d-------------------------------------> ap=%d  sb->getDef()=%d", i,ap, sb->getDef());
                                    CCDelayTime* delay = CCDelayTime::create(c);
                                    CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                    sb->runAction(CCSequence::create( delay, call, NULL));
                                     heroLayer->showBattleLabel( sb->getDef(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,c);
                                }
                                
    //                            CCDelayTime* delay = CCDelayTime::create(c);
    //                            CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
    //                            sb->runAction(CCSequence::create( delay, call, NULL));
                            }
                                break;
                            case kdefend:
                            {
                                ShiBing* sb = battleMap->getShiBingWithGrid(grid1);
                                
                                if (sb&&sb->getDef() > 0) {
                                    ap = ap - sb->getDef();
                                    if (ap < 0) {
                                        heroLayer->showBattleLabel( sb->getDef() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex(),false,c);
                                    }
                                    CCLOG("i=%d-------------------------------------> ap=%d  sb->getDef()=%d", i,ap, sb->getDef());
                                    sb->setDef(-ap);
                                    
                                    CCDelayTime* delay = CCDelayTime::create(c);
                                    CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                    sb->runAction(CCSequence::create( delay, call, NULL));
                                }
                                
    //                            CCDelayTime* delay = CCDelayTime::create(c);
    //                            CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
    //                            sb->runAction(CCSequence::create( delay, call, NULL));
                                
                            }
                                break;
                            case kattack:
                            {
                                SGAttackList* attack = battleMap->getAttackListByGrid(grid1);
                                if (attack&&attack->getAp() > 0) {
                                    ap = ap - attack->getAp();
                                    
                                    int a = 0;
                                    if (ap >= 0) {
                                        a = attack->getAp();
                                    }else{
                                        a = ap;
                                    }
                                    attack->heroSkillSubAp(a);
                                    
                                    CCLOG("i=%d-------------------------------------> ap=%d  sb->getDef()=%d", i,ap, attack->getAp());
                                    //if (ap < 0)
                                    {
                                        heroLayer->showBattleLabel( attack->getAp() + ap , "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attack->getAttackIndex(),false,c);
                                    }

                                    attack->setAp(-ap);
                                    
                                    if (ap > 0)
                                    {

                                        attack->playEffectWithType(kAttackEffect);
                                    }
                                    
                                    CCDelayTime* delay = CCDelayTime::create(c);
                                    CCCallFuncND* call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                    ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                                    sb->runAction(CCSequence::create( delay, call, NULL));
                                }
                                
                                
    //                            CCDelayTime* delay = CCDelayTime::create(c);
    //                            CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
    //                            ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
    //                            sb->runAction(CCSequence::create( delay, call, NULL));
                            }
                                break;
                                
                            default:
                                break;
                        }
                    }
                }
            }else{
                break;
            }
            
        }
    
    CCLOG("结束攻击了-------------------------------------> ap：%d",ap);
    }
    
}

//void SGHeroSkill13::baoZa(CCObject *obj, ShiBing *sb)
//{
//    SGHeroLayer *hero = (SGHeroLayer*)obj;
//    hero->showEffectLayer(8, false, sb->getSbindex());
//}

