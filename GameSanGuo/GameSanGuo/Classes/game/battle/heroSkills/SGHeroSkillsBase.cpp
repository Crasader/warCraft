//
//  SGHeroSkillsBase.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkillsBase.h"


SGHeroSkillsBase::SGHeroSkillsBase()
{
    
}

SGHeroSkillsBase::~SGHeroSkillsBase()
{
    
}


void SGHeroSkillsBase::activateSkill_20000(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid){}
float SGHeroSkillsBase::activateSkill_20001(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid){}
void SGHeroSkillsBase::activateSkill_20002(SGHeroLayer *heroLayer){}
void SGHeroSkillsBase::activateSkill_20003(SGHeroLayer *heroLayer, int value1, int value2, int roleid){}
void SGHeroSkillsBase::activateSkill_20004(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float valule2,int heroId){}
void SGHeroSkillsBase::activateSkill_20005(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, CCPoint point, int roleid, int skillid){}
void SGHeroSkillsBase::activateSkill_20006(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, CCPoint point, int skillId){}
void SGHeroSkillsBase::activateSkill_20007(SGHeroLayer *heroLayer, float value1, int value2, int buffid){}
void SGHeroSkillsBase::activateSkill_20008(SGHeroLayer *heroLayer, float rate){}
void SGHeroSkillsBase::activateSkill_20010(SGHeroLayer *heroLayer,  float rate){}
void SGHeroSkillsBase::activateSkill_20012(SGHeroLayer *heroLayer,SGHeroLayer *heroLayer1, float value1, int s_id, CCPoint pos){}
void SGHeroSkillsBase::activateSkill_20014(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid){}
void SGHeroSkillsBase::activateSkill_20018(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, GameIndex index, cocos2d::CCArray *array, float value2, int num){}
void SGHeroSkillsBase::activateSkill_20018(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, GameIndex index, cocos2d::CCArray *array, float value2, int num, bool isend){}
void SGHeroSkillsBase::activateSkill_20020(SGHeroLayer *heroLayer, int addValue){}
float SGHeroSkillsBase::activateSkill_20022(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid){}
void SGHeroSkillsBase::activateSkill_20024(SGHeroLayer *heroLayer, int value){}
void SGHeroSkillsBase::activateSkill_20028(SGHeroLayer *heroLayer,  float value1, float value2){}
void SGHeroSkillsBase::activateSkill_20030(SGHeroLayer *heroLayer, int skillsId){}
void SGHeroSkillsBase::activateSkill_20033(SGHeroLayer *heroLayer, float value){}
void SGHeroSkillsBase::activateSkill_20034(SGHeroLayer *heroLayer, int round, int buffid){}
void SGHeroSkillsBase::activateSkill_20038(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid, int jianid){}
void SGHeroSkillsBase::activateSkill_20046(SGHeroLayer *heroLayer, float value){}
void SGHeroSkillsBase::activateSkill_20048(SGHeroLayer *heroLayer, float value){}
void SGHeroSkillsBase::activateSkill_20052(SGHeroLayer *heroLayer, float value1,int buffId){}
void SGHeroSkillsBase::activateSkill_20058(SGHeroLayer *heroLayer, float value1){}
void SGHeroSkillsBase::activateSkill_20060(SGHeroLayer *heroLayer){}
void SGHeroSkillsBase::activateSkill_20063(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, int roleid, int buffid, int round, float value1, float value2, float value3){}
void SGHeroSkillsBase::activateSkill_20065(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid){}
void SGHeroSkillsBase::activateSkill_20066(SGHeroLayer *heroLayer, float value){}

//增加现在所有蓄力状态阵列攻击力20%
void SGHeroSkillsBase::activateSkill_20032(SGHeroLayer *heroLayer, float value)
{
    CCArray *attackList = heroLayer->getBattleMap()->getAttackList();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackList, obj)
    {
        SGAttackList *attackList = (SGAttackList *)obj;
//        int ap = attackList->getMax_Ap() * value;
        int ap = value;
        attackList->setSpAddAp(attackList->getSpAddAp() + ap);
        attackList->changeAP(attackList->getAp() + ap, true);
        attackList->drawSkillsEffect(2);
    }
}

//void SGHeroSkillsBase::removesb(cocos2d::CCObject *obj, SGHeroLayer*hero)
//{
//    ShiBing* sb = (ShiBing*)obj;
//    SGGridBase* grid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
//    if (grid->getStype() == knormal) {
//        hero->battleMap->removeAttackedSB(sb, 0);
//    }else if(grid->getStype() == kdefend)
//    {
//        if (sb->getDef() > 0) {
//            sb->setDefImage();
//            sb->showDef(sb->getDef());
//        }else{
//            hero->battleMap->removeAttackedSB(sb, 0, hero);
//        }
//    }else{
//        SGAttackList* attack = hero->battleMap->getAttackListByGrid(grid);
//        if (attack->getAp() > 0 &&
//            hero->battleMap->myGrids[grid->getIndex().i][grid->getIndex().j]) {
//            attack->changeAP(attack->getAp());
//        }else{
//            hero->battleMap->removeAttackList(attack, hero, grid);
//        }
//    }
//    CCLOG("删除了兵。。。");
//}

void SGHeroSkillsBase::beginAi(SGHeroLayer *heroLayer)
{
    SGAIManager::shareManager()->startRound(heroLayer->battleMap);
}

void SGHeroSkillsBase::beginUpTime(SGHeroLayer *heroLayer)
{
    heroLayer->beginRoundTime();
    heroLayer->battleMap->m_touchState = eTouchNull;
}


