

//
//  SGBattleMap.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-20.
//

//com.xdyou.sanguolol

#include "SGBattleMap.h"
//#include "TestData.h"
#include "SGPrinceBase.h"
#include "SGSuperPrinceBase.h"
#include "SGNormalAttackList.h"
#include "SGTwoAttackList.h"
#include "SGFourAttackList.h"
#include "SGHeroLayer.h"
#include "SG_SkillsManager.h"
#include "SG_BuffBase.h"
#include "SGAIManager.h"
#include "SGSkillManager.h"
#include "SGHeroLayer.h"
#include "SGPlayerInfo.h"
#include "SGGuideManager.h"
#include "GameMusicmanage.h"
#include "ResourceManager.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"
#include "SGMainManager.h"
#include <algorithm>

#define AI_REMIND_TIME 7
//动画时间
#define MOVETIME    0.5
#define GAPTIME     0.8
#define DOWNTIME    0.1
#define UPTIME      0.2
#define INSTANTIME  0.01
#define DOWNSCALE   0.6


#define  clickTime 2
#define  RemoveMenuTag 30
#define  RntervalTime 0.6
#define  SB_TAG 20
#define Delete_Red_Rect 1111    //删除红色底框
#define Delete_Red_Rect2 1112    //删除红色底框
#define Move_Green_Rect 1002    //移动时的绿色底框
#define SB_RECT_TAY 35
#define SB_COPY_TAY 37
#define ROUND_ADD_TIME_NUM 4
#define WITH_SB_TAG 1212
#define TOUCH_INIT_SB_ADDACT 0
#define TOUCH_INIT_SB_BJ 28
#define REMOVEPROGRESSTAG -30
#define MSGLABELTAG -9
#define FRIENDLYGAPTIME 15
#define AI_START_TIME 0.8
#define BATTLEDARROWTAG 20

//旗子用另一个tag,这里与虚空盾,幽冥盾分开
#define FLAG 99
#define SPTOUCHTAG 100

SGBattleMap::SGBattleMap()
:attackLists(NULL),
soldiers(NULL),
waitSoldiers(NULL),
m_fangYuUpBingArray(NULL),
m_nomalAttackUpBingArray(NULL),
m_spAttackUpArray(NULL),
m_deleteDownArray(NULL),
m_moveAllGrid(NULL),
m_jointlySbArray(NULL),
m_jointlySbArrarCopy(NULL),
m_synthesisGrid(NULL),
m_spAttackGrid(NULL),
m_moveSanGrid(NULL),
m_spAttackArray(NULL),
m_removeSbDef(NULL),
sbCopy(NULL),
m_clickTime(0),
m_touchPosIndex(gameIndex(-1, -1)),
m_touchInitIndex(gameIndex(-1, -1)),
isAddGrid(false),
isAddGrid1(false),
isAddGrid2(false),
goalList(-1),
isTouch(true),
isUpdateEnd(true),
isPlayerOperation(false),
isSynthesis(false),
isMoveOperation(false),
isAddBingIn(false),
_isRefreshMap(false),
closeMap(false),
isOperation(true),
isTouchInit(false),
moveToX(0),
moveToY(0),
fromX(0),
tox(0),
operationNum(0)
,moveNum(0)
,addNum(0)
,m_removeSbObj(NULL)
,princeNum(0)
,chubingnum(0)
,_dlEfTime(0.0)
,touchInitSb(NULL)
,touchSpXbj(NULL)
,touchSpXbj_(NULL)
,left(NULL)
,needStartShowFriendly(true)
,fourList(NULL)
,twoList(NULL)
,isRemovebegin(false)
,spTouch(NULL)
,needremoveArrow(false)
,isFirstRound(false)
,m_battleInitpve(false)
,m_battleCurstoryId(0)
,battleType(0)
,m_isGuide(false)
,m_showDeletPop(false)
,roundNum(0)
,m_isShowMask(true)
,m_skillDelayCount(0)
,m_aiSkillDelayWatch(0)
,hero_obj(0)
,isme(false)
,m_touchGrid(NULL)
,selectSB(NULL)
,isDeleteOperation(false)
,bboard(NULL)
,m_lastClickj(0)
,oneList(NULL)
,m_heroBuff(NULL)
,m_isStartFillUpSoldier(false)
,m_nTheFormerPosI(8)
,m_autoBattleInfoDisp(false)
,m_sameTimeSB(NULL)
,m_sameTimeWJ(NULL)
,m_nAItips_time(7)
,liansuoChar(NULL)
{
	
    for (int i= 0; i<mapRow; i++) {
        for (int j = 0; j<mapList; j++) {
            fillUnitGrids[i][j] = NULL;
        }
    }
	
    if (touchSpXbj) {
//        touchSpXbj->removeAllChildrenWithCleanup(true);
        touchSpXbj->removeFromParentAndCleanup(true);
        touchSpXbj = NULL;
    }
    if (touchSpXbj_) {
//        touchSpXbj_->removeAllChildrenWithCleanup(true);
        touchSpXbj_->removeFromParentAndCleanup(true);
        touchSpXbj_ = NULL;
    }
    if (spTouch) {
//        spTouch->removeAllChildrenWithCleanup(true);
        spTouch->removeFromParentAndCleanup(true);
        spTouch = NULL;
    }
    
    while(getChildByTag(SPTOUCHTAG))
    {
//        getChildByTag(SPTOUCHTAG)->removeAllChildrenWithCleanup(true);
        getChildByTag(SPTOUCHTAG)->removeFromParentAndCleanup(true);
    }
    
	ResourceManager::sharedInstance()->bindTexture("animationFile/wqlg.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
	ResourceManager::sharedInstance()->bindTexture("animationFile/wjdslv.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
	ResourceManager::sharedInstance()->bindTexture("animationFile/wjdsl.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/szgszg.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/round1-2.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/jiantou_new.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/lsrh_icon.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
}

SGBattleMap::~SGBattleMap()
{
	CCLOG("~SGBattleMap()");
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapList; j++)
        {
            if (m_seveBuffGrid[i][j])
            {
                m_seveBuffGrid[i][j]->release();
            }
        }
    }
    
    clearFillUnitGrids();
    if (waitSoldiers->count())
    {
        waitSoldiers->removeAllObjects();
    }
    CC_SAFE_RELEASE(waitSoldiers);
    
    if (attackLists->count())
    {
        attackLists->removeAllObjects();
    }
    CC_SAFE_RELEASE(attackLists);
    
    if (m_fangYuUpBingArray->count())
    {
        m_fangYuUpBingArray->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_fangYuUpBingArray);
    
    if (m_nomalAttackUpBingArray->count())
    {
        m_nomalAttackUpBingArray->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_nomalAttackUpBingArray);
    
    if (m_spAttackUpArray->count())
    {
        m_spAttackUpArray->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_spAttackUpArray);
    
    if (m_deleteDownArray->count()) {
        m_deleteDownArray->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_deleteDownArray);
    
    if (m_moveAllGrid->count()) {
        m_moveAllGrid->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_moveAllGrid);
    
    if (m_jointlySbArray->count()) {
        m_jointlySbArray->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_jointlySbArray);
    
    if (m_jointlySbArrarCopy->count()) {
        m_jointlySbArrarCopy->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_jointlySbArrarCopy);
    
    if (m_synthesisGrid->count()) {
        m_synthesisGrid->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_synthesisGrid);
    
    if (m_spAttackGrid->count()) {
        m_spAttackGrid->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_spAttackGrid);
    
    if (m_moveSanGrid->count()) {
        m_moveSanGrid->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_moveSanGrid);
    
    if (m_spAttackArray->count()) {
        m_spAttackArray->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_spAttackArray);
    
    if (m_heroBuff->count()) {
        m_heroBuff->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_heroBuff);
    
    if (m_removeSbObj->count()) {
        m_removeSbObj->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_removeSbObj);
    
    if (m_removeSbDef->count()) {
        m_removeSbDef->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_removeSbDef);
    
    if (m_sameTimeSB->count()) {
        m_sameTimeSB->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_sameTimeSB);
    if (m_sameTimeWJ->count()) {
        m_sameTimeWJ->removeAllObjects();
    }
    CC_SAFE_RELEASE(m_sameTimeWJ);

    
    if (fourList->count()) {
        fourList->removeAllObjects();
    }
    CC_SAFE_RELEASE(fourList);
    
    if (twoList->count()) {
        twoList->removeAllObjects();
    }
    CC_SAFE_RELEASE(twoList);
    if (oneList->count()) {
        oneList->removeAllObjects();
    }
    CC_SAFE_DELETE(oneList);
    if (this->soldiers->count()) {
        for (int i = 0; i < this->soldiers->count(); i++) {
            ShiBing *sb = (ShiBing*)this->soldiers->objectAtIndex(i);
            if (sb)
            {
//                CCString *name = CCString::createWithFormat("animationFile/%d_%d.plist", sb->getFileId(), sb->getSbColor());
//                CCString *effName = CCString::createWithFormat("animationFile/%d_eff.plist", sb->getFileId());
//                ResourceManager::sharedInstance()->unBindTexture(name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_soldiersLayer);
//                ResourceManager::sharedInstance()->unBindTexture(effName->getCString(), RES_TYPE_BATTLE_IMAGE, sg_soldiersLayer);
//                sb->removeAllChildrenWithCleanup(true);
                sb->removeFromParentAndCleanup(true);
                sb = NULL;
            }
        }
        this->soldiers->removeAllObjects();
    }
	
    CC_SAFE_RELEASE(soldiers);
    soldiers = NULL;
	
    if (spTouch) {
//        spTouch->removeAllChildrenWithCleanup(true);
        spTouch->removeFromParentAndCleanup(true);
        spTouch = NULL;
    }
}

SGBattleMap* SGBattleMap::create(CCArray* soldiers, bool isme)
{
    SGBattleMap *battleMap = new SGBattleMap();
    if (battleMap && battleMap->init(soldiers, isme))
    {
        battleMap->autorelease();
        return battleMap;
    }
    CC_SAFE_DELETE(battleMap);
    return NULL;
}

bool SGBattleMap::init(CCArray* sbs, bool isme)
{

    if (!CCLayer::init())
    {
        return false;
    }
    
    this->isme = isme;
    
    for (int i = 0; i<mapRow; i++)
    {
        for (int j= 0; j<mapList; j++)
        {
            myGrids[i][j] = NULL;
            myGridsReady[i][j] = 0;
            
        }
    }
    soldiers = CCArray::create();
    soldiers->retain();
    waitSoldiers = CCArray::create();
    waitSoldiers->retain();
    attackLists = CCArray::create();
    attackLists->retain();
    
    fourList=CCArray::create();
    fourList->retain();
    
    twoList=CCArray::create();
    twoList->retain();
    
    oneList=CCArray::create();
    oneList->retain();
    
    this->initMapSBs(sbs);
    
    sbs->removeAllObjects();
    
    m_touchState = eTouchNull;
    
	
    m_fangYuUpBingArray = CCArray::create();
    m_fangYuUpBingArray->retain();
    m_nomalAttackUpBingArray = CCArray::create();
    m_nomalAttackUpBingArray->retain();
    m_spAttackUpArray = CCArray::create();
    m_spAttackUpArray->retain();
    m_deleteDownArray = CCArray::create();
    m_deleteDownArray->retain();
    
    
    m_spAttackArray = CCArray::create();
    m_spAttackArray->retain();
    
    m_jointlySbArray = CCArray::create();
    m_jointlySbArray->retain();
    m_jointlySbArrarCopy = CCArray::create();
    m_jointlySbArrarCopy->retain();
    
    m_moveAllGrid = CCArray::create();
    m_moveAllGrid->retain();
    
    m_synthesisGrid = CCArray::create();
    m_synthesisGrid->retain();
    
    m_spAttackGrid = CCArray::create();
    m_spAttackGrid->retain();
    
    m_moveSanGrid = CCArray::create();
    m_moveSanGrid->retain();
    
    m_removeSbObj = CCArray::create();
    m_removeSbObj->retain();
    
    m_heroBuff = CCArray::create();
    m_heroBuff->retain();
    
    m_removeSbDef = CCArray::create();
    m_removeSbDef->retain();
    m_sameTimeSB = CCArray::create();
    m_sameTimeSB->retain();

	m_sameTimeWJ = CCArray::create();
    m_sameTimeWJ->retain();

	
    
    for (int i = 0; i < mapList; i++)
    {
        for (int j = 0; j < mapRow; j++)
        {
            m_seveBuffGrid[j][i] = NULL;
            SGGridBase *grid = new SGGridBase();
            grid->setStype(knormal);
            grid->initBaffObj();
            GameIndex index = gameIndex(j, i);
            grid->setIndex(index);
            m_seveBuffGrid[j][i] = grid;
        }
    }
    
    left = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("jingyantiao1.png"));
    left->setType(kCCProgressTimerTypeBar);
    left->setMidpoint(ccp(0,0));
    left->setBarChangeRate(ccp(1, 0));
    addChild(left,3,REMOVEPROGRESSTAG);
    
    spTouch = CCSpriterX::create("animationFile/jiantou.scml", true, true);
    spTouch->setAnchorPoint(ccp(0.5f, 0.5f));
    spTouch->setanimaID(0);
    spTouch->setisloop(true);
    spTouch->setVisible(false);
    spTouch->play();
    addChild(spTouch,52,SPTOUCHTAG);
    
	
    m_battleInitpve =  SGBattleManager::sharedBattle()->getHeroLayer(isme)->isInitPve;
    if (isme)
    {
        battleType =  SGBattleManager::sharedBattle()->getBattleType();
        
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        bboard = CCSprite::createWithSpriteFrameName("guide_board_new.png");
        bboard->setScaleX(size.width/bboard->getContentSize().width);
        
        bboard->setAnchorPoint(ccp(0.5,0.5));
        bboard->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 140)));
        bboard->setVisible(false);
        addChild(bboard);
    }
	
    m_battleCurstoryId = SGPlayerInfo::sharePlayerInfo()->getCurrentStoryId();
    m_isGuide = SGGuideManager::shareManager()->isGuide;
    
    if ( isme && m_battleInitpve )
    {
        if (m_battleCurstoryId != 1 && needStartShowFriendly && SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
        {
            isFirstRound=true;
            schedule(schedule_selector(SGBattleMap::hideFriendlyHelp), 0) ;
        }
    }
    
	
    
    return true;
}
//一部分技能发动顺序排序规则
static bool AttackActivitySkillSortRule(const CCObject * obj1, const CCObject * obj2)
{
    SGAttackList *attack1 = (SGAttackList *)obj1;
    SGAttackList *attack2 = (SGAttackList *)obj2;
    if(attack1->getAttackType() != kattacknormal && attack2->getAttackType() == kattacknormal)
    {
        return  true;
    }
    else if(attack1->getAttackType() == kattacknormal && attack2->getAttackType() != kattacknormal)
    {
        return false;
    }
    else if(attack1->getAttackType() != kattacknormal && attack2->getAttackType() != kattacknormal)
    {
        int skillid1 = attack1->getSkillsId();
        int skillid2 = attack2->getSkillsId();
        if(skillid1 != 0 && skillid2 != 0 )
        {
             SGSkillDataModel *skdata1 = SGStaticDataManager::shareStatic()->getGeneralSkillById(skillid1);
             SGSkillDataModel *skdata2 = SGStaticDataManager::shareStatic()->getGeneralSkillById(skillid2);
             if(skdata1&&skdata2)
             {
                 
                 if(skdata1->getSkillPriority()<skdata2->getSkillPriority())
                 {
                     return true;
                 }
                 else if(skdata1->getSkillPriority() == skdata2->getSkillPriority())
                 {
                     if(skdata1->getSkillStarLvl() < skdata2->getSkillStarLvl())
                     {
                         return true;
                     }
                     else if(skdata1->getSkillStarLvl() == skdata2->getSkillStarLvl())
                     {
                         
                         return skdata1->getLevel() <skdata2->getLevel();
                        
                     }
                     else
                     {
                         return false;
                     }

                 }
                 else
                 {
                     return false;
                 }

             }
             else
             {
                 return false;
             }
        }
        else
        {
            return false;
        }
     }
    else
    {
        return false;
    }
    
}

//切换回合的
void SGBattleMap::changeRound( bool isSetTouch, bool ismyRound, bool isFirst)
{

    //pvp中 在删除按钮出现时 切换回合时偶现崩溃
    if (m_touchState == eTouchRemove) {
		        backRemoveSB();
        
    }
    
    if (isSetTouch) {
        if (!ismyRound)
        {
            this->setTouchEnabled(ismyRound && isme);//设置地图是否可触摸(即是玩家的回合,并且是玩家在操作的时候,地图可触摸)
            if (!needStartShowFriendly /* && m_battleCurstoryId != 1 */)
            {
                unschedule(schedule_selector(SGBattleMap::showFriendlyHelp));
                needStartShowFriendly = true;
                hideFriendlyHelp(false);
            }
            return;
        }
        else
        {
            if (needStartShowFriendly /*&& m_battleCurstoryId != 1 */)
            {
                hideFriendlyHelp(false);
            }
        }
		
        if (!isFirst)
        {
            this->isPlayerOperation = false;
            this->m_touchState = eTouchDai;
        }
        this->setTouchEnabled(ismyRound && isme);
        
        if (!this->getAttackList()) {
            //没有攻击阵列，直接设置攻击阵列打出标记
            SGAIManager::shareManager()->setIsCompleteAttack(true);
            CCLOG("BattleMap no attackList setIsCompleteAttack || %d", SGAIManager::shareManager()->getIsCompleteAttack());
            return;
        }
    }
    m_nAItips_time = m_nAItips_time+this->getAttackList()->count();
    //作用在网格上的技能buff激活处理
    this->processOnGridBuff();
    
    //处理攻击阵列上的静态buff
    this->processAttackListStaticBuff();

    //处理攻击阵列每回合触发技能释放
    this->processPerActivitySkill(ismyRound);
    
    //处理攻击阵列蓄力完成是要是激活的非相互作用技能
    this->processActivitySkill_UnInteract();
  
    //墙的防御力变化处理
    this->processWallDef();
    
    bool isRefresh = true;
    //处理攻击阵列攻击力增长更改回合數
    isRefresh = this->processAttackListAtlAdd();

    //遍历每个攻击阵列,然后计算相关加成(如融合, 锁定)
    this->processAttackListChainFuse();
    
    if (!isFirst &&
        isRefresh)
    {
        this->refreshMapGrid(true);
    }
    
    CCObject * obj = NULL;
    bool canAttack = false;
    //检测每个阵列，如果没有回合数为0的，则攻击完成标记直接设置为true
    CCARRAY_FOREACH(this->getAttackList(), obj)
    {
        SGAttackList *attack = static_cast<SGAttackList *>(obj);
        if (attack->getRoundV() <= 0)
        {
            canAttack = true;
            break;
        }
    }
    if (!canAttack && this->getIsMe())
    {
        SGAIManager::shareManager()->setIsCompleteAttack(true);
        CCLOG("检测每个阵列1 setIsCompleteAttack || %d", SGAIManager::shareManager()->getIsCompleteAttack());
    }
    else
    {
        SGAIManager::shareManager()->setIsCompleteAttack(false);
        CCLOG("检测每个阵列2 setIsCompleteAttack || %d", SGAIManager::shareManager()->getIsCompleteAttack());
    }
    
    //如果自动战斗模式，则地图全部可触(为了显示提示自动战斗信息),
    if (SGAIManager::shareManager()->getIsOpenAutoBattle())
    {
        this->setTouchEnabled(true);
    }
    
}
//处理攻击阵列攻击力增长
bool SGBattleMap::processAttackListAtlAdd()
{
    bool isRefresh = true;
    CCObject *obj2 = NULL;
    CCArray * removeAttackLists = CCArray::create();
    
    CCARRAY_FOREACH(this->getAttackList(), obj2)
    {
        SGAttackList *attack = (SGAttackList *)obj2;
        attack->retain();
        
        if (attack->getAp() > 0 ) {
            
            attack->IsRoundBuff = false;
            if (attack->getAddRoundV() > 0)
            {
                int round = attack->getAddRoundV();
                int a = round - 1;
                attack->setAddRoundV(a);
                //每回合增加攻击力
                //attack->changeAP(attack->getAp() + attack->getAddAp());
                
                ///发给服务器日志
                if (isme || SGAIManager::shareManager()->isPVE) {
                    CCString *str = CCString::createWithFormat(str_Format_before_sub_round_attack, attack->getAp());
                    SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
                }
                CCLOG("attack->getAddAp()==%d", attack->getAddAp());
                if (!attack->assistSkills7(this, NULL, NULL)) {
                    attack->changeAP(attack->getAp() + attack->getAddAp());
                }
                if (isme || SGAIManager::shareManager()->isPVE) {
                    CCString *str = CCString::createWithFormat(str_Format_after_sub_round_attack, attack->getAp());
                    SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
                }
                ///
            }
           
            
            //减回合数
            attack->changeRound(-1, false);
            
            attack->subRoundEffect();
            
            if(attack->getRoundV() == 0)
            {
                isRefresh = false;
            }
            
        }
        else
        {
            removeAttackLists->addObject(attack);
            //this->removeAttackList(attack, this->getHero_Obj(), myGrids[attack->getAttackIndex().i][attack->getAttackIndex().j]);
        }
        
        attack->release();
    }
    
    if (removeAttackLists->count() > 0)
    {
        CCObject *obj = NULL;
        CCARRAY_FOREACH(removeAttackLists, obj)
        {
            SGAttackList *attack = (SGAttackList *)obj;
            this->removeAttackList(attack, this->getHero_Obj(), myGrids[attack->getAttackIndex().i][attack->getAttackIndex().j]);
        }
        
    }

    return isRefresh;
}
//遍历每个攻击阵列,然后计算相关加成(如融合, 锁定)
void SGBattleMap::processAttackListChainFuse()
{
    CCObject *obj1 = NULL;
    CCARRAY_FOREACH(this->getAttackList(), obj1)
    {
        SGAttackList *attack = (SGAttackList *)obj1;
        CCLOG("attackRound:%d",attack->getRoundV());
        if (attack->getRoundV() == 0)
        {
            attack->m_effectsLayer->removeAllChildren();
            attack->calculateChainAp(NULL);
            attack->calculateFuseAp(NULL);
            
        }
    }

}


//作用在网格上的技能buff激活处理
void SGBattleMap::processOnGridBuff()
{
    for (int i = 0; i < mapList; i++) {
        for (int j = 0; j < mapRow; j ++) {
            SGGridBase *grid = m_seveBuffGrid[j][i];
            
            if (grid->getBuffList()->count()) {
                CCArray* buffobj = CCArray::create();
                for (int k = 0; k < grid->getBuffList()->count(); k++) {
                    SG_BuffBase* buff = grid->getIndexBuff(k);
                    if (buff->getRoundNum() > 0) {
                        buff->activateBuff(gameIndex(j, i));
                    }
                    
                    if (buff->getRoundNum() <= 0) {
                        buffobj->addObject(buff);
                    }
                }
                
                if (buffobj->count()) {
                    for (int k = 0; k < buffobj->count(); k++) {
                        SG_BuffBase* buff = (SG_BuffBase*) buffobj->objectAtIndex(k);
                        int tag = buff->getEffectType() * BUFF_EFFECT_TAG;
                        if (this->getHero_Obj()->getChildByTag(tag)) {
                            CCSpriterX *sp = (CCSpriterX *)this->getHero_Obj()->getChildByTag(tag);
//                            sp->removeAllChildrenWithCleanup(true);
                            sp->removeFromParentAndCleanup(true);
                            sp = NULL;
                        }
                        int removeID = -1;
                        for (int h = 0; h < grid->getBuffList()->count(); h++) {
                            SG_BuffBase* buff1 = grid->getIndexBuff(h);
                            if (buff == buff1) {
                                removeID = h;
                            }
                        }
                        if (removeID != -1) {
                            grid->getBuffList()->removeObjectAtIndex(removeID);
                        }
                    }
                }
            }
        }
    }
    

}
//处理攻击阵列上的静态buff
void SGBattleMap::processAttackListStaticBuff()
{
    CCObject *obj = NULL;
    
    CCARRAY_FOREACH(this->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        attack->retain();
		//每轮回合检测是否攻击队列可复活,如果有对应的技能并且,revivenum>1, 攻击队列已死的情况就可以对队列进行复活取决于SG_SkillsID21技能
        attack->changeRevive();
        //处理攻击阵列上的buff
        if(attack->getAp() > 0 ){
            
            if (attack->m_attackBuffObj->count()) {
                CCArray *rearr = CCArray::create();
                for (int i = 0; i < attack->m_attackBuffObj->count(); i++) {
                    SG_BuffBase* buff = (SG_BuffBase*)attack->m_attackBuffObj->objectAtIndex(i);
                    if (buff->getRoundNum() == -1 ) {
                        buff->activateBuff(attack);
                    }else if(buff->getRoundNum() > 0)
                    {
                        buff->activateBuff(attack);
                        int num = buff->getRoundNum();
                        num = num - 1;
                        buff->setRoundNum(num);
                        if (num <= 0) {
                            rearr->addObject(buff);
                        }
                    }
                }
                
                if (rearr->count()) {
                    CCObject *obj = NULL;
                    CCARRAY_FOREACH(rearr, obj)
                    {
                        SG_BuffBase *buff = (SG_BuffBase*)obj;
                        attack->m_attackBuffObj->removeObject(buff);
                    }
                }
            }
        }
        
        attack->release();
    }

}
//处理攻击阵列蓄力完成是要是激活的非相互作用技能
void SGBattleMap::processActivitySkill_UnInteract()
{
    CCArray * attackList = CCArray::create();
    attackList->retain();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(this->getAttackList(), obj)
    {
        attackList->addObject(obj);
    }
    //CCArray * attackList = CCArray::createWithArray(this->getAttackList());
    std::sort(attackList->data->arr,attackList->data->arr + attackList->data->num,AttackActivitySkillSortRule);
    CCObject *obj3 = NULL;
    
	//检测每个攻击阵列,并检测技能,画出相关特效
    CCARRAY_FOREACH(attackList, obj3)
    {
        SGAttackList *attack = (SGAttackList *)obj3;
        CCLOG("attackRound:%d",attack->getRoundV());
        if (attack->getRoundV() == 1)
        {
            switch (attack->getSkillsId()) {
                case SKILLS_ID6:
                case SKILLS_ID7:
                case SKILLS_ID9:
                case SKILLS_ID17:
                case SKILLS_ID19:
                case SKILLS_ID23:
                case SKILLS_ID28:
                case SKILLS_ID29:
                case SKILLS_ID35:
                case SKILLS_ID41:
                case SKILLS_ID47:
                case SKILLS_ID39:
                case SKILLS_ID5:
                case SKILLS_ID3:
                {
                    attack->showSkillName();
                }
                    break;
                    
                default:
                    break;
            }
            
            if (attack->getSkillsId() == SKILLS_ID27 ||
                attack->getSkillsId() == SKILLS_ID28 ||
                attack->getSkillsId() == SKILLS_ID94 ||
                attack->getSkillsId() == SKILLS_ID61 ||
                attack->getSkillsId() == SKILLS_ID62 ||
                attack->getSkillsId() == SKILLS_ID63) {
                attack->drawSkillsEffect(14);
            }
            
            
            
            if (attack->m_effectsLayer->getChildByTag(RONGHE_EFFECT_TAG)) {
                CCSpriterX *s = (CCSpriterX*)attack->m_effectsLayer->getChildByTag(RONGHE_EFFECT_TAG);
//                s->removeAllChildrenWithCleanup(true);
                s->removeFromParentAndCleanup(true);
                s = NULL;
            }
            if (attack->m_effectsLayer->getChildByTag(LIANSUO_EFFECT_TAG)) {
                CCSpriterX *s = (CCSpriterX*)attack->m_effectsLayer->getChildByTag(LIANSUO_EFFECT_TAG);
//                s->removeAllChildrenWithCleanup(true);
                s->removeFromParentAndCleanup(true);
                s = NULL;
            }
            
            
            if (attack->getSkillsDelegate()) {
                
                attack->activateSkills3(NULL, attack, 0);
            }
            
            CCObject *sbobj = NULL;
            CCARRAY_FOREACH(attack->attackSbs, sbobj)
            {
                ShiBing *sb = (ShiBing*)sbobj;
                if (!isme) {
                    sb->setZOrder(0);
                }
                
            }
            
        }
    }

    attackList->release();
    
}
//处理墙的防御力变换
void SGBattleMap::processPerActivitySkill(bool ismyRound)
{
    CCArray * attackList = CCArray::create();
    attackList->retain();
     CCObject *obj = NULL;
    CCARRAY_FOREACH(this->getAttackList(), obj)
    {
        attackList->addObject(obj);
    }
    //CCArray * attackList = CCArray::createWithArray(this->getAttackList());
    std::sort(attackList->data->arr,attackList->data->arr + attackList->data->num,AttackActivitySkillSortRule);
    CCObject *obj3 = NULL;
    
    CCARRAY_FOREACH(attackList, obj3)
    {
        SGAttackList *attack = (SGAttackList *)obj3;
        attack->retain();
        
        if (attack->getAp() > 0 ) {
            
            if (attack->getRoundV()>0) {
                switch (attack->getSkillsId()) {
                    case SKILLS_ID1:
                    case SKILLS_ID53:
                    case SKILLS_ID4:
                    case SKILLS_ID15:
                    case SKILLS_ID11:
                    case SKILLS_ID8:
                    case SKILLS_ID2:
                    {
                        attack->showSkillName();
                    }
                        break;
                        
                    default:
                        break;
                }
#pragma mark "---------------------------技能激活----------------------------"
                //attracklist(一个skillManager代理)->skillManager(一个skillbase代理)->skillbase(持有所有skill的实现接口,由子类继承并实现)->skill去调用真正的实现
                
				CCLOG("================开始[]激活技能[应该在复活之后]==================");
				attack->activateSkills( this, attack, 0);
                attack->activateSkills(ismyRound, this, attack, 0);
            }
            
        }
 
        attack->release();
    }
    attackList->release();

}
//处理墙的防御力变换
void SGBattleMap::processWallDef()
{
    for (int i = 0; i < mapRow; i++) {
        for (int j = 0; j < mapList; j++) {
            SGGridBase *grid = myGrids[i][j];
            if (grid &&
                grid->getStype() == kdefend) {
                ShiBing *bing = getShiBingWithGrid(grid);
                if(bing){
                    if (bing->getRoundVAddDef() > 0) {
                        
                        int a = round(bing->getInitDef() * bing->getDefRend()) + round(round(bing->getInitDef() * bing->getDefRend()) * bing->getAddDef());
                        int b = bing->getDef();
                        if (b < a) {
                            int def = round(round(bing->getInitDef() * bing->getDefRend()) * bing->getRoundVAddDef());
                            if (isme || SGAIManager::shareManager()->isPVE) {
                                CCString *defstr = CCString::createWithFormat(str_Format_every_round_before_defense, bing->getDef());
                                SGBattleManager::sharedBattle()->fightLogRequest(defstr->m_sString);
                            }
                            
                            if ((bing->getDef() + def) >= a)
                            {
                                bing->setDef(a);
                            }
                            else
                            {
                                bing->setDef(bing->getDef() + def);
                            }
                            
                            //bing->setDef(bing->getDef() + def);
                            bing->showDef(bing->getDef());
                            bing->setDefImage();
                        }
                    }
				}
            }
        }
    }
    

    
    
}
//根据index获取地图上的兵
ShiBing * SGBattleMap::getShiBingWithIndex(GameIndex index)
{
    
    if (index.i < 0
        || index.i >= mapRow
        || index.j < 0
        || index.j >= mapList)
    {
        return  NULL;
    }
    SGGridBase *grid = (SGGridBase *)myGrids[index.i][index.j];
    return getShiBingWithGrid(grid);
    
}
//根据网格位置获取网格上的士兵
ShiBing * SGBattleMap::getShiBingWithGrid(SGGridBase *grid)
{
    if (!grid) {
        return NULL;
    }
    CCObject *si = NULL;
    CCARRAY_FOREACH(soldiers, si)
    {
        ShiBing *sb = (ShiBing *)si;
        if (sb->isGridIn(grid))//士兵是否在网格中
        {
            return sb;
        }
    }
    return NULL;
}
//根据index获取网格上的士兵
ShiBing * SGBattleMap::getShiBingByIndex(int index)
{
    if (index < 0 || index >= soldiers->count())
    {
        return NULL;
    }
    
    return (ShiBing *)soldiers->objectAtIndex(index);
}

int SGBattleMap::getShiBingCount()
{
    return soldiers->count();
}

//通过网格获取当前网格所在攻击阵列
SGAttackList* SGBattleMap::getAttackListByGrid(SGGridBase *grid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackLists, obj)
    {
        SGAttackList *attackList = (SGAttackList*)obj;
        if (attackList->isGridin(grid))//查看攻击阵列中的每一个兵是否在这个网格中
        {
            return attackList;
        }
    }
    return NULL;
}
//填充指定的网格
void SGBattleMap::initGrid(SGGridBase *grid)
{
    myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
}
//初始化兵,通过传过来的数据,初始化到地图上
void SGBattleMap::initMapSBs(CCArray *sbdatas)
{
    for (int k = 0; k<sbdatas->count(); k++)
    {
        SGSBObj *data = (SGSBObj *)sbdatas->objectAtIndex(k);
        switch (data->getType())
        {
				//如果是散兵
            case ksbone:
            {
                ShiBing *sb = ShiBing::createWithData(data,this,this->isme);
                this->addChild(sb, SB_TAG);
                sb->setBingZOrder();
                GameIndex index=sb->getGridIndex();
                SGGridBase *bingGrid=this->myGrids[index.i][index.j];
				//格子非空,并且是正常可放置兵的位置
                if (bingGrid && bingGrid->getStype()==knormal)
                {
					oneList->addObject(sb);
                }
                
                bool repair = SGBattleManager::sharedBattle()->getBattleLayer()->getIsRepairMap();
                if (repair)
                {
                    soldiers->addObject(sb);
                }
                else
                {
                    waitSoldiers->addObject(sb);
                }
            }
                break;
				//如果是二格武将
            case ksbtwo:
            {
                SGPrinceBase *sb = SGPrinceBase::createWithData(data ,this, this->isme);
                this->addChild(sb, SB_TAG);
                sb->setBingZOrder();
                SGGridBase *bingGrid=sb->getNumGrid(kup);
				
                if (bingGrid && bingGrid->getStype()==knormal)
                {
                    twoList->addObject(sb);
                }
                bool repair = SGBattleManager::sharedBattle()->getBattleLayer()->getIsRepairMap();
                if (repair)
                {
                    soldiers->addObject(sb);
                }
                else
                {
                    waitSoldiers->addObject(sb);
                }
            }
                break;
				//四格武将
            case ksbfour:
            {
                SGSuperPrinceBase *wujiang4 = SGSuperPrinceBase::createWithData(data,this, this->isme);
                this->addChild(wujiang4, SB_TAG);
                wujiang4->setBingZOrder();
                SGGridBase *bingGrid=wujiang4->getNumGrid(kleftup);
                if (bingGrid && bingGrid->getStype()==knormal)
                {
                    fourList->addObject(wujiang4);
                }
                
                
                bool repair = SGBattleManager::sharedBattle()->getBattleLayer()->getIsRepairMap();
                if (repair)
                {
                    soldiers->addObject(wujiang4);
                }
                else
                {
                    waitSoldiers->addObject(wujiang4);
                }
            }
                break;
            default:
                
                break;
        }
    }
	setIsMoveAble();
    
}

//网格对象加入到数组里
void SGBattleMap::indexAddToArray(SGGridBase *gird, cocos2d::CCArray *array)
{
    array->addObject(gird);
}
//删除网格,士兵消除
void SGBattleMap::removeGrids(cocos2d::CCArray *array)
{
    if (!array) {
        return;
    }
    
    bool isFriend = false;
    
    for (int i = 0; i < array->count(); i++)
    {
        ShiBing* sb = (ShiBing*) array->objectAtIndex(i);
        
        int indexi = -1;
        
        GameIndex lie = gameIndex(-1, -1);
        
        for (int i = 0; i < sb->getPrinceGridArray()->count(); i++)
        {
            SGGridBase *grid = (SGGridBase *)sb->getPrinceGridArray()->objectAtIndex(i);
            
            if (indexi <= grid->getIndex().i)
            {
                indexi = grid->getIndex().i;
            }
            
            if (lie.i != grid->getIndex().j)
            {
                lie.j = lie.i;
                lie.i = grid->getIndex().j;
            }
        }
        
        if (indexi < (mapRow - 1))
        {
            for (int i = indexi + 1; i < mapRow; i++)
            {
                
                SGGridBase *grid = myGrids[i][lie.i];
                if (grid)
                {
                    ShiBing *bing = getShiBingWithGrid(grid);
                    if (bing)
                    {
                        switch (bing->getSbType())
                        {
                            case ksbone:
                            {
                                m_deleteDownArray->addObject(bing);
                            }
                                break;
                            case ksbtwo:
                            {
                                i += 1;
                                m_deleteDownArray->addObject(bing);
                                
                                
                            }
                                break;
                            case ksbfour:
                            {
                                i += 1;
                                m_deleteDownArray->addObject(bing);
                                
                                
                                
                                SGGridBase * spgrid = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(2);//四格将占四个格的后左一个
                                SGGridBase * spgrid1 = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(3);//后右一个
                                
                                if (lie.i > spgrid->getIndex().j)
                                {
                                    int rand = 0;
                                    if (lie.i > 0)
                                    
                                    {
                                        rand = lie.i - 1;
                                    }else
                                    {
                                        rand = lie.i;
                                    }
                                    for (int h = spgrid->getIndex().i + 1; h < mapRow; h++)
                                    {
                                        SGGridBase *grid = myGrids[h][rand];
                                        if (grid)
                                        {
                                            ShiBing *bing = getShiBingWithGrid(grid);
                                            if(bing)
                                            {
                                                switch (bing->getSbType())
                                                {
                                                    case ksbone:
                                                    {
                                                        m_deleteDownArray->addObject(bing);
                                                    }
                                                        break;
                                                    case ksbtwo:
                                                    {
                                                        h += 1;//纵向多跳过一个格,二格和四格武将都纵向占二个格
                                                        m_deleteDownArray->addObject(bing);
                                                    }
                                                        break;
                                                    case ksbfour:
                                                    {
                                                        h += 1;
                                                        m_deleteDownArray->addObject(bing);
                                                        SGGridBase * spgrid = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(2);
                                                        if (rand > spgrid->getIndex().j)
                                                        {
                                                            for (int k = spgrid->getIndex().i + 1; k < mapRow; k++)
                                                            {
                                                                SGGridBase *grid = myGrids[k][spgrid->getIndex().j];
                                                                if (grid)
                                                                {
                                                                    ShiBing *bing = getShiBingWithGrid(grid);
                                                                    if(bing)
                                                                    {
                                                                        switch (bing->getSbType())
                                                                        {
                                                                            case ksbone:
                                                                            {
                                                                                //占一格士兵直接放入
                                                                                m_deleteDownArray->addObject(bing);
                                                                            }
                                                                                break;
                                                                            case ksbtwo:
                                                                            {
                                                                                k+=1;//继续向下多跳一个格,二格武将多纵向多占一个格
                                                                                m_deleteDownArray->addObject(bing);
                                                                            }
                                                                                break;
                                                                                
                                                                            default:
                                                                                break;
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        
                                                    }
                                                        break;
                                                        
                                                    default:
                                                        break;
                                                }
                                            }
                                        }
                                        
                                    }
                                }
                                if (lie.i < spgrid1->getIndex().j)
                                {
                                    int rand = 0;
                                    if (lie.i < mapList - 1 )
                                    {
                                        rand = lie.i + 1;
                                    }
                                    else
                                    {
                                        rand = lie.i;
                                    }
                                    for (int h = spgrid1->getIndex().i + 1; h < mapRow; h++)
                                    {
                                        SGGridBase *grid = myGrids[h][rand];
                                        if (grid)
                                        {
                                            ShiBing *bing = getShiBingWithGrid(grid);
                                            if(bing)
                                            {
                                                switch (bing->getSbType())
                                                {
                                                    case ksbone:
                                                    {
                                                        m_deleteDownArray->addObject(bing);
                                                    }
                                                        break;
                                                    case ksbtwo:
                                                    {
                                                        h += 1;
                                                        m_deleteDownArray->addObject(bing);
                                                    }
                                                        break;
                                                    case ksbfour:
                                                    {
                                                        h += 1;
                                                        m_deleteDownArray->addObject(bing);
                                                        
                                                        SGGridBase * spgrid1 = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(3);
                                                        if (rand < spgrid1->getIndex().j) {
                                                            for (int k = spgrid1->getIndex().i + 1; k < mapRow; k++) {
                                                                SGGridBase *grid = myGrids[k][spgrid1->getIndex().j];
                                                                if (grid) {
                                                                    ShiBing *bing = getShiBingWithGrid(grid);
                                                                    if(bing)
                                                                    {
                                                                        switch (bing->getSbType()) {
                                                                            case ksbone:
                                                                            {
                                                                                m_deleteDownArray->addObject(bing);
                                                                            }
                                                                                break;
                                                                            case ksbtwo:
                                                                            {
                                                                                k+=1;
                                                                                m_deleteDownArray->addObject(bing);
                                                                            }
                                                                                break;
                                                                                
                                                                            default:
                                                                                break;
                                                                        }
                                                                    }
                                                                    
                                                                }
                                                            }
                                                        }
                                                        
                                                    }
                                                        break;
                                                        
                                                    default:
                                                        break;
                                                }
                                            }
                                        }
                                        
                                    }
                                }
                                
                                
                            }
                                break;
                                
                            default:
                                break;
                        }
                    }
                    
                    
                    
                }
            }
        }
        
        switch (sb->getSbType()) {
            case ksbfour:
            {
                if (indexi < (mapRow - 1)) {
                    for (int i = indexi + 1; i < mapRow; i++) {
                        
                        SGGridBase *grid = myGrids[i][lie.j];
                        if (grid) {
                            ShiBing *bing = getShiBingWithGrid(grid);
                            if(bing)
                                m_deleteDownArray->addObject(bing);
                            
                        }
                    }
                }
            }
                break;
                
            default:
                break;
        }
        
        
        m_removeSbObj->addObject(sb);
        if (sb->getSbType() == ksbone) {
            GameIndex index=sb->getGridIndex();
            SGGridBase *bingGrid=this->myGrids[index.i][index.j];
            if (bingGrid && bingGrid->getStype()==knormal) {
                oneList->removeObject(sb);
            }
            
        }else{
            
            if (sb->getSbType()==ksbfour) {
                //如果删除的没有形成蓄力的四格格武将，就删除
                SGSuperPrinceBase *fourSolder=(SGSuperPrinceBase *)sb;
                SGGridBase *bingGrid=fourSolder->getNumGrid(kleftup);
                if (bingGrid && bingGrid->getStype()==knormal) {
                    fourList->removeObject(fourSolder);
                }
            }
            if (sb->getSbType()==ksbtwo) {
                //如果删除的没有形成蓄力的二格武将，就删除
                SGPrinceBase *twoSolder=(SGPrinceBase *)sb;
                SGGridBase *bingGrid=twoSolder->getNumGrid(kup);
                if (bingGrid && bingGrid->getStype()==knormal) {
                    twoList->removeObject(twoSolder);
                }
                
            }
            isFriend = sb->getFriendOff();
        }
        for (int i = 0; i < sb->getPrinceGridArray()->count(); i++) {
            SGGridBase * spgrid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(i);
            myGrids[spgrid->getIndex().i][spgrid->getIndex().j] = NULL;
        }
        sb->cleanSbFile();
//        sb->removeAllChildrenWithCleanup(true);
        sb->removeFromParentAndCleanup(true);
        deleteSoldiersObj(sb);
        sb = NULL;
    }
    
    
    m_moveAllGrid->addObjectsFromArray(m_deleteDownArray);
    
    
    for (int i = 0; i < array->count(); i++) {
        ShiBing* sb = (ShiBing*) array->objectAtIndex(i);
        for (int i = 0; i < sb->getPrinceGridArray()->count(); i++) {
            SGGridBase *grid = (SGGridBase *)sb->getPrinceGridArray()->objectAtIndex(i);
            myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
        }
    }
    
    array->removeAllObjects();
    
    SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
    heroLayer->changeSoldierNum(0);
    if (isFriend) {
        heroLayer->isFriendOffDeath = false;
    }
}
//开始向地图上布兵
void SGBattleMap::chubing()
{
    //GPCCLOG("SGBattleMap::chubing 出兵\n");
    m_isStartFillUpSoldier = true;
	//计算出位置在最前面的士兵的i值
    for (int i = 0; i<waitSoldiers->count(); i++)
    {
        ShiBing *shibing = (ShiBing *)waitSoldiers->objectAtIndex(i);
        if(m_nTheFormerPosI > shibing->getSbindex().i)
        {
            m_nTheFormerPosI = shibing->getSbindex().i;
        }
        
    }
    //把没有上场的士兵加入到地图上去
    for (int i = 0; i < waitSoldiers->count(); i++)
    {
        ShiBing *shibing = (ShiBing *)waitSoldiers->objectAtIndex(i);
		//地图上的士兵们
        soldiers->addObject(shibing);
		//开始向地图上出兵,包括动画(应该是)
        shibing->moveToSceneCall(this, isAddBingIn ? callfuncN_selector(SGBattleMap::addBingEnd) : callfuncN_selector(SGBattleMap::chubingEnd));
    }
    if (!isAddBingIn)
    {
        waitSoldiers->removeAllObjects();
    }
    m_isStartFillUpSoldier = false;
    m_nTheFormerPosI = 8;
    setIsMoveAble();
}

//remove军旗????
void SGBattleMap::removeJQ()
{
    SGHeroLayer *heroLayer = this->getHero_Obj();
    if (heroLayer->getChildByTag(TONGJING_EFFECT_TAG + FLAG)) {
        CCSpriterX *spx = (CCSpriterX*)heroLayer->getChildByTag(TONGJING_EFFECT_TAG + FLAG);
//        spx->removeAllChildrenWithCleanup(true);
        spx->removeFromParentAndCleanup(true);
        spx = NULL;
        heroLayer->removeChildByTag(TONGJING_EFFECT_TAG + FLAG, true);
    }
}

//首次出兵结束??
void SGBattleMap::chubingEnd(cocos2d::CCObject *obj)
{
    //GPCCLOG("chubingEnd 出兵结束");
    SGHeroLayer *heroLayer = this->getHero_Obj();
    if (heroLayer->getChildByTag(TONGJING_EFFECT_TAG + FLAG))    //旗帜
    {
        CCSpriterX *spx = (CCSpriterX*)heroLayer->getChildByTag(TONGJING_EFFECT_TAG + FLAG);
        spx->removeFromParentAndCleanup(true);
        spx = NULL;
        heroLayer->removeChildByTag(TONGJING_EFFECT_TAG + FLAG, true);
    }
    this->removeChildByTag(1024, true);
    if (spTouch->isVisible())
    {
        spTouch->setVisible(false);
    }
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
    if (hero->statusBarLayer)
    {
        hero->statusBarLayer->setVisible(false);
        hero->statusBarLayer->setColor(ccWHITE);
        hero->gbSprite->stopAllActions();
        hero->gbSprite->setVisible(false);
    }
    chubingnum++;
	//应该将地图上的兵派上之后,出兵数量置0
    if (chubingnum == soldiers->count())
    {
        chubingnum=0;
        //GPCCLOG("chubingEnd");
        if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
        {
            SGGuideManager::shareManager()->startFightGuide(7);
        }
        else
        {
			//GPCCLOG("7th Guide in [%s-%d]", __FUNCTION__, __LINE__);
			
            SGGuideManager::shareManager()->startAutoGuide(7);
        }
    }
}

//援兵结束
void SGBattleMap::addBingEnd(CCObject *obj )
{
    //GPCCLOG("addBingEnd");
    bool isRefresh = true;
    this->princeNum++;
    SGHeroLayer *heroLayer = SGBattleManager::sharedBattle()->getHeroLayer(isme);

    if (heroLayer->getChildByTag(TONGJING_EFFECT_TAG + FLAG))
    {
        CCSpriterX *spx = (CCSpriterX*)heroLayer->getChildByTag(TONGJING_EFFECT_TAG + FLAG);
        spx->removeFromParentAndCleanup(true);
        spx = NULL;
        heroLayer->removeChildByTag(TONGJING_EFFECT_TAG + FLAG, true);
    }

    if (heroLayer->getCountry() == 1)
    {
        //有数据才进行操作
        if (waitSoldiers && waitSoldiers->data && waitSoldiers->data->arr)
        {
            ShiBing *shibing = (ShiBing *)waitSoldiers->objectAtIndex(addNum);
            if (shibing->getSbType() == ksbfour)
            {
                CCArray *sbar =  SGPlayerInfo::sharePlayerInfo()->getEmbattleOfficers();//获取上阵的将
                int a = sbar->count();
                if (a == this->princeNum)
                {// 武将,并且是最后一个出的
                    if (sbar->count() >= 10)
                    {
                        SGOfficerCard *oc = (SGOfficerCard*) sbar->objectAtIndex(sbar->count() - 1);
                        //如果武将是五星以上的,设置回合数1
                        if (oc->getCurrStar() >= 5)
                        {
                            
                            shibing->setRoundV(1);
                        }
                    }
                }
            }
        }
		
    }
    
    addNum++;
	//待出兵都派到地图上了,重置等待区士兵
    if (addNum == waitSoldiers->count())
    {
        addNum = 0;
        waitSoldiers->removeAllObjects();
        isAddBingIn = false;
        if (isRefresh)
        {
            refreshMapGrid(true);
        }
    }
    
}

void SGBattleMap::qzFillUnit(SEL_CallFuncN selector)
{
	//将等待区士兵派到地图上去
    for (int i = 0; i<waitSoldiers->count(); i++)
    {
        ShiBing *shibing = (ShiBing *)waitSoldiers->objectAtIndex(i);
        soldiers->addObject(shibing);
        shibing->moveToSceneCall(this,selector);
    }
    
    waitSoldiers->removeAllObjects();
}

void SGBattleMap::qzEnd()
{
    SGHeroLayer *hero = this->getHero_Obj();
    hero->stateUpdateTime();
}

//将移除的兵从地图上的兵中去除
void SGBattleMap::deleteSoldiersObj(ShiBing *sb)
{
    CCLOG("删除前是%d个兵", soldiers->count());
    if (m_removeSbObj->count()) {
        for (int i = 0; i < m_removeSbObj->count(); i++) {
            ShiBing* bing = (ShiBing*)m_removeSbObj->objectAtIndex(i);
            if (soldiers->count())
            {
                //bing->stopAllActions();
				soldiers->removeObject(bing);
            }
        }
    }
    if(sb)
    {
        if (soldiers->count())
        {
            //sb->stopAllActions();
			soldiers->removeObject(sb);
        }
    }
    CCLOG("删除后是%d个兵", soldiers->count());
    m_removeSbObj->removeAllObjects();
}


void SGBattleMap::removeSB()
{
    //1300号消息   新手引导  删除成功
    if (SGGuideManager::shareManager()->isGuide )
    {
        SGMainManager::preProcessDataInFightSceneForSend(3);
    }
    
    
    
	//需要显示操作提示信息
    needStartShowFriendly=true;
    //隐藏操作提示
	this->hideFriendlyHelp();
    
    bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    removeChildByTag(RemoveMenuTag, true);
    ShiBing *soldier = getShiBingWithIndex(m_touchPosIndex);
    if (touchSpXbj) {
//        touchSpXbj->removeAllChildrenWithCleanup(true);
        touchSpXbj->removeFromParentAndCleanup(true);
    }
    touchSpXbj = NULL;
    soldier->removeChildByTag(Delete_Red_Rect, true);//对应的待删除的红底也去掉
    if (touchSpXbj_) {
//        touchSpXbj_->removeAllChildrenWithCleanup(true);
        touchSpXbj_->removeFromParentAndCleanup(true);
    }
    touchSpXbj_ = NULL;
    soldier->removeChildByTag(Delete_Red_Rect2, true);
    if (!round) {
        m_touchState = eTouchNull;
        isUpdateEnd = false;
        return;
    }
    if (round &&
        isme) {
        SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
        heroLayer->endRoundUpdataTime();
    }
    m_touchState = eTouchRemoveIn;
    isUpdateEnd = false;
    deleteSbObj(m_touchPosIndex.i, m_touchPosIndex.j);
}
//展示删除士兵的特效
void SGBattleMap::showRemoveEff(cocos2d::CCObject *obj)
{
    ShiBing *sb = (ShiBing*) obj;
    sb->removeSbEffect();
}

//删除真正的士兵对象(从地图上视觉上移除这个兵)
void SGBattleMap::deleteSbObj(int i, int j, float _t)
{
    SGGridBase *grid = myGrids[i][j];
    if (grid)//点击的位置有格子
    {
        if (grid->getStype() == knormal ||
            grid->getStype() == kdefend ||
            closeMap)
        {
            isPlayerOperation = true;
            isMoveOperation = false;
            ShiBing *sb = getShiBingWithGrid(grid);
            if(sb)
            {
                sb->retain();//在callRemoveSb()里release
                m_removeSbObj->addObject(sb);
                removeSbObj(sb);
                CCDelayTime *_time = CCDelayTime::create(_t);
                CCCallFuncN *_call = CCCallFuncN::create(this, callfuncN_selector(SGBattleMap::showRemoveEff));//显示移除特效
                CCDelayTime *time = CCDelayTime::create(0.5);
                CCCallFuncND* call = CCCallFuncND::create(this, callfuncND_selector(SGBattleMap::callRemoveSb), grid);//去掉士兵
                sb->runAction(CCSequence::create( _time, _call,  time, call, NULL));
            }
        }
    }
    
}
//删除sb这个士兵对象,(在deleteSBOBJ中进行,在删除动画进行的时候,删除这个士兵对象)
void SGBattleMap::removeSbObj(ShiBing *sb)
{
    
	//    sb->removeSbEffect(sb->getSbType());
    
    int indexi = -1;
    
    GameIndex lie = gameIndex(-1, -1);
    
    for (int i = 0; i < sb->getPrinceGridArray()->count(); i++) {
        SGGridBase *grid = (SGGridBase *)sb->getPrinceGridArray()->objectAtIndex(i);
        
        myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
        
        if (indexi <= grid->getIndex().i) {
            indexi = grid->getIndex().i;
        }
        
        if (lie.i != grid->getIndex().j) {
            lie.j = lie.i;
            lie.i = grid->getIndex().j;
        }
    }
    if (closeMap) return;
    
    if (indexi < (mapRow - 1)) {
        for (int i = indexi + 1; i < mapRow; i++) {
            SGGridBase *grid = myGrids[i][lie.i];
            if (grid) {
                ShiBing *bing = getShiBingWithGrid(grid);
                if(bing)
                {
                    switch (bing->getSbType()) {
                        case ksbone:
                        {
                            m_deleteDownArray->addObject(bing);
                        }
                            break;
                        case ksbtwo:
                        {
                            i += 1;
                            m_deleteDownArray->addObject(bing);
                        }
                            break;
                        case ksbfour:
                        {
                            i += 1;
                            m_deleteDownArray->addObject(bing);
                            SGGridBase * spgrid = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(2);
                            SGGridBase * spgrid1 = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(3);
                            if (lie.i == spgrid->getIndex().j) {
                                lie.j = spgrid1->getIndex().j;
                            }else{
                                lie.j = spgrid->getIndex().j;
                            }
                            
                            for (int h = spgrid->getIndex().i + 1; h < mapRow; h++) {
                                SGGridBase *grid = myGrids[h][lie.j];
                                if (grid) {
                                    ShiBing *bing = getShiBingWithGrid(grid);
                                    if(bing)
                                        m_deleteDownArray->addObject(bing);
                                }
                                
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
    
    
    switch (sb->getSbType()) {
			
			//是四格武将
        case ksbfour:
        {
			//四格武将还没有在最后一列
            if (indexi < (mapRow - 1)) {
				// 从这个武将向后检测,把这个武将之后的兵都存起来
                for (int i = indexi + 1; i < mapRow; i++) {
                    
                    SGGridBase *grid = myGrids[i][lie.j];
                    if (grid) {
                        ShiBing *bing = getShiBingWithGrid(grid);
                        if(bing)
                            m_deleteDownArray->addObject(bing);
                        
                    }
                }
            }
        }
            break;
            
        default:
            break;
    }
    
    
	//    for (int i = 0; i < m_deleteDownArray->count(); i++) {
	//        ShiBing *sb = (ShiBing*)m_deleteDownArray->objectAtIndex(i);
	//        SGGridBase *grid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
	//        CCLOG("grid.i===%d, grid.j===%d",grid->getIndex().i, grid->getIndex().j);
	//    }
    
    
	//    sb->removeFromParentAndCleanup(true);
	
}
//删除士兵
void SGBattleMap::callRemoveSb(cocos2d::CCObject *obj, SGGridBase *grid)
{
    //GPCCLOG("删除士兵");
    if (grid) {
        if (grid->getStype() == kdefend) {
            EFFECT_PLAY(MUSIC_8);
        }
        else
        {
            EFFECT_PLAY(MUSIC_17);
        }
    }
    ShiBing *sb = ((ShiBing*)obj);
    sb->release();
    if (sb->getSbType() != ksbone) {
        if (sb->getSbType()==ksbfour) {
            //如果删除的没有形成蓄力的四格格武将，就删除
            SGSuperPrinceBase *fourSolder=(SGSuperPrinceBase *)sb;
            SGGridBase *bingGrid=fourSolder->getNumGrid(kleftup);
            if (bingGrid->getStype()==knormal) {
                fourList->removeObject(fourSolder);
                CCLog("删除的没有形成蓄力的四格格武将");
            }
        }
        if (sb->getSbType()==ksbtwo) {
            //如果删除的没有形成蓄力的二格武将，就删除
            SGPrinceBase *twoSolder=(SGPrinceBase *)sb;
            SGGridBase *bingGrid=twoSolder->getNumGrid(kup);
            if (bingGrid->getStype()==knormal) {
                twoList->removeObject(twoSolder);
                CCLog("删除的没有形成蓄力的二格格武将");
            }
			
        }
    }
    else{
        //Grid可能会出现空指针//10.23//george
        if (sb->getSbType()==ksbone && grid) {
            
            if (grid->getStype()==knormal) {
                oneList->removeObject(sb);
                //CCLog("删除的没有形成蓄力的一格武将");
            }
        }
    }
    sb->cleanSbFile();
    
    if (sb->getSbType() != ksbone) {
        SGOfficerDataModel *officerData=SGStaticDataManager::shareStatic()->getOfficerById(sb->getSbId());
        if (officerData) {
            int num=officerData->getSoundId();
            CCString *name = CCString::createWithFormat("music/%d_Dead.mp3",num);
            EFFECT_PLAY(name->getCString());
        }
    }
    
    bool isfrendoff = sb->getFriendOff();
//    sb->removeAllChildrenWithCleanup(true);
    sb->removeFromParentAndCleanup(true);
    sb = NULL;
    deleteSwapGridObj(m_deleteDownArray);
    
    if (this->closeMap) {
        deleteSoldiersObj(((ShiBing*)obj));
        return;
    }
    
    if (grid) {
        
		//        SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
        SGHeroLayer *heroLayer = SGBattleManager::sharedBattle()->getHeroLayer(this->isme);
        heroLayer->changeSoldierNum(0);
        if (isfrendoff) {
            heroLayer->isFriendOffDeath = false;
        }
        
        if (isme || SGAIManager::shareManager()->isPVE)
        {
            GameIndex deleteIndex = gameIndex(-1, -1);
            switch (grid->getSbType())
			{
                case ksbone:
                {
                    deleteIndex = gameIndex(grid->getIndex().i, grid->getIndex().j);
                }
                    break;
                case ksbtwo:
                {
                    SGPrinceBase *pSb = (SGPrinceBase*)((ShiBing*)obj);
                    SGGridBase *grid = pSb->getNumGrid(kup);
                    deleteIndex = gameIndex(grid->getIndex().i, grid->getIndex().j);
                }
                    break;
                    
                case ksbfour:
                {
                    SGSuperPrinceBase *suSb = (SGSuperPrinceBase*)((ShiBing*)obj);
                    SGGridBase *grid = suSb->getNumGrid(kleftup);
                    deleteIndex = gameIndex(grid->getIndex().i, grid->getIndex().j);
                }
                    break;
                    
                default:
                    break;
            }
            if (deleteIndex.i != -1)
			{
                SGBattleManager::sharedBattle()->deleteRequest(deleteIndex.j, deleteIndex.i);
            }
        }
        
        deleteSoldiersObj(((ShiBing*)obj));
        
    }
    
    
    //    isDeleteOperation = true;
    
    if (m_moveAllGrid->count()) {
		//        schedule(schedule_selector(SGBattleMap::deleteSbMap) , RntervalTime);
        schedule(schedule_selector(SGBattleMap::deleteMoveEndDetection) , RntervalTime + sbMoveSpeed);
    }else{
        refreshMapGrid(true);
    }
	
}

void SGBattleMap::deleteSbMap()
{
    unschedule(schedule_selector(SGBattleMap::deleteSbMap));
    m_moveAllGrid->removeAllObjects();
    _isRefreshMap = false;
    
    //CCArray *allsb = CCArray::create();
	//检测每一个网格,得到每一个散兵
    for (int i = 0; i < mapRow; i++) {
        for (int j = 0; j < mapList; j++) {
            SGGridBase *grid = myGrids[i][j];
            if (grid) {
                ShiBing *bing = getShiBingWithGrid(grid);
                if(bing)
                {
                    if (grid->getStype() == knormal) {
                        m_moveAllGrid->addObject(bing);
                    }
                }
            }
        }
    }
	//    deleteSwapGridObj(allsb);
    schedule(schedule_selector(SGBattleMap::deleteMoveEndDetection) , RntervalTime + sbMoveSpeed);
}
//检查四格武将是不是能移动,检测其占四格的后二个格是不是有兵
bool SGBattleMap::chackSPMove(ShiBing *bing)
{
    SGGridBase *grid = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
    SGGridBase *grid1 = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(1);
	
    if (NULL == myGrids[grid->getIndex().i - 1][grid->getIndex().j] &&
        NULL == myGrids[grid1->getIndex().i - 1][grid1->getIndex().j]) {
        return true;
    }
    
    return false;
}
//删除单个的士兵,长按删除
void SGBattleMap::backRemoveSB()
{
    unschedule(schedule_selector(SGBattleMap::upRemoveT));
    removeChildByTag(RemoveMenuTag, true);
    ShiBing *soldier = getShiBingWithIndex(m_touchPosIndex);
    if (touchSpXbj) {
//        touchSpXbj->removeAllChildrenWithCleanup(true);
        touchSpXbj->removeFromParentAndCleanup(true);
    }
    touchSpXbj = NULL;
    soldier->removeChildByTag(Delete_Red_Rect, true);
    if (touchSpXbj_) {
//        touchSpXbj_->removeAllChildrenWithCleanup(true);
        touchSpXbj_->removeFromParentAndCleanup(true);
    }
    touchSpXbj_ = NULL;
    soldier->removeChildByTag(Delete_Red_Rect2, true);
    m_touchState = eTouchNull;
    m_touchPosIndex = gameIndex(-1, -1);
    
}
//攻击修正
void SGBattleMap::repairAttack(cocos2d::CCDictionary *data, cocos2d::CCArray *array)
{
	
	/*
	 
	 DATA1->DATA9
	 
	 attack.x()), "DATA1");
	 attack.y()), "DATA2");
	 attack.currentround()), "DA
	 attack.currentattack()), "D
	 attack.currentchaincount())
	 attack.currentmergecount())
	 attack.acceptamage()), "DAT
	 attack.ishasfuhuo()), "DATA
	 attack.isfuhuo()), "DATA9")
	 
	 
	 */
	// 根据字典里的数据修正攻击阵列的数值
    int DATA1 = ((CCString*)data->objectForKey("DATA1"))->intValue();
    int DATA2 = ((CCString*)data->objectForKey("DATA2"))->intValue();
    int DATA3 = ((CCString*)data->objectForKey("DATA3"))->intValue();
    int DATA4 = ((CCString*)data->objectForKey("DATA4"))->intValue();
    int DATA5 = ((CCString*)data->objectForKey("DATA5"))->intValue();
    int DATA6 = ((CCString*)data->objectForKey("DATA6"))->intValue();
    int DATA7 = ((CCString*)data->objectForKey("DATA7"))->intValue();
    bool DATA8 = ((CCString*)data->objectForKey("DATA8"))->boolValue();
    int DATA9 = ((CCString*)data->objectForKey("DATA9"))->intValue();
    
    
    SGGridBase *grid = myGrids[DATA2][DATA1];
    if (grid) {
        SGAttackList *attack = NULL;
        ShiBing *s = getShiBingWithGrid(grid);//
        if(s)
            s->setRoundV(DATA3);
        if (grid->getSbType() == ksbone) {
            CCArray *_array = CCArray::create();
            _array->addObject(s);
            SGGridBase *_grid = myGrids[DATA2 + 1][DATA1];
            ShiBing *_s = getShiBingWithGrid(_grid);//
            if(_s)
            {
                _array->addObject(_s);
                _s->setRoundV(DATA3);
            }
            
            SGGridBase *grid_ = myGrids[DATA2 + 2][DATA1];
            ShiBing *s_ = getShiBingWithGrid(grid_);//
            if(s_)
            {
                _array->addObject(s_);
                s_->setRoundV(DATA3);
            }
            SGNormalAttackList *al = SGNormalAttackList::createWithSbs(_array,this->isme);
            attack = al;
            
        }
        else if(grid->getSbType() == ksbtwo)
        {
            SGTwoAttackList *al = SGTwoAttackList::createWithSoldier(s, this->isme);
            attack = al;
			
            
        }else{
            SGFourAttackList *al = SGFourAttackList::createWithSoldier(s, this->isme);
            attack = al;
			
        }
        
        
        attack->setRoundV(DATA3);
        attack->changeAP(DATA4);
        attack->setAddChain(DATA5);
        
		
        //连锁的个数
        if (DATA5 > 0) {
			
            
            CCSpriterX *s = CCSpriterX::create("animationFile/ls_icon.scml", true, true);
            s->setanimaID(0);
            s->setisloop(true);
            s->play();
            s->setPosition(ccp(0, gridHeight * 2));
            attack->m_effectsLayer->addChild(s, 0, LIANSUO_EFFECT_TAG);
            
            attack->changeLRType(lr_l);
        }
        attack->setFuseNum(DATA6);
        if (DATA6 > 0) {
			         
            CCSpriterX *s = CCSpriterX::create("animationFile/rh_icon.scml", true, true);
            s->setanimaID(0);
            s->setisloop(true);
            s->play();
            s->setPosition(ccp(0, gridHeight * 2));
            attack->m_effectsLayer->addChild(s, 1, RONGHE_EFFECT_TAG);
            
            attack->changeLRType(lr_r);
        }
        attack->damageNum = DATA7;
        attack->setIsRevive(DATA8);
		//复活的次数数值
        if (DATA8 && DATA9) {
            attack->setReviveNum(DATA9);
        }else{
            attack->setReviveNum(DATA9);
        }
        
        
        for (int i = 0; i < array->count(); i++) {
            CCDictionary *_data = (CCDictionary*)array->objectAtIndex(i);
            SG_BuffBase *buff = SG_BuffManager::repariBuffData(_data)->getbfBase();
			//            attack->setBuffListObj(buff);
            attack->m_attackBuffObj->addObject(buff);
        }
        
        if (array->count() > 0) {
            CCObject *obj = NULL;
            CCARRAY_FOREACH(attack->attackSbs, obj)
            {
                ShiBing *sb = (ShiBing*)obj;
                sb->show_Buff_Effect();
            }
        }
        
        this->attackLists->addObject(attack);
        
    }
    m_fangYuUpBingArray->removeAllObjects();
    m_nomalAttackUpBingArray->removeAllObjects();
    m_spAttackUpArray->removeAllObjects();
    m_deleteDownArray->removeAllObjects();
    m_moveAllGrid->removeAllObjects();
    m_jointlySbArray->removeAllObjects();
    m_jointlySbArrarCopy->removeAllObjects();
    m_synthesisGrid->removeAllObjects();
    m_spAttackGrid->removeAllObjects();
    m_moveSanGrid->removeAllObjects();
    m_removeSbObj->removeAllObjects();
    m_removeSbDef->removeAllObjects();
    
}
//长按之后的显示删除按钮
void SGBattleMap::showRemoveMenu()
{

    needStartShowFriendly=true;
    this->hideFriendlyHelp();
    SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
    if ( (heroLayer->_roundTime == 1 && !heroLayer->isInitPve)||
        m_touchState == eTouchMove) {
        unschedule(schedule_selector(SGBattleMap::showRemoveMenu));
        if (left) {
            left->setVisible(false);
        }
        m_clickTime = 0;
        return;
    }
    if (m_isGuide && SGGuideManager::shareManager()->actionType == 1)
    {
        unschedule(schedule_selector(SGBattleMap::showRemoveMenu));
        if (left) {
			left->setVisible(false);
        }
        return;
    }
    if (m_isGuide && SGGuideManager::shareManager()->actionType == 6)
    {
        unschedule(schedule_selector(SGBattleMap::showRemoveMenu));
        if (left) {
            left->setVisible(false);
        }
        return;
    }
    
    m_clickTime++;
    
    if (m_clickTime == clickTime)
    {
        
        setTouchInitSbOpacity();
        
        m_touchState = eTouchRemove;
        m_clickTime = 0;
        if (left) {
			left->setVisible(false);
        }
        
        //选中这个兵，把它设置为半透
        ShiBing *soldier = getShiBingWithIndex(m_touchPosIndex);
        
        if (!touchSpXbj || !touchSpXbj_) {
            
            if(soldier->getSbType() == ksbone)
            {
                touchSpXbj = CCSpriterX::create("animationFile/hkd1g.scml", true, true);
                touchSpXbj_ = CCSpriterX::create("animationFile/hkjt1g.scml", true, true);
                
            }else if(soldier->getSbType() == ksbtwo)
            {
                touchSpXbj = CCSpriterX::create("animationFile/hkd2g.scml", true, true);
                touchSpXbj_ = CCSpriterX::create("animationFile/hkjt2g.scml", true, true);
            }else if(soldier->getSbType() == ksbfour)
            {
                touchSpXbj = CCSpriterX::create("animationFile/hkd4g.scml", true, true);
                touchSpXbj_ = CCSpriterX::create("animationFile/hkjt4g.scml", true, true);
            }
            if (touchSpXbj) {
                touchSpXbj->setanimaID(0);
                touchSpXbj->setisloop(true);
                touchSpXbj->play();
                touchSpXbj->setAnchorPoint(ccp(0.5, 0));
                soldier->addChild(touchSpXbj, -1, Delete_Red_Rect);
            }
            if (touchSpXbj_) {
                touchSpXbj_->setanimaID(0);
                touchSpXbj_->setisloop(true);
                touchSpXbj_->play();
                touchSpXbj_->setAnchorPoint(ccp(0.5, 0));
                soldier->addChild(touchSpXbj_, Delete_Red_Rect2, Delete_Red_Rect2);
                
            }
        }
		
        CCSprite *spriteDelete = CCSprite::createWithSpriteFrameName("battle_delete_1.png");
        CCSprite *text = CCSprite::createWithSpriteFrameName("battle_delete_text.png");
        CCSprite * y = CCSprite::createWithSpriteFrameName("battle_delete_y.png");
        CCProgressTimer *cpt = CCProgressTimer::create(y);
        
        cpt->setPercentage(0);
        cpt->setType(kCCProgressTimerTypeRadial);
        cpt->setPosition(ccp(spriteDelete->getContentSize().width * 0.5, spriteDelete->getContentSize().height * 0.67));
        spriteDelete->addChild(cpt, 1,23);
        
        spriteDelete->setAnchorPoint(ccp(0.5, 0.2));
        spriteDelete->setPosition(ccpAdd(GameConfig::getGridPoint(m_touchPosIndex.i, m_touchPosIndex.j), ccp(0, gridHeight)));
        text->setPosition(ccp(spriteDelete->getContentSize().width * 0.5, spriteDelete->getContentSize().height * 0.67));
        spriteDelete->addChild(text, 2);
        
        this->addChild(spriteDelete, RemoveMenuTag, RemoveMenuTag);
        this->schedule(schedule_selector(SGBattleMap::upRemoveT), 0.001);
        isRemovebegin = true;
        
        unschedule(schedule_selector(SGBattleMap::showRemoveMenu));
        
    }
}

void SGBattleMap::upRemoveT()
{
    needStartShowFriendly=true;
    this->hideFriendlyHelp();
    
    if (!isRemovebegin) {
        return;
    }
    CCSprite *s = (CCSprite*)this->getChildByTag(RemoveMenuTag);
    CCProgressTimer *p = (CCProgressTimer*)s->getChildByTag(23);
    int percentage = p->getPercentage();
    if (percentage < 100)
    {
        //percentage+= 6;  //modified by cgp
         percentage+= 15;
        p->setPercentage(percentage);
    }
    // 按键时间已经足够  稳定显示 删除按钮。
    else
    {
        isRemovebegin = false;
        this->unschedule(schedule_selector(SGBattleMap::upRemoveT));
        this->removeChildByTag(RemoveMenuTag);
        
        //删除按钮
        CCSprite *spriteDelete = CCSprite::createWithSpriteFrameName("battle_delete_2.png");
        CCSprite *spriteDeleteSelect = CCSprite::createWithSpriteFrameName("battle_delete_2.png");
        spriteDeleteSelect->setColor(ccGRAY);
        CCMenuItemSprite *deleteItem = CCMenuItemSprite::create(spriteDelete, spriteDeleteSelect, this, menu_selector(SGBattleMap::removeSB));
        deleteItem->setAnchorPoint(ccp(0.5, 0.2));
        CCMenu *menu = CCMenu::create(deleteItem, NULL);
        menu->setPosition(ccpAdd(GameConfig::getGridPoint(m_touchPosIndex.i, m_touchPosIndex.j), ccp(0, gridHeight)));
        this->removeChildByTag(1025, true);
        this->addChild(menu, RemoveMenuTag, RemoveMenuTag);
        
        if (spTouch->isVisible()) {
            spTouch->setVisible(false);
        }
        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
        if (hero->statusBarLayer)
        {
            hero->statusBarLayer->setVisible(false);
            hero->statusBarLayer->setColor(ccWHITE);
            hero->gbSprite->stopAllActions();
            hero->gbSprite->setVisible(false);
        }
        
        //1300号消息   成功操作  显示出  删除按钮
        if (SGGuideManager::shareManager()->isGuide)
        {
            SGMainManager::preProcessDataInFightSceneForSend(5);
        }
    }
    
}

//
void SGBattleMap::showRemindMsg()
{
    SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
    if ((heroLayer->_roundTime == 1 && !heroLayer->isInitPve )||
        m_touchState == eTouchMove) {
        unschedule(schedule_selector(SGBattleMap::showRemoveMenu));
        if (left) {
            left->setVisible(false);
        }
        m_clickTime = 0;
        return;
    }
    if (m_isGuide && SGGuideManager::shareManager()->actionType == 1)
    {
        unschedule(schedule_selector(SGBattleMap::showRemoveMenu));
        if (left) {
            left->setVisible(false);
        }
        return;
    }
    if (m_isGuide && SGGuideManager::shareManager()->actionType == 6)
    {
        unschedule(schedule_selector(SGBattleMap::showRemoveMenu));
        if (left) {
            left->setVisible(false);
        }
        return;
    }
    m_clickTime++;
    
    if (m_clickTime == clickTime) {
        
        setTouchInitSbOpacity();
        
        m_touchState = eTouchRemove;
        m_clickTime = 0;
        unschedule(schedule_selector(SGBattleMap::showRemoveMenu));
        if (left) {
            left->setVisible(false);
        }
        
        //选中这个兵，把它设置为半透
        ShiBing *soldier = getShiBingWithIndex(m_touchPosIndex);
        //删除按钮
        CCSprite *spriteDelete = CCSprite::createWithSpriteFrameName("battle_delete.png");
        CCSprite *spriteDeleteSelect = CCSprite::createWithSpriteFrameName("battle_delete.png");
        spriteDeleteSelect->setColor(ccGRAY);
        CCMenuItemSprite *deleteItem = CCMenuItemSprite::create(spriteDelete, spriteDeleteSelect, this, menu_selector(SGBattleMap::removeSB));
        deleteItem->setAnchorPoint(ccp(0.5, 0.2));
        CCMenu *menu = CCMenu::create(deleteItem, NULL);
        menu->setPosition(ccpAdd(GameConfig::getGridPoint(m_touchPosIndex.i, m_touchPosIndex.j), ccp(0, gridHeight)));
        this->removeChildByTag(1025, true);
        
        //        //红色底框
        //        CCSprite *spriteBottomRect = NULL;
        //        sgShiBingType soldierType = soldier->getSbType();
        //
        //        //----------------------------------------------------------------------
        //        // 等待优化
        //        //----------------------------------------------------------------------
        //        if(soldierType == ksbone)
        //        {//散兵
        //            spriteBottomRect = CCSprite::create("battle/battle_1_red_rect.png");
        //        }else if(soldierType == ksbtwo)
        //        {//两格武将
        //            spriteBottomRect = CCSprite::create("battle/battle_2_red_rect.png");
        //        }else if(soldierType == ksbfour)
        //        {//四格武将
        //            spriteBottomRect = CCSprite::create("battle/battle_4_red_rect.png");
        //        }
        //        spriteBottomRect->setAnchorPoint(ccp(0.5, 0));
        //        soldier->addChild(spriteBottomRect, -1, Delete_Red_Rect);
        this->addChild(menu, RemoveMenuTag, RemoveMenuTag);
        
        
        if (!touchSpXbj || !touchSpXbj_) {
            
            if(soldier->getSbType() == ksbone)
            {
                touchSpXbj = CCSpriterX::create("animationFile/hkd1g.scml", true, true);
                touchSpXbj_ = CCSpriterX::create("animationFile/hkjt1g.scml", true, true);
                
            }else if(soldier->getSbType() == ksbtwo)
            {
                touchSpXbj = CCSpriterX::create("animationFile/hkd2g.scml", true, true);
                touchSpXbj_ = CCSpriterX::create("animationFile/hkjt2g.scml", true, true);
            }else if(soldier->getSbType() == ksbfour)
            {
                touchSpXbj = CCSpriterX::create("animationFile/hkd4g.scml", true, true);
                touchSpXbj_ = CCSpriterX::create("animationFile/hkjt4g.scml", true, true);
            }
            if (touchSpXbj) {
                touchSpXbj->setanimaID(0);
                touchSpXbj->setisloop(true);
                touchSpXbj->play();
                touchSpXbj->setAnchorPoint(ccp(0.5, 0));
                soldier->addChild(touchSpXbj, -1, Delete_Red_Rect);
            }
            if (touchSpXbj_) {
                touchSpXbj_->setanimaID(0);
                touchSpXbj_->setisloop(true);
                touchSpXbj_->play();
                touchSpXbj_->setAnchorPoint(ccp(0.5, 0));
                soldier->addChild(touchSpXbj_, Delete_Red_Rect2, Delete_Red_Rect2);
                
            }
        }
        
        
    }
}

void SGBattleMap::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}



void SGBattleMap::initAiWatch(bool startNew)
{
    m_skillDelayCount = 0;
    m_aiSkillDelayWatch = 0;
    this->unschedule(schedule_selector(SGBattleMap::aiCanStart));
    
    if(startNew)
    {
        this->schedule(schedule_selector(SGBattleMap::aiCanStart), AI_START_TIME);
    }
    
}

void SGBattleMap::addSkillDelayCount()
{
    if (m_skillDelayCount < 0) {
        m_skillDelayCount = 0;
    }
    m_skillDelayCount++;
}

void SGBattleMap::reduceSkillDelayCount()
{
    if (m_skillDelayCount > 0) {
		m_skillDelayCount--;
    }
}


void SGBattleMap::aiCanStart()
{
    //MM&ZY: 當切回合消息返回true後,才允許AI開始行動.
    if ( (aiCanTouchMap() || m_aiSkillDelayWatch > AI_START_TIME * 5) && SGAIManager::shareManager()->m_isVarifyActiviteAI)
    {
        beginAi();
        initAiWatch(false);
    }
}

//检测是不是有未完成的主将技
void SGBattleMap::callStartSkill()
{
    //如果手动点击了释放主将技的按钮，并且动画已经在地图上显示了，点击自动战斗的时候，先将主将技发出去
    if (!SGAIManager::shareManager()->getIsCompleteSkill())
    {
        SGAIManager::shareManager()->startMainSkill(this);
    }
}

//检测是否完成了主将技
void SGBattleMap::skillIsComplete()
{
    CCLOG("getIsCompleteSkill || %d -- [%s -> %d]", SGAIManager::shareManager()->getIsCompleteSkill(), __FUNCTION__, __LINE__);
    bool isComplete = SGAIManager::shareManager()->getIsCompleteSkill();
    
    if (isComplete)
    {
        this->beginAi();
    }
}


//先检测是不是有未完成的主将技
void SGBattleMap::checkUnCompleteSkillAndBeginAi()
{
    CCCallFunc *check = CCCallFunc::create(this, callfunc_selector(SGBattleMap::callStartSkill));
    CCCallFunc *ai = CCCallFunc::create(this, callfunc_selector(SGBattleMap::skillIsComplete));
    
    this->runAction(CCSequence::create(check, ai, NULL));
}


//开始自动战斗的定时器，（即到我方回合的时候，AI检测的定时器开启)
void SGBattleMap::autoBattleTimerStart()
{
    
    //只有自动战斗开启并且是我方回合的情况下，开启定时调用AI
    if (SGAIManager::shareManager()->getIsOpenAutoBattle() && this->getIsMe())
    {
        
        CCLOG("autoBattleTimerStart || 我方回合且开启自动战斗，开始对我方地图进行AI [%s -- %d]", __FUNCTION__, __LINE__);
        //当前地图传给AIManager，即对我方地图进行AI,在攻击阵列未全部攻击去时候的时候不进行AI
        if (SGAIManager::shareManager()->getIsCompleteAttack())
        {
            this->checkUnCompleteSkillAndBeginAi();
            //this->beginAi();
        }
        //开始AI后，下面进行单回合的结束操作
        this->endSingleRoundOrAddAICheck(false);
    }
}

//对我方地图的单回合的操作结束，类似于initAiWatch函数的作用
void SGBattleMap::endSingleRoundOrAddAICheck(bool isStartTimer)
{
    CCLOG("endSingleRoundOrAddAICheck || isStartTimer == [%d]", isStartTimer);
    
    //首先去掉原有的定时器，保证是一次新的回合AI检测
    this->unschedule(schedule_selector(SGBattleMap::autoBattleTimerStart));
    
    //如果 isStartTimer == true 再次开始循环下一回合的AI操作 （只针对我方的AI）
    if (isStartTimer)
    {
        //加入对我方的AI操作的回合检测
        CCLOG("isStart == true || 开始加入定时器");
        this->schedule(schedule_selector(SGBattleMap::autoBattleTimerStart), AI_START_TIME);
    }
}


bool SGBattleMap::aiCanTouchMap()
{
    SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
    if (m_skillDelayCount > 0) {
        
        m_aiSkillDelayWatch++;
        
        return false;
    }
    if (heroLayer->getRound() <= 0) {
        return false;
    }
    m_clickTime = 0;
    
    if (heroLayer->_roundTime == 0 &&
        !heroLayer->isInitPve) {
        return false;
    }
    
    if (!isTouch ||
        !isUpdateEnd ||
        isAddBingIn ||
        _isRefreshMap ||
        m_touchState == eTouchSkillsFang ||
        m_touchState == eTouchDai) {
        return false;
    }
    return true;
}

//由一个动作来显示延时，直到信息显示完成并消息，才能进行下一次点
void SGBattleMap::dispAutoBattleInfo()
{
    CCSequence *seq = CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(SGBattleMap::showInfo)), CCDelayTime::create(3.5f),
                                         CCCallFuncN::create(this, callfuncN_selector(SGBattleMap::setAutoBattleFlag)), NULL);
    this->runAction(seq);
}
//设置显示信息标记（m_autoBattleInfoDisp）
void SGBattleMap::setAutoBattleFlag()
{
    this->m_autoBattleInfoDisp = false;
}
//显示信息，调用SG_SHOW_MESSAGE(),（为了动作回调）
void SGBattleMap::showInfo()
{
    SGMainManager::shareMain()->showMessage(str_AutoBattleTips);
}

bool SGBattleMap::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if(SGBattleManager::sharedBattle()->getBattleType() == BT_ARENA)//竞技场不能触摸
    {
        return false;
    }
    if( !SGBattleManager::sharedBattle()->getCanDoBattleOperation() )
    {
        return false;
    }
    //如果开始自动战斗，点击地图任何位置都无效
    if (SGAIManager::shareManager()->getIsOpenAutoBattle())
    {
        //此判定分支不能合并到上方
        if (!m_autoBattleInfoDisp)
        {
            m_autoBattleInfoDisp = true;
            this->dispAutoBattleInfo();
        }
        return false;
    }
    
    
    SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
    if (heroLayer->getRound() <= 0) {
        return false;
    }
    m_clickTime = 0;
    
    hideRemindMe();
//    if (heroLayer->_roundTime == 0 &&
//        !heroLayer->isInitPve) {
//        return false;
//    }
    
    if (!isTouch ||
        !isUpdateEnd ||
        isAddBingIn ||
        _isRefreshMap ||
        m_touchState == eTouchSkillsFang ||
        m_touchState == eTouchDai) {
        return false;
    }
    m_clickPos = convertTouchToNodeSpace(pTouch);
    
    if (m_battleCurstoryId != 1) {
        needStartShowFriendly = true;
        hideFriendlyHelp();
    }
    
    if (m_touchState == eTouchRemove) {
		//        bool rond = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
		//        if (rond) {
		//            SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
		//            heroLayer->beginRoundTime();
		//        }
        backRemoveSB();
        return false;
    }
    m_touchPosIndex = gameIndex(-1, -1);
	
    int clicki = 0;
    int clickj = 0;
    
    if ( m_clickPos.x < (GameConfig::getGridPoint(0, 0, true).x - (gridWidth * 0.45)) ||
        m_clickPos.x > (GameConfig::getGridPoint(0, mapList,true).x - (gridWidth * 0.55)) ) {
        return false;
    }
    
    if (isme)
    {
        if (m_clickPos.y > GameConfig::getGridPoint(0, 0, isme).y + gridHeight ||
            m_clickPos.y < GameConfig::getGridPoint(mapRow - 1, 0,isme).y) {
            return false;
        }
        clicki = m_clickPos.y - (GameConfig::getGridPoint(mapRow - 1, 0,isme).y);
        clicki = clicki/gridHeight;
        clicki = (mapRow - 1) - clicki;
        
        clickj = m_clickPos.x - (GameConfig::getGridPoint(0, 0, isme).x - (gridWidth * 0.45));
        clickj = clickj/gridWidth;
    }
    else
    {
        if (m_clickPos.y > GameConfig::getGridPoint(mapRow - 1, 0, isme).y ||
            m_clickPos.y < GameConfig::getGridPoint(0, 0,isme).y - gridHeight) {
            return false;
        }
        clicki = screenheight * .5 + gridHeight * .5;
        clicki = m_clickPos.y - clicki;
        clicki /= gridHeight;
        
        clickj = (GameConfig::getGridPoint(0, 0, isme).x + (gridWidth * 0.55)) - m_clickPos.x;
        clickj = clickj/gridWidth;
    }
    
    if (clicki >= 0 && clicki < mapRow && clickj >= 0 && clickj < mapList)
    {
        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
        if (hero->statusBarLayer) {
            CCPoint p = GameConfig::getGridPoint(0, (clickj >= mapList) ? clickj - 1 : clickj);
            hero->statusBarLayer->setPosition(ccp( p.x -  gridWidth * 0.5, 0));
            hero->statusBarLayer->setVisible(true);
        }
        
        m_touchPosIndex = gameIndex(clicki, clickj);
        m_touchInitIndex = m_touchPosIndex;
        moveToX = clickj;
        moveToY = clicki;
        SGGridBase *grid = myGrids[clicki][clickj];
        if (!grid) {
            CCLOG("点击空置！！");
        }
        
        if (m_touchState == eTouchNull)
        {
            selectSB = NULL;
            SGGridBase *_g = NULL;
            
            for (int i = (mapRow - 1); i >= 0; i--) {
                SGGridBase *_grid = myGrids[i][clickj];//目前只适用的单格小 多格加判断
                if (_grid) {
                    if (_grid->getStype() != knormal) {
                        break;
                    }
                    _g = NULL;
                    _g = _grid;
                    break;
                }
            }
            
            if (_g)
            {
                touchInitSb = getShiBingWithGrid(_g);
                if (touchInitSb) {
                    
                    //bool isLeft = false;
                    if (_g->getSbType() == ksbfour)//当前列的最后一格是四格兵
                    {
                       
                        if (!detectionIsLastSoldier(touchInitSb))//如果有兵 不能移动
                        {
                            _g = NULL;
                            touchInitSb = NULL;
                        }
                    }
                }
                //bool move = false;
                if (touchInitSb) {
#if TOUCH_INIT_SB_ADDACT
                    
                    CCBlink *blink = CCBlink::create(0.4, 2);
                    touchInitSb->runAction(CCSequence::create(blink, NULL));
#else
                    if (!SGGuideManager::shareManager()->isGuide) {
                        int a = _g->getIndex().i;
                        float scale = 1.0f; //modified by cgp ,before is 1.8
                        if (_g->getSbType() != ksbone) {
                            a = _g->getIndex().i - 1;
                            scale = 1.0f;  //modified by cgp ,before is 1.5
                        }
                        if (clicki >= a) {
                            touchInitSb->setScale(scale);
                            
                            if (!touchSpXbj || !touchSpXbj_) {
                                
                                    addTouchSoldierEffects();
                             }
                            
                        }else if (!touchSpXbj || !touchSpXbj_) {
                            
                            addTouchSoldierEffects();                        }
                    }
#endif
                }
            }
            
            if (SGGuideManager::shareManager()->isGuide && SGGuideManager::shareManager()->actionType == 2)
            {
                CCPoint sbPoint = SGGuideManager::shareManager()->getPoint();
                CCLOG("<%f><%f>",sbPoint.x, sbPoint.y);
                if ((clickj != sbPoint.y) || (clicki != sbPoint.x))
                {
                    CCLOG("删除时没点第%f列",sbPoint.y);
                    return  false;
                }
				//                else
				//                {
				//                    this->removeChildByTag(1024, true);
				//                }
            }
            
            if (SGGuideManager::shareManager()->isGuide && SGGuideManager::shareManager()->actionType == 6)
            {
                CCPoint sbPoint = SGGuideManager::shareManager()->getPoint();
                if ((clickj != sbPoint.y))
                {
                    CCLOG("移动时没点第%f列",sbPoint.y);
                    return  false;
                }
                CCLOG("start drag shibin");
            }
        }
        if (grid &&
            !this->getChildByTag(RemoveMenuTag)
            && m_touchState == eTouchNull &&
            grid->getStype() != kattack)
        {
            
            if (left)
            {
				//                left->setVisible(true);
            }
            else
            {
                left = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("jingyantiao1.png"));//转动的进度条
                left->setType(kCCProgressTimerTypeBar);
                left->setMidpoint(ccp(0,0));
                left->setBarChangeRate(ccp(1, 0));
                addChild(left,3,REMOVEPROGRESSTAG);
            }
            left->setPosition(ccpAdd(GameConfig::getGridPoint(m_touchPosIndex.i, m_touchPosIndex.j), ccp(0, gridHeight)));
            left->setPercentage(0);
            left->runAction(CCProgressFromTo::create(0.45, 0, 100));
            left->setVisible(false);
            schedule(schedule_selector(SGBattleMap::showRemoveMenu), 0.15);
        }
    }
	//如果有引导中的箭头,点击之后去除
    if (needremoveArrow) {
        removeMoveArrow();
        SGGuideManager::shareManager()->getFightGuideLayer()->deleteFreeTip();
    }
    m_lastClickj = 0;
    return true;
}
void  SGBattleMap::addTouchSoldierEffects()
{
    if(touchInitSb->getSbType() == ksbone)
    {
        touchSpXbj = CCSpriterX::create("animationFile/lkd1g.scml", true, true);
        touchSpXbj_ = CCSpriterX::create("animationFile/lkjt1g.scml", true, true);
        
    }else if(touchInitSb->getSbType() == ksbtwo)
    {
        touchSpXbj = CCSpriterX::create("animationFile/lkd2g.scml", true, true);
        touchSpXbj_ = CCSpriterX::create("animationFile/lkjt2g.scml", true, true);
    }else if(touchInitSb->getSbType() == ksbfour)
    {
        touchSpXbj = CCSpriterX::create("animationFile/lkd4g.scml", true, true);
        touchSpXbj_ = CCSpriterX::create("animationFile/lkjt4g.scml", true, true);
    }
    if (touchSpXbj) {
        touchSpXbj->setanimaID(0);
        touchSpXbj->setisloop(true);
        touchSpXbj->play();
        touchSpXbj->setAnchorPoint(ccp(0.5, 0));
        touchInitSb->addChild(touchSpXbj, -TOUCH_INIT_SB_BJ, -TOUCH_INIT_SB_BJ);
    }
    if (touchSpXbj_) {
        touchSpXbj_->setanimaID(0);
        touchSpXbj_->setisloop(true);
        touchSpXbj_->play();
        touchSpXbj_->setAnchorPoint(ccp(0.5, 0));
        touchInitSb->addChild(touchSpXbj_, TOUCH_INIT_SB_BJ, TOUCH_INIT_SB_BJ);
        
    }
}
#define moveJzTag 96996
void SGBattleMap::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!isTouch) {
        return;
    }
    m_clickPos = convertTouchToNodeSpace(pTouch);
	//    if (m_touchState == eTouchMove) {
	//        CCSpriterX *s = (CCSpriterX*)this->getParent()->getChildByTag(WITH_SB_TAG);
	//        if (s) {
	//            s->setPosition(m_clickPos);
	//        }
	//    }
    int clicki = 0;
    int clickj = 0;
    
    if (isme)
    {
		//        CCLOG("m_touchState==%d",m_touchState);
        if (m_touchState==eTouchMove
            && selectSB) {
            selectSB->setPosition(m_clickPos);
            if (m_clickPos.y > GameConfig::getGridPoint(0, 0, isme).y + (gridHeight * 0.45) ||
                m_clickPos.y < GameConfig::getGridPoint(mapRow, 0,isme).y + (gridHeight * 0.45)) {
                if (!isTouchInit) {
                    isTouchInit = true;
                    if (m_touchState==eTouchMove && selectSB) {
                        m_touchPosIndex = m_touchInitIndex;
                        m_lastClickj = m_touchInitIndex.j;
                        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
                        if (hero->statusBarLayer) {
                            CCPoint p = GameConfig::getGridPoint(0, m_touchInitIndex.j);
                            hero->statusBarLayer->setPosition(ccp( p.x - ((selectSB->getSbType() == ksbfour) ? 0 : gridWidth * 0.5), 0));
                        }
                        if (!selectSB->getChildByTag(moveJzTag)) {
                            CCSprite *s = CCSprite::createWithSpriteFrameName("move_jz.png");
                            selectSB->addChild(s, moveJzTag, moveJzTag);
                            s->setAnchorPoint(ccp(0.5f, 0));
                        }
                        if (sbCopy) {
//                            sbCopy->removeAllChildrenWithCleanup(true);
                            sbCopy->removeFromParentAndCleanup(true);
                            sbCopy = NULL;
                            this->removeChildByTag(SB_COPY_TAY, true);
                        }
                        
                        
                    }
                }
                return;
            }
        }
    }
    
    if ( m_clickPos.x < (GameConfig::getGridPoint(0, 0, true).x - (gridWidth * 0.45)) ||
        m_clickPos.x > (GameConfig::getGridPoint(0, mapList,true).x - (gridWidth * 0.55)) ) {
        return;
    }
    float lastPosX = GameConfig::getGridPoint(0, m_lastClickj,true).x - m_clickPos.x;
    lastPosX = lastPosX >0?lastPosX:-lastPosX;
    
	//    float lastPosY = GameConfig::getGridPoint(m_touchInitIndex.i, m_touchInitIndex.j,true).y - m_clickPos.y;
	//    lastPosY = lastPosY >0?lastPosY:-lastPosY;
	//
	//    //是否为有效移动
	//    if (lastPosX < gridWidth /2 && lastPosY < gridHeight /2) {
	//        return;
	//    }
    if (isme)
    {
        isTouchInit = false;
        clicki = m_clickPos.y - (GameConfig::getGridPoint(mapRow - 1, 0,isme).y);
        clicki = clicki/gridHeight;
        clicki = (mapRow - 1) - clicki;
        clickj = m_clickPos.x - (GameConfig::getGridPoint(0, 0, isme).x - (gridWidth * 0.45));
        clickj = clickj/gridWidth;
        if ( m_battleCurstoryId != 1) {
            hideFriendlyHelp(false);
        }
    }
    else
    {
        if (m_clickPos.y > GameConfig::getGridPoint(mapRow, 0, isme).y + (gridHeight * 0.45) ||
            m_clickPos.y < GameConfig::getGridPoint(0, 0,isme).y - (gridHeight * 0.45)) {
            return;
        }
        clicki = screenheight * .5 + gridHeight * .5;
        clicki = m_clickPos.y - clicki;
        clicki /= gridHeight;
        
        clickj = (GameConfig::getGridPoint(0, 0, isme).x + (gridWidth * 0.55)) - m_clickPos.x;
        clickj = clickj/gridWidth;
    }
    if (clicki >= 0 && clicki < mapRow && clickj >= 0 && clickj < mapList)
    {
        m_lastClickj = clickj;
        if (!SGGuideManager::shareManager()->isGuide) {
            if (m_touchState == eTouchNull ||
                m_touchState == eTouchRemove) {
                if (m_touchState == eTouchRemove){
                    if (moveToX != clickj ||
                        moveToY != clicki) {
                        ShiBing *soldier = getShiBingWithIndex(m_touchPosIndex);
                        if (soldier) {
							//                            soldier->setCCSpriterXOpacity(1.0);
                            if (touchSpXbj) {
//                                touchSpXbj->removeAllChildrenWithCleanup(true);
                                touchSpXbj->removeFromParentAndCleanup(true);
                            }
                            touchSpXbj = NULL;
                            soldier->removeChildByTag(Delete_Red_Rect, true);
                            if (touchSpXbj_) {
//                                touchSpXbj_->removeAllChildrenWithCleanup(true);
                                touchSpXbj_->removeFromParentAndCleanup(true);
                            }
                            touchSpXbj_ = NULL;
                            soldier->removeChildByTag(Delete_Red_Rect2, true);
                        }
                        if (isRemovebegin) {
                            isRemovebegin = false;
                            unschedule(schedule_selector(SGBattleMap::upRemoveT));
                            this->removeChildByTag(RemoveMenuTag, true);
                        }
                        this->removeChildByTag(RemoveMenuTag, true);
                    }
                }
                if( m_touchInitIndex.j != clickj ||
                   m_touchInitIndex.i != clicki) {
                    m_touchState = eTouchMove;
                    setTouchInitSbOpacity(false);
                    unschedule(schedule_selector(SGBattleMap::showRemoveMenu));
					//                    unschedule(schedule_selector(SGBattleMap::upRemoveT));
                    if (left) {
                        left->setVisible(false);
                    }
                    m_clickTime = 0;
                    initTouchGrid(m_touchInitIndex.j);
                    if (sbCopy) {
//                        sbCopy->removeAllChildrenWithCleanup(true);
                        sbCopy->removeFromParentAndCleanup(true);
                        sbCopy = NULL;
                        this->removeChildByTag(SB_COPY_TAY, true);
                    }
                }
            }
        }
        if (SGGuideManager::shareManager()->isGuide && SGGuideManager::shareManager()->actionType == 6) {
            if (m_touchState == eTouchNull ||
                m_touchState == eTouchRemove) {
                if (m_touchInitIndex.j != clickj || m_touchInitIndex.i != clicki) {
                    m_touchState = eTouchMove;
                    unschedule(schedule_selector(SGBattleMap::showRemoveMenu));
                    if (left) {
                        left->setVisible(false);
                    }
                    m_clickTime = 0;
                    initTouchGrid(m_touchInitIndex.j);
                    if (sbCopy) {
//                        sbCopy->removeAllChildrenWithCleanup(true);
                        sbCopy->removeFromParentAndCleanup(true);
                        sbCopy = NULL;
                        this->removeChildByTag(SB_COPY_TAY, true);
                    }
                    if (SGGuideManager::shareManager()->getFightGuideLayer()) {
                        SGGuideManager::shareManager()->getFightGuideLayer()->closeTips(true);
                    }
                    removeChildByTag(1024,true);
                    SGGuideManager::shareManager()->startFightGuide(3);
                }
            }
        }
        
        
        if (m_touchState == eTouchSkills) {
            m_touchPosIndex = gameIndex(clicki, clickj);
            SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
            heroLayer->setSkillsPosj(clickj);
            
            SGHeroLayer *heroBar = SGBattleManager::sharedBattle()->getHeroLayer(false);
            if (heroBar->statusBarLayer)
            {
                CCPoint p = GameConfig::getGridPoint(0, (clickj >= mapList) ? clickj - 1 : clickj);
                heroBar->statusBarLayer->setPosition(ccp( p.x - gridWidth * 0.5, 0));
                heroBar->statusBarLayer->setVisible(true);
                heroBar->statusBarLayer->setScaleX(1);
            }
        }
        else if( m_touchState == eTouchMove )
        {
            if (m_touchGrid) {
                m_touchPosIndex = gameIndex(clicki, clickj);
                
                if (selectSB) {
                    selectSB->removeChildByTag(moveJzTag, true);
                }
				
                if (!moveTouchGrid(clickj))
                {
                    m_touchPosIndex = m_touchInitIndex;
                    if (selectSB &&
                        selectSB->getSbType() == ksbfour) {
                        SGGridBase *grid = (SGGridBase*)selectSB->getPrinceGridArray()->objectAtIndex(0);
                        m_touchPosIndex = gameIndex(grid->getIndex().i, grid->getIndex().j);
                    }
                    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
                    if (hero->statusBarLayer) {
                        CCPoint p = GameConfig::getGridPoint(0, (clickj >= mapList) ? clickj - 1 : clickj);
                        hero->statusBarLayer->setPosition(ccp( p.x - ((selectSB->getSbType() == ksbfour) ? 0 : gridWidth * 0.5), 0));
                        hero->statusBarLayer->setVisible(true);
                    }
                    
                    if (sbCopy)
						sbCopy->setVisible(false);
					//                    if (selectSB)
					//                        selectSB->setVisible(false);
                    if (moveToX != clickj) {
                        EFFECT_PLAY(MUSIC_3);
                        isOperation = false;
                    }
                }
                else
                {
                    isOperation = true;
                    if (sbCopy)
						sbCopy->setVisible(true);
					//                    if (selectSB)
					//                        selectSB->setVisible(true);
                    if (moveToX != clickj) {
                        EFFECT_PLAY(MUSIC_27);
                    }
                }
                
                if (moveToX != clickj) {
                    moveToX = clickj;
                }
                
            }else{
            }
        }
    }
}

void SGBattleMap::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
    if (hero->statusBarLayer) {
        hero->statusBarLayer->setVisible(false);
    }
    isTouchInit = false;
    if (m_touchState == eTouchMove &&
        selectSB) {
        selectSB->removeChildByTag(moveJzTag, true);
    }
	//11.18
	spTouch->setVisible(false);
    if (m_showDeletPop) {
        m_showDeletPop = false;
        this->removeChildByTag(1025, true);
    }

    setTouchInitSbOpacity();
    
    if (m_touchState == eTouchMove && !m_touchGrid) {
        m_touchState = eTouchNull;
        return;
    }
    
    SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();

    if (m_touchPosIndex.i < 0) {
        return;
    }
    if (m_clickTime < clickTime) {
        unschedule(schedule_selector(SGBattleMap::showRemoveMenu));
        if (left) {
            left->setVisible(false);
        }
        m_clickTime = 0;
    }
	
    if (isRemovebegin) {
        isRemovebegin = false;
        unschedule(schedule_selector(SGBattleMap::upRemoveT));
        this->removeChildByTag(RemoveMenuTag, true);
    }
    
    bool isGuideMove = false;
    if (m_touchState == eTouchMove) {
        heroLayer->endRoundUpdataTime();
        m_touchState = eTouchXuan;
        if (sbCopy) {
            sbCopy->removeFromParentAndCleanup(true);
            sbCopy = NULL;
        }
        this->removeChildByTag(SB_COPY_TAY, true);
        if (isme && SGGuideManager::shareManager()->actionType == 6) {
            isGuideMove = true;
        }
        
        if (m_touchPosIndex.j == m_touchInitIndex.j &&
            !isOperation) {
            EFFECT_PLAY(MUSIC_3);
        }
        
    }
    
    int clicki = m_touchPosIndex.i;
    int clickj = m_touchPosIndex.j;
    
    if (clicki >= 0 && clicki < mapRow && clickj >= 0 && clickj < mapList)
    {
        //这里选定“选位型技能”的释放位置。
        if (m_touchState == eTouchSkills) {
            
            SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
            heroLayer->setSkillsPosj(clickj);
			
            CCLOG("当前玩家选择列为：%d",heroLayer->bulletJ);
            
            //MM: 自动战斗以前的附带修改，现在技能释放期间不能自动战斗，此标识回归原意“技能是否释放完成”。
//            SGAIManager::shareManager()->setIsCompleteSkill(true);
            CCLOG("setIsCompleteSkill || %d -- [%s -> %d]", SGAIManager::shareManager()->getIsCompleteSkill(), __FUNCTION__, __LINE__);
            SGSkillManager::shareSkill()->sendSkillRequest(heroLayer->bulletJ);
            if (m_battleCurstoryId != 1) {
                hideFriendlyHelp();
            }
            return;
        }
        
        if (m_touchState == eTouchXuan) {
            SGGridBase *grid = myGrids[mapRow - 1][clickj];
            if (grid) {
                CCLOG("这列满了   不能放。。。");
                return;
            }
        }
        
		
        if (m_touchState == eTouchNull)
        {
            selectSB = NULL;
        }
        else if(m_touchState == eTouchXuan)
        {
            if (SGGuideManager::shareManager()->isGuide && !isGuideMove)
            {
                CCPoint sbPoint = SGGuideManager::shareManager()->getPoint();
                if (clickj != sbPoint.y)
                {
                    CCLOG("入场时没点第%f列",sbPoint.y);
                    return;
                }
                else
                {
                    this->removeChildByTag(1024, true);
                    if (spTouch->isVisible()) {
                        spTouch->setVisible(false);
                    }
                    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
                    if (hero->statusBarLayer)
                    {
                        hero->statusBarLayer->setVisible(false);
                        hero->statusBarLayer->setColor(ccWHITE);
                        hero->gbSprite->stopAllActions();
                        hero->gbSprite->setVisible(false);
                        
                        
                    }
                }
            }
            else if (SGGuideManager::shareManager()->isGuide && isGuideMove)
            {
                CCPoint sbPoint = SGGuideManager::shareManager()->getPoint();
                if (clickj != sbPoint.y)
                {
                    CCLOG("没移动到第%f列",sbPoint.y);
                    SGGuideManager::shareManager()->step--;
                    CCPoint sbOldPoint = SGGuideManager::shareManager()->getPoint();
                    clickj = sbOldPoint.y;
                    
                    SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
                    int step = SGGuideManager::shareManager()->step;
                    int story = m_battleCurstoryId;
                    int scene = SGPlayerInfo::sharePlayerInfo()->getCurrentSceneId();
                    
                    SGFightGuideVO *model = manager->getFightGuideModel(story, scene, step);
                    SGBattleMap *bm = heroLayer->getBattleMap();
                    CCPoint point = model->getPoint();
                    
                    GameIndex gameIndex = gameIndex(point.x, point.y);
                    bm->flashIndex(gameIndex(point.x, point.y),1024,true);
                    
                    CCPoint nextPoint = ccp(point.x +200, point.y);
                    SGFightGuideVO *nextModel = manager->getFightGuideModel(story, scene, step+1);
                    if (nextModel)
                    {
                        nextPoint = nextModel->getPoint();
                    }
                    
                    GameIndex gameNexIndex = gameIndex(nextPoint.x, nextPoint.y);
                    SGGuideManager::shareManager()->getFightGuideLayer()->creatTouchTipMoving(GameConfig::getGridPoint(gameIndex.i, gameIndex.j),
																							  GameConfig::getGridPoint(gameNexIndex.i, gameNexIndex.j),true);
                }
                else
                {
                    while (getChildByTag(1024)) {
                        this->removeChildByTag(1024, true);
                    }
                    while (getChildByTag(1025)) {
                        this->removeChildByTag(1025, true);
                    }
                    if (spTouch->isVisible()) {
                        spTouch->setVisible(false);
                    }
                    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
                    if (hero->statusBarLayer)
                    {
                        hero->statusBarLayer->setVisible(false);
                        hero->statusBarLayer->setColor(ccWHITE);
                        hero->gbSprite->stopAllActions();
                        hero->gbSprite->setVisible(false);
                    }
                    //TODO: 引导下的移动精灵成功，根据storyid/sceneid发送不同的统计ID。
                    SGMainManager::preProcessDataInFightSceneForSend(2);
                }
                
                
            }
            if (selectSB){
                selectSB->setVisible(true);
                selectSB->moveToBottom(this, NULL);
            }
            
            if (moveTouchGrid(clickj))
            {
                m_touchState = eTouchFang;
                bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
                if (round &&
                    isme) {
                    heroLayer->endRoundUpdataTime();
                    if (m_battleCurstoryId != 1) {
                        hideFriendlyHelp();
                    }
                }
            }
        }
    }
	
}

//初始化点击到的网格
void SGBattleMap::initTouchGrid(int list)
{
    EFFECT_PLAY(MUSIC_7);
    
    for (int i = (mapRow - 1); i >= 0; i--) {
        SGGridBase *grid = myGrids[i][list];//目前只适用的单格小 多格加判断
        if (grid) {
            if (grid->getStype() != knormal) {
                break;
            }
            m_touchGrid = NULL;
            m_touchGrid = grid;
            m_touchIndex = gameIndex(i, list);
            break;
        }else
        {
            m_touchGrid = NULL;
        }
    }
	
    if (m_touchGrid)
    {
        ShiBing * s = getShiBingWithGrid(m_touchGrid);
        if (s) {
            
            bool isLeft = false;
            if (m_touchGrid->getSbType() == ksbfour)//当前列的最后一格是四格兵
            {
                SGSuperPrinceBase *foursb = (SGSuperPrinceBase *)getShiBingWithGrid(m_touchGrid);
                if(foursb)
                {
                    bool isnexthave = false;
                    isLeft = foursb->isAtLeft(m_touchGrid->getIndex().j);
                    int nexti = m_touchGrid->getIndex().i + 1;
                    int nextj = isLeft?m_touchGrid->getIndex().j+1:m_touchGrid->getIndex().j-1;
                    for (int ni = nexti; ni< mapRow; ni ++)//判读四格的另一列后面是否有兵
                    {
                        SGGridBase *nextGrid = myGrids[ni][nextj];
                        if (nextGrid)
                        {
                            isnexthave = true;
                            break;
                        }
                    }
                    if (isnexthave)//如果有兵 不能移动
                    {
                        m_touchGrid = NULL;
                        return;
                    }
                }
                
            }
            
            selectSB = s;
            
            if (isme || SGAIManager::shareManager()->isPVE)
            {
                SGBattleManager::sharedBattle()->fightLogRequest(str_moveout_verify);
                if (!SGBattleManager::sharedBattle()->getIsBattlePve())
                {
                    //pve不验证
                    //SGBattleManager::sharedBattle()->validationRequet(getMapGrid());
                }
                
            }
            
            /*
			 //做个替身
			 CCString *str = CCString::createWithFormat("animationFile/%d_%d.scml",selectSB->getFileId(), selectSB->getSbColor());
			 CCSpriterX *spritex = CCSpriterX::create(str->getCString(),true,true);
			 if (spritex) {
			 spritex->setisloop(true);
			 spritex->play();
			 spritex->setanimaID(selectSB->getanimaID());
			 
			 if (!touchSpXbj || !touchSpXbj_) {
			 
			 if(selectSB->getSbType() == ksbone)
			 {
			 touchSpXbj = CCSpriterX::create("animationFile/lkd1g.scml", true, true);
			 touchSpXbj_ = CCSpriterX::create("animationFile/lkjt1g.scml", true, true);
			 spritex->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i, selectSB->getGridIndex().j,isme));
			 }else if(selectSB->getSbType() == ksbtwo)
			 {
			 touchSpXbj = CCSpriterX::create("animationFile/lkd2g.scml", true, true);
			 touchSpXbj_ = CCSpriterX::create("animationFile/lkjt2g.scml", true, true);
			 spritex->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i+1, selectSB->getGridIndex().j,isme));
			 }else if(selectSB->getSbType() == ksbfour)
			 {
			 touchSpXbj = CCSpriterX::create("animationFile/lkd4g.scml", true, true);
			 touchSpXbj_ = CCSpriterX::create("animationFile/lkjt4g.scml", true, true);
			 CCPoint point = GameConfig::getGridPoint(selectSB->getGridIndex().i+1, selectSB->getGridIndex().j,isme);
			 point.x += gridWidth*0.5;
			 spritex->setPosition(point);
			 
			 }
			 if (touchSpXbj) {
			 touchSpXbj->setanimaID(0);
			 touchSpXbj->setisloop(true);
			 touchSpXbj->play();
			 touchSpXbj->setAnchorPoint(ccp(0.5, 0));
			 spritex->addChild(touchSpXbj, -1, -TOUCH_INIT_SB_BJ);
			 }
			 if (touchSpXbj_) {
			 touchSpXbj_->setanimaID(0);
			 touchSpXbj_->setisloop(true);
			 touchSpXbj_->play();
			 touchSpXbj_->setAnchorPoint(ccp(0.5, 0));
			 spritex->addChild(touchSpXbj_, TOUCH_INIT_SB_BJ, TOUCH_INIT_SB_BJ);
			 }
			 }
			 
			 
			 this->getParent()->addChild(spritex,INT16_MAX,Move_Green_Rect);
			 }
			 */
            
            
            //无复制兵
            if (!touchSpXbj || !touchSpXbj_) {
                
                if(selectSB->getSbType() == ksbone)
                {
                    touchSpXbj = CCSpriterX::create("animationFile/lkd1g.scml", true, true);
                    touchSpXbj_ = CCSpriterX::create("animationFile/lkjt1g.scml", true, true);
                    touchSpXbj->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i, selectSB->getGridIndex().j,isme));
                    touchSpXbj_->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i, selectSB->getGridIndex().j,isme));
                }else if(selectSB->getSbType() == ksbtwo)
                {
                    touchSpXbj = CCSpriterX::create("animationFile/lkd2g.scml", true, true);
                    touchSpXbj_ = CCSpriterX::create("animationFile/lkjt2g.scml", true, true);
                    touchSpXbj->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i+1, selectSB->getGridIndex().j,isme));
                    touchSpXbj_->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i+1, selectSB->getGridIndex().j,isme));
                }else if(selectSB->getSbType() == ksbfour)
                {
                    touchSpXbj = CCSpriterX::create("animationFile/lkd4g.scml", true, true);
                    touchSpXbj_ = CCSpriterX::create("animationFile/lkjt4g.scml", true, true);
                    CCPoint point = GameConfig::getGridPoint(selectSB->getGridIndex().i+1, selectSB->getGridIndex().j,isme);
                    point.x += gridWidth*0.5;
                    touchSpXbj->setPosition(point);
                    touchSpXbj_->setPosition(point);
                    
                }
                if (touchSpXbj) {
                    touchSpXbj->setanimaID(0);
                    touchSpXbj->setisloop(true);
                    touchSpXbj->play();
                    touchSpXbj->setAnchorPoint(ccp(0.5, 0));
                    this->addChild(touchSpXbj, -1, -TOUCH_INIT_SB_BJ);
                }
                if (touchSpXbj_) {
                    touchSpXbj_->setanimaID(0);
                    touchSpXbj_->setisloop(true);
                    touchSpXbj_->play();
                    touchSpXbj_->setAnchorPoint(ccp(0.5, 0));
                    this->addChild(touchSpXbj_, TOUCH_INIT_SB_BJ, TOUCH_INIT_SB_BJ);
                }
				
            }
            
            //点击到的网格的士兵的类型(兵,二格,四格)
            switch (m_touchGrid->getSbType())
            {
                case ksbone:
                {
                    fromX = list;
                    myGrids[m_touchGrid->getIndex().i][m_touchGrid->getIndex().j] = NULL;
                }
                    break;
                case ksbtwo:
                {
                    fromX = list;
                    myGrids[m_touchGrid->getIndex().i][m_touchGrid->getIndex().j] = NULL;
                    myGrids[m_touchGrid->getIndex().i-1][m_touchGrid->getIndex().j] = NULL;
                    ShiBing *sb = getShiBingWithGrid(m_touchGrid);
                    if(sb)
                    {
                        SGGridBase *sbgrid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
                        m_touchIndex = gameIndex(sbgrid->getIndex().i, sbgrid->getIndex().j);
                    }
                }
                    break;
                case ksbfour:
                {
                    int nextj = isLeft?1:-1;
                    fromX = isLeft?m_touchGrid->getIndex().j:m_touchGrid->getIndex().j-1;
                    
                    myGrids[m_touchGrid->getIndex().i][m_touchGrid->getIndex().j] = NULL;
                    myGrids[m_touchGrid->getIndex().i - 1][m_touchGrid->getIndex().j] = NULL;
                    myGrids[m_touchGrid->getIndex().i][m_touchGrid->getIndex().j + nextj] = NULL;
                    myGrids[m_touchGrid->getIndex().i -1][m_touchGrid->getIndex().j + nextj] = NULL;
                    
                    ShiBing *sb = getShiBingWithGrid(m_touchGrid);
                    if(sb)
                    {
                        SGGridBase *sbgrid = (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
                        m_touchIndex = gameIndex(sbgrid->getIndex().i, sbgrid->getIndex().j);
                    }
                }
                    break;
                default:
                    break;
            }
            
            if (m_touchState == eTouchMove) {
                isTouch = false;
                isPlayerOperation = true;
				//                selectSB->moveToBottom(this, NULL);
//                if (selectSB->getSbType() != ksbone) {
//                    selectSB->setScale(1.5f);
//                }else{
//                    selectSB->setScale(1.8f);
//                }
                
                if (selectSB->getSbType() != ksbone) {
                    selectSB->setScale(1.0f);
                }else{
                    selectSB->setScale(1.0f);
                }
                
                selectSB->setZOrder(SB_COPY_TAY);
                
				//                WITH_SB_TAG
				//                CCString *str = CCString::createWithFormat("animationFile/%d_%d.scml",selectSB->getFileId(), selectSB->getSbColor());
				//                CCSpriterX *spritex = CCSpriterX::create(str->getCString(),true,true);
				//                spritex->setisloop(true);
				//                spritex->play();
				//                spritex->setanimaID(selectSB->getanimaID());
				//                if(selectSB->getSbType() == ksbone)
				//                {
				//                    spritex->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i, selectSB->getGridIndex().j,isme));
				//
				//                }else if(selectSB->getSbType() == ksbtwo)
				//                {
				//                    spritex->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i+1, selectSB->getGridIndex().j,isme));
				//
				//                }else if(selectSB->getSbType() == ksbfour)
				//                {
				//                    CCPoint point = GameConfig::getGridPoint(selectSB->getGridIndex().i+1, selectSB->getGridIndex().j,isme);
				//                    point.x += gridWidth*0.5;
				//                    spritex->setPosition(point);
				//                }
				//                this->getParent()->addChild(spritex,INT16_MAX,WITH_SB_TAG);
                
            }
            else
            {
                //这里情况做修改，在我方回合并且没开启自动战斗的情况下才手动操作
                
                if (isme && !SGAIManager::shareManager()->getIsOpenAutoBattle())
                {
                    isTouch = false;
                    isPlayerOperation = true;
                    selectSB->setLoopAction(NULL, ACT_moveDown);
                    selectSB->moveToBottom(this, callfunc_selector(SGBattleMap::moveToBottomEnd));
                }
                else
                {
                    isPlayerOperation = true;
                    selectSB->setLoopAction(NULL, ACT_moveUp);
                    selectSB->moveToBottom(this, callfunc_selector(SGBattleMap::touchGridToList));
                }
            }
        }
        else
        {
			//            CCString *str = CCString::createWithFormat("行: %d 列: %d, 是空对象不合法请谅解！", m_touchGrid->getIndex().i, m_touchGrid->getIndex().j);
			//            CCNative::createAlert("换一位置试试", str->getCString(), "Cancel");
			//            CCNative::addAlertButton("OK");
			//            CCNative::showAlert();
            m_touchGrid = NULL;
            if (m_touchState != eTouchMove) {
                m_touchState = eTouchNull;
            }
        }
    }
    
    
    
}

void SGBattleMap::touchGridToList()
{
    CCLOG("######moveToBottomEnd,执行AI ");
    removeReplaceSoldier();
	//wwlchuangye 11-14
//    if (!_isRefreshMap &&!isme && SGAIManager::shareManager()->isPVE) {
    //条件修改，也就是在pve敌方或者开启我方自动战斗的情况下，都进行自动AI
    if (!_isRefreshMap && (!isme || SGAIManager::shareManager()->getIsOpenAutoBattle()) && SGAIManager::shareManager()->isPVE) {

		// ########地图刷新前的ai操作#####
        if(!SGGuideManager::shareManager()->isGuide)
            SGAIManager::shareManager()->startMoveToScene(this);
        else
            SGAIManager::shareManager()->startGuideAI(this);
		// this->schedule(schedule_selector(SGBattleMap::beginAi), AI_START_TIME);
    }
    
    if (goalList != -1) {
        moveTouchGrid(goalList);
        goalList = -1;
    }
    CCLog("touchGridTolistEDN");
}

void SGBattleMap::moveToBottomEnd()
{
    isTouch = true;
    this->removeChildByTag(1024, true);
    
	//    SGGuideManager::shareManager()->step++;
    if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle()) {
		SGGuideManager::shareManager()->startFightGuide(3);
    }
    else{
        SGGuideManager::shareManager()->startAutoGuide(3);
		CCLOG("Third Guide in [%s-%d]", __FUNCTION__, __LINE__);
    }
    /*
	 switch (SGGuideManager::shareManager()->step)
	 {
	 case 6:
	 case 16:
	 {
	 SGGuideManager::shareManager()->step++;
	 SGGuideManager::shareManager()->startFightGuide(3);
	 }
	 
	 break;
	 case 10:
	 {
	 
	 SGGuideManager::shareManager()->step++;
	 SGGuideManager::shareManager()->startFightGuide(3);
	 //            CCPoint point = SGGuideManager::shareManager()->getPoint();
	 //            this->flashIndex(gameIndex(point.x, point.y));
	 }
	 break;
	 default:
	 break;
	 }
     */
}
//要移动过去的网格
bool SGBattleMap::moveTouchGrid(int list)
{
    isTouch = true;
    if (m_touchState != eTouchMove)
		EFFECT_PLAY(MUSIC_6);
    
    GameIndex moveToIndex = gameIndex(0, list);
    
    for (int i = mapRow - 1; i >= 0; i--)
    {
        SGGridBase *grid = myGrids[i][list];//只适用的单格小 多格加判断
        if (grid)
        {
            moveToIndex = gameIndex(i+1,list);
            break;
        }
    }
    
    bool isCanMove = false;
    GameIndex _index = gameIndex(0, list);
    
    if (moveToIndex.i>= 0 && moveToIndex.i < mapRow)
    {
        switch (selectSB->getSbType())
        {
            case ksbone:
            {
                isCanMove = true;
                tox = moveToIndex.j;
                _index = moveToIndex;
                if (m_touchState != eTouchMove) {
                    if (m_touchGrid == NULL) {
                        m_touchGrid = selectSB->getOnlyGrid();
                    }
                    m_touchGrid->setIndex(moveToIndex);
                    myGrids[moveToIndex.i][moveToIndex.j] = m_touchGrid;
                }
				//                bluesky
                
                
            }
                break;
            case ksbtwo:
            {
                if (mapRow - moveToIndex.i > 1)//只有空两个格，才能放两格武将
                {
                    isCanMove = true;
                    tox = moveToIndex.j;
                    
                    _index = moveToIndex;
                    if (m_touchState != eTouchMove) {
                        SGPrinceBase *sb2 = (SGPrinceBase *)selectSB;
                        SGGridBase *g1 = sb2->getNumGrid(kup);
                        SGGridBase *g2 = sb2->getNumGrid(kdown);
                        myGrids[moveToIndex.i][moveToIndex.j] = g1;
                        g1->setIndex(moveToIndex);
                        myGrids[moveToIndex.i+1][moveToIndex.j] = g2;
                        g2->setIndex(gameIndex(moveToIndex.i+1, moveToIndex.j));
                    }
                }
            }
                break;
            case ksbfour:
            {
                if (mapRow - moveToIndex.i < 2)//只有空两个格，才能放4格武将
                {
                    return false;
                }
                GameIndex leftList;
                GameIndex rightList;
                if (moveToIndex.j < (mapList-1))//如果不是最后一列
                {
                    GameIndex nextList = gameIndex(0, moveToIndex.j+1);
                    for (int ni = mapRow-1; ni>=0; ni--)
                    {
                        
                        SGGridBase *grid = myGrids[ni][moveToIndex.j+1];
                        if (grid)
                        {
                            nextList = gameIndex(ni + 1,moveToIndex.j+1);
                            break;
                        }
                    }
                    
                    if (mapRow-nextList.i >1)//只有空两个格，才能放4格武将
                    {
                        isCanMove = true;
                        leftList = moveToIndex;
                        rightList = nextList;
                    }
                    else
                        
                    {
                        return false;
                    }
                }
                else
                {
                    GameIndex nextList = gameIndex(0, moveToIndex.j-1);
                    for (int ni = mapRow-1; ni >= 0; ni--)
                    {
                        SGGridBase *grid = myGrids[ni][moveToIndex.j-1];
                        if (grid)
                        {
                            nextList = gameIndex(ni + 1,moveToIndex.j-1);
                            break;
                        }
                    }
                    if (mapRow - nextList.i >1)//只有空两个格，才能放4格武将
                    {
                        isCanMove = true;
                        leftList = nextList;
                        rightList = moveToIndex;
                    }
                    else
                    {
                        return false;
                    }
                }
                
                SGSuperPrinceBase *sb4 = (SGSuperPrinceBase*)selectSB;
                SGGridBase *sb4grid = (SGGridBase*)selectSB->getPrinceGridArray()->objectAtIndex(0);
                moveToIndex = sb4grid->getIndex();
                //判断两列哪个靠前
                if (leftList.i >= rightList.i)
                {
                    _index = leftList;
                    if (m_touchState != eTouchMove) {
                        SGGridBase *grid1 = sb4->getNumGrid(kleftup);
                        
                        myGrids[leftList.i][leftList.j] = grid1;
                        
                        SGGridBase *grid2 = sb4->getNumGrid(krightup);
                        GameIndex index2 = gameIndex(leftList.i, leftList.j+1);
                        
                        myGrids[index2.i][index2.j] = grid2;
                        
                        SGGridBase *grid3 = sb4->getNumGrid(kleftdown);
                        GameIndex index3 = gameIndex(leftList.i+1, leftList.j);
                        
                        myGrids[index3.i][index3.j] = grid3;
                        
                        SGGridBase *grid4 = sb4->getNumGrid(krightdown);
                        GameIndex index4 = gameIndex(leftList.i+1, leftList.j+1);
                        
                        myGrids[index4.i][index4.j] = grid4;
                        
                        grid1->setIndex(leftList);
                        grid2->setIndex(index2);
                        grid3->setIndex(index3);
                        grid4->setIndex(index4);
                    }
                    
                    
                }
                else
                {
                    _index = gameIndex(rightList.i, rightList.j-1);
                    if (m_touchState != eTouchMove) {
                        SGGridBase *grid1 = sb4->getNumGrid(krightup);
                        myGrids[rightList.i][rightList.j] = grid1;
                        
                        
                        SGGridBase *grid2 = sb4->getNumGrid(kleftup);
                        GameIndex index2 = gameIndex(rightList.i, rightList.j-1);
                        myGrids[index2.i][index2.j] = grid2;
                        
                        
                        SGGridBase *grid3 = sb4->getNumGrid(krightdown);
                        GameIndex index3 = gameIndex(rightList.i+1, rightList.j);
                        myGrids[index3.i][index3.j] = grid3;
                        
                        
                        SGGridBase *grid4 = sb4->getNumGrid(kleftdown);
                        GameIndex index4 = gameIndex(rightList.i+1, rightList.j-1);
                        myGrids[index4.i][index4.j] = grid4;
                        
                        grid1->setIndex(rightList);
                        grid2->setIndex(index2);
                        grid3->setIndex(index3);
                        grid4->setIndex(index4);
                    }
                }
                tox = leftList.j;
            }
                break;
            default:
                break;
        }
        
    }
    
    if (isCanMove)
    {
        if (m_touchState != eTouchMove) {
            GameIndex index = gameIndex(moveToIndex.i, tox);
            if (SGAIManager::shareManager()->isPVE || isme) {
                if (!isIndexEquate(index,m_touchIndex))
                {
                    CCString *str = CCString::createWithFormat(str_Format_moveCol_goalCol, fromX, tox);
                    SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
                    SGBattleManager::sharedBattle()->moveRequest(fromX, tox);
                }
            }
            
            if (isme)
            {
                selectSB->setLoopAction(NULL, ACT_moveUp);
            }else{
                selectSB->setLoopAction(NULL, ACT_moveDown);
            }
        }
        
        if (m_touchState != eTouchMove) {
            removeReplaceSoldier();
            if (isme) {
				
                if (!touchSpXbj || !touchSpXbj_) {
                    
                    if(selectSB->getSbType() == ksbone)
                    {
                        touchSpXbj = CCSpriterX::create("animationFile/lkd1g.scml", true, true);
                        touchSpXbj_ = CCSpriterX::create("animationFile/lkjt1g.scml", true, true);
                        touchSpXbj->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i, selectSB->getGridIndex().j,isme));
                        touchSpXbj_->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i, selectSB->getGridIndex().j,isme));
                    }else if(selectSB->getSbType() == ksbtwo)
                    {
                        touchSpXbj = CCSpriterX::create("animationFile/lkd2g.scml", true, true);
                        touchSpXbj_ = CCSpriterX::create("animationFile/lkjt2g.scml", true, true);
                        touchSpXbj->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i+1, selectSB->getGridIndex().j,isme));
                        touchSpXbj_->setPosition(GameConfig::getGridPoint(selectSB->getGridIndex().i+1, selectSB->getGridIndex().j,isme));
                    }else if(selectSB->getSbType() == ksbfour)
                    {
                        touchSpXbj = CCSpriterX::create("animationFile/lkd4g.scml", true, true);
                        touchSpXbj_ = CCSpriterX::create("animationFile/lkjt4g.scml", true, true);
                        CCPoint point = GameConfig::getGridPoint(selectSB->getGridIndex().i+1, selectSB->getGridIndex().j,isme);
                        point.x += gridWidth*0.5;
                        touchSpXbj->setPosition(point);
                        touchSpXbj_->setPosition(point);
                        
                    }
                    if (touchSpXbj) {
                        touchSpXbj->setanimaID(0);
                        touchSpXbj->setisloop(true);
                        touchSpXbj->play();
                        touchSpXbj->setAnchorPoint(ccp(0.5, 0));
                        this->addChild(touchSpXbj, -SB_RECT_TAY, -SB_RECT_TAY);
                    }
                    if (touchSpXbj_) {
                        touchSpXbj_->setanimaID(0);
                        touchSpXbj_->setisloop(true);
                        touchSpXbj_->play();
                        touchSpXbj_->setAnchorPoint(ccp(0.5, 0));
                        this->addChild(touchSpXbj_, SB_RECT_TAY, SB_RECT_TAY - 1);
                    }
                }
				
            }
            selectSB->setScale(1.0f);
            selectSB->moveToSceneCall(this, callfuncN_selector(SGBattleMap::moveEnd));
            isMoveOperation = true;
            isUpdateEnd = false;
			
			CCLOG("run here [%s-%d]", __FUNCTION__, __LINE__);
			
        }else{
            
            SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
            if(hero->statusBarLayer)
				hero->statusBarLayer->setVisible(true);
            CCPoint pos = ccp(0,0);
            
            
            if (NULL == sbCopy) {
				//                CCString *str = CCString::createWithFormat("animationFile/%d_%d.scml",selectSB->getFileId(), selectSB->getSbColor());
				//                sbCopy = CCSpriterX::create(str->getCString(),true,true);
				//                sbCopy->setisloop(true);
				//                sbCopy->play();
				//                sbCopy->setanimaID(selectSB->getanimaID());
				//                sbCopy->setCCSpriterXOpacity(0.7f);
				//                this->addChild(sbCopy, SB_COPY_TAY, SB_COPY_TAY);
                if(selectSB->getSbType() == ksbone)
                {
                    sbCopy = CCSprite::createWithSpriteFrameName("lk_7.png");
                }else if(selectSB->getSbType() == ksbtwo)
                {
                    sbCopy = CCSprite::createWithSpriteFrameName("lk_8.png");
                }else if(selectSB->getSbType() == ksbfour)
                {
                    sbCopy = CCSprite::createWithSpriteFrameName("lk_9.png");
                }
                
                this->addChild(sbCopy, SB_COPY_TAY, SB_COPY_TAY);
                sbCopy->setAnchorPoint(ccp(0.5, 0));
            }
            
            if (sbCopy) {
                if(selectSB->getSbType() == ksbone)
                {
                    sbCopy->setPosition(GameConfig::getGridPoint(_index.i, _index.j));
                    pos = GameConfig::getGridPoint(0,  _index.j);
                    if(hero->statusBarLayer)
						hero->statusBarLayer->setScaleX(1);
                    
                }else if(selectSB->getSbType() == ksbtwo)
                {
                    sbCopy->setPosition(GameConfig::getGridPoint(_index.i + 1, _index.j));
                    pos = GameConfig::getGridPoint(0,  _index.j);
                    if(hero->statusBarLayer)
						hero->statusBarLayer->setScaleX(1);
                    
                }else if(selectSB->getSbType() == ksbfour)
                {
                    CCPoint point = GameConfig::getGridPoint(_index.i + 1, _index.j);
                    point.x += gridWidth*0.5;
                    sbCopy->setPosition(point);
                    pos = ccp( point.x,0);
                    if(hero->statusBarLayer)
						hero->statusBarLayer->setScaleX(2);
                }
            }
            
            //设置当前选择的兵
			//                if(selectSB->getSbType() == ksbone)
			//                {
			////                    selectSB->setPosition(GameConfig::getGridPoint(_index.i, _index.j));
			//                    pos = GameConfig::getGridPoint(0,  _index.j);
			//                    if(hero->statusBarLayer)
			//                        hero->statusBarLayer->setScaleX(1);
			//
			//                }else if(selectSB->getSbType() == ksbtwo)
			//                {
			////                    selectSB->setPosition(GameConfig::getGridPoint(_index.i + 1, _index.j));
			//                    pos = GameConfig::getGridPoint(0,  _index.j);
			//                    if(hero->statusBarLayer)
			//                        hero->statusBarLayer->setScaleX(1);
			//
			//                }else if(selectSB->getSbType() == ksbfour)
			//                {
			//                    CCPoint point = GameConfig::getGridPoint(_index.i + 1, _index.j);
			//                    point.x += gridWidth*0.5;
			////                    selectSB->setPosition(point);
			//                    pos = ccp( point.x,0);
			//                    if(hero->statusBarLayer)
			//                        hero->statusBarLayer->setScaleX(2);
			//                }
            selectSB->setZOrder(SB_COPY_TAY);
            
            
            if(hero->statusBarLayer)
				hero->statusBarLayer->setPosition(ccp( pos.x - gridWidth * 0.5,0));
        }
    }
    else
    {
        CCLog("不能移动了");
    }
	
	CCLOG("isCanMove : [%d] in [%s-%d]", isCanMove, __FUNCTION__, __LINE__);
    return isCanMove;
}

//移动完成
void SGBattleMap::moveEnd()
{
    //GPCCLOG("###moveEnd");
    SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
    if (m_touchGrid)
    {
        
        ShiBing *bing = getShiBingWithGrid(m_touchGrid);
        if(bing)
        {
            SGGridBase *bingGrid = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
            
            GameIndex index = bingGrid->getIndex();
            
            
            bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
            if (isIndexEquate(m_touchIndex,index))//判断是否为之前的位置
            {
                isUpdateEnd = true;
                isPlayerOperation = false;
                isMoveOperation = false;
                m_touchState = eTouchNull;
                if (round && isme)
                {
                    heroLayer->beginRoundTime();
                }
            }
            else
            {
                if (round) {
                    
                    heroLayer->endRoundUpdataTime();
                }
                m_touchState = eTouchDai;
                
                myGrids[index.i][index.j] = m_touchGrid;
                
                SGGridBase *grid = myGrids[index.i][index.j];
                CCArray *fangYuArray = chackdefendList(grid);
                CCArray * spAttackArray = chack2AttackList(grid);
                
                
                CCArray *nomalAttackArray = chackNormalAttackList(grid);
                
                if (fangYuArray->count())
                {
                    fangYuSwapGridObj(fangYuArray, false);
                }
                
                if (nomalAttackArray->count()) {
                    gongJiSwapGridObj(nomalAttackArray);
                }
                
                if (spAttackArray->count()) {
                    removeGrids(spAttackArray);
                    spgongJiSwapGridObj(m_spAttackArray);
                    
                    deleteSwapGridObj(m_deleteDownArray);
                    
                }
                
                if (m_moveAllGrid->count()) {
                    schedule(schedule_selector(SGBattleMap::deleteMoveEndDetection) , RntervalTime + sbMoveSpeed);
                }else{
                    refreshMapGrid(true);
                }
                
            }
       }
    }
    m_touchIndex = gameIndex(-1, -1);
    m_touchGrid = NULL;//选择格子设为空
    heroLayer->changeSoldierNum(0);
    
    if (touchSpXbj) {
//        touchSpXbj->removeAllChildrenWithCleanup(true);
        touchSpXbj->removeFromParentAndCleanup(true);
    }
    touchSpXbj = NULL;
	
	if (this->getChildByTag(-SB_RECT_TAY))
	{
		this->removeChildByTag(-SB_RECT_TAY, true);
	}
    if (touchSpXbj_) {
//        touchSpXbj_->removeAllChildrenWithCleanup(true);
        touchSpXbj_->removeFromParentAndCleanup(true);
    }
    touchSpXbj_ = NULL;
	if (this->getChildByTag(SB_RECT_TAY - 1))
	{
		this->removeChildByTag(SB_RECT_TAY-1, true);
	}
	if (this->getChildByTag(SB_RECT_TAY))
	{
		this->removeChildByTag(SB_RECT_TAY, true);
    }
	sbCopy = NULL;
	
	CCLOG("MoveEnd : [%s-%d]", __FUNCTION__, __LINE__);
}
//刷新地图上的网格
void SGBattleMap::refreshMapGrid( bool isChack)
{
    //GPCCLOG("SGBattleMap::refreshMapGrid");
    CCArray *fang = CCArray::create();
    CCArray *fangs = CCArray::create();
	   
	//检测防御墙
    for (int i = 0; i < mapRow; i++)
	{
        for (int j = 0; j < mapList; j++)
		{
            SGGridBase *grid = myGrids[i][j];
            if (grid)
			{
                
                ShiBing *bing = getShiBingWithGrid(grid);
                if(bing)
                {
                    switch (grid->getStype())
                    {
                        case kdefend:
                        {
                            fang->addObject(bing);
                        }
                            break;
                            
                        default:
                            break;
                    }
                }
            }
        }
        if (fang->count())
		{
            CCArray *ar = CCArray::create();
            ar->addObjectsFromArray(fang);
            fangs->addObject(ar);
            fang->removeAllObjects();
        }
    }
    

    CCArray *attactList = CCArray::create();
    CCObject *atObj = NULL;
	//检测所形成的攻击阵列
    CCARRAY_FOREACH(this->getAttackList(), atObj)
    {
        SGAttackList *spat = (SGAttackList*)atObj;
        if (spat->getAttackType() == kattacknormal)
        {
            GameBattleArray *gamegj = new GameBattleArray();
            CCArray *attact = CCArray::create();
			
            for (int i = 0; i < spat->getAttackSbs()->count(); i++) {
                ShiBing *bing = (ShiBing*)spat->getAttackSbs()->objectAtIndex(i);
                attact->addObject(bing);
            }
            
            //攻击都刷新成现在的
            gamegj->refreshGameGJ = attact;
            attactList->addObject(gamegj);
			gamegj->autorelease();
        }
    }
    //兵阵列
    //GPCCLOG("attactList-count==%d",attactList->count());
    CCArray *spattactList = CCArray::create();
    
    CCObject *spatObj = NULL;
    CCARRAY_FOREACH(this->getAttackList(), spatObj)
    {
        SGAttackList *spat = (SGAttackList*)spatObj;
        if (spat->getAttackType() != kattacknormal) {
            ShiBing *bing = (ShiBing*)spat->getAttackSbs()->objectAtIndex(0);
            GameBattleArray *gamespgj = new GameBattleArray();
            CCArray *spattact = CCArray::create();
            spattact->addObject(bing);
            gamespgj->refreshGameSPGJ = spattact;
            spattactList->addObject(gamespgj);
			gamespgj->autorelease();
        }
    }
    
    //武将阵列
   // GPCCLOG("spattactList-count==%d",spattactList->count());
   	
    //地图上所有的散兵
    CCArray *allsb = CCArray::create();
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapList; j++)
        {
            SGGridBase *grid = myGrids[i][j];
            if (grid)
            {
                ShiBing *bing = getShiBingWithGrid(grid);
                if(bing)
                {
                    if (grid->getStype() == knormal)
                    {
                        if (bing)
                            allsb->addObject(bing);
                    }
                }
            }
        }
    }
    
    _isRefreshMap = true;//到此地图刷新完

	//都换成现在最新的攻击防御和攻击阵列
    for (int i = 0; i < fangs->count(); i++) {
        CCArray *ar = (CCArray*)fangs->objectAtIndex(i);
        fangYuSwapGridObj(ar, false);
    }
    
    for (int i = 0; i < spattactList->count(); i++) {
        GameBattleArray *gamespgj = (GameBattleArray*)spattactList->objectAtIndex(i);
        spgongJiSwapGridObj(gamespgj->refreshGameSPGJ);
    }
    
    for (int i = 0; i < attactList->count(); i++) {
        GameBattleArray *gamegj = (GameBattleArray*)attactList->objectAtIndex(i);
        gongJiSwapGridObj(gamegj->refreshGameGJ);
    }
    
    setIsMoveAble();  //加可移动图标
    if (m_moveAllGrid->count())
    {
        m_moveAllGrid->removeAllObjects();
    }
    //刷新后的地图, 如果散兵能组成墙或者攻击阵列
    if (allsb->count())
    {
        deleteSwapGridObj(allsb);
    }
    
    if (isChack)
    {
		//开始定时检测删除和移动兵能不能组成攻击阵列
        schedule(schedule_selector(SGBattleMap::deleteMoveEndDetection) , RntervalTime + sbMoveSpeed);
    }
    else
    {
        _isRefreshMap = false;
    }
    
}
void SGBattleMap::setIsMoveAble()
{
    ///start mask
    if(m_isShowMask)
    {
        for(int i = 0; i <soldiers->count();i++)
        {
            ShiBing *sb = (ShiBing*)soldiers->objectAtIndex(i);
			
            //对应的士兵后面的 “可移动” 图，3就是对应的可移动sprite的tag
			if (sb->getChildByTag(3))
			{
				sb->removeChildByTag(3);
            }
            if(this->getIsMe()&&SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
            {
                //士兵下面的可移动，removed by cgp
//                if(detectionIsLastSoldier(sb))
//                {
//                    this->addMoveFlag(sb);
//                }
		
            }
            
        }
        
    }
	
	
}
bool SGBattleMap::detectionIsLastSoldier(ShiBing *sb)
{
    if(sb->getPrinceGridArray()->count()==1)//普通士兵
    {
        GameIndex index=sb->getGridIndex();
        SGGridBase *bingGrid=this->myGrids[index.i][index.j];
        
        
        if(bingGrid&&bingGrid->getStype()==1)
            
        {
            if(index.i == 5)
            {
                return true;
            }
            else
            {
                bool flag = false;
                for (int k =1;k <= 5-index.i;k++)
                {
                    SGGridBase *bingGrid=this->myGrids[index.i+k][index.j];
                    if (bingGrid) {
                        flag = true;
                        break;
                        
                    }
                    else { flag = false;}
                }
                return !flag;
            }
           
        }
        
    }
    else if(sb->getPrinceGridArray()->count()==2)//两格武将
    {
        GameIndex index=sb->getGridIndex();
        SGGridBase *bingGrid=this->myGrids[index.i][index.j];
        
        
        if(bingGrid&&bingGrid->getStype()==1)
        {
            if(index.i == 4)
            {
                return  true;
            }
            else
            {
                bool flag = false;
                for (int k =1;k <= 4-index.i;k++)
                {
                    SGGridBase *bingGrid=this->myGrids[index.i+1+k][index.j];
                    if (bingGrid) {
                        flag = true;
                        break;
                    
                    }
                    else { flag = false;}
                }
            return !flag;
            
            }
        }
     
    }
    else if(sb->getPrinceGridArray()->count()==4)//四格武将
    {
        GameIndex index=sb->getGridIndex();
        SGGridBase *bingGrid=this->myGrids[index.i][index.j];
        
        
        if(bingGrid&&bingGrid->getStype()==1)
        {
            if(index.i == 4)
            {
                return  true;
            }
            else
            {
                bool flag = false;
                for (int k =1;k <= 4-index.i;k++)
                {
                    SGGridBase *bingGrid=this->myGrids[index.i+1+k][index.j];
                    SGGridBase *bingGrid2=this->myGrids[index.i+1+k][index.j+1];
                    if (bingGrid||bingGrid2) {
                        flag = true;
                        break;
                        
                    }
                    else { flag = false;}
                }
                return !flag;
                
            }
        
        }
    
    }
    
    return false;

}
void SGBattleMap::addMoveFlag(ShiBing *sb)
{
    double time = 1.2;
    CCPoint pos = CCPoint(0, 10);
    float deytime = 1.5;

    CCSprite * moveAble;
    if(sb->getSbColor()==0)
    {
        moveAble = CCSprite::createWithSpriteFrameName("bamv_Hong.png");
    }
    else if(sb->getSbColor()==1)
    {
        moveAble = CCSprite::createWithSpriteFrameName("bamv_Lv.png");
    }
    else if(sb->getSbColor()==2)
    {
        moveAble = CCSprite::createWithSpriteFrameName("bamv_Lan.png");
    }
    
    
    CCDelayTime *delayTime = CCDelayTime::create(deytime);
    moveAble->runAction(CCRepeatForever::create(CCSequence::create(CCFadeIn::create(time),delayTime,CCFadeOut::create(time),NULL)));
    moveAble->setPosition(pos);
    sb->addChild(moveAble,100,3);
}
//每次移动兵的替身,删除掉
void SGBattleMap::removeReplaceSoldier()
{
    if (this->getParent()->getChildByTag(Move_Green_Rect)) {
        CCSpriterX *spx = (CCSpriterX*)this->getParent()->getChildByTag(Move_Green_Rect);
        if (touchSpXbj) {
//            touchSpXbj->removeAllChildrenWithCleanup(true);
            touchSpXbj->removeFromParentAndCleanup(true);
        }
        touchSpXbj = NULL;
        if (touchSpXbj_) {
//            touchSpXbj_->removeAllChildrenWithCleanup(true);
            touchSpXbj_->removeFromParentAndCleanup(true);
        }
        touchSpXbj_ = NULL;
        
//        spx->removeChildByTag(-TOUCH_INIT_SB_BJ, true);
//        spx->removeChildByTag(TOUCH_INIT_SB_BJ, true);
//        spx->removeAllChildrenWithCleanup(true);
        if (spx)
        {
            spx->removeFromParentAndCleanup(true);
            spx = NULL;
        }
    }
    //移除移动兵的替身
    this->getParent()->removeChildByTag(Move_Green_Rect, true);
    
    if (sbCopy) {
//        sbCopy->removeAllChildrenWithCleanup(true);
        sbCopy->removeFromParentAndCleanup(true);
        sbCopy = NULL;
        
        this->removeChildByTag(SB_COPY_TAY, true);
    }
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
    if (hero->statusBarLayer) {
        hero->statusBarLayer->setVisible(false);
        hero->statusBarLayer->setColor(ccWHITE);
        hero->gbSprite->stopAllActions();
        hero->gbSprite->setVisible(false);
    }
    
}

void SGBattleMap::setXuanSbPos()
{
	//    moveTouchGrid(m_touchIndex.j);
    moveTouchGrid(m_touchInitIndex.j);
}

void SGBattleMap::synthesisDefen(cocos2d::CCArray *array)
{
    //GPCCLOG("synthesisDefen");
    SGBattleManager::sharedBattle()->fightLogRequest(str_Start_fusion);
    CCArray *removebing = CCArray::create();
    
    for (int i = 0; i < array->count() ; i++) {
        ShiBing *bing = (ShiBing*)array->objectAtIndex(i);
        SGGridBase *grid = (SGGridBase*) bing->getOnlyGrid();
        if (myGrids[grid->getIndex().i][grid->getIndex().j]) {
            if (grid->getIndex().i > 0) {
                for (int h = (grid->getIndex().i - 1); h >= 0; h--) {
                    SGGridBase *fgrid = myGrids[h][grid->getIndex().j];
                    if (fgrid) {
                        ShiBing *bing1 = getShiBingWithGrid(fgrid);
                        if(bing1)
                        {
                            if (fgrid->getStype() == kdefend) {
                                
                                int a =  round(bing1->getInitDef() * bing1->getDefRend()) + round(round(bing1->getInitDef() * bing1->getDefRend()) * bing1->getAddDef());
                                
                                
                                int b = bing1->getDef();
                                if (b < a) {
                                    bing1->drawSynthesisDefend(a);
                                    removebing->addObject(bing);
                                    myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
	
    
    if (array->count()) {
        array->removeAllObjects();
    }
    
    if (removebing->count()) {
        removeGrids(removebing);
        deleteSwapGridObj(m_deleteDownArray);
    }
    
    if (m_moveAllGrid->count()) {
        schedule(schedule_selector(SGBattleMap::deleteMoveEndDetection) , RntervalTime + sbMoveSpeed);
    }
    else{
        refreshMapGrid(true);
    }
	
    
}



void SGBattleMap::fangYuSwapGridObj(CCArray *array, bool isRefresh)
{
    if (!array->count())
    {
        return;
    }
	
    if (m_removeSbDef->count())
    {
        CCArray *removesb = CCArray::create();
        for (int i = 0; i < array->count(); i++)
        {
            ShiBing *s = (ShiBing*)array->objectAtIndex(i);
            SGGridBase *grid = (SGGridBase*)s->getPrinceGridArray()->objectAtIndex(0);
            for (int j = 0; j < m_removeSbDef->count(); j++)
            {
                ShiBing *_s = (ShiBing*)m_removeSbDef->objectAtIndex(j);
                SGGridBase *_grid = (SGGridBase*)_s->getPrinceGridArray()->objectAtIndex(0);
				if (grid->getIndex().i == _grid->getIndex().i &&
					grid->getIndex().j == _grid->getIndex().j )
                {
					s->removeFromParentAndCleanup(true);
					myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
					removesb->addObject(s);
				}
            }
        }
        
        if (removesb->count())
        {
            for (int i = 0; i < removesb->count(); i++)
            {
                ShiBing *s = (ShiBing*)removesb->objectAtIndex(i);
                array->removeObject(s);
            }
        }
    }
    
    
    
    
    ShiBing *bingk = (ShiBing *)array->objectAtIndex(0);
    SGGridBase *gridBegin = (SGGridBase*)bingk->getPrinceGridArray()->objectAtIndex(0);
    SGGridBase *gridEnd = gridBegin;
    if (!isRefresh)
    {
        
        for (int k = 0; k < array->count(); k++)
        {
            ShiBing *bingkk = (ShiBing *)array->objectAtIndex(k);
            SGGridBase *gridk = (SGGridBase*)bingkk->getPrinceGridArray()->objectAtIndex(0);
            
            if (gridBegin->getIndex().j < gridk->getIndex().j)
            {
                if (gridEnd->getIndex().j <= gridk->getIndex().j)
                {
                    gridEnd = gridk;
                }
                
            }
            else
            {
                if (gridEnd->getIndex().j <= gridBegin->getIndex().j)
                {
                    gridEnd = gridBegin;
                }
                
                gridBegin = gridk;
                
            }
        }
    }
	
    int beginNum = gridBegin->getIndex().j;
    
    
    CCArray *sbarray = CCArray::create();
    for (int i = 0; i < array->count(); i++)
    {
        
        bool isjoiDraw = true;
        ShiBing *bing = NULL;

        if (isRefresh)
        {
            
			        
        }
        else
        {
            for (int begin = 0; begin < array->count(); begin++)
            {
                ShiBing *sb = (ShiBing *)array->objectAtIndex(begin);
                SGGridBase *grid =  (SGGridBase*)sb->getPrinceGridArray()->objectAtIndex(0);
                if (beginNum == grid->getIndex().j)
                {
                    bing = sb;
                    begin = array->count();
                    break;
                }
            }
        }
        
        beginNum++;
        if (!bing)
        {
            continue;
        }
        
        
        
        SGGridBase *grid =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
        
        CCLOG("grid i==%d  j==%d", grid->getIndex().i, grid->getIndex().j);
        
        if (isRefresh)
        {
            
        }
        else
        {
            
            if (m_jointlySbArrarCopy->count())
            {
                for (int i = m_jointlySbArrarCopy->count(); i > 0; i--)
                {
                    ShiBing *joisb = (ShiBing*)m_jointlySbArrarCopy->objectAtIndex(i-1);
                    SGGridBase *joigrid = (SGGridBase*)joisb->getPrinceGridArray()->objectAtIndex(0);
					
                    if (joigrid->getIndex().i == grid->getIndex().i &&
                        joigrid->getIndex().j == grid->getIndex().j
                        )
                    {
                        if(grid->getIndex().i == mapRow - 1&&myGrids[joigrid->getIndex().i-1][joigrid->getIndex().j])
                        {
						// _grid_ = joigrid;
                            bing->removeFromParentAndCleanup(true);
                            isjoiDraw = false;
                            myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
                            myGrids[joigrid->getIndex().i][joigrid->getIndex().j] = joigrid;
                             m_jointlySbArrarCopy->removeObject(joisb);
                        }
                        else if(joigrid->getIndex().i == mapRow - 1&&!myGrids[mapRow - 2][joigrid->getIndex().j])
                        {
                            bing->removeFromParentAndCleanup(true);
                            isjoiDraw = false;
                            myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
                            m_jointlySbArrarCopy->removeObject(joisb);
                            
                        }
                        else
                        {
                            
                            SGGridBase *tempg =  myGrids[mapRow - 1][grid->getIndex().j];
                            if(tempg)
                            {
                                bing->removeFromParentAndCleanup(true);
                                isjoiDraw = false;
                                myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
                                myGrids[joigrid->getIndex().i][joigrid->getIndex().j] = joigrid;
                                m_jointlySbArrarCopy->removeObject(joisb);
                            }
                        }
                    }
                    else
                    {
                        
                    }
                }
            }
        }
        
       
        if (isjoiDraw || isRefresh)
        {
            
            checkSBCanDown(gridBegin, gridEnd,grid,bing, sbarray);
        }
        
    }
	
    
    for (int i = 0; i < sbarray->count(); i++)
    {
        ShiBing *sbk = (ShiBing*)sbarray->objectAtIndex(i);
        if (chackFangYuObj(m_fangYuUpBingArray, sbk))
        {
            m_fangYuUpBingArray->addObject(sbk);
        }
    }
    sbarray->removeAllObjects();
    
    if (m_jointlySbArrarCopy->count())
    {
        
        for (int i = 0; i < m_jointlySbArrarCopy->count(); i++)
        {
            ShiBing *joisb = (ShiBing*)m_jointlySbArrarCopy->objectAtIndex(i);
            SGGridBase *grid = (SGGridBase*)joisb->getPrinceGridArray()->objectAtIndex(0);
            if (grid->getIndex().i < mapRow -1 )
            {
                for (int k = grid->getIndex().i + 1; k < mapRow -1; k++)
                {
                    SGGridBase *movegrid = myGrids[k][grid->getIndex().j];
                    ShiBing *movebing = getShiBingWithGrid(movegrid);
                    if(movebing)
                    {
                        bool is_add = true;
                        if (m_spAttackGrid->count())
                        {
                            for (int spid = 0; spid < m_spAttackGrid->count(); spid++)
                            {
                                ShiBing *spidbing = (ShiBing*)m_spAttackGrid->objectAtIndex(spid);
                                SGGridBase *spidgrid = spidbing->getOnlyGrid();
                                if (grid->getIndex().i == spidgrid->getIndex().i &&
                                    grid->getIndex().j == spidgrid->getIndex().j)
                                {
                                    is_add = false;
                                    break;
                                }
                                
                            }
                        }
                        
                        if (is_add)
                        {
                            if (!movegrid)
                            {
                                break;
                            }
                            else
                            {
                                switch (movebing->getSbType())
                                {
                                    case ksbone:
                                    {
                                        m_fangYuUpBingArray->addObject(movebing);
                                        myGrids[movegrid->getIndex().i][movegrid->getIndex().j] = NULL;
                                        
                                    }
                                        break;
                                    case ksbtwo:
                                    case ksbfour:
                                    {
                                        k += 1;
                                        m_fangYuUpBingArray->addObject(movebing);
                                        for (int i = 0; i < movebing->getPrinceGridArray()->count(); i++) {
                                            SGGridBase *g = (SGGridBase*)movebing->getPrinceGridArray()->objectAtIndex(i);
                                            myGrids[g->getIndex().i][g->getIndex().j] = NULL;
                                        }
                                        
                                    }
                                        break;
                                        
                                    default:
                                        break;
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }

        m_fangYuUpBingArray->addObjectsFromArray(m_jointlySbArrarCopy);
        m_jointlySbArrarCopy->removeAllObjects();
    }
    
    for (int i = 0; i < m_fangYuUpBingArray->count(); i++)
    {
        ShiBing *bing = (ShiBing *)m_fangYuUpBingArray->objectAtIndex(i);
        for (int j = 0; j < bing->getPrinceGridArray()->count(); j++)
        {
            SGGridBase *g = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(j);
            SGGridBase *_g = myGrids[g->getIndex().i][g->getIndex().j];
            if (NULL != _g &&
                _g->getStype() != kdefend) {
                myGrids[g->getIndex().i][g->getIndex().j] = NULL;
            }
        }
    }
    
    for (int i = 0; i < m_fangYuUpBingArray->count(); i++)
    {
        ShiBing *bing = (ShiBing *)m_fangYuUpBingArray->objectAtIndex(i);
        downMoveSBRow(bing, 1);
    }
    
    if (array->count())
    {
        array->removeAllObjects();
    }
    
    if (m_fangYuUpBingArray->count())
    {
        m_moveAllGrid->addObjectsFromArray(m_fangYuUpBingArray);
        m_fangYuUpBingArray->removeAllObjects();
    }
    
    SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
    heroLayer->changeSoldierNum(0);
    
}
/**
 *   检查士兵能不能下移
 */
void  SGBattleMap::checkSBCanDown(SGGridBase *gridBegin, SGGridBase *gridEnd,SGGridBase * grid,ShiBing *bing, CCArray *sbarray)
{
    int rand = 0;
    bool isChackNul = false;
    myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
    
    CCArray *sbar = CCArray::create();
    
    for (int h = (grid->getIndex().i - 1); h >= 0; h--) {
        
        SGGridBase *upgrid = myGrids[h][grid->getIndex().j];
        
        if (upgrid) {
            if (false == isChackNul) {
                ShiBing *upbing = getShiBingWithGrid(upgrid);
                if(upbing)
                {
                    if (upbing && upgrid->getStype() != kdefend) {
                        switch (upbing->getSbType()) {
                            case ksbone:
                            {
                                //                                if (!isChackNul) {
                                if (chackFangYuObj(m_fangYuUpBingArray, upbing)) {
                                    rand++;
                                    m_fangYuUpBingArray->addObject(upbing);
                                }else
                                {
                                    rand++;
                                }
                                //                                }
                            }
                                break;
                            case ksbtwo:
                            {
                                //                                if (!isChackNul) {
                                if (chackFangYuObj(m_fangYuUpBingArray, upbing)) {
                                    h -= 1;
                                    rand += 2;
                                    m_fangYuUpBingArray->addObject(upbing);
                                }else{
                                    h -= 1;
                                    rand += 2;
                                }
                                //                                }
                            }
                                break;
                            case ksbfour:
                            {
                                h -= 1;
                                CCArray *arrayNum = chackSPDownMove(gridBegin, gridEnd, upbing, grid->getIndex().j, 1, grid);
                                if (arrayNum->count()) {
                                    bool isUbBing = false;
                                    for (int i = 0; i < arrayNum->count(); i++) {
                                        ShiBing *spbing = (ShiBing *) arrayNum->objectAtIndex(i);
                                        if(spbing)
                                        {
                                            if (spbing == upbing) {
                                                isUbBing = true;
                                                break;
                                            }
                                        }
                                    }
                                    
                                    if (isUbBing) {
                                        if (chackFangYuObj(m_fangYuUpBingArray, upbing)) {
                                            rand += 2;
                                            m_fangYuUpBingArray->addObjectsFromArray(arrayNum);
                                            //                                        sparray->addObject(upbing);
                                        }else{
                                            rand += 2;
                                        }
                                    }else
                                    {
                                        m_fangYuUpBingArray->addObjectsFromArray(arrayNum);
                                    }
                                    
                                    
                                }else{
                                    isChackNul = true;
                                }
                            }
                                break;
                            default:
                                break;
                        }
                        
                    }else{
                        if (grid->getIndex().i - rand > 0) {
                            SGGridBase *upgrid = myGrids[grid->getIndex().i - rand - 1][grid->getIndex().j];
                            ShiBing *upShibing=this->getShiBingWithGrid(upgrid);
                            if (upgrid && upgrid->getStype() == kdefend && !upShibing->isMaxDef()) {
                                if (chackFangYuObj(m_synthesisGrid, bing)) {
                                    m_synthesisGrid->addObject(bing);
                                }
                                
                                h = -1;
                            }
                        }
                    }
                }
            }
        }else{
            
            m_fangYuUpBingArray->removeAllObjects();
            sbar->removeAllObjects();
            int a = grid->getIndex().i;
            rand = a - h;
            isChackNul = false;
            
        }
        
        for (int k = 0; k < m_fangYuUpBingArray->count(); k++) {
            
            ShiBing *sbk = (ShiBing*)m_fangYuUpBingArray->objectAtIndex(k);
            if (chackFangYuObj(sbar, sbk)) {
                sbar->addObject(sbk);
            }
            
        }
        m_fangYuUpBingArray->removeAllObjects();
        
    }
    
    m_fangYuUpBingArray->addObjectsFromArray(sbar);
    sbarray->addObjectsFromArray(sbar);
    sbar->removeAllObjects();
    
    //            CCLOG("长度为：%d",m_fangYuUpBingArray->count());
    for (int i = 0; i < m_fangYuUpBingArray->count(); i++) {
        ShiBing *removesb = (ShiBing*)m_fangYuUpBingArray->objectAtIndex(i);
        for (int k = 0; k < removesb->getPrinceGridArray()->count(); k++) {
            SGGridBase *regrid = (SGGridBase*)removesb->getPrinceGridArray()->objectAtIndex(k);
            SGGridBase *remove = myGrids[regrid->getIndex().i][regrid->getIndex().j];
            if (NULL != remove&&
                remove->getStype() != kdefend &&
                NULL != myGrids[regrid->getIndex().i][regrid->getIndex().j]) {
                if (grid->getIndex().j >= regrid->getIndex().j) {
                    myGrids[regrid->getIndex().i][regrid->getIndex().j] = NULL;
                }
                
                if (removesb->getSbType() == ksbfour) {
                    
                    if (regrid->getIndex().j > gridEnd->getIndex().j ||
                        regrid->getIndex().j < gridBegin->getIndex().j) {
                        myGrids[regrid->getIndex().i][regrid->getIndex().j] = NULL;
                    }
                    
                }
            }
        }
    }
    
    for (int k = 0; k < bing->getPrinceGridArray()->count(); k++) {
        SGGridBase *grid = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(k);
        if (myGrids[grid->getIndex().i][grid->getIndex().j]) {
            ShiBing *sb = getShiBingWithGrid(grid);
            if(sb)
            {
                if (sb->getSbType() == ksbfour) {
                    myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
                }
            }
        }
        
    }
    
    if (rand != 0) {
        if (_isRefreshMap) {
            _isRefreshMap = false;
        }
    }
    
    bing->swapGridMove(this,gameIndex((grid->getIndex().i - rand), grid->getIndex().j),NULL);
    grid->setIndex(gameIndex((grid->getIndex().i - rand), grid->getIndex().j));
    myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
    bing->setBingZOrder();
    m_fangYuUpBingArray->removeAllObjects();
}
//是不是防御墙
bool SGBattleMap::chackFangYuObj(CCArray *array ,ShiBing *bing)
{
	//当前武将占的格
    SGGridBase *addgrid = (SGGridBase *)bing->getPrinceGridArray()->objectAtIndex(0);
    
    for (int i = 0; i < array->count(); i++)
	{
        ShiBing *spSB = (ShiBing *)array->objectAtIndex(i);
        SGGridBase *spgrid1 = (SGGridBase *)spSB->getPrinceGridArray()->objectAtIndex(0);
        if (addgrid->getIndex().i == spgrid1->getIndex().i &&
            addgrid->getIndex().j == spgrid1->getIndex().j )
		{
            return false;
        }
    }
    
    return true;
}


//小兵攻击阵列
void SGBattleMap::gongJiSwapGridObj(cocos2d::CCArray *array)
{
    GameIndex index = gameIndex(0, 0);
    if (array->count())
    {
        index = getNormalAttackR(array);
    }
    
    for (int i = 0; i < array->count(); i++)
    {
        ShiBing *bing = (ShiBing *)array->objectAtIndex(i);
        SGGridBase *grid =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
        myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
        bing->hideDef();
    }
    
    
    int rand = 0;
    int nullGrid = 0;
    
    if (index.i > 0)
    {
        for (int h = (index.i - 1); h >= 0; h--)
        {
            SGGridBase *upgrid = myGrids[h][index.j];
            if (upgrid)
            {
                ShiBing *upbing = getShiBingWithGrid(upgrid);
                if(upbing)
                {
                    if (upgrid->getStype() == knormal)
                    {
                        switch (upbing->getSbType())
                        {
                            case ksbone:
                            {
                                rand++;
                                m_nomalAttackUpBingArray->addObject(upbing);
                            }
                                break;
                            case ksbtwo:
                            {
                                h -= 1;
                                rand += 2;
                                m_nomalAttackUpBingArray->addObject(upbing);
                            }
                                break;
                            case ksbfour:
                            {
                                h -= 1;
                                CCArray *arrayNum = chackSPDownMoveIndex(upbing, NULL, index.j, nomalAttackLength);
                                if (arrayNum->count()) {
                                    rand += 2;
                                    m_nomalAttackUpBingArray->addObjectsFromArray(arrayNum);
                                }else{
                                    h = -1;
                                }
                                
                            }
                                break;
                                
                            default:
                                break;
                        }
                    }
                    else if(upgrid->getStype() == kattack)
                    {
                        h = -1;
                    }
                }
            }
            else
            {
                
                if (!_isRefreshMap)
                {
                    nullGrid++;
                }
                
                rand ++;
				
				
            }
            
            
            for (int i = 0; i < m_nomalAttackUpBingArray->count(); i++)
            {
                ShiBing *removesb = (ShiBing*)m_nomalAttackUpBingArray->objectAtIndex(i);
                for (int k = 0; k < removesb->getPrinceGridArray()->count(); k++)
                {
                    SGGridBase *regrid = (SGGridBase*)removesb->getPrinceGridArray()->objectAtIndex(k);
                    myGrids[regrid->getIndex().i][regrid->getIndex().j] = NULL;
                }
            }
        }
    }
    
    
    for (int i = 0; i < array->count(); i++)
    {
        ShiBing *bing = (ShiBing *)array->objectAtIndex(i);
        SGGridBase *grid =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
        if (grid)
        {
            bing->swapGridMove(this,gameIndex(grid->getIndex().i - rand , grid->getIndex().j),NULL);
            grid->setIndex(gameIndex(grid->getIndex().i - rand, grid->getIndex().j));
            myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
            bing->setBingZOrder();
        }
    }
    
    for (int i = 0; i < m_nomalAttackUpBingArray->count(); i++)
    {
        ShiBing *bing = (ShiBing *)m_nomalAttackUpBingArray->objectAtIndex(i);
        downMoveSBRow(bing, nomalAttackLength - nullGrid);
        
    }
	
    
    if (array->count())
    {
        array->removeAllObjects();
    }
    
    if (m_nomalAttackUpBingArray->count())
    {
        m_moveAllGrid->addObjectsFromArray(m_nomalAttackUpBingArray);
        m_nomalAttackUpBingArray->removeAllObjects();
    }
    
}
static bool sameTimeAttackSortRule(const CCObject * obj1, const CCObject * obj2)
{
    SGAttackList *attack1 = (SGAttackList *)obj1;
    SGAttackList *attack2 = (SGAttackList *)obj2;
    if(attack1->getAttackIndex().j>attack2->getAttackIndex().j)
    {
        return false;
    }
    else if(attack1->getAttackIndex().j<attack2->getAttackIndex().j)
    {
        return true;
    }else
    {
        
        return attack1->getAttackIndex().i>attack2->getAttackIndex().i;
        
    }
    
}

//武将攻击阵列
void SGBattleMap::spgongJiSwapGridObj(cocos2d::CCArray *array)
{
    if (array->count() == 0)
    {
        return;
    }
    std::sort(array->data->arr,array->data->arr + array->data->num,sameTimeAttackSortRule);
    for (int i = 0; i < array->count(); i++)
    {
        ShiBing *spbing = (ShiBing*)array->objectAtIndex(i);
        SGGridBase *_grid = (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(0);
        bool isCanSwap = true;
        if(_grid->getSbType() == ksbfour)
        {
            SGGridBase *grid =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(0);
            SGGridBase *grid1 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(1);
            SGGridBase *grid2 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(2);
            SGGridBase *grid3 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(3);
            myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
            myGrids[grid1->getIndex().i][grid1->getIndex().j] = NULL;
            myGrids[grid2->getIndex().i][grid2->getIndex().j] = NULL;
            myGrids[grid3->getIndex().i][grid3->getIndex().j] = NULL;
            int rand = 7;
            for(int i1 = 0 ; i1 < 2; i1++)
            {
                int ntemp_rand = 0;
                for(int h = _grid->getIndex().i-1; h>=0; h--)
                {
                    SGGridBase *temp_grid = myGrids[h][_grid->getIndex().j+i1];
                    if(temp_grid)
                    {
                        
                        if(temp_grid->getStype()==knormal)
                        {
                            ntemp_rand++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        ntemp_rand++;
                    }
                }
                if(rand>ntemp_rand)
                {
                    rand = ntemp_rand;
                }
            }
            
            /////////////////////////////////////////////////
            CCArray *array1 = CCArray::create();
            CCArray *array2 = CCArray::create();
            if(_grid->getIndex().i > 0)
            {
                for(int i1 = 0 ; i1 < 2; i1++)
                {
                    
                    for(int h = _grid->getIndex().i-1; h>=_grid->getIndex().i-rand; h--)
                    {
                        SGGridBase *temp_grid = myGrids[h][_grid->getIndex().j+i1];
                        if(temp_grid)
                        {
                            ShiBing *_upbing = getShiBingWithGrid(temp_grid);
                            if(temp_grid->getStype()==knormal&&_upbing)
                            {
                               	switch (_upbing->getSbType())
                                {
									case ksbone:
									{
                                        if (chackFangYuObj(array1, _upbing) )
                                        {
                                            array1->addObject(_upbing);
                                        }
									}
										break;
									case ksbtwo:
									{
										h--;
                                        if(rand ==1)
                                        {
                                            if (chackFangYuObj(array2, _upbing) )
                                            {
                                                array2->addObject(_upbing);
                                            }
                                        }
                                        else
                                        {
                                            if (chackFangYuObj(array1, _upbing) )
                                            {
                                                array1->addObject(_upbing);
                                            }

                                        }
									}
										break;
									case ksbfour:
									{
										
										h--;
                                        if(rand ==1)
                                        {
                                            CCArray *arrayNum = chackSPDownMoveIndex(_upbing, spbing, 0, spAttackLength + 1);
                                            if (arrayNum->count()) {
                                                
                                                    array2->addObjectsFromArray(arrayNum);
                                                    
                                               
                                            }else{
                                                
                                                isCanSwap = false;
                                            }
                                        }
                                        else
                                        {
                                            CCArray *arrayNum = chackSPDownMoveIndex(_upbing, spbing, 0, spAttackLength );
                                            if (arrayNum->count()) {
                                                
                                                    array1->addObjectsFromArray(arrayNum);
                                                    
                                               
                                            }else{
                                                
                                                isCanSwap = false;
                                            }
                                            
                                        }
										
									}
										break;
										
									default:
										break;
								}
                            }
                            else
                            {
                                if(h ==_grid->getIndex().i-1)
                                {
                                    isCanSwap = false;
                                }
                                break;
                            }
                        }
                        else
                        {
                           
                        }
                    }
                    
                }
            }
            else
            {
                isCanSwap = false;
            }
            
            if(isCanSwap)
            {
                for (int k = 0; k < array1->count(); k++)
                {
                    
                    ShiBing *sbk = (ShiBing*)array1->objectAtIndex(k);
                    if (chackFangYuObj(m_moveSanGrid, sbk))
                    {
                        m_moveSanGrid->addObject(sbk);
                    }
                    
                }
                array1->removeAllObjects();
                
                for (int k = 0; k < array2->count(); k++)
                {
                    
                    ShiBing *sbk = (ShiBing*)array2->objectAtIndex(k);
                    if (chackFangYuObj(m_spAttackUpArray, sbk))
                    {
                        m_spAttackUpArray->addObject(sbk);
                    }
                    
                }
                array2->removeAllObjects();
                
                
                SGGridBase *grid =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(0);
                SGGridBase *grid1 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(1);
                SGGridBase *grid2 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(2);
                SGGridBase *grid3 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(3);
                myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
                myGrids[grid1->getIndex().i][grid1->getIndex().j] = NULL;
                myGrids[grid2->getIndex().i][grid2->getIndex().j] = NULL;
                myGrids[grid3->getIndex().i][grid3->getIndex().j] = NULL;
                if (grid->getIndex().i - rand < 0)
                {
                    rand = grid->getIndex().i;
                }
                
                spbing->swapGridMove(this,gameIndex(grid->getIndex().i - rand, grid->getIndex().j),NULL);
                grid->setIndex(gameIndex(grid->getIndex().i - rand, grid->getIndex().j));
                grid1->setIndex(gameIndex(grid1->getIndex().i - rand, grid1->getIndex().j));
                grid2->setIndex(gameIndex(grid2->getIndex().i - rand, grid2->getIndex().j));
                grid3->setIndex(gameIndex(grid3->getIndex().i - rand, grid3->getIndex().j));
                myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
                myGrids[grid1->getIndex().i][grid1->getIndex().j] = grid1;
                myGrids[grid2->getIndex().i][grid2->getIndex().j] = grid2;
                myGrids[grid3->getIndex().i][grid3->getIndex().j] = grid3;
                
            
            
               spbing->setBingZOrder();

            }
            else
            {
                SGGridBase *grid =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(0);
                SGGridBase *grid1 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(1);
                SGGridBase *grid2 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(2);
                SGGridBase *grid3 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(3);
                myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
                myGrids[grid1->getIndex().i][grid1->getIndex().j] = grid1;
                myGrids[grid2->getIndex().i][grid2->getIndex().j] = grid2;
                myGrids[grid3->getIndex().i][grid3->getIndex().j] = grid3;

                
            }

        }
        else
        {
            CCArray *array1 = CCArray::create();
             int rand = 0;
            if(_grid->getIndex().i>0)
            {
                SGGridBase *grid =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(0);
                SGGridBase *grid1 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(1);
                myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
                myGrids[grid1->getIndex().i][grid1->getIndex().j] = NULL;
                for(int h = _grid->getIndex().i-1; h >= 0; h--)
                {
                    SGGridBase *temp_grid = myGrids[h][_grid->getIndex().j];
                    if(temp_grid)
                    {
                        ShiBing *_upbing = getShiBingWithGrid(temp_grid);
                        if(temp_grid->getStype()==knormal&&_upbing)
                        {
                            switch (_upbing->getSbType())
                            {
                                case ksbone:
                                {
                                    
                                    rand+=1;
                                    if (chackFangYuObj(array1, _upbing) )
                                    {
                                        array1->addObject(_upbing);
                                    }
                                    
                                }
                                    break;
                                case ksbtwo:
                                {
                                    h--;
                                    rand+=2;
                                   
                                        if (chackFangYuObj(array1, _upbing) )
                                        {
                                            array1->addObject(_upbing);
                                        }
                                    
                                }
                                    break;
                                case ksbfour:
                                {
                                    
                                    h--;
                                    rand+=2;
                                      CCArray *arrayNum = chackSPDownMoveIndex(_upbing, spbing, 0, spAttackLength );
                                        if (arrayNum->count())
                                        {
                                            
                                            array1->addObjectsFromArray(arrayNum);
                                            
                                            
                                        }
                                        else
                                        {
                                            
                                            isCanSwap = false;
                                        }
                                        
                                    
                                    
                                }
                                    break;
                                    
                                default:
                                    break;
                            }
                        }
                        else
                        {
                            if(h ==_grid->getIndex().i-1)
                            {
                                isCanSwap = false;
                            }
                            break;
                        }
                    }
                    else
                    {
                        rand++;
                        
                    }

                }
            }
            else
            {
                isCanSwap = false;
            }
            
            
            if(isCanSwap)
            {
                for (int k = 0; k < array1->count(); k++)
                {
                    
                    ShiBing *sbk = (ShiBing*)array1->objectAtIndex(k);
                    if (chackFangYuObj(m_moveSanGrid, sbk))
                    {
                        m_moveSanGrid->addObject(sbk);
                    }
                    
                }
                array1->removeAllObjects();

                SGGridBase *grid =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(0);
                SGGridBase *grid1 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(1);
                myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
                myGrids[grid1->getIndex().i][grid1->getIndex().j] = NULL;
                
                if (grid->getIndex().i - rand < 0)
                {
                    rand = grid->getIndex().i;
                }
                
                spbing->swapGridMove(this,gameIndex(grid->getIndex().i - rand, grid->getIndex().j),NULL);
                grid->setIndex(gameIndex(grid->getIndex().i - rand, grid->getIndex().j));
                grid1->setIndex(gameIndex(grid1->getIndex().i - rand, grid1->getIndex().j));
                myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
                myGrids[grid1->getIndex().i][grid1->getIndex().j] = grid1;
                spbing->setBingZOrder();
            }
            else
            {
                SGGridBase *grid =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(0);
                SGGridBase *grid1 =  (SGGridBase*)spbing->getPrinceGridArray()->objectAtIndex(1);
                myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
                myGrids[grid1->getIndex().i][grid1->getIndex().j] = grid1;
            }
            
        }
    }
    
    for (int i = 0; i < array->count(); i++)
    {
        ShiBing *bing = (ShiBing *)array->objectAtIndex(i);
        for (int i = 0; i < m_spAttackUpArray->count(); i++)
        {
            ShiBing *bing1 = (ShiBing *)m_spAttackUpArray->objectAtIndex(i);
            if (bing1 != bing)
            {
                SGGridBase * grid = (SGGridBase *)bing1->getPrinceGridArray()->objectAtIndex(0);
                if (grid)
                {
                    {
                        downMoveSBRow(bing1, spAttackLength+1);
                    }
                    
                    
                }
            }
            
        }
        
        for (int i = 0; i < m_moveSanGrid->count(); i++)
        {
            ShiBing *bing1 = (ShiBing *)m_moveSanGrid->objectAtIndex(i);
            if (bing1 != bing)
            {
                SGGridBase * grid = (SGGridBase *)bing1->getPrinceGridArray()->objectAtIndex(0);
                if (grid)
                {
                    downMoveSBRow(bing1, spAttackLength);
                }
            }
            
        }
        
        
    }
	
    
    if (array->count())
    {
        array->removeAllObjects();
    }
    
    if (m_spAttackUpArray->count())
    {
        m_spAttackUpArray->removeAllObjects();
    }
    if (m_moveSanGrid->count())
    {
        m_moveSanGrid->removeAllObjects();
    }
    
}
//删除之后,形成阵列或者防御,交换上下的顺序
void SGBattleMap::deleteSwapGridObj(CCArray *array)
{
    if (!array->count()) {
        return;
    }
    
    CCArray * moveArray = CCArray::create();
    for (int i = 0; i < array->count(); i++) {
        ShiBing *bing = (ShiBing *)array->objectAtIndex(i);
        SGGridBase *grid = bing->getOnlyGrid();
        if( myGrids[grid->getIndex().i][grid->getIndex().j] )
        {
            switch (bing->getSbType()) {
                case ksbone:
                {
                    SGGridBase *grid =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
                    if (grid->getIndex().i > 0) {
                        int rand = 0;
                        for (int j = grid->getIndex().i - 1; j >=0; j--) {
                            
                            if (NULL == myGrids[j][grid->getIndex().j]) {
                                rand++;
                            }else
                            {
                                break;
                            }
                        }
                        if (rand > 0) {
                            myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
                            bing->swapGridMove(this,gameIndex(grid->getIndex().i - rand, grid->getIndex().j),NULL);
                            grid->setIndex(gameIndex(grid->getIndex().i - rand, grid->getIndex().j));
                            myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
                            if (grid->getStype() != kdefend) {
                                moveArray->addObject(bing);
                            }
                        }
                        
                    }
                }
                    break;
                case ksbtwo:
                {
                    SGGridBase *grid =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
                    SGGridBase *grid1 =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(1);
                    if (grid->getIndex().i > 0) {
                        int rand = 0;
                        for (int j = grid->getIndex().i - 1; j >=0; j--) {
                            
                            if (NULL == myGrids[j][grid->getIndex().j]) {
                                rand++;
                            }else
                            {
                                break;
                            }
                        }
                        if (rand > 0) {
                            myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
                            myGrids[grid1->getIndex().i][grid1->getIndex().j] = NULL;
                            bing->swapGridMove(this,gameIndex(grid->getIndex().i - rand, grid->getIndex().j),NULL);
                            grid->setIndex(gameIndex(grid->getIndex().i - rand, grid->getIndex().j));
                            grid1->setIndex(gameIndex(grid1->getIndex().i - rand, grid1->getIndex().j));
                            myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
                            myGrids[grid1->getIndex().i][grid1->getIndex().j] = grid1;
                            moveArray->addObject(bing);
                        }
                    }
                }
                    break;
                    
                case ksbfour:
                {
                    SGGridBase *grid =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
                    SGGridBase *grid1 =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(1);
                    SGGridBase *grid2 =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(2);
                    SGGridBase *grid3 =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(3);
                    if (grid->getIndex().i > 0) {
                        int rand = 0;
                        int rand1 = 0;
                        for (int j = grid->getIndex().i - 1; j >=0; j--) {
                            
                            if (NULL == myGrids[j][grid->getIndex().j]) {
                                rand++;
                            }else
                            {
                                break;
                            }
                        }
                        
                        for (int j = grid1->getIndex().i - 1; j >=0; j--) {
                            
                            if (NULL == myGrids[j][grid1->getIndex().j]) {
                                rand1++;
                            }else
                            {
                                break;
                            }
                        }
                        
                        if (rand > rand1) {
                            rand = rand1;
                        }
                        
                        if (rand > 0 &&
                            rand1 > 0) {
                            myGrids[grid->getIndex().i][grid->getIndex().j] = NULL;
                            myGrids[grid1->getIndex().i][grid1->getIndex().j] = NULL;
                            myGrids[grid2->getIndex().i][grid2->getIndex().j] = NULL;
                            myGrids[grid3->getIndex().i][grid3->getIndex().j] = NULL;
                            
                            bing->swapGridMove(this,gameIndex(grid->getIndex().i - rand, grid->getIndex().j),NULL);
                            grid->setIndex(gameIndex(grid->getIndex().i - rand, grid->getIndex().j));
                            grid1->setIndex(gameIndex(grid1->getIndex().i - rand, grid1->getIndex().j));
                            grid2->setIndex(gameIndex(grid2->getIndex().i - rand, grid2->getIndex().j));
                            grid3->setIndex(gameIndex(grid3->getIndex().i - rand, grid3->getIndex().j));
                            
                            myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
                            myGrids[grid1->getIndex().i][grid1->getIndex().j] = grid1;
                            myGrids[grid2->getIndex().i][grid2->getIndex().j] = grid2;
                            myGrids[grid3->getIndex().i][grid3->getIndex().j] = grid3;
                            
                            moveArray->addObject(bing);
                        }
                        
                        
                    }
                }
                    break;
                default:
                    break;
            }
            bing->setBingZOrder();
        }
		
    }
	
    if (array->count()) {
        array->removeAllObjects();
    }
    m_deleteDownArray->removeAllObjects();
    m_moveAllGrid->addObjectsFromArray(moveArray);
    
    for (int i = 0; i < m_moveAllGrid->count(); i++) {
        ShiBing *bing = (ShiBing *)m_moveAllGrid->objectAtIndex(i);
        switch (bing->getSbType()) {
            case ksbone:
            {
                SGGridBase *grid =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
                if (grid->getStype() == kdefend) {
                    m_moveAllGrid->removeObject(bing);
                }
            }
                break;
                
            default:
            {
            }
                break;
        }
    }
    
}

CCArray * SGBattleMap::chackSPDownMoveIndex(ShiBing *sp, ShiBing *bing, int lie, int rand)
{
    CCArray * array = CCArray::create();
    
    
    SGGridBase *spgrid0 = (SGGridBase*) sp->getPrinceGridArray()->objectAtIndex(0);
    SGGridBase *spgrid1 = (SGGridBase*) sp->getPrinceGridArray()->objectAtIndex(3);
    bool isCanMoveDown = true;
    GameIndex index_ = spgrid0->getIndex();
    if(index_.i <= mapRow-rand-2)
    {
        for(int i = 0 ;i < 2 ; i++)
        {
            GameIndex index = spgrid0->getIndex();
            CCArray *sbarray = CCArray::create();
            for(int k = index.i+2; k < mapRow; k++)
            {
                SGGridBase *sbGrid = myGrids[k][index.j+i];
                if(sbGrid&&sbGrid->getStype()!= kdefend)
                {
                    if(k == mapRow-rand)
                    {
                        isCanMoveDown = false;
                    }
                    ShiBing *spbing = getShiBingWithGrid(sbGrid);
                    if(spbing)
                    {
                        switch (spbing->getSbType())
                        {
                            case ksbone:
                            {
                                sbarray->addObject(spbing);
                            }
                                break;
                            case ksbtwo:
                            {
                                k++;
                                sbarray->addObject(spbing);
                            }
                                break;
                            case ksbfour:
                            {
                                k++;
                                CCArray *arrayNum = chackDownMoveIndex2(bing,spbing, spgrid1->getIndex(), rand,0);
                                if (arrayNum->count())
                                {
                                    sbarray->addObjectsFromArray(arrayNum);
                                }
                                else
                                {
                                    isCanMoveDown = false;
                                }
                                
                            }
                                break;
                            default:
                                break;
                                
                        }
                    }
                }
                else
                {
                    if(sbGrid&&sbGrid->getStype()== kdefend)
                    {
                        isCanMoveDown = false;
                    }
                    break;
                }
            }
            
            array->addObjectsFromArray(sbarray);
        }
    }
    else
    {
        isCanMoveDown = false;
    }
    if(!isCanMoveDown)
    {
        array -> removeAllObjects();
    }
    else
    {
        array->addObject(sp);
    }

//    if (lie == spgrid->getIndex().j) {
//        lie = spgrid1->getIndex().j;
//    }else
//    {
//        lie = spgrid->getIndex().j;
//    }
//    int num = 0;
//    
//    for (int h = mapRow - 1; h >= spgrid->getIndex().i; h--) {
//        bool sgGrid = false;
//        
//        SGGridBase *grid = myGrids[h][lie];
//        bool isadd = false;
//        bool isadd1 = false;
//        bool isadd2 = false;
//        if (bing) {
//            for (int i = 0; i < bing->getPrinceGridArray()->count(); i++) {
//                SGGridBase *grid_ = (SGGridBase*) bing->getPrinceGridArray()->objectAtIndex(i);
//                if (h == grid_->getIndex().i &&
//                    lie == grid_->getIndex().j) {
//                    sgGrid = true;
//                }
//            }
//        }
//        
//        
//        if (grid &&
//            !sgGrid) {
//            
//            if (num >= rand) {
//                
//                isadd1 = true;
//                ShiBing *spbing = getShiBingWithGrid(grid);
//                switch (spbing->getSbType()) {
//                    case ksbone:
//                    {
//                        array->addObject(spbing);
//                    }
//                        break;
//                    case ksbtwo:
//                    {
//                        h -= 1;
//                        array->addObject(spbing);
//                    }
//                        break;
//                    case ksbfour:
//                    {
//                        isadd2 = true;
//                        SGGridBase *ggg = (SGGridBase *)spbing->getPrinceGridArray()->objectAtIndex(2);
//                        if (ggg->getIndex().i == spgrid->getIndex().i) {
//                            
//                            isadd = true;
//                        }else{
//                            SGGridBase *ggg1 = (SGGridBase *)spbing->getPrinceGridArray()->objectAtIndex(3);
//                            int num = lie;
//                            if (num == ggg->getIndex().j) {
//                                num = ggg1->getIndex().j;
//                            }else
//                            {
//                                num = ggg->getIndex().j;
//                            }
//                            
//                            for (int k = mapRow -1; k > ggg->getIndex().i; k--) {
//                                SGGridBase *grid = myGrids[k][num];
//                                if (grid) {
//                                    isadd = false;
//                                    break;
//                                }else{
//                                    isadd = true;
//                                }
//                            }
//                            
//                            if (isadd) {
//                                if (chackFangYuObj(array, spbing)) {
//                                    array->addObject(spbing);
//                                }
//                            }
//                            h -= 1;
//                        }
//                        
//                    }
//                        break;
//                    default:
//                        break;
//                }
//				
//                if (isadd1) {
//                    if (isadd2) {
//                        if (isadd) {
//                            if (chackFangYuObj(array, sp)) {
//                                array->addObject(sp);
//                            }
//                        }
//                    }else{
//                        if (chackFangYuObj(array, sp)) {
//                            array->addObject(sp);
//                        }
//                    }
//                    
//                }
//                
//            }
//            
//        }else{
//            num++;
//            
//        }
//    }
    
    
    return array;
}

CCArray* SGBattleMap::chackSPDownMove(SGGridBase *gridBegin, SGGridBase *gridEnd, ShiBing *bing, int lie, int rand, SGGridBase* fangSbGrid)
{
    isAddGrid = false;
    isAddGrid1 = false;
    isAddGrid2 = false;
    
    CCArray * array = CCArray::create();
    
    SGGridBase *spgrid0 = (SGGridBase*) bing->getPrinceGridArray()->objectAtIndex(0);
    //SGGridBase *spgrid01 = (SGGridBase*) bing->getPrinceGridArray()->objectAtIndex(1);
    //SGGridBase *spgrid = (SGGridBase*) bing->getPrinceGridArray()->objectAtIndex(2);
    SGGridBase *spgrid1 = (SGGridBase*) bing->getPrinceGridArray()->objectAtIndex(3);
    bool isCanMoveDown = true;
    GameIndex index_ = spgrid0->getIndex();
    if(index_.i != mapRow-2)
    {
        for(int i = 0 ;i < 2 ; i++)
        {
            GameIndex index = spgrid0->getIndex();
            CCArray *sbarray = CCArray::create();
            for(int k = index.i+2; k < mapRow; k++)
            {
                SGGridBase *sbGrid = myGrids[k][index.j+i];
                if(sbGrid&&sbGrid->getStype()!= kdefend)
                {
                    if(k == mapRow-1)
                    {
                        isCanMoveDown = false;
                    }
                    ShiBing *spbing = getShiBingWithGrid(sbGrid);
                    if(spbing)
                    {
                        switch (spbing->getSbType())
                        {
                            case ksbone:
                            {
                                sbarray->addObject(spbing);
                            }
                                break;
                            case ksbtwo:
                            {
                                k++;
                                sbarray->addObject(spbing);
                            }
                                break;
                            case ksbfour:
                            {
                                k++;
                                CCArray *arrayNum = chackDownMoveIndex(bing,spbing, spgrid1->getIndex(), 1, fangSbGrid->getIndex().i);
                                if (arrayNum->count())
                                {
                                    sbarray->addObjectsFromArray(arrayNum);
                                }
                                else
                                {
                                    isCanMoveDown = false;
                                }
                                
                            }
                                break;
                            default:
                                break;

                        }
                    }
                }
                else
                {
                    break;
                }
            }
            
            array->addObjectsFromArray(sbarray);
        }
    }
    else
    {
        isCanMoveDown = false;
    }
    if(!isCanMoveDown)
    {
        array -> removeAllObjects();
    }
    else
    {
        array->addObject(bing);
    }
//    if (gridBegin->getIndex().j <=spgrid->getIndex().j &&
//        gridEnd->getIndex().j > spgrid->getIndex().j) {
//        
//        if (fangSbGrid->getIndex().j == spgrid->getIndex().j) {
//            SGGridBase *fSbGrid = myGrids[fangSbGrid->getIndex().i][fangSbGrid->getIndex().j + 1];
//            if (fSbGrid) {
//                ShiBing *fSb = getShiBingWithGrid(fSbGrid);
//                if (fSb) {
//                    SGGridBase *gg = (SGGridBase*) fSb->getPrinceGridArray()->objectAtIndex(0);
//					
//                    if (gg->getStype() != kdefend) {
//						
//                        
//                        int num = 0;
//                        CCArray *sbarray = CCArray::create();
//                        for (int sp = mapRow - 1; sp > spgrid1->getIndex().i; sp--) {
//                            SGGridBase *grid = myGrids[sp][spgrid1->getIndex().j];
//                            if (grid) {
//                                
//                                if (num > 0) {
//                                    ShiBing *spbing = getShiBingWithGrid(grid);
//                                    switch (spbing->getSbType()) {
//                                        case ksbone:
//                                        {
//                                            sbarray->addObject(spbing);
//                                        }
//                                            break;
//                                        case ksbtwo:
//                                        {
//                                            sp -= 1;
//                                            sbarray->addObject(spbing);
//                                        }
//                                            break;
//                                        case ksbfour:
//                                        {
//                                            isAddGrid2 = true;
//                                            sp -= 1;
//                                            CCArray *arrayNum = chackDownMoveIndex(bing,spbing, spgrid1->getIndex(), 1, fangSbGrid->getIndex().i);
//                                            if (arrayNum->count()) {
//                                                sbarray->addObjectsFromArray(arrayNum);
//                                            }
//                                            
//                                        }
//                                            break;
//                                        default:
//                                            break;
//                                    }
//                                }
//                                
//                            }else{
//                                
//                                num++;
//                                if (sbarray->count()) {
//                                    sbarray->removeAllObjects();
//                                }
//                                isAddGrid = true;
//                            }
//                        }
//                        
//                        if (isAddGrid) {
//                            if (isAddGrid2) {
//                                if (isAddGrid1) {
//                                    sbarray->addObject(bing);
//                                }
//                            }else{
//                                sbarray->addObject(bing);
//                            }
//                            
//                        }
//                        
//                        array->addObjectsFromArray(sbarray);
//                        
//                        return array;
//                    }
//                }
//            }
//			
//        }
//        if (fangSbGrid->getIndex().j == spgrid1->getIndex().j) {
//            SGGridBase *fSbGrid = myGrids[fangSbGrid->getIndex().i][fangSbGrid->getIndex().j - 1];
//            if (fSbGrid) {
//                ShiBing *fSb = getShiBingWithGrid(fSbGrid);
//                if (fSb) {
//                    SGGridBase *gg = (SGGridBase*) fSb->getPrinceGridArray()->objectAtIndex(0);
//                    if (gg->getStype() != kdefend) {
//                        
//						
//                        int num = 0;
//                        CCArray *sbarray = CCArray::create();
//                        for (int sp = mapRow - 1; sp > spgrid->getIndex().i; sp--) {
//                            SGGridBase *grid = myGrids[sp][spgrid->getIndex().j];
//                            if (grid) {
//                                
//                                if (num > 0) {
//                                    ShiBing *spbing = getShiBingWithGrid(grid);
//                                    switch (spbing->getSbType()) {
//                                        case ksbone:
//                                        {
//                                            sbarray->addObject(spbing);
//                                        }
//                                            break;
//                                        case ksbtwo:
//                                        {
//                                            sp -= 1;
//                                            sbarray->addObject(spbing);
//                                        }
//                                            break;
//                                        case ksbfour:
//                                        {
//                                            isAddGrid2 = true;
//                                            sp -= 1;
//                                            CCArray *arrayNum = chackDownMoveIndex(bing,spbing, spgrid->getIndex(), 1, fangSbGrid->getIndex().i);
//                                            if (arrayNum->count()) {
//                                                sbarray->addObjectsFromArray(arrayNum);
//                                            }else{
//                                                sbarray->removeAllObjects();
//                                            }
//                                        }
//                                            break;
//                                        default:
//                                            break;
//                                    }
//                                }
//                                
//                            }else{
//                                
//                                num++;
//                                if (sbarray->count()) {
//                                    sbarray->removeAllObjects();
//                                }
//                                
//                                isAddGrid = true;
//                            }
//                        }
//                        
//                        if (isAddGrid) {
//                            if (isAddGrid2) {
//                                if (isAddGrid1) {
//                                    sbarray->addObject(bing);
//                                }
//                            }else{
//                                sbarray->addObject(bing);
//                            }
//                            
//                        }
//                        
//                        array->addObjectsFromArray(sbarray);
//                        
//                        return array;
//                    }
//                }
//            }
//			
//        }
//        
//        if (spgrid0->getIndex().i > 0) {
//            if (!myGrids[spgrid0->getIndex().i - 1][spgrid0->getIndex().j]) {
//                myGrids[spgrid0->getIndex().i][spgrid0->getIndex().j] = NULL;
//            }
//        }
//        if (spgrid01->getIndex().i > 0) {
//            if (!myGrids[spgrid01->getIndex().i - 1][spgrid01->getIndex().j]) {
//                myGrids[spgrid01->getIndex().i][spgrid01->getIndex().j] = NULL;
//            }
//        }
//        
//        if (gridEnd->getIndex().i > spgrid->getIndex().i + 1 ) {
//            int _i = spgrid1->getIndex().j;
//            if (fangSbGrid->getIndex().j == spgrid1->getIndex().j) {
//                _i = spgrid->getIndex().j;
//            }
//            for (int kk = spgrid->getIndex().i + 1; kk < gridEnd->getIndex().i ; kk++) {
//                SGGridBase *grid = myGrids[kk][_i];
//                if (grid) {
//                    if (grid->getStype() == kdefend) {
//                        return array;
//                    }else{
//                        ShiBing *spbing = getShiBingWithGrid(grid);
//                        
//                        switch (spbing->getSbType()) {
//                            case ksbone:
//                            {
//                                if (chackFangYuObj(m_fangYuUpBingArray, spbing)) {
//                                    array->addObject(spbing);
//                                }
//                                if (kk + 1 == gridEnd->getIndex().i) {
//                                    SGGridBase *grid1 = myGrids[spgrid->getIndex().i + 1][spgrid->getIndex().j];
//                                    if (grid1) {
//                                        if (grid1->getStype() == kdefend) {
//                                            return array;
//                                        }
//                                    }
//                                    
//                                    array->addObject(bing);
//                                }
//                            }
//                                break;
//                            case ksbtwo:
//                            {
//                                if (chackFangYuObj(m_fangYuUpBingArray, spbing)) {
//                                    kk += 1;
//                                    array->addObject(spbing);
//                                }
//                                if (kk + 1 == gridEnd->getIndex().i) {
//                                    SGGridBase *grid1 = myGrids[kk][spgrid->getIndex().j];
//                                    if (grid1->getStype() == kdefend) {
//                                        return array;
//                                    }
//                                    array->addObject(bing);
//                                }
//                            }
//                                break;
//                            case ksbfour:
//                            {
//                                kk += 1;
//                                
//                                SGGridBase *sggrid = (SGGridBase *)spbing->getPrinceGridArray()->objectAtIndex(2);
//                                SGGridBase *sggrid1 = (SGGridBase *)spbing->getPrinceGridArray()->objectAtIndex(3);
//                                if (gridBegin->getIndex().j <= sggrid->getIndex().j &&
//                                    gridEnd->getIndex().j > sggrid->getIndex().j) {
//                                    if (sggrid->getIndex().j != spgrid->getIndex().j){
//                                        array->addObject(spbing);
//                                    }
//                                    array->addObject(bing);
//                                    kk = gridEnd->getIndex().i;
//                                }else if(gridBegin->getIndex().j > spgrid->getIndex().j){
//                                    int num = 0;
//                                    CCArray *sbarray = CCArray::create();
//                                    for (int o = mapRow - 1; o > sggrid->getIndex().i; o--) {
//                                        SGGridBase *grid = myGrids[o][sggrid->getIndex().j];
//                                        if (grid) {
//                                            
//                                            if (num > 0) {
//                                                ShiBing *spbing = getShiBingWithGrid(grid);
//                                                switch (spbing->getSbType()) {
//                                                    case ksbone:
//                                                    {
//														sbarray->addObject(spbing);
//                                                    }
//                                                        break;
//                                                    case ksbtwo:
//                                                    {
//                                                        o -= 1;
//														sbarray->addObject(spbing);
//                                                    }
//                                                        break;
//                                                    default:
//                                                        break;
//                                                }
//                                            }
//                                            
//                                        }else{
//                                            num++;
//                                            if (sbarray->count()) {
//                                                sbarray->removeAllObjects();
//                                            }
//                                        }
//                                        
//                                    }
//                                    if (sbarray->count()) {
//                                        array->addObjectsFromArray(sbarray);
//                                        array->addObject(spbing);
//                                        
//                                        array->addObject(bing);
//                                        
//                                    }
//                                    
//                                }else{
//                                    int num = 0;
//                                    CCArray *sbarray = CCArray::create();
//                                    for (int o = mapRow - 1; o > sggrid1->getIndex().i; o--) {
//                                        SGGridBase *grid = myGrids[o][sggrid1->getIndex().j];
//                                        if (grid) {
//                                            
//                                            if (num > 0) {
//                                                ShiBing *spbing = getShiBingWithGrid(grid);
//                                                switch (spbing->getSbType()) {
//                                                    case ksbone:
//                                                    {
//														sbarray->addObject(spbing);
//                                                    }
//                                                        break;
//                                                    case ksbtwo:
//                                                    {
//                                                        o -= 1;
//														sbarray->addObject(spbing);
//                                                    }
//                                                        break;
//                                                    default:
//                                                        break;
//                                                }
//                                            }
//                                            
//                                        }else{
//                                            num++;
//                                            if (sbarray->count()) {
//                                                sbarray->removeAllObjects();
//                                            }
//                                        }
//                                        
//                                    }
//                                    if (sbarray->count()) {
//                                        array->addObjectsFromArray(sbarray);
//                                        
//                                        array->addObject(spbing);
//                                        
//                                        array->addObject(bing);
//                                        
//                                        
//                                    }
//                                    
//                                }
//                            }
//                                break;
//                            default:
//                                break;
//                        }
//                    }
//					
//                }else{
//                    if (chackFangYuObj(m_fangYuUpBingArray, bing)) {
//                        array->addObject(bing);
//                    }
//                }
//            }
//        }else{
//            
//            array->addObject(bing);
//			
//        }
//    }
//    else if(gridBegin->getIndex().j > spgrid->getIndex().j){
//        int num = 0;
//        CCArray *sbarray = CCArray::create();
//        for (int sp = mapRow - 1; sp > spgrid->getIndex().i; sp--) {
//            SGGridBase *grid = myGrids[sp][spgrid->getIndex().j];
//            if (grid) {
//                
//                if (num > 0) {
//                    ShiBing *spbing = getShiBingWithGrid(grid);
//                    switch (spbing->getSbType()) {
//                        case ksbone:
//                        {
//                            sbarray->addObject(spbing);
//                        }
//                            break;
//                        case ksbtwo:
//                        {
//                            sp -= 1;
//                            sbarray->addObject(spbing);
//                        }
//                            break;
//                        case ksbfour:
//                        {
//                            isAddGrid2 = true;
//                            sp -= 1;
//                            CCArray *arrayNum = chackDownMoveIndex(bing,spbing, spgrid->getIndex(), 1, fangSbGrid->getIndex().i);
//                            if (arrayNum->count()) {
//                                sbarray->addObjectsFromArray(arrayNum);
//                            }else{
//                                sbarray->removeAllObjects();
//                            }
//                        }
//                            break;
//                        default:
//                            break;
//                    }
//                }
//                
//            }else{
//                
//                num++;
//                if (sbarray->count()) {
//                    sbarray->removeAllObjects();
//                }
//                
//                isAddGrid = true;
//            }
//        }
//        
//        if (isAddGrid) {
//            if (isAddGrid2) {
//                if (isAddGrid1) {
//                    sbarray->addObject(bing);
//                }else{
//                    sbarray->removeAllObjects();
//                }
//            }else{
//                sbarray->addObject(bing);
//            }
//        }else{
//            sbarray->removeAllObjects();
//        }
//        
//        array->addObjectsFromArray(sbarray);
//    }else{
//        int num = 0;
//        CCArray *sbarray = CCArray::create();
//        for (int sp = mapRow - 1; sp > spgrid1->getIndex().i; sp--) {
//            SGGridBase *grid = myGrids[sp][spgrid1->getIndex().j];
//            if (grid) {
//                
//                if (num > 0) {
//                    ShiBing *spbing = getShiBingWithGrid(grid);
//                    switch (spbing->getSbType()) {
//                        case ksbone:
//                        {
//                            sbarray->addObject(spbing);
//                        }
//                            break;
//                        case ksbtwo:
//                        {
//                            sp -= 1;
//                            sbarray->addObject(spbing);
//                        }
//                            break;
//                        case ksbfour:
//                        {
//                            isAddGrid2 = true;
//                            sp -= 1;
//                            CCArray *arrayNum = chackDownMoveIndex(bing,spbing, spgrid1->getIndex(), 1, fangSbGrid->getIndex().i);
//                            if (arrayNum->count()) {
//                                sbarray->addObjectsFromArray(arrayNum);
//                            }
//                            
//                        }
//                            break;
//                        default:
//                            break;
//                    }
//                }
//                
//            }else{
//                
//                num++;
//                if (sbarray->count()) {
//                    sbarray->removeAllObjects();
//                }
//                isAddGrid = true;
//            }
//        }
//        
//        if (isAddGrid) {
//            if (isAddGrid2) {
//                if (isAddGrid1) {
//                    sbarray->addObject(bing);
//                }else{
//                    sbarray->removeAllObjects();
//                }
//            }else{
//                sbarray->addObject(bing);
//            }
//        }else
//        {
//            sbarray->removeAllObjects();
//        }
//        
//        array->addObjectsFromArray(sbarray);
//        
//    }
    return array;
}
/**
 * 当前是否可以向下移动rand行 用来换攻击阵列
 */
CCArray *SGBattleMap::chackDownMoveIndex2(ShiBing *sb, ShiBing *bing, GameIndex index, int rand, int row)
{
    CCArray *array = CCArray::create();
    //int num = 0;
    //int lie = index.j;
    SGGridBase *spgrid = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
    
    bool isCanMoveDown = true;
    GameIndex index_ = spgrid->getIndex();
    if(index_.i <= mapRow-rand-2)
    {
        for(int i = 0 ;i < 2 ; i++)
        {
            GameIndex index = spgrid->getIndex();
            CCArray *sbarray = CCArray::create();
            for(int k = index.i+2; k < mapRow; k++)
            {
                SGGridBase *sbGrid = myGrids[k][index.j+i];
                if(sbGrid&&sbGrid->getStype()!= kdefend)
                {
                    if(k == mapRow-rand)
                    {
                        isCanMoveDown = false;
                    }
                    ShiBing *spbing = getShiBingWithGrid(sbGrid);
                    if(spbing)
                    {
                        switch (spbing->getSbType())
                        {
                            case ksbone:
                            {
                                sbarray->addObject(spbing);
                            }
                                break;
                            case ksbtwo:
                            {
                                k++;
                                sbarray->addObject(spbing);
                            }
                                break;
                            case ksbfour:
                            {
                                k++;
                                //CCArray *arrayNum = chackDownMoveIndex(bing,spbing, spgrid1->getIndex(), 1, fangSbGrid->getIndex().i);
                                CCLOG("This is a joke!");
                            }
                                break;
                            default:
                                break;
                                
                        }
                    }
                }
                else
                {
                    if(sbGrid&&sbGrid->getStype()== kdefend)
                    {
                         isCanMoveDown = false;
                    }
                    break;
                }
            }
            
            array->addObjectsFromArray(sbarray);
        }
    }
    else
    {
        isCanMoveDown = false;
    }
    if(!isCanMoveDown)
    {
        array -> removeAllObjects();
    }
    else
    {
        array->addObject(bing);
    }
    
    return array;
}
//
CCArray* SGBattleMap::chackDownMoveIndex(ShiBing *sb,ShiBing *bing,GameIndex index, int rand, int row)
{
    CCArray *array = CCArray::create();
    //int num = 0;
    //int lie = index.j;
    SGGridBase *spgrid = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
    
    bool isCanMoveDown = true;
    GameIndex index_ = spgrid->getIndex();
    if(index_.i <= mapRow-rand-2)
    {
        for(int i = 0 ;i < 2 ; i++)
        {
            GameIndex index = spgrid->getIndex();
            CCArray *sbarray = CCArray::create();
            for(int k = index.i+2; k < mapRow; k++)
            {
                SGGridBase *sbGrid = myGrids[k][index.j+i];
                if(sbGrid&&sbGrid->getStype()!= kdefend)
                {
                    if(k == mapRow-rand)
                    {
                        isCanMoveDown = false;
                    }
                    ShiBing *spbing = getShiBingWithGrid(sbGrid);
                    if(spbing)
                    {
                        switch (spbing->getSbType())
                        {
                            case ksbone:
                            {
                                sbarray->addObject(spbing);
                            }
                                break;
                            case ksbtwo:
                            {
                                k++;
                                sbarray->addObject(spbing);
                            }
                                break;
                            case ksbfour:
                            {
                                k++;
                                //CCArray *arrayNum = chackDownMoveIndex(bing,spbing, spgrid1->getIndex(), 1, fangSbGrid->getIndex().i);
                                CCLOG("This is a joke!");
                            }
                                break;
                            default:
                                break;
                                
                        }
                    }
                }
                else
                {
                    break;
                }
            }
            
            array->addObjectsFromArray(sbarray);
        }
    }
    else
    {
        isCanMoveDown = false;
    }
    if(!isCanMoveDown)
    {
        array -> removeAllObjects();
    }
    else
    {
        array->addObject(bing);
    }

    
//    if (index.j == spgrid->getIndex().j) {
//        lie += 1;
//    }else
//    {
//        lie -= 1;
//    }
//    
//    for (int sp = mapRow - 1; sp > row-1; sp--) {
//        SGGridBase *grid = myGrids[sp][lie];
//        if (grid) {
//            
//            if (num >= rand) {
//                ShiBing *spbing = getShiBingWithGrid(grid);
//                switch (spbing->getSbType()) {
//                    case ksbone:
//                    {
//                        array->addObject(spbing);
//                    }
//                        break;
//                    case ksbtwo:
//                    {
//                        sp -= 1;
//                        array->addObject(spbing);
//                    }
//                        break;
//                    case ksbfour:
//                    {
//                        sp -= 1;
//                        array->addObject(bing);
//                        
//                    }
//                        break;
//                    default:
//                        break;
//                }
//            }
//            
//        }else{
//            num++;
//            if (array->count()) {
//                array->removeAllObjects();
//            }
//            if (num >= rand) {
//                isAddGrid1 = true;
//            }
//        }
//    }
//    
    
    return array;
}
//bing向下移动rand个格子
void SGBattleMap::downMoveSBRow(ShiBing *bing, int rand)
{
    switch (bing->getSbType()) {
        case ksbone:
        {
            SGGridBase *grid =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
            if (grid->getIndex().i < mapRow - 1) {
                bing->swapGridMove(this,gameIndex(grid->getIndex().i + rand, grid->getIndex().j), NULL);
                grid->setIndex(gameIndex(grid->getIndex().i + rand, grid->getIndex().j));
            }
            
            myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
        }
            break;
        case ksbtwo:
        {
            SGGridBase *grid =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
            SGGridBase *grid1 =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(1);
            if (grid1->getIndex().i < mapRow - 1) {
                bing->swapGridMove(this,gameIndex(grid->getIndex().i + rand, grid->getIndex().j), NULL);
                grid->setIndex(gameIndex(grid->getIndex().i + rand, grid->getIndex().j));
                grid1->setIndex(gameIndex(grid1->getIndex().i + rand, grid1->getIndex().j));
            }
            
            myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
            myGrids[grid1->getIndex().i][grid1->getIndex().j] = grid1;
        }
            break;
        case ksbfour:
        {
            SGGridBase *grid =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
            SGGridBase *grid1 =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(1);
            SGGridBase *grid2 =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(2);
            SGGridBase *grid3 =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(3);
            
            if (grid2->getIndex().i < mapRow - 1) {
                bing->swapGridMove(this,gameIndex(grid->getIndex().i + rand, grid->getIndex().j), NULL);
                grid->setIndex(gameIndex(grid->getIndex().i + rand, grid->getIndex().j));
                grid1->setIndex(gameIndex(grid1->getIndex().i + rand, grid1->getIndex().j));
                grid2->setIndex(gameIndex(grid2->getIndex().i + rand, grid2->getIndex().j));
                grid3->setIndex(gameIndex(grid3->getIndex().i + rand, grid3->getIndex().j));
            }
            
            myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
            myGrids[grid1->getIndex().i][grid1->getIndex().j] = grid1;
            myGrids[grid2->getIndex().i][grid2->getIndex().j] = grid2;
            myGrids[grid3->getIndex().i][grid3->getIndex().j] = grid3;
        }
            break;
        default:
            break;
    }
    
    bing->setBingZOrder();
}

//每次删除或者移动士兵后检测是否形成攻击阵列或者防御墙
void SGBattleMap::deleteMoveEndDetection()
{
   // GPCCLOG("deleteMoveEndDetection");
    unschedule(schedule_selector(SGBattleMap::deleteMoveEndDetection));
    
    isSynthesis = false;
	
    //地图上的所有格子
    //得到地图上所有的可以移动的状态为normal 的格子，包括非蓄力状态下的四格，二格武将，
    m_moveAllGrid->removeAllObjects();
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapList; j++)
        {
            SGGridBase *grid = myGrids[i][j];
            if (grid)
            {
                ShiBing *bing = getShiBingWithGrid(grid);
                if (bing && grid->getStype() == knormal)
                {//是正常可移动过去的格子
                    if( chackFangYuObj(m_moveAllGrid, bing))//并且不是防御墙
						m_moveAllGrid->addObject(bing);//将这个兵加到可移动的格子数组中
                }
            }
        }
    }
    
    CCArray *fangyu = CCArray::create();
    CCArray *spgongji = CCArray::create();
    CCArray *gongji = CCArray::create();
    
    for (int i = 0; i < m_moveAllGrid->count(); i++)
    {
        ShiBing *bing = (ShiBing*)m_moveAllGrid->objectAtIndex(i);
        SGGridBase *grid = (SGGridBase*) bing->getPrinceGridArray()->objectAtIndex(0);
        
		//正常格子.或者中防御格子,且这个是士兵
        if ((grid->getStype() == knormal ||
             grid->getStype() == kdefend) &&
            bing->getSbType() == ksbone)
        {
			
            CCArray *farray = chackdefendList(grid);//这个格子里的兵是不是可以组成防御墙
            if (farray->count())
            {
                GameBattleArray * gamef = new GameBattleArray();
                gamef->gameFY = CCArray::create();
                gamef->gameFY->addObjectsFromArray(farray);
                fangyu->addObject(gamef);
				gamef->autorelease();
            }
        }
		
    }
	
    

    m_sameTimeWJ->removeAllObjects();
    for (int i = 0; i < m_moveAllGrid->count(); i++)
    {
        ShiBing *bing = (ShiBing*)m_moveAllGrid->objectAtIndex(i);
        SGGridBase *grid = (SGGridBase*) bing->getPrinceGridArray()->objectAtIndex(0);
        
        if ((grid->getStype() == knormal ||
             grid->getStype() == kdefend) &&
            grid->getSbType() == ksbone) {
            CCArray *array = chack2AttackList(grid);//二格和四格武将的攻击阵列检测
			
			
            if (array->count()) {
                GameBattleArray * gamespgj = new GameBattleArray();
                gamespgj->gameSPGJ = array;
                spgongji->addObject(gamespgj);
				gamespgj->autorelease();
				
            }
        }
    }
  
    
    std::sort(m_sameTimeWJ->data->arr,m_sameTimeWJ->data->arr + m_sameTimeWJ->data->num,sameTimeAttackSortRule);
    
    for (int i = 0; i < m_sameTimeWJ->count(); i++) {
        
        attackLists->removeObject(m_sameTimeWJ->objectAtIndex(i));
    }
    
    for (int i = 0; i < m_sameTimeWJ->count(); i++) {
        
        attackLists->addObject(m_sameTimeWJ->objectAtIndex(i));
    }
    m_sameTimeWJ->removeAllObjects();

    m_sameTimeSB->removeAllObjects();
    for (int i = 0; i < m_moveAllGrid->count(); i++) {
        ShiBing *bing = (ShiBing*)m_moveAllGrid->objectAtIndex(i);
        SGGridBase *grid = (SGGridBase*) bing->getPrinceGridArray()->objectAtIndex(0);
        if ((grid->getStype() == knormal ||
             grid->getStype() == kdefend) &&
            grid->getSbType() == ksbone)
		{
            CCArray *garray = chackNormalAttackList(grid);//正常的三个兵阵列组成检测
            if (garray->count())
			{
                GameBattleArray * gamegj = new GameBattleArray();
                gamegj->gameGJ = garray;
                gongji->addObject(gamegj);
				gamegj->autorelease();
            }
        }
        
    }
	
    std::sort(m_sameTimeSB->data->arr,m_sameTimeSB->data->arr + m_sameTimeSB->data->num,sameTimeAttackSortRule);

    for (int i = 0; i < m_sameTimeSB->count(); i++) {

        attackLists->removeObject(m_sameTimeSB->objectAtIndex(i));
    }

    for (int i = 0; i < m_sameTimeSB->count(); i++) {

        attackLists->addObject(m_sameTimeSB->objectAtIndex(i));
    }
    m_sameTimeSB->removeAllObjects();
    
    for (int i = m_moveAllGrid->count() - 1; i >= 0; i--) {
        ShiBing *bing = (ShiBing*)m_moveAllGrid->objectAtIndex(i);
        SGGridBase *grid = (SGGridBase*) bing->getPrinceGridArray()->objectAtIndex(0);
        if (grid && grid->getStype() == kdefend ) {
            myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
        }
    }
    
    m_moveAllGrid->removeAllObjects();
	
    
    for (int j = 0; j < fangyu->count(); j++) {
        GameBattleArray *gamef = (GameBattleArray*)fangyu->objectAtIndex(j);
        for (int i = 0; i < gamef->gameFY->count(); i++) {
            ShiBing *bing = (ShiBing*) gamef->gameFY->objectAtIndex(i);
            SGGridBase *grid = bing->getOnlyGrid();
            myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
        }
    }
    
	
    CCArray *fang = CCArray::create();
    CCArray *fangs = CCArray::create();
    
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapList; j++)
        {
            if (i != 0)
            {
                SGGridBase *grid = myGrids[i][j];
                if (grid)
                {
                    
                    ShiBing *bing = getShiBingWithGrid(grid);
                    if(bing)
                    {
                        switch (grid->getStype())
                        {
                            case kdefend:
                            {
                                if (j > 0)
                                {
                                    SGGridBase *_grid = myGrids[i][j - 1];
                                    if (_grid)
                                    {
                                        if (_grid->getStype() == kdefend)
                                        {
                                            
                                        }
                                        else
                                        {
                                            if (fang->count())
                                            {
                                                CCArray *ar = CCArray::create();
                                                ar->addObjectsFromArray(fang);
                                                fangs->addObject(ar);
                                                fang->removeAllObjects();
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if (fang->count())
                                        {
                                            CCArray *ar = CCArray::create();
                                            ar->addObjectsFromArray(fang);
                                            fangs->addObject(ar);
                                            fang->removeAllObjects();
                                        }
                                    }
                                }
                                else
                                {
                                    
                                }
                                fang->addObject(bing);
                                
                            }
                                break;
                                
                            default:
                                break;
                        }
                    }
                }
            }
        }
        if (fang->count())
        {
            CCArray *ar = CCArray::create();
            ar->addObjectsFromArray(fang);
            fangs->addObject(ar);
            fang->removeAllObjects();
        }
    }
    for (int i = 0; i < fangs->count(); i++)
    {
        CCArray *ar = (CCArray*)fangs->objectAtIndex(i);
        fangYuSwapGridObj(ar, false);
    }
    
	
	
    for (int i = 0; i < gongji->count(); i++)
    {
        GameBattleArray *gamegj = (GameBattleArray*)gongji->objectAtIndex(i);
        gongJiSwapGridObj(gamegj->gameGJ);
        CC_SAFE_RELEASE(gamegj);
    }
	
    for (int i = 0; i < spgongji->count(); i++)
    {
        GameBattleArray *gamespgj = (GameBattleArray*)spgongji->objectAtIndex(i);
        if (m_spAttackArray->count())
        {
            for (int k = 0; k < m_spAttackArray->count(); k++)
            {
                CCArray *a = CCArray::create();
                a->addObject(m_spAttackArray->objectAtIndex(k));
                spgongJiSwapGridObj(a);
            }
            m_spAttackArray->removeAllObjects();
        }
        removeGrids(gamespgj->gameSPGJ);
        deleteSwapGridObj(m_deleteDownArray);
        CC_SAFE_RELEASE(gamespgj);
    }
	
    
    if (m_moveAllGrid->count())
    {
        m_moveAllGrid->removeAllObjects();
        _isRefreshMap = false;
        CCArray *allsb = CCArray::create();
        for (int i = 0; i < mapRow; i++)
        {
            for (int j = 0; j < mapList; j++)
            {
                SGGridBase *grid = myGrids[i][j];
                if (grid)
                {
                    ShiBing *bing = getShiBingWithGrid(grid);
                    if (grid->getStype() == knormal&&bing)
                    {
                        allsb->addObject(bing);
                    }
                }
            }
        }
        deleteSwapGridObj(allsb);
        schedule(schedule_selector(SGBattleMap::deleteMoveEndDetection) , RntervalTime + sbMoveSpeed);
		
    }
    else
    {

        if (m_synthesisGrid->count())
        {
            isSynthesis = true;
            synthesisDefen(m_synthesisGrid);
            m_synthesisGrid->removeAllObjects();
        }
        else
        {
            bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
            if (_isRefreshMap)
            {
                SGBattleManager::sharedBattle()->openReceive();
                if (isPlayerOperation)
                {
                    if (!isSynthesis)
                    {
                        SGBattleManager::sharedBattle()->fightLogRequest(str_Player_operation_end);
                        SGHeroLayer *heroLayer = SGBattleManager::sharedBattle()->getHeroLayer(isme);
                        
                        if (isMoveOperation)
                        {
                            if (operationNum > 0)
                            {
                                if (operationNum - 2 > 0)
                                {
                                    heroLayer->changRoundNum(operationNum - 2, true);
                                    activateSkills4(operationNum - 2);
                                }
                                else
                                {
                                    heroLayer->changRoundNum(operationNum - 2, false);
                                }
                                if (round &&
                                    isme)
                                {
                                    if (operationNum - 2 >= 0)
                                    {
                                        int a = (operationNum - 1) * ROUND_ADD_TIME_NUM;
                                        heroLayer->setRoundTime(heroLayer->_roundTime + a);
                                    }
                                }
                            }
                            else
                            {
                                heroLayer->changRoundNum(-1, false);
                            }
                            isMoveOperation = false;
                            isPlayerOperation = false;
                        }
                        else
                        {
                            if (operationNum > 0)
                            {
                                if (operationNum - 1 > 0)
                                {
                                    heroLayer->changRoundNum(operationNum - 1, true);
                                    activateSkills4(operationNum - 1);
                                }
                                else
                                {
                                    heroLayer->changRoundNum(operationNum - 1, false);
                                }
                                
                                if (round &&
                                    isme)
                                {
                                    if (operationNum - 1 >= 0)
                                    {
                                        int a = (operationNum) * ROUND_ADD_TIME_NUM;
                                        heroLayer->setRoundTime(heroLayer->_roundTime + a);
                                    }
                                }
                            }
                            else
                            {
                                heroLayer->changRoundNum(-1, false);
                            }
                            isPlayerOperation = false;
                        }
                        
                        SGBattleManager::sharedBattle()->getBattleLayer()->setOperationName(str_No_operation);
                        
                        if (operationNum >= 2)
                        {
                            heroLayer->setSkillsRoundNum(-(operationNum - 1), true);
                        }
                        operationNum = 0;
                        isUpdateEnd = true;
                        
                        if (round &&
                            isme)
                        {
                            if (heroLayer->isRoundEnd &&
                                heroLayer->getRound() > 0)
                            {
                                heroLayer->isRoundEnd = false;
                                SGBattleManager::sharedBattle()->giveupRequest();
                                SGBattleManager::sharedBattle()->getBattleLayer()->changeRound();
                            }
                            else
                            {
                                heroLayer->beginRoundTime();
                            }
                        }
                        
                        if (isme || SGAIManager::shareManager()->isPVE)
                        {
                            if (heroLayer->getRound() > 0)
                            {
                                if (!SGBattleManager::sharedBattle()->getIsBattlePve())
                                {
                                    //pve不验证
                                    SGBattleManager::sharedBattle()->fightLogRequest(str_Operation_end_verify);
                                    SGBattleManager::sharedBattle()->validationRequet(getMapGrid());
                                }
								
                            }
                            
                            if (heroLayer->getRound() <= 0)
                            {
                                if (SGGuideManager::shareManager()->isGuide)
                                {
                                    if (SGGuideManager::shareManager()->getIsChangeRound())
                                    {
                                        SGBattleManager::sharedBattle()->giveupRequest();
                                        SGBattleManager::sharedBattle()->getBattleLayer()->changeRound();
                                    }
                                }
                                else
                                {
                                    SGBattleManager::sharedBattle()->giveupRequest();
                                    SGBattleManager::sharedBattle()->getBattleLayer()->changeRound();
                                }
								
                            }
                            
                        }
                    }
                    else
                    {
                        SGBattleManager::sharedBattle()->fightLogRequest(str_Player_operation_fusioning);
                    }
                    
                }
                else
                {
                    if (!isSynthesis)
                    {
                        SGBattleManager::sharedBattle()->fightLogRequest(str_Not_Player_operation);
                        SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
                        
                        if (operationNum > 0)
                        {
                            heroLayer->changRoundNum(operationNum, true);
                            activateSkills4(operationNum);
                            if (operationNum >= 2)
                            {
                                heroLayer->setSkillsRoundNum(-(operationNum - 1), true);
                            }
                            
                            if (round &&
                                isme)
                            {
                                int a = operationNum * ROUND_ADD_TIME_NUM;
                                heroLayer->setRoundTime(heroLayer->_roundTime + a);
                            }
                            
                        }else
                        {
                            heroLayer->changRoundNum(operationNum, false);
                        }
                        operationNum = 0;
                        
                        if (round &&
                            isme)
                        {
                            heroLayer->beginRoundTime();
                        }
                    }
                    else
                    {
                        SGBattleManager::sharedBattle()->fightLogRequest(str_Not_Player_operation_fusioning);
                    }
                    
                    SGBattleManager::sharedBattle()->fightLogRequest(str_Not_Player_operation_verify);
                }
				
                this->removeChildByTag(1024, true);
                _isRefreshMap = false;
                if (isme)
                {
                    if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
                    {
                        SGGuideManager::shareManager()->startFightGuide(4);
                    }
                    else
                    {
                        _isRefreshMap = true;
						CCLOG("Forth Guide in [%s-%d]", __FUNCTION__, __LINE__);
                        SGGuideManager::shareManager()->startAutoGuide(4);
                    }
                }
                
				m_touchState = eTouchNull;
                
                SGBattleManager::sharedBattle()->fightLogRequest(str_All_operation_over);
                if (!isme)
                {
                    CCLog("$*******刷新结束，进入ai:SGBattleMap::deleteMoveEndDetection******");
                    this->initAiWatch();
                }
                else
                {
                    CCLog("$####AI: 自己操作不ai");
                    
                    //如果开启了自动战斗，那就进行加入定时器检测AI
                    this->endSingleRoundOrAddAICheck(true);
                }
				
            }
            else
            {
                CCLOG("$--刷新地图");
                refreshMapGrid(true);
				
            }
			
			
        }
        
    }
    
    if(fangyu->count())
		fangyu->removeAllObjects();
    if(spgongji->count())
		spgongji->removeAllObjects();
    if(gongji->count())
		gongji->removeAllObjects();
    
    
    //已经操作，提示同一个地方
    if (m_battleCurstoryId != 1)
    {
        needStartShowFriendly = true;
        CCLog("刷新之后提示");
        hideFriendlyHelp();
    }
    setIsMoveAble();
}

void SGBattleMap::activateSkills4(int num)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(this->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        
        if (attack->getSkillsDelegate()) {
            attack->activateSkills4( this, attack, num);
        }
    }
    
    
}

GameIndex SGBattleMap::getNormalAttackR(cocos2d::CCArray *array)
{
    GameIndex index = gameIndex(mapRow, mapList);
    for (int i = 0; i < array->count(); i++) {
        ShiBing *bing = (ShiBing *)array->objectAtIndex(i);
        SGGridBase *grid =  (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
        if (index.i > grid->getIndex().i) {
            index = grid->getIndex();
        }
    }
    return index;
}

//检测所的的地图上所有的网格
void SGBattleMap::detectionAllGrid()
{
   // GPCCLOG("detectionAllGrid");
    unschedule(schedule_selector(SGBattleMap::detectionAllGrid));
    refreshMapGrid(true);
    _isRefreshMap = false;
}

void SGBattleMap::addJointlyGrid( ShiBing *bing, cocos2d::CCArray *array)
{
    
	//    SGSBObj *sbObj = new SGSBObj();
	
    SGSBObj *obj = new SGSBObj();
    
    
    obj->setAp(bing->getAp());
    obj->setDef(bing->getResetInitDef());
    
    obj->setAddAp(bing->getAddAp());
    obj->setChainNUm(bing->getChainNUm());
    obj->setInitDef(bing->getInitDef());
    obj->setResetInitDef(bing->getResetInitDef());
    obj->setDefRend(bing->getDefRend());
    obj->setSkillsId(bing->getSkillsId());
    obj->setAfftributeNum(bing->getAfftributeNum());
    obj->setAfftributeNum1(bing->getAfftributeNum1());
    obj->setAfftributeNum2(bing->getAfftributeNum2());
    obj->setRound(bing->getRoundV());
    obj->setIndex(bing->getSbindex());
    obj->setSid(bing->getSbId());
    obj->setColorId(bing->getSbColor());
    obj->setType(bing->getSbType());
    obj->setName(bing->getName());
    obj->setAtkmode(bing->getAtkmode());
    obj->setAtkMode2(bing->getAtkMode2());
    obj->setAtkModeSkill(bing->getAtkModeSkill());
    obj->setHeadId(bing->getHeadId());
    obj->setFileId(bing->getFileId());
    obj->setPair(bing->getPair());
    obj->setHitEff(bing->getHitEff());
    obj->setAttackEffect(bing->getAttackEffect());
    obj->setDeathEffect(bing->getDeathEffect());
    obj->setChainNUm(bing->getChainNUm());
    obj->setRoundVAddDef(bing->getRoundVAddDef());
    obj->setFriendOff(bing->getFriendOff());
    obj->setDefType(bing->getDefType());
    obj->setSkill_Head(bing->getSkill_Head());

    ShiBing* m_jointlySb = ShiBing::createWithData(obj,this,this->isme);
    SGGridBase *gb = (SGGridBase*)m_jointlySb->getPrinceGridArray()->objectAtIndex(0);
    m_jointlySb->setPosition(GameConfig::getGridPoint(gb->getIndex().i, gb->getIndex().j, isme));
    soldiers->addObject(m_jointlySb);
//    m_jointlySb->setanimaID(ACT_wait);
//    m_jointlySb->setisloop(true);
    m_jointlySb->setanimaID(ACT_wait, true);
    
    
    array->addObject(m_jointlySb);
    m_jointlySbArray->addObject(m_jointlySb);
    obj->release();
}

void SGBattleMap::removeJointlyGrid(ShiBing *bing)
{
    SGGridBase *gb = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
    myGrids[gb->getIndex().i][gb->getIndex().j] = NULL;
}

bool SGBattleMap::chackAttackChain(SGAttackList *attack)
{
	
    if (attackLists->count())
    {
        
        bool isaddList = false;
        for (int i = 0; i < attackLists->count(); i++)
        {
            
            SGAttackList *al = (SGAttackList*)attackLists->objectAtIndex(i);
            if (al->chackAttackFuse(attack))
            {
                return true;
            }
        }
        
        for (int i = 0; i < attackLists->count(); i++)
        {
            SGAttackList *al = (SGAttackList*)attackLists->objectAtIndex(i);
            if(al->chackAttackChain(attack))
            {
                if (!isaddList)
                {
                    isaddList = true;
                }
            }
        }
        
        if (isaddList)
        {
            attack->drawEffectSp(1);
            if(liansuoChar == NULL)
            {
                liansuoChar = CCSkeletonAnimation::createWithFile("effSpine/liansuo.json", "effSpine/liansuo.atlas");
                liansuoChar->setAnimation("animation", false);
                addChild(liansuoChar, 1000, LIANSUO_CHAR_TAG);
                liansuoChar->setPosition(0, 600);
            }
            else
            {
                liansuoChar->setAnimation("animation", false);
            }
            
            
            bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
            SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(round);
            for (int i = 0; i < hero->battleMap->m_heroBuff->count(); i++)
            {
                CCDictionary *buff = (CCDictionary*)hero->battleMap->m_heroBuff->objectAtIndex(i);
                int id = ((CCString*)buff->objectForKey("buffId"))->intValue();
                if (id == 1011)
                {
                    float value1 = ((CCString*)buff->objectForKey("value1"))->floatValue();
                    int ap = attack->getMax_Ap() * value1;
                    attack->setSpAddAp(attack->getSpAddAp() + ap);
                    attack->changeAP(attack->getAp() + ap, true);
                    ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);
                    attack->showBattleLabel(ap, "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
                    break;
                }
            }
            
        }
    }
    
    return false;
}
//检测正常的攻击阵列
CCArray* SGBattleMap::chackNormalAttackList(SGGridBase *grid)
{
	CCArray *attackArray = CCArray::create();
	
    if (grid->getIndex().i < 0 ) {
        return attackArray;
    }
    ShiBing *selectSB = getShiBingWithGrid(grid);
	
    if (grid->getStype() == kdefend&&selectSB) {
		
        addJointlyGrid(selectSB, attackArray);
    }
    else
    {
        attackArray->addObject(selectSB);
    }
	
    GameIndex index = grid->getIndex();
    
	//不是最后一列,从上往下扫描
    if (index.i > 0) {
        for (int i=index.i-1; i>=0; i--)
        {
            SGGridBase *nextGrid = myGrids[i][index.j];
            if (nextGrid &&
                nextGrid->getStype() == knormal)//正常格子
            {
                ShiBing *nextSB = getShiBingWithGrid(nextGrid);
				if(nextSB)
                {
                    if ( nextSB->getSbId() == selectSB->getSbId() &&
                        selectSB->getSbColor() == nextSB->getSbColor() &&
                        nextSB->getSbType() == ksbone)//同兵种,同颜色,士兵
                    {
                        attackArray->addObject(nextSB);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                break;
            }
        }
    }
    bool isRouhe = false;
    if (attackArray->count() >= 3)//可以构成阵列
    {
        if (m_jointlySbArray->count())
		{
            for (int joi = 0; joi < m_jointlySbArray->count(); joi++)
			{
                ShiBing *jointlybing = (ShiBing*)m_jointlySbArray->objectAtIndex(joi);
                this->addChild(jointlybing);
            }
            m_jointlySbArrarCopy->addObjectsFromArray(m_jointlySbArray);
            m_jointlySbArray->removeAllObjects();
        }
        
        SGNormalAttackList *al = SGNormalAttackList::createWithSbs(attackArray,this->isme);
        
        if (!chackAttackChain(al))
		{
            attackLists->addObject(al);
            m_sameTimeSB->addObject(al);
        }
		else
		{
            isRouhe = true;
            //removeGrids(attackArray);
        }
        
        operationNum++;
    }
    else
    {
        if (index.i < (mapRow - 1))
        {
            for (int i=index.i+1; i < mapRow; i++)
            {
                SGGridBase *nextGrid = myGrids[i][index.j];
                if (nextGrid && nextGrid->getStype() == knormal)
                {
                    ShiBing *nextSB = getShiBingWithGrid(nextGrid);
                    if (nextSB&& nextSB->getSbId() == selectSB->getSbId() &&
                        selectSB->getSbColor() == nextSB->getSbColor())
                    {
                        attackArray->addObject(nextSB);
                        
                        if (attackArray->count() ==3)
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
					
                }
                else
                {
                    break;
                }
            }
        }
        if (attackArray->count() == 3)
        {
            if (m_jointlySbArray->count())
            {
                for (int joi = 0; joi < m_jointlySbArray->count(); joi++)
                {
                    ShiBing *jointlybing = (ShiBing*)m_jointlySbArray->objectAtIndex(joi);
                    this->addChild(jointlybing);
                }
                m_jointlySbArrarCopy->addObjectsFromArray(m_jointlySbArray);
                m_jointlySbArray->removeAllObjects();
            }
            
            SGNormalAttackList *al = SGNormalAttackList::createWithSbs(attackArray, this->isme);
            
            if (!chackAttackChain(al))
            {
                attackLists->addObject(al);
                m_sameTimeSB->addObject(al);
            }
            else
            {
                isRouhe = true;
                //removeGrids(attackArray);
            }
			
            operationNum++;
        }
        else
        {
            attackArray->removeAllObjects();
        }
    }
    if (!attackArray->count())
    {
        if (m_jointlySbArray->count())
        {
            m_jointlySbArray->removeAllObjects();
            myGrids[grid->getIndex().i][grid->getIndex().j] = grid;
        }
    }

    
    if(isRouhe)
    {
        removeGrids(attackArray);
    }
    return attackArray;
    
}
//检测能否组成攻击阵列
CCArray * SGBattleMap::chack2AttackList(SGGridBase *grid)
{
	
	CCArray *attackArray = CCArray::create();
	
	
	//如果不是士兵,或者已经是攻击阵列
    if (grid->getSbType() != ksbone || grid->getStype() == kattack)
    {
        return attackArray;
    }
    ShiBing *selectSB = getShiBingWithGrid(grid);
    if(selectSB)
        attackArray->addObject(selectSB);
	
    GameIndex index = grid->getIndex();
    bool is2can = false;
    bool is4can = false;
    ShiBing *prince = NULL;
    
	//向这个格子之后一直扫描
    for (int i=index.i-1; i>=0; i--)
    {
        SGGridBase *nextGrid = myGrids[i][index.j];
        if (nextGrid && nextGrid ->getStype() == knormal)//确保之后的格子是正常可用的
        {
            if (nextGrid->getSbType() == ksbone)//如果是士兵
            {
                if (nextGrid->getSaveID() == grid->getSaveID() &&
                    nextGrid->getSaveColorID() == grid->getSaveColorID() &&
                    attackArray->count()<2)//兵的id相同(同种类的兵),颜色id相同,并且目前还没组成攻击阵列
                {
                    ShiBing *nextSB = getShiBingWithGrid(nextGrid);
                    if(nextSB)
                        attackArray->addObject(nextSB);
                }
                else
                {
                    attackArray->removeAllObjects();
                    break;
                }
            }
            else//不是兵的情况
            {
                if (nextGrid->getSbType() == ksbtwo)//如果是二格武将
                {
					//后面的兵与这个武将的颜色相同
                    if (nextGrid->getSaveColorID() == grid->getSaveColorID())
					{
                        if (attackArray->count() == 2)
                        {
                            ShiBing *nextSB = getShiBingWithGrid(nextGrid);
                            if(nextSB)
                            {
                                SGTwoAttackList *al = SGTwoAttackList::createWithSoldier(nextSB, this->isme);
                                if (!chackAttackChain(al))
                                {
                                    attackLists->addObject(al);
                                    m_sameTimeWJ->addObject(al);

                                }
                                else
                                {
                                    removeSbObj(nextSB);
                                }
                                m_spAttackArray->addObject(nextSB);
                                m_jointlySbArrarCopy->addObjectsFromArray(m_jointlySbArray);
                                
                                for (int i = 0; i < attackArray->count(); i++) {
                                    ShiBing *sbbing = (ShiBing*)attackArray->objectAtIndex(i);
                                    SGGridBase * sbgrid = sbbing->getOnlyGrid();
                                    if (sbgrid->getStype() == kdefend) {
                                        attackArray->removeObjectAtIndex(i);
                                    }else{
                                        sbbing->changSBState(kattack);
                                    }
                                    
                                }
                                
                                operationNum++;
                                
                                return attackArray;
                            }
                        }
                        else
                        {
                            prince = getShiBingWithGrid(nextGrid);
                            is2can = true;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }else if (nextGrid->getSbType() == ksbfour)
                {
                    if (nextGrid->getSaveColorID() == grid->getSaveColorID())
                    {
                        prince = getShiBingWithGrid(nextGrid);
                        is4can = true;
                        break;
                    }
                    else
                    {
                        attackArray->removeAllObjects();
                        break;
                    }
                }
            }
        }
        else
        {
            attackArray->removeAllObjects();
            break;
        }
    }
    if (is2can)
    {
        if (index.i < mapRow - 1) {
            SGGridBase *nextGrid = myGrids[index.i+1][index.j];
            if (nextGrid &&
                nextGrid->getSaveID() == grid->getSaveID() &&
                nextGrid->getSaveColorID() == grid->getSaveColorID() )
            {
                ShiBing *nextSB = getShiBingWithGrid(nextGrid);
                if(nextSB)
                    attackArray->addObject(nextSB);
				
                m_spAttackArray->addObject(prince);
                CCLog("chack2AttackList:蓄力。。。。");
                SGTwoAttackList *al = SGTwoAttackList::createWithSoldier(prince, this->isme);
                if (!chackAttackChain(al))
                {
                    attackLists->addObject(al);
                    m_sameTimeWJ->addObject(al);
                    
                }else{
                    removeSbObj(prince);
                }
                m_jointlySbArrarCopy->addObjectsFromArray(m_jointlySbArray);
                for (int i = 0; i < attackArray->count(); i++) {
                    ShiBing *sbbing = (ShiBing*)attackArray->objectAtIndex(i);
                    SGGridBase * sbgrid = sbbing->getOnlyGrid();
                    if (sbgrid->getStype() == kdefend) {
                        attackArray->removeObjectAtIndex(i);
                    }else{
                        sbbing->changSBState(kattack);
                    }
                }
                
                operationNum++;
                
                return attackArray;
            }
            else
            {
                attackArray->removeAllObjects();
            }
        }
        
    }
	
    if (is4can)
    {
        if (attackArray->count() <2)
        {
            if (index.i < mapRow - 1) {
                SGGridBase *nextGrid = myGrids[index.i+1][index.j];
                if (nextGrid &&
                    nextGrid->getSaveID() == grid->getSaveID() &&
                    nextGrid->getSaveColorID() == grid->getSaveColorID())
                {
                    ShiBing *nextSB = getShiBingWithGrid(nextGrid);
                    if(nextSB)
                        attackArray->addObject(nextSB);
                    m_jointlySbArrarCopy->addObjectsFromArray(m_jointlySbArray);
                    
                }
                else
                {
                    attackArray->removeAllObjects();
                }
            }
            
        }
        if (attackArray->count()== 2)
		{
			SGSuperPrinceBase* sp = (SGSuperPrinceBase *)prince;
			bool isatLeft = sp->isAtLeft(grid->getIndex().j);
			int toj = isatLeft?1:-1;
			CCObject *obj = NULL;
			CCArray *attackTemp = CCArray::create();
			CCARRAY_FOREACH(attackArray, obj)
			{
				ShiBing *inSb = (ShiBing *)obj;
				
				SGGridBase *nowGrid = inSb->getOnlyGrid();
				SGGridBase *nextGrid = myGrids[nowGrid->getIndex().i][nowGrid->getIndex().j+toj];
				if (nextGrid && (nextGrid->getStype() == knormal ||
								 nextGrid->getStype() == kdefend))
				{
					ShiBing *nextSb = getShiBingWithGrid(nextGrid);
					if (nextSb && inSb->getSbId() == nextSb->getSbId() &&
						inSb->getSbColor() == nextSb->getSbColor())
					{
						attackTemp->addObject(nextSb);
						//attackArray->addObject(nextSb);
					}
				}
			}
			
			CCARRAY_FOREACH(attackTemp, obj)
			{
				attackArray->addObject(obj);
			}
		}
		
        if (attackArray->count() ==4)
        {
            m_spAttackArray->addObject(prince);
            
            CCLog("chack2AttackList:蓄力。。。。");
            SGFourAttackList *al = SGFourAttackList::createWithSoldier(prince, this->isme);
            if (!chackAttackChain(al)) {
                attackLists->addObject(al);
                m_sameTimeWJ->addObject(al);
            }else{
                removeSbObj(prince);
            }
            
            m_jointlySbArrarCopy->addObjectsFromArray(m_jointlySbArray);
            for (int i = 0; i < attackArray->count(); i++) {
                ShiBing *sbbing = (ShiBing*)attackArray->objectAtIndex(i);
                SGGridBase * sbgrid = sbbing->getOnlyGrid();
                if (sbgrid->getStype() == kdefend) {
                    attackArray->removeObjectAtIndex(i);
                }else{
                    sbbing->changSBState(kattack);
                }
            }
            operationNum++;
        }
        else
        {
            attackArray->removeAllObjects();
        }
    }else
    {
        attackArray->removeAllObjects();
    }
    return attackArray;
    
}

void SGBattleMap::delayChangeSbState(cocos2d::CCNode *psender)
{
    ShiBing*  sb = (ShiBing*)psender;
    sb->changSBState(kdefend);//改变士兵的状态成为墙,并在内部改变外观,并做数值改变(墙的防御值)

}

//检测是否可以组成防御
CCArray* SGBattleMap::chackdefendList(SGGridBase *grid)
{
    CCArray* defendArray = CCArray::create();
    if (grid->getStype() != knormal) {
        return defendArray;
    }
    
    GameIndex index = grid->getIndex();
    ShiBing *selectSB = getShiBingWithGrid(grid);
	if(selectSB)
    {
        defendArray->addObject(selectSB);
        //右,从这个格子一直向右边遍历
        
        if (index.j < (mapList - 1)) {
            
            for (int j=index.j+1; j < mapList; j++)
            {
                SGGridBase *nextGrid = myGrids[index.i][j];
                if (nextGrid && nextGrid->getStype() == knormal&&
                    nextGrid->getSbType() == ksbone)//向右遍历并且右侧是正常的格子,并且是士兵
                {
                    ShiBing *nextSB = getShiBingWithGrid(nextGrid);
                    
                    if (nextSB&&nextSB->getSbId() == selectSB->getSbId() &&
                        selectSB->getSbColor() == nextSB->getSbColor())//士兵ID相同,士兵颜色相同,才可以组成墙
                    {
                        defendArray->addObject(nextSB);
                    }
                    else
                    {
                        break;
                    }
                    
                }
                else
                {
                    break;
                }
            }
        }
        //左,从这个格子一直向左遍历
        if(index.j > 0 ){
            for (int j=index.j-1; j >=0; j--)
            {
                SGGridBase *nextGrid = myGrids[index.i][j];
                if (nextGrid && nextGrid->getStype() == knormal && nextGrid->getSbType() == ksbone)//是可用的正常格子,并且是士兵
                {
                    ShiBing *nextSB = getShiBingWithGrid(nextGrid);
                    if (nextSB&&selectSB->getSbId() == nextSB->getSbId() &&
                        selectSB->getSbColor() == nextSB->getSbColor())//相同id,相同颜色
                    {
                        defendArray->addObject(nextSB);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        
        if (defendArray->count() >= 3)//可以组成墙
        {
            for (int i = 0; i<defendArray->count(); i++)
            {
                ShiBing *bing = (ShiBing *)defendArray->objectAtIndex(i);
//                ResourceManager::sharedInstance()->bindTexture("effects/wall_1.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
//                CCSpriterX *effect1 = CCSpriterX::create("effects/wall_1.scml", true, true);
//                effect1->setanimaID(0);
//                effect1->setisloop(false);
//                CCPoint pos1 =   GameConfig::getGridPoint(bing->getGridIndex().i , bing->getGridIndex().j, isme);
//                effect1->setPosition(pos1);
//                this->addChild(effect1, 10000);
//                CCDelayTime*  delayShow = CCDelayTime::create(0.0f);
//                CCCallFuncN*  callShow = CCCallFuncN::create(this, callfuncN_selector(SGBattleMap::delayChangeSbState));
//                bing->runAction(CCSequence::create(delayShow, callShow, NULL));
                
                bing->changSBState(kdefend);//改变士兵的状态成为墙,并在内部改变外观,并做数值改变(墙的防御值)
                
                
            }
            
            ShiBing *bing=(ShiBing *)defendArray->objectAtIndex(1);
            showDefendName(bing);//在墙中间的位置出现名称
            operationNum++;//行动力加1?
        }
        else//不可以组成,就清空
        {
            defendArray->removeAllObjects();
        }
        //组成多个墙,行动力再次加1
        if (defendArray->count() >= 5) {
            operationNum++;
        }
    }
    return defendArray;
}

CCArray *SGBattleMap::getAttackList()
{
    return attackLists;
}

void SGBattleMap::shackDrop(cocos2d::CCArray *array)
{
    if (this->getHero_Obj()->isme) return;
    
    if (SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound()) {
        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(true);
        if (hero->isInitPve) {
            hero->chackBattleFieldDrop(array);
        }
    }
}
//删除一个攻击阵列
void SGBattleMap::removeAttackList(SGAttackList *attackList, SGHeroLayer *hero, SGGridBase *grid, bool isMe,bool showDrops, bool isDelay)
{
    if (isDelay == false)
    {

        if (!isMe && attackList->getAttackType() != kattacknormal)
        {
            attackList->playEffectWithType(kDeadEffect);
        }
        if (attackList->getReviveNum() > 0 && !attackList->getIsRevive())//可复活,且没有复活
        {
            attackList->setAp(0);
            attackList->setIsDeath(true);
            CCObject *sbobj = NULL;
            CCARRAY_FOREACH(attackList->attackSbs, sbobj)
            {
                ShiBing *sbo = (ShiBing*)sbobj;
                sbo->setVisible(false);
             }
            return;
        }
        
        EFFECT_PLAY(MUSIC_4);   //删除士兵的声音
        if (hero && grid)
        {
            //hero->showBattleLabel(attackList->getAp(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attackList->getAttackIndex());
        }
        if (attackList->getSkillsDelegate())
        {
            attackList->getSkillsDelegate()->removeReviveAttack(this, attackList);
        }
        if (showDrops)
        {
            //this->shackDrop(attackList->attackSbs);  //removed by cgp , 战斗中掉落
        }

        CCObject *sbobj1 = NULL;

        CCARRAY_FOREACH(attackList->attackSbs, sbobj1)
        {
            ShiBing *sbo1 = (ShiBing*)sbobj1;
            sbo1->setVisible(false);

            
        }

        
        this->removeGrids(attackList->attackSbs);
        attackList->removeFromParentAndCleanup(true);
        attackLists->removeObject(attackList);

    }
    else
    {
        
    }
}
//删除被攻击的士兵
void SGBattleMap::removeAttackedSB(ShiBing *sb, int attackv, SGHeroLayer *hero)
{
    
    if (sb->getSbType() != ksbone) {
        SGOfficerDataModel *officerData=SGStaticDataManager::shareStatic()->getOfficerById(sb->getSbId());
        if (officerData) {
            int num=officerData->getSoundId();
            CCString *name = CCString::createWithFormat("music/%d_Dead.mp3",num);
            EFFECT_PLAY(name->getCString());
        }
    }
    
    if (hero)
    {
        //hero->showBattleLabel(sb->getDef(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
    }
    SGGridBase *grid = sb->getOnlyGrid();
    if (grid->getStype() == kdefend)
    {
        EFFECT_PLAY(MUSIC_13);
    }
    else
    {
    	std::string eff(MUSIC_4);   //删除士兵的声音
        EFFECT_PLAY(eff.c_str());
    }
    CCArray *array = CCArray::create();
    array->addObject(sb);
    //this->shackDrop(array);
    this->removeGrids(array);
}


/*void SGBattleMap::refreshMapEnd(CCObject *sender)
 {
 
 //    moveNum++;
 //    if (moveNum == m_moveAllGrid->count()) {
 //        deleteMoveEndDetection();
 //        moveNum = 0;
 //    }
 }
 
 //void SGBattleMap::refreshMapEnd(CCObject *sender)
 //{
 //
 ////    moveNum++;
 ////    if (moveNum == m_moveAllGrid->count()) {
 ////        deleteMoveEndDetection();
 ////        moveNum = 0;
 ////    }
 //}
 
 */



//获取地图上的网格
CCArray * SGBattleMap::getMapGrid()
{
    
    if (isme) {
    }else{
    }
    
    CCString * str = NULL;
    
    SGHeroLayer *heroLayer = SGBattleManager::sharedBattle()->getHeroLayer(isme);
    
    str = CCString::createWithFormat("%d", heroLayer->getRoleId());
    str = CCString::createWithFormat("%s:%d", str->getCString(), heroLayer->getSoldier());
    str = CCString::createWithFormat("%s:%d",str->getCString(), heroLayer->getRound());
    str = CCString::createWithFormat("%s:%d",str->getCString(), heroLayer->getBlood());
    str = CCString::createWithFormat("%s:%d",str->getCString(), heroLayer->getSkillsRoundNum());
    
    for (int i = 0; i < mapList; i++) {
        str = CCString::createWithFormat("%s      ",str->getCString());
        for (int j = 0; j < mapRow; j++) {
            
            SGGridBase *grid = myGrids[j][i];
            if (grid) {
                
                ShiBing *bing = getShiBingWithGrid(grid);
                if(bing)
                {
                    int numId = 0;
                    int apNum = 0;
                    int defNum = 0;
                    int roundNum = 0;
                    switch (bing->getSbType()) {
                            
                        case ksbtwo:
                        {
                            numId = 1;
                        }
                            break;
                        case ksbfour:
                        {
                            numId = 2;
                        }
                            break;
                            
                        default:
                            break;
                    }
                    
                    str = CCString::createWithFormat("%s:%d",str->getCString(), numId);
                    
                    numId = 1;
                    switch (grid->getStype()) {
                        case knormal:
                        {
                            apNum = bing->getAp();
                            defNum = bing->getDef();
                            roundNum = 0;
                        }
                            break;
                        case kattack:
                        {
                            numId = 2;
                            SGAttackList *soldier = getAttackListByGrid(grid);
                            //andrtoid 下会出现空指针
                            if (soldier)
                            {
                                apNum = soldier->getAp();
                                defNum = 0;
                                roundNum = soldier->getRoundV();
                                if (apNum <= 0 && soldier->getReviveNum() <= 0) {
                                    //不能复活
                                    numId = -2;
                                }
                            }
                        }
                            break;
                        case kdefend:
                        {
                            numId = 3;
                            apNum = 0;
                            defNum = bing->getDef();
                            roundNum = 0;
                        }
                            break;
                            
                        default:
                            break;
                    }
                    if (numId < 0) {
                        str = CCString::createWithFormat("%s:%d ",str->getCString(), -1);
                    }
                    else
                    {
                        str = CCString::createWithFormat("%s:%d",str->getCString(), numId);
                        str = CCString::createWithFormat("%s:%d",str->getCString(), bing->getSbColor());
                        str = CCString::createWithFormat("%s:%d",str->getCString(), apNum);
                        str = CCString::createWithFormat("%s:%d",str->getCString(), defNum);
                        str = CCString::createWithFormat("%s:%d ",str->getCString(), roundNum);
                    }
                }
                
            }else
            {
                str = CCString::createWithFormat("%s:%d ",str->getCString(), -1);
            }
        }
    }
    str = CCString::createWithFormat("%s:",str->getCString());
	
    CCLOG("str==%s",str->getCString());
    CCArray *data = CCArray::create();
    
    int a = 0;
    int b = 0;
    while (a <str->length())
    {
        if (str->m_sString[a] == ':')
        {
            CCString *_data = CCString::create(str->m_sString.substr(b,a-b));
            data->addObject(_data);
            b = a+1;
        }
        a ++;
    }
    
    bool is = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    if (is) {
        int a = heroLayer->getBlood();
        CCString *str = CCString::createWithFormat(str_Format_Send_verify_hp,a);
        SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
    }
    
    
	
	////////////////////
	//////enemyLayer
#if (NEEDENEMYVALIDATION == 1)
    SGBattleMap * enemyMap = SGBattleManager::sharedBattle()->getBattleMap(true);
    heroLayer = SGBattleManager::sharedBattle()->getHeroLayer(!isme);
    
    str = CCString::createWithFormat("%d", heroLayer->getRoleId());
    str = CCString::createWithFormat("%s:%d", str->getCString(), heroLayer->getSoldier());
    str = CCString::createWithFormat("%s:%d",str->getCString(), heroLayer->getRound());
    str = CCString::createWithFormat("%s:%d",str->getCString(), heroLayer->getBlood());
    str = CCString::createWithFormat("%s:%d",str->getCString(), heroLayer->getSkillsRoundNum());
    
    bool getAllFal = true;
    for (int i = 0; i < mapList; i++) {
        str = CCString::createWithFormat("%s      ",str->getCString());
        for (int j = 0; j < mapRow; j++) {
            
            SGGridBase *grid = enemyMap->myGrids[j][i];
            if (grid) {
                
                ShiBing *bing = enemyMap->getShiBingWithGrid(grid);
                if(bing)
                {
                    int numId = 0;
                    int apNum = 0;
                    int defNum = 0;
                    int roundNum = 0;
                    switch (bing->getSbType()) {
                            
                        case ksbtwo:
                        {
                            numId = 1;
                        }
                            break;
                        case ksbfour:
                        {
                            numId = 2;
                        }
                            break;
                            
                        default:
                            break;
                    }
                    
                    str = CCString::createWithFormat("%s:%d",str->getCString(), numId);
                    
                    numId = 1;
                    switch (grid->getStype()) {
                        case knormal:
                        {
                            apNum = bing->getAp();
                            defNum = bing->getDef();
                            roundNum = 0;
                        }
                            break;
                        case kattack:
                        {
                            numId = 2;
                            SGAttackList *soldier = enemyMap->getAttackListByGrid(grid);
                            // 做一防御 (android下会出现空指针)
                            if (soldier)
                            {
                                apNum = soldier->getAp();
                                defNum = 0;
                                roundNum = soldier->getRoundV();
                                if (apNum <= 0 && soldier->getReviveNum() <= 0) {
                                    //不能复活
                                    numId = -2;
                                }
                            }
                        }
                            break;
                        case kdefend:
                        {
                            numId = 3;
                            apNum = 0;
                            defNum = bing->getDef();
                            roundNum = 0;
                        }
                            break;
                            
                        default:
                            break;
                    }
                    
                    if (numId < 0) {
                        str = CCString::createWithFormat("%s:%d ",str->getCString(), -1);
                    }
                    else
                    {
                        str = CCString::createWithFormat("%s:%d",str->getCString(), numId);
                        str = CCString::createWithFormat("%s:%d",str->getCString(), bing->getSbColor());
                        str = CCString::createWithFormat("%s:%d",str->getCString(), apNum);
                        str = CCString::createWithFormat("%s:%d",str->getCString(), defNum);
                        str = CCString::createWithFormat("%s:%d ",str->getCString(), roundNum);
                        getAllFal = false;
                    }
				}
                
            }
            else
            {
                str = CCString::createWithFormat("%s:%d ",str->getCString(), -1);
            }
        }
    }
    if (getAllFal)
    {
        CCLOG("AAAAA getAllFal!");
    }
    str = CCString::createWithFormat("%s:",str->getCString());
    
    CCLOG("EnemyStr==%s",str->getCString());
	
    a = 0;
    b = 0;
    while (a < str->length())
    {
        if (str->m_sString[a] == ':')
        {
            CCString *_data = CCString::create(str->m_sString.substr(b,a-b));
            data->addObject(_data);
            b = a+1;
        }
        a ++;
    }
    
    is = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    if (is) {
        int a = heroLayer->getBlood();
        CCString *str = CCString::createWithFormat(str_Format_Send_verify_enemy_hp,a);
        SGBattleManager::sharedBattle()->fightLogRequest(str->m_sString);
    }
	
#endif
	//////enemyLayer end
    
    return data;
}
//给主将加上buff
void SGBattleMap::addHeroBuff(cocos2d::CCDictionary *data)
{
    m_heroBuff->addObject(data);
}
//激活主将的buff,
void SGBattleMap::activateHeroBuff(bool isadd )
{
    bool activate = false;
    CCObject *obj = NULL;
    CCARRAY_FOREACH(m_heroBuff, obj)
    {
        CCDictionary *buff = (CCDictionary *)obj;
        int buffId = ((CCString *)buff->objectForKey("buffId"))->intValue();
        bool isRound = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
        if (isRound) {
            SGBattleManager::sharedBattle()->fightLogRequest(CCString::createWithFormat("buffid:%d", buffId)->getCString());
        }
        switch (buffId) {
            case 1004:
            {
                if (!activate) {
                    float value2 = ((CCString*)buff->objectForKey("value2"))->floatValue();
                    CCObject *attack = NULL;
                    CCARRAY_FOREACH(this->getAttackList(), attack)
                    {
                        SGAttackList *attackObj = (SGAttackList*)attack;
                        CCLOG("value2===%f",value2);
                        CCLOG("attackObj->getAp()==%d",attackObj->getAp());
						//                        int ap = attackObj->getAp() * value2;
                        attackObj->changeAP(attackObj->getAp() + value2);
                        attackObj->drawSkillsEffect(2);
                    }
                }
                
                activate = true;
            }
                break;
            case 1011://连锁加攻击
            {
                if (!activate) {
                    float value1 = ((CCString*)buff->objectForKey("value1"))->floatValue();
                    CCObject *obj = NULL;
                    CCARRAY_FOREACH(this->getAttackList(), obj)
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
                }
                
                activate = true;
            }
                break;
                
            default:
                break;
        }
        
        
    }
}


void SGBattleMap::removeHeroBuff()
{
    
}

//11.18
//------------------------------------------------------------------------------------------------------------------------------------------------


//显示闪烁的同时,显示手指指引
void SGBattleMap::displayTipsFinger(CCPoint startPos)
{
	//spTouch->stopAllActions();
    spTouch->stopActionByTag(0);
    spTouch->setVisible(true);
    spTouch->setPosition(ccpAdd(startPos, ccp(0, 0)));
    spTouch->setanimaID(0);
	spTouch->stop();
	
    CCAction *moveAction = CCRepeatForever::create(
                                                   CCSequence::create(
																	  CCScaleTo::create(DOWNTIME, DOWNSCALE),
																	  CCDelayTime::create(GAPTIME),
																	  CCScaleTo::create(UPTIME, 1.0), NULL));
    
	
    moveAction->setTag(0);
    spTouch->runAction(moveAction);
}
//------------------------------------------------------------------------------------------------------------------------------------------------

//void SGBattleMap::textYanZheng()
//{
//    if (isme) {
//
//    }
//}

//闪动指定index的网格
void SGBattleMap::flashIndex(GameIndex index ,int flashTag,bool isblue)
{
    const char* file = "battle_1_red_rect.png";
    if (SGGuideManager::shareManager()->actionType == 1 || isblue ) {
        file = "battle_1_green_rect.png";
    }
    CCSprite *sp = CCSprite::createWithSpriteFrameName(file);
    this->addChild(sp,-1);
    sp->setScale(1.2);
    sp->setTag(flashTag);
    sp->setPosition(ccpAdd(GameConfig::getGridPoint(index.i, index.j), ccp(0, sp->getContentSize().height*.5)));
    CCFadeIn *fadein = CCFadeIn::create(.5);
    CCFadeOut *fadeout = CCFadeOut::create(.5);
    sp->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(fadein,fadeout,NULL)));
    
    if (SGGuideManager::shareManager()->actionType == 2 || m_showDeletPop) {
        file = "uplevel_fclick.png";
        if (index.j == 7) { //最右边 换气泡
            file = "uplevel_lclick.png";
        }
        CCSprite *click = CCSprite::createWithSpriteFrameName(file);
        float posX = click->getContentSize().width*.5f;
        float posY = 0;//click->getContentSize().height*.06f;
        if (index.j == 7) { //最右边 换位置
            posX = -click->getContentSize().width*.5f;
        }
        this->addChild(click, 10);
        click->setTag(1025);
        click->setPosition(ccpAdd(sp->getPosition(), ccp(posX, posY)));
        CCAction *action = CCSequence::create(CCScaleTo::create(0.12f, 1.1f),
                                              CCScaleTo::create(0.12f, 1.2f),
                                              CCScaleTo::create(0.12f, 1.1f),
                                              CCScaleTo::create(0.12f, 1.0f),
                                              NULL);
        click->runAction(CCRepeatForever::create((CCActionInterval*)action));
    }
}

//显示移动操作的箭头
void SGBattleMap::showMoveArrow()
{
    ResourceManager::sharedInstance()->bindTexture("animationFile/Darrow.plist",RES_TYPE_BATTLE_IMAGE ,sg_battleLayer);
    CCSpriterX *darrow = NULL;
    int listIndex[5] ={1,2,4,5,7};
    int rowIndex[5] ={3,3,1,2,3};
    for (int i=0; i<5; i++) {
        darrow = CCSpriterX::create("animationFile/Darrow.scml",true,true);
        darrow->setisloop(true);
        darrow->play();
        //MMDEBUG: FLIPY
        darrow->setScaleY(-1);
//        darrow->setCCSpriterXFlipY(true);
        darrow->setanimaID(0);
        addChild(darrow,5,BATTLEDARROWTAG+i);
        darrow->setPosition(GameConfig::getGridPoint(rowIndex[i], listIndex[i]));
//        darrow->setPosition(ccpAdd(GameConfig::getGridPoint(rowIndex[i], listIndex[i]),ccp(0,-20)));
    }
    needremoveArrow = true;
}
//移除指示移动操作的箭头
void SGBattleMap::removeMoveArrow()
{
    for (int i=0; i<5; i++) {
        removeChildByTag(BATTLEDARROWTAG+i,true);
    }
    needremoveArrow = false;
}

void SGBattleMap::beginAi()
{
    //this->unschedule(schedule_selector(SGBattleMap::beginAi));
            SGAIManager::shareManager()->isActivateAI=true;
   
    SGAIManager::shareManager()->isAi=true;
	CCLog("$####开始ai,battleMap:beginAI######");
    SGAIManager::shareManager()->startRound(this);
	
}

//清空地图
void SGBattleMap::removeSoldiers(bool showDrops)
{
    SGAIManager::shareManager()->setNowStep(s_null);
    unschedule(schedule_selector(SGBattleMap::deleteMoveEndDetection));
    
    m_fangYuUpBingArray->removeAllObjects();
    m_nomalAttackUpBingArray->removeAllObjects();
    m_spAttackUpArray->removeAllObjects();
    m_deleteDownArray->removeAllObjects();
    m_moveAllGrid->removeAllObjects();
    m_jointlySbArray->removeAllObjects();
    m_jointlySbArrarCopy->removeAllObjects();
    m_synthesisGrid->removeAllObjects();
    m_spAttackGrid->removeAllObjects();
    m_moveSanGrid->removeAllObjects();
    m_removeSbObj->removeAllObjects();
    m_removeSbDef->removeAllObjects();
    
    
    m_touchState = eTouchNull;
    isTouch = true;
    _isRefreshMap = false;
    m_touchGrid = NULL;
    m_touchIndex = gameIndex(-1, -1);
    m_touchPosIndex = gameIndex(-1, -1);
    isPlayerOperation = false;
    isSynthesis = false;
    isMoveOperation = false;
    
    if (this->getParent()->getChildByTag(Move_Green_Rect)) {
        CCSpriterX *spx = (CCSpriterX*)this->getParent()->getChildByTag(Move_Green_Rect);
        if (touchSpXbj) {
//            touchSpXbj->removeAllChildrenWithCleanup(true);
            touchSpXbj->removeFromParentAndCleanup(true);
        }
        touchSpXbj = NULL;
//        spx->removeChildByTag(-TOUCH_INIT_SB_BJ, true);
        if (touchSpXbj_) {
//            touchSpXbj_->removeAllChildrenWithCleanup(true);
            touchSpXbj_->removeFromParentAndCleanup(true);
        }
        touchSpXbj_ = NULL;
//        spx->removeChildByTag(TOUCH_INIT_SB_BJ, true);
//        spx->removeAllChildrenWithCleanup(true);
        if (spx)
        {
            spx->removeFromParentAndCleanup(true);
            spx = NULL;
        }
    }
    
    this->getParent()->removeChildByTag(Move_Green_Rect, true);
    
    for (int i = 0; i < attackLists->count(); i++) {
        SGAttackList *at = (SGAttackList*)attackLists->objectAtIndex(i);
        removeAttackList(at,NULL,NULL,false,showDrops);
    }
    attackLists->removeAllObjects();
    
    for (int i = 0; i < soldiers->count(); i++) {
        ShiBing *sb = (ShiBing*)soldiers->objectAtIndex(i);
        
        //this->removeChild(sb, true);
        sb->cleanSbFile();
        
//        sb->removeAllChildrenWithCleanup(true);
        sb->removeFromParentAndCleanup(true);
    }
    
    soldiers->removeAllObjects();
    
    this->removeChildByTag(SB_TAG, true);
    
    for (int i = 0; i < mapRow; i++) {
        for (int j = 0; j < mapList; j++) {
            myGrids[i][j] = NULL;
        }
    }
}

void SGBattleMap::removeSbAr(cocos2d::CCArray *ar)
{
    if (ar->count()) {
        removeGrids(ar);
    }
}

void SGBattleMap::setTouchInitSbOpacity(bool scale)
{
    if (touchInitSb) {
        if (scale) {
            touchInitSb->setScale(1.0f);
        }
        
        touchInitSb->stopAllActions();
        touchInitSb->setVisible(true);
        if (touchSpXbj) {
//            touchSpXbj->removeAllChildrenWithCleanup(true);
            touchSpXbj->removeFromParentAndCleanup(true);
        }
        touchSpXbj = NULL;
//        touchInitSb->removeChildByTag(-TOUCH_INIT_SB_BJ, true);
        if (touchSpXbj_) {
//            touchSpXbj_->removeAllChildrenWithCleanup(true);
            touchSpXbj_->removeFromParentAndCleanup(true);
        }
        touchSpXbj_ = NULL;
//        touchInitSb->removeChildByTag(TOUCH_INIT_SB_BJ, true);
//        touchInitSb->removeFromParent();
        touchInitSb = NULL;
    }
}

//长时间没有操作后的友情提示
void SGBattleMap::showFriendlyHelp()
{
    unschedule(schedule_selector(SGBattleMap::showFriendlyHelp));
    
    //如果是在自动战斗中，不再进行这个提示
    if (SGAIManager::shareManager()->getIsOpenAutoBattle())
    {
        this->hideFriendlyHelp();
        return ;
    }
    
    
    //新手引导的第二关不再引导之后要进行操作提示
    if (!(SGPlayerInfo::sharePlayerInfo()->getPlotId() == 2 && !SGGuideManager::shareManager()->isGuide))
    {
        if (m_isGuide ||
            m_battleCurstoryId > SGMainManager::shareMain()->getShowFriendlyTipsChapId() ||
            battleType == (int)BT_SURVIVAL||
            battleType == (int)BT_LADDER) {
            return;
        }
    }
    
    if (m_battleInitpve && isme && SGAIManager::shareManager()->isPVE) {
        needStartShowFriendly = true;
        
        SuggestIndex suggestIndex = SGAIManager::shareManager()->returnSuggestIndex(this);
        SGHeroLayer *heroLayer = (SGHeroLayer*)this->getParent();
        CCSprite *msgLabel = NULL;
        if (bboard && !bboard->isVisible()) {
            bboard->setVisible(true);
        }
        else if(bboard && bboard->isVisible())
        {
            bboard->setVisible(false);
        }
        
        bboard->setVisible(true);// 小提示没有文字12.18
        
        removeChildByTag(MSGLABELTAG, true);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
        CCSize dsize = CCSizeMake(tsize.width-40, tsize.height-50);
        int tipZ = 5555;
        
        switch (suggestIndex.actionType) {
            case at_null:
            {
                if (spTouch != NULL) {
                    spTouch->setVisible(true);
                    spTouch->setPosition(heroLayer->xdl_btn->getPosition());
                }
                msgLabel = SGRichLabel::getFomLabel(str_give_up_this_round_tips, dsize, FONT_BOXINFO, 32);
                msgLabel->setPosition(ccpAdd(bboard->getPosition(),ccp(0,-15)));
                addChild(msgLabel,tipZ,MSGLABELTAG);
            }
                break;
            case at_delete:
            {
                m_showDeletPop = true;
                flashIndex(suggestIndex.deleteIndex,1000);
                msgLabel = SGRichLabel::getFomLabel(str_Delete_sb_tips, dsize, FONT_BOXINFO, 32);
                msgLabel->setPosition(ccpAdd(bboard->getPosition(),ccp(0,-15)));
                addChild(msgLabel,tipZ,MSGLABELTAG);
                if (spTouch != NULL) {
                    spTouch->setVisible(true);
                    spTouch->setPosition(GameConfig::getGridPoint(suggestIndex.deleteIndex.i, suggestIndex.deleteIndex.j));
                }
            }
                break;
            case  at_move :
            {
                SGGuideManager::shareManager()->actionType = 1;
                m_showDeletPop=false;
                flashIndex(suggestIndex.toIndex,1000);
                flashIndex(suggestIndex.fromIndex,1001);
                if (spTouch != NULL) {
                    spTouch->setVisible(true);
                    if (suggestIndex.fromIndex.j >8 || suggestIndex.fromIndex.i > 8) {
                        spTouch->setPosition(GameConfig::getGridPoint(suggestIndex.fromIndex.i, suggestIndex.fromIndex.j));
                    }
                    else
                    {
                        spTouch->setPosition(GameConfig::getGridPoint(suggestIndex.fromIndex.i, suggestIndex.fromIndex.j));
                    }
                }
                msgLabel = SGRichLabel::getFomLabel(str_Move_role_tips, dsize, FONT_BOXINFO, 32);
                msgLabel->setPosition(ccpAdd(bboard->getPosition(),ccp(0,-15)));
                addChild(msgLabel,tipZ,MSGLABELTAG);
            }
                break;
            case  at_mainSkill:
            {
                if (spTouch != NULL) {
                    spTouch->setVisible(true);
                    spTouch->setPosition(heroLayer->zjj_btn->getPosition());
                }
                msgLabel = SGRichLabel::getFomLabel(str_Start_zhujiangji_tips, dsize, FONT_BOXINFO, 32);
                msgLabel->setPosition(ccpAdd(bboard->getPosition(),ccp(0,-15)));
                addChild(msgLabel,tipZ,MSGLABELTAG);
            }
                break;
            case at_bu:
            {
                if (spTouch != NULL) {
                    spTouch->setVisible(true);
                    spTouch->setPosition(heroLayer->yb_btn->getPosition());
                }
                msgLabel = SGRichLabel::getFomLabel(str_Fill_soldier_tips, dsize, FONT_BOXINFO, 32);
                msgLabel->setPosition(ccpAdd(bboard->getPosition(),ccp(0,-15)));
                addChild(msgLabel,tipZ,MSGLABELTAG);
            }
                break;
            default:
                break;
        }
    }
}
//隐藏友情提示
void SGBattleMap::hideFriendlyHelp(bool startCounting)
{
    unschedule(schedule_selector(SGBattleMap::hideFriendlyHelp));
    unschedule(schedule_selector(SGBattleMap::showFriendlyHelp));
    
    
    if ((!(SGPlayerInfo::sharePlayerInfo()->getPlotId() == 2 && !SGGuideManager::shareManager()->isGuide)))
    {
        if ( m_isGuide||
            m_battleCurstoryId > SGMainManager::shareMain()->getShowFriendlyTipsChapId() ||
            battleType == (int)BT_SURVIVAL||
            battleType == (int)BT_LADDER) {
            return;
        }
    }
    if (m_battleInitpve && isme && SGAIManager::shareManager()->isPVE)
    {
        bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
        if ( startCounting && round && needStartShowFriendly)
        {
           
            if (isFirstRound)
            {
                schedule(schedule_selector(SGBattleMap::showFriendlyHelp),m_nAItips_time+8);
                isFirstRound=false;
            }
            else
            {
                schedule(schedule_selector(SGBattleMap::showFriendlyHelp),m_nAItips_time);
            }
             m_nAItips_time = 7;
            needStartShowFriendly = false;
        }
        else if (startCounting == false)
        {
            unschedule(schedule_selector(SGBattleMap::showFriendlyHelp));
            needStartShowFriendly = true;
        }
        removeChildByTag(MSGLABELTAG, true);
        
        bboard->setVisible(false);
        this->removeChildByTag(1000, true);
        this->removeChildByTag(1001, true);
        this->removeChildByTag(1025, true);
        if (spTouch != NULL)
        {
            spTouch->setVisible(false);
            spTouch->setPosition(ccp(-100,-100));
        }
    }
}

GameIndex SGBattleMap::findLastShiBin()
{
    GameIndex gameIndex = gameIndex(1, 1);
    int shiBinCount[mapList];
    for (int i = 0; i < mapList; i++)
    {
        shiBinCount[i] = 0;
    }

    for (int j=0; j<mapList; j++) {
        for (int i = mapRow -1; i>=0; i--)
        {
            if (myGrids[i][j])
            {
                if (myGrids[i][j]->getSbType() == ksbone) {
                    shiBinCount[j] = i;
                }
                break;
            }
        }
    }
    int list = 0;
    int row = shiBinCount[list];
    for (int k=1; k<mapList; k++) {
        if (shiBinCount[k] >row) {
            row = shiBinCount[k];
            list = k;
        }
    }
    gameIndex.i = row;
    gameIndex.j = list;
    return gameIndex;
}

GameIndex SGBattleMap::findFirstShiBin()
{
    GameIndex gameIndex = gameIndex(1, 1);
    int shiBinCount[mapList] ;
    for (int i = 0; i < mapList; i++)
    {
        shiBinCount[i] = mapRow;
    }
    for (int j=0; j<mapList; j++) {
        for (int i = mapRow -1; i>=0; i--)
        {
			if (myGrids[i][j])
            {
                if (myGrids[i][j]->getSbType() == ksbone) {
                    shiBinCount[j] = i;
                }
                break;
            }
        }
    }
    int list = 0;
    int row = shiBinCount[list];
    for (int k=1; k<mapList; k++) {
        if (shiBinCount[k] < row) {
            row = shiBinCount[k];
            list = k;
        }
    }
    gameIndex.i = row;
    gameIndex.j = list;
    return gameIndex;
}

GameIndex SGBattleMap::findMiddleShiBin()
{
    GameIndex gameIndex = gameIndex(1, 1);

    int middleList[mapList] = {3,4,2,5,1,6,0,7}; //removed by cgp for list
    //int middleList[mapList] = {2,3,1,4,5,0};



    for (int k =0; k<mapList; k++) {
        for (int i = mapRow-1; i>=0; i--) {
            if (myGrids[i][middleList[k]] && myGrids[i][middleList[k]]->getSbType() == ksbone) {
                gameIndex.i = i;
                gameIndex.j = middleList[k];
                return gameIndex;
            }
        }
    }
    return gameIndex;
}
void SGBattleMap::hideRemindMe(){
	
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(false);
    SGHeroLayer *hero1 = SGBattleManager::sharedBattle()->getHeroLayer(true);
    if (hero->statusBarLayer)
    {
        hero->statusBarLayer->setVisible(false);
        hero->statusBarLayer->setColor(ccWHITE);
        hero->gbSprite->stopAllActions();
        hero->gbSprite->setVisible(false);
        
        hero1->battleMap->setStartShowFriendly(true);
        hero1->battleMap->hideFriendlyHelp();
    }
}

void SGBattleMap::showDefendName(ShiBing *sb)
{
    if (SGBattleManager::sharedBattle()->isGuide) {
        
        CCLabelTTF *label1 = CCLabelTTF::create(str_Formation_defense_wall, FONT_BOXINFO, FONTSIZE(20));
        label1->setColor(ccBLACK);
        
        CCLabelTTF *label = CCLabelTTF::create(str_Formation_defense_wall, FONT_BOXINFO, FONTSIZE(20));
        label->addChild(label1);
        label->enableStroke(ccc3(52, 0, 0), 4);
        label1->setPosition(ccp(label1->getContentSize().width * 0.5, label->getContentSize().height * 0.52 + 2));
        label1->setColor(ccYELLOW);
        label->setColor(ccc3(52, 0, 0));
        label1->setOpacity(0);
        label->setOpacity(0);
        label->setPosition(ccp(0, gridHeight));
        sb->addChild(label);
        
        float time = 0.5f;
        CCMoveTo *move = CCMoveTo::create(time, ccp(label->getPosition().x, label->getPosition().y + gridHeight));
        CCFadeIn *fadei = CCFadeIn::create(time-0.2);
        CCScaleTo *scalet = CCScaleTo::create(time, 1.0f);
        CCSpawn *spawn = CCSpawn::create( move, fadei, scalet, NULL);
        CCDelayTime *dtime = CCDelayTime::create(time);
        CCFadeOut *fadeo = CCFadeOut::create(time-0.2);
        
        CCFadeIn *fadei1 = CCFadeIn::create(time-0.2);
        CCScaleTo *scalet1 = CCScaleTo::create(time, 1.0f);
        CCSpawn *spawn1 = CCSpawn::create( fadei1, scalet1, NULL);
        CCDelayTime *dtime1 = CCDelayTime::create(time);
        CCFadeOut *fadeo1 = CCFadeOut::create(time-0.2);
        
        label->runAction(CCSequence::create( spawn, dtime, fadeo, NULL));
        label1->runAction(CCSequence::create( spawn1, dtime1, fadeo1, NULL));
        
    }
}

CCArray *SGBattleMap::getFourList(){
    return fourList;
}

CCArray *SGBattleMap::getTwoList(){
    return twoList;
}

void SGBattleMap::setStartShowFriendly(bool _needStartShowFriendly){
	
    needStartShowFriendly=_needStartShowFriendly;
}

bool SGBattleMap::normalIsSameColor(int row,int listIndex,int colorId,bool isAtk)
{
    if (row < 0
        || row >= mapRow
        || listIndex < 0
        || listIndex >= mapList)
    {
        return false;
    }
	
    SGGridBase *tmp = fillUnitGrids[row][listIndex];
    if (tmp && tmp->getStype() == knormal
        && (isAtk||tmp->getSbType() == ksbone)
        && tmp->getSaveColorID() == colorId)
    {
        return true;
    }
    return false;
}

int SGBattleMap::testFillOne(int listIndex,int &colorId)
{
    if (listIndex<0 || listIndex >= mapList) {
        return -1;
    }
    int i = mapRow - 1;
    for (; i >= 0; i--) {
        if (fillUnitGrids[i][listIndex] != NULL)
        {
            //最后一个兵
            break;
        }
    }
    
    bool canFill = false;
    if (i < mapRow - 1)
    {
        i=i+1;
        for (int k = 0; k < 3; k++)
        {
            colorId = colorId > 2 ? (colorId%3):colorId;
            canFill = false;
            if ((normalIsSameColor(i,listIndex-2,colorId) && normalIsSameColor(i,listIndex-1,colorId) )//左边
                || (normalIsSameColor(i,listIndex-1,colorId) && normalIsSameColor(i,listIndex+1,colorId))//中间
                || (normalIsSameColor(i,listIndex+1,colorId) && normalIsSameColor(i,listIndex+2,colorId))//右边
                || (normalIsSameColor(i-2,listIndex,colorId,true) && normalIsSameColor(i-1,listIndex,colorId,true)))//下方
            {
                canFill = false;
            }
            else
            {
                canFill = true;
                break;
            }
            colorId++;
        }
    }
    if (canFill)
    {
        return i;
    }
    return -1;
}
CCPoint SGBattleMap::testFillTow(int listIndex)
{
    if (listIndex<0 || listIndex >= mapList) {
        return ccp(-1, -1);
    }
    int i = -1;
    for (int k = 0; k < mapList; k++) {
        i = mapRow - 1;
        for (; i >= 0; i--) {
            if (fillUnitGrids[i][listIndex] != NULL)
            {
                //最后一个兵
                break;
            }
        }
        if (i < mapRow - 2) {
            return ccp(i+1,listIndex);
        }
        listIndex++;
        listIndex = listIndex >= mapList ?(listIndex - mapList):listIndex;
    }
    
    return ccp(-1, -1);
}
CCPoint SGBattleMap::testFillFour(int listIndex)
{
    if (listIndex<0 || listIndex >= mapList) {
        return ccp(-1, -1);
    }
    
    int i = -1;
    for (int k = 0; k < mapList; k++) {
        i = mapRow - 1;
        for (; i >= 0; i--) {
            if (fillUnitGrids[i][listIndex] != NULL)
            {
                //最后一个兵
                break;
            }
        }
        if (i < mapRow - 2
            && listIndex < mapList -1
            && fillUnitGrids[i+1][listIndex+1] == NULL
            &&fillUnitGrids[i+2][listIndex+1] == NULL)
        {
            return ccp(i+1,listIndex);
        }
        listIndex++;
        listIndex = listIndex >= mapList ?(listIndex - mapList):listIndex;
    }
    
    return ccp(-1, -1);
}


void SGBattleMap::doclientFillOne(SGGridBase * gridBase)
{
    gridBase->retain();
    fillUnitGrids[gridBase->getIndex().i][gridBase->getIndex().j] = gridBase;
}

bool SGBattleMap::clientFillOne(SGGridBase * gridBase)
{
    if (fillUnitGrids[gridBase->getIndex().i][gridBase->getIndex().j] == NULL)
    {
        return true;
    }
    return false;
}

void SGBattleMap::clearFillUnitGrids()
{
    for (int i= 0; i<mapRow; i++) {
        for (int j = 0; j<mapList; j++) {
            if (fillUnitGrids[i][j])
            {
                fillUnitGrids[i][j]->release();
            }
            fillUnitGrids[i][j] = NULL;
        }
    }
}

void SGBattleMap::initFillUnitGrids()
{
    clearFillUnitGrids();
    for (int i= 0; i<mapRow; i++) {
        for (int j = 0; j<mapList; j++) {
            fillUnitGrids[i][j] = myGrids[i][j];
            if (fillUnitGrids[i][j]) {
                fillUnitGrids[i][j]->retain();
            }
        }
    }
}

int SGBattleMap::getCanFillHeroPow()
{
    int power = 0;
    ShiBing *sb = NULL;
    for (int i = 0; i < soldiers->count(); i++) {
        sb = (ShiBing*)soldiers->objectAtIndex(i);
        if (sb->getSbType() == ksbtwo)
        {
            power += 1;
        }
        else if (sb->getSbType() == ksbfour)
        {
            power += 2;
        }
    }
    power = power > 3 ? 3 : power;

    return (3 - power);//最多上3个2格武将
}

int SGBattleMap::getFillHeroColorId(int (* colorId)[3])
{
    cc_timeval psv;
    CCTime::gettimeofdayCocos2d(&psv, NULL);
    unsigned long int rand_seed = psv.tv_sec*1000 + psv.tv_usec/1000;
    srand(rand_seed);
    
    int setColorId = (int)(CCRANDOM_0_1()*(2));
    
    ShiBing *sb = NULL;

    for (int i = 0; i < soldiers->count(); i++)
    {
        sb = (ShiBing*)soldiers->objectAtIndex(i);
        if (sb->getSbType() == ksbtwo || sb->getSbType() == ksbfour)
        {
            if (sb->getSbColor() < 3) {
                (*colorId)[sb->getSbColor()] = -1;//有相同颜色
            }
        }
    }
    
    for (int index = 0; index < 3; index ++)
    {
        if ((*colorId)[index] != -1)
        {
            setColorId = (*colorId)[index];
            (*colorId)[index] = -1;
            break;
        }
    }
    
    return setColorId;
}

int SGBattleMap::getHeroCountWithoutFriend()
{
    int heroCount = 0;
    int friendSid = SGBattleManager::sharedBattle()->getFriendSid();
    ShiBing *sb = NULL;
    for (int i = 0; i < soldiers->count(); i++) {
        sb = (ShiBing*)soldiers->objectAtIndex(i);
        if (sb
            && sb->getSbType() != ksbone
            && sb->getSbId() != friendSid)
        {
            heroCount++;
        }
    }
    
    return heroCount;//最多上3个2格武将
}


