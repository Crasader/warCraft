//
//  SGPlayerInfo.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-5.
//
//

#include "SGPlayerInfo.h"
#include "SGStoryBean.h"
#include "SGStorySceneBean.h"
#include "SGStaticDataManager.h"
#include "SGTeamgroup.h"
#include "SGGuideManager.h"
#include <algorithm>

static SGPlayerInfo *s_sharePlayerInfo = NULL;
SGPlayerInfo::SGPlayerInfo()
: _piecesCards(NULL)//碎片卡牌
, _piecesCommon(NULL)//通用碎片
, _consumCards(NULL)//消耗品道具
, m_index(-10)
, m_chatTime(60)
, searchfriendcode(NULL)
, invitationCode(NULL)
, playerRoleId(0)
, playerName(NULL)
, nickName(NULL)
, friendSize(100)
, expandCount(0)
, playerSsid(NULL)
, playerLevel(0)
, playerCoins(0)
//, playerSilver(0)
, playerliandan(0)
, playerGold(0)
, playerGovering(0)
, AddPlayerGovering(0)
, playerCountry(0)
, playerImageID(0)
, playerPower(0)
, playerBagSize(0)
, playerModifyCount(0)
, playerExp(0)
, m_freevisit(0)
//, speakerCount(0)
//, award(0)
, pvpPower(0)
, pveLeftTime(0)
, pveCircleTime(0)
, pvpLeftTime(0)
, pvpCircleTime(0)
, pvpRank(NULL)
, _storyData(NULL)
, _activityData(NULL)
, m_isCan(0)
, m_loginday(0)
, m_chance(0)
, _plotId(0)
, m_Victory(0)
, _pvpCredit(0)
, _guideId(0)
, _isFirstFour(0)
, m_viplvl(0)
, m_pvplvl(0)
, m_combatValue(0)
, m_pvpnum(0)
, m_HasPveFight(false)
, m_MilitaryRank(0)
, m_MilitaryRankName(NULL)
, m_battleType(0)
, m_pvpWinPvp(0)
, m_winPvp(0)
, m_canRewardDayTask(0)
, m_cdkeyneed(0)
, loginAwardGold(0)
, isshowvip(0)
, isCgBattle(0)
, ispvp(0)
, fubenLevelLimit(0)
, shouJinZhouLimit(0)
, netbattleLimit(0)
, superFubenLimit(0)
, pvPRewardRate(0)
, skipFightCount(0)
, wait(0)
, m_idid(0)
, m_ismaxlevel(0)
, _officerCards(NULL)
, _soldierCards(NULL)
, _equipCards(NULL)
, _propCards(NULL)
, _serverList(NULL)
, _lotteryItems(NULL)
, _rewardNotice(NULL)
, _rewardOtherPlayer(NULL)
, _arrayFriend(NULL)
, _arrayBlack(NULL)
, _arrayApply(NULL)
, _friendPowerList(NULL)
, _arrayFriendInfoCache(NULL)
, _arrayFriendMails(NULL)
, _arraySystemMails(NULL)
, _arrayGMMails(NULL)
, _arrayAnnoun(NULL)
, _arrayQAcontent(NULL)
, m_oldtime(0)
, m_withgmoldtime(0)
, m_haveCharged(false)
, m_bestEquip(false)
, m_isNeedVeriFy(false)
, maxPlotId(0)
, _sjzData(NULL)
, mainTaskDict(NULL)
, taskGroupArray(NULL)
, ranktype(0)
, nation(0)
, playerid(0)
, starnum(0)
, isshowed(false)
, powerGiftTimes(0)
, isBossBattleTime(false)
, _junHun(0)
,_materialCards(NULL)
,_maxBossPlotId(0)
,m_saodangType(0)
,m_isActiviNotAccess(false)
,playerJewels(0)
{
    m_limitGuideTags.clear();
    
    _officerCards = CCArray::create();
    _officerCards->retain();
    
    _soldierCards = CCArray::create();
    _soldierCards->retain();
    
    _equipCards = CCArray::create();
    _equipCards->retain();
    
    _propCards = CCArray::create();
    _propCards->retain();
	
	_piecesCards = CCArray::create();
	_piecesCards->retain();//碎片卡牌
	
	_piecesCommon = CCArray::create();
	_piecesCommon->retain();//通用碎片
	
	_consumCards = CCArray::create();
	_consumCards->retain();//消耗品道具
    
    _materialCards = CCArray::create();
    _materialCards->retain();//材料卡
    
    _arrayFriend = CCArray::create();
    _arrayFriend->retain();
    
    _arrayBlack = CCArray::create();
    _arrayBlack->retain();
    _arrayApply = CCArray::create();
    _arrayApply->retain();
    
    _arrayFriendInfoCache = CCArray::create();
    _arrayFriendInfoCache->retain();
    
    _lotteryItems = CCArray::create();
    _lotteryItems->retain();
    
    _arrayFriendMails = CCArray::create();
    _arrayFriendMails->retain();
    
    _arraySystemMails = CCArray::create();
    _arraySystemMails->retain();
    
    _arrayGMMails = CCArray::create();
    _arrayGMMails->retain();
    ///////kanata
    _arrayQAcontent=CCArray::create();
    _arrayQAcontent->retain();
    
    //////
    
    _rewardNotice = CCArray::create();
    _rewardNotice->retain();
    
    _rewardOtherPlayer = CCArray::create();
    _rewardOtherPlayer->retain();
    
    _storyData = NULL;
    _activityData = CCArray::create(); //bymm，起手初始化，解决使用该数据时候的NULL问题。
    _activityData->retain();
    
    
    _arrayAnnoun = CCArray::create();
    _arrayAnnoun->retain();
    
    msgstyleDic = CCDictionary::create();
    msgstyleDic->retain();
    
    //守荆州数据
    _sjzData = SGSjzData::create();
    _sjzData->retain();

    mainTaskDict=CCDictionary::create();
    mainTaskDict->retain();
    
   
   _friendPowerList = CCArray::create();
   _friendPowerList->retain();
    
    this->analyseUnlockLev();
    
    //背包中经验丹数量多少时添加武将强化红点提示  数据从controlvalue中读取---拥有多少金钱且武将身上装备未达到上限，添加装备强化红点提示
    CCDictionary * dict = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    
    CCDictionary * dict1 = (CCDictionary *)dict->objectForKey("65");
    setShowPlayerNoticeLimitLev(dict1->valueForKey("value")->intValue());
    
    dict1 = (CCDictionary*)dict->objectForKey("66");
    setShowNoticeLimitLev2(dict1->valueForKey("value")->intValue());
    
    dict1 = (CCDictionary*)dict->objectForKey("67");
    setOfficerStrengLevBigger(dict1->valueForKey("value")->intValue());
    
    dict1 = (CCDictionary*)dict->objectForKey("68");
    setExpCardNumCanShowNotice(dict1->valueForKey("value")->intValue());
    
    dict1 = (CCDictionary*)dict->objectForKey("69");
    setEquipStrengLevBigger(dict1->valueForKey("value")->intValue());
    
    dict1 = (CCDictionary*)dict->objectForKey("70");
    setCoinNumCanShowNotice(dict1->valueForKey("value")->intValue());
    
    dict1 = (CCDictionary*)dict->objectForKey("85");
    setDevPelletLimitCnt(dict1->valueForKey("value")->intValue());
    
    dict1 = (CCDictionary*)dict->objectForKey("86");
    setDevPointLimitCnt(dict1->valueForKey("value")->intValue());
    
    dict1 = (CCDictionary*)dict->objectForKey("82");
    setDevOfficerLimitLev(dict1->valueForKey("value")->intValue());
    
    dict1 = (CCDictionary*)dict->objectForKey("81");
    setDevOfficerLimitPlayerLev(dict1->valueForKey("value")->intValue());
}


SGPlayerInfo::~SGPlayerInfo()
{
    CC_SAFE_RELEASE(_soldierCards);
    CC_SAFE_RELEASE(_officerCards);
    CC_SAFE_RELEASE(_equipCards);
    CC_SAFE_RELEASE(_propCards);
	CC_SAFE_RELEASE(_piecesCommon);//通用
	CC_SAFE_RELEASE(_piecesCards);//碎片
	CC_SAFE_RELEASE(_consumCards);//消耗品
	CC_SAFE_RELEASE(_materialCards);//材料
    
    
    CC_SAFE_RELEASE(_arrayFriend);
    CC_SAFE_RELEASE(_arrayBlack);
    CC_SAFE_RELEASE(_arrayApply);
    CC_SAFE_RELEASE(nickName);
    CC_SAFE_RELEASE(playerName);
    CC_SAFE_RELEASE(playerSsid);
    CC_SAFE_RELEASE(_arrayFriendInfoCache);
    CC_SAFE_RELEASE(_lotteryItems);

    CC_SAFE_RELEASE(_rewardNotice);

    CC_SAFE_RELEASE(_arrayFriendMails);
    CC_SAFE_RELEASE(_arrayGMMails);
    //////
    CC_SAFE_RELEASE(_arrayQAcontent);
    
    //////
    CC_SAFE_RELEASE(_arraySystemMails);
    CC_SAFE_RELEASE(_arrayAnnoun);
    CC_SAFE_RELEASE(msgstyleDic);
    CC_SAFE_RELEASE(m_MilitaryRankName);
    CC_SAFE_RELEASE(mainTaskDict);
    CC_SAFE_RELEASE(taskGroupArray);
    CC_SAFE_RELEASE(_friendPowerList);
    
}
SGPlayerInfo* SGPlayerInfo::sharePlayerInfo(void)
{
    if (!s_sharePlayerInfo)
    {
        s_sharePlayerInfo = new SGPlayerInfo();
    }
    return s_sharePlayerInfo;
}
void SGPlayerInfo::addOfficerCard(SGOfficerCard *card)
{
    if (_isFirstFour==0) //&& card->getCurrStar()>4
    {
        SGOfficerDataModel * cardData = SGStaticDataManager::shareStatic()->getOfficerById(card->getItemId());
        if (cardData->getArea()== 4) {
             _isFirstFour = 1;
        }
    }
        
    _officerCards->addObject(card);
    
}
void SGPlayerInfo::addSoldierCard(SGSoldierCard *card)
{
    _soldierCards->addObject(card);
}
void SGPlayerInfo::addEquipCard(SGEquipCard *card)
{
    _equipCards->addObject(card);
}
void SGPlayerInfo::addPropCard(SGPropsCard *card)
{
    _propCards->addObject(card);
}
void SGPlayerInfo::addPiecesCard(SGPiecesCard *card)
{
	for (int i = 0; i < _piecesCards->count(); i++)
	{
		//如果当前的碎片已经拥有，就把当前删除，再填入当前碎片
		if (card->getItemId() == static_cast<SGPiecesCard *>(_piecesCards->objectAtIndex(i))->getItemId())
		{
//			_piecesCards->removeObjectAtIndex(i);
            static_cast<SGPiecesCard *>(_piecesCards->objectAtIndex(i))->setPiecesCurNum(card->getPiecesCurNum());
            return ;
		}
	}
	_piecesCards->addObject(card);
}
void SGPlayerInfo::addPiecesCommonCard(SGPiecesCard *card)
{
	_piecesCommon->addObject(card);
}
//获取消耗品道具
void SGPlayerInfo::addConsumableCard(SGConsumableCard *card)
{
	//可叠加,不允许重复的存在，如果达到峰值，重新加一个
	for (int i = 0; i < _consumCards->count(); i++)
	{
		if (card->getItemId() == static_cast<SGConsumableCard *>(_consumCards->objectAtIndex(i))->getItemId())
		{
			if (static_cast<SGConsumableCard *>(_consumCards->objectAtIndex(i))->getCurrNum() < 999)
			{
				_consumCards->removeObjectAtIndex(i);
			}
			
		}
	}
	_consumCards->addObject(card);
}

//加材料，逻辑同消耗品，都是可堆叠
void SGPlayerInfo::addMaterialCard(SGMaterialCard *card)
{
    //可叠加,不允许重复的存在，如果达到峰值，重新加一个
	for (int i = 0; i < _materialCards->count(); i++)
	{
		if (card->getItemId() == static_cast<SGMaterialCard *>(_materialCards->objectAtIndex(i))->getItemId())
		{
			if (static_cast<SGMaterialCard *>(_materialCards->objectAtIndex(i))->getMaterialCurrNum() < 999)
			{
				_materialCards->removeObjectAtIndex(i);
			}
			
		}
	}
	_materialCards->addObject(card);
}


CCArray* SGPlayerInfo::getOfficerCards()
{
//   先不排序，有问题再加上
    
//    int len = _officerCards->count();
//    for(int i=0;i<len-1;i++)
//    {
//        for(int m=i+1;m<len;m++)
//        {
//            
//            SGOfficerCard *cardSoldier = (SGOfficerCard*)_officerCards->objectAtIndex(i);
//            SGOfficerCard *card = (SGOfficerCard*)_officerCards->objectAtIndex(m);
//            if(cardSoldier->getPositionId() < card->getPositionId())
//            {
//                _officerCards->exchangeObjectAtIndex(i, m);
//            }
//        }
//    }

    return _officerCards;
}

CCArray *SGPlayerInfo::getShowOfficerCards(bool isshow)
{
    CCArray *showArray = CCArray::create();
    if (_officerCards)
    {
        SGTeamgroup *teamGroup = SGTeamgroup::shareTeamgroupInfo();
        CCObject *obj =NULL;
        CCARRAY_FOREACH(_officerCards, obj)
        {
            SGOfficerCard *card = (SGOfficerCard *)obj;

            if ( teamGroup->isOnEmbattle(card) == isshow)
            {
                showArray->addObject(card);
            }
        }
    }
    return showArray;
}
SGOfficerCard *SGPlayerInfo::getOfficerCard(int sid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_officerCards, obj)
    {
        SGOfficerCard *card = (SGOfficerCard *)obj;
        if (card  && card->getSsid() == sid)
        {
            return card;
        }
    }
    return NULL;
}
SGSoldierCard *SGPlayerInfo::getSoldierCard(int sid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_soldierCards, obj)
    {
        SGSoldierCard *card = (SGSoldierCard *)obj;
        if (card  && card->getSsid() == sid)
        {
            return card;
        }
    }
    
    return NULL;
}
SGEquipCard *SGPlayerInfo::getEquipCard(int sid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_equipCards, obj)
    {
        SGEquipCard *card = (SGEquipCard *)obj;
        if (card  && card->getSsid() == sid)
        {
            return card;
        }
    }
    return NULL;
}

SGPropsCard *SGPlayerInfo::getPropCardB(int ssid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_propCards, obj)
    {
        SGPropsCard *card = (SGPropsCard *)obj;
        if (card  && card->getSsid() == ssid)
        {
            return card;
        }
    }
    return NULL;
}

SGPiecesCard *SGPlayerInfo::getPiecesCardById(int ssid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_piecesCards, obj)
    {
        SGPiecesCard *card = (SGPiecesCard *)obj;
        if (card  && card->getSsid() == ssid)
        {
            return card;
        }
    }
	CCARRAY_FOREACH(_piecesCommon, obj)
    {
        SGPiecesCard *card = (SGPiecesCard *)obj;
        if (card  && card->getSsid() == ssid)
        {
            return card;
        }
    }
    return NULL;
}
//根据ssid获取对应的消耗品
SGConsumableCard *SGPlayerInfo::getConsumableCardById(int ssid)
{
	CCObject *obj = NULL;
	CCARRAY_FOREACH(_consumCards, obj)
	{
		SGConsumableCard *card = (SGConsumableCard *)obj;
		if (card && card->getSsid() == ssid)
		{
			return card;
		}
	}
	return NULL;
}
//根据id获取对应的材料卡牌
SGMaterialCard *SGPlayerInfo::getMaterialCardById(int ssid)
{
    CCObject *obj = NULL;
	CCARRAY_FOREACH(_materialCards, obj)
	{
		SGMaterialCard *card = (SGMaterialCard *)obj;
		if (card && card->getSsid() == ssid)
		{
			return card;
		}
	}
	return NULL;
}
//根据itemId获取材料数量
SGMaterialCard *SGPlayerInfo::getMaterialCardByItemId(int itemId)
{
    CCObject *obj = NULL;
	CCARRAY_FOREACH(_materialCards, obj)
	{
		SGMaterialCard *card = (SGMaterialCard *)obj;
		if (card && card->getItemId() == itemId)
		{
			return card;
		}
	}
	return NULL;
}

void SGPlayerInfo::removeOfficerCardById(int ssid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_officerCards, obj)
    {
        SGPropsCard *card = (SGPropsCard *)obj;
        if (card  && card->getSsid() == ssid)
        {
            _officerCards->removeObject(card);
        }
    }
}
void SGPlayerInfo::removeSoldierCardById(int ssid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_soldierCards, obj)
    {
        SGSoldierCard *card = (SGSoldierCard *)obj;
        if (card  && card->getSsid() == ssid)
        {
            _soldierCards->removeObject(card);
        }
    }
}
void SGPlayerInfo::removeEquipCardById(int ssid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_equipCards, obj)
    {
        SGEquipCard *card = (SGEquipCard *)obj;
        if (card  && card->getSsid() == ssid)
        {
            _equipCards->removeObject(card);
        }
    }
}
void SGPlayerInfo::removePropCardById(int ssid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_propCards, obj)
    {
        SGPropsCard *card = (SGPropsCard *)obj;
        if (card  && card->getSsid() == ssid)
        {
            _propCards->removeObject(card);
        }
    }
}

//移除碎片卡牌
void SGPlayerInfo::removePiecesCardById(int ssid)
{
    CCObject *obj = NULL;
    CCARRAY_FOREACH(_piecesCards, obj)
    {
        SGPiecesCard *card = (SGPiecesCard *)obj;
        if (card  && card->getSsid() == ssid)
        {
            _piecesCards->removeObject(card);
        }
    }
	
	CCARRAY_FOREACH(_piecesCommon, obj)
    {
        SGPiecesCard *card = (SGPiecesCard *)obj;
        if (card  && card->getSsid() == ssid)
        {
            _piecesCommon->removeObject(card);
        }
    }
}

//移除消耗品
void SGPlayerInfo::removeConsumableCardById(int ssid)
{
	CCObject *obj = NULL;
	CCARRAY_FOREACH(_consumCards, obj)
	{
		SGConsumableCard *card = (SGConsumableCard *)obj;
		if (card && card->getSsid() == ssid)
		{
			_consumCards->removeObject(card);
		}
			
	}
}
//移除对应的材料卡牌
void SGPlayerInfo::removeMaterialCardById(int ssid)
{
    CCObject *obj = NULL;
	CCARRAY_FOREACH(_materialCards, obj)
	{
		SGMaterialCard *card = (SGMaterialCard *)obj;
		if (card && card->getSsid() == ssid)
		{
			_materialCards->removeObject(card);
		}
        
	}
}

CCArray * SGPlayerInfo::getSoldierCards()
{
    int len = _soldierCards->count();
    for(int i=0;i<len-1;i++)
    {
        for(int m=i+1;m<len;m++)
        {
            SGSoldierCard *cardSoldier = (SGSoldierCard*)_soldierCards->objectAtIndex(i);
            SGSoldierCard *card = (SGSoldierCard*)_soldierCards->objectAtIndex(m);
            if(cardSoldier->getItemId() > card->getItemId())
            {
                _soldierCards->exchangeObjectAtIndex(i, m);
            }
        }
    }

    return _soldierCards;
}
CCArray *SGPlayerInfo::getEquipCards()
{
    int len = _equipCards->count();
    for(int i=0;i<len-1;i++)
    {
        for(int m=i+1;m<len;m++)
        {
            
            SGEquipCard *cardSoldier = (SGEquipCard*)_equipCards->objectAtIndex(i);
            SGEquipCard *card = (SGEquipCard*)_equipCards->objectAtIndex(m);
            if(cardSoldier->getOfficerCardId() < card->getOfficerCardId())
            {
                _equipCards->exchangeObjectAtIndex(i, m);
            }
        }
    }

    return _equipCards;
}

CCArray *SGPlayerInfo::getPropCards()
{
    if (!_propCards) {
        _propCards = CCArray::create();
        _propCards->retain();
    }
    return _propCards;
}

//获取角色碎片信息
CCArray *SGPlayerInfo::getPiecesCards()
{
    if (!_piecesCards) {
        _piecesCards = CCArray::create();
        _piecesCards->retain();
    }
    return _piecesCards;
}
//获取通用碎片信息
CCArray *SGPlayerInfo::getPiecesCommonCards()
{
    if (!_piecesCommon) {
        _piecesCommon = CCArray::create();
        _piecesCommon->retain();
    }
    return _piecesCommon;
}
//获取消耗品数组
CCArray *SGPlayerInfo::getConsumableCards()
{
	if (!_consumCards)
	{
		_consumCards = CCArray::create();
		_consumCards->retain();
	}
	return _consumCards;
}
//获取材料
CCArray *SGPlayerInfo::getMaterialCards()
{
    if (!_materialCards)
    {
        _materialCards = CCArray::create();
        _materialCards->retain();
    }
    return _materialCards;
}


void SGPlayerInfo::addFriend(SGGamePlayer *player)
{
    _arrayFriend->addObject(player);
}

void SGPlayerInfo::addFriendPower(SGGiftPower *player)
{
    _friendPowerList->addObject(player);
}

void SGPlayerInfo::addBlack(SGGamePlayer *player)
{
    _arrayBlack->addObject(player);
}
///////kanata
void SGPlayerInfo::addQAContent(SGQAConcent *content)
{
    _arrayQAcontent->addObject(content);
}

//////
void SGPlayerInfo::addApply(SGGamePlayer *player)
{
    _arrayApply->addObject(player);
}

CCArray* SGPlayerInfo::getFriendList()
{
    return _arrayFriend;
}

CCArray* SGPlayerInfo::getBlackList()
{
    return _arrayBlack;
}

CCArray* SGPlayerInfo::getApplyList()
{
    return _arrayApply;
}
void SGPlayerInfo::addServerList(cocos2d::CCArray *array)
{
//    if(NULL != _serverList)
//    {
//        _serverList->release();
//    }
//    
//    _serverList = array;
//    _serverList->retain();
}

CCArray* SGPlayerInfo::getServerList()
{
    return _serverList;
}

SGGamePlayerInfo* SGPlayerInfo::getGamePlayerInfoById(int roleId)
{
    int friendCacheCount = _arrayFriendInfoCache->count();
    for(int i=0;i<friendCacheCount;i++)
    {
        SGGamePlayerInfo *playerInfo = (SGGamePlayerInfo*)_arrayFriendInfoCache->objectAtIndex(i);
        if(playerInfo->getRoleId() == roleId)
        {
            return playerInfo;
        }
    }
    
    return NULL;
}

CCArray* SGPlayerInfo::getEmbattleOfficers()
{
    CCArray *arrayEmbattle = CCArray::create();
    
    int officerCardCount = _officerCards->count();
    SGTeamgroup *teamGroup = SGTeamgroup::shareTeamgroupInfo();
    for(int i=0;i<officerCardCount;i++)
    {
        SGOfficerCard *card = (SGOfficerCard*)_officerCards->objectAtIndex(i);
        if(teamGroup->isOnEmbattle(card))
        {
            //布阵了，出征了
            arrayEmbattle->addObject(card);
        }
    }
//    上阵武将先不排序了
    
//    int len = arrayEmbattle->count();
//    for(int i=0;i<len-1;i++)
//    {
//        for(int m=i+1;m<len;m++)
//        {
//            SGOfficerCard *cardSoldier = (SGOfficerCard*)arrayEmbattle->objectAtIndex(i);
//            SGOfficerCard *card = (SGOfficerCard*)arrayEmbattle->objectAtIndex(m);
//            if(cardSoldier->getPositionId() > card->getPositionId())
//            {
//                arrayEmbattle->exchangeObjectAtIndex(i, m);
//            }
//        }
//    }    
    return arrayEmbattle;
}
void SGPlayerInfo::add9Item(SGLottery9ViewItem *item)
{
    _lotteryItems->addObject(item);
}
CCArray *SGPlayerInfo::get9Items()
{
    return _lotteryItems;
}

void SGPlayerInfo::updateStoryData(int plotId, int rank)
{
    if (rank == 0 || plotId == 0)return;

    if (plotId >= 20000) {
        _plotId = plotId;//根据服务器数据更新关卡数 zenghui 2014.1.27
        if (plotId == MaxSceneId || plotId == MaxCreamId ) {//精英副本
            
            for (int ii = 0; ii < _storyData->count(); ii++) {
                SGStoryBean *story = (SGStoryBean *)_storyData->objectAtIndex(ii);
                if (story->getStoryId() == MaxStoryId) {
                    SGStorySceneBean *scene = story->getSceneBeanById(plotId);
                    if (scene != NULL) scene->setRank(rank);
                    
                    // 设置旧故事的数据
                    story->setIsWin(1); // set old to win
                    // set old story's rank
                    int minRank = 5;
                    SGStorySceneBean *oscene = NULL;
                    for (int jj = 0; jj < story->getStorySections()->count(); jj++) {
                        oscene = (SGStorySceneBean *)story->getStorySections()->objectAtIndex(jj);
                        int rank = oscene->getRank();
                        if (rank < minRank) {
                            minRank = rank;
                            break;
                        }
                    }
                    story->setRank(minRank);
                }
            }
            //return;
        }
        
        return;
    }

    if (_storyData) {
        SGStoryBean *sBean = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(plotId+1);
        bool isNewStory = true;
        //CCLOG("sBeanId:%d",sBean->getStoryId());
        _plotId = plotId;//根据服务器数据更新关卡数 zenghui 2014.1.27
        setMaxPlotId(plotId, true);
        for (int ii = 0; ii < _storyData->count(); ii++) {
            SGStoryBean *story = (SGStoryBean *)_storyData->objectAtIndex(ii);
            if (sBean && story->getStoryId() == sBean->getStoryId()) {
                CCLOG("story:%d",story->getStoryId());
                isNewStory = false;
                break;
            }
        }
        int scount = _storyData->count();
        for (int ii = 0; ii < scount; ii++) {
            SGStoryBean *story = (SGStoryBean *)_storyData->objectAtIndex(ii);
            SGStorySceneBean *scene = story->getSceneBeanById(plotId);
            if (scene != NULL) {
                story->setVisible(true);
                scene->setVisible(true);
                scene->setRank(rank);
                
                // open new scene
                story->setSceneVisibleById(plotId + 1, true);
                
                int scCount = story->getStorySections()->count();
                // if open new story
                if (isNewStory || plotId == 16) {
                    // 设置旧故事的数据
                    story->setIsWin(1); // set old to win
                    // set old story's rank
                    int minRank = 5;
                    SGStorySceneBean *oscene = NULL;
                    for (int jj = 0; jj < scCount; jj++) {
                        oscene = (SGStorySceneBean *)story->getStorySections()->objectAtIndex(jj);
                        int rank = oscene->getRank();
                        if (rank < minRank) {
                            minRank = rank;
                            break;
                        }
                    }
                    story->setRank(minRank);
                    
                    // 开启新故事
                    if (isNewStory == true) {
                        if (sBean)
                        {
                            sBean->setVisible(true); // set new to open
                            sBean->setIsWin(0);
                            _storyData->insertObject(sBean, 0);
                        }
                    }
                    
//                    if (plotId == LAST4PLOTID) { // 开启非进阶引导的新故事
//                        SGStoryBean *oldBean = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(plotId+1);
//                        int pcount = plotId + oldBean->getStorySections()->count() + 1;
//                        SGStoryBean *newBean = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(pcount);
//                        newBean->setVisible(true);
//                        newBean->setIsWin(0);
//                        _storyData->insertObject(newBean, 0);
//                    }
                }else if (isNewStory==false) { // 更新旧数据
                    int minRank = 5;
                    SGStorySceneBean *oscene = NULL;
                    for (int jj = 0; jj < scCount; jj++) {
                        oscene = (SGStorySceneBean *)story->getStorySections()->objectAtIndex(jj);
                        int rank = oscene->getRank();
                        if (rank < minRank) {
                            minRank = rank;
                            break;
                        }
                    }
                    story->setRank(minRank);
                }
                break;
            }
        }
    }
}

bool SGPlayerInfo::isLastSceneOfStory(int plotId)
{
    SGStoryBean *story = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(plotId);
    if (story != NULL) {
        int maxPlotId = 0;
        for (int ii = 0; ii < story->getStorySections()->count(); ii++) {
            SGStorySceneBean *scene = (SGStorySceneBean *)story->getStorySections()->objectAtIndex(ii);
            if (maxPlotId < scene->getPlotId()) {
                maxPlotId = scene->getPlotId();
            }
        }
        if (plotId < maxPlotId) {
            return false;
        }else {
            return true;
        }
    }
    return false;
}

int SGPlayerInfo::getCurrentStoryId()
{
    int currStoryId = 0;
    SGStoryBean *story = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(_plotId);
    if (story != NULL) {
        currStoryId = story->getStoryId();
    }
    return currStoryId;
}

int SGPlayerInfo::getCurrentSceneId()
{
    int currSceneId = 0;
    SGStoryBean *story = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(_plotId);
    if (story != NULL) {
        SGStorySceneBean *scene = story->getSceneBeanById(_plotId);
        if (scene != NULL) {
            currSceneId = scene->getSceneId();
        }
    }
    return currSceneId;
}

void SGPlayerInfo::addrewardNotice(CCObject* object)
{
    _rewardNotice->addObject(object);
}
CCArray *SGPlayerInfo::getrewardNotice()
{
    return _rewardNotice;
}

#pragma mark - 邮件系统

CCArray *SGPlayerInfo::getFriendMails()
{
    return _arrayFriendMails;
}

CCArray *SGPlayerInfo::getSystemMails()
{
    return _arraySystemMails;
}

CCArray *SGPlayerInfo::getGMMails()
{
    return _arrayGMMails;
}
//////kanata
CCArray *SGPlayerInfo::getQAContent()
{
    return  _arrayQAcontent;

}

CCArray *SetQAContent(CCArray*arr)
{
  // return  _arrayQAcontent;
}

////////
CCArray *SGPlayerInfo::getrewardRegPlayers()
{
    return _rewardOtherPlayer;
}

void SGPlayerInfo::setRewardRegPlayers(CCArray *bscArr){
    _rewardOtherPlayer=bscArr;
}
#pragma mark - ------ 更改数值 铜钱 金子 炼丹秘方 小喇叭 -----

//void SGPlayerInfo::changeAwardCount(int count)
//{
//    award += count;
//}

void SGPlayerInfo::changeCoinsCount(int count)
{
    playerCoins += count;
}

void SGPlayerInfo::changeGoldCount(int count)
{
    playerGold += count;
    if (playerGold <= 0)
        playerGold = 0;
}

void SGPlayerInfo::changeLiandanCount(int count)
{
    playerliandan += count;
}

//void SGPlayerInfo::changeSpeakerCount(int count)
//{
//    speakerCount += count;
//}

int SGPlayerInfo::getTotalCardsCount()
{
    return _officerCards->count() + _equipCards->count() +_soldierCards->count() +_propCards->count() + _consumCards->count();
}
CCArray* SGPlayerInfo::getAnnouns()
{
    return _arrayAnnoun;
}
CCDictionary *SGPlayerInfo::getmsgstyleDic()
{
    return msgstyleDic;
}
int SGPlayerInfo::checkStartLimitGuide(int checkType,int checkPlotId)
{
    int bossChap = CCUserDefault::sharedUserDefault()->getIntegerForKey("bossChap");
    
    if (m_limitGuideTags.size() > 0)
    {
        int size = m_limitGuideTags.size();
        
        for (int i =0; i<size; i++)
        {
            if (m_limitGuideTags[i].triggerType == checkType)
            {
                //类型，对应的trigger表中type,分别检测等级，关卡
                if (checkType == 1)
                {
                    //如果当前 已经在引导中 或者已经有要马上激活的引导。就不再激活与等级相关的引导 ,
                    if (SGGuideManager::shareManager()->isGuide
                        || SGGuideManager::shareManager()->getLimitGuideSize() > 0)
                    {
                        //小于0的值表示没有可用激活的引导
                        return -1;
                    }
                    
                    if (playerLevel == m_limitGuideTags[i].triggerValue )
                    {
                        m_limitGuideTags[i].triggerValue = -1;
                        return m_limitGuideTags[i].triggerId;
                    }
                }
                //
                else if (checkType == 2)
                {
                    if (checkPlotId == maxPlotId + 1 || checkPlotId == bossChap)//精英副本，或者普通闯关
                    {
                        if (checkPlotId == m_limitGuideTags[i].triggerValue)
                        {
                            m_limitGuideTags[i].triggerValue = -1;
                            return m_limitGuideTags[i].triggerId;
                        }
                    }
                    
                }
                else if (checkType == 3)
                {
                    if (checkPlotId == m_limitGuideTags[i].triggerValue)
                    {
                        m_limitGuideTags[i].triggerValue = -1;
                        return m_limitGuideTags[i].triggerId;
                    }
                }
                else if (checkType == 4)
                {
                    //for testing
//                    if (checkPlotId == 4)
//                    {
//                        return 16;
//                    }
                    if (checkPlotId == m_limitGuideTags[i].triggerValue)
                    {
                        m_limitGuideTags[i].triggerValue = -1;
                        return m_limitGuideTags[i].triggerId;
                    }
                }
                else if (checkType == 5)
                {
                    if (checkPlotId == m_limitGuideTags[i].triggerValue)
                    {
                        m_limitGuideTags[i].triggerValue = -1;
                        return m_limitGuideTags[i].triggerId;
                    }
                }
            }
        }
        
    }
    return -1;
}

int SGPlayerInfo::getMaxPlotId()
{
    return maxPlotId;
}
void SGPlayerInfo::setMaxPlotId(int maxId,bool foceSet)
{
    //精英与副本不设置maxplotid
    if (maxId >= 10000)
    {
        return;
    }
        //必须保证最新的关卡最多只可能比我大1，如果大1一定是最新关卡，不能进行设置！
    int newId = maxId - 1;
    if ( (foceSet && maxPlotId <= newId) || maxPlotId < newId ) {
        //这里略坑，如果他减去1的值比我大的话，则把我设置成他的原值，但这是建立在每次最新的关卡只比我大1的基础上。假设以后一次下发多个未打过的新关卡，则会变成其倒数第二个发送的关卡为最新，虽然实际上我并没有打过。
        maxPlotId = maxId;
    }
}

//精英关卡相关。
void SGPlayerInfo::setMaxBossPlotId(int maxId)
{
    
    int newId = maxId - 1;
    
    if (_maxBossPlotId < newId)
    {
        this->_maxBossPlotId = maxId;
    }
}

int SGPlayerInfo::getMaxBossPlotId()
{
    return this->_maxBossPlotId;
}

bool SGPlayerInfo::checkIsSkipDayPlaying()
{
    CCString* playerDay = CCString::createWithFormat("playerDay%d",playerRoleId);
    int playingDate = CCUserDefault::sharedUserDefault()->getIntegerForKey(playerDay->getCString());
    int currentDate = 0;
    
    
    struct tm *tm;
    time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    time(&timep);
#else
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    timep = now.tv_sec;
#endif
    
    tm = localtime(&timep);
    //int year = tm->tm_year + 1900;
    //int month = tm->tm_mon + 1;
    currentDate = tm->tm_mday;
    //int hour=tm->tm_hour;
    //int min=tm->tm_min;
    //int second=tm->tm_sec;
    
    
    if (playingDate == currentDate)
    {
        return false;
    }
    else
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey(playerDay->getCString(),currentDate);
        return true;
    }
    
}

void SGPlayerInfo::resetSendFriendTili()
{
    CCObject *obj;
    SGGamePlayer* friendPlayer;
    CCARRAY_FOREACH(_arrayFriend, obj)
    {
        friendPlayer = (SGGamePlayer*)obj;
        friendPlayer->setIsGiftPower(0);
    }
}

bool SGPlayerInfo::canBreakLimitById(LimitId triggerId)
{
    if (m_limitGuideTags.size() > 0) {
        int size = m_limitGuideTags.size();
        for (int i =0; i<size; i++) {
            if (m_limitGuideTags[i].triggerId == (int)triggerId) {
                if (playerLevel < m_limitGuideTags[i].triggerValue ) {
                    return false;
                }
            }
        }
    }
    return true;
}
int SGPlayerInfo::getLimitLevelById(LimitId triggerId)
{
    if (m_limitGuideTags.size() > 0) {
        int size = m_limitGuideTags.size();
        for (int i =0; i<size; i++) {
            if (m_limitGuideTags[i].triggerId == (int)triggerId) {
                    return m_limitGuideTags[i].triggerValue;
            }
        }
    }
    return 0;
}

CCArray * SGPlayerInfo::getSubTaskByGroupName(const char *taskName)
{
    //CCLog("%s",taskName);
    CCArray *subTaskArray=(CCArray *)mainTaskDict->objectForKey(taskName);
    if (subTaskArray) {
        return subTaskArray;
    }
    return NULL;
}

void SGPlayerInfo::addSubTask(SGMainTaskDataModel *dataModel){
    const char *groupName=getGroupNamebyId(dataModel->gettaskGroupId());
    if (groupName ==  NULL) {
        return;
    }
    
    CCArray *subTaskArray=(CCArray *)mainTaskDict->objectForKey(groupName);
    if (subTaskArray==NULL) {
        subTaskArray=CCArray::create();
        subTaskArray->retain();
        mainTaskDict->setObject(subTaskArray, groupName);
    }

    CCObject *obj  =NULL;
    CCARRAY_FOREACH(subTaskArray, obj)
    {
        SGMainTaskDataModel *p_data=(SGMainTaskDataModel *)obj;
        
        if (dataModel->getcomplateState()==3 && p_data->gettaskId()==dataModel->gettaskId()) {
            subTaskArray->removeObject(p_data);
            return;
        }
        if (dataModel->gettaskId()==p_data->gettaskId()) {
            subTaskArray->removeObject(p_data);
//            subTaskArray->addObject(dataModel);
            subTaskArray->insertObject(dataModel, 0);
            return;
        }
    }
    if (dataModel->getcomplateState()==2) {
         subTaskArray->insertObject(dataModel, 0);
    }
    else
        subTaskArray->addObject(dataModel);
}
const char * SGPlayerInfo::getGroupNamebyId(int groupId){
//    CCArray *mainGroup=  SGStaticDataManager::shareStatic()->getMainGroupTask();
//    mainGroup->retain();
    CCArray *mainGroup = this->getTaskGroup();
    
    CCObject *obj=NULL;
   
    CCARRAY_FOREACH(mainGroup,obj){
    
        SGMainTaskGroup *group=(SGMainTaskGroup *)obj;
        if (group->gettaskGroupId()==groupId) {
           return  group->gettaskGroupName()->getCString();
        }
    }
    return NULL;
}

void SGPlayerInfo::removeSubTask(SGMainTaskDataModel *dataModel){
   const char *groupName=getGroupNamebyId(dataModel->gettaskGroupId());
    CCArray *subTaskArray=(CCArray *)mainTaskDict->objectForKey(groupName);
    if (subTaskArray)
        subTaskArray->removeObject(dataModel);
}
CCArray *SGPlayerInfo::getTaskGroup(){

    if (!taskGroupArray) {
        taskGroupArray=SGStaticDataManager::shareStatic()->getMainGroupTask();
        taskGroupArray->retain();
    }
    return taskGroupArray;
}
CCDictionary *SGPlayerInfo::getMainTaskDict(){

    return mainTaskDict;
}


void SGPlayerInfo::setCreamData(int plotId, int canJoinCount , int isCanJoin)
{
    std::map<int , std::vector<int > >::iterator iter = m_creamPlotIdNum.find(plotId);
    if(iter == m_creamPlotIdNum.end())
    {
        std::vector<int> temp;
        temp.push_back(canJoinCount);
        temp.push_back(isCanJoin);
        m_creamPlotIdNum.insert(std::pair<int , std::vector<int> >(plotId ,temp ));
    }
    //更新值
    else
    {
        (iter->second)[0] = canJoinCount;
        (iter->second)[1] = isCanJoin;
    }
}

int SGPlayerInfo::getCreamCanJoinCount(int plotId)
{
    std::map<int , std::vector<int > >::iterator iter = m_creamPlotIdNum.find(plotId);
    if(iter != m_creamPlotIdNum.end())
        return (iter->second)[0];
    else
//        assert(false);
        return 0;
}


bool SGPlayerInfo::getCreamIsCanJoin(int plotId)
{
   std::map<int , std::vector<int > >::iterator iter = m_creamPlotIdNum.find(plotId);
    if(iter != m_creamPlotIdNum.end())
        return (iter->second)[1] == 1;
    else
//        assert(false);
        return false;
}

bool SGPlayerInfo::isGuideStory(int type, int storyId)
{
    int size = m_limitGuideTags.size();
    
    for (int i =0; i<size; i++)
    {
        if (m_limitGuideTags[i].triggerType == type && m_limitGuideTags[i].triggerValue == storyId)
        {
            //类型，对应的trigger表中type,分别检测等级
            return true;
        }
    }
    return false;
}

bool SGPlayerInfo::isGuid()
{
    return CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId") < guide_tag_53;
}

bool SGPlayerInfo::canSoldierStreng()
{
    bool soldierLevelFlag = false;
    int soulNum = 0;
    CCArray *soulCards = SGPlayerInfo::sharePlayerInfo()->getPropCards();
    CCObject *obj_ = NULL;
    CCARRAY_FOREACH(soulCards, obj_)
    {
        
        SGPropsCard *card = (SGPropsCard *)obj_;
        if (card && card->getType() == 3 )
        {
            soulNum++;
        }
    }
    
    if(soulNum <15)
        return false;
    
    int teamid = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
    int soldierId = 0;
    CCArray *allSoldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();
    CCObject *obj1 = NULL;
    int playerLevel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    
    
    for (int i = 0; i<3; i++)
    {
        switch (i)
        {
            case 0:
            {
                soldierId = SGTeamgroup::shareTeamgroupInfo()->getsoldierblue(teamid);
            }
                break;
            case 1:
            {
                soldierId = SGTeamgroup::shareTeamgroupInfo()->getsoldierred(teamid);
            }
                break;
            case 2:
            {
                soldierId = SGTeamgroup::shareTeamgroupInfo()->getsoldiergreen(teamid);
            }
                
                break;
            default:
            {
                soldierId = 0;
            }
                break;
        }
        
        CCARRAY_FOREACH(allSoldiers, obj1)
        {
            SGSoldierCard *card = (SGSoldierCard *)obj1;
            if (card->getItemId() == soldierId)
            {
                if (card->getCurrLevel() < playerLevel)
                {
                    soldierLevelFlag = true;
                    break;
                }
            }
        }
        
        if (soldierLevelFlag)
        {
            break;
        }
        
    }

    return  soldierLevelFlag;
}


bool SGPlayerInfo::alreadyHasTheOfficer(int officerProtoId)
{
    bool ret = false;
    CCArray * array = getOfficerCards();
    for(int i=0 ; i <array->count() ; ++i)
    {
        SGOfficerCard * temp = (SGOfficerCard*)array->objectAtIndex(i);
        if(officerProtoId == temp->getProtoId())
        {
            ret = true;
            break;
        }
    }
    
    return ret;
}

void SGPlayerInfo::analyseUnlockLev()
{
    SGStaticDataManager * manager = SGStaticDataManager::shareStatic();
    CCDictionary * dict = manager->getDisplayValueDict();
    for(int i=54 ; i<=63 ; ++i)
    {
        CCDictionary * temp = (CCDictionary *)dict->objectForKey(CCString::createWithFormat("%d" , i)->m_sString);
        m_unlockAtMinLevVec.push_back(temp->valueForKey("value")->intValue());
    }
}

int SGPlayerInfo::getTipAtLevel(int index)
{
    return m_unlockAtMinLevVec.at(index-1);
}

int SGPlayerInfo::getGoodsNumInBag()
{
    return getOfficerCards()->count() + getEquipCards()->count() + getSoldierCards()->count() + getPropCards()->count();
}

int SGPlayerInfo::ownItemCount(SGBaseItemType itemType, int itemId)
{
    int count = 0;
    CCArray * arr = NULL;
    switch(itemType)
    {
            //todo:在此处补充完整。
            
            
            
            
            
            
            
            
            
            
            
        case BIT_CONSUMABLE:
            arr = SGPlayerInfo::sharePlayerInfo()->getConsumableCards();
            for(int i=0 ; i<arr->count() ; ++i)
            {
                SGConsumableCard * temp = dynamic_cast<SGConsumableCard *>(arr->objectAtIndex(i));
                if(temp->getItemId() == itemId)
                {
                    count = temp->getCurrNum();
                    break;
                }
            }
            break;
        default:
            break;

    }
    
    
    
    
    
    return count;
}










