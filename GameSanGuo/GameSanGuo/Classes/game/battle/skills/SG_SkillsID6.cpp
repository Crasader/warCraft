//
//  SG_SkillsID6.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#include "SG_SkillsID6.h"


SG_SkillsID6::SG_SkillsID6()
{
    setHeroLayer();
}


SG_SkillsID6::~SG_SkillsID6(){
    
}
//瞬杀 蓄力完成后，随机吞噬一个对方处于蓄力阶段的2格武将，并吸收其5%攻击力
void SG_SkillsID6::activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    bool isme = true;
    SGBattleMap *enemyBattleMap = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        enemyBattleMap = getenemyHero()->getBattleMap();
    }else{
        enemyBattleMap = getmyHero()->getBattleMap();
        isme = false;
    }
    
    CCObject *obj = NULL;
    CCArray *array = CCArray::create();
	//将对方目前阵列中所有的二格武将加入数组中
    CCARRAY_FOREACH(enemyBattleMap->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        
		//如果是二格武将
        if (attack->getAttackType() == kattacktwo) {
            array->addObject(attack);
        }
    }
    //如果有二格武将
    if (array->count()) {
        int randNum = 0;
        int a = attackList->getAfftributeNum2();
        randNum =  a%array->count();//获取含有二格武将里的随机的一个二格武将

        SGAttackList *attack = (SGAttackList*)array->objectAtIndex(randNum);
        GameIndex index =  attack->getAttackIndex();


        CCString *name = CCString::createWithFormat("animationFile/%s.plist","paoXiao");
        CCString *name1 = CCString::createWithFormat("animationFile/%s.scml","paoXiao");
        ResourceManager::sharedInstance()->bindTexture(name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
        
        CCSpriterX *effect = CCSpriterX::create(name1->getCString(), true, true);
        if (effect) {
            effect->setanimaID(0);
            effect->setisloop(false);
            effect->play();
            effect->setPosition(GameConfig::getGridPoint(!isme ? index.i + 1 : -(index.i+1),   abs(!isme ? -index.j :  (mapList - 1 - index.j)),true));
            float c = effect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM;
            getmyHero()->addChild(effect, 1000);
            CCDelayTime *del = CCDelayTime::create(c);
            CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SG_SkillsID6::removePlist));
            effect->runAction(CCSequence::create(del, call, NULL));
            
//            GameIndex index1 = attackList->getAttackIndex();
//            name1 = CCString::createWithFormat("animationFile/%s_000.scml","nlq");
//            CCSpriterX *effect1 = CCSpriterX::create(name1->getCString(), true, true);
//            if (effect1) {
//                effect1->setanimaID(0);
//                effect1->setisloop(true);
//                effect1->play();
//                effect1->setPosition(GameConfig::getGridPoint(!isme ? index.i + 1 : -(index.i+1),   abs(!isme ? -index.j :  (mapList - 1 - index.j)),true));
//                
//                CCPoint aapos = GameConfig::getGridPoint(!isme ? index.i + 1 : -(index.i+1),   abs(!isme ? -index.j :  (mapList - 1 - index.j)),true);
//                CCLOG("x====%f", aapos.x);
//                
//                CCPoint pos = GameConfig::getGridPoint(isme ? index1.i + 1 : -(index1.i+1),   abs(isme ? -index1.j + 1 :  (mapList - 1 - index1.j)),true);
//                CCLOG("x====%f", pos.x);
//                getmyHero()->addChild(effect1);
//                
//                CCHide *hide = CCHide::create();
//                CCDelayTime *dal = CCDelayTime::create(c);
//                CCShow *show = CCShow::create();
//                CCMoveTo *move = CCMoveTo::create(0.2, pos);
//                CCString *str = CCString::create("nlq");
//                str->retain();
//                CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SG_SkillsID6::removePlist), (void*)str);
//                effect1->runAction(CCSequence::create(hide, dal, show, move, call, NULL));
//            }

        }

        
//        GameConfig::getGridPoint(isme ? index.i + 1 : -(index.i+1),   abs(isme ? -index.j :  (mapList - 1 - index.j)),true)
        
//        float b = attackList->getAfftributeNum();
        CCLOG("aattackList->getAfftributeNum()==%f",attackList->getAfftributeNum());
//        CCLOG("attack->getAp() * b==%d",attack->getAp() * b);
		//吸收被干掉的这个攻击阵列的 attackList->getAfftributeNum()的百分比的攻击力,加到自己的攻击身上
        attackList->changeAP(attackList->getAp() + floor(attack->getAp() * attackList->getAfftributeNum()));
        
		//干掉这个武将
        enemyBattleMap->removeAttackList(attack, enemyBattleMap->getHero_Obj(), enemyBattleMap->myGrids[ attack->getAttackIndex().i][attack->getAttackIndex().j]);
    }
    
}

void SG_SkillsID6::removePlist(CCObject *obj, CCString *str)
{
    CCSpriterX *sp = (CCSpriterX*) obj;
    sp->removeFromParentAndCleanup(true);
    sp = NULL;
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("animationFile/paoXiao.plist");
}


