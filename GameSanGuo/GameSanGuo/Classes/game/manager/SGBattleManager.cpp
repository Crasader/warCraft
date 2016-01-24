//
//  SGBattleManager.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-20.
//
//

#include "SGBattleManager.h"
#include "SGSocketClient.h"

#include "CooperationModule.pb.h"
#include "Logout.pb.h"
#include "ZQMD5.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGSoldier.h"
#include "SGMainManager.h"
#include "SGOfficerCard.h"
#include "SGPlayerInfo.h"
#include "SGSkillManager.h"
#include "SGAIManager.h"
#include "SGStaticDataManager.h"
#include "SGBattleSpeedLayer.h"
#include "SGStoryReward.h"
#include "SGStoryRewardBox.h"
#include "SGStoryRewardItem.h"
#include "SGGuideManager.h"
#include "GameMusicmanage.h"
#include "SGOverBattleBox.h"
#include "SGBattlePreNLayer.h"
#include "CGPBattleLoadingLayer.h"

#include "SGCantAdvanceBox.h"
#include "SGMainLayer.h"
#include "SGTestSwitch.h"
#include "SurvivalModule.pb.h"
#include "SGSjzData.h"
#include "PlotModule.pb.h"
#include "SGStringConfig.h"
#include "SG_BuffBase.h"
#include "SGQfightBox.h"
#include "SGPvpFightRewardLayer.h"
#include "ArenaModule.pb.h"
#include "SGArenaFightRewardLayer.h"


#define TOTALDROPCOUNT      "TotalDropCount"
#define MYSBSDIC            "mySbsDic"
#define ENEMYSBSDIC         "enemySbsDic"
#define MYATTACKDIC         "myAttackDic"
#define ENEMYATTACKDIC      "enemyAttackDic"
#define MYDataDIC           "myDataDic"
#define ENEMYDataDIC        "enemyDataDic"
#define ATTACKBUFFDIC       "buffDic"
#define MYBACKUPSBDIC       "myBackupSbDic"
#define ENEMYBACKUPSBDIC    "enemyBackupSbDic"
#define FRIENDHERODATA      "friendHeroData"

#define CLIENTSAVEBATTLE  "_battleSbs.plist"


using namespace main;
static SGBattleManager *s_ShareBattle = NULL;

//战斗延时展示锁屏时间。
const static float BATTLE_DELAY_SHOW_TIME = 0.4f;  //before is 1.0f

SGBattleManager* SGBattleManager::sharedBattle(void)
{
    if (!s_ShareBattle) {
        s_ShareBattle = new SGBattleManager();
        
    }
    return s_ShareBattle;
}

SGBattleManager::SGBattleManager()
:mainBattleLayer(NULL)
,isHotSeat(false)
,isContinue(false)
,isWin(false)
,isMatchSucc(false)
,isSecond(false)
,_battleType(BT_PEACE)
,m_isBattlePve(false)
,startFiveRound(-1)
,pveLoadMoveFinish(false)
,_sjzLevel(0)
,isGuide(false)
,myHeroObj(NULL)
,enemyHeroObj(NULL)
,himsbs(NULL)
,mysbs(NULL)
,himBackupSbs(NULL)
,myBackupsbs(NULL)
,himBackupHeros(NULL)
,myBackupHeros(NULL)
,cooTempOfficer(NULL)
,m_canTouchAutoBattleBtn(true)
{
    himBackupSbs = CCArray::create();
    myBackupsbs = CCArray::create();
    
    himBackupHeros = CCArray::create();
    myBackupHeros = CCArray::create();
    
    himBackupSbs->retain();
    myBackupsbs->retain();
    
    himBackupHeros->retain();
    myBackupHeros->retain();
    
    isLoadingReady = false;
}

SGBattleManager::~SGBattleManager()
{
    CCLOG("~SGBattleManager");
    CC_SAFE_RELEASE(himBackupSbs);
    CC_SAFE_RELEASE(myBackupsbs);
    CC_SAFE_RELEASE(himBackupHeros);
    CC_SAFE_RELEASE(myBackupHeros);
     CC_SAFE_RELEASE(cooTempOfficer);
}
void SGBattleManager::initNotification()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_BATTLE_MATCH, this, callfuncO_selector(SGBattleManager::matchListener));
    notification->addObserver(MSG_BATTLE_MOVE, this, callfuncO_selector(SGBattleManager::moveListener));
    notification->addObserver(MSG_BATTLE_DELETE, this, callfuncO_selector(SGBattleManager::deleteListener));
    notification->addObserver(MSG_BATTLE_FILL, this, callfuncO_selector(SGBattleManager::fillUnitListener));
    notification->addObserver(MSG_BATTLE_GIVEUP, this, callfuncO_selector(SGBattleManager::giveUpListener));
    notification->addObserver(MSG_BATTLE_OVER, this, callfuncO_selector(SGBattleManager::gameoverListener));
    notification->addObserver(MSG_VALIDATION, this, callfuncO_selector(SGBattleManager::validationListener));
    notification->addObserver(MSG_BATTLE_PVESTART, this, callfuncO_selector(SGBattleManager::pveListener));   //cgp_pro
    notification->addObserver(MSG_LOST_CONNECTION, this, callfuncO_selector(SGBattleManager::lostConnection));
    notification->addObserver(MSG_STORY_BATTLE_OVER, this, callfuncO_selector(SGBattleManager::overSettlement));
    notification->addObserver(MSG_RUNAWAY, this, callfuncO_selector(SGBattleManager::runAwayResponse));
//    notification->addObserver(MSG_FRIEND, this, callfuncO_selector(SGBattleManager::cooperationFillResponse));
    notification->addObserver(MSG_BATTLE_REVIVE, this, callfuncO_selector(SGBattleManager::fightReviveResponse));
    notification->addObserver(MSG_PVE_LOADING, this, callfuncO_selector(SGBattleManager::pveLoadingListener));
    notification->addObserver(MSG_FightFixMapRequest, this, callfuncO_selector(SGBattleManager::fightFixMapResponse));
    notification->addObserver(MSG_PveContinuePlayRequest, this, callfuncO_selector(SGBattleManager::pveContinuListener));
    notification->addObserver(MSG_FillUnitRevive, this, callfuncO_selector(SGBattleManager::reviveResponse));
    notification->addObserver(MSG_QiangZhiFillUnitRequest, this, callfuncO_selector(SGBattleManager::QiangZhiFillUnitResponse));
  //  notification->addObserver(MSG_SURVIVAL_CHALLENGE, this, callfuncO_selector(SGBattleManager::sjzChallengeResponse)); //守荆州试图挑战
    notification->addObserver(MSG_SURVIVAL_START, this, callfuncO_selector(SGBattleManager::sjzStartResponse)); //守荆州试图开始
    notification->addObserver(MSG_SURVIVAL_FINAL_STAT, this, callfuncO_selector(SGBattleManager::sjzFinalStatResponse)); //守荆州战败战绩信息
    notification->addObserver(MSG_PVP_CANCEL_WAITE, this, callfuncO_selector(SGBattleManager::PveCancelWaiteListener));//取消等待
    
    notification->addObserver(MSG_PVP_FIGHT_REWARD,this, callfuncO_selector(SGBattleManager::pvpFightRewardResponse));//新pvp战斗奖励
    //这两个消息从SGMainLayer里挪过来的。
    notification->addObserver(MSG_PVP_LOADING, this, callfuncO_selector(SGBattleManager::pvpLoadingListener));
    notification->addObserver(MSG_PVPFIGHT_LOGIN, this , callfuncO_selector(SGBattleManager::matchListener1));
    //监听竞技场战斗结束消息
    notification->addObserver(MSG_ARENA_END_GIGHT, this , callfuncO_selector(SGBattleManager::arenaFightRewardListen));
    
    SGSkillManager::shareSkill()->initNotification();
}

void SGBattleManager::logSbData(SGSBObj *obj, int i)
{
    if (obj->getSkillsId() > 0)
    {
        GPCCLOG("sbid===%d",obj->getSid());
        GPCCLOG("sbName===%s",obj->getName().c_str());
        GPCCLOG("AfftributeNum===%f",obj->getAfftributeNum());
        GPCCLOG("AfftributeNum1===%f",obj->getAfftributeNum1());
        GPCCLOG("AfftributeNum2===%d",obj->getAfftributeNum2());
        GPCCLOG("data->getSkillHead()==%d",obj->getSkill_Head());
        if (obj->getBuffId() > 0) {
            GPCCLOG("buffid===%d",obj->getBuffId());
            GPCCLOG("BuffRound===%d",obj->getBuffRound());
            GPCCLOG("BuffAfftribute===%f",obj->getBuffAfftribute());
            GPCCLOG("setBuffType===%d",obj->getBuffType());
        }
        GPCCLOG("\n%d", i);
    }
}

void SGBattleManager::pveStart(int storyId, int plotId, int roleId)
{
    SGAIManager::shareManager()->isPVE = true;

    main::StartStatePVERequest *sr = new main::StartStatePVERequest();
    
    sr->set_plotid(plotId);
    sr->set_cooid(roleId);
    sr->set_storyid(storyId);
    
    
    SGSocketClient::sharedSocketClient()->send(MSG_PVE_LOADING, sr);
    
    SGMainManager::shareMain()->trackMsnBegin(plotId);
}
void SGBattleManager::pveListener(cocos2d::CCObject *obj)
{
    
    SGPlayerInfo::sharePlayerInfo()->setBattleType(0);
    SGSocketRequest *socketRequest = (SGSocketRequest *)obj;
    
    if (socketRequest)
    {
        main::PveStarResponse *sr = (main::PveStarResponse *)socketRequest->m_msg;
        
        if (sr && sr->state() == 1)
        {
            SGAIManager::shareManager()->isBattleOver = true;
            SGAIManager::shareManager()->isInitPve = true;
            //获取随即地图
            cc_timeval psv;
            CCTime::gettimeofdayCocos2d(&psv, NULL);
            unsigned long int rand_seed = psv.tv_sec * 1000 + psv.tv_usec/1000;
            srand((unsigned int)rand_seed);
            
            int maxRandomMap = 5;
            
            setRandomMapId((int)(CCRANDOM_0_1() * maxRandomMap + 1)) ;//产生一个随机数
            
            CCString *randmapIdStr = CCString::createWithFormat("background_%d.png", getRandomMapId());
            mainBattleLayer = SGBattleLayer::create(randmapIdStr->getCString());

            mainBattleLayer->setBattleCardNum(sr->dropitemcount());
            SGPlayerInfo::sharePlayerInfo()->setPlotId(sr->plotid());
            
            GPCCLOG("BattleCardNum==%d", mainBattleLayer->getBattleCardNum());
            
            himBackupHeros->removeAllObjects();
            myBackupHeros->removeAllObjects();
            
            GPCCLOG("自己方英雄数据");
            int mysblength = sr->spiritsyou_size();
            mysbs = CCArray::create();
            mysbs->retain();
            myBackupsbs->removeAllObjects();
            
            for (int i = 0; i< mysblength; i++)
            {
                main::FightSpirit fs = sr->spiritsyou(i);
                SGSBObj *obj = new SGSBObj();
                obj->setSid(fs.sid());
                
                obj->setColorId(fs.color());
                obj->setType(fs.type());
                obj->setIndex(gameIndex(fs.y(),fs.x()));
                
                obj->setsbBaseDefendPlus(fs.defeatplus());
                
                obj->setsbBaseAttack(fs.baseattack());
                obj->setsbBaseDefend(fs.basedef());
                
                obj->setAp(fs.attack());
                obj->setDef(fs.def());
                
                obj->setResetInitDef(fs.basedef());
                obj->setDefRend(fs.walltype());
                obj->setRound(fs.round());
                obj->setSkillsId(fs.skillid());//服务器给到的技能id
                obj->setAfftributeNum(fs.value1());
                obj->setAfftributeNum1(fs.value2());
                obj->setAfftributeNum2(fs.randnum());//杀死武将队列的随机位置,(对于破袭技能)
                obj->setBuffId(fs.buffid());
                obj->setBuffRound(fs.effectround());
                obj->setBuffAfftribute(fs.bvalue1());
                obj->setBuffType(fs.bufftype());
                obj->setAddDef(fs.wallceiling());
                obj->setAddAp(fs.perroundaddatk());
                obj->setChainNUm(fs.chainandmergeattackplus());
                obj->setRoundVAddDef(fs.wallgrow());
                obj->setAtkModeSkill(fs.atkmodeskill());
                obj->setsbSkLevel(1);
                
                if (fs.type() == ksbone)
                {
                    SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(fs.sid());
                    obj->setAtkmode(data->getSoldierAtkMode());
                    obj->setAtkMode2(data->getSoldierAtkMode2());
                    obj->setHeadId(data->getSoldierHead());
                    obj->setFileId(data->getSoldierFileId());
                    obj->setPair(data->getSoldierPair());
                    obj->setHitEff(data->getSoldierLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getSoldierName()->getCString());
                    addMyBackUpSb(obj);
                }
                else
                {
                    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(fs.sid());
                    obj->setAtkmode(data->getOfficerAtkMode());
                    obj->setAtkMode2(data->getOfficerAtkMode2());
                    obj->setHeadId(data->getOfficerHead());
                    obj->setFileId(data->getOfficerFileId());
                    obj->setPair(data->getOfficerPair());
                    obj->setHitEff(data->getOfficerLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getOfficerName()->getCString());
                    obj->setSkill_Head(data->getSkillHead());
                }
                logSbData(obj, i);
                mysbs->addObject(obj);
                obj->autorelease();
                
            }
            GPCCLOG("敌人方英雄数据");
            
            int himsblength = sr->spiritshim_size();
            himsbs = CCArray::create();
            himsbs->retain();
            himBackupSbs->removeAllObjects();
            for (int i = 0; i< himsblength; i++)
            {
                main::FightSpirit fs = sr->spiritshim(i);
                SGSBObj *obj = new SGSBObj();
                obj->setSid(fs.sid());
                
                obj->setColorId(fs.color());
                obj->setType(fs.type());
                
                obj->setsbBaseDefendPlus(fs.defeatplus());
                
                obj->setsbBaseAttack(fs.baseattack());
                obj->setsbBaseDefend(fs.basedef());
                
                obj->setAp(fs.attack());
                obj->setDef(fs.def());
                
                obj->setResetInitDef(fs.basedef());
                obj->setDefRend(fs.walltype());
                obj->setRound(fs.round());
                obj->setIndex(gameIndex(fs.y(),fs.x()));
                obj->setSkillsId(fs.skillid());
                obj->setAfftributeNum(fs.value1());
                obj->setAfftributeNum1(fs.value2());
                obj->setAfftributeNum2(fs.randnum());
                obj->setBuffId(fs.buffid());
                obj->setBuffRound(fs.effectround());
                obj->setBuffAfftribute(fs.bvalue1());
                obj->setBuffType(fs.bufftype());
                obj->setAddDef(fs.wallceiling());
                obj->setAddAp(fs.perroundaddatk());
                obj->setChainNUm(fs.chainandmergeattackplus());
                obj->setRoundVAddDef(fs.wallgrow());
                obj->setAtkModeSkill(fs.atkmodeskill());
                obj->setsbSkLevel(1);
                if (fs.type() == ksbone)
                {
                    SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(fs.sid());
                    obj->setAtkmode(data->getSoldierAtkMode());
                    obj->setAtkMode2(data->getSoldierAtkMode2());
                    obj->setHeadId(data->getSoldierHead());
                    obj->setFileId(data->getSoldierFileId());
                    obj->setPair(data->getSoldierPair());
                    obj->setHitEff(data->getSoldierLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getSoldierName()->getCString());
                    addHisBackUpSb(obj);
                }
                else
                {
                    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(fs.sid());
                    obj->setAtkmode(data->getOfficerAtkMode());
                    obj->setAtkMode2(data->getOfficerAtkMode2());
                    obj->setHeadId(data->getOfficerHead());
                    obj->setFileId(data->getOfficerFileId());
                    obj->setPair(data->getOfficerPair());
                    obj->setHitEff(data->getOfficerLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getOfficerName()->getCString());
                    obj->setSkill_Head(data->getSkillHead());
                    
                }
                logSbData(obj, i);
                himsbs->addObject(obj);
                obj->autorelease();
                
            }
            
            
            myHeroObj = new HeroObj();
            myHeroObj->setRoleId(sr->roleid());
            myHeroObj->setHp(sr->youhp());
            myHeroObj->setBaseHp(sr->youbasehp());
            myHeroObj->setActionCount(sr->youactioncount());
            myHeroObj->setActionCount1(sr->youactioncountbyadd());
            myHeroObj->setItemId(sr->lordid());
            myHeroObj->setFriendLevel(sr->friendofficerlevel());
            myHeroObj->setFriendHeadId(sr->friendofficerid());
            myHeroObj->setName(CCString::create(sr->lordname()));
            myHeroObj->setSkillId(sr->lordskillbelongid());//技能id
            myHeroObj->setSkillRoundCur(sr->lordcdround());
            myHeroObj->setSkillRoundMax(sr->lordcdroundbase());
            myHeroObj->setMax_soldierNum(sr->maxfillunit());
            myHeroObj->setClientType(sr->clienttype());
            myHeroObj->setCountry(sr->nation());
            myHeroObj->setFriendRound(sr->friendofficercdround());
            myHeroObj->set_friendRound(sr->friendofficercdroundcurrent());
            myHeroObj->setLordSkillID(sr->lordskillid());//展示id
            myHeroObj->setAtkmode(sr->lordatkmode());
            
            
            
            
            enemyHeroObj = new HeroObj();
            enemyHeroObj->setRoleId(sr->troleid());
            enemyHeroObj->setHp(sr->himhp());
            enemyHeroObj->setBaseHp(sr->himbasehp());
            enemyHeroObj->setActionCount(sr->himactioncount());
            enemyHeroObj->setActionCount1(sr->himactioncountbyadd());
            enemyHeroObj->setItemId(sr->tlordid());
            enemyHeroObj->setFriendLevel(sr->friendofficerlevel());
            enemyHeroObj->setFriendHeadId(sr->friendofficerid());
            enemyHeroObj->setName(CCString::create(sr->tlordname()));
            enemyHeroObj->setSkillId(sr->tlordskillbelongid());//技能id
            enemyHeroObj->setSkillRoundCur(sr->tlordcdroundbase());
            enemyHeroObj->setSkillRoundMax(sr->tlordcdroundbase());
            enemyHeroObj->setMax_soldierNum(sr->tmaxfillunit());
            enemyHeroObj->setClientType(sr->tclienttype());
            enemyHeroObj->setCountry(sr->tnation());
            enemyHeroObj->setLordSkillID(sr->tlordskillid());
            enemyHeroObj->setAtkmode(sr->tlordatkmode());
            
            if (sr->isfirstattack())
            {
                enemyHeroObj->setActionCount1(0);
            }
            else
            {
                myHeroObj->setActionCount1(0);
            }
            
            //被收回备用的英雄，敌我双方,不包括士兵
            addPveBackHero(sr);
            
            bool isPveContinue = (himsblength == 0 && mysblength == 0);
            SGMainManager::shareMain()->showBattleLayer(mainBattleLayer,isPveContinue);
            
            mainBattleLayer->setIsMyRound(sr->isfirstattack());
            mainBattleLayer->changeColorSp();
            
            pveLoadMoveFinish = false;
            mainBattleLayer->addEffectLayer();
            
            mainBattleLayer->showBattleId(sr->fid());
            mainBattleLayer->m_battleId =  sr->fid();
            
            mainBattleLayer->textNewEff();

        }
        
    }
    
    this->initHisMap();
    this->initMyMap();

    setIsLoadingReady(true);

}

void SGBattleManager::pveListenerDelay()
{
    
    SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_batttleLoadingLayer, true);

    int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
    CCString *word = NULL;
    if (plotId>0 && plotId < 10000)
    { // 非活动
        word = SGStaticDataManager::shareStatic()->getPKWordById(plotId);
        if (word != NULL) { // set pk pro
            SGGuideManager::shareManager()->isFightDesc = true;
        }
    }
    
    ////守荆州
    if (plotId >= 0 && plotId < 4
        && !SGBattleManager::sharedBattle()->isSecond
        && !(_battleType == BT_SURVIVAL))
    {
        SGBattleManager::sharedBattle()->isGuide=true;
    }
    else
    {
        SGBattleManager::sharedBattle()->isGuide=false;
    }


    CCLOG("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM");
    
    if (SGGuideManager::shareManager()->isFightDesc)
    { // show pk words
        SGGuideManager::shareManager()->showPKWords(word, mainBattleLayer);
    }
    
    
    //播放音乐
    if(SGPlayerInfo::sharePlayerInfo()->isLastSceneOfStory(SGPlayerInfo::sharePlayerInfo()->getPlotId()))
    {
        MUSIC_PLAY(MUSIC_BOSS);
    }
    else
    {
        MUSIC_PLAY(MUSIC_NORMAL);
    }
    EFFECT_PLAY(MUSIC_24);
    
    SGBattleManager::sharedBattle()->getBattleLayer()->setSpeedUiNew(false);
    SGBattleManager::sharedBattle()->getBattleLayer()->setSpeedUiNew(true);

    //MMHACK: 进入战斗在此允许“自动战斗”
    
    SGBattleManager::sharedBattle()->setCanTouchAutoBattleBtn(true);

}


void SGBattleManager::addPveBackHero(void *ptrSr,bool needFriendly )
{
    main::PveStarResponse *sr = NULL;
    
    main::FillUnitReviveResponse *msg =  NULL;
    
    if (needFriendly) {
        sr = (main::PveStarResponse *)ptrSr;
    }
    else
    {
        msg = (main::FillUnitReviveResponse *)ptrSr;
    }
    
    SGStaticDataManager *officerDataManager = SGStaticDataManager::shareStatic();
    SGOfficerDataModel *data = NULL;
    myBackupHeros->removeAllObjects();
    
    //my
    int myHerolength = needFriendly ? sr->selftempofficer_size() : msg->selftempofficer_size();
    for (int i = 0; i< myHerolength; i++)
    {
        main::FightSpirit fs = needFriendly ? sr->selftempofficer(i) : msg->selftempofficer(i);
        SGSBObj *obj = new SGSBObj();
        obj->setSid(fs.sid());
        
        obj->setColorId(fs.color());
        obj->setType(fs.type());
        obj->setIndex(gameIndex(fs.y(),fs.x()));
        
        obj->setsbBaseDefendPlus(fs.defeatplus());
        
        obj->setsbBaseAttack(fs.baseattack());
        obj->setsbBaseDefend(fs.basedef());
        
        obj->setAp(fs.attack());
        obj->setDef(fs.def());
        
        obj->setResetInitDef(fs.basedef());
        obj->setDefRend(fs.walltype());
        obj->setRound(fs.round());
        obj->setSkillsId(fs.skillid());//服务器给到的技能id
        obj->setAfftributeNum(fs.value1());
        obj->setAfftributeNum1(fs.value2());
        obj->setAfftributeNum2(fs.randnum());//杀死武将队列的随机位置,(对于破袭技能)
        obj->setBuffId(fs.buffid());
        obj->setBuffRound(fs.effectround());
        obj->setBuffAfftribute(fs.bvalue1());
        obj->setBuffType(fs.bufftype());
        obj->setAddDef(fs.wallceiling());
        obj->setAddAp(fs.perroundaddatk());
        obj->setChainNUm(fs.chainandmergeattackplus());
        obj->setRoundVAddDef(fs.wallgrow());
        obj->setAtkModeSkill(fs.atkmodeskill());
        obj->setsbSkLevel(1);
        
        data = officerDataManager->getOfficerById(fs.sid());
        obj->setAtkmode(data->getOfficerAtkMode());
        obj->setAtkMode2(data->getOfficerAtkMode2());
        obj->setHeadId(data->getOfficerHead());
        obj->setFileId(data->getOfficerFileId());
        obj->setPair(data->getOfficerPair());
        obj->setHitEff(data->getOfficerLastHit());
        obj->setAttackEffect(data->getAttackEffect());
        obj->setDeathEffect(data->getDeathEffect());
        obj->setName(data->getOfficerName()->getCString());
        obj->setSkill_Head(data->getSkillHead());
        obj->setSkillsId(data->getSkill());
        myBackupHeros->addObject(obj);
        obj->autorelease();
    }
    ///////////////////不想封函数
    
    
    if (needFriendly)
    {
        int himHerolength = needFriendly?sr->aitempofficer_size():0;
        himBackupHeros->removeAllObjects();
        for (int i = 0; i< himHerolength; i++)
        {
            main::FightSpirit fs = sr->aitempofficer(i);
            SGSBObj *obj = new SGSBObj();
            obj->setSid(fs.sid());
            
            obj->setColorId(fs.color());
            obj->setType(fs.type());
            obj->setIndex(gameIndex(fs.y(),fs.x()));
            
            obj->setsbBaseDefendPlus(fs.defeatplus());
            
            obj->setsbBaseAttack(fs.baseattack());
            obj->setsbBaseDefend(fs.basedef());
            
            obj->setAp(fs.attack());
            obj->setDef(fs.def());
            
            obj->setResetInitDef(fs.basedef());
            obj->setDefRend(fs.walltype());
            obj->setRound(fs.round());
            obj->setSkillsId(fs.skillid());//服务器给到的技能id
            obj->setAfftributeNum(fs.value1());
            obj->setAfftributeNum1(fs.value2());
            obj->setAfftributeNum2(fs.randnum());//杀死武将队列的随机位置,(对于破袭技能)
            obj->setBuffId(fs.buffid());
            obj->setBuffRound(fs.effectround());
            obj->setBuffAfftribute(fs.bvalue1());
            obj->setBuffType(fs.bufftype());
            obj->setAddDef(fs.wallceiling());
            obj->setAddAp(fs.perroundaddatk());
            obj->setChainNUm(fs.chainandmergeattackplus());
            obj->setRoundVAddDef(fs.wallgrow());
            obj->setAtkModeSkill(fs.atkmodeskill());
            obj->setsbSkLevel(1);

                data = officerDataManager->getOfficerById(fs.sid());
                obj->setAtkmode(data->getOfficerAtkMode());
                obj->setAtkMode2(data->getOfficerAtkMode2());
                obj->setHeadId(data->getOfficerHead());
                obj->setFileId(data->getOfficerFileId());
                obj->setPair(data->getOfficerPair());
                obj->setHitEff(data->getOfficerLastHit());
                obj->setAttackEffect(data->getAttackEffect());
                obj->setDeathEffect(data->getDeathEffect());
                obj->setName(data->getOfficerName()->getCString());
                obj->setSkill_Head(data->getSkillHead());
                obj->setSkillsId(data->getSkill());

            himBackupHeros->addObject(obj);
            obj->autorelease();
        }
    
        //////友军
        if (cooTempOfficer)
        {
            cooTempOfficer->autorelease();
            cooTempOfficer = NULL;
        }
        if(_battleType != BT_ARENA)//竞技场没有友军
        {
            main::FightSpirit fs = sr->cootempofficer();
            SGSBObj *obj = new SGSBObj();
            obj->setSid(fs.sid());
            
            obj->setColorId(fs.color());
            obj->setType(fs.type());
            obj->setIndex(gameIndex(fs.y(),fs.x()));
            
            obj->setsbBaseDefendPlus(fs.defeatplus());
            
            obj->setsbBaseAttack(fs.baseattack());
            obj->setsbBaseDefend(fs.basedef());
            
            obj->setAp(fs.attack());
            obj->setDef(fs.def());
            
            obj->setResetInitDef(fs.basedef());
            obj->setDefRend(fs.walltype());
            obj->setRound(fs.round());
            obj->setSkillsId(fs.skillid());//服务器给到的技能id
            obj->setAfftributeNum(fs.value1());
            obj->setAfftributeNum1(fs.value2());
            obj->setAfftributeNum2(fs.randnum());//杀死武将队列的随机位置,(对于破袭技能)
            obj->setBuffId(fs.buffid());
            obj->setBuffRound(fs.effectround());
            obj->setBuffAfftribute(fs.bvalue1());
            obj->setBuffType(fs.bufftype());
            obj->setAddDef(fs.wallceiling());
            obj->setAddAp(fs.perroundaddatk());
            obj->setChainNUm(fs.chainandmergeattackplus());
            obj->setRoundVAddDef(fs.wallgrow());
            obj->setAtkModeSkill(fs.atkmodeskill());
            obj->setsbSkLevel(1);

            data = officerDataManager->getOfficerById(fs.sid());
            obj->setAtkmode(data->getOfficerAtkMode());
            obj->setAtkMode2(data->getOfficerAtkMode2());
            obj->setHeadId(data->getOfficerHead());
            obj->setFileId(data->getOfficerFileId());
            obj->setPair(data->getOfficerPair());
            obj->setHitEff(data->getOfficerLastHit());
            obj->setAttackEffect(data->getAttackEffect());
            obj->setDeathEffect(data->getDeathEffect());
            obj->setName(data->getOfficerName()->getCString());
            obj->setSkill_Head(data->getSkillHead());
    
            cooTempOfficer = obj;
            cooTempOfficer->retain();

            obj->autorelease();
        }
    }

    startFiveRound = -1;
}

void SGBattleManager::dealCgPveListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *socketRequest = (SGSocketRequest *)obj;
    
    if (socketRequest)
    {
        main::PlotPrologueStartResponse *srCg = (main::PlotPrologueStartResponse *)socketRequest->m_msg;
        main::PveStarResponse sr = srCg->resp();
        if (sr.state() == 1)
        {
            //获取随即地图
            CCString *randmapIdStr = CCString::createWithFormat("background_%d.png",sr.mapid());
            mainBattleLayer = SGBattleLayer::create(randmapIdStr->getCString(), true);
            
            CCLOG("自己方英雄数据");
            int mysblength = sr.spiritsyou_size();
            mysbs = CCArray::create();
            mysbs->retain();
            for (int i = 0; i< mysblength; i++)
            {
                main::FightSpirit fs = sr.spiritsyou(i);
                SGSBObj *obj = new SGSBObj();
                obj->setSid(fs.sid());
                
                obj->setColorId(fs.color());
                obj->setType(fs.type());
                obj->setIndex(gameIndex(fs.y(),fs.x()));
                
                obj->setsbBaseDefendPlus(fs.defeatplus());
                
                obj->setsbBaseAttack(fs.baseattack());
                obj->setsbBaseDefend(fs.basedef());
                
                obj->setAp(fs.attack());
                obj->setDef(fs.def());
                
                obj->setResetInitDef(fs.basedef());
                obj->setDefRend(fs.walltype());
                obj->setRound(fs.round());//
                obj->setSkillsId(fs.skillid());
                obj->setAfftributeNum(fs.value1());
                obj->setAfftributeNum1(fs.value2());
                obj->setAfftributeNum2(fs.randnum());
                obj->setBuffId(fs.buffid());
                obj->setBuffRound(fs.effectround());
                obj->setBuffAfftribute(fs.bvalue1());
                obj->setBuffType(fs.bufftype());
                obj->setAddDef(fs.wallceiling());
                obj->setAddAp(fs.perroundaddatk());
                obj->setChainNUm(fs.chainandmergeattackplus());
                obj->setRoundVAddDef(fs.wallgrow()  );
                obj->setAtkModeSkill(fs.atkmodeskill());
                
                
                if (fs.type() == ksbone) {
                    SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(fs.sid());
                    obj->setAtkmode(data->getSoldierAtkMode());
                    obj->setAtkMode2(data->getSoldierAtkMode2());
                    obj->setHeadId(data->getSoldierHead());
                    obj->setFileId(data->getSoldierFileId());
                    obj->setPair(data->getSoldierPair());
                    obj->setHitEff(data->getSoldierLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getSoldierName()->getCString());
                }else{
                    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(fs.sid());
                    obj->setAtkmode(data->getOfficerAtkMode());
                    obj->setAtkMode2(data->getOfficerAtkMode2());
                    obj->setHeadId(data->getOfficerHead());
                    obj->setFileId(data->getOfficerFileId());
                    obj->setPair(data->getOfficerPair());
                    obj->setHitEff(data->getOfficerLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getOfficerName()->getCString());
                    obj->setSkill_Head(data->getSkillHead());
                }
                logSbData(obj);
                mysbs->addObject(obj);
                obj->autorelease();
                
            }
            CCLOG("敌人方英雄数据");
            
            int himsblength = sr.spiritshim_size();
            himsbs = CCArray::create();
            himsbs->retain();
            for (int i = 0; i< himsblength; i++)
            {
                main::FightSpirit fs = sr.spiritshim(i);
                SGSBObj *obj = new SGSBObj();
                obj->setSid(fs.sid());
                
                obj->setColorId(fs.color());
                obj->setType(fs.type());
                
                obj->setsbBaseDefendPlus(fs.defeatplus());
                
                obj->setsbBaseAttack(fs.baseattack());
                obj->setsbBaseDefend(fs.basedef());
                
                obj->setAp(fs.attack());
                obj->setDef(fs.def());
                
                obj->setResetInitDef(fs.basedef());
                obj->setDefRend(fs.walltype());
                obj->setRound(fs.round());
                obj->setIndex(gameIndex(fs.y(),fs.x()));
                obj->setSkillsId(fs.skillid());
                obj->setAfftributeNum(fs.value1());
                obj->setAfftributeNum1(fs.value2());
                obj->setAfftributeNum2(fs.randnum());
                obj->setBuffId(fs.buffid());
                obj->setBuffRound(fs.effectround());
                obj->setBuffAfftribute(fs.bvalue1());
                obj->setBuffType(fs.bufftype());
                obj->setAddDef(fs.wallceiling());
                obj->setAddAp(fs.perroundaddatk());
                obj->setChainNUm(fs.chainandmergeattackplus());
                obj->setRoundVAddDef(fs.wallgrow());
                obj->setAtkModeSkill(fs.atkmodeskill());
                
                if (fs.type() == ksbone) {
                    SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(fs.sid());
                    obj->setAtkmode(data->getSoldierAtkMode());
                    obj->setAtkMode2(data->getSoldierAtkMode2());
                    obj->setHeadId(data->getSoldierHead());
                    obj->setFileId(data->getSoldierFileId());
                    obj->setPair(data->getSoldierPair());
                    obj->setHitEff(data->getSoldierLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getSoldierName()->getCString());
                }else{
                    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(fs.sid());
                    obj->setAtkmode(data->getOfficerAtkMode());
                    obj->setAtkMode2(data->getOfficerAtkMode2());
                    obj->setHeadId(data->getOfficerHead());
                    obj->setFileId(data->getOfficerFileId());
                    obj->setPair(data->getOfficerPair());
                    obj->setHitEff(data->getOfficerLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getOfficerName()->getCString());
                    obj->setSkill_Head(data->getSkillHead());
                }
                logSbData(obj);
                himsbs->addObject(obj);
                obj->autorelease();
                
            }
            
            
            myHeroObj = new HeroObj();
            //myHeroObj->setRoleId(sr->roleid());
            myHeroObj->setHp(sr.youhp());
            myHeroObj->setBaseHp(sr.youbasehp());
            myHeroObj->setActionCount(sr.youactioncount());
            myHeroObj->setActionCount1(sr.youactioncountbyadd());
            myHeroObj->setItemId(sr.lordid());
            myHeroObj->setFriendLevel(sr.friendofficerlevel());
            myHeroObj->setFriendHeadId(sr.friendofficerid());
            myHeroObj->setName(CCString::create(sr.lordname()));
            myHeroObj->setSkillId(sr.lordskillbelongid());
            myHeroObj->setSkillRoundCur(sr.lordcdround());
            myHeroObj->setSkillRoundMax(sr.lordcdroundbase());
            myHeroObj->setMax_soldierNum(sr.maxfillunit());
            myHeroObj->setClientType(sr.clienttype());
            myHeroObj->setCountry(sr.nation());
            myHeroObj->setFriendRound(sr.friendofficercdround());
            myHeroObj->set_friendRound(sr.friendofficercdroundcurrent());
            myHeroObj->setLordSkillID(sr.lordskillid());
            myHeroObj->setAtkmode(sr.lordatkmode());
            
            
            enemyHeroObj = new HeroObj();
            //enemyHeroObj->setRoleId(sr.troleid());
            enemyHeroObj->setHp(sr.himhp());
            enemyHeroObj->setBaseHp(sr.himbasehp());
            enemyHeroObj->setActionCount(sr.himactioncount());
            enemyHeroObj->setActionCount1(sr.himactioncountbyadd());
            enemyHeroObj->setItemId(sr.tlordid());
            enemyHeroObj->setFriendLevel(sr.friendofficerlevel());
            enemyHeroObj->setFriendHeadId(sr.friendofficerid());
            enemyHeroObj->setName(CCString::create(sr.tlordname()));
            enemyHeroObj->setSkillId(sr.tlordskillbelongid());
            enemyHeroObj->setSkillRoundCur(sr.tlordcdroundbase());
            enemyHeroObj->setSkillRoundMax(sr.tlordcdroundbase());
            enemyHeroObj->setMax_soldierNum(sr.tmaxfillunit());
            enemyHeroObj->setClientType(sr.tclienttype());
            enemyHeroObj->setCountry(sr.tnation());
            enemyHeroObj->setLordSkillID(sr.tlordskillid());
            enemyHeroObj->setAtkmode(sr.tlordatkmode());
            
            SGMainManager::shareMain()->showCgBattleLayer(mainBattleLayer);
            mainBattleLayer->setIsMyRound(sr.isfirstattack());
            mainBattleLayer->changeColorSp();
            
            SGAIManager::shareManager()->isPVE = true;
            
            pveLoadMoveFinish = false;
            mainBattleLayer->addEffectLayer();
            
            mainBattleLayer->showBattleId(sr.fid());
            mainBattleLayer->m_battleId =  sr.fid();

            mainBattleLayer->setSpeed(sr.speed(), sr.tspeed());
            this->initHisMap();
            this->initMyMap();
            
            SGGuideManager::shareManager()->startAutoGuide(1);
            SGBattleManager::sharedBattle()->setIsBattlePve(true);
            //播放音乐
            MUSIC_PLAY(MUSIC_BOSS);
            EFFECT_PLAY(MUSIC_24);
            
        }
    }
}

void SGBattleManager::matchListener1(cocos2d::CCObject *obj)
{
//    int a;
//    a = 0;
}

void SGBattleManager::initMyMap()
{
    mainBattleLayer->initMap(myHeroObj, mysbs, true);
    myHeroObj->autorelease();
   // mysbs->release();      //removed by cgp
}
void SGBattleManager::initHisMap()
{
    mainBattleLayer->initMap(enemyHeroObj, himsbs, false);
    enemyHeroObj->autorelease();
   // himsbs->release();     //
}
#pragma mark - 匹配
void SGBattleManager::matchRequest()
{
    CCLOG("matchRequest");
    main::StartRequest *sr = new main::StartRequest();
    sr->set_enemyroleid(10);
    SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_MATCH, sr);
}
void SGBattleManager::matchListener(cocos2d::CCObject *obj)
{
    CCLOG("matchListener");
    
 
    SGAIManager::shareManager()->isPVE=false;
    SGAIManager::shareManager()->isInitPve = false;
 
    
    SGSocketRequest *socketRequest = (SGSocketRequest *)obj;
    if (socketRequest)
    {
        main::StartResponse *sr = (main::StartResponse *)socketRequest->m_msg;
        
        if (sr && sr->state() == 1)
        {
            closeReceive();
            
            //获取随即地图
            CCString *randmapIdStr = CCString::createWithFormat("background_%d.png",sr->mapid());
            mainBattleLayer = SGBattleLayer::create(randmapIdStr->getCString());
            SGMainManager::shareMain()->showBattleLayer(mainBattleLayer);
            mainBattleLayer->setIsMyRound(sr->isfirstattack());
//            mainBattleLayer->showColorSp();
            mainBattleLayer->changeColorSp();

            CCLOG("自己方英雄数据");
            int mysblength = sr->spiritsyou_size();
            CCArray *mysbs = CCArray::create();
            
            for (int i = 0; i< mysblength; i++)
            {
                main::FightSpirit fs = sr->spiritsyou(i);
                SGSBObj *obj = new SGSBObj();
                obj->setSid(fs.sid());
                
                obj->setColorId(fs.color());
                obj->setType(fs.type());
                obj->setIndex(gameIndex(fs.y(),fs.x()));
                
                obj->setsbBaseDefendPlus(fs.defeatplus());
                
                obj->setsbBaseAttack(fs.baseattack());
                obj->setsbBaseDefend(fs.basedef());
                
                obj->setAp(fs.attack());
                obj->setDef(fs.def());
                
                obj->setResetInitDef(fs.basedef());
                obj->setDefRend(fs.walltype());
                obj->setRound(fs.round());
                obj->setSkillsId(fs.skillid());
                obj->setAfftributeNum(fs.value1());
                obj->setAfftributeNum1(fs.value2());
                obj->setAfftributeNum2(fs.randnum());
                obj->setBuffId(fs.buffid());
                obj->setBuffRound(fs.effectround());
                obj->setBuffAfftribute(fs.bvalue1());
                obj->setBuffType(fs.bufftype());
                obj->setEffectType(fs.effect());
                obj->setAddDef(fs.wallceiling());
                obj->setAddAp(fs.perroundaddatk());
                obj->setChainNUm(fs.chainandmergeattackplus());
                obj->setRoundVAddDef(fs.wallgrow());
                obj->setAtkModeSkill(fs.atkmodeskill());
                obj->setsbSkLevel(1);
                if (obj->getIndex().i < 0 || obj->getIndex().i >= mapRow || obj->getIndex().j < 0 || obj->getIndex().j >= mapList) {
                    CCString *str = CCString::createWithFormat(str_MainLayer_str38, obj->getIndex().i, obj->getIndex().j);
                    CCNative::createAlert(str_MainLayer_str39, str->getCString(), NULL);
                    CCNative::showAlert();
                }
                
                if (fs.type() == ksbone)
                {
                    SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(fs.sid());
                    obj->setAtkmode(data->getSoldierAtkMode());
                    obj->setAtkMode2(data->getSoldierAtkMode2());
                    obj->setHeadId(data->getSoldierHead());
                    obj->setFileId(data->getSoldierFileId());
                    obj->setPair(data->getSoldierPair());
                    obj->setHitEff(data->getSoldierLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getSoldierName()->getCString());
                    //                    obj->setSkillsId(data->getSkill());
                }
                else
                {
                    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(fs.sid());
                    obj->setAtkmode(data->getOfficerAtkMode());
                    obj->setAtkMode2(data->getOfficerAtkMode2());
                    obj->setHeadId(data->getOfficerHead());
                    obj->setFileId(data->getOfficerFileId());
                    obj->setPair(data->getOfficerPair());
                    obj->setHitEff(data->getOfficerLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getOfficerName()->getCString());
                    obj->setSkill_Head(data->getSkillHead());
                    //                    obj->setSkillsId(data->getSkill());
                }
                logSbData(obj);
                mysbs->addObject(obj);
                obj->autorelease();
                
            }
            
            CCLOG("敌人方英雄数据");
            
            int himsblength = sr->spiritshim_size();
            CCArray *himsbs = CCArray::create();
            himsbs->retain();
			
            for (int i = 0; i< himsblength; i++)
            {
                main::FightSpirit fs = sr->spiritshim(i);
                SGSBObj *obj = new SGSBObj();
                obj->setSid(fs.sid());
                
                obj->setColorId(fs.color());
                obj->setType(fs.type());
                
                obj->setsbBaseDefendPlus(fs.defeatplus());
                
                obj->setsbBaseAttack(fs.baseattack());
                obj->setsbBaseDefend(fs.basedef());
                
                obj->setAp(fs.attack());
                obj->setDef(fs.def());
                
                obj->setResetInitDef(fs.basedef());
                obj->setDefRend(fs.walltype());
                obj->setRound(fs.round());
                obj->setIndex(gameIndex(fs.y(),fs.x()));
                obj->setSkillsId(fs.skillid());
                obj->setAfftributeNum(fs.value1());
                obj->setAfftributeNum1(fs.value2());
                obj->setAfftributeNum2(fs.randnum());
                obj->setBuffId(fs.buffid());
                obj->setBuffRound(fs.effectround());
                obj->setBuffAfftribute(fs.bvalue1());
                obj->setBuffType(fs.bufftype());
                obj->setEffectType(fs.effect());
                obj->setAddDef(fs.wallceiling());
                obj->setAddAp(fs.perroundaddatk());
                obj->setChainNUm(fs.chainandmergeattackplus());
                obj->setRoundVAddDef(fs.wallgrow());
                obj->setAtkModeSkill(fs.atkmodeskill());
                obj->setsbSkLevel(1);
                if (obj->getIndex().i < 0 || obj->getIndex().i >= mapRow || obj->getIndex().j < 0 || obj->getIndex().j >= mapList) {
                    CCString *str = CCString::createWithFormat(str_MainLayer_str38, obj->getIndex().i, obj->getIndex().j);
                    CCNative::createAlert(str_MainLayer_str39, str->getCString(), NULL);
                    CCNative::showAlert();
                }
                if (fs.type() == ksbone)
                {
                    SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(fs.sid());
                    obj->setAtkmode(data->getSoldierAtkMode());
                    obj->setAtkMode2(data->getSoldierAtkMode2());
                    obj->setHeadId(data->getSoldierHead());
                    obj->setFileId(data->getSoldierFileId());
                    obj->setPair(data->getSoldierPair());
                    obj->setHitEff(data->getSoldierLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getSoldierName()->getCString());
                    //                    obj->setSkillsId(data->getSkill());
                }
                else
                {
                    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(fs.sid());
                    obj->setAtkmode(data->getOfficerAtkMode());
                    obj->setAtkMode2(data->getOfficerAtkMode2());
                    obj->setHeadId(data->getOfficerHead());
                    obj->setFileId(data->getOfficerFileId());
                    obj->setPair(data->getOfficerPair());
                    obj->setHitEff(data->getOfficerLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getOfficerName()->getCString());
                    obj->setSkill_Head(data->getSkillHead());
                    //                    obj->setSkillsId(data->getSkill());
                }
                logSbData(obj);
                himsbs->addObject(obj);
                obj->autorelease();
                
            }

            HeroObj *myHeroObj = new HeroObj();
            myHeroObj->setRoleId(sr->roleid());
            myHeroObj->setHp(sr->youhp());
           // CCLOG("%d", sr->youhp());
           // CCLOG("%d", sr->youbasehp());
            myHeroObj->setBaseHp(sr->youbasehp());
            myHeroObj->setActionCount(sr->youactioncount());
            myHeroObj->setActionCount1(sr->youactioncountbyadd());
            myHeroObj->setItemId(sr->lordid());
            myHeroObj->setName(CCString::create(sr->lordname()));
            myHeroObj->setSkillId(sr->lordskillbelongid());
            myHeroObj->setSkillRoundCur(sr->lordcdround());
            myHeroObj->setSkillRoundMax(sr->lordcdround());
            myHeroObj->setClientType(sr->clienttype());
            myHeroObj->setAtkmode(sr->lordatkmode());
            myHeroObj->setCountry(sr->nation());
            myHeroObj->setMax_soldierNum(sr->maxfillunit());
            myHeroObj->setLordSkillID(sr->lordskillid());
            myHeroObj->setActionTime(sr->actiontime());
            
            HeroObj *enemyHeroObj = new HeroObj();
            enemyHeroObj->setRoleId(sr->troleid());
            enemyHeroObj->setHp(sr->himhp());
            enemyHeroObj->setBaseHp(sr->himbasehp());
            enemyHeroObj->setActionCount(sr->himactioncount());
            enemyHeroObj->setActionCount1(sr->himactioncountbyadd());
            enemyHeroObj->setItemId(sr->tlordid());
            enemyHeroObj->setName(CCString::create(sr->tlordname()));
            enemyHeroObj->setSkillId(sr->tlordskillbelongid());
            enemyHeroObj->setSkillRoundCur(sr->tlordcdround());
            enemyHeroObj->setSkillRoundMax(sr->tlordcdround());
            enemyHeroObj->setAtkmode(sr->tlordatkmode());
            enemyHeroObj->setCountry(sr->tnation());
            enemyHeroObj->setMax_soldierNum(sr->tmaxfillunit());
            enemyHeroObj->setLordSkillID(sr->tlordskillid());
            enemyHeroObj->setActionTime(sr->tactiontime());
            
            mainBattleLayer->setSpeed(sr->speed(), sr->tspeed());
            
            if (sr->isfirstattack())
            {
                enemyHeroObj->setActionCount1(0);
            }
            else
            {
                myHeroObj->setActionCount1(0);
            }
            
            
            mainBattleLayer->initMap(enemyHeroObj, himsbs, false);
            mainBattleLayer->initMap(myHeroObj, mysbs, true);
            
            mainBattleLayer->addEffectLayer();
            myHeroObj->autorelease();
            enemyHeroObj->autorelease();
            
            mainBattleLayer->showBattleId(sr->fid());
            
           // showFirstActionLayer(sr->speed(),sr->tspeed());
            
            mainBattleLayer->m_battleId =  sr->fid();
            
            MUSIC_PLAY(MUSIC_PVP);
            EFFECT_PLAY(MUSIC_24);
            
        }
    }
    //    CC_SAFE_RELEASE(socketRequest);
    
    if (SGAIManager::shareManager()->isLostCon==true ||
        SGPlayerInfo::sharePlayerInfo()->getBattleType() == 1) {
        SGAIManager::shareManager()->isPVE = true;
    }
        
    SGAIManager::shareManager()->isBattleOver=true;
    
    CCLog("######****111***######");
    showWaiteBox();
}
void SGBattleManager::showWaiteBox()
{
   // SGShadeBox2 * shade2 = SGShadeBox2::create(this,0);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGShadeBox2 * basebox = SGShadeBox2::create(mainScene,1);
    basebox->setTag(1315);
    SGMainManager::shareMain()->showBox(basebox);
    main::PvPEntryMapRequest *msg = new main::PvPEntryMapRequest();
    
    SGSocketClient::sharedSocketClient()->send(MSG_PVP_WAITE, msg, false);
    bool ismy =  mainBattleLayer->getIsMyRound();
    if (!SGAIManager::shareManager()->isPVE ||
        ismy) {
        SGHeroLayer *hero = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_myHeroLayerTag);
        hero->setRoundTime(-10);
        
        
        SGHeroLayer *enemyHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_enemyHeroLayerTag);
        CCLOG("enemyHeroLayer->getInitRoundNum()==%d",enemyHeroLayer->getInitRoundNum());
        enemyHeroLayer->setRoundNum(enemyHeroLayer->getInitRoundNum());
    }
    
}
void SGBattleManager::PveCancelWaiteListener(cocos2d::CCObject *obj)
{
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGShadeBox2 * basebox = (SGShadeBox2 * )mainScene->getChildByTag(1315);
    if(basebox)
    {
        basebox->resetMainTouch(true);
        basebox->removeFromParentAndCleanup(true);
    }
    bool ismy =  mainBattleLayer->getIsMyRound();
    if (!SGAIManager::shareManager()->isPVE ||
        ismy) {
        SGHeroLayer *hero = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_myHeroLayerTag);
        hero->setRoundTime(50);
        
        
        SGHeroLayer *enemyHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_enemyHeroLayerTag);
        CCLOG("enemyHeroLayer->getInitRoundNum()==%d",enemyHeroLayer->getInitRoundNum());
        enemyHeroLayer->setRoundNum(enemyHeroLayer->getInitRoundNum());
    }
    
}
#pragma mark - 移动
void SGBattleManager::moveRequest(int fromx, int tox)
{
    if (!m_isBattlePve) {
        mainBattleLayer->setOperationName(str_MainLayer_str40);
        
        SGBattleManager::sharedBattle()->fightLogRequest(str_MainLayer_str41);
        
        main::MoveRequest *msg = new main::MoveRequest();
        msg->set_x(fromx);
        msg->set_tox(tox);
        
        SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_MOVE, msg, false);
    }
}

void SGBattleManager::moveListener(CCObject *obj)
{
    CCLOG("接收移动消息成功！！");
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        MoveResponse *msg = (MoveResponse *)sr->m_msg;
        if (msg && SGNotificationCenter::sharedNotificationCenter()->isPostData)
        {
            closeReceive();
            SGHeroLayer *heroLayer = (SGHeroLayer*)mainBattleLayer->getChildByTag(sg_enemyHeroLayerTag);
            SGBattleMap *enemyBattleMap = heroLayer->getBattleMap();
 
 
            CCLOG("移动列%d   目标列%d", msg->x(),msg->tox());
 
            CCString *str = CCString::createWithFormat(str_MainLayer_str42, msg->x(), msg->tox());
            SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
            enemyBattleMap->goalList = msg->tox();
            enemyBattleMap->initTouchGrid(msg->x());
            
        }
        else
        {
            //int a;
        }
    }
    else{
        CCLOG("move err");

    }
    //    CC_SAFE_RELEASE(sr);
}
#pragma mark - 删除
void SGBattleManager::deleteRequest(int x,int y)
{
    if (!m_isBattlePve)
    {
        mainBattleLayer->setOperationName(str_MainLayer_str43);
        SGBattleManager::sharedBattle()->fightLogRequest(str_MainLayer_str44);
        
        RemoveRequest *msg = new RemoveRequest();
        msg->set_x(x);
        msg->set_y(y);
        
        SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_DELETE, msg, false);
    }
}

void SGBattleManager::deleteListener(cocos2d::CCObject *obj)
{
    
    CCLOG("接收删除消息成功！！");
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        RemoveResponse *msg = (RemoveResponse *)sr->m_msg;
        if (msg && msg->state() == 1 &&
            SGNotificationCenter::sharedNotificationCenter()->isPostData)
        {
            closeReceive();
            SGHeroLayer *heroLayer = (SGHeroLayer*)mainBattleLayer->getChildByTag(sg_enemyHeroLayerTag);
            SGBattleMap *enemyBattleMap = heroLayer->getBattleMap();
            enemyBattleMap->deleteSbObj(msg->y(), msg->x());
            
        }
    }
    //    CC_SAFE_RELEASE(sr);
}
#pragma mark - 补兵

void SGBattleManager::clientFillHeroLoader(CCArray *sbs ,const int &list)
{
    SGHeroLayer *heroLayer = getHeroLayer(mainBattleLayer->getIsMyRound());
    SGBattleMap * battleMap = heroLayer->getBattleMap();
    
    int randList = list;
    int fillHeroPow = battleMap->getCanFillHeroPow();
    if (fillHeroPow <=0)
    {
        return;
    }

    SGSBObj *backUpObj = NULL;
    bool canfillOne = true;
    CCPoint filllHeroRow = ccp(-1,-1);
    sgShiBingType heroType = ksbone;
    int checkHeroCount = 0;
    int canUseColorId[3] = {0,1,2};
    while (fillHeroPow > 0)
    {
        //跳出死循环处理
        if (getBackUpHeroCount() <= 0)
        {
            //没武将添加了
            break;
        }
        if (checkHeroCount >= getBackUpHeroCount()) {
            //没合适的武将或位置添加了，这次就不加了
            break;
        }
        randList = (randList + 1) % mapList;
        filllHeroRow = ccp(-1,-1);
        canfillOne = true;
        checkHeroCount = 0;
        for (int addHeroIndex = 0; addHeroIndex < getBackUpHeroCount(); addHeroIndex++)
        {
            checkHeroCount++;
            
            heroType = getBackUpHeroTypeByIndex(addHeroIndex);
            
            if (heroType == ksbtwo)
            {
                filllHeroRow = battleMap->testFillTow(randList);
                if (filllHeroRow.y >= 0) {
                    
                    backUpObj = getBackUpHeroByIndex(addHeroIndex);
                    backUpObj->setIndex(gameIndex(filllHeroRow.x, filllHeroRow.y));
                    for (int i = 0; i<2; i++)
                    {
                        SGGridBase *grid = SGGridBase::createWithType(knormal);
                        
                        grid->setIndex(gameIndex(filllHeroRow.x + i, filllHeroRow.y));
                        canfillOne = battleMap->clientFillOne(grid);
                        if (!canfillOne) {
                            break;
                        }
                    }
                    if (canfillOne)
                    {
                        int fillColor = battleMap->getFillHeroColorId(&canUseColorId);
                        for (int i = 0; i<2; i++)
                        {
                            SGGridBase *grid = SGGridBase::createWithType(knormal);
                            
                            grid->setIndex(gameIndex(filllHeroRow.x + i, filllHeroRow.y));
                            grid->setSbType(ksbtwo);
                            grid->setSaveID(backUpObj->getSid());
                            grid->setSaveColorID(fillColor);
                            
                            battleMap->doclientFillOne(grid);

                        }
                        backUpObj->setColorId(fillColor);
                        sbs->addObject(backUpObj);
                        fillHeroPow -= 1;
                        break;//添加上，再继续添加
                    }
                    
                }
            }
            else if (heroType == ksbfour)
            {
                if (fillHeroPow < 2)
                {
                    continue;//还不能上4格，继续添加查找
                }
                filllHeroRow = battleMap->testFillFour(randList);
                if (filllHeroRow.y >= 0) {
                    
                    backUpObj = getBackUpHeroByIndex(addHeroIndex);
                    backUpObj->setIndex(gameIndex(filllHeroRow.x, filllHeroRow.y));
                    
                    for (int i = 0; i<2; i++)
                    {
                        for (int j = 0; j<2; j++)
                        {
                            SGGridBase *grid = SGGridBase::createWithType(knormal);
                            
                            grid->setIndex(gameIndex(filllHeroRow.x + i, filllHeroRow.y + j));
                            canfillOne = battleMap->clientFillOne(grid);
                            if (!canfillOne)
                            {
                                break;
                            }
                        }
                        if (!canfillOne)
                        {
                            break;
                        }
                    }
                    
                    if (canfillOne)
                    {
                        
                        int fillColor = battleMap->getFillHeroColorId(&canUseColorId);
                        
                        for (int i = 0; i<2; i++)
                        {
                            for (int j = 0; j<2; j++)
                            {
                                SGGridBase *grid = SGGridBase::createWithType(knormal);
                                
                                grid->setIndex(gameIndex(filllHeroRow.x + i, filllHeroRow.y + j));
                                grid->setSbType(ksbfour);
                                grid->setSaveID(backUpObj->getSid());
                                grid->setSaveColorID(fillColor);
                                
                                battleMap->doclientFillOne(grid);
                            }
                        }
 
                        backUpObj->setColorId(fillColor);
                        sbs->addObject(backUpObj);
                        fillHeroPow -= 2;
                        break;//添加上，再继续添加
                    }
                }
            }
            
        }
    }
}

void SGBattleManager::clientFillSbLoader(SGBattleMap * battleMap,CCArray *sbs,int fillUnitCont,const int& list,const int& color)
{
    
    int randColor = color > 2 ? 0 : color;
    int randList = list;
    int fillRow = -1;
    
    SGSBObj *obj = NULL;
    SGSBObj *backUpObj = NULL;
    
    while (sbs->count() < fillUnitCont)
    {
        randList = (randList + 1) % mapList;
        randColor = (randColor + 1) % 3 ;
        fillRow = -1;
        
        fillRow = battleMap->testFillOne(randList,randColor);
        if (fillRow >=0) {
            backUpObj = getBackUpSbById(randColor);
            if (backUpObj) {
                obj = new SGSBObj();
                
                obj->setSid(backUpObj->getSid());
                obj->setColorId(backUpObj->getColorId());
                obj->setType(backUpObj->getType());
                
                obj->setsbBaseDefendPlus(backUpObj->getsbBaseDefendPlus());
                
                obj->setsbBaseAttack(backUpObj->getsbBaseAttack());
                obj->setsbBaseDefend(backUpObj->getsbBaseDefend());
                
                obj->setAp(backUpObj->getAp());
                obj->setDef(backUpObj->getDef());
                
                obj->setResetInitDef(backUpObj->getResetInitDef());
                obj->setDefRend(backUpObj->getDefRend());
                obj->setRound(backUpObj->getRound());
                obj->setIndex(gameIndex(fillRow,randList));
                obj->setSkillsId(backUpObj->getSkillsId());
                obj->setAfftributeNum(backUpObj->getAfftributeNum());
                obj->setAfftributeNum1(backUpObj->getAfftributeNum1());
                obj->setAfftributeNum2(backUpObj->getAfftributeNum2());
                obj->setBuffId(backUpObj->getBuffId());
                obj->setBuffRound(backUpObj->getBuffRound());
                obj->setBuffAfftribute(backUpObj->getBuffAfftribute());
                obj->setAfftributeNum2(backUpObj->getAfftributeNum2());
                obj->setBuffType(backUpObj->getBuffType());
                obj->setEffectType(backUpObj->getEffectType());
                obj->setAddDef(backUpObj->getAddDef());
                obj->setAddAp(backUpObj->getAddAp());
                obj->setChainNUm(backUpObj->getChainNUm());
                obj->setRoundVAddDef(backUpObj->getRoundVAddDef());
                obj->setAtkModeSkill(backUpObj->getAtkModeSkill());
                obj->setFriendOff(backUpObj->getFriendOff());
                obj->setsbSkLevel(1);
                
                obj->setAtkmode(backUpObj->getAtkmode());
                obj->setAtkMode2(backUpObj->getAtkMode2());
                obj->setHeadId(backUpObj->getHeadId());
                obj->setFileId(backUpObj->getFileId());
                obj->setPair(backUpObj->getPair());
                obj->setHitEff(backUpObj->getHitEff());
                obj->setAttackEffect(backUpObj->getAttackEffect());
                obj->setDeathEffect(backUpObj->getDeathEffect());
                obj->setName(backUpObj->getName());
                
                SGGridBase *grid = SGGridBase::createWithType(knormal);
                grid->setIndex(obj->getIndex());
                grid->setSbType(ksbone);
                grid->setSaveID(obj->getSid());
                grid->setSaveColorID(randColor);
                
                if (battleMap->clientFillOne(grid))
                {
                    battleMap->doclientFillOne(grid);
                    sbs->addObject(obj);
                }
                obj->autorelease();
            }
        }
    }
}

bool SGBattleManager::clientFillUnitLoader(CCArray *sbs)
{
    SGHeroLayer *heroLayer = getHeroLayer(mainBattleLayer->getIsMyRound());
    SGBattleMap * battleMap = heroLayer->getBattleMap();
    battleMap->initFillUnitGrids();
    int fillUnitCont = heroLayer->getFreeSoldierNum();
    if (fillUnitCont <= 0) {
        return false;
    }
    
    cc_timeval psv;
    CCTime::gettimeofdayCocos2d(&psv, NULL);
    unsigned long int rand_seed = psv.tv_sec*1000 + psv.tv_usec/1000;
    srand(rand_seed);
    
    int randList = (int)(CCRANDOM_0_1()*(mapList-1));
    int randColor = (int)(CCRANDOM_0_1()*(2));

    sbs->removeAllObjects();

    clientFillHeroLoader(sbs,randList);
    
    clientFillSbLoader(battleMap,sbs,fillUnitCont,randList,randColor);
    
    return true;
}

void SGBattleManager::clientFillUnit()
{
    SGHeroLayer *heroLayer = getHeroLayer(mainBattleLayer->getIsMyRound());
    
    CCLOG("补兵");
    CCArray *sbs = CCArray::create();
    
    bool fillSuccess = clientFillUnitLoader(sbs);
    if (fillSuccess)
    {
        heroLayer->showEffectLayerCgp(Eff_faluo , true);
        if (heroLayer->isFriend) {
            EFFECT_PLAY(MUSIC_18);
        }else{
            EFFECT_PLAY(MUSIC_21);
        }
    }
    
    SGBattleMap *battleMap = heroLayer->getBattleMap();
    //if (mainBattleLayer->getIsMyRound() )
    {
        battleMap->isPlayerOperation = true;
        battleMap->isAddBingIn = true;
    }
    
    battleMap->initMapSBs(sbs);
    battleMap->chubing();
    
    heroLayer->changeSoldierNum(0);
    heroLayer->isFriend = false;

}

void SGBattleManager::startFillFriend()
{
    SGHeroLayer *heroLayer = getHeroLayer(mainBattleLayer->getIsMyRound());
    SGBattleMap * battleMap = heroLayer->getBattleMap();
    CCArray *sbs = CCArray::create();
    
    battleMap->initFillUnitGrids();
    cc_timeval psv;
    CCTime::gettimeofdayCocos2d(&psv, NULL);
    unsigned long int rand_seed = psv.tv_sec*1000 + psv.tv_usec/1000;
    srand(rand_seed);
    int randList = (int)(CCRANDOM_0_1()*(mapList-1));
    sbs->removeAllObjects();
    
    bool fillSuccess = false;

    SGSBObj *backUpObj = cooTempOfficer;
    bool canfillOne = true;
    CCPoint filllHeroRow = ccp(-1,-1);
    int friendColor[3] = {0,1,2};
    sgShiBingType heroType = (sgShiBingType)backUpObj->getType();
    if (heroType == ksbtwo)
    {
        filllHeroRow = battleMap->testFillTow(randList);
        if (filllHeroRow.y >= 0)
        {
            backUpObj->setIndex(gameIndex(filllHeroRow.x, filllHeroRow.y));
            for (int i = 0; i<2; i++)
            {
                SGGridBase *grid = SGGridBase::createWithType(knormal);
                
                grid->setIndex(gameIndex(filllHeroRow.x + i, filllHeroRow.y));
                canfillOne = battleMap->clientFillOne(grid);
                
                if (!canfillOne) {
                    break;
                }
            }
            if (canfillOne)
            {
                
                
                int fillColor = battleMap->getFillHeroColorId(&friendColor);
                for (int i = 0; i<2; i++)
                {
                    SGGridBase *grid = SGGridBase::createWithType(knormal);
                    
                    grid->setIndex(gameIndex(filllHeroRow.x + i, filllHeroRow.y));
                    grid->setSbType(ksbtwo);
                    grid->setSaveID(backUpObj->getSid());
                    grid->setSaveColorID(fillColor);
                    
                    battleMap->doclientFillOne(grid);
                }
                
                backUpObj->setColorId( fillColor);
                sbs->addObject(backUpObj);
                fillSuccess = true;
            }
            
        }
    }
    else if (heroType == ksbfour )
    {
        filllHeroRow = battleMap->testFillFour(randList);
        if (filllHeroRow.y >= 0) {
            backUpObj->setIndex(gameIndex(filllHeroRow.x, filllHeroRow.y));
            
            for (int i = 0; i<2; i++)
            {
                for (int j = 0; j<2; j++)
                {
                    SGGridBase *grid = SGGridBase::createWithType(knormal);
                    
                    grid->setIndex(gameIndex(filllHeroRow.x + i, filllHeroRow.y + j));
                    canfillOne = battleMap->clientFillOne(grid);
                    if (!canfillOne)
                    {
                        break;
                    }
                }
                if (!canfillOne)
                {
                    break;
                }
            }
            
            if (canfillOne)
            {
                int fillColor = battleMap->getFillHeroColorId(&friendColor);
                
                for (int i = 0; i<2; i++)
                {
                    for (int j = 0; j<2; j++)
                    {
                        SGGridBase *grid = SGGridBase::createWithType(knormal);
                        
                        grid->setIndex(gameIndex(filllHeroRow.x + i, filllHeroRow.y + j));
                        grid->setSbType(ksbfour);
                        grid->setSaveID(backUpObj->getSid());
                        grid->setSaveColorID(fillColor);
                        
                        battleMap->doclientFillOne(grid);
                    }
                }

                backUpObj->setColorId( fillColor);
                sbs->addObject(backUpObj);
                fillSuccess = true;
            }
        }
    }
    
    if (fillSuccess)
    {
        heroLayer->friendNum = 0;
        heroLayer->changeFriendNum(heroLayer->initFriendNum);
        heroLayer->isFriendOffDeath = false;
    
       heroLayer->showEffectLayerCgp(Eff_faluo , true);
        
        //if (mainBattleLayer->getIsMyRound() )
        {
            battleMap->isPlayerOperation = true;
            battleMap->isAddBingIn = true;
        }
        
        battleMap->initMapSBs(sbs);
        battleMap->chubing();
        
        heroLayer->isFriend = false;
        
        EFFECT_PLAY(MUSIC_18);
    }
    else
    {
        SGMainManager::shareMain()->showMessage(str_NoSpace_for_thecall);
    }
}

void SGBattleManager::clientfillFriend()
{
    if (cooTempOfficer == NULL) {
        return;
    }
    SGHeroLayer *heroLayer = getHeroLayer(mainBattleLayer->getIsMyRound());
    CCCallFunc* delayCall = CCCallFunc::create(this, callfunc_selector(SGBattleManager::startFillFriend));
    heroLayer->runAction(CCSequence::create(CCDelayTime::create(0.1), delayCall, NULL));
    
}


void SGBattleManager::fillUnitRequest()
{
    if (!m_isBattlePve)
    {
        main::FillUnitRequest *msg = new main::FillUnitRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_FILL, msg);
    }
    else
    {
        SGHeroLayer *heroLayer = getHeroLayer(mainBattleLayer->getIsMyRound());
        if (heroLayer->getFreeSoldierNum() > 0)
        {
            clientFillUnit();
        }
        else
        {
            CCLOG("没有士兵可以补充！");
        }
        
    }
}

void SGBattleManager::fillUnitListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::FillUnitResponse *msg = (main::FillUnitResponse *)sr->m_msg;
        if (msg &&
            msg->state() == 1 &&
            SGNotificationCenter::sharedNotificationCenter()->isPostData)
        {
            closeReceive();
            
            bool ismy = getBattleLayer()->getIsMyRound();
            SGHeroLayer *heroLayer = NULL;
            if (msg->isself() == 1 && ismy)
            {
                heroLayer = getHeroLayer(true);
            }else{
                heroLayer = getHeroLayer(false);
            }
           heroLayer->showEffectLayerCgp(Eff_faluo , true);
            
            
            if (heroLayer->isFriend) {
                EFFECT_PLAY(MUSIC_18);
            }else{
                EFFECT_PLAY(MUSIC_21);
            }
            
            CCLOG("补兵时英雄数据");
            CCArray *sbs = CCArray::create();
            int sbLength = msg->spirits_size();
            CCString *s = CCString::createWithFormat(str_MainLayer_str45, sbLength);
            SGBattleManager::sharedBattle()->fightLogRequest(s->getCString());
            for (int i = 0; i < sbLength; i++)
            {
                main::FightSpirit fs = msg->spirits(i);
                
                SGSBObj *obj = new SGSBObj();
                obj->setSid(fs.sid());
                
                obj->setColorId(fs.color());
                obj->setType(fs.type());
                
                obj->setsbBaseDefendPlus(fs.defeatplus());
                
                obj->setsbBaseAttack(fs.baseattack());
                obj->setsbBaseDefend(fs.basedef());
                
                obj->setAp(fs.attack());
                obj->setDef(fs.def());
                
                obj->setResetInitDef(fs.basedef());
                obj->setDefRend(fs.walltype());
                obj->setRound(fs.round());
                obj->setIndex(gameIndex(fs.y(),fs.x()));
                obj->setSkillsId(fs.skillid());
                obj->setAfftributeNum(fs.value1());
                obj->setAfftributeNum1(fs.value2());
                obj->setAfftributeNum2(fs.randnum());
                obj->setBuffId(fs.buffid());
                obj->setBuffRound(fs.effectround());
                obj->setBuffAfftribute(fs.bvalue1());
                obj->setAfftributeNum2(fs.randnum());
                obj->setBuffType(fs.bufftype());
                obj->setEffectType(fs.effect());
                obj->setAddDef(fs.wallceiling());
                obj->setAddAp(fs.perroundaddatk());
                obj->setChainNUm(fs.chainandmergeattackplus());
                obj->setRoundVAddDef(fs.wallgrow());
                obj->setAtkModeSkill(fs.atkmodeskill());
                obj->setFriendOff(heroLayer->isFriend);
                obj->setsbSkLevel(1);
                if (obj->getIndex().i < 0 || obj->getIndex().i >= mapRow || obj->getIndex().j < 0 || obj->getIndex().j >= mapList) {
                    CCString *str = CCString::createWithFormat(str_MainLayer_str38, obj->getIndex().i, obj->getIndex().j);
                    CCNative::createAlert(str_MainLayer_str39, str->getCString(), NULL);
                    CCNative::showAlert();
                }
                
                
                if (fs.type() == ksbone)
                {
                    SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(fs.sid());
                    obj->setAtkmode(data->getSoldierAtkMode());
                    obj->setAtkMode2(data->getSoldierAtkMode2());
                    obj->setHeadId(data->getSoldierHead());
                    obj->setFileId(data->getSoldierFileId());
                    obj->setPair(data->getSoldierPair());
                    obj->setHitEff(data->getSoldierLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getSoldierName()->getCString());
                    //                    obj->setSkillsId(data->getSkill());
                }
                else
                {
                    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(fs.sid());
                    obj->setAtkmode(data->getOfficerAtkMode());
                    obj->setAtkMode2(data->getOfficerAtkMode2());
                    obj->setHeadId(data->getOfficerHead());
                    obj->setFileId(data->getOfficerFileId());
                    obj->setPair(data->getOfficerPair());
                    obj->setHitEff(data->getOfficerLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getOfficerName()->getCString());
                    obj->setSkill_Head(data->getSkillHead());
                    //                    obj->setSkillsId(data->getSkill());
                }
                logSbData(obj);
                sbs->addObject(obj);
                obj->autorelease();
            }
            
            SGBattleMap *battleMap = heroLayer->getBattleMap();
            if (msg->isself() == 1 ) {
                battleMap->isPlayerOperation = true;
                battleMap->isAddBingIn = true;
            }
            
            
            
            battleMap->initMapSBs(sbs);
            battleMap->chubing();
            
            heroLayer->changeSoldierNum(0);
            
            heroLayer->isFriend = false;
        }
        else
        {
            CCLog("$这补兵失败");
            bool ismy = getBattleLayer()->getIsMyRound();
            SGHeroLayer *heroLayer = NULL;
            if (ismy)
            {
                heroLayer = getHeroLayer(true);
            }else{
                heroLayer = getHeroLayer(false);
            }
            
            if (SGAIManager::shareManager()->isPVE && !ismy) {
                heroLayer->aiEndRound();
            }
        }
    }
    else
    {
        CCLog("$这里补兵失败！");
        
        //ai补兵失败的话就结束战斗
        bool ismy = getBattleLayer()->getIsMyRound();
        SGHeroLayer *heroLayer = NULL;
        if (ismy)
        {
            heroLayer = getHeroLayer(true);
        }else{
            heroLayer = getHeroLayer(false);
        }
        
        if (SGAIManager::shareManager()->isPVE && !ismy) {
            heroLayer->aiEndRound();
        }
    }
   
        //    CC_SAFE_RELEASE(sr);
    
    //    SGBattleManager::sharedBattle()->getHeroLayer(true)->changRoundNum(-1);
}

void SGBattleManager::reviveRequest()
{
    main::FillUnitReviveRequest *msg = new main::FillUnitReviveRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_FillUnitRevive, msg);
}

void SGBattleManager::reviveResponse(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::FillUnitReviveResponse *msg = (main::FillUnitReviveResponse *)sr->m_msg;
        if (msg &&
            SGNotificationCenter::sharedNotificationCenter()->isPostData)
        {
            EFFECT_PLAY(MUSIC_21);
            closeReceive();
            int roleid = msg->roleid();
            
            SGHeroLayer *heroLayer = getHeroLayer(true);
            
            if (heroLayer->getRoleId() != roleid) {
                heroLayer = getHeroLayer(false);
            }
            heroLayer->showEffectLayerCgp(Eff_faluo , true);
            
            CCArray *sbs = CCArray::create();
            int sbLength = msg->spirits_size();
            
            for (int i = 0; i < sbLength; i++)
            {
                main::FightSpirit fs = msg->spirits(i);
                
                SGSBObj *obj = new SGSBObj();
                obj->setSid(fs.sid());
                
                obj->setColorId(fs.color());
                obj->setType(fs.type());
                
                obj->setsbBaseDefendPlus(fs.defeatplus());
                
                obj->setsbBaseAttack(fs.baseattack());
                obj->setsbBaseDefend(fs.basedef());
                
                obj->setAp(fs.attack());
                obj->setDef(fs.def());
                

                obj->setResetInitDef(fs.basedef());
                obj->setDefRend(fs.walltype());
                obj->setRound(fs.round());
                obj->setIndex(gameIndex(fs.y(),fs.x()));
                obj->setSkillsId(fs.skillid());
                obj->setAfftributeNum(fs.value1());
                obj->setAfftributeNum1(fs.value2());
                obj->setAfftributeNum2(fs.randnum());
                obj->setBuffId(fs.buffid());
                obj->setBuffRound(fs.effectround());
                obj->setBuffAfftribute(fs.bvalue1());
                obj->setAfftributeNum2(fs.randnum());
                obj->setBuffType(fs.bufftype());
                obj->setEffectType(fs.effect());
                obj->setAddDef(fs.wallceiling());
                obj->setAddAp(fs.perroundaddatk());
                obj->setChainNUm(fs.chainandmergeattackplus());
                obj->setRoundVAddDef(fs.wallgrow());
                obj->setAtkModeSkill(fs.atkmodeskill());
                obj->setFriendOff(heroLayer->isFriend);
                
                if (obj->getIndex().i < 0 || obj->getIndex().i >= mapRow || obj->getIndex().j < 0 || obj->getIndex().j >= mapList) {
                    CCString *str = CCString::createWithFormat(str_MainLayer_str38, obj->getIndex().i, obj->getIndex().j);
                    CCNative::createAlert(str_MainLayer_str39, str->getCString(), NULL);
                    CCNative::showAlert();
                }
                
                
                if (fs.type() == ksbone) {
                    SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(fs.sid());
                    obj->setAtkmode(data->getSoldierAtkMode());
                    obj->setAtkMode2(data->getSoldierAtkMode2());
                    obj->setHeadId(data->getSoldierHead());
                    obj->setFileId(data->getSoldierFileId());
                    obj->setPair(data->getSoldierPair());
                    obj->setHitEff(data->getSoldierLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getSoldierName()->getCString());
                    //                    obj->setSkillsId(data->getSkill());
                }else{
                    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(fs.sid());
                    obj->setAtkmode(data->getOfficerAtkMode());
                    obj->setAtkMode2(data->getOfficerAtkMode2());
                    obj->setHeadId(data->getOfficerHead());
                    obj->setFileId(data->getOfficerFileId());
                    obj->setPair(data->getOfficerPair());
                    obj->setHitEff(data->getOfficerLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getOfficerName()->getCString());
                    obj->setSkill_Head(data->getSkillHead());
                    //                    obj->setSkillsId(data->getSkill());
                }
                logSbData(obj);
                sbs->addObject(obj);
                obj->autorelease();
            }
            CCLOG("得到要补的兵的个数 :%d",sbLength);
            if (sbLength <= 0)
            {
                CCNative::createAlert("^ ^",str_MainLayer_str46, NULL);
                CCNative::addAlertButton(str_MainLayer_str47);
                CCNative::showAlert();
            }
            if (m_isBattlePve) {
                addPveBackHero(msg,false);
            }
            
            SGBattleMap *battleMap = heroLayer->getBattleMap();
            battleMap->isPlayerOperation = false;
            battleMap->initMapSBs(sbs);
            //            battleMap->reviveInitMap();
            battleMap->qzFillUnit(callfuncN_selector(SGBattleMap::removeJQ));
            heroLayer->setSoldierNum(0);
        }
        else
        {
            CCNative::createAlert("^ ^", str_MainLayer_str48, NULL);
            CCNative::addAlertButton(str_MainLayer_str49);
            CCNative::showAlert();
        }
    }
}

#pragma mark - 放弃回合

void SGBattleManager::clientSaveSbData(SGBattleMap * battleMap ,CCDictionary *totalSbDics,bool IsMe)
{
    CCObject *soldier = NULL;
    CCDictionary *sbDic = CCDictionary::create();
    ShiBing *shiBing = NULL;
    int index = 0;
    char sbData[512];
    for (int p = 0; p<mapRow; p++)
    {
        for (int q = 0; q<mapList; q++)
        {
            if (battleMap->myGrids[p][q] && battleMap->myGrids[p][q]->getSbType() == ksbone) {
                shiBing = battleMap->getShiBingWithGrid(battleMap->myGrids[p][q]);
                if (shiBing) {
                    sprintf(sbData, "%d - %d - %d - %d - %d  - %d - %d - %d - %f - %d - %d - %f - %f - %d - %d - %d - %f - %d - %f - %d - %f - %f - %d - %d - %d - %d - %d - %d - %f",
                            shiBing->getSbId(),shiBing->getSbColor(),(int)shiBing->getSbType(),shiBing->getSbindex().i,shiBing->getSbindex().j,
                            shiBing->getAp(),shiBing->getInitDef(),shiBing->getResetInitDef(),shiBing->getDefRend(),shiBing->getRoundV(),
                            shiBing->getSkillsId(),shiBing->getAfftributeNum(),shiBing->getAfftributeNum1(),shiBing->getAfftributeNum2(),
                            shiBing->getBuffId(),shiBing->getBuffRound(),shiBing->getBuffAfftribute(),shiBing->getBuffType(),shiBing->getAddDef(),shiBing->getAddAp(),
                            shiBing->getChainNUm(),shiBing->getRoundVAddDef(),shiBing->getAtkModeSkill(), shiBing->getEffectType(),
                            shiBing->getOnlyGrid()->getStype() == kdefend?1:0,shiBing->getDef(),shiBing->getsbBaseAttack(),shiBing->getsbBaseDefend(),shiBing->getsbDefeatPlus());
                    
                    std::string key(CCString::createWithFormat("sbData%d",index)->getCString());
                    sbDic->setObject(CCString::createWithFormat("%s",sbData), key);
                    
                    index++;
                }

            }
            
        }
    }
    
    CCARRAY_FOREACH(battleMap->getSoldiers(), soldier)
    {
        shiBing = (ShiBing *)soldier;
        if (shiBing->getSbType() != ksbone) {
            sprintf(sbData, "%d - %d - %d - %d - %d  - %d - %d - %d - %f - %d - %d - %f - %f - %d - %d - %d - %f - %d - %f - %d - %f - %f - %d - %d - %d - %d - %d - %d - %f",
                    shiBing->getSbId(),shiBing->getSbColor(),(int)shiBing->getSbType(),shiBing->getSbindex().i,shiBing->getSbindex().j,
                    shiBing->getAp(),shiBing->getInitDef(),shiBing->getResetInitDef(),shiBing->getDefRend(),shiBing->getRoundV(),
                    shiBing->getSkillsId(),shiBing->getAfftributeNum(),shiBing->getAfftributeNum1(),shiBing->getAfftributeNum2(),
                    shiBing->getBuffId(),shiBing->getBuffRound(),shiBing->getBuffAfftribute(),shiBing->getBuffType(),shiBing->getAddDef(),shiBing->getAddAp(),
                    shiBing->getChainNUm(),shiBing->getRoundVAddDef(),shiBing->getAtkModeSkill(), shiBing->getEffectType(),
                    shiBing->getOnlyGrid()->getStype() == kdefend?1:0,shiBing->getDef(),shiBing->getsbBaseAttack(),shiBing->getsbBaseDefend(),shiBing->getsbDefeatPlus());
            
            std::string key(CCString::createWithFormat("sbData%d",index)->getCString());
            sbDic->setObject(CCString::createWithFormat("%s",sbData), key);
            index++;
        }
    }
    
    totalSbDics->setObject(sbDic, IsMe ? MYSBSDIC :ENEMYSBSDIC);
}

void SGBattleManager::clientSaveAttackData(SGBattleMap * battleMap ,CCDictionary *totalSbDics,bool IsMe)
{
    
    CCObject *obj = NULL;
    
    CCDictionary * attackDic = CCDictionary::create();
    CCDictionary *data = NULL;
    CCDictionary *buffDic = NULL;
    CCDictionary *buffData = NULL;
    
    SGAttackList *attack = NULL;
    int index = 0;
    int buffCount = 0;
    SG_BuffBase* sgbuff = NULL;
    CCARRAY_FOREACH(battleMap->getAttackList(), obj)
    {
        data = CCDictionary::create();
        attack = (SGAttackList *)obj;
        
        data->setObject(CCString::createWithFormat("%d",attack->getAttackIndex().j), "DATA1");
        data->setObject(CCString::createWithFormat("%d",attack->getAttackIndex().i), "DATA2");
        data->setObject(CCString::createWithFormat("%d",attack->getRoundV() <= 0 ? 1 : attack->getRoundV()), "DATA3");
        data->setObject(CCString::createWithFormat("%d",attack->getAp()), "DATA4");
        data->setObject(CCString::createWithFormat("%d",attack->getAddChain()), "DATA5");
        data->setObject(CCString::createWithFormat("%d",attack->getFuseNum()), "DATA6");
        data->setObject(CCString::createWithFormat("%d",attack->damageNum), "DATA7");
        data->setObject(CCString::createWithFormat("%d",attack->getIsRevive() ? 1 : 0), "DATA8");
        data->setObject(CCString::createWithFormat("%d",attack->getReviveNum()), "DATA9");
        
        
        buffDic = CCDictionary::create();
        buffCount = attack->m_attackBuffObj->count();
        
        for (int buffIndex = 0; buffIndex < buffCount; buffIndex++)
        {
            buffData = CCDictionary::create();
            sgbuff = (SG_BuffBase*)(attack->m_attackBuffObj->objectAtIndex(buffIndex));
            buffData = CCDictionary::create();
            buffData->setObject(CCString::createWithFormat("%d",sgbuff->getBuffID()), "BuffId");
            buffData->setObject(CCString::createWithFormat("%d",sgbuff->m_buffType), "BuffType");
            buffData->setObject(CCString::createWithFormat("%d",sgbuff->getRoundNum()), "BuffRound");
            buffData->setObject(CCString::createWithFormat("%f",sgbuff->getAfftributeNum()), "BuffAfftribute");
            buffData->setObject(CCString::createWithFormat("%d",sgbuff->getEffectType()), "EffectType");
            
            std::string key(CCString::createWithFormat("buffData%d",buffIndex)->getCString());
            buffDic->setObject(buffData, key);
            
        }
        data->setObject(buffDic, ATTACKBUFFDIC);
        std::string key(CCString::createWithFormat("attackDic%d",index)->getCString());
        attackDic->setObject(data, key);
        index++;
    }
    
    totalSbDics->setObject(attackDic,IsMe ? MYATTACKDIC :ENEMYATTACKDIC);
}

void SGBattleManager::clientSaveHeroData(SGHeroLayer * heroLayer ,CCDictionary *totalSbDics,bool IsMe)
{
    char heroData[512];
    sprintf(heroData, "%d - %d - %d - %d - %d - %d",
            heroLayer->getMorale(),heroLayer->heroBloodMax,heroLayer->roundNum,heroLayer->getSkillsRoundNum(),heroLayer->friendNum,heroLayer->getFreeSoldierNum());

    totalSbDics->setObject(CCString::createWithFormat("%s",heroData), IsMe ? MYDataDIC:ENEMYDataDIC);
    
}


const char* SGBattleManager::getClientSaveBattleDataName()
{
    std::string filePath = CCFileUtils::sharedFileUtils()->getWritablePath();
    
    CCString *saveFileName = CCString::createWithFormat("%s%d%s",
                                                        CCFileUtils::sharedFileUtils()->getWritablePath().c_str(),
                                                        SGPlayerInfo::sharePlayerInfo()->getplayerID(),
                                                        CLIENTSAVEBATTLE);
    return saveFileName->getCString();
}



void SGBattleManager::writeDicData(CCDictionary *sbDic,SGSBObj *shiBing,const std::string key)
{
    if (shiBing) {
        char sbData[512];
        
        sprintf(sbData, "%d - %d - %d - %d - %d  - %d - %d - %d - %f - %d - %d - %f - %f - %d - %d - %d - %f - %d - %f - %d - %f - %f - %d - %d - %d - %f",
                shiBing->getSid(),shiBing->getColorId(),shiBing->getType(),shiBing->getIndex().i,shiBing->getIndex().j,
                shiBing->getAp(),shiBing->getDef(),shiBing->getResetInitDef(),shiBing->getDefRend(),shiBing->getRound(),
                shiBing->getSkillsId(),shiBing->getAfftributeNum(),shiBing->getAfftributeNum1(),shiBing->getAfftributeNum2(),
                shiBing->getBuffId(),shiBing->getBuffRound(),shiBing->getBuffAfftribute(),shiBing->getBuffType(),shiBing->getAddDef(),shiBing->getAddAp(),
                shiBing->getChainNUm(),shiBing->getRoundVAddDef(),shiBing->getAtkModeSkill(),shiBing->getsbBaseAttack(),shiBing->getsbBaseDefend(),shiBing->getsbBaseDefendPlus());
        
        sbDic->setObject(CCString::createWithFormat("%s",sbData), key);
    }
}

void SGBattleManager::clientSaveBackupSbs(CCDictionary *totalSbDics)
{
    CCObject *obj = NULL;
    int index = 0;
    
    CCDictionary *backupSbDic = CCDictionary::create();
    
    CCARRAY_FOREACH(myBackupsbs, obj)
    {
        std::string key(CCString::createWithFormat("backupSbData%d",index)->getCString());
        writeDicData(backupSbDic,(SGSBObj *)obj ,key );
        index++;
    }
    
    CCARRAY_FOREACH(myBackupHeros, obj)
    {
        std::string key(CCString::createWithFormat("backupSbData%d",index)->getCString());
        writeDicData(backupSbDic,(SGSBObj *)obj ,key );
        index++;
    }
    totalSbDics->setObject(backupSbDic, MYBACKUPSBDIC);

    /////////
    index = 0;
    backupSbDic = CCDictionary::create();
    CCARRAY_FOREACH(himBackupSbs, obj)
    {
        std::string key(CCString::createWithFormat("backupSbData%d",index)->getCString());
        writeDicData(backupSbDic,(SGSBObj *)obj ,key );
        index++;
    }
    
    CCARRAY_FOREACH(himBackupHeros, obj)
    {
        std::string key(CCString::createWithFormat("backupSbData%d",index)->getCString());
        writeDicData(backupSbDic,(SGSBObj *)obj ,key );
        index++;
    }
    
    
    totalSbDics->setObject(backupSbDic, ENEMYBACKUPSBDIC);

    writeDicData(totalSbDics,cooTempOfficer ,FRIENDHERODATA);
    
}

void SGBattleManager::clientSaveBattleData(SGHeroLayer *myLayer ,SGHeroLayer* enemyLayer)
{
    //保存战斗数据，用于继续战斗
    SGBattleMap * myBattle = myLayer->getBattleMap();
    SGBattleMap * enemyBattle = enemyLayer->getBattleMap();
    
    CCDictionary *totalSbDics = CCDictionary::create();

    clientSaveSbData(myBattle,totalSbDics,true);
    clientSaveAttackData(myBattle,totalSbDics,true);
    clientSaveHeroData(myLayer,totalSbDics,true);
    
    clientSaveSbData(enemyBattle,totalSbDics,false);
    clientSaveAttackData(enemyBattle,totalSbDics,false);
    clientSaveHeroData(enemyLayer,totalSbDics,false);
    
    clientSaveBackupSbs(totalSbDics);
    
    totalSbDics->setObject(CCString::createWithFormat("%d",mainBattleLayer->getBattleCardNum()), TOTALDROPCOUNT);
    totalSbDics->setObject(CCString::createWithFormat("%s",mainBattleLayer->m_battleId.c_str()), BATTLESID);
    
    totalSbDics->writeToFile(getClientSaveBattleDataName());
}

void SGBattleManager::giveupRequest()
{
    mainBattleLayer->setOperationName(str_MainLayer_str50);
    
    SGHeroLayer *h = getHeroLayer(true);
    SGHeroLayer *e = getHeroLayer(false);
    h->isRoundEnd = false;
    e->isRoundEnd = false;
    if (!h->isInitPve) {
        
        if (h->isBattleOver || e->isBattleOver) {
            
        }else{
            SGMainManager::shareMain()->closeBox();
        }
    }
    
    bool ismy =  mainBattleLayer->getIsMyRound();
    
    if (ismy &&
        h->isInitPve) {
//        关闭
    }
    if (!SGAIManager::shareManager()->isPVE ||
        ismy) {
        SGHeroLayer *hero = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_myHeroLayerTag);
        hero->setRoundTime(-10);
        
        
        SGHeroLayer *enemyHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_enemyHeroLayerTag);
        CCLOG("enemyHeroLayer->getInitRoundNum()==%d",enemyHeroLayer->getInitRoundNum());
        enemyHeroLayer->setRoundNum(enemyHeroLayer->getInitRoundNum());
    }
    if (m_isBattlePve)
    {
        if (!ismy)
        {
             //AI放弃回合，需要设置回合数据
            CCDelayTime* delayActivate = CCDelayTime::create(0.1);
            CCCallFunc* delayCall = CCCallFunc::create(this, callfunc_selector(SGBattleManager::clientGiveUpHandler));
            h->runAction(CCSequence::create(delayActivate, delayCall, NULL));
        }
    }
    else
    {
        main::GiveUpRequest *msg = new main::GiveUpRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_GIVEUP, msg, false);
    }
    
    EFFECT_PLAY(MUSIC_1);
    
//    mainBattleLayer->showColorSp();
      mainBattleLayer->changeColorSp(false);
  
}
//void SGBattleManager::setHeroRoundData()
//{
//    SGHeroLayer *myHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_myHeroLayerTag);
//    SGHeroLayer *enemyHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_enemyHeroLayerTag);
//    myHeroLayer->isRoundEnd = false;
//    enemyHeroLayer->isRoundEnd = false;
//    
//    myHeroLayer->setRoundNum(myHeroLayer->getInitRoundNum());
//    
//    
//    myHeroLayer->upDAtaControlRound(-1);//msg->round()
//    
//    int skillNum = enemyHeroLayer->getSkillsRoundNum();//msg->skillresidueround();//enemyHero剩余的主将技回合数
//    
//    //下边的if注释是为了即时显示5回合血量判定胜负, 可能会有未知bug,目前没发现,2014-04-24
//    //if (!SGAIManager::shareManager()->isPVE)
//    {
//        enemyHeroLayer->giveupMessage(skillNum);
//    }
//}

#pragma mark "=================切换五回合消息监听========================="


int SGBattleManager::getFriendSid()
{
    if (mainBattleLayer->getIsMyRound() && cooTempOfficer)
    {
        return cooTempOfficer->getSid();
    }
    return -1;
}

int SGBattleManager::clientCheckControlRound()
{
    SGHeroLayer *myHeroLayer = getHeroLayer(true);
    SGHeroLayer *enemyHeroLayer = getHeroLayer(false);
    SGBattleMap *myBattle = myHeroLayer->getBattleMap();
    SGBattleMap *enemyBattle = enemyHeroLayer->getBattleMap();
    
    if ( himBackupHeros->count() <= 0
        && myBackupHeros->count() <= 0
        && enemyBattle->getHeroCountWithoutFriend() <=0
        && myBattle->getHeroCountWithoutFriend() <=0)
    {
        if (startFiveRound <0 ) {
            startFiveRound = 4;
        }
        else
        {
            startFiveRound--;
        }
        
        return startFiveRound;
    }
    return -1;
}

void SGBattleManager::clientGiveUpHandler()
{
    SGHeroLayer *myHeroLayer = getHeroLayer(true);
    SGHeroLayer *enemyHeroLayer = getHeroLayer(false);
    
    myHeroLayer->isRoundEnd = false;
    enemyHeroLayer->isRoundEnd = false;
    
    myHeroLayer->setRoundNum(myHeroLayer->getInitRoundNum());
    
    clientSaveBattleData(myHeroLayer,enemyHeroLayer);
    
    int fiveRound = this->clientCheckControlRound();
    if (fiveRound <= 3)
    {
         myHeroLayer->upDAtaControlRound(fiveRound);
    }
   
    
    int skillNum = enemyHeroLayer->getSkillsRoundNum();//msg->skillresidueround();//enemyHero剩余的主将技回合数
    
    //下边的if注释是为了即时显示5回合血量判定胜负
    //if (!SGAIManager::shareManager()->isPVE)
    {
        enemyHeroLayer->giveupMessage(skillNum);
    }
}


void SGBattleManager::giveUpListener(cocos2d::CCObject *obj)
{
    EFFECT_PLAY(MUSIC_1);
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        //特殊处理,如果此时battleMap有一个不存在,则返回.因为修改了战斗驱动逻辑,现在由服务器驱动,而此时由于新手引导的阻止,可能还未初始化battleMap,导致这里null失败.
        SGHeroLayer *myHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_myHeroLayerTag);
        SGHeroLayer *enemyHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_enemyHeroLayerTag);
        if (!myHeroLayer->battleMap || !enemyHeroLayer->battleMap)
        {
            return;
        }
        
        main::GiveUpResponse *msg = (main::GiveUpResponse *)sr->m_msg;
        mainBattleLayer->changeRound_needVerify(msg->state());
        if (msg && msg->state() == 1 && SGNotificationCenter::sharedNotificationCenter()->isPostData) //我方行动
        {
//            SGHeroLayer *myHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_myHeroLayerTag);
//            SGHeroLayer *enemyHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_enemyHeroLayerTag);
            myHeroLayer->isRoundEnd = false;
            enemyHeroLayer->isRoundEnd = false;
            
            myHeroLayer->setRoundNum(msg->actioncount());
            myHeroLayer->upDAtaControlRound(msg->round());
            
			int skillNum = msg->skillresidueround();//enemyHero剩余的主将技回合数
			
			//下边的if注释是为了即时显示5回合血量判定胜负, 可能会有未知bug,目前没发现,2014-04-24
            //if (!SGAIManager::shareManager()->isPVE)
            {
                enemyHeroLayer->giveupMessage(skillNum);
            }
        }
        else if (0 == msg->state()) //对方行动，只有AI PVP时候才会收到。
        {
//            SGHeroLayer *myHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_myHeroLayerTag);
//            SGHeroLayer *enemyHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_enemyHeroLayerTag);
            enemyHeroLayer->setRoundNum(msg->actioncount());
        }
        else if (2 == msg->state()) //对方卡死后的修补。
        {
            enemyHeroLayer->aiEndRound();
        }
    }
}

#pragma mark - 结束
void SGBattleManager::gameoverRequet()
{
    main::LogoutRequest *msg = new main::LogoutRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_OVER, msg);
}

void SGBattleManager::gameoverListener(cocos2d::CCObject *obj)
{
    
    CCLOG("gameoverListener");
}

#pragma mark - 验证
void SGBattleManager::validationRequet(cocos2d::CCArray *array)
{
    if (mainBattleLayer->getIsMyRound() ||
        SGAIManager::shareManager()->isPVE) {
        main::VerifyRequest *msg = new main::VerifyRequest();
        for (int i = 0; i < array->count(); i++) {
//            CCLOG("==== %d",atoi(((CCString*)array->objectAtIndex(i))->getCString()));
            msg->add_check(atoi(((CCString*)array->objectAtIndex(i))->getCString()));
        }
        //主要是用来控制倒计时。
        SGAIManager::shareManager()->m_isVarifyActiviteAI = false;
        //MMDBG: BATTLE FIX 测试，战斗消息需要强制配对。
        SGSocketClient::sharedSocketClient()->send(MSG_VALIDATION, msg, true, true, BATTLE_DELAY_SHOW_TIME);
    }
} 

void SGBattleManager::validationListener(cocos2d::CCObject *obj)
{
    //主要是用来控制倒计时。
    SGAIManager::shareManager()->m_isVarifyActiviteAI = true;
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::VerifyResponse *msg = (main::VerifyResponse *)sr->m_msg;
        if (msg && SGNotificationCenter::sharedNotificationCenter()->isPostData)
        {
            if (msg->result() == 0)
            {
                if (SGAIManager::shareManager()->isPVE)
                {
                    
                    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
                }
                
//                getBattleLayer()->setStopAi(true);

#if (NEEDALERT==1)
                CCNative::createAlert("^ ^", str_MainLayer_str51, NULL);
                CCNative::addAlertButton(str_MainLayer_str52);
                CCNative::showAlert(this);
#endif
                SGHeroLayer *hero = getHeroLayer(true);
                hero->endRoundUpdataTime();
                
#if (NEEDALERT != 1)
                fightFixMapRequest();        
#endif
            }
            else
            {
                ;
            }
        }
    }
}

void SGBattleManager::alertViewClickedButtonAtIndex(int buttonIndex)
{
    CCRenderTexture* rt = CCRenderTexture::create(screenwidth, screenheight);
    rt->begin();
    SGBattleManager::sharedBattle()->getBattleLayer()->visit();
    rt->end();
    rt->saveToFile("sanGuoBattlephoto.png", kCCImageFormatPNG);
    GameConfig::severImage();
    fightFixMapRequest();
}

SGBattleLayer *SGBattleManager::getBattleLayer()
{
    return mainBattleLayer;
}

SGBattleMap *SGBattleManager::getBattleMap(bool enemy)
{
    if (mainBattleLayer)
    {
        BattleLayerTag layerTag;
        if (enemy)
        {
            layerTag = mainBattleLayer->getIsMyRound()?sg_enemyHeroLayerTag:sg_myHeroLayerTag;
        }
        else
        {
            layerTag = mainBattleLayer->getIsMyRound()?sg_myHeroLayerTag:sg_enemyHeroLayerTag;
        }
        
        SGHeroLayer *enemyHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(layerTag);
        
        return enemyHeroLayer->getBattleMap();
    }
    return NULL;
}

SGHeroLayer *SGBattleManager::getHeroLayer(bool isMyLayer)
{
    if (mainBattleLayer)
    {
        BattleLayerTag ltag = isMyLayer?sg_myHeroLayerTag:sg_enemyHeroLayerTag;
        return (SGHeroLayer *)mainBattleLayer->getChildByTag(ltag);
    }
    return NULL;
}

void SGBattleManager::guideBattleOver()
{
    //    SGGuideManager::shareManager()->step++;
    
    if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
    {
//        if (SGGuideManager::shareManager()->isGuide)
//        {
//            SGMainManager::preProcessDataInFightSceneForSend(4); // 闯关结束
//        }

        SGGuideManager::shareManager()->startFightGuide(6);
    }
    else{
		CCLOG("Sixth Guide in [%s-%d]", __FUNCTION__, __LINE__);

        SGGuideManager::shareManager()->startAutoGuide(6);
    }
    
}

void SGBattleManager::battleOver()
{
    
    CCLOG("isGuide = %d", SGGuideManager::shareManager()->isGuide );
    if (SGGuideManager::shareManager()->isGuide)
    {
        SGMainManager::preProcessDataInFightSceneForSend(4); // 1-2 闯关结束
    }

    
    SGAIManager::shareManager()->isBattleOver = false;
   
    if (_battleType == BT_ADVENTURE) //闯关/副本走900
//    if (m_isBattlePve)
    {
        main::FightThtoughRewardRequest *rewar = new main::FightThtoughRewardRequest;
        rewar->set_fighprossid( atoll((SGBattleManager::sharedBattle()->getBattleLayer()->m_battleId).c_str()) );
        
        int isMeWin = 0;
        if (getHeroLayer(true)->getMorale() > 0 && getHeroLayer(true)->getMorale() > getHeroLayer(false)->getMorale()) {
            isMeWin = 1;
        }
        rewar->set_iswin(isMeWin);//1自己胜利，0自己失败
        rewar->set_currhp(getHeroLayer(true)->getMorale());
        SGSocketClient::sharedSocketClient()->send( MSG_STORY_CLIENTBATTLE_OVER, rewar);
        
        SGBattleManager::sharedBattle()->setIsBattlePve(false);
    }
    else if (_battleType == BT_SURVIVAL) //守荆州走902
    {
        main::FightRewardRequest* rewar = new main::FightRewardRequest();
        rewar->set_fighprossid(SGBattleManager::sharedBattle()->getBattleLayer()->m_battleId);
        SGSocketClient::sharedSocketClient()->send( MSG_STORY_BATTLE_OVER, rewar);
        
        SGBattleManager::sharedBattle()->setIsBattlePve(false);

    }
    else if(_battleType == BT_ARENA)//竞技场
    {
        bool isMeWin = false;
        if (getHeroLayer(true)->getMorale() > 0 && getHeroLayer(true)->getMorale() > getHeroLayer(false)->getMorale()) {
            isMeWin = true;
        }
        main::ArenaEndFightRequest  * req = new main::ArenaEndFightRequest();
        req->set_iswin(isMeWin);
        req->set_quick(false);
        SGSocketClient::sharedSocketClient()->send( MSG_ARENA_END_GIGHT, req);
    }
    //pvp pvp战斗奖励消息更改为1510.
    else
    {
//        main::FightRewardRequest *rewar = new main::FightRewardRequest;
//        rewar->set_fighprossid(SGBattleManager::sharedBattle()->getBattleLayer()->m_battleId);
//        
//        SGSocketClient::sharedSocketClient()->send( MSG_STORY_BATTLE_OVER, rewar);
        
        CCLog("\n\n\n\n zyc debug for pvp...战斗正常结束...%d\n%s" , __LINE__ , __FILE__);
        CCLog("战斗类型%d" , _battleType);
        
        main::FightPvpRewardRequest *res = new main::FightPvpRewardRequest();
        res->set_fightid(atoll( SGBattleManager::sharedBattle()->getBattleLayer()->m_battleId.c_str() ) );
        SGSocketClient::sharedSocketClient()->send(MSG_PVP_FIGHT_REWARD, res );
    }
    
    getBattleMap(false)->hideFriendlyHelp(false);
    getBattleMap(false)->getHero_Obj()->getMenu()->setEnabled(false);
    getBattleMap(false)->getHero_Obj()->autoMenu->setEnabled(false);
}

void SGBattleManager::overSettlement(cocos2d::CCObject *obj)
{
    
    SGHeroLayer *hero = getHeroLayer(true);
    SGAIManager::shareManager()->isBattleOver = false;
    getBattleLayer()->setIsOver(true);
    SGSocketRequest *req = (SGSocketRequest *)obj;
    main::FightRewardResponse *response = (main::FightRewardResponse *)req->m_msg;
    SGPlayerInfo *playInfo = SGPlayerInfo::sharePlayerInfo();
    int _plotId = response->plotid();
    
    //////////////////设置守荆州有没有下一关
    
    playInfo->setismaxlevel(response->ismaxlevel());
    
    /////////////////
    
    CCLOG("response->state()==%d",response->state());
    if (response && response->state()==1)
    {
        int jexp = response->meritorious();
        int var1 = response->rating();
        int var2 = response->exp();
        int var3 = response->coins();
		int soul = response->gold();
		int curHp = response->currhp();
		int maxHp = response->basehp();
        bool isShowExoticMerchant = (0 < response->xiyu());
        
        CCArray *var4 = CCArray::create();
        SGStoryReward *reward = SGStoryReward::create(var1, var2, var3, jexp, soul, curHp, maxHp, var4);
        reward->setExtExp(response->addexp());
        reward->setGold(response->gold());
        reward->setHpRate(response->hprate());
        CCLOG("response->liandan()==%d",response->liandan());
        reward->setLianDan(response->liandan());
        
		reward->setBeforeLevel(response->oldlevel());
		
        int size = response->dropitems_size();
        SGBaseMilitaryCard *card = NULL;
        if (size > 0)// 先将卡片加入背包中
        {
            CCArray *rItems = reward->getItems();
            CCArray *rewardItem = CCArray::create();
            rewardItem ->retain();

            for (int ii = 0; ii < size; ii++)
            {
                bool isHave= false;
                SGStoryRdItem *rdModel = NULL;
                main::FightRewardDropItems item = response->dropitems(ii);
                if (item.officercards().itemid() > 0) {
                    int id = item.officercards().itemid();
                    SGOfficerDataModel *off = SGStaticDataManager::shareStatic()->getOfficerById(item.officercards().itemid());
                    //int tag = off->getOfficerRace();
                    int tag = off->getOfficerVisitType();
                    isHave = detectIsHaveItem(rewardItem,id);
                    if (!isHave)
                    {
                        rdModel = SGStoryRdItem::create(id, tag, 0);
                        rdModel->addCount();
                    }
                    //rdModel = SGStoryRdItem::create(id, tag, 0);
                    card = createOfficerCardWithInfo(item.officercards());
                    //playInfo->addOfficerCard((SGOfficerCard *)card);
                }else if (item.equipmentcards().itemid() > 0) {
                    int id = item.equipmentcards().itemid();
                    SGEquipmentDataModel *equ = SGStaticDataManager::shareStatic()->getEquipById(id);
                    //int tag = equ->getEquipType();
                    
                    ///
                    int size = rewardItem->count();
                    if(size==0)
                    {
                        isHave =  false;
                    }else
                    {
                        for(int i = 0; i < size; i++)
                        {
                            int nItemId =  id;
                            SGStoryRdItem* item_ = (SGStoryRdItem*)rewardItem->objectAtIndex(i);
                            int nItemid2 = item_->getItemId();
                            if(nItemId==nItemid2)
                            {
                                item_->setCount(item_->getCount()+1);
                                isHave =  true;
                                break;
                            }
                            
                        }
                        
                    }
                    
                    ///
                    
                    int tag = equ->getEquipVisitType();
                    //isHave = detectIsHaveItem(rewardItem,id);
                    if (!isHave)
                    {
                        rdModel = SGStoryRdItem::create(id, tag, 1);
                        rdModel->addCount();
                    }
                    
                }else if (item.propscards().itemid() > 0) {
                    int id = item.propscards().itemid();
                    ///
                    int size = rewardItem->count();
                    if(size==0)
                    {
                        isHave =  false;
                    }else
                    {
                        for(int i = 0; i < size; i++)
                        {
                            int nItemId =  id;
                            SGStoryRdItem* item_ = (SGStoryRdItem*)rewardItem->objectAtIndex(i);
                            int nItemid2 = item_->getItemId();
                            if(nItemId==nItemid2)
                            {
                                item_->setCount(item_->getCount()+item.propscards().curritemnumber());
                                isHave =  true;
                                break;
                            }
                            
                        }
                        
                    }
                    
                    ///
                    
                    //isHave = detectIsHaveItem(rewardItem,id);
                    if (!isHave)
                    {
                        rdModel = SGStoryRdItem::create(id, 0, 3);
                        rdModel->setCount(item.propscards().curritemnumber());
                    }
                    
                    
                }//加入碎片掉落处理
                else if (item.fragmentcards().itemid() > 0)
                {
                    int id = item.fragmentcards().itemid();
                    
                    ///
                    int size = rewardItem->count();
                    if(size==0)
                    {
                        isHave =  false;
                    }else
                    {
                        for(int i = 0; i < size; i++)
                        {
                            int nItemId =  id;
                            SGStoryRdItem* item_ = (SGStoryRdItem*)rewardItem->objectAtIndex(i);
                            int nItemid2 = item_->getItemId();
                            if(nItemId==nItemid2)
                            {
                                item_->setCount(item_->getCount()+item.fragmentcards().curritemnumber());
                                isHave =  true;
                                break;
                            }
                            
                        }
                        
                    }
                    
                    ///
                    
                    //isHave = detectIsHaveItem(rewardItem,id);
                    if (!isHave)
                    {
                        rdModel = SGStoryRdItem::create(id, 0, 5);
                        rdModel->setCount(item.fragmentcards().curritemnumber());
                    }
                    
                }
                //加入消耗品
                else if (item.consumablecards().itemid() > 0)
                {
                    //消耗品掉落
                    int id = item.consumablecards().itemid();
                    ///
                    int size = rewardItem->count();
                    if(size==0)
                    {
                        isHave =  false;
                    }else
                    {
                        for(int i = 0; i < size; i++)
                        {
                            int nItemId =  id;
                            SGStoryRdItem* item_ = (SGStoryRdItem*)rewardItem->objectAtIndex(i);
                            int nItemid2 = item_->getItemId();
                            if(nItemId==nItemid2)
                            {
                                item_->setCount(item_->getCount()+item.consumablecards().curritemnumber());
                                isHave =  true;
                                break;
                            }
                            
                        }
                        
                    }
                    if (!isHave)
                    {
                        rdModel = SGStoryRdItem::create(id, 0, 6);
                        rdModel->setCount(item.consumablecards().curritemnumber());
                    }
                    
                    
                }
                //材料
                else if (item.materialitemprotos().itemid() > 0)
                {
                    //iType 7
                    //材料掉落
                    int id = item.materialitemprotos().itemid();
                    ///
                    int size = rewardItem->count();
                    if(size==0)
                    {
                        isHave =  false;
                    }else
                    {
                        for(int i = 0; i < size; i++)
                        {
                            int nItemId =  id;
                            SGStoryRdItem* item_ = (SGStoryRdItem*)rewardItem->objectAtIndex(i);
                            int nItemid2 = item_->getItemId();
                            if(nItemId==nItemid2)
                            {
                                item_->setCount(item_->getCount()+item.materialitemprotos().itemnum());
                                isHave =  true;
                                break;
                            }
                            
                        }
                        
                    }
                    if (!isHave)
                    {
                        rdModel = SGStoryRdItem::create(id, 0, 7);
                        rdModel->setCount(item.materialitemprotos().itemnum());
                    }
                }
                //其他数据类型，这里先是勾玉
                else if (item.itemdata_size() > 0)
                {
                    main::ItemDataProto idp = item.itemdata(0);//目前只有勾玉
                    int type = idp.itemtype();
                    if (type == 18)
                    {
                        int id = -1;
                        ///
                        int size = rewardItem->count();
                        if(size==0)
                        {
                            isHave =  false;
                        }else
                        {
                            for(int i = 0; i < size; i++)
                            {
                                int nItemId =  id;
                                SGStoryRdItem* item_ = (SGStoryRdItem*)rewardItem->objectAtIndex(i);
                                int nItemid2 = item_->getItemId();
                                if(nItemId==nItemid2)
                                {
                                    item_->setCount(item_->getCount()+idp.itemnum());
                                    isHave =  true;
                                    break;
                                }
                                
                            }
                            
                        }
                        if (!isHave)
                        {
                            rdModel = SGStoryRdItem::create(id, 0, 18);
                            rdModel->setCount(idp.itemnum());
                        }

                    }
                }
                
                
                if (rdModel)
                {
                    if (!isHave)
                        rewardItem->addObject(rdModel);
                }
                
            }
            rItems->addObject(rewardItem);
        }
        else
        {
            CCArray *rItems = reward->getItems();
            CCArray *rewardItem = CCArray::create();
            rewardItem ->retain();
            rItems->addObject(rewardItem);
        }
        
        
        //只查看过关引导
        if (response->rating() > 0  )
        {
            
            //如果当前的plotid大于已有的最大的plotid，则设置当前plotid为最大的plotid
            SGPlayerInfo *pInfo = SGPlayerInfo::sharePlayerInfo();
            
            pInfo->setMaxPlotId(_plotId);
            pInfo->setMaxBossPlotId(_plotId);
            //关卡完成检测掉落
            SGGuideManager::shareManager()->setLimitGuideTag(SGPlayerInfo::sharePlayerInfo()->checkStartLimitGuide(2,_plotId));
        }

        SGPlayerInfo::sharePlayerInfo()->updateStoryData(_plotId, response->rating());
        SGOverBattleBox *box = SGOverBattleBox::create(hero, reward , 0 , isShowExoticMerchant);
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        mainScene->addChild(box);
        
        //这场战斗的bean
        SGMainManager::shareMain()->trackMsnFailed(_plotId, CCString::create("defeat"));
        
    }
    else if (response && response->state()!=1)
    {
        SGCantAdvanceBox *box = SGCantAdvanceBox::create(hero, NULL, 30, 0);
        SGMainManager::shareMain()->showBox(box);
    }
    
    //播放场景音乐
    MUSIC_PLAY(MUSIC_SCENE); 
     
}
bool SGBattleManager::detectIsHaveItem(CCArray *arr, int  nid)
{
    int size = arr->count();
    if(size==0)
        return false;
    for(int i = 0; i < size; i++)
    {
        int nItemId =  nid;
        SGStoryRdItem* item = (SGStoryRdItem*)arr->objectAtIndex(i);
        int nItemid2 = item->getItemId();
        if(nItemId==nItemid2)
        {
            item->addCount();
            return true;
        }
        
    }
    return  false;
}
void SGBattleManager::runAwayRequest()
{
    main::FightRunAwayRequest *runAway = new main::FightRunAwayRequest;
    SGSocketClient::sharedSocketClient()->send(MSG_RUNAWAY, runAway);
}

//战斗中点击投降
void SGBattleManager::runAwayResponse(cocos2d::CCObject *obj)
{
    GPCCLOG("战斗中点击投降\n");
    SGHeroLayer *hero = getHeroLayer(true);
    
    SGSocketRequest *ssc = (SGSocketRequest*)obj;
    FightRunAwayResponse *runAway = (FightRunAwayResponse*)ssc->m_msg;
    
    int _plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
    CCLOG("aaa==%d",_plotId);
    
    if (runAway && runAway->state() == 1)
    {
        BATTLE_TYPE btp = SGBattleManager::sharedBattle()->getBattleType();
        //pvp投降
        if (BT_LADDER == btp)  //天梯
        {
            CCLog("\n\n\n\n zyc debug for pvp 投降！！！！！...%d\n%s" , __LINE__ , __FILE__);
            main::FightPvpRewardRequest *res = new main::FightPvpRewardRequest();
            res->set_fightid(atoll( SGBattleManager::sharedBattle()->getBattleLayer()->m_battleId.c_str() ) );
            SGSocketClient::sharedSocketClient()->send(MSG_PVP_FIGHT_REWARD, res );        }
        else      //pve
        {
            int jexp = 0;
            int var1 = runAway->rating();
            int var2 = runAway->exp();
            int var3 = runAway->gold();
            int soul = 0;//逃跑没有军魂奖励
            int curHp = runAway->currhp();
            int maxHp = runAway->bashhp();
            
            int isRunAway = runAway->isrunaway();
            
            //FIXME: 暂时将当前剩余的exp值传入当做剩余的exp值，因为runAway->exp()永远是0，所以会一直显示为0。
            var2 = SGPlayerInfo::sharePlayerInfo()->getPlayerExp();
            CCArray *var4 = CCArray::create();
            SGStoryReward *reward = SGStoryReward::create(var1, var2, var3, jexp, soul, curHp, maxHp, var4);
            reward->setExtExp(runAway->addexp());
            reward->setHpRate(runAway->hprate());
            //FIXME: 投降一定还是当前等级，暂时不修改这个消息，实际上需要一个oldLevel一样的参数，类似902/903消息，这是非常大的坑。
            reward->setBeforeLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
            SGPlayerInfo::sharePlayerInfo()->updateStoryData(_plotId, runAway->rating());
            
            //removed by cgp
            SGOverBattleBox * box = SGOverBattleBox::create(hero, reward,isRunAway,false);
            SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
            mainScene->addChild(box);
            
        }
        
        SGMainManager::shareMain()->trackMsnFailed(_plotId, CCString::create("defeat"));
        //播放场景音乐
        MUSIC_PLAY(MUSIC_SCENE);
    }
    else if(runAway && runAway->state() == 2)  //pvp
    {
        //pvp战斗中，3回合后才能投降。
        
        int runAwayCount = runAway->round();
        
        if (runAwayCount > 0) {
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_PvpSurrenderLimitMsg,runAwayCount)->getCString());
        }
        
        hero->restartRoundUpdataTime();
        hero->getBattleMap()->hideFriendlyHelp(true);
        SGAIManager::shareManager()->isBattleOver = true;
    }
}


void SGBattleManager::fightReviveRequest(int revive)
{
    FightReviveRequest *_revive = new FightReviveRequest();
    _revive->set_isrevive(revive);
    SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_REVIVE, _revive);
}

void SGBattleManager::fightReviveResponse(cocos2d::CCObject *obj)
{
    SGSocketRequest *msg = (SGSocketRequest*)obj;
    FightReviveResponse *revive = (FightReviveResponse*)msg->m_msg;
    if (revive->state() == 1) {
        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(true);
        hero->isBattleOver = false;
        getBattleLayer()->setIsOver(false);
        hero->setBloodNum(hero->heroBloodMax);
        hero->battleMap->closeMap = false;
        reviveRequest();
        
        SGHeroLayer *hero1 = SGBattleManager::sharedBattle()->getHeroLayer(false);
        hero1->isBattleOver = false;
        SGAIManager::shareManager()->isAi = true;
        hero1->battleMap->refreshMapGrid(true);
        SGMainManager::shareMain()->trackPurchase(CCString::create(str_MainLayer_str53), 1, 60);
    }else{
        SGBattleManager::sharedBattle()->battleOver();
    }
    
    
}

void SGBattleManager::fightLogRequest(const std::string &str)
{
    //关闭战斗日志，需要调试请自行打开。
    return;
    
    if (!m_isBattlePve)
    {
#if NEEDFIGHTLOG
        main::FightLogRequest *log = new main::FightLogRequest;
        log->set_str(str);
        //CCLOG("%s",str.c_str());
        SGSocketClient::sharedSocketClient()->send(MSG_fightLogRequest, log, false);
#endif
    }
}


void SGBattleManager::openReceive()
{
    SGNotificationCenter::sharedNotificationCenter()->isPostData = true;
}
void SGBattleManager::closeReceive()
{
    //ZHANGYU: guan le!
    return;
//    SGNotificationCenter::sharedNotificationCenter()->isPostData = false;
}
void SGBattleManager::lostConnection()
{
    
    if (NULL == SGBattleManager::sharedBattle() ||
        NULL == SGBattleManager::sharedBattle()->getBattleLayer()) {
        return;
    }
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGShadeBox2 * basebox = (SGShadeBox2 * )mainScene->getChildByTag(1315);
    if(basebox)
    {
        basebox->resetMainTouch(true);
        basebox->removeFromParentAndCleanup(true);
    }
    bool ismy =  mainBattleLayer->getIsMyRound();
    if (!SGAIManager::shareManager()->isPVE ||
        ismy) {
        SGHeroLayer *hero = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_myHeroLayerTag);
        hero->setRoundTime(50);
        
        
        SGHeroLayer *enemyHeroLayer = (SGHeroLayer *)mainBattleLayer->getChildByTag(sg_enemyHeroLayerTag);
        CCLOG("enemyHeroLayer->getInitRoundNum()==%d",enemyHeroLayer->getRound());
        enemyHeroLayer->setRoundNum(enemyHeroLayer->getRound());
    }

    SGAIManager::shareManager()->isLostCon = true;
    SGAIManager::shareManager()->isPVE = true;
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
    //MMDEBUG: VIP 对方如果掉线，如果还是我的回合中，则不能直接启动AI，而交给系统来控制AI。
    if (ismy)
    {
        ;
    }
    else
    {
        //对面真人掉线，AI接管的话，需要此标识为true，因为接管的当时，这个标识是false，会阻止AI做任何操作。
        SGAIManager::shareManager()->m_isVarifyActiviteAI = true;
        SGAIManager::shareManager()->startRound(hero->battleMap);
    }
}

#pragma mark - 剧情loading
void SGBattleManager::pveLoadingListener(CCObject *obj)
{
    //MMDBG: AIBATTLE 手动HACK使得PVP能直接进行！
    SGAIManager::shareManager()->m_isVarifyActiviteAI = true;
    
    SGAIManager::shareManager()->isPVE = true;
    SGGuideManager::shareManager()->isGuide=false;
    pveLoadMoveFinish = false;
    SGSocketRequest *msg = (SGSocketRequest*)obj;
    StartStatePVEResponse *respone = (StartStatePVEResponse*)msg->m_msg;
    if (respone->state()==1)
    {
        //本次战斗类型
        _battleType = (BATTLE_TYPE) respone->branchtype();
        
        SGLoadData *selfInfo = new SGLoadData();
        selfInfo->setRoleId(respone->image());
        selfInfo->setRoleName(CCString::create(respone->name().c_str()));
        selfInfo->setMIndex(respone->majorposition());
        selfInfo->setRoleType(respone->nation());
        CCArray *sarr = CCArray::create();
        for (int ii = 0; ii < respone->item_size(); ii++) {
            StartStateItemModel itemData = respone->item(ii);
            int value = itemData.itemid();
            int level = itemData.itemlevel();
            sarr->addObject(CCString::createWithFormat("%d:%d", value, level));
            GPCCLOG("\n^^^^^^^^^^^^^^ mySolder");
            GPCCLOG("%d:%d", value, level);
        }
        selfInfo->setSoldiers(sarr);
        
        SGLoadData *armyInfo = new SGLoadData();
        armyInfo->setRoleId(respone->timage());
        armyInfo->setRoleName(CCString::create(respone->tname().c_str()));
        armyInfo->setMIndex(respone->tmajorposition());
        armyInfo->setRoleType(respone->tnation());
        CCArray *aarr = CCArray::create();
        for (int ii = 0; ii < respone->titem_size(); ii++) {
            StartStateItemModel itemData = respone->titem(ii);
            int value = itemData.itemid();
            int level = itemData.itemlevel();
            aarr->addObject(CCString::createWithFormat("%d:%d", value, level));
            GPCCLOG("\n^^^^^^^^^^^^^^ enemySolder");
            GPCCLOG("%d:%d", value, level);
            
        }
        armyInfo->setSoldiers(aarr);
        
        /*                                                              */
        /************************  haha it is here***********************/
        /*                                                              */
        
        //SGBattlePreNLayer *layer = SGBattlePreNLayer::create(selfInfo, armyInfo); //cgp_pro
        
        CGPBattleLoadingLayer *layer = CGPBattleLoadingLayer::create();
        SGMainManager::shareMain()->showLayer(layer);
        layer->setZOrder(2);  //added by cgp
        
        
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        mainLayer->setIsCanTouch(false);
        
        
        //added by cgp
//        bool isPVE = SGAIManager::shareManager()->isPVE;
//        if (isPVE == true)
//        {
//            SGAIManager *aiMgr = SGAIManager::shareManager();
//            //重新进战斗重置自动战斗所有变量
//            aiMgr->setIsCompleteAttack(false);
//            aiMgr->setIsCompleteSkill(true);
//            aiMgr->setIsOpenAutoBattle(false);
//            aiMgr->setIsDispSkillHead(false);
//            aiMgr->setAnimationSpeed(1);
//            SGBattleManager::sharedBattle()->pveStarts();   //cgp_pro
//        }
//        else
//        {
//            SGBattleManager::sharedBattle()->pvpStarts();
//        }
        
        
        
        armyInfo->release();
        selfInfo->release();

        //本次战斗是否验证
        SGBattleManager::sharedBattle()->setIsBattlePve(!respone->needverify());
        
        //荆州进度
        _sjzLevel = respone->survivallevel();
    }
    else if (respone->state()==2)
    {
        SG_SHOW_WINDOW(str_MainLayer_str54);
    }
    else if (respone->state()==3)
    {
        SG_SHOW_WINDOW(str_MainLayer_str55);
    }
    else if (respone->state()==4)
    {
        SG_SHOW_WINDOW(str_MainLayer_str56);
    }
    else if (respone->state()==5)
    {
        SG_SHOW_WINDOW(str_MainLayer_str57);
    }
    else if (respone->state()==6)
    {
        SG_SHOW_WINDOW(str_MainLayer_str58);
    }
    else if (respone->state()==7)
    {
        SG_SHOW_WINDOW(str_MainLayer_str59);
    }
    else if (respone->state()==8)
    {
        SG_SHOW_WINDOW(str_MainLayer_str60);
    }
	else if (respone->state() == 10)
	{
		SG_SHOW_WINDOW(str_MainLayer_str61);
	}
    else if (respone->state() == 11)
	{
		SG_SHOW_WINDOW(str_MainLayer_str65);
	}
    if (respone->state() != 1) {
        SGGuideManager::shareManager()->setMainTouchEnable(true);
    }
}

void SGBattleManager::pvpLoadingListener(CCObject *obj)
{
    //MMDBG: AIBATTLE 手动HACK使得PVP能直接进行！
    SGAIManager::shareManager()->m_isVarifyActiviteAI = true;
    
    SGBattleManager::sharedBattle()->isGuide=false;

    SGAIManager::shareManager()->isPVE = false;
    

    SGGuideManager::shareManager()->isGuide=false;
 
    pveLoadMoveFinish = false;
    SGBattleManager::sharedBattle()->isMatchSucc = true;

    SGBattleManager::sharedBattle()->setIsBattlePve(false);

    SGMainManager::shareMain()->closeBox();
    
    SGSocketRequest *msg = (SGSocketRequest*)obj;
    StartStatePVPResponse *respone = (StartStatePVPResponse*)msg->m_msg;
    if (respone->state()==1) {
        //本次战斗类型
        _battleType = (BATTLE_TYPE) respone->branchtype();
        
        SGLoadData *selfInfo = new SGLoadData();
        selfInfo->autorelease();
        selfInfo->setRoleName(CCString::create(respone->name().c_str()));
        selfInfo->setMIndex(respone->majorposition());
        selfInfo->setRoleType(respone->nation());
        selfInfo->setRoleId(respone->nation()); //MM: 原为selfInfo->setRoleId(respone->image());由于有image()的值为0的情况，所以用nation()，因为国家代号和图片代号是一致的。
        CCArray *sarr = CCArray::create();
        for (int ii = 0; ii <  respone->itemid_size(); ii++) {
            StartStateItemModel itemData = respone->itemid(ii);
            int value = itemData.itemid();
            int level = itemData.itemlevel();
            sarr->addObject(CCString::createWithFormat("%d:%d", value, level));
        }
        selfInfo->setSoldiers(sarr);
        
        SGLoadData *armyInfo = new SGLoadData();
        armyInfo->autorelease();
        armyInfo->setRoleName(CCString::create(respone->tname().c_str()));
        armyInfo->setMIndex(respone->tmajorposition());
        armyInfo->setRoleType(respone->tnation());
        armyInfo->setRoleId(respone->tnation()); //MM: 原为armyInfo->setRoleId(respone->timage());由于有image()的值为0的情况，所以用nation()，因为国家代号和图片代号是一致的。
        CCArray *aarr = CCArray::create();
        for (int ii = 0; ii < respone->titemid_size(); ii++) {
            StartStateItemModel itemData = respone->titemid(ii);
            int value = itemData.itemid();
            int level = itemData.itemlevel();
            aarr->addObject(CCString::createWithFormat("%d:%d", value, level));
        }
        armyInfo->setSoldiers(aarr);
        
        
        SGBattlePreNLayer *layer = SGBattlePreNLayer::create(selfInfo, armyInfo);
        SGMainManager::shareMain()->showLayer(layer);
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        mainLayer->setIsCanTouch(false);
        CCLOG("respone->isai()==%d",respone->isai());
        SGPlayerInfo::sharePlayerInfo()->setBattleType(respone->isai());
    }
}

void SGBattleManager::fightFixMapRequest()
{
    //主要是用来控制倒计时。
    SGAIManager::shareManager()->m_isVarifyActiviteAI = false;
    bool round = SGBattleManager::SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(round);
    main::FightFixMapRequest *fixmap = new main::FightFixMapRequest();
    fixmap->set_roleid(hero->getRoleId());
    SGSocketClient::sharedSocketClient()->send(MSG_FightFixMapRequest, fixmap, true, true, BATTLE_DELAY_SHOW_TIME);
}

SGHeroLayer*  SGBattleManager::origFightFixMapResponse(void *fixmapResponse)
{
    main::FightFixMapResponse *fixmap = (main::FightFixMapResponse*)fixmapResponse;
    
    bool round = SGBattleManager::SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(round);
    if (hero->getRoleId() != fixmap->roleid()) {
        hero = SGBattleManager::sharedBattle()->getHeroLayer(!round);
    }
    hero->battleMap->removeSoldiers(false);
    hero->setRoundNum(fixmap->actioncount());
    hero->heroBloodMax = fixmap->basehp();
    hero->setBloodNum(fixmap->hp());
    hero->setSkillsRound(fixmap->majorskillcurrentround());
    hero->setSoldierNum(fixmap->freecount());
    hero->friendNum=0;
    hero->changeFriendNum(fixmap->cooround());
    
    
    int count = fixmap->spirits_size();
    CCArray *ar = CCArray::create();
    
    for (int i = 0; i< count; i++)
    {
        main::FightSpirit fs = fixmap->spirits(i);
        
        CCLOG("fs.basedef()===%d",fs.basedef());
        
        SGSBObj *obj = new SGSBObj();
        obj->setSid(fs.sid());
        obj->setColorId(fs.color());
        obj->setType(fs.type());
        obj->setIndex(gameIndex(fs.y(),fs.x()));
        
        obj->setsbBaseDefendPlus(fs.defeatplus());
        
        obj->setsbBaseAttack(fs.baseattack());
        obj->setsbBaseDefend(fs.basedef());
        
        obj->setAp(fs.attack());
        obj->setDef(fs.def());
        
        obj->setDefRend(fs.walltype());
        obj->setRound(fs.round());
        obj->setSkillsId(fs.skillid());
        obj->setAfftributeNum(fs.value1());
        obj->setAfftributeNum1(fs.value2());
        obj->setAfftributeNum2(fs.randnum());
        obj->setBuffId(fs.buffid());
        obj->setBuffRound(fs.effectround());
        obj->setBuffAfftribute(fs.bvalue1());
        obj->setBuffType(fs.bufftype());
        obj->setEffectType(fs.effect());
        obj->setAddDef(fs.wallceiling());
        obj->setAddAp(fs.perroundaddatk());
        obj->setChainNUm(fs.chainandmergeattackplus());
        obj->setRoundVAddDef(fs.wallgrow());
        obj->setAtkModeSkill(fs.atkmodeskill());
        obj->setDefType(fs.defeat());
        obj->setCurrentdef(fs.currentdef());
        obj->setsbSkLevel(1);
        if (obj->getIndex().i < 0 || obj->getIndex().i >= mapRow || obj->getIndex().j < 0 || obj->getIndex().j >= mapList) {
            CCString *str = CCString::createWithFormat(str_MainLayer_str38, obj->getIndex().i, obj->getIndex().j);
            CCNative::createAlert(str_MainLayer_str39, str->getCString(), NULL);
            CCNative::showAlert();
        }
        
        if (fs.type() == ksbone) {
            SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(fs.sid());
            obj->setAtkmode(data->getSoldierAtkMode());
            obj->setAtkMode2(data->getSoldierAtkMode2());
            obj->setHeadId(data->getSoldierHead());
            obj->setFileId(data->getSoldierFileId());
            obj->setPair(data->getSoldierPair());
            obj->setHitEff(data->getSoldierLastHit());
            obj->setAttackEffect(data->getAttackEffect());
            obj->setDeathEffect(data->getDeathEffect());
            obj->setName(data->getSoldierName()->getCString());
            //                    obj->setSkillsId(data->getSkill());
        }else{
            SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(fs.sid());
            obj->setAtkmode(data->getOfficerAtkMode());
            obj->setAtkMode2(data->getOfficerAtkMode2());
            obj->setHeadId(data->getOfficerHead());
            obj->setFileId(data->getOfficerFileId());
            obj->setPair(data->getOfficerPair());
            obj->setHitEff(data->getOfficerLastHit());
            obj->setAttackEffect(data->getAttackEffect());
            obj->setDeathEffect(data->getDeathEffect());
            obj->setName(data->getOfficerName()->getCString());
            obj->setSkill_Head(data->getSkillHead());
            //                    obj->setSkillsId(data->getSkill());
        }
        logSbData(obj);
        ar->addObject(obj);
        obj->autorelease();
    }
    
    hero->battleMap->initMapSBs(ar);
    
    ar->removeAllObjects();
    
    
    int _count = fixmap->attacklist_size();
    for (int i = 0; i < _count; i++) {
        CCDictionary *data = CCDictionary::create();
        main::FightFixAttackUnit attack = fixmap->attacklist(i);
        data->setObject(CCString::createWithFormat("%d",attack.x()), "DATA1");
        data->setObject(CCString::createWithFormat("%d",attack.y()), "DATA2");
        data->setObject(CCString::createWithFormat("%d",attack.currentround()), "DATA3");
        data->setObject(CCString::createWithFormat("%d",attack.currentattack()), "DATA4");
        data->setObject(CCString::createWithFormat("%d",attack.currentchaincount()), "DATA5");
        data->setObject(CCString::createWithFormat("%d",attack.currentmergecount()), "DATA6");
        data->setObject(CCString::createWithFormat("%d",attack.acceptamage()), "DATA7");
        data->setObject(CCString::createWithFormat("%d",attack.ishasfuhuo()), "DATA8");
        data->setObject(CCString::createWithFormat("%d",attack.isfuhuo()), "DATA9");
        
        int buff_i = attack.bufflist_size();
        CCArray *array = CCArray::create();
        for (int j = 0; j < buff_i; j++) {
            CCDictionary *_data = CCDictionary::create();
            main::FightFixBuff buff = attack.bufflist(j);
            _data->setObject(CCString::createWithFormat("%d",buff.buffid()), "BuffId");
            _data->setObject(CCString::createWithFormat("%d",buff.bufftype()), "BuffType");
            _data->setObject(CCString::createWithFormat("%d",buff.buffround()), "BuffRound");
            _data->setObject(CCString::createWithFormat("%f",buff.buffroundvalue()), "BuffAfftribute");
            _data->setObject(CCString::createWithFormat("%d",buff.buffeffect()), "EffectType");
            array->addObject(_data);
        }
		//根据服务器数据做攻击修正
        hero->battleMap->repairAttack(data, array);
        
    }
    return hero;
}

void SGBattleManager::fightFixMapResponse(cocos2d::CCObject *obj)
{
    CCLog("fightFixMapResponse");
    SGHeroLayer *hero =  NULL;
    SGBattleManager::sharedBattle()->getBattleLayer()->setIsRepairMap(true);
    
    SGSocketRequest *msg = (SGSocketRequest*)obj;
    main::FightFixMapResponse *fixmap = (main::FightFixMapResponse*)msg->m_msg;
    
    if (m_isBattlePve)
    {
        hero = clientPveContinueDeal(fixmap);
    }
    else
    {
        hero = origFightFixMapResponse(fixmap);
    }
    bool roundValue = SGBattleManager::SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    
    SGBattleManager::sharedBattle()->getBattleLayer()->setIsRepairMap(false);
    if (!isContinue) {
        if (SGAIManager::shareManager()->isPVE) {
            if (!roundValue) {
                SGAIManager::shareManager()->isAi = true;
                if (hero->getRound() > 0)
                    SGAIManager::shareManager()->startRound(hero->battleMap);
            }
        }
        
        
        if (hero->getRound() <= 0)
        {
            if (!roundValue &&
                SGAIManager::shareManager()->isPVE)
            {
                if (!hero->getBattleMap()->getIsMe())
                {
                    hero->aiEndRound();
                }
                
            }
            else
            {
                if (roundValue == hero->getBattleMap()->getIsMe())
                {
                    giveupRequest();
                    SGBattleManager::sharedBattle()->getBattleLayer()->changeRound();
                }

            }
        }
    }
    
    hero->beginRoundTime();
    
    //主要是用来控制倒计时。
    SGAIManager::shareManager()->m_isVarifyActiviteAI = true;
}

void SGBattleManager::QiangZhiFillUnitRequest()
{
    main::QiangZhiFillUnitRequest *msg = new main::QiangZhiFillUnitRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_QiangZhiFillUnitRequest, msg);
}

void SGBattleManager::QiangZhiFillUnitResponse(cocos2d::CCObject *obj)
{
    
    //    SGSocketRequest *msg = (SGSocketRequest*)obj;
    //    main::FightFixMapResponse *fixmap = (main::FightFixMapResponse*)msg->m_msg;
    
    SGSocketRequest *sr = (SGSocketRequest*)obj;
    
    if (sr) {
        main::QiangZhiFillUnitResponse *msg = (main::QiangZhiFillUnitResponse*)sr->m_msg;
        if (msg &&
            msg->state() == 1) {
            
            SGHeroLayer *heroLayer = NULL;
            if (msg->isself() == 1) {
                heroLayer = getHeroLayer(true);
            }else{
                heroLayer = getHeroLayer(false);
            }
            if (heroLayer) {
                 heroLayer->endRoundUpdataTime();
            }
           
            
            CCArray *sbs = CCArray::create();
                
            int sbLength = msg->spirits_size();
            
            for (int i = 0; i < sbLength; i++)
            {
                main::FightSpirit fs = msg->spirits(i);
                
                SGSBObj *obj = new SGSBObj();
                obj->setSid(fs.sid());
                
                obj->setColorId(fs.color());
                obj->setType(fs.type());
                
                obj->setsbBaseDefendPlus(fs.defeatplus());
                
                obj->setsbBaseAttack(fs.baseattack());
                obj->setsbBaseDefend(fs.basedef());
                
                obj->setAp(fs.attack());
                obj->setDef(fs.def());
                
                obj->setResetInitDef(fs.basedef());
                obj->setDefRend(fs.walltype());
                obj->setRound(fs.round());
                obj->setIndex(gameIndex(fs.y(),fs.x()));
                obj->setSkillsId(fs.skillid());
                obj->setAfftributeNum(fs.value1());
                obj->setAfftributeNum1(fs.value2());
                obj->setAfftributeNum2(fs.randnum());
                obj->setBuffId(fs.buffid());
                obj->setBuffRound(fs.effectround());
                obj->setBuffAfftribute(fs.bvalue1());
                obj->setAfftributeNum2(fs.randnum());
                obj->setBuffType(fs.bufftype());
                obj->setEffectType(fs.effect());
                obj->setAddDef(fs.wallceiling());
                obj->setAddAp(fs.perroundaddatk());
                obj->setChainNUm(fs.chainandmergeattackplus());
                obj->setRoundVAddDef(fs.wallgrow());
                obj->setAtkModeSkill(fs.atkmodeskill());
                if (heroLayer) {
                    obj->setFriendOff(heroLayer->isFriend);
                }
                
                
                if (fs.type() == ksbone) {
                    SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(fs.sid());
                    obj->setAtkmode(data->getSoldierAtkMode());
                    obj->setAtkMode2(data->getSoldierAtkMode2());
                    obj->setHeadId(data->getSoldierHead());
                    obj->setFileId(data->getSoldierFileId());
                    obj->setPair(data->getSoldierPair());
                    obj->setHitEff(data->getSoldierLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getSoldierName()->getCString());
                    //                    obj->setSkillsId(data->getSkill());
                }else{
                    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(fs.sid());
                    obj->setAtkmode(data->getOfficerAtkMode());
                    obj->setAtkMode2(data->getOfficerAtkMode2());
                    obj->setHeadId(data->getOfficerHead());
                    obj->setFileId(data->getOfficerFileId());
                    obj->setPair(data->getOfficerPair());
                    obj->setHitEff(data->getOfficerLastHit());
                    obj->setAttackEffect(data->getAttackEffect());
                    obj->setDeathEffect(data->getDeathEffect());
                    obj->setName(data->getOfficerName()->getCString());
                    obj->setSkill_Head(data->getSkillHead());
                    //                    obj->setSkillsId(data->getSkill());
                }
                logSbData(obj);
                sbs->addObject(obj);
                obj->autorelease();
            }
            
            SGBattleMap *battleMap = NULL;
            if (heroLayer) {
                battleMap = heroLayer->getBattleMap();
                heroLayer->setSoldierNum(0);
            }
            if (battleMap) {
                battleMap->initMapSBs(sbs);
                battleMap->qzFillUnit(callfuncN_selector(SGBattleMap::qzEnd));
            }
            
        }
    }
}



void SGBattleManager::clientPveContinueInitHeroLayer(CCDictionary * totalSbDics,bool isMyhero)
{
    mainBattleLayer->setIsMyRound(true);
    mainBattleLayer->changeColorSp();
    
    pveLoadMoveFinish = false;
    
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(isMyhero);
    if (hero->battleMap) {
        hero->battleMap->removeSoldiers();
    }
    
    CCString *heroData = NULL;
    if (isMyhero)
    {
        heroData = (CCString *)totalSbDics->objectForKey(MYDataDIC);
        hero->changeRound(true, true, true);
    }
    else
    {
        heroData = (CCString *)totalSbDics->objectForKey(ENEMYDataDIC);
    }
    
    int tHp = 0,tBaseHp = 0,tActionCount = 0,tSkillRoundCur = 0,tFriendRound=0,tFreeCount = 0;
    
    sscanf(heroData->getCString(), "%d - %d - %d - %d - %d - %d", &tHp,&tBaseHp,&tActionCount,&tSkillRoundCur,&tFriendRound,&tFreeCount);
    
    hero->heroBloodMax = tBaseHp;
    hero->setBloodNum(tHp);
    hero->setRoundNum(tActionCount);
    hero->setSkillsRound(tSkillRoundCur);
    hero->setSoldierNum(tFreeCount);
    hero->friendNum=0;
    hero->changeFriendNum(tFriendRound);


    
}

void SGBattleManager::clientPveContinueInitBattleSbs(CCDictionary * totalSbDics,bool isMyhero)
{
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(isMyhero);
    CCDictionary *sbsData = NULL;
    if (isMyhero) {
        sbsData = (CCDictionary *)totalSbDics->objectForKey(MYSBSDIC);
    }
    else
    {
        sbsData = (CCDictionary *)totalSbDics->objectForKey(ENEMYSBSDIC);
    }
    
    int count = sbsData->count();
    SGSBObj *obj = NULL;
    CCString *tempData = NULL;
    
    int tSid = 0,tColorId = 0, tType = 0,fsX = 0,fsY = 0,tAp = 0,tDef = 0,tResetInitDef = 0,tRound = 0,tSkillsId = 0,tAfftributeNum2 = 0;
    int tBuffId = 0,tBuffRound = 0,tBuffType = 0,tAddAp = 0,tAtkModeSkill = 0 , tEffect = 0,tDefType = 0,tCurrentDef = 0,tBaseAttack = 0,tBaseDefend = 0 ;
    float  tDefRend = 0, tAfftributeNum = 0, tAfftributeNum1  = 0, tBuffAfftribute = 0, tAddDef = 0, tChainNUm  = 0,tRoundVAddDef = 0,tDefeatPlus = 0;

    CCArray *ar = CCArray::create();
    
    for (int i = 0; i< count; i++)
    {
        obj = new SGSBObj();
        std::string key(CCString::createWithFormat("sbData%d",i)->getCString());
        tempData = (CCString *)sbsData->objectForKey(key);
        
        sscanf(tempData->getCString(), "%d - %d - %d - %d - %d  - %d - %d - %d - %f - %d - %d - %f - %f - %d - %d - %d - %f - %d - %f - %d - %f - %f - %d - %d - %d - %d - %d - %d - %f",
               &tSid,&tColorId,&tType,&fsX,&fsY,
               &tAp,&tDef,&tResetInitDef,&tDefRend,&tRound,
               &tSkillsId,&tAfftributeNum,&tAfftributeNum1,&tAfftributeNum2,
               &tBuffId,&tBuffRound,&tBuffAfftribute,&tBuffType,&tAddDef,&tAddAp,
               &tChainNUm,&tRoundVAddDef,&tAtkModeSkill,&tEffect,&tDefType,&tCurrentDef,&tBaseAttack,&tBaseDefend,&tDefeatPlus);
        
        obj->setSid(tSid);
        obj->setColorId(tColorId);
        obj->setType(tType);
        obj->setIndex(gameIndex(fsX,fsY));
        
        obj->setsbBaseDefendPlus(tDefeatPlus);
        
        obj->setsbBaseAttack(tBaseAttack);
        obj->setsbBaseDefend(tBaseDefend);
    
        obj->setAp(tAp);
        obj->setDef(tDef);
        
        obj->setResetInitDef(tResetInitDef);
        obj->setDefRend(tDefRend);
        obj->setRound(tRound);
        obj->setSkillsId(tSkillsId);//服务器给到的技能id
        obj->setAfftributeNum(tAfftributeNum);
        obj->setAfftributeNum1(tAfftributeNum1);
        obj->setAfftributeNum2(tAfftributeNum2);//杀死武将队列的随机位置,(对于破袭技能)
        obj->setBuffId(tBuffId);
        obj->setBuffRound(tBuffRound);
        obj->setBuffAfftribute(tBuffAfftribute);
        obj->setBuffType(tBuffType);
        obj->setEffectType(tEffect);
        
        obj->setAddDef(tAddDef);
        obj->setAddAp(tAddAp);
        obj->setChainNUm(tChainNUm);
        obj->setRoundVAddDef(tRoundVAddDef);
        obj->setAtkModeSkill(tAtkModeSkill);
        
        obj->setDefType(tDefType == 1 ? true : false);
        obj->setCurrentdef(tCurrentDef);
        
        obj->setsbSkLevel(1);
        
        if ((sgShiBingType)tType == ksbone) {
            SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(tSid);
            obj->setAtkmode(data->getSoldierAtkMode());
            obj->setAtkMode2(data->getSoldierAtkMode2());
            obj->setHeadId(data->getSoldierHead());
            obj->setFileId(data->getSoldierFileId());
            obj->setPair(data->getSoldierPair());
            obj->setHitEff(data->getSoldierLastHit());
            obj->setAttackEffect(data->getAttackEffect());
            obj->setDeathEffect(data->getDeathEffect());
            obj->setName(data->getSoldierName()->getCString());
        }else{
            SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(tSid);
            obj->setAtkmode(data->getOfficerAtkMode());
            obj->setAtkMode2(data->getOfficerAtkMode2());
            obj->setHeadId(data->getOfficerHead());
            obj->setFileId(data->getOfficerFileId());
            obj->setPair(data->getOfficerPair());
            obj->setHitEff(data->getOfficerLastHit());
            obj->setAttackEffect(data->getAttackEffect());
            obj->setDeathEffect(data->getDeathEffect());
            obj->setName(data->getOfficerName()->getCString());
            obj->setSkill_Head(data->getSkillHead());
        }
        ar->addObject(obj);
        obj->autorelease();
    }
    
    hero->battleMap->initMapSBs(ar);
    
    ar->removeAllObjects();
}

void SGBattleManager::clientPveContinueInitAttackList(CCDictionary * totalSbDics,bool isMyhero)
{
    
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(isMyhero);
    CCDictionary *attackDic = NULL;
    if (isMyhero) {
        attackDic = (CCDictionary *)totalSbDics->objectForKey(MYATTACKDIC);
    }
    else
    {
        attackDic = (CCDictionary *)totalSbDics->objectForKey(ENEMYATTACKDIC);
    }
    
    CCDictionary *attackData = NULL;
    CCDictionary *buffDic = NULL;
    int count = attackDic->count();
    int buffCount = 0;
    
    for (int index = 0; index < count; index++)
    {
        std::string key(CCString::createWithFormat("attackDic%d",index)->getCString());
        attackData = (CCDictionary *)attackDic->objectForKey(key);
        
        buffDic = (CCDictionary *)attackData->objectForKey(ATTACKBUFFDIC);
        buffCount = buffDic->count();
        CCArray *buffArray = CCArray::create();
        for (int buffIndex = 0; buffIndex < buffCount; buffIndex++)
        {
            std::string key(CCString::createWithFormat("buffData%d",buffIndex)->getCString());
            buffArray->addObject(buffDic->objectForKey(key));
        }
        
        hero->battleMap->repairAttack(attackData, buffArray);
    }
}

void SGBattleManager::clientPveContinueInitBackupSbs(CCDictionary * totalSbDics,bool isMyhero)
{
    CCDictionary *backupSbsData = NULL;
    CCArray *backupSbs = NULL;
    CCArray *backupHero = NULL;
    if (isMyhero)
    {
        backupSbsData = (CCDictionary*)totalSbDics->objectForKey(MYBACKUPSBDIC);
        backupSbs = myBackupsbs;
        backupHero = myBackupHeros;
    }
    else
    {
        backupSbsData = (CCDictionary*)totalSbDics->objectForKey(ENEMYBACKUPSBDIC);
        backupSbs = himBackupSbs;
        backupHero = himBackupHeros;
    }
    backupSbs->removeAllObjects();
    backupHero->removeAllObjects();
    
    SGSBObj *obj = NULL;
    CCString *tempData = NULL;
    
    int tSid = 0,tColorId = 0, tType = 0,fsX = 0,fsY = 0,tAp = 0,tDef = 0,tResetInitDef = 0,tRound = 0,tSkillsId = 0,tAfftributeNum2 = 0;
    int tBuffId = 0,tBuffRound = 0,tBuffType = 0,tAddAp = 0,tAtkModeSkill = 0,tBaseAttack = 0,tBaseDefend = 0 ;
    float  tDefRend = 0, tAfftributeNum = 0, tAfftributeNum1  = 0, tBuffAfftribute = 0, tAddDef = 0, tChainNUm  = 0,tRoundVAddDef = 0,tDefeatPlus = 0;
    
    int sbCount = backupSbsData->count();
    for (int index = 0; index < sbCount; index++)
    {
        obj = new SGSBObj();
        std::string key(CCString::createWithFormat("backupSbData%d",index)->getCString());
        tempData = (CCString *)backupSbsData->objectForKey(key);
        
        sscanf(tempData->getCString(), "%d - %d - %d - %d - %d  - %d - %d - %d - %f - %d - %d - %f - %f - %d - %d - %d - %f - %d - %f - %d - %f - %f - %d - %d - %d - %f",
               &tSid,&tColorId,&tType,&fsX,&fsY,
               &tAp,&tDef,&tResetInitDef,&tDefRend,&tRound,
               &tSkillsId,&tAfftributeNum,&tAfftributeNum1,&tAfftributeNum2,
               &tBuffId,&tBuffRound,&tBuffAfftribute,&tBuffType,&tAddDef,&tAddAp,
               &tChainNUm,&tRoundVAddDef,&tAtkModeSkill,&tBaseAttack,&tBaseDefend,&tDefeatPlus);
        
        obj->setSid(tSid);
        obj->setColorId(tColorId);
        obj->setType(tType);
        obj->setIndex(gameIndex(fsX,fsY));
        
        obj->setsbBaseDefendPlus(tDefeatPlus);
        
        obj->setsbBaseAttack(tBaseAttack);
        obj->setsbBaseDefend(tBaseDefend);
        
        obj->setAp(tAp);
        obj->setDef(tDef);
        
        obj->setResetInitDef(tResetInitDef);
        obj->setDefRend(tDefRend);
        obj->setRound(tRound);
        obj->setSkillsId(tSkillsId);//服务器给到的技能id
        obj->setAfftributeNum(tAfftributeNum);
        obj->setAfftributeNum1(tAfftributeNum1);
        obj->setAfftributeNum2(tAfftributeNum2);//杀死武将队列的随机位置,(对于破袭技能)
        obj->setBuffId(tBuffId);
        obj->setBuffRound(tBuffRound);
        obj->setBuffAfftribute(tBuffAfftribute);
        obj->setBuffType(tBuffType);
        obj->setAddDef(tAddDef);
        obj->setAddAp(tAddAp);
        obj->setChainNUm(tChainNUm);
        obj->setRoundVAddDef(tRoundVAddDef);
        obj->setAtkModeSkill(tAtkModeSkill);
        obj->setsbSkLevel(1);

        if ((sgShiBingType)tType == ksbone)
        {
            SGSoldierDataModel *data = SGStaticDataManager::shareStatic()->getSoliderById(tSid);
            obj->setAtkmode(data->getSoldierAtkMode());
            obj->setAtkMode2(data->getSoldierAtkMode2());
            obj->setHeadId(data->getSoldierHead());
            obj->setFileId(data->getSoldierFileId());
            obj->setPair(data->getSoldierPair());
            obj->setHitEff(data->getSoldierLastHit());
            obj->setAttackEffect(data->getAttackEffect());
            obj->setDeathEffect(data->getDeathEffect());
            obj->setName(data->getSoldierName()->getCString());
            backupSbs->addObject(obj);
        }
        else
        {
            SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(tSid);
            obj->setAtkmode(data->getOfficerAtkMode());
            obj->setAtkMode2(data->getOfficerAtkMode2());
            obj->setHeadId(data->getOfficerHead());
            obj->setFileId(data->getOfficerFileId());
            obj->setPair(data->getOfficerPair());
            obj->setHitEff(data->getOfficerLastHit());
            obj->setAttackEffect(data->getAttackEffect());
            obj->setDeathEffect(data->getDeathEffect());
            obj->setName(data->getOfficerName()->getCString());
            obj->setSkill_Head(data->getSkillHead());
            backupHero->addObject(obj);
        }
        obj->autorelease();
    }
    
    //友军
    if (isMyhero)
    {
        if (cooTempOfficer != NULL) {
            cooTempOfficer->release();
            cooTempOfficer = NULL;
        }
        
        obj = new SGSBObj();
        tempData = (CCString *)totalSbDics->objectForKey(FRIENDHERODATA);
        
        sscanf(tempData->getCString(), "%d - %d - %d - %d - %d  - %d - %d - %d - %f - %d - %d - %f - %f - %d - %d - %d - %f - %d - %f - %d - %f - %f - %d - %d - %d - %f",
               &tSid,&tColorId,&tType,&fsX,&fsY,
               &tAp,&tDef,&tResetInitDef,&tDefRend,&tRound,
               &tSkillsId,&tAfftributeNum,&tAfftributeNum1,&tAfftributeNum2,
               &tBuffId,&tBuffRound,&tBuffAfftribute,&tBuffType,&tAddDef,&tAddAp,
               &tChainNUm,&tRoundVAddDef,&tAtkModeSkill,&tBaseAttack,&tBaseDefend,&tDefeatPlus);
        
        obj->setSid(tSid);
        obj->setColorId(tColorId);
        obj->setType(tType);
        obj->setIndex(gameIndex(fsX,fsY));
        
        obj->setsbBaseDefendPlus(tDefeatPlus);
        
        obj->setsbBaseAttack(tBaseAttack);
        obj->setsbBaseDefend(tBaseDefend);
        
        obj->setAp(tAp);
        obj->setDef(tDef);
        
        obj->setResetInitDef(tResetInitDef);
        obj->setDefRend(tDefRend);
        obj->setRound(tRound);
        obj->setSkillsId(tSkillsId);//服务器给到的技能id
        obj->setAfftributeNum(tAfftributeNum);
        obj->setAfftributeNum1(tAfftributeNum1);
        obj->setAfftributeNum2(tAfftributeNum2);//杀死武将队列的随机位置,(对于破袭技能)
        obj->setBuffId(tBuffId);
        obj->setBuffRound(tBuffRound);
        obj->setBuffAfftribute(tBuffAfftribute);
        obj->setBuffType(tBuffType);
        obj->setAddDef(tAddDef);
        obj->setAddAp(tAddAp);
        obj->setChainNUm(tChainNUm);
        obj->setRoundVAddDef(tRoundVAddDef);
        obj->setAtkModeSkill(tAtkModeSkill);
        obj->setsbSkLevel(1);
        
        if ((sgShiBingType)tType != ksbone)
        {
            SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(tSid);
            obj->setAtkmode(data->getOfficerAtkMode());
            obj->setAtkMode2(data->getOfficerAtkMode2());
            obj->setHeadId(data->getOfficerHead());
            obj->setFileId(data->getOfficerFileId());
            obj->setPair(data->getOfficerPair());
            obj->setHitEff(data->getOfficerLastHit());
            obj->setAttackEffect(data->getAttackEffect());
            obj->setDeathEffect(data->getDeathEffect());
            obj->setName(data->getOfficerName()->getCString());
            obj->setSkill_Head(data->getSkillHead());
            
            cooTempOfficer = obj;
            cooTempOfficer->retain();
        }
        obj->autorelease();
    }
}

SGHeroLayer * SGBattleManager::clientPveContinueDeal(void *fixmapResponse)
{
    
    main::FightFixMapResponse *fixmap = (main::FightFixMapResponse*)fixmapResponse;
    
    bool round = SGBattleManager::SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(round);
    
    bool isMyHero = round;
    if (hero->getRoleId() != fixmap->roleid()) {
        hero = SGBattleManager::sharedBattle()->getHeroLayer(!round);
        isMyHero = !round;
    }
    

    CCDictionary *totalSbDics = CCDictionary::createWithContentsOfFileThreadSafe(getClientSaveBattleDataName());
    
    mainBattleLayer->setBattleCardNum( ((CCString*)totalSbDics->objectForKey(TOTALDROPCOUNT))->intValue() );
    
    clientPveContinueInitHeroLayer(totalSbDics,isMyHero);
    
    clientPveContinueInitBattleSbs(totalSbDics,isMyHero);
    
    clientPveContinueInitAttackList(totalSbDics,isMyHero);
    
    clientPveContinueInitBackupSbs(totalSbDics,isMyHero);
    
    SGBattleManager::sharedBattle()->setIsBattlePve(true);
    
    return hero;
}

void SGBattleManager::pveContinuListener(CCObject *obj)
{
    CCLOG("pveContinuListener");
}

void SGBattleManager::pveContinuStart(int back)
{
    SGAIManager::shareManager()->isPVE = true;
    this->isContinue = true;
    main::PveContinuePlayRequest *request = new main::PveContinuePlayRequest();
    request->set_request(back);
    SGSocketClient::sharedSocketClient()->send(MSG_PveContinuePlayRequest, request,back);
    CCLog("继续战斗");
    
}
void SGBattleManager::pveStarts()
{
    if (_battleType == BT_ADVENTURE) //闯关模式 cgp_pro
    {
        // PVE开始不扣除体力，胜利时才扣除体力。
        int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
        SGSpotDataModel *model = SGStaticDataManager::shareStatic()->getSpotById(plotId);
        
        int power = 0;
        int pSread = 0;

        
        if (model)
        {
            power = SGPlayerInfo::sharePlayerInfo()->getPlayerPower();
            pSread = power - model->getSpotRequestPower();
        }
        if (pSread >= 0 || plotId >= 30000)//如果是幻境，不做战斗力比对
        {
            main::PveStarRequest *sr = new main::PveStarRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_PVESTART, sr);
        }
        else   //体力不够
        {
            SGMainManager::shareMain()->showMessage(str_MainLayer_str62);
        }
    }
    else if (_battleType == BT_SURVIVAL) //守荆州模式
    {
        main::PveStarRequest *sr = new main::PveStarRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_PVESTART, sr);
    }
    else if(_battleType == BT_ARENA)//竞技场
    {
        main::PveStarRequest *sr = new main::PveStarRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_PVESTART, sr);
        SGAIManager::shareManager()->setAnimationSpeed(1);
        SGAIManager::shareManager()->setIsOpenAutoBattle(true);
    }
    else
    {
        CCLOG("试图进入战斗出现错误！");
    }
}

void SGBattleManager::pvpStarts()
{

    CCLOG("pvpStarts");
    // 减军粮
    int junliang = SGPlayerInfo::sharePlayerInfo()->getPvpPower();
    if (junliang > 0) {
        SGPlayerInfo::sharePlayerInfo()->setPvpPower(junliang-1);
        main::StartRequest *sr = new main::StartRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_BATTLE_MATCH, sr);
    }else {
        SGMainManager::shareMain()->showMessage(str_MainLayer_str63);
    }
}

#pragma mark 守荆州相关
//守荆州战败后战绩统计信息
void SGBattleManager::sjzFinalStatResponse(CCObject* obj)
{
    //战绩消息解析
    SGSocketRequest* sr = (SGSocketRequest*) obj;
    if (sr)
    {
        main::SurvivalFinalStatResponse* rsp = (main::SurvivalFinalStatResponse*) sr->m_msg;
        SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
        data->setIsShowBonus(true);
        data->setBonusDamage(rsp->moralenum());
        data->setBonusSoul(rsp->soulnum());
    }
}

//开始守城请求。
void SGBattleManager::sjzChallengeRequest()
{
    //发送获取详情
    main::SurvivalChallengeRequest* req = new main::SurvivalChallengeRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_SURVIVAL_CHALLENGE, req);
}

//开始守城响应。
void SGBattleManager::sjzChallengeResponse(CCObject* obj)
{
//    //移除监听
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_SURVIVAL_CHALLENGE);
    //接收
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::SurvivalChallengeResponse *rsp = (main::SurvivalChallengeResponse *) sr->m_msg;
///////////////////////////////////////////////////////逻辑变动
//        //解析数据
//        if ( rsp->canjump() ) //可跳过，显示confirm
//        {
//            int prct = rsp->lastpercentage();
//            //弹出二次确认框界面
//            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 50, prct, NULL);
//            SGMainManager::shareMain()->showBox(cantadvanceBox);
//        }
//        else //否则，直接进入战斗
//        {
//            //发送start
//            sjzStartRequest(false);
//        }
///////////////////////////////////////////////////
        ////////直接进入战斗
        sjzStartRequest(false);
        
    }
}

//尝试进入战斗req。 isJump:true则跳过本场战斗，false则不跳过，若如此以后都不能跳过了。
void SGBattleManager::sjzStartRequest(bool isJump)
{
    //发送获取详情
    main::SurvivalStartRequest* req = new main::SurvivalStartRequest();
    req->set_isjump(isJump ? 1 : 0);
    SGSocketClient::sharedSocketClient()->send(MSG_SURVIVAL_START, req);
}

//尝试进入战斗rsp。
void SGBattleManager::sjzStartResponse(CCObject* obj)
{
    //接收
    SGSocketRequest *sr = (SGSocketRequest *) obj;
    if (sr)
    {
        main::SurvivalStartResponse* rsp = (main::SurvivalStartResponse*) sr->m_msg;
        if (rsp->state() != 1) //非1则失败，显示失败原因，1则预置战斗类型。
        {
            std::string str = rsp->errmsg();
            SGMainManager::shareMain()->showMessage(str.c_str());
        }
        else
        {
            this->_battleType = BT_SURVIVAL;
            this->isWin = true;
        }
    }
}

#pragma mark 其他
void SGBattleManager::startCountingForHelp(bool counting){
    SGHeroLayer *hero1 = SGBattleManager::sharedBattle()->getHeroLayer(true);
    hero1->battleMap->unschedule(schedule_selector(SGBattleMap::hideFriendlyHelp));
    hero1->battleMap->hideFriendlyHelp(counting);
}


void SGBattleManager::addHisBackUpSb(SGSBObj *battleSb)
{
    CCObject * obj = NULL;
    CCARRAY_FOREACH(himBackupSbs, obj)
    {
        if (((SGSBObj *)obj)->getColorId() == battleSb->getColorId() ) {
            return;
        }
    }
    himBackupSbs->addObject(battleSb);
    
}
void SGBattleManager::addMyBackUpSb(SGSBObj *battleSb)
{
    CCObject * obj = NULL;
    CCARRAY_FOREACH(myBackupsbs, obj)
    {
        if (((SGSBObj *)obj)->getColorId() == battleSb->getColorId() ) {
            return;
        }
    }
    myBackupsbs->addObject(battleSb);
}

SGSBObj * SGBattleManager::getBackUpSbById(int colorId)
{
    CCObject * obj = NULL;
    if (mainBattleLayer->getIsMyRound()) {
        CCARRAY_FOREACH(myBackupsbs, obj)
        {
            if (((SGSBObj *)obj)->getColorId() == colorId ) {
                return  (SGSBObj *)obj;
            }
        }
    }
    else
    {
        CCARRAY_FOREACH(himBackupSbs, obj)
        {
            if (((SGSBObj *)obj)->getColorId() == colorId) {
                return  (SGSBObj *)obj;
            }
        }
    }

    return NULL;
}

SGSBObj * SGBattleManager::getBackUpHeroByIndex(int index)
{
    SGSBObj *backHero = NULL;
    if (mainBattleLayer->getIsMyRound())
    {
        if (index < myBackupHeros->count())
        {
            backHero = (SGSBObj *) myBackupHeros->objectAtIndex(index);
            backHero->retain();
            myBackupHeros->removeObjectAtIndex(index);
            return  (SGSBObj *)backHero;
        }
    }
    else
    {
        if (index < himBackupHeros->count())
        {
            backHero = (SGSBObj *) himBackupHeros->objectAtIndex(index);
            backHero->retain();
            himBackupHeros->removeObjectAtIndex(index);
            return  (SGSBObj *)backHero;
        }
    }

    return NULL;
}

sgShiBingType SGBattleManager::getBackUpHeroTypeByIndex(int index)
{
    
    if (mainBattleLayer->getIsMyRound()) {
        if (index < myBackupHeros->count() )
        {
            return (sgShiBingType)((SGSBObj *)myBackupHeros->objectAtIndex(index))->getType();

        }
    }
    else
    {
        if (index < himBackupHeros->count())
        {
            return (sgShiBingType)((SGSBObj *)himBackupHeros->objectAtIndex(index))->getType();
        }
    }
    
    return ksbone;
}

int SGBattleManager::getBackUpHeroCount()
{
    if (mainBattleLayer->getIsMyRound())
    {
        return myBackupHeros->count();
    }
    else
    {
        return himBackupHeros->count();
    }

}

void SGBattleManager:: pvpFightRewardResponse(CCObject * sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender ;
    main::FightPvpRewardResponse *response = (main::FightPvpRewardResponse *)request->m_msg;
    
    if (response)
    {
        //state: 0=找不到 1=成功 2=战斗未结束
        if(1 == response->state())
        {
            SGPvpFightRewardData * data = new SGPvpFightRewardData();
            data->setShengWang(response->prestige());
            data->setScore(response->score());
            data->setRank(response->rank());
            data->setJoinCount(response->count());
            data->setIsWin(response->iswin());
            
            CCAssert(NULL!=data, "\n\n\nzyc debug for pvp ...不能为空。\n\n\n\n");
            
            SGHeroLayer *hero = getHeroLayer(true);
            SGOverBattleBox * box = SGOverBattleBox::create(hero, data);
            SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
            mainScene->addChild(box);
        }
    }
    //战斗结束后，记得播放场景音乐。
    MUSIC_PLAY(MUSIC_SCENE);
}
void SGBattleManager::arenaFightRewardListen(CCObject * obj)
{
    SGSocketRequest *request = (SGSocketRequest *)obj ;
    main::ArenaEndFightResponse *response = (main::ArenaEndFightResponse *)request->m_msg;
    
    if (response)
    {
        SGArenaFightRewardData * data = new SGArenaFightRewardData();
        data->setAfterRank(response->afterrank());
        data->setBeforeRank(response->beforerank());
        data->setArenaCoin(response->coins());
        data->setCount(response->count());
        data->setMaxCount(response->maxcount());
        data->setIsWin(response->iswin());
               
        SGHeroLayer *hero = getHeroLayer(true);
        SGOverBattleBox * box = SGOverBattleBox::create(hero, data);
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        mainScene->addChild(box);
        
    }
    //战斗结束后，记得播放场景音乐。
    MUSIC_PLAY(MUSIC_SCENE);

}
//曲线救国方式改变herolayer中自动战斗按钮的穿透
void SGBattleManager::setCanTouchAutoBattleBtn(bool canTouch)
{
    //不求效果，只求作用
//    SGHeroLayer *layer = static_cast<SGHeroLayer *>(SGBattleManager::sharedBattle()->getHeroLayer(true));
//    if (layer)
//    {
//        layer->setAutoBattleBtnCanTouch(canTouch);
//    }
    m_canTouchAutoBattleBtn = canTouch;
}

bool SGBattleManager::getCanTouchAutoBattleBtn()
{
    return m_canTouchAutoBattleBtn;
}


void SGBattleManager::setBattleType(BATTLE_TYPE bt)
{
    _battleType = bt;
}

BATTLE_TYPE SGBattleManager::getBattleType()
{
    return _battleType;
}

//MM&ZY: 如果是需要验证的战斗,则需要回合驱动标志为true,才能执行某些动作,目前针对sgherolayer放里面的所有按钮.
bool SGBattleManager::getCanDoBattleOperation()
{
    SGAIManager* aim = SGAIManager::shareManager();
    //当且仅当"需要验证(m_isBattlePve=false) && 回合驱动未完成(m_isVarifyActiviteAI=false) && 是PVP打AI电脑"的时候,阻止某些操作.
    if (!m_isBattlePve && !aim->m_isVarifyActiviteAI /*&& aim->isPVE*/)
    {
        return false;
    }
    return true;
}
