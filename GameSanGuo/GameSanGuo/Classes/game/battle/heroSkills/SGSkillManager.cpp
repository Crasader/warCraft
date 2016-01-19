//
//  SGSkillManager.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#include "SGSkillManager.h"
#include "SGBattleManager.h"
#include "SGAttackList.h"
#include "SGBattleMap.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "FightBaseModule.pb.h"
#include "SGPlayerInfo.h"
#include "SGNormalAttackList.h"
#include "SGTwoAttackList.h"
#include "SGAIManager.h"
#include "Utils.h"
#include "SGStringConfig.h"


static SGSkillManager *mananger = NULL;
#define AISTARTGAP 2.4
SGSkillManager::SGSkillManager()
:array(NULL)
,heroMe(NULL)
,heroEnemy(NULL)
,valueNum(0.0)
,roleidNum(0)
,heroLayer(NULL)
,heroLayer1(NULL)
,skillId(0)
,isBeginAi(true)
,m_HeroSkills(NULL)
,isRemoveSkill(true)
,endData(NULL)
,protoValue1 (0)
,protoValue2 (0)
,protoValue3 (0)
,protoRound (0)
,protoBuffid (0)
,protoRoleid (0)
,m_operateNum (0)
,num (0)
,jianNum (0)

{
    array = CCArray::create();
    array->retain();
}

SGSkillManager *SGSkillManager::shareSkill()
{
    if (!mananger)
    {
        mananger = new SGSkillManager();
    }
    return mananger;
}

void SGSkillManager::initNotification()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_BATTLE_MAJORSKILL, this, callfuncO_selector(SGSkillManager::mainSkillListener));
}

void SGSkillManager::sendSkillRequest(int operateNum)
{
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound());
    if (hero->m_skillsId <= 0) {
        hero->battleMap->m_touchState = eTouchNull;
        return;
    }
    
    if (SGAIManager::shareManager()->isPVE && !SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        SGHeroLayer *heroLayer = SGBattleManager::sharedBattle()->getHeroLayer(false);
        
        if (heroLayer->m_skillsId == 20004 || heroLayer->m_skillsId == 20075)
        {
            SGHeroLayer *opponentHeroLayer = heroLayer->getOpponentHeroLayer();
            if (opponentHeroLayer->getItemId() == 1008 || opponentHeroLayer->getItemId() == 1117)
            {
                operateNum = 1;
            }
        }
        else if (heroLayer->m_skillsId == 20064)
        {
            if (heroLayer->clientType != 2 && heroLayer->clientType != 3) {
                heroLayer->setBulletSp(5);
            }
            
        }
        else if (heroLayer->m_skillsId == skill_id_20001
                 || heroLayer->m_skillsId == skill_id_20072
                 || heroLayer->m_skillsId == skill_id_20095
                 || heroLayer->m_skillsId == skill_id_20040
                 || heroLayer->m_skillsId == skill_id_20041
                 || heroLayer->m_skillsId == skill_id_20042)
        {
            if (heroLayer->clientType != 2 && heroLayer->clientType != 3) {
                operateNum = 4;
                heroLayer->setBulletSp(7);
            }
        }
    }
    
    //CCLog("SGSkillManager::sendSkillRequest");
    hero->battleMap->m_touchState = eTouchSkillsFang;
    if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound()) {
        hero->endRoundUpdataTime();
    }
    
    CCString *op = CCString::createWithFormat("%d", operateNum);
    op->retain();
    m_operateNum = operateNum;
    if ( !SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
    {
        CCDelayTime *time = CCDelayTime::create(0);    //cgp mainSkill
        CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::beginMainSkill), (void*)op);
        hero->runAction(CCSequence::create(time, call, NULL));
    }
    //防止在自动战斗快速切换的时候，千万重复播放
    if (!SGAIManager::shareManager()->getIsDispSkillHead())
    {
        //GPCCLOG("主将技头像");
        //hero->showAttactHeadEffect(NULL);
    }
}


//开始触发武将的技能
void SGSkillManager::beginMainSkill(cocos2d::CCObject *obj, cocos2d::CCString *op)
{
    CCString *s = (CCString*)op;
    
    CCLOG("operate->intValue()==%d",s->intValue());
    
    if(SGBattleManager::sharedBattle()->getIsBattlePve())
    {
        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound());
        skillId = hero->m_skillsId;
        pushHskillData(hero->m_skillsId);
        if(setHeroSkillData())
        {
            CCCallFunc* delayCall = CCCallFunc::create(this, callfunc_selector(SGSkillManager::activeSkillById));
            hero->runAction(CCSequence::create(CCDelayTime::create(0.1), delayCall, NULL));
            hero->initSkillRoundNum();
        }
    }
    else
    {
        main::MajorSkillRequest *request = new main::MajorSkillRequest();
        request->set_value1(s->intValue());
        request->set_value2(0);
        //    request->set_test(20020);
        SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_MAJORSKILL, request);
        s->release();
    }

}

int SGSkillManager::getAttackAp(SGBattleMap *map, SGGridBase *grid)
{
    SGSoldier *soldier = NULL;
    
    int rand = 0;
    switch (grid->getStype()) {
        case knormal:
        case kdefend:
        {
            soldier = map->getShiBingWithGrid(grid);
            if (soldier)
            {
                rand = soldier->getDef();
            }
        }
            break;
        case kattack:
        {
            soldier = map->getAttackListByGrid(grid);
            if(soldier)
            {
                SGAttackList * soldierattack = (SGAttackList *)soldier;
                if (soldierattack)
                {
                  rand = soldier->getAp();
                }
            }
        }
            break;
            
        default:
            break;
    }
    
    return rand;
}
int SGSkillManager::attackGrid(SGBattleMap *map, SGGridBase *grid, int num)
{
    SGSoldier *soldier = NULL;
    
    int rand = 0;
    
    switch (grid->getStype()) {
        case knormal:
        {
            soldier = map->getShiBingWithGrid(grid);
            if (soldier) {
                rand = num - soldier->getDef();
            }
        }
            break;
        case kdefend:
        {
            soldier = map->getShiBingWithGrid(grid);
            if (soldier) {
                rand = num - soldier->getDef();
            }
        }
            break;
        case kattack:
        {
            soldier = map->getAttackListByGrid(grid);
            SGAttackList * soldierattack = (SGAttackList *)soldier;
            if (soldierattack) {
                if (!soldierattack->getIsDeath()) {
                    if (soldierattack->getSkillsDelegate())
                    {
                        //放被动技能
                        num =  num - num *  soldierattack->getAfftributeNum();
                        rand = num - soldierattack->getAp() ;
                    }
                    else
                    {
                        rand = num - soldier->getAp();
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

bool SGSkillManager::setHeroSkillData()
{
    protoValue1 = 0;
    protoValue2 = 0;
    protoValue3 = 0;
    protoRound  = 0;
    protoBuffid  = 0;
    coordPointList.clear();

    
    bool isMyRound = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    
    
    if (isMyRound)
    {
        protoRoleid = SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId();
        heroLayer = SGBattleManager::sharedBattle()->getHeroLayer(true);
        heroLayer1 = SGBattleManager::sharedBattle()->getHeroLayer(false);
        CCLOG("----------------------> 自己用了主将计");
    }
    else
    {
        protoRoleid = 0;
        CCLOG("----------------------> 不是自己用了主将计");
        heroLayer = SGBattleManager::sharedBattle()->getHeroLayer(false);
        heroLayer1 = SGBattleManager::sharedBattle()->getHeroLayer(true);
    }

    SGSkillDataModel*  heroSkillData = SGStaticDataManager::shareStatic()->getLordSkillById(heroLayer->lordSkillId);
    if (!heroSkillData) {
        return false;
    }
    switch (skillId)
    {
        case skill_id_20000:
        {
            protoValue1 = heroSkillData->getValue1();
            protoValue2 = heroSkillData->getValue2();
            protoRound = heroSkillData->getEffectRound();
            protoBuffid =  0; //没使用
        }
            break;
        case skill_id_20001:
        case skill_id_20072://武将计刀波（绿色）
        case skill_id_20095:
        case skill_id_20040:
        case skill_id_20041:
        case skill_id_20042://发射弩箭
        {
            protoValue1 = heroSkillData->getValue1();
            protoValue2 = m_operateNum;
        }
            break;
        case skill_id_20002:
        case skill_id_20073:
        {

        }
        break;
            
        case skill_id_20003:
        case skill_id_20056:
        case skill_id_20057:
        case skill_id_20074:
        {
            protoValue1 = heroSkillData->getValue1();
            protoBuffid = heroSkillData->getBuff();
        }
            break;
            
        case skill_id_20004:
        case skill_id_20075:
        {
            protoValue1 = heroSkillData->getValue1();
            protoValue2 = heroSkillData->getValue2();
        }
            break;
            
        case skill_id_20005:
        case skill_id_20076:
        case skill_id_20016:
        case skill_id_20017:
        case skill_id_20081:
        {
            cc_timeval psv;
            CCTime::gettimeofdayCocos2d(&psv, NULL);
            unsigned long int rand_seed = psv.tv_sec*1000 + psv.tv_usec/1000;
            srand(rand_seed);
            
            int shiBingCount = heroLayer1->getBattleMap()->getShiBingCount();
            int index = 0;
            ShiBing * shibing = NULL;
            
            for (int i = 0; i<shiBingCount; i++) {
                shibing = heroLayer1->getBattleMap()->getShiBingByIndex(i);
                if (shibing && shibing->getOnlyGrid()->getStype() == knormal) {
                    coordPointList.push_back(ccp(shibing->getGridIndex().i, shibing->getGridIndex().j));
                }
            }
            while (coordPointList.size() > 5) {
                index = (int)(CCRANDOM_0_1()*(coordPointList.size()-1));//产生一个随机数
                coordPointList.erase(coordPointList.begin()+index);
            }
            
        }
            break;
        case skill_id_20006:
        case skill_id_20077://落雷
        case skill_id_20026:
        case skill_id_20027:
        case skill_id_20086:
        case skill_id_20096://陨石
        case skill_id_20036:
        case skill_id_20037:
        case skill_id_20091://落雷
        {
            protoValue1 = heroSkillData->getValue2();
            int listSize = heroSkillData->getValue1();
            int shiBingCount = heroLayer1->getBattleMap()->getShiBingCount();
            int index = 0;
            ShiBing * shibing = NULL;
            
            for (int i = 0; i< shiBingCount; i++) {
                shibing = heroLayer1->getBattleMap()->getShiBingByIndex(i);
                //if (shibing && shibing->getOnlyGrid()->getStype() == knormal)
                {
                    coordPointList.push_back(ccp(shibing->getGridIndex().j,shibing->getGridIndex().i));
                }
            }
            
            cc_timeval psv;
            CCTime::gettimeofdayCocos2d(&psv, NULL);
            unsigned long int rand_seed = psv.tv_sec*1000 + psv.tv_usec/1000;
            srand(rand_seed);
            while (coordPointList.size() > listSize) {
                index = (int)(CCRANDOM_0_1()*(coordPointList.size()-1));//产生一个随机数
                coordPointList.erase(coordPointList.begin()+index);
            }
        }
            break;
        case skill_id_20007:// 空城计
        case skill_id_20078:
        {
            protoValue1 = heroSkillData->getValue1();
            protoRound = heroSkillData->getEffectRound();
            protoBuffid = heroSkillData->getBuff();
        }
            break;
        case skill_id_20008:
        case skill_id_20009:
        case skill_id_20079:
        case skill_id_20094:
        case skill_id_20010:
        case skill_id_20011:
        {
            protoValue1 = heroSkillData->getValue1();
        }
            break;
        case skill_id_20012:
        case skill_id_20013:
        {
            protoValue1 = heroSkillData->getValue1();
            
            
            CCArray * enemyattackList = heroLayer1->getBattleMap()->getAttackList();
            CCArray * myattackList = heroLayer->getBattleMap()->getAttackList();
            SGAttackList *attack = NULL;
            for (int i = 0; i< enemyattackList->count(); i++)
            {
                attack = (SGAttackList *)enemyattackList->objectAtIndex(i);
                
                coordPointList.push_back(ccp(attack->getAttackIndex().i, attack->getAttackIndex().j));
            }
            if (enemyattackList->count() < myattackList->count()) {
                int num =  myattackList->count() - enemyattackList->count();
                for (int i = 0; i < num; i++)
                {
                    attack = (SGAttackList *)enemyattackList->objectAtIndex(i);
                    
                    coordPointList.push_back(ccp(-1, -1));
                }
            }
            for (int i = 0; i< myattackList->count(); i++)
            {
                attack = (SGAttackList *)myattackList->objectAtIndex(i);
                
                coordPointList.push_back(ccp(attack->getAttackIndex().i, attack->getAttackIndex().j));
            }
            if (myattackList->count() < enemyattackList->count() ) {
                int num =  enemyattackList->count() - myattackList->count();
                for (int i = 0; i < num; i++)
                {
                    attack = (SGAttackList *)myattackList->objectAtIndex(i);
                    coordPointList.push_back(ccp(-1, -1));
                }
            }
        }
            break;
        case skill_id_20014://破釜沉舟
        case skill_id_20015:
        case skill_id_20080:
        case skill_id_20028:
        case skill_id_20029:
        case skill_id_20087:
        {
            protoValue1 = heroSkillData->getValue1();
            protoValue2 = heroSkillData->getValue2();
            protoRound =  heroSkillData->getEffectRound();
            protoBuffid = heroSkillData->getBuff();
        }
            break;
        case skill_id_20018:
        case skill_id_20019:
        case skill_id_20082:
        case skill_id_20097://火烧连营
        {
            protoValue2 = heroSkillData->getValue2();
            int attackDefNum = heroSkillData->getValue1();
            
            SGBattleMap * enemyBattle = heroLayer1->getBattleMap();
            std::vector<SGGridBase *> defGridList;
            for (int i = 0; i < mapRow; i++) {
                for (int j = 0; j < mapList; j++) {
                    SGGridBase *grid = enemyBattle->myGrids[i][j];
                    if (grid && grid->getStype() == kdefend)
                    {
                        defGridList.push_back(grid);
                    }
                }
            }

            cc_timeval psv;
            CCTime::gettimeofdayCocos2d(&psv, NULL);
            unsigned long int rand_seed = psv.tv_sec*1000 + psv.tv_usec/1000;
            srand(rand_seed);
            
            int index = 0;
            int damage = 0;
            int row = 0;
            int list = 0;
            while (defGridList.size() > attackDefNum) {
                index = (int)(CCRANDOM_0_1()*(defGridList.size()-1));//产生一个随机数
                defGridList.erase(defGridList.begin()+index);
            }

            if (endData)
            {
                endData->removeAllObjects();
            }
            else
            {
                endData = CCArray::create();
                endData->retain();
            }
            CCArray *attackedShiBingList = CCArray::create();
            CCArray *deadShiBingList = CCArray::create();
            if (defGridList.size() > 0)
            {
                CCArray *defendShiBingList = CCArray::create();
                bool canAttack = true;
                for (int i = 0; i < defGridList.size(); i++)
                {
                    defendShiBingList->addObject(enemyBattle->getShiBingWithIndex(gameIndex(defGridList[i]->getIndex().i, defGridList[i]->getIndex().j)));
                }
                
                for (int i = 0; i<defGridList.size(); i++) {
                    coordPointList.push_back(ccp( defGridList[i]->getIndex().j,defGridList[i]->getIndex().i));
                    damage = heroSkillData->getValue2();
                    {
                        row = defGridList[i]->getIndex().i + 1;
                        list = defGridList[i]->getIndex().j - 1;
                        attackedShiBingList->removeAllObjects();

                        CCObject * obj = NULL;
                        ShiBing * shibing = NULL;
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 2; j>=0 ; j--)
                            {
                                canAttack = true;
                                if (damage >0)
                                {
                                    shibing = enemyBattle->getShiBingWithIndex(gameIndex(row - i, list +j));
                                    if (shibing)
                                    {
                                        CCARRAY_FOREACH(attackedShiBingList, obj)
                                        {
                                            if (obj == shibing) {
                                                canAttack = false;
                                            }
                                        }
                                        CCARRAY_FOREACH(defendShiBingList, obj)
                                        {
                                            if (obj == shibing) {
                                                canAttack = false;
                                            }
                                        }
                                        CCARRAY_FOREACH(deadShiBingList, obj)
                                        {
                                            if (obj == shibing) {
                                                canAttack = false;
                                            }
                                        }
                                        if (canAttack) {
                                            
                                            int oldDamage = damage;
                                            damage = attackGrid(enemyBattle,enemyBattle->myGrids[row - i][list +j],damage);
                                            
                                            EndObjData *objD = new EndObjData();
                                            objD->autorelease();
                                            objD->x = list +j;
                                            objD->y = row - i;
                                            
                                            
                                            if (damage > 0
                                                || (damage <= 0 && enemyBattle->myGrids[row - i][list +j]->getStype() == knormal))
                                            {
                                                deadShiBingList->addObject(shibing);
                                            }
                                            else
                                            {
                                                attackedShiBingList->addObject(shibing);
                                            }
                                            
                                            if (damage > 0)
                                            {
                                                objD->num = 0;
                                            }
                                            else
                                            {
                                                objD->num = getAttackAp(enemyBattle,enemyBattle->myGrids[row - i][list +j]) - oldDamage;
                                                objD->num = objD->num < 0 ? 0 : objD->num;
                                            }
                                            endData->addObject(objD);
                                        }
                                    }
                                }
                            }
                        }
                        
                    }
                }
            }
        
//            int dataCound = proto.coordinateeffectproto_size();
//            //能打到的人
//            if (dataCound > 0)
//            {
//                for (int i = 0; i < dataCound; i++) {
//                    main::CoordinateEffectProto coord = proto.coordinateeffectproto(i);
//                    EndObjData *objD = new EndObjData();
//                    objD->x = coord.x();
//                    objD->y = coord.y();
//                    objD->num = coord.hp();
//                    endData->addObject(objD);
//                }
//            }
        
        }
            break;
            
        case skill_id_20020:
        case skill_id_20021:
        case skill_id_20083:
        //同下
        case skill_id_20022:
        case skill_id_20023:
        case skill_id_20084:
        case skill_id_20098:
        //同下
        case skill_id_20024:
        case skill_id_20025:
        case skill_id_20085:
        {
            protoValue1 = heroSkillData->getValue1();
        }
            break;
        case skill_id_20030:
        case skill_id_20031:
        case skill_id_20088:
        {
        
        }
            break;
        case skill_id_20032:
        case skill_id_20071:
        case skill_id_20093:
        //同下
        case skill_id_20033:
        case skill_id_20089:
        case skill_id_20054:
        case skill_id_20055:
        {
            protoValue1 = heroSkillData->getValue1();
        }
            break;
        case skill_id_20034:
        case skill_id_20035:
        case skill_id_20090:
        {
            protoRound = heroSkillData->getEffectRound();
            protoBuffid = heroSkillData->getBuff();
        }
            break;
            //aaaaaaaaaaaaa
        case skill_id_20038://霹雳火阵
        case skill_id_20039:
        case skill_id_20092:
        case skill_id_20099:
        case skill_id_20043:
        case skill_id_20044:
        case skill_id_20045:
        //同下
        case skill_id_20046:
        case skill_id_20047:
//同下
        case skill_id_20048:
        case skill_id_20049:
        case skill_id_20050:
        case skill_id_20051:
        {
            protoValue1 = heroSkillData->getValue1();
        }
          break;
        case skill_id_20052:
        case skill_id_20053:
        {
            protoValue1 = heroSkillData->getValue1();
            protoBuffid = heroSkillData->getBuff();
        }
            break;
        case skill_id_20058:
        case skill_id_20059:
        case skill_id_20069:
        {
            protoValue1 = heroSkillData->getValue1();
        }
            break;
            
        case skill_id_20060:
        case skill_id_20061:
        {
        }
            break;
        case skill_id_20062:
        case skill_id_20063:
        case skill_id_20100://绝·幽冥盾
        {
            protoValue1 = heroSkillData->getValue1();
            protoValue2 = heroSkillData->getValue2();
            protoRound = heroSkillData->getEffectRound();
            protoBuffid = heroSkillData->getBuff();
            protoValue3 = 0;
        }
            break;
        case skill_id_20064:
        case skill_id_20065:
        {
            protoValue1 = heroSkillData->getValue1() * (heroLayer->getSanCount() > 0 ? heroLayer->getSanCount() : 1);
            protoValue2 = m_operateNum;
        }
            break;
        case skill_id_20066:
        case skill_id_20067:
        case skill_id_20068:
        {
            protoValue1 = heroSkillData->getValue1();
        }
            break;
        default:
        {
            return false;
        }
        break;
    }
    return true;
}
void SGSkillManager::pushHskillData(int skid)
{
    CCString *str = CCString::createWithFormat("Hskill: %d", skid);
    SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
}

//根据服务器发送来的状态,发动主将计,带有技能特效和后续回合操作
void SGSkillManager::mainSkillListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        if (NULL != getHeroSkills())
            CC_SAFE_RELEASE(getHeroSkills());
        isRemoveSkill = true;
        isBeginAi = true;
        main::MajorSkillResponse *response = (main::MajorSkillResponse *)sr->m_msg;
        main::MajorSkillProto proto = response->majorskillproto();

        if (proto.skillid() <= 0)
        {

        }
        heroLayer = NULL;
        heroLayer1 = NULL;

        CCLOG("id:%d",proto.roleid());
        CCLOG("myid:%d",SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId());
        
        if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() == proto.roleid())
        {
            heroLayer = SGBattleManager::sharedBattle()->getHeroLayer(true);
            heroLayer1 = SGBattleManager::sharedBattle()->getHeroLayer(false);
            GPCCLOG("----------------------> 自己用了主将计");
        }
        else
        {
            GPCCLOG("----------------------> 不是自己用了主将计");
            heroLayer = SGBattleManager::sharedBattle()->getHeroLayer(false);
            heroLayer1 = SGBattleManager::sharedBattle()->getHeroLayer(true);
            if (!SGAIManager::shareManager()->isPVE)
            heroLayer->showAttactHeadEffect(NULL);
        }
        
        int a = proto.state();
        if (a == 0) {
            heroLayer->initSkillRoundNum();
        }
        else{
            //在这里改变游戏的状态
            heroLayer->battleMap->m_touchState = eTouchNull;
            return;
        }
        skillId = 0;
        
        CCLOG("state:%d",proto.state());
        CCLOG("skillId:%d",proto.skillid());
        skillId = proto.skillid();
        pushHskillData(skillId);
        
        protoValue1 = proto.value1();
        protoValue2 = proto.value2();
        protoValue3 = proto.value3();
        protoRound  = proto.round();
        protoBuffid  = proto.buffid();
        protoRoleid  = proto.roleid();
        
        int len = proto.coordinateprotolist_size();
        coordPointList.clear();
        if (len > 0) {
            CCPoint point = CCPointZero;
            
            for (int i = 0; i<len; i++)
            {
                main::CoordinateProto coord = proto.coordinateprotolist(i);
                point = ccp(coord.x(), coord.y());
                coordPointList.push_back(point);
            }
        }
        
        int dataCound = proto.coordinateeffectproto_size();
        //能打到的人
        if (dataCound > 0)
        {
            if (endData)
            {
                endData->removeAllObjects();
            }
            else
            {
                endData = CCArray::create();
                endData->retain();
            }
            for (int i = 0; i < dataCound; i++) {
                main::CoordinateEffectProto coord = proto.coordinateeffectproto(i);
                EndObjData *objD = new EndObjData();
                objD->autorelease();
                objD->x = coord.x();
                objD->y = coord.y();
                objD->num = coord.hp();
                endData->addObject(objD);
            }
        }
        
        activeSkillById();
    }
}

void SGSkillManager::activeSkillById()
{
    SGAIManager::shareManager()->setIsDispSkillHead(false);
    switch (skillId)
    {
    case skill_id_20000:
        {
            SGHeroSkill1 *skill = new SGHeroSkill1();
            setHeroSkills(skill);
            skill->activateSkill_20000(heroLayer, protoValue1, protoValue2, protoRound, protoBuffid);
            //                this->activateSkill_20000(heroLayer, protoValue1, protoValue2, protoRound, protoBuffid);
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
        break;
    case skill_id_20001:
    case skill_id_20072://武将计刀波（绿色）
    case skill_id_20095:
    case skill_id_20040:
    case skill_id_20041:
    case skill_id_20042://发射弩箭
        {
            isBeginAi = false;
            //                this->activateSkill_20001(heroLayer, heroLayer1, protoValue1, protoValue2, protoRoleid);
            
            SGHeroSkill2 *skill = new SGHeroSkill2();
            setHeroSkills(skill);
            float delayTime = skill->activateSkill_20001(heroLayer, heroLayer1, protoValue1, protoValue2, protoRoleid);
            delayTime = delayTime > 2 ? delayTime : 2;
            CCDelayTime *delay = CCDelayTime::create(delayTime - 0.5);
            CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGSkillManager::bbb));
            heroLayer->runAction(CCSequence::create( delay, call, NULL));
        }
        break;
    case skill_id_20002:
    case skill_id_20073:
        {
            SGHeroSkill3 *skill = new SGHeroSkill3();
            setHeroSkills(skill);
            skill->activateSkill_20002(heroLayer1);
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
        break;
        
    case skill_id_20003:
    case skill_id_20056:
    case skill_id_20057:
    case skill_id_20074:
        {
            SGHeroSkill4 *skill = new SGHeroSkill4();
            setHeroSkills(skill);
            skill->activateSkill_20003(heroLayer1, protoValue1,protoBuffid,protoRoleid);
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            
            // this->beginAi();
            this->beginUpTime();
        }
        break;
        
    case skill_id_20004:
    case skill_id_20075:
        {
            SGHeroSkill5 *skill = new SGHeroSkill5();
            setHeroSkills(skill);
            skill->activateSkill_20004(heroLayer, heroLayer1, protoValue1, protoValue2, protoRoleid);
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
        break;
        
    case skill_id_20005:
    case skill_id_20076:
    case skill_id_20016:
    case skill_id_20017:
    case skill_id_20081:
        {
            SGHeroSkill6 *skill = new SGHeroSkill6();
            setHeroSkills(skill);
            int len = coordPointList.size();
            for (int i = 0; i<len; i++)
            {
                skill->activateSkill_20005(heroLayer, heroLayer1, coordPointList[i], protoRoleid, skillId);
            }
            
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            //  this->beginAi();
            this->beginUpTime();
            
        }
        break;
        
    case skill_id_20006:
    case skill_id_20077://落雷
    case skill_id_20026:
    case skill_id_20027:
    case skill_id_20086:
    case skill_id_20096://陨石
    case skill_id_20036:
    case skill_id_20037:
    case skill_id_20091://落雷
        {
            SGHeroSkill7 *skill = new SGHeroSkill7();
            setHeroSkills(skill);
            
            int len = coordPointList.size();
            for (int i = 0; i<len; i++)
            {
                skill->activateSkill_20006(heroLayer, heroLayer1, protoValue1, coordPointList[i], skillId);
            }
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
            
        }
        break;
    case skill_id_20007:// 空城计
    case skill_id_20078:
        {
            
            SGHeroSkill8 *skill = new SGHeroSkill8();
            setHeroSkills(skill);
            skill->activateSkill_20007(heroLayer, protoValue1, protoRound, protoBuffid);
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
        break;
    case skill_id_20008:
    case skill_id_20009:
    case skill_id_20079:
    case skill_id_20094:
        {
            SGHeroSkill9 *skill = new SGHeroSkill9();
            setHeroSkills(skill);
            skill->activateSkill_20008(heroLayer, protoValue1);
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
        break;
    case skill_id_20010:
    case skill_id_20011:
        {
            SGHeroSkill10 *skill = new SGHeroSkill10();
            setHeroSkills(skill);
            skill->activateSkill_20010(heroLayer, protoValue1);
            
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
        break;
    case skill_id_20012:
    case skill_id_20013:
        {
            SGHeroSkill11 *skill = new SGHeroSkill11();
            setHeroSkills(skill);
            
            int len = coordPointList.size();
            int a = len / 2;
            for (int i = 0; i<len; i++)
            {
                CCPoint point = coordPointList[i];
                if (point.x != -1 &&
                    point.y != -1) {
                    if (i < a) {
                        skill->activateSkill_20012(heroLayer1, NULL, protoValue1, skillId, point);
                    }else{
                        skill->activateSkill_20012(heroLayer, NULL, protoValue1, skillId, point);
                    }
                }
            }
            heroLayer->battleMap->refreshMapGrid(true);
        }
        break;
        
    case skill_id_20014://破釜沉舟
    case skill_id_20015:
    case skill_id_20080:
    case skill_id_20028:
    case skill_id_20029:
    case skill_id_20087:
        {
            SGHeroSkill12 *skill = new SGHeroSkill12();
            setHeroSkills(skill);
            skill->activateSkill_20014(heroLayer, protoValue1, protoValue2, protoRound, protoBuffid);
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            
            //this->beginAi();
            this->beginUpTime();
        }
        break;
    case skill_id_20018:
    case skill_id_20019:
    case skill_id_20082:
    case skill_id_20097://火烧连营
        {
            SGHeroSkill13 *skill = new SGHeroSkill13();
            setHeroSkills(skill);
            int len = coordPointList.size();
            /*本地*/
            //                CCArray *indexArray = CCArray::create();
            //                for (int i = 0; i < len; i++) {
            //                    main::CoordinateProto coord = proto.coordinateprotolist(i);
            //                    GameIndex index = gameIndex(coord.y(), coord.x());
            //                    CCLOG("行==%d    列===%d",index.i, index.j);
            //                    SGGridBase* grid = heroLayer1->battleMap->myGrids[index.i][index.j];
            //                    indexArray->addObject(grid);
            //                }
            //
            //                for (int i = 0; i<len; i++)
            //                {
            //                    main::CoordinateProto coord = proto.coordinateprotolist(i);
            //                    GameIndex index = gameIndex(coord.y(), coord.x());
            //                    skill->activateSkill_20018(heroLayer1, heroLayer, index, indexArray, protoValue2, i);
            //                }
            
            
            /*服务器*/
//            CCArray *endData = CCArray::create();
//            endData->retain();
//            int dataCound = proto.coordinateeffectproto_size();
//            //能打到的人
//            for (int i = 0; i < dataCound; i++) {
//                main::CoordinateEffectProto coord = proto.coordinateeffectproto(i);
//                EndObjData *objD = new EndObjData();
//                objD->x = coord.x();
//                objD->y = coord.y();
//                objD->num = coord.hp();
//                endData->addObject(objD);
//            }
            //墙
            for (int i = 0; i < len; i++) {
                CCLog("Coord.y=%d,coord.x=%d",(int)coordPointList[i].x, (int)coordPointList[i].y);
                GameIndex index = gameIndex((int)coordPointList[i].y,(int)coordPointList[i].x);
                skill->activateSkill_20018(heroLayer1, heroLayer, index, endData, protoValue2, i, (i == (len - 1)));
            }
            
            this->beginUpTime();
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
        }
        break;
        
    case skill_id_20020:
    case skill_id_20021:
    case skill_id_20083:
        {
            SGHeroSkill14 *skill = new SGHeroSkill14();
            setHeroSkills(skill);
            skill->activateSkill_20020(heroLayer1, protoValue1);
            // this->beginAi();
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            this->beginUpTime();
        }
        break;
    case skill_id_20022:
    case skill_id_20023:
    case skill_id_20084:
    case skill_id_20098:
        {
            isBeginAi = false;
            SGHeroSkill15 *skill = new SGHeroSkill15();
            setHeroSkills(skill);
            float delayTime = skill->activateSkill_20022(heroLayer, heroLayer1, protoValue1, protoRoleid);
            delayTime = delayTime > 2 ? delayTime : 2;
            CCDelayTime *delay = CCDelayTime::create(delayTime);
            CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGSkillManager::bbb));
            heroLayer->runAction(CCSequence::create( delay, call, NULL));
            //                this->beginAi();
        }
        break;
    case skill_id_20024:
    case skill_id_20025:
    case skill_id_20085:
        {
            SGHeroSkill16 *skill = new SGHeroSkill16();
            setHeroSkills(skill);
            skill->activateSkill_20024(heroLayer, protoValue1);
            
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
        break;
        //            case skill_id_20028:  // 原苦肉计 降低总血量25%，增加所有当前攻击阵列攻击力25%
        //            case skill_id_20029:
        //            case skill_id_20087:
        //            {
        //                SGHeroSkill17 *skill = new SGHeroSkill17();
        //                setHeroSkills(skill);
        //                skill->activateSkill_20028(heroLayer, protoValue1 , protoValue2);
        //
        //                CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
        //                CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
        //                heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
        //                //  this->beginAi();
        //                this->beginUpTime();
        //            }
        //                break;
    case skill_id_20030:
    case skill_id_20031:
    case skill_id_20088:
        {
            SGHeroSkill18 *skill = new SGHeroSkill18();
            setHeroSkills(skill);
            skill->activateSkill_20030(heroLayer1, skillId);
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
        break;
    case skill_id_20032:
    case skill_id_20071:
    case skill_id_20093:
        {
            SGHeroSkillsBase *skill = new SGHeroSkillsBase();
            setHeroSkills(skill);
            skill->activateSkill_20032(heroLayer, protoValue1);
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            
            // this->beginAi();
            this->beginUpTime();
        }
        break;
    case skill_id_20033:
    case skill_id_20089:
    case skill_id_20054:
    case skill_id_20055:
        {
            SGHeroSkill19 *skill = new SGHeroSkill19();
            setHeroSkills(skill);
            skill->activateSkill_20033(heroLayer, protoValue1);
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            //this->beginAi();
            this->beginUpTime();
        }
        break;
    case skill_id_20034:
    case skill_id_20035:
    case skill_id_20090:
        {
            SGHeroSkill20 *skill = new SGHeroSkill20();
            setHeroSkills(skill);
            skill->activateSkill_20034(heroLayer, protoRound, protoBuffid);
            
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
        break;
        //aaaaaaaaaaaaa
    case skill_id_20038://霹雳火阵
    case skill_id_20039:
    case skill_id_20092:
    case skill_id_20099:
    case skill_id_20043:
    case skill_id_20044:
    case skill_id_20045:
        {
            //                isRemoveSkill = false;
            isBeginAi = false;
            for (int j = 0; j < mapList; j++) {
                for (int i = 0; i < mapRow; i++) {
                    SGGridBase *grid = heroLayer->battleMap->myGrids[i][j];
                    if (grid) {
                        if (grid->getStype() == kdefend) {
                            ShiBing* sb = heroLayer->battleMap->getShiBingWithGrid(grid);
                            if(sb)
                                array->addObject(sb);
                        }
                    }
                }
            }
            CCLOG("array==%d", array->count());
            num = 0;
            
            heroMe = heroLayer;
            heroEnemy = heroLayer1;
            valueNum = protoValue1;
            roleidNum = protoRoleid;
            jianNum = heroLayer->m_jianId;
            
            SGHeroSkill21 *skill = new SGHeroSkill21();
            setHeroSkills(skill);
            
            skill->activateSkill_20038(heroLayer, heroLayer1, protoValue1, protoRoleid, heroLayer->m_jianId);
            //为自动战斗服务，延时是为了主将技完全放完再开始自动战斗AI
            CCDelayTime *delay = CCDelayTime::create(skill->seconds * 1.2);
            CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGSkillManager::setIsCompleteSkill));
            heroLayer->runAction(CCSequence::create( delay, call, NULL));
        }
        break;
    case skill_id_20046:
    case skill_id_20047:
        {
            SGHeroSkill22 *skill = new SGHeroSkill22();
            setHeroSkills(skill);
            skill->activateSkill_20046(heroLayer, protoValue1);
            
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
        break;
    case skill_id_20048:
    case skill_id_20049:
    case skill_id_20050:
    case skill_id_20051:
        {
            SGHeroSkill23 *skill = new SGHeroSkill23();
            setHeroSkills(skill);
            skill->activateSkill_20048(heroLayer1, protoValue1);
            
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
    case skill_id_20052:
    case skill_id_20053:
        {
            SGHeroSkill24 *skill = new SGHeroSkill24();
            setHeroSkills(skill);
            skill->activateSkill_20052(heroLayer, protoValue1,protoBuffid);
            
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            //  this->beginAi();
            this->beginUpTime();
        }
        break;
    case skill_id_20058:
    case skill_id_20059:
    case skill_id_20069:
        {
            SGHeroSkill25 *skill = new SGHeroSkill25();
            setHeroSkills(skill);
            skill->activateSkill_20058(heroLayer, protoValue1);
        }
        break;
        
    case skill_id_20060:
    case skill_id_20061:
        {
            SGHeroSkill26 *skill = new SGHeroSkill26();
            setHeroSkills(skill);
            skill->activateSkill_20060(heroLayer);
        }
        break;
    case skill_id_20062://幽冥盾
    case skill_id_20063:
    case skill_id_20100://虚空盾
        {
            SGHeroSkill27 *skill = new SGHeroSkill27();
            setHeroSkills(skill);
            skill->activateSkill_20063(heroLayer, heroLayer1, protoRoleid, protoBuffid, protoRound, protoValue1, protoValue2,protoValue3);
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            //this->beginAi();
            this->beginUpTime();
        }
        break;
    case skill_id_20064:
    case skill_id_20065:
        {
            SGHeroSkill28 *skill = new SGHeroSkill28();
            setHeroSkills(skill);
            skill->activateSkill_20065(heroLayer, heroLayer1, protoValue1, protoValue2, protoRoleid);
        }
        break;
    case skill_id_20066:
    case skill_id_20067:
    case skill_id_20068:
        {
            SGHeroSkill29 *skill = new SGHeroSkill29();
            setHeroSkills(skill);
            skill->activateSkill_20066(heroLayer, protoValue1);
            
            CCDelayTime *delay=CCDelayTime::create(AISTARTGAP);
            CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGSkillManager::beginAi));
            heroLayer->runAction(CCSequence::create(delay,fun1,NULL));
            // this->beginAi();
            this->beginUpTime();
        }
        break;
    default:
        break;
    }
}

void SGSkillManager::activateSkill_20000(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid)
{
    beginUpTime();
    CCObject *attack = NULL;
    CCARRAY_FOREACH(heroLayer->battleMap->getAttackList(), attack)
    {
        SGAttackList *attackObj = (SGAttackList*)attack;
        attackObj->attackAddAp(value1);
        
    }
    
    CCObject* objsb = NULL;
    CCARRAY_FOREACH(heroLayer->battleMap->getSoldiers(), objsb)
    {
        ShiBing *fSb = (ShiBing*) objsb;
        if (fSb)
        {
            SGGridBase *gg = (SGGridBase*) fSb->getPrinceGridArray()->objectAtIndex(0);
            if (gg)
            {
                if (gg->getStype() == knormal) {
                    fSb->setDef(fSb->getDef() + fSb->getDef() * value2);
                    fSb->showDef(fSb->getDef());
                    fSb->showEffect(1);
                }
            }
        }
    }
}


/*

*/

//选择三列，发射攻击总额为自身攻击力100%的弩箭。
float SGSkillManager::activateSkill_20001(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid)
{
    int indexJ = value2;
    
    CCLOG("选择的列 %d",indexJ);
    CCLOG("打列%d",abs(mapList-1-indexJ));
    
    float time = 1.0;
    float sbTime = time/12;
    
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
    
    
    for (int j = indexJ - 1; j <= indexJ + 1; j++) {
        int ap = value1;
        
        if (isme) {
            CCString *str = CCString::createWithFormat(str_Format_attack_column, ap, j);
            SGBattleManager::sharedBattle()->fightLogRequest(str->getCString());
        }
         
        
        for (int i = 0; i <= mapRow; i++) {
            if (ap > 0) {
                float b = sbTime * (i+5.5);
                if (i == mapRow) {
                    //播放打到底线动作
//                    removeBullet(heroLayer, (void*)(SKILLS_BULLET_TAG + j - indexJ));
                    GameSbIndex *sbin = new GameSbIndex();
                    sbin->i = mapRow;
                    sbin->j = indexJ;
                    CCDelayTime* delay1 = CCDelayTime::create(b + sbTime);
                    CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGSkillManager::removeBullet),
                                                               (void*)(SKILLS_BULLET_TAG + j - indexJ));
                    CCCallFuncND *call3 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGSkillManager::playBulletBlast),
                                                               (void*)sbin);
                    heroLayer->runAction(CCSequence::create(delay1, call1, call3, NULL));
                    
                    CCDelayTime* delay2 = CCDelayTime::create(b + sbTime);
                    CCCallFuncND* call2 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGSkillManager::changBloodNum),
                                                               (void*)ap);
                    heroLayer1->runAction(CCSequence::create(delay2, call2, NULL));
                }else{
                    SGGridBase *grid = heroLayer1->battleMap->myGrids[i][abs(mapList-1-j)];
                    
                    if (grid) {
                        if (grid->getStype() == knormal) {
                            
                            ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                            if(sb)
                            {
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create(delay, call, NULL));
                                ap = ap - sb->getDef();
                                if (sb->getSbType() == ksbtwo || sb->getSbType() == ksbfour) {
                                    i++;
                                }
                                if (isme) {
                                    CCString *str = CCString::createWithFormat(str_Format_behit_defense_residual, sb->getFileId(), sb->getDef(), ap);
                                    SGBattleManager::sharedBattle()->fightLogRequest(str->getCString());
                                }
                            }
                            
                        }else if(grid->getStype() == kdefend)
                        {
                            ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                            if(sb)
                            {
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create(delay, call, NULL));
                                ap = ap - sb->getDef();
                                sb->setDef(-ap);
                                
                                if (isme) {
                                    CCString *str = CCString::createWithFormat(str_Format_behit_defense_residual, sb->getFileId(), sb->getDef(), ap);
                                    SGBattleManager::sharedBattle()->fightLogRequest(str->getCString());
                                }
                            }
                            
//                            if (ap > sb->getDef()) {
//                                heroLayer1->battleMap->removeAttackedSB(sb, 0);
//                            }else{
//                                sb->setDef(sb->getDef() - ap);
//                                sb->setDefImage();
//                                sb->showDef(sb->getDef());
//                            }
                        }else
                        {
                            SGAttackList* attack = heroLayer1->battleMap->getAttackListByGrid(grid);
                            if(attack)
                            {
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                                sb->runAction(CCSequence::create( delay, call, NULL));
                                ap = ap - attack->getAp();
                                attack->setAp(-ap);
                                
                                if (ap > 0)
                                {
                                    attack->playEffectWithType(kAttackEffect);
                                }
                                
    //                            if (ap >= 0) {
    //                                CCObject* obj = NULL;
    //                                CCARRAY_FOREACH(attack->attackSbs, obj)
    //                                {
    //                                    ShiBing* sb = (ShiBing*)obj;
    //                                    sb->removeSBGrid();
    //                                }
    //                            }

                                if (attack->getAttackType() == kattacknormal) {
                                    i += 2;
                                }else{
                                    i += 1;
                                }
                                if (isme) {
                                    CCString *str = CCString::createWithFormat("被打：%d 防御力： %d, 剩余：%d", sb->getFileId(), attack->getAp(), ap);
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
                                                                       callfuncND_selector(SGSkillManager::removeBullet),
                                                                       (void*)(SKILLS_BULLET_TAG + j - indexJ));
                            
                            CCCallFuncND *call3 = CCCallFuncND::create(this,
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
    }

}

//移除打出去的特效
void SGSkillManager::removeBullet(cocos2d::CCObject *obj, void* tag)
{
    CCLOG("removeBullet");
    
    ((SGHeroLayer*)obj)->removeChildByTag( (long)tag, true);
    if (SGAIManager::shareManager()->isPVE) {
        SGAIManager::shareManager()->isAi = true;
        if (((SGHeroLayer*)obj)->battleMap->_isRefreshMap ||
            ((SGHeroLayer*)obj)->battleMap->m_touchState != eTouchNull)
        {
        }else{
            if (isBeginAi)
                beginAi();
        }
        if (isBeginAi)
            beginUpTime();
    }
    
}
//播放攻击的特效
void SGSkillManager::playBulletBlast(CCObject *obj, GameSbIndex *index)
{
    CCLOG("index.i.%d=", index->i);
    CCLOG("index.j.%d=", index->j);
    SGHeroLayer *hero = (SGHeroLayer *)obj;
    switch (hero->m_skillsId) {
        case skill_id_20001:
        case skill_id_20072://武将计刀波（绿色）
        case skill_id_20095:
        case skill_id_20022:
        case skill_id_20023:
        case skill_id_20084:
        case skill_id_20098:
        {
            hero->showEffectLayer(11, false, gameIndex(-(index->i + 1), index->j));
        }
            break;
        case skill_id_20040:
        case skill_id_20041:
        case skill_id_20042://发射弩箭
        case skill_id_20038:
        case skill_id_20039:
        case skill_id_20092:
        case skill_id_20099:
        case skill_id_20043:
        case skill_id_20044:
        case skill_id_20045:
        {
            hero->showEffectLayer(5, false,  gameIndex(-(index->i + 1), index->j));
        }
            break;
        case skill_id_20062://幽冥盾
        case skill_id_20063:
        case skill_id_20100://虚空盾
        {
            hero->showEffectLayer(13, false,  gameIndex(-index->i - 1, index->j));
        }
            break;
        case skill_id_20064:
        case skill_id_20065:
        {
            //能量球 没有爆炸效果
        }
            break;
            
        default:
            break;
    }
    
    CC_SAFE_DELETE(index);
    index = NULL;
    
}

void SGSkillManager::callback()
{
    if (num < array->count()) {
        num++;
        activateSkill_20038(heroMe, heroEnemy, valueNum, roleidNum, jianNum);
    }
}

//技能伤血
void SGSkillManager::changBloodNum(cocos2d::CCObject *obj, void *num)
{
    SGHeroLayer *hoer = (SGHeroLayer*)obj;
    
    hoer->changBloodNum(-(long)num);
    CCLOG("heroLayer1->getBlood()==%d",hoer->getBlood());
//    heroLayer->showMoraleLabel((int)num, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H);
    if (hoer->getBlood() <= 0) {
        SGBattleManager::sharedBattle()->getBattleLayer()->showIsWin();
    }
}

//解散敌方所有一格蓄力部队。
void SGSkillManager::activateSkill_20002(SGHeroLayer *heroLayer)
{
    SGHeroLayer *opponentHeroLayer = heroLayer->getOpponentHeroLayer();
    CCArray *attackLists = opponentHeroLayer->getBattleMap()->getAttackList();
    CCObject *obj = NULL;

    CCArray *remove = CCArray::create();
    remove->retain();
    CCARRAY_FOREACH(attackLists, obj)
    {
       SGAttackList *attackList = (SGAttackList *)obj;
        
        if (attackList->getAttackType() == kattacknormal)
        {
            
//            attackList->drawSkillsEffect(2);
            CCObject *sbObj = NULL;
            CCARRAY_FOREACH(attackList->getAttackSbs(), sbObj)
            {
                ShiBing *sb = (ShiBing *)sbObj;
                sb->removeChildByTag(EFFECT_LAYER_TAG, true);
                sb->removeChildByTag(EFFECT_LAYER_TAG * 2, true);
                sb->removeChildByTag(EFFECT_LAYER_TAG * 3, true);
                sb->removeChildByTag(EFFECT_LAYER_TAG * 4, true);
                sb->resetSbData();
                opponentHeroLayer->showEffectLayer(12, false, sb->getSbindex());
            }
            remove->addObject(attackList);
            SGNormalAttackList *nAttackList = (SGNormalAttackList *)attackList;
            opponentHeroLayer->getBattleMap()->removeAttackedSB(nAttackList->getFirstSB(), 0);
            
        }
    }
    
    CCARRAY_FOREACH(remove, obj)
    {
        SGAttackList *attackList = (SGAttackList *)obj;
        if (attackLists) {
            attackLists->removeObject(attackList);
        }
    }
    remove->removeAllObjects();
    remove->release();
}
//阻止敌方增兵3回合。（辕门射戟）
void SGSkillManager::activateSkill_20003(SGHeroLayer *heroLayer, int value1, int value2, int roleid)
{
    beginUpTime();
    CCDictionary *buff = CCDictionary::create();
    buff->setObject(CCString::createWithFormat("%d",value1), "round");
    buff->setObject(CCString::createWithFormat("%d",value2), "buffId");
    SGHeroLayer *opponentHeroLayer = heroLayer->getOpponentHeroLayer();
    opponentHeroLayer->addBuff(buff);
    
    //bool isme = true;
    CCPoint pos = ccp(90, screenheight - 60);
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() != roleid)
    {
        //isme = false;
        pos = ccp(screenwidth - 60, 30);
    }
    
    ResourceManager::sharedInstance()->bindTexture("animationFile/jzan.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/jzan.scml", true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(false);
    
    fermEffect->setPosition(pos);
    
    heroLayer->addChild(fermEffect, FORBID_EFFECT_TAG, FORBID_EFFECT_TAG);
    fermEffect->play();
    
    
    
    CCSpriterX *fermEffect1 = CCSpriterX::create("animationFile/jzan_000.scml", true, true);
    fermEffect1->setanimaID(0);
    fermEffect1->setisloop(true);
    fermEffect1->setPosition(pos);
    heroLayer->addChild(fermEffect1, FORBID_EFFECT_TAG * 5, FORBID_EFFECT_TAG * 5);
    fermEffect1->play();
    
    float a = fermEffect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM;
    CCHide *hide = CCHide::create();
    CCDelayTime *time = CCDelayTime::create(a);
    CCShow *show = CCShow::create();
    fermEffect1->runAction(CCSequence::create(hide, time, show, NULL));
}
//发动后，我方在本回合内增加N个行动力，替对方补齐场上兵种（可选择释放）。
void SGSkillManager::activateSkill_20004(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1,float value1, float valule2, int heroId)
{
    heroLayer->changRoundNum(value1, true);

    
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() != heroId)
    {
        if (valule2 > 0) {
            heroLayer1->roundNum ++;
            heroLayer1->fillUp();
        }
//        CCNative::createAlert("^~^", "是否要给对方补兵！", "Cancel");
//        CCNative::addAlertButton("OK");
//        CCNative::showAlert(this);
    }else
    {
        if (valule2 > 0) {
            heroLayer1->roundNum ++;
        }
    }
    
    beginUpTime();
    
}
//随机杀死一个敌方未进入蓄力的武将（2格和4格均可）
void SGSkillManager::activateSkill_20005(SGHeroLayer *heroLayer, CCPoint point, int roleid)
{
    SGHeroLayer *opponentHeroLayer = heroLayer->getOpponentHeroLayer();
    SGBattleMap *opponentBattleMap = opponentHeroLayer->getBattleMap();
    SGGridBase *grid = opponentBattleMap->myGrids[(int)point.y][(int)point.x];
    ShiBing * sb = opponentBattleMap->getShiBingWithGrid(grid);
    if(sb)
    {
        CCString *name = NULL;
        CCString *name1 = NULL;
        if (skill_id_20005 == skillId ||
            skill_id_20076 == skillId) {
            name = CCString::create("animationFile/hfc.plist");
            name1 = CCString::create("animationFile/hfc.scml");
        }else{
            name = CCString::create("animationFile/jy.plist");
            name1 = CCString::create("animationFile/jy.scml");
        }
        
        bool me = !heroLayer->isme;

        CCPoint pos = GameConfig::getGridPoint((int)point.y, (int)point.x, me);
        
        ResourceManager::sharedInstance()->bindTexture(name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
        CCSpriterX *fermEffect = CCSpriterX::create(name1->getCString(), true, true);
        fermEffect->setanimaID(0);
        fermEffect->setisloop(false);
        fermEffect->setPosition(pos);
        fermEffect->play();
        
        if (me) {
            heroLayer->addChild(fermEffect);
        }else{
            heroLayer1->addChild(fermEffect);
        }
        
        
        int a = (fermEffect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM);

        
        CCDelayTime *time = CCDelayTime::create(a);
        CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::sbSkill_20005), (void*)(a * 10000));
        sb->runAction(CCSequence::create(time, call, NULL));
    }
    
}

void SGSkillManager::sbSkill_20005(CCObject *obj, int rand)
{
//    heroLayer1 = SGSkillManager::shareSkill()->heroLayer1;
    
    CCString *str = NULL;
    ShiBing *sb = (ShiBing*)obj;
    
    SGGridBase *grid = (SGGridBase*) sb->getPrinceGridArray()->objectAtIndex(0);
    
    switch (grid->getStype()) {
        case knormal:
        case kdefend:
        {
            heroLayer1->battleMap->removeAttackedSB(sb, 0);
        }
            break;
        case kattack:
        {
            SGAttackList *attack = heroLayer1->battleMap->getAttackListByGrid(grid);
            if(attack)
                 heroLayer1->battleMap->removeAttackList(attack);
        }
            break;
            
        default:
            break;
    }
    
    
    
    
    
    switch (skillId) {
        case skill_id_20005:
        {
            str = CCString::create("animationFile/hfc.plist");
        }
            break;
        case skill_id_20016:
        case skill_id_20017:
        case skill_id_20081:
        {
           str = CCString::create("animationFile/jy.plist");
        }
            break;
        case skill_id_20006:
        case skill_id_20077:
        case skill_id_20036:
        case skill_id_20037:
        case skill_id_20091:
        {
            str = CCString::create("animationFile/ll.plist");
        }
            break;
        case skill_id_20026:
        case skill_id_20027:
        case skill_id_20086:
        {
            str = CCString::create("animationFile/yinsi.plist");
        }
            break;
            
        default:
            break;
    }
     
    float a = rand / 10000;
    CCDelayTime *time = CCDelayTime::create(a);
    CC_SAFE_RETAIN(str);
    CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removePlist), (void*)str);
    
    heroLayer1->runAction(CCSequence::create(time, call, NULL));
    
}

void SGSkillManager::removePlist(CCObject *obj, CCString *str)
{
    CC_SAFE_RELEASE(str);
}

//对敌方阵营随机5个位置进行伤害，总额为主将攻击力的100%
void SGSkillManager::activateSkill_20006(SGHeroLayer *heroLayer, float value1, CCPoint point)
{
    SGHeroLayer *opponentHeroLayer = heroLayer->getOpponentHeroLayer();
    SGBattleMap *opponentBattleMap = opponentHeroLayer->getBattleMap();
    SGGridBase *grid = opponentBattleMap->myGrids[(int)point.y][(int)point.x];
    ShiBing * sb = opponentBattleMap->getShiBingWithGrid(grid);
    if(sb)
    {
        bool a = !heroLayer->isme;
        CCString *name = NULL;
        CCString *name1 = NULL;
        if (skillId == skill_id_20026 ||
            skillId == skill_id_20027 ||
            skillId == skill_id_20086 ) {
            name = CCString::create("animationFile/yinsi.plist");
            name1 = CCString::create("animationFile/yinsi.scml");
        }else{
            name = CCString::create("animationFile/ll.plist");
            name1 = CCString::create("animationFile/ll.scml");
        }
        
        ResourceManager::sharedInstance()->bindTexture(name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
        
        CCSpriterX *fermEffect = CCSpriterX::create(name1->getCString(), true, true);
        fermEffect->setanimaID(0);
        fermEffect->setisloop(false);
    //    fermEffect->setPosition(GameConfig::getGridPoint(a ? -((int)point.y + 1) : (int)point.y, a ? mapList - 1 - (int)point.x :(int)point.x));
        fermEffect->setPosition(GameConfig::getGridPoint((int)point.y, (int)point.x, a));
        fermEffect->play();
        if (heroLayer->isme) {
            heroLayer->addChild(fermEffect, 100);
        }else{
            heroLayer1->addChild(fermEffect, 100);
        }
        
        
        if (!grid) {
            return;
        }
        
        bool isDe = false;
        CCLOG(">>>>>%f",value1);
        if (grid->getStype() == knormal)
        {
            CCLOG("散兵");
            isDe = true;

        }
        else if (grid->getStype() == kattack)
        {
            CCLOG("攻击阵列");
            SGAttackList *attackList = opponentBattleMap->getAttackListByGrid(grid);
            if(attackList)
            {
                if (value1 >= attackList->getAp())
                {
                    CCLOG("被打死");
                    isDe = true;
                }
                else
                {
                    CCLOG("未被打死");
                    attackList->setAp(attackList->getAp() - value1);
                    
                    if (value1 > 0)
                    {
                        attackList->playEffectWithType(kAttackEffect);
                    }
                    
                }
            }
        }
        else if(grid ->getStype() == kdefend)
        {
            CCLOG("防御");
            if (value1 >= sb->getDef())
            {
                CCLOG("被打死");
                isDe = true;
            }
            else
            {
                CCLOG("未被打死");
                sb->setDef(sb->getDef()- value1);
            }
        }
        if (isDe) {
            int a = (fermEffect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM);
            CCDelayTime *time = CCDelayTime::create(a);
            CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::sbSkill_20005), (void*)(a* 10000));
            sb->runAction(CCSequence::create(time, call, NULL));
        }
    }
    beginUpTime();

}
//发动后，3回合内，我方场上兵种数量如果小于等于16，将无视所有对血量值的伤害，并且将所有伤害转移给对方。
void SGSkillManager::activateSkill_20007(SGHeroLayer *heroLayer,  float value1, int value2, int buffid)
{
    CCLOG("value:%d",(int)value1);
    CCLOG("round:%d",value2);
    CCLOG("buffid:%d",buffid);
    CCDictionary *buff = CCDictionary::create();
    buff->setObject(CCString::createWithFormat("%d",(int)value1), "value");
    buff->setObject(CCString::createWithFormat("%d",value2), "round");
    buff->setObject(CCString::createWithFormat("%d",buffid), "buffId");
    
    heroLayer->addBuff(buff);
    heroLayer->showEffectLayer(0, false);
    beginUpTime();
    
}
//发动后，恢复我方血量值10%
void SGSkillManager::activateSkill_20008(SGHeroLayer *heroLayer, float rate)
{
    heroLayer->changBloodNum(heroLayer->heroBloodMax *rate);
//    heroLayer->showMoraleLabel(heroLayer->heroBloodMax *rate, "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H);
    beginUpTime();
}
//增加连锁和融合部队攻击力10%
void SGSkillManager::activateSkill_20010(SGHeroLayer *heroLayer,  float rate)
{
    CCArray *attackList = heroLayer->getBattleMap()->getAttackList();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackList, obj)
    {
        SGAttackList *attackList = (SGAttackList*)obj;
        if (attackList->getLRType() == lr_l || attackList->getLRType() == lr_r || attackList->getLRType() == lr_lr)
        {
            int ap = attackList->getMax_Ap() * rate;
            attackList->setSpAddAp(attackList->getSpAddAp() + ap);
            attackList->changeAP(attackList->getAp() + ap);
            attackList->drawSkillsEffect(2);
        }
    }
    beginUpTime();
}
// 同时降低敌方一名四格武将和己方一名四格武将，当前攻击力的50%
//（优先蓄力武将，其次散兵武将，对其造成他攻击力50%的伤害。）
void SGSkillManager::activateSkill_20012(SGHeroLayer *heroLayer,SGHeroLayer *heroLayer1, float value1, int s_id)
{
    float value = value1;
    CCLOG("value1:%f",value1);
    CCLOG("value:%f",value);
    checkTheHero(heroLayer, value1, s_id);
    checkTheHero(heroLayer1, value1, s_id);
    
    heroLayer->battleMap->refreshMapGrid(true);
//    beginUpTime();
}

//降低总血量值25%，增加所有攻击力（使用后包括现在攻击阵列的，和以后形成攻击阵列的）5%
void SGSkillManager::activateSkill_20014(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid)
{
    CCDictionary *buff = CCDictionary::create();
    
    buff->setObject(CCString::createWithFormat("%d",buffid), "buffId");
    buff->setObject(CCString::createWithFormat("%d",round), "round");
    buff->setObject(CCString::createWithFormat("%f",value1), "value1");
    buff->setObject(CCString::createWithFormat("%f",value2), "value2");
    
    heroLayer->battleMap->addHeroBuff(buff);
    
    heroLayer->battleMap->activateHeroBuff(false);
    heroLayer->activateBuff();
    
    
    beginUpTime();
    
    
}

//void SGSkillManager::baoZa(CCObject *obj, ShiBing *sb)
//{
//
//    SGHeroLayer *hero = (SGHeroLayer*)obj;
//    hero->showEffectLayer(8, false, sb->getSbindex());
//    
//}

void SGSkillManager::baoZa(CCObject *obj, GameIndex index)
{

    SGHeroLayer *hero = (SGHeroLayer*)obj;
    hero->showEffectLayer(8, false, index);

}

//将敌方所有的墙转化为炸弹，伤害周围所有相邻单位（极限情况是5个）
//无处调用，已改进到SGHeroSkill13里头！
void SGSkillManager::activateSkill_20018(SGHeroLayer *heroLayer, GameIndex index, cocos2d::CCArray *array, float value2, int num)
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
        CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::baoZa), (void*)& index);
        heroLayer->runAction(CCSequence::create(time, call, NULL));
        float b = 1.6 * ANIMA_TIME_NUM;
        
        float c = a + b;
        int ap = sb->getDef() * value2;
        
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
                                if(sb)
                                {
                                    if (sb->getDef() > 0) {
                                        ap = ap - sb->getDef();
                                        CCLOG("i=%d-------------------------------------> ap=%d  sb->getDef()=%d", i,ap, sb->getDef());
                                    }
                                    
                                    CCDelayTime* delay = CCDelayTime::create(c);
                                    CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                    sb->runAction(CCSequence::create( delay, call, NULL));
                                }
                            }
                                break;
                            case kdefend:
                            {
                                ShiBing* sb = battleMap->getShiBingWithGrid(grid1);
                                if(sb)
                                {

                                    if (sb->getDef() > 0) {
                                        ap = ap - sb->getDef();
                                        if (ap < 0) {
                                            heroLayer->showBattleLabel( sb->getDef() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                                        }
                                        CCLOG("i=%d-------------------------------------> ap=%d  sb->getDef()=%d", i,ap, sb->getDef());
                                        sb->setDef(-ap);
                                    }
                                    
                                    CCDelayTime* delay = CCDelayTime::create(c);
                                    CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                    sb->runAction(CCSequence::create( delay, call, NULL));
                                }
                                
                            }
                                break;
                            case kattack:
                            {
                                SGAttackList* attack = battleMap->getAttackListByGrid(grid1);
                                if(attack)
                                {
                                    if (attack->getAp() > 0) {
                                        ap = ap - attack->getAp();
                                        CCLOG("i=%d-------------------------------------> ap=%d  sb->getDef()=%d", i,ap, attack->getAp());
                                        if (ap < 0) {
                                            heroLayer->showBattleLabel( attack->getAp() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attack->getAttackIndex());
                                        }
                                        attack->setAp(-ap);
                                        
                                        if (ap > 0)
                                        {
                                            attack->playEffectWithType(kAttackEffect);
                                        }
                                        
                                    }
                                    
                                    
                                    CCDelayTime* delay = CCDelayTime::create(c);
                                    CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                    ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                                    sb->runAction(CCSequence::create( delay, call, NULL));
                                }
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
        battleMap->removeAttackedSB(sb, 0);
    }
    beginUpTime();
}
//增加所有敌方攻击阵列蓄力回合2，攻击力上限不变。
void SGSkillManager::activateSkill_20020(SGHeroLayer *heroLayer, int addValue)
{
    SGHeroLayer *opponentHeroLayer = heroLayer->getOpponentHeroLayer();
    CCArray* attackLists = opponentHeroLayer->getBattleMap()->getAttackList();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackLists, obj)
    {
        SGAttackList *attackList = (SGAttackList *)obj;
        attackList->changeRound(addValue, true);
    }
    beginUpTime();
}
//对敌方全列，进行武将攻击力总额75%的伤害
float SGSkillManager::activateSkill_20022(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid)
{
    float time = 1.2;
    float sbTime = time/12;
    
    bool isme = true;
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() != roleid)
    {
        isme = false;
    }
    
    for (int i = 0; i < mapList; i++) {
        
        
//        CCSprite* sp = CCSprite::create("animationFile/Ratt_u14.png");
//        
//        heroLayer->addChild(sp, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG + i);
//        
//        if (false == isme) {
//            sp->setScaleY(-1);
//        }
//        
//        sp->setPosition(GameConfig::getGridPoint(isme ? 5 : -5,   abs(isme ? -i :  (mapList - 1 - i)),true));
//        
//        CCDelayTime *delaytime = CCDelayTime::create(sbTime*0.5*i);
//        CCMoveTo *move = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow : mapRow, abs(isme ? -i : (mapList - 1 - i)) ,true));
//        sp->runAction(CCSequence::create(delaytime, move, NULL));
        
        
        
        
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
                
                if (i == mapRow) {
                    //播放打到底线动作
                    GameSbIndex *sbin = new GameSbIndex();
                    sbin->i = mapRow;
                    sbin->j = j;
                    CCDelayTime* deal0 = CCDelayTime::create(a);
                    CCDelayTime* delay1 = CCDelayTime::create(b + sbTime);
                    CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGSkillManager::removeBullet),
                                                               (void*)(SKILLS_BULLET_TAG + j));
                    CCCallFuncND* call3 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGSkillManager::playBulletBlast),
                                                               (void*)sbin);
                    heroLayer->runAction(CCSequence::create( deal0, delay1, call1, call3, NULL));
                    
                    CCDelayTime* deal01 = CCDelayTime::create(a);
                    CCDelayTime* delay11 = CCDelayTime::create(b + sbTime);
                    CCCallFuncND* call11 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGSkillManager::changBloodNum),
                                                               (void*)ap);
                    heroLayer1->runAction(CCSequence::create( deal01, delay11, call11, NULL));
                    
                    
                }else{
                    SGGridBase *grid = heroLayer1->battleMap->myGrids[i][abs(mapList-1-j)];
                    if (grid) {
                        if (grid->getStype() == knormal) {
                            
                            ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                            if(sb)
                            {
                                CCDelayTime* deal = CCDelayTime::create(a);
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create( deal, delay, call, NULL));
                                ap = ap - sb->getDef();
                                if (sb->getSbType() == ksbtwo || sb->getSbType() == ksbfour) {
    //                                sb->removeSBGrid();
                                    i++;
                                }
                            }
                            
                            
                        }else if(grid->getStype() == kdefend)
                        {
                            ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                            
                            if(sb)
                            {
                                CCDelayTime* deal = CCDelayTime::create(a);
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create( deal, delay, call, NULL));

                                ap = ap - sb->getDef();
                                if (ap < 0) {
                                    heroLayer1->showBattleLabel( sb->getDef() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                                }
                                sb->setDef(-ap);
                            }

                            
                            
//                            if (ap > sb->getDef()) {
//                                heroLayer1->battleMap->removeAttackedSB(sb, 0);
//                            }else{
//                                sb->setDef(sb->getDef() - ap);
//                                sb->setDefImage();
//                                sb->showDef(sb->getDef());
//                            }
                        }else
                        {
                            SGAttackList* attack = heroLayer1->battleMap->getAttackListByGrid(grid);
                            if(attack)
                            {
                                CCDelayTime* deal = CCDelayTime::create(a);
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                                sb->runAction(CCSequence::create( deal, delay, call, NULL));
                                ap = ap - attack->getAp();
                                if (ap < 0) {
                                    heroLayer1->showBattleLabel( attack->getAp() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                                }
                                attack->setAp(-ap);
                                
                                if (ap > 0)
                                {
                                    attack->playEffectWithType(kAttackEffect);
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
                            CCDelayTime* deal0 = CCDelayTime::create(a);
                            CCDelayTime* delay1 = CCDelayTime::create(b);
                            CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                                       callfuncND_selector(SGSkillManager::removeBullet),
                                                                       (void*)(SKILLS_BULLET_TAG + j));
//                            CCCallFuncND* call3 = CCCallFuncND::create(this,
//                                                                       callfuncND_selector(SGSkillManager::playBulletBlast),
//                                                                       (void*)sbin);
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

}

//技能效果 移除兵
void SGSkillManager::removesb(cocos2d::CCObject *obj, SGHeroLayer*hero)
{
    ShiBing* sb = (ShiBing*)obj;
    SGGridBase* grid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
    if (grid->getStype() == knormal)
    {
        hero->battleMap->removeAttackedSB(sb, 0, hero);
    }
    else if(grid->getStype() == kdefend)
    {
        if (sb->getDef() > 0)
        {
            sb->setDefImage();
            sb->showDef(sb->getDef());
        }
        else
        {
            hero->battleMap->removeAttackedSB(sb, 0, hero);
        }
    }
    else
    {
        SGAttackList* attack = hero->battleMap->getAttackListByGrid(grid);
        if (attack)
        {
            if (attack->getAp() > 0 &&
                hero->battleMap->myGrids[grid->getIndex().i][grid->getIndex().j]) {
                attack->changeAP(attack->getAp());
            }
            else
            {
                hero->battleMap->removeAttackList(attack, hero, grid);
            }
        }
    }
    //GPCCLOG("删除了兵。。。");
}

//使我方四格武将攻击阵列蓄力回合立刻降低3。（如果已经是1的不变）
void SGSkillManager::activateSkill_20024(SGHeroLayer *heroLayer, int value)
{
    CCArray *attacklist = heroLayer->getBattleMap()->getAttackList();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attacklist, obj)
    {
        SGAttackList *al = (SGAttackList *)obj;
        if (al->getAttackType() == kattackfour)
        {
            if ((al->getRoundV()- value) >0)
            {
                al->changeRound(-value, true);
                int a = al->getAddAp() * value;
                al->changeAP(al->getAp() + a);
            }
            else
            {
                al->changeRound(1-al->getRoundV(), true);
                int a = al->getAddAp() * al->getRoundV() - 1;
                al->changeAP(al->getAp() + a);
            }
        }
    }
    beginUpTime();
}
//对敌方阵营随机5个位置进行伤害，总额主将攻击力的50%
void SGSkillManager::activateSkill_20026(SGHeroLayer *heroLayer, cocos2d::CCObject *data)
{
    //CCDictionary *dict = CCDictionary::create();
    
    //SGHeroLayer *opponentHeroLayer = heroLayer->getOpponentHeroLayer();
    //SGBattleMap *opponentBattleMap = opponentHeroLayer->getBattleMap();
    
}
//降低总血量值25%，增加所有当前攻击阵列攻击力10%
void SGSkillManager::activateSkill_20028(SGHeroLayer *heroLayer,  float value1, float value2)
{
    int ap = heroLayer->heroBloodMax*value1;
    heroLayer->changBloodNum(-ap);
//    heroLayer->showMoraleLabel(-ap, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H);
    
    this->activateSkill_20032(heroLayer, value2);
    if (heroLayer->getBlood() <= 0) {
        SGBattleManager::sharedBattle()->getBattleLayer()->showIsWin();
    }
    beginUpTime();
}
//解除敌方当前融合效果
void SGSkillManager::activateSkill_20030(SGHeroLayer *heroLayer, int skillsId)
{
//    SGHeroLayer *opponentHeroLayer = heroLayer->getOpponentHeroLayer();
//    CCArray *attackList = opponentHeroLayer->getAttackLists();
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(heroLayer->battleMap->getAttackList(), obj)
    {
        SGAttackList *al = (SGAttackList *)obj;
        if (al->getLRType() == lr_r ||
            al->getLRType() == lr_l) {
        }
        if (al->getLRType() == lr_r)
        {
//            al->drawSkillsEffect(6);
            heroLayer->showEffectLayer(15, false, gameIndex(al->getAttackIndex().i + 1, al->getAttackIndex().j));
            al->setFuseNum(0);
//            al->setLRType(lr_normal);
        }else if(al->getLRType() == lr_l){
//            al->drawSkillsEffect(5);
            heroLayer->showEffectLayer(14, false, gameIndex(al->getAttackIndex().i + 1, al->getAttackIndex().j));
//            al->setLRType(lr_normal);
        }
        
        if (skillsId == skill_id_20030) {
            if (al->getLRType() == lr_l) {
                al->setAddChain(0);
            }
        }
    }
    beginUpTime();
}
//增加现在所有蓄力状态阵列攻击力20%
void SGSkillManager::activateSkill_20032(SGHeroLayer *heroLayer, float value)
{
    CCArray *attackList = heroLayer->getBattleMap()->getAttackList();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackList, obj)
    {
        SGAttackList *attackList = (SGAttackList *)obj;
        int ap = attackList->getMax_Ap() * value;
        attackList->setSpAddAp(attackList->getSpAddAp() + ap);
        attackList->changeAP(attackList->getAp() + ap, true);
        attackList->drawSkillsEffect(2);
    }
    beginUpTime();
}
//增加现在两格蓄力状态阵列攻击力10%
void SGSkillManager::activateSkill_20033(SGHeroLayer *heroLayer, float value)
{
    CCArray *attackList = heroLayer->battleMap->getAttackList();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackList, obj)
    {
        SGAttackList *attackList = (SGAttackList *)obj;
        if (attackList->getAttackType() == kattacktwo)
        {
            int ap = attackList->getMax_Ap() * value;
            attackList->setSpAddAp(attackList->getSpAddAp() + ap);
            attackList->changeAP(attackList->getAp() + ap, true);
            ShiBing *sb = (ShiBing*) attackList->attackSbs->objectAtIndex(0);

            attackList->showBattleLabel(ap, "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
        }
    }
    beginUpTime();
}
//发动后，我方在下2个回合内，召唤等待区的兵不费回合数。
void SGSkillManager::activateSkill_20034(SGHeroLayer *heroLayer, int round, int buffid)
{
    CCDictionary *buff = CCDictionary::create();
    
    buff->setObject(CCString::createWithFormat("%d",buffid), "buffId");
    buff->setObject(CCString::createWithFormat("%d",round), "round");
    heroLayer->addBuff(buff);
    
    beginUpTime();
}
//对敌方阵营随机5个位置进行伤害，总额主将攻击力的75%
void SGSkillManager::activateSkill_20036(SGHeroLayer *heroLayer, cocos2d::CCObject *data)
{
    
}

void SGSkillManager::huoQiu(CCObject *obj, ShiBing *sb)
{
    SGHeroLayer *hero = (SGHeroLayer*)obj;
    hero->showEffectLayer(9, true, sb->getSbindex());
}

//将所有己方墙转化为火球进行攻击（墙当前的列不变）攻击力等同于当前墙的防御值。
void SGSkillManager::activateSkill_20038(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid, int jianid)
{
    
    if (num >= array->count()) {
        CCLOG("所有火球攻击完毕！！");
        array->removeAllObjects();
        heroLayer->battleMap->refreshMapGrid(true);
        return;
    }
    
    if (array->count() &&
        num < array->count()) {
        
        ShiBing* sb = (ShiBing*)array->objectAtIndex(num);
        
        SGGridBase* sbgrid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
        heroLayer->battleMap->removeAttackedSB(sb, 0);
        heroLayer->showEffectLayer(7, true, sb->getSbindex());
        float a = 0.5 * ANIMA_TIME_NUM;
        CCDelayTime *dtime = CCDelayTime::create(a);
        CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::huoQiu), (void*)sb);
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
        ResourceManager::sharedInstance()->bindTexture(CCString::createWithFormat("animationFile/jian_%d.plist", num)->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
        CCSpriterX *fermEffect = CCSpriterX::create(CCString::createWithFormat("animationFile/jian_%d.scml",num)->getCString(), true, true);
        fermEffect->setanimaID(0);
        fermEffect->setisloop(true);
        fermEffect->play();
        heroLayer->addChild(fermEffect, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG + num);
    
        if (false == isme) {
            //MMDEBUG: FLIPY
            fermEffect->setScaleY(-1);
//            fermEffect->setCCSpriterXFlipY(true);
        }

        fermEffect->setPosition(GameConfig::getGridPoint(isme ? sbgrid->getIndex().i : -sbgrid->getIndex().i + OLD_EFFECT_FLIPY_FIX,   abs(isme ? -sbgrid->getIndex().j :  (mapList - 1 - sbgrid->getIndex().j)),true));
        CCHide *hide = CCHide::create();
        CCDelayTime *time_d = CCDelayTime::create(c);
        CCShow *show = CCShow::create();
        CCMoveTo *move = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow : mapRow + OLD_EFFECT_FLIPY_FIX, abs(isme ? -sbgrid->getIndex().j : (mapList - 1 - sbgrid->getIndex().j)) ,true));
        fermEffect->runAction(CCSequence::create(hide, time_d, show, move, NULL));
        int ap = sb->getDef() * value1;
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
                        CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                                   callfuncND_selector(SGSkillManager::removeBullet),
                                                                   (void*)(SKILLS_BULLET_TAG + num));
                        CCCallFuncND* call12 = CCCallFuncND::create(this,
                                                                   callfuncND_selector(SGSkillManager::playBulletBlast),
                                                                   (void*)sbin);
                        
                        heroLayer->runAction(CCSequence::create( delay1, call1, call12, NULL));
                        
                        CCDelayTime* delay11 = CCDelayTime::create(b);
                        CCCallFuncND* call11 = CCCallFuncND::create(this,
                                                                    callfuncND_selector(SGSkillManager::changBloodNum),
                                                                    (void*)ap);
                        CCDelayTime* delay13 = CCDelayTime::create(0.4);
                        CCCallFunc* call3 = CCCallFunc::create(this, callfunc_selector(SGSkillManager::callback));
                        heroLayer1->runAction(CCSequence::create( delay11, call11, delay13, call3, NULL));
                        
                        
                    }else{
                        SGGridBase *grid = heroLayer1->battleMap->myGrids[i][abs(mapList-1-sbgrid->getIndex().j)];
                        if (grid) {
                            if (grid->getStype() == knormal) {
                                
                                ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                                if(sb)
                                {
                                    CCDelayTime* delay = CCDelayTime::create(b);
                                    CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                    sb->runAction(CCSequence::create( delay, call, NULL));
                                    ap = ap - sb->getDef();
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
                                    CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                    sb->runAction(CCSequence::create( delay, call, NULL));
                                    ap = ap - sb->getDef();
                                    if (ap < 0) {
                                        heroLayer1->showBattleLabel( sb->getDef() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                                    }
                                    sb->setDef(-ap);
                                }
                                
                            }else
                            {
                                SGAttackList* attack = heroLayer1->battleMap->getAttackListByGrid(grid);
                                if(attack)
                                {
                                    CCDelayTime* delay = CCDelayTime::create(b);
                                    CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                    ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                                    sb->runAction(CCSequence::create( delay, call, NULL));
                                    ap = ap - attack->getAp();
                                    if (ap < 0) {
                                        heroLayer1->showBattleLabel( sb->getDef() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                                    }
                                    attack->setAp(-ap);
                                    if (ap > 0)
                                    {
                                        attack->playEffectWithType(kAttackEffect);
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
                                CCCallFuncND* call12 = CCCallFuncND::create(this,
                                                                           callfuncND_selector(SGSkillManager::removeBullet),
                                                                           (void*)(SKILLS_BULLET_TAG + num));
                                CCCallFuncND* call14 = CCCallFuncND::create(this,
                                                                            callfuncND_selector(SGSkillManager::playBulletBlast),
                                                                            (void*)sbin);
                                CCDelayTime* delay13 = CCDelayTime::create(0.4);
                                CCCallFunc* call3 = CCCallFunc::create(this, callfunc_selector(SGSkillManager::callback));
                                heroLayer->runAction(CCSequence::create( delay12, call12, call14, delay13, call3, NULL));

                            }
                        }
                    }
                }
                else{
                    break;
                }
            }
        
    }else{
        
    }
}
//选择三列，发射攻击总额为自身攻击力50%的弩箭。
void SGSkillManager::activateSkill_20040(SGHeroLayer *heroLayer, cocos2d::CCObject *data)
{
    
}
// 在敌方阵营第二排放置一颗炸弹，炸弹移动过去时也有攻击力，
//（玩家控制）2回合后爆炸，前两排都受到武将10%攻击力的伤害
void SGSkillManager::activateSkill_20043(SGHeroLayer *heroLayer, cocos2d::CCObject *data)
{
    
}
//增加散兵状态防御力10%
void SGSkillManager::activateSkill_20046(SGHeroLayer *heroLayer, float value)
{
    SGBattleMap *battleMap = heroLayer->getBattleMap();
    CCArray *soldiers = battleMap->getSoldiers();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(soldiers, obj)
    {
        ShiBing *sb = (ShiBing *)obj;
        CCArray *grids = sb->getPrinceGridArray();
        SGGridBase *grid = (SGGridBase *)grids->objectAtIndex(0);
        if (grid->getStype() == knormal)
        {
            sb->showEffect(1);
            int addValue = sb->getDef() * value;
            sb->setDef(sb->getDef() + addValue);
            sb->showDef(sb->getDef());
        }
    }
    beginUpTime();
}
//使对方主将计发动增加2回合。
void SGSkillManager::activateSkill_20048(SGHeroLayer *heroLayer, float value)
{
    SGHeroLayer *opponentHeroLayer = heroLayer->getOpponentHeroLayer();

    opponentHeroLayer->setSkillsRoundNum(value, true);
    beginUpTime();
}
//增加连锁部队攻击力2%（永久起效）
void SGSkillManager::activateSkill_20052(SGHeroLayer *heroLayer, float value1,int buffId)
{

    CCObject *obj = NULL;
    CCARRAY_FOREACH(heroLayer->battleMap->getAttackList(), obj)
    {
        SGAttackList *al = (SGAttackList *)obj;
        if (al->getLRType() == lr_l)
        {
            int ap = al->getMax_Ap() * value1;
            al->setSpAddAp(al->getSpAddAp() + ap);
            al->changeAP(al->getAp() + ap, true);
            ShiBing *sb = (ShiBing*)al->attackSbs->objectAtIndex(0);
            al->showBattleLabel(ap, "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
        }
    }
    CCDictionary *buff = CCDictionary::create();
    buff->setObject(CCString::createWithFormat("%d",buffId), "buffId");
    buff->setObject(CCString::createWithFormat("%f",value1), "value");
    
    
    
//    for (int i = 0; i < heroLayer->buffs->count(); i++) {
//        bool isadd = true;
//        CCDictionary *_buff = (CCDictionary *)heroLayer->buffs->objectAtIndex(i);
//        if (buffId == ((CCString*)_buff->objectForKey("buffId"))->intValue()) {
//            float a = ((CCString*)_buff->objectForKey("value"))->floatValue();
//            float b = a + value1;
//            _buff->setObject(CCString::createWithFormat("%f", b), "value");
//            isadd = false;
//        }
//        
//        if (isadd) {
//            heroLayer->addBuff(buff);
//        }
//    }
//    
//    if (!heroLayer->buffs->count()) {
//        heroLayer->addBuff(buff);
//    }
    
    
    beginUpTime();
}
//阻止敌方增兵2回合。
void SGSkillManager::activateSkill_20056(SGHeroLayer *heroLayer, cocos2d::CCObject *data)
{
    
}
//发动后，等于点击了一次等待区，补齐了兵种。
void SGSkillManager::activateSkill_20058(SGHeroLayer *heroLayer, float value1)
{
//    beginUpTime();
    heroLayer->battleMap->m_touchState = eTouchNull;
    if (value1 > 0) {
        heroLayer->changRoundNum(1, false);
        heroLayer->fillUp();
    }else{
        this->beginAi();
        beginUpTime();
    }
}
//发动后，所有我方散兵状态2格武将直接形成攻击阵列。
void SGSkillManager::activateSkill_20060(SGHeroLayer *heroLayer)
{
    SGBattleMap *battleMap = heroLayer->getBattleMap();
    CCArray *soldiers = battleMap->getSoldiers();
    CCArray *sbArray = CCArray::create();
    CCObject *obj = NULL;
    bool isR = false;
    CCARRAY_FOREACH(soldiers, obj)
    {
        ShiBing *sb = (ShiBing *)obj;
        if (sb->getSbType() == ksbtwo )
        {
            sbArray->removeAllObjects();
            SGTwoAttackList *al = SGTwoAttackList::createWithSoldier(sb, true);
            battleMap->getAttackList()->addObject(al);
            sbArray->addObject(sb);
            al->drawSkillsEffect(4);
            sb->showEffect(2, false);
            isR = true;
        }
        battleMap->spgongJiSwapGridObj(sbArray);
    }
    
    if (!isR) {
        beginAi();
        beginUpTime();
    }else{
        CCDelayTime *time = CCDelayTime::create(1.5);
        CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGSkillManager::ccc));
        heroLayer->runAction(CCSequence::create(time, call, NULL));
    }
}
// 在我方底线放置一排防御墙，可以抵御下回合75%伤害，
//（上限为武将攻击力）如果没有受到伤害，将以主将攻击力25%的伤害进行全列攻击
void SGSkillManager::activateSkill_20063(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, int roleid, int buffid, int round, float value1, float value2, float value3)
{
    CCDictionary *buff = CCDictionary::create();
    
    buff->setObject(heroLayer, "hero");
    buff->setObject(heroLayer1, "hero1");
    buff->setObject(CCString::createWithFormat("%d",roleid), "roleid");
    buff->setObject(CCString::createWithFormat("%d",buffid), "buffId");//
    buff->setObject(CCString::createWithFormat("%d",round), "round");
    buff->setObject(CCString::createWithFormat("%f",value1), "value1");
    buff->setObject(CCString::createWithFormat("%f",value2), "value2");
    buff->setObject(CCString::createWithFormat("%f",value3), "value3");
    
    heroLayer->addBuff(buff);
    
    bool isme = true;
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() != roleid)
    {
        isme = false;
    }
    
    ResourceManager::sharedInstance()->bindTexture("animationFile/dq.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    for (int i = 0; i < mapList; i++) {

        
        CCSpriterX *fermEffect = CCSpriterX::create("animationFile/dq.scml", true, true);
        fermEffect->setanimaID(0);
        fermEffect->setisloop(true);
//        fermEffect->setPosition(GameConfig::getGridPoint(isme ? 5 : -5,   abs(isme ? -i :  (mapList - 1 - i)),true));
        CCPoint pos = GameConfig::getGridPoint(isme ? mapRow : -mapRow,   abs(isme ? -i : (mapList - 1 - i)),true);
        fermEffect->setPosition(ccp(pos.x, isme ? heroLayer->uibgY : (screenheight - heroLayer->uibgY)));
        fermEffect->play();
        heroLayer->addChild(fermEffect, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG + i);
        
    }
    
    beginUpTime();

}
//发动后，以我方所有散兵（不包括武将）的防御力总和形成一个攻击球，玩家选择列发射出去。
void SGSkillManager::activateSkill_20065(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid)
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
    
    CCMoveTo *move = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow : mapRow, abs(isme ? -indexJ : (mapList - 1 - indexJ)) ,true));
    heroLayer->m_skills_Bullet->runAction(CCSequence::create(move, NULL));
    
        int ap = value1;
        for (int i = 0; i <= mapRow; i++) {
            if (ap > 0) {
                float b = sbTime * (i+5.5);
                if (i == mapRow) {
                    //播放打到底线动作
                    CCDelayTime* delay1 = CCDelayTime::create(b + sbTime);
                    CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGSkillManager::removeBullet),
                                                               (void*)(SKILLS_BULLET_TAG));
                    GameSbIndex *sbin = new GameSbIndex();
                    sbin->i = mapRow;
                    sbin->j = indexJ;
                    CCCallFuncND *call3 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGSkillManager::playBulletBlast),
                                                               (void*)sbin);
                    heroLayer->runAction(CCSequence::create(delay1, call1, call3, NULL));
                    
                    CCDelayTime* delay2 = CCDelayTime::create(b + sbTime);
                    CCCallFuncND* call2 = CCCallFuncND::create(this,
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
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create(delay, call, NULL));
                                ap = ap - sb->getDef();
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
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create(delay, call, NULL));
                                ap = ap - sb->getDef();
                                if (ap < 0) {
                                    heroLayer->showBattleLabel( sb->getDef() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                                }
                                sb->setDef(-ap);
                            }
                            
                        }else
                        {
                            SGAttackList* attack = heroLayer1->battleMap->getAttackListByGrid(grid);
                            if(attack)
                            {
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                                sb->runAction(CCSequence::create( delay, call, NULL));
                                ap = ap - attack->getAp();
                                if (ap < 0) {
                                    heroLayer->showBattleLabel( attack->getAp() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                                }
                                attack->setAp(-ap);
                                if (ap > 0)
                                {
                                    attack->playEffectWithType(kAttackEffect);
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
                                                                       callfuncND_selector(SGSkillManager::removeBullet),
                                                                       (void*)(SKILLS_BULLET_TAG));
                            GameSbIndex *sbin = new GameSbIndex();
                            sbin->i = i;
                            sbin->j = indexJ;
                            CCCallFuncND *call3 = CCCallFuncND::create(this,
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
    
    heroLayer->battleMap->refreshMapGrid(true);
}

void SGSkillManager::activateSkill_20066(SGHeroLayer *heroLayer, float value)
{
    SGBattleMap *battleMap = heroLayer->getBattleMap();
    CCArray *soldiers = battleMap->getSoldiers();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(soldiers, obj)
    {
        ShiBing *sb = (ShiBing *)obj;
        SGGridBase*grid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
        if ( grid->getStype() == knormal )
        {
            sb->setDef(sb->getDef() + sb->getDef() * value);
            sb->showDef(sb->getDef());
            sb->showEffect(1);
            heroLayer->showBattleLabel(sb->getDef() * value, "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, gameIndex(grid->getIndex().i, grid->getIndex().j), true);
        }
    }
    beginUpTime();
}

void SGSkillManager::readySkill_20001(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid)
{
//    SGBattleMap *battleMap = heroLayer->getBattleMap();
//    battleMap->getTag();

}

void SGSkillManager::alertViewClickedButtonAtIndex(int buttonIndex)
{
    
}


void SGSkillManager::checkTheHero(SGHeroLayer *hero, float value1, int s_id)
{
    CCLOG("value1::::%f",value1);
    sgShiBingType sbType;
    sgAttackSoldierType type;
    if (s_id == skill_id_20012)
    {
        sbType = ksbfour;
        type = kattackfour;
    }else{
        sbType = ksbtwo;
        type = kattacktwo;
    }
    
    bool isFind = false;
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapList; j++)
        {
            SGGridBase *grid = hero->battleMap->myGrids[i][j];
            if (grid && grid->getStype() == kattack)
            {
                SGAttackList* attack = hero->battleMap->getAttackListByGrid(grid);
                if (attack&&attack->getAttackType() == type)
                {
                    attack->drawSkillsEffect(0);
                    int ap = attack->getMax_Ap() * value1;
                    CCLOG("nowap:%d",attack->getAp());
                    CCLOG("ap:%d",ap);
                    attack->setAp(attack->getAp() - ap);
					
					if (ap > 0)
					{
						attack->playEffectWithType(kAttackEffect);
					}
					
                    CCLOG("afterap:%d",attack->getAp());
                    if (attack->getAp()<=0)
                    {
                        hero->battleMap->removeAttackList(attack);
                    }else{
                        attack->changeAP(attack->getAp(), true, true);
                    }
                    isFind  = true;
                    break;
                }
                
            }
        }
        if (isFind)
        {
            break;
        }
    }
    
    if (!isFind)
    {
        for (int i = 0; i < mapList; i++)
        {
            for (int j = 0; j < mapRow; j++)
            {
                SGGridBase *grid = hero->battleMap->myGrids[j][i];
                if (grid && grid->getStype() == knormal && grid->getSbType() == sbType)
                {
                    isFind = true;
                    ShiBing *bing = hero->battleMap->getShiBingWithGrid(grid);
                    if(bing)
                         hero->battleMap->removeAttackedSB(bing, 0);
                    break;
//                    if (bing->getSbType() == ksbfour)
//                    {
//                        hero->battleMap->removeAttackedSB(bing, 0);
//                    }
                }
            }
            if (isFind)
            {
                break;
            }
        }
    }
    
}
//幽冥盾反击作用
void SGSkillManager::skill_20063_Attack(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid, int acttime)
{
    heroLayer->endRoundUpdataTime();
    //heroLayer->battleMap->m_touchState = eTouchSkillsFang;
    SGAIManager::shareManager()->isAi = false;
    
    float time = acttime;
    float sbTime = time/13;
    
    bool isme = true;
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() != roleid)
    {
        isme = false;
    }
    
    float removeTime = 0.;
    float removeTime1 = 0.;
    ResourceManager::sharedInstance()->bindTexture("animationFile/dq.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    for (int i = 0; i < mapList; i++) {
        heroLayer->removeChildByTag(SKILLS_BULLET_TAG + i, true);
        

        CCSpriterX *fermEffect = CCSpriterX::create("animationFile/dq_000.scml", true, true);
        fermEffect->setanimaID(0);
        fermEffect->setisloop(false);
        
        CCPoint pos = GameConfig::getGridPoint(isme ? mapRow : -mapRow,   abs(isme ? -i : (mapList - 1 - i)),true);
        fermEffect->setPosition(ccp(pos.x, isme ? heroLayer->uibgY : (screenheight - heroLayer->uibgY)));
        
        fermEffect->play();
        heroLayer->addChild(fermEffect, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG - 1);
        
        removeTime = fermEffect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM;
        
        
        CCSpriterX *fermEffect1 = CCSpriterX::create("animationFile/dq_001.scml", true, true);
        fermEffect1->setanimaID(0);
        fermEffect1->setisloop(true);
        fermEffect1->setPosition(GameConfig::getGridPoint(isme ? mapRow - 1 : -mapRow + 1,   abs(isme ? -i :  (mapList - 1 - i)),true));
        fermEffect1->play();
        heroLayer->addChild(fermEffect1, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG + i);
        
        //removeTime1 = fermEffect1->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM;
        
        CCHide *hide = CCHide::create();
        CCDelayTime *del = CCDelayTime::create(removeTime);
        CCShow *show = CCShow::create();
        CCMoveTo *move = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow -2 : mapRow, abs(isme ? -i : (mapList - 1 - i)) ,true));
        fermEffect1->runAction(CCSequence::create( hide, del, show, move, NULL));
        
        
//        CCSprite* sp = CCSprite::create("animationFile/Ratt_u14.png");
//        heroLayer->addChild(sp, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG + i);
//        
//        if (false == isme) {
//            sp->setScaleY(-1);
//        }
//        
//        sp->setPosition(GameConfig::getGridPoint(isme ? mapRow : -mapRow,   abs(isme ? -i :  (mapList - 1 - i)),true));
//        
//        CCMoveTo *move = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow : mapRow, abs(isme ? -i : (mapList - 1 - i)) ,true));
//        sp->runAction(CCSequence::create( move, NULL));
    }
    int nAP = ceil(value1/8);
    
    for (int j = 0; j < mapList; j++) {
        int ap = nAP;
        for (int i = 0; i <= mapRow; i++) {
            if (ap > 0) {
                float b = sbTime * (i+9);
                b = b + (removeTime );
                if (i == mapRow) {
                    GameSbIndex *sbin = new GameSbIndex();
                    sbin->i = mapRow;
                    sbin->j = j;
                    CCDelayTime* delay1 = CCDelayTime::create(b + sbTime);
                    CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGSkillManager::removeBullet),
                                                               (void*)(SKILLS_BULLET_TAG + j));
                    CCCallFuncND *call3 = CCCallFuncND::create(this,
                                                               callfuncND_selector(SGSkillManager::playBulletBlast),
                                                               (void*)sbin);
                    heroLayer->runAction(CCSequence::create( delay1, call1, call3, NULL));
                    
                    CCDelayTime* delay11 = CCDelayTime::create(b + sbTime);
                    CCCallFuncND* call11 = CCCallFuncND::create(this,
                                                                callfuncND_selector(SGSkillManager::changBloodNum),
                                                                (void*)ap);
                    heroLayer1->runAction(CCSequence::create( delay11, call11, NULL));
                    
                }else{
                    SGGridBase *grid = heroLayer1->battleMap->myGrids[i][abs(mapList-1-j)];
                    if (grid) {
                        if (grid->getStype() == knormal) {
                            
                            ShiBing* sb = heroLayer1->battleMap->getShiBingWithGrid(grid);
                            if(sb)
                            {
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create( delay, call, NULL));
                                ap = ap - sb->getDef();
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
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                sb->runAction(CCSequence::create( delay, call, NULL));
                                ap = ap - sb->getDef();
                                if (ap < 0) {
                                    heroLayer->showBattleLabel( sb->getDef() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                                }
                                sb->setDef(-ap);
                            }
                            
                            
                            //                            if (ap > sb->getDef()) {
                            //                                heroLayer1->battleMap->removeAttackedSB(sb, 0);
                            //                            }else{
                            //                                sb->setDef(sb->getDef() - ap);
                            //                                sb->setDefImage();
                            //                                sb->showDef(sb->getDef());
                            //                            }
                        }else
                        {
                            SGAttackList* attack = heroLayer1->battleMap->getAttackListByGrid(grid);
                            if(attack)
                            {
                                CCDelayTime* delay = CCDelayTime::create(b);
                                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::removesb), heroLayer1);
                                ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                                sb->runAction(CCSequence::create( delay, call, NULL));
                                ap = ap - attack->getAp();
                                if (ap < 0) {
                                    heroLayer->showBattleLabel( attack->getAp() - (ap * -1), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                                }
                                attack->setAp(-ap);
                                
                                if (ap > 0)
                                {
                                    attack->playEffectWithType(kAttackEffect);
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
                            CCDelayTime* delay1 = CCDelayTime::create(b);
                            CCCallFuncND* call1 = CCCallFuncND::create(this,
                                                                       callfuncND_selector(SGSkillManager::removeBullet),
                                                                       (void*)(SKILLS_BULLET_TAG + j));
                            CCCallFuncND *call3 = CCCallFuncND::create(this,
                                                                       callfuncND_selector(SGSkillManager::playBulletBlast),
                                                                       (void*)sbin);
                            
                            heroLayer->runAction(CCSequence::create( delay1, call1, call3, NULL));
                        }
                        
                    }
                }
            }
            else{
            }
        }
    }
    
    heroLayer->restartRoundUpdataTime();

}

void SGSkillManager::beginAi()
{
    this->setIsCompleteSkill();
   SGAIManager::shareManager()->startRound(heroLayer->battleMap);
    CCLog("**********SGSkillManager::beginAi()**********");
}
//触发回合改变
void SGSkillManager::beginUpTime()
{
    heroLayer->beginRoundTime();
    heroLayer->battleMap->m_touchState = eTouchNull;
}

void SGSkillManager::setIsCompleteSkill()
{
    SGAIManager::shareManager()->setIsCompleteSkill(true);
    CCLOG("setIsCompleteSkill || %d -- [%s -> %d]", SGAIManager::shareManager()->getIsCompleteSkill(), __FUNCTION__, __LINE__);

}


//
void SGSkillManager::bbb(cocos2d::CCObject *obj)
{
    SGHeroLayer *layer = (SGHeroLayer *)obj;
    //设置对应主将技发完的标记
    if (layer->m_skills_Bullet)
    {
        CCLOG("SGHeroLayer || m_skills_Bullet");
        layer->removeChild(layer->m_skills_Bullet, true);
        layer->m_skills_Bullet = NULL;
    }
    if (layer->m_skills_Bullet1)
    {
        CCLOG("SGHeroLayer || m_skills_Bullet1");
        layer->removeChild(layer->m_skills_Bullet1, true);
        layer->m_skills_Bullet1 = NULL;
    }
    if (layer->m_skills_Bullet2)
    {
        CCLOG("SGHeroLayer || m_skills_Bullet2");
        layer->removeChild(layer->m_skills_Bullet2, true);
        layer->m_skills_Bullet2 = NULL;
    }
    
    
    //一个技能打完，回合数重置的时候
    SGAIManager::shareManager()->setIsDispSkillHead(false);
    SGAIManager::shareManager()->setIsCompleteSkill(true);
    CCLOG("setIsCompleteSkill || %d -- [%s -> %d]", SGAIManager::shareManager()->getIsCompleteSkill(), __FUNCTION__, __LINE__);

    isBeginAi = true;
    SGHeroLayer *hero = (SGHeroLayer*)obj;
    if (!SGAIManager::shareManager()->isAi) {
        SGAIManager::shareManager()->isAi = true;
    }
    
    hero->beginRoundTime();
    hero->battleMap->m_touchState = eTouchNull;
    SGAIManager::shareManager()->startRound(hero->battleMap);
}

void SGSkillManager::ccc(cocos2d::CCObject *obj)
{
    isBeginAi = true;
    SGHeroLayer *hero = (SGHeroLayer*)obj;
    if (!SGAIManager::shareManager()->isAi) {
        SGAIManager::shareManager()->isAi = true;
    }
    hero->battleMap->refreshMapGrid(true);
}
