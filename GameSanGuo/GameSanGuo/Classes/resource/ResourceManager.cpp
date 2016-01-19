//
//  ResourceManager.cpp
//  GameSanGuo
//
//  Created by Loong on 13-10-31.
//
//

#include "ResourceManager.h"
#include <map>

//是否开启资源引用LOG
#define IS_DISPLAY_LOG_REF 1

USING_NS_CC;

using std::map;
using std::pair;

/////////////////////////////////////////////////////////---------ResourceManager--------////////////////////////////////////////////////////////
static ResourceManager* managerInstane=NULL;

ResourceManager* ResourceManager::sharedInstance(void)
{
    if(!managerInstane)
    {
        managerInstane=new ResourceManager();
    }
    return managerInstane;
}

ResourceManager::ResourceManager() : _resRefMap(NULL)
{
    _layerResListMap = new std::map<LayerTag, std::list<std::string>* >();
    _resRefMap = new map<std::string, ResTypeTagCount>();
    _fileStatMap = new std::map< FILE_TYPE, STR_INT_MAP* >();
    _toRemoveResList = new std::list<std::string>();
}

ResourceManager::~ResourceManager()
{
    CCLOG("~ResourceManager()");
    for (std::map<LayerTag, std::list<std::string>* >::iterator iter = _layerResListMap->begin(); iter != _layerResListMap->end(); ++iter)
    {
        iter->second->clear();
        CC_SAFE_DELETE(iter->second);
    }
    _layerResListMap->clear();
    CC_SAFE_DELETE(_layerResListMap);
    _resRefMap->clear();
    CC_SAFE_DELETE(_resRefMap);
    _fileStatMap->clear();
    CC_SAFE_DELETE(_fileStatMap);
}

//@重要：绑定纹理，并交由laTag所在的layer进行管理。//cgp
void ResourceManager::bindTexture(const char *pszName, RES_TYPE resType,LayerTag laTag, LIM_OPTION limOpt)
{
    
    //是否已存在该laTag的数据？
    std::list<std::string>* laResList = NULL;
    std::map<LayerTag, std::list<std::string>* >::iterator iter = _layerResListMap->find(laTag);
    if (iter != _layerResListMap->end()) //存在
    {
        laResList = iter->second;
    }
    else
    {
        laResList = new std::list<std::string>();
        _layerResListMap->insert( std::pair<LayerTag, std::list<std::string>*>(laTag, laResList) );
    }
    
    std::string name = pszName;
    
    laResList->push_back(name);
    
    //添加计数，若返回true，则添加纹理。
    if (this->addResRef(name, resType, laTag))
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(name.c_str());
        _tidyTexture(name);
    }
    else //HACK: 业务代码有问题，并非严谨的add和remove，只好每次都添加plist。
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(name.c_str());
        _tidyTexture(name);
    }
  
}

//释放指定纹理
void ResourceManager::unBindTexture(const char *pszName,RES_TYPE resType,LayerTag laTag)
{
    //是否已存在该laTag的数据？
    std::list<std::string>* laResList = NULL;
    std::map<LayerTag, std::list<std::string>* >::iterator iter = _layerResListMap->find(laTag);
    if (iter != _layerResListMap->end()) //存在
    {
        laResList = iter->second;
    }
    else //不存在不处理，为了兼容稳定性。
    {
        return;
    }

    //循环查找，理论上一定有。
    std::list<std::string>::iterator listIter = laResList->begin();
    
    for (; listIter != laResList->end(); )
    {
        if (strcmp((*listIter).c_str(), pszName) == 0)
        {
            //删除计数，若返回true，则删除纹理。
            if (this->reduceResRef(*listIter, resType, laTag))
            {
                _removeTexture(*listIter);
            }
            laResList->erase(listIter++);
            //FIXME: 是否这里break？如果不，则会将所有pszName的纹理减引用，会出问题么？
        }
        else
        {
            listIter++;
        }
    }
    
    //空了就移除，保持效果一致。
    if (laResList->empty())
    {
        _layerResListMap->erase(iter);
        CC_SAFE_DELETE(laResList);
    }
}

//@重要：释放纹理，释放laTag对应的Layer管理下的全部资源。
void ResourceManager::unBindLayerTexture(LayerTag laTag)
{
    //是否已存在该laTag的数据？
    std::list<std::string>* laResList = NULL;
    std::map<LayerTag, std::list<std::string>* >::iterator iter = _layerResListMap->find(laTag);
    if (iter != _layerResListMap->end()) //存在
    {
        laResList = iter->second;
        _layerResListMap->erase(iter);
    }
    else //不存在则不处理，为了兼容稳定性。
    {
        return;
    }
    
    //挨个释放
    std::list<std::string>::iterator listIter = laResList->begin();
    
    for (; listIter != laResList->end(); ++listIter)
    {
        //删除计数，若返回true，则删除纹理。
        if (this->reduceResRef(*listIter, RES_TYPE_LAYER_UI, laTag))
        {
            _removeTexture(*listIter);
        }
    }
    laResList->clear();
    CC_SAFE_DELETE(laResList);
}

//绑定初始资源，这是从APK里直接读取，这里的资源永远不要更新。
void ResourceManager::bindInitialImageTexture()
{
   
    CCSpriteFrameCache* sfc = CCSpriteFrameCache::sharedSpriteFrameCache();
    //公用资源。
    sfc->addSpriteFramesWithFile("public/public1.plist");
    sfc->addSpriteFramesWithFile("public/public2.plist");
    //LOADING小红帽。
    sfc->addSpriteFramesWithFile("animationFile/animation_loading.plist");

}

//指示下方绑定公共资源，只绑定一次。
static bool isCitLoaded = false;

//绑定公共UI纹理，这个资源不会释放。游戏运行过程中不需要释放的资源，这个资源在游戏创建的时候创建。永远也不释放。
void ResourceManager::bindCommonImageTexture()
{
    if (isCitLoaded)
    {
        return;
    }
    
    CCSpriteFrameCache* sfc = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    //可变的资源。
	sfc->addSpriteFramesWithFile("public/public3.plist");
    sfc->addSpriteFramesWithFile("public/public4.plist");
    
    sfc->addSpriteFramesWithFile("public/public_text.plist");
    
    //强化特效，历史原因。
	sfc->addSpriteFramesWithFile("animationFile/supersuccesssmall.plist");
	sfc->addSpriteFramesWithFile("animationFile/supersuccess.plist");
    
    //武将详情页大光效图片
    sfc->addSpriteFramesWithFile("sggeneralinfolayer/sggeneralinfolayer4.plist");
    
    isCitLoaded = true;
}

//移除纹理
void ResourceManager::_removeTexture(std::string& texName)
{
    _toRemoveResList->push_back(texName);

}

//整理纹理。主要是将需要加载的纹理从待删除的纹理中移除。
void ResourceManager::_tidyTexture(std::string& texName)
{
    for ( std::list<std::string>::iterator iter = _toRemoveResList->begin(); iter != _toRemoveResList->end(); )
    {
        if ( strcmp( (*iter).c_str(), texName.c_str() ) == 0 )
        {
            _toRemoveResList->erase(iter++);
            CCLOG("MM! HOLY! CATCH ONE TEDDY! ID = %s", texName.c_str());
        }
        else
        {
            iter++;
        }
    }
}

//展示当前全部得信息
void ResourceManager::showInfo()
{
    CCLOG("ResourceManager::showInfo() >>>>>>>>>>>>>>>>>>>>>>>> BEGIN");
    for (std::map<LayerTag, std::list<std::string>* >::iterator iter = _layerResListMap->begin(); iter != _layerResListMap->end(); ++iter)
    {
        CCLOG("[%s]",this->getResLayerNameByTag(iter->first));
        //iter->second = &std::list<std::string>;
        for (std::list<std::string>::iterator listIter = iter->second->begin(); listIter != iter->second->end(); ++listIter)
        {
            CCLOG("    |----%s", (*listIter).c_str());
        }
    }
    CCLOG("ResourceManager::showInfo() >>>>>>>>>>>>>>>>>>>>>>>> END");
}

//增加plist计数。返回true表示首次增加。
bool ResourceManager::addResRef(const std::string& key, RES_TYPE tp, LayerTag tag)
{
    map<std::string, ResTypeTagCount>::iterator iter;
    iter = _resRefMap->find(key);
    if (iter != _resRefMap->end()) //found
    {
        CCAssert(iter->second.count >= 1, "!!ResourceManager::addResRef(error) key=%s, the count must be >= 1");
        iter->second.count++;
#if IS_DISPLAY_LOG_REF
        CCLOG("[addResRef] (add) laTag=%s, key=%s, resCount=%d", this->getResLayerNameByTag(tag), key.c_str(), iter->second.count);
#endif
        return false;
        
    }
    else //not found
    {
        _resRefMap->insert(pair<std::string, ResTypeTagCount>(key, ResTypeTagCount(tp, tag, 1)));
#if IS_DISPLAY_LOG_REF
        CCLOG("[addResRef] (insert) laTag=%s,  key=%s, resCount=%d", this->getResLayerNameByTag(tag), key.c_str(), 1);
#endif
        return true;
        
    }
}

//减少plist计数。返回true表示可以删除。
bool ResourceManager::reduceResRef(const std::string& key, RES_TYPE tp, LayerTag tag)
{
    map<std::string, ResTypeTagCount>::iterator iter;
    iter = _resRefMap->find(key);
    if (iter != _resRefMap->end()) //found
    {
        iter->second.count--;
        if (0 == iter->second.count) //为0清空
        {
#if IS_DISPLAY_LOG_REF
            CCLOG("[reduceResRef] (destroy) laTag=%s, key=%s, resCount=%d", this->getResLayerNameByTag(tag), key.c_str(), iter->second.count);
#endif
            _resRefMap->erase(iter);
            return true;
        }
        else
        {
#if IS_DISPLAY_LOG_REF
            CCLOG("[reduceResRef] (reduce) laTag=%s, key=%s, resCount=%d", this->getResLayerNameByTag(tag), key.c_str(), iter->second.count);
#endif
            return false;
        }
    }
    else //not found
    {
#if IS_DISPLAY_LOG_REF
        CCLOG("[reduceResRef] (miss) laTag=%s, key=%s, resCount=NULL", this->getResLayerNameByTag(iter->second.tag), key.c_str());
#endif
        return false;
    }
}

//真·奥义·移除纹理。
void ResourceManager::bangkai()
{
    if (!_toRemoveResList->empty())
    {
        CCSpriteFrameCache* ccsfc = CCSpriteFrameCache::sharedSpriteFrameCache();
        for (std::list<std::string>::iterator listIter = _toRemoveResList->begin(); listIter != _toRemoveResList->end(); ++listIter)
        {
            ccsfc->removeSpriteFramesFromFile( (*listIter).c_str() );
        }
        _toRemoveResList->clear();
        CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    }
}

//显示引用计数器的概况。
void ResourceManager::showResRefInfo()
{
    CCLOG("ResourceManager::showResRefInfo() >>>>>>>>>>>>>>>>>>>>>>>> BEGIN");
    map<std::string, ResTypeTagCount>::iterator iter = _resRefMap->begin();
    for (; iter != _resRefMap->end(); iter++)
    {
        CCLOG("    |----latag = %s, name = %s, count = %d,", this->getResLayerNameByTag(iter->second.tag), iter->first.c_str(), iter->second.count);
    }
    CCLOG("ResourceManager::showResRefInfo() >>>>>>>>>>>>>>>>>>>>>>>> END");
}

//为统计MINI版所需文件，添加此文件统计。
void ResourceManager::addFileStat(const char* name, FILE_TYPE tp)
{
    //此函数已暂时完成其历史使命。
    return;
    std::map<FILE_TYPE, STR_INT_MAP*>::iterator iter = _fileStatMap->find(tp);
    STR_INT_MAP* map = NULL;
    if (iter != _fileStatMap->end())
    {
        map = iter->second;
    }
    else
    {
        map = new STR_INT_MAP();
        _fileStatMap->insert(std::pair<FILE_TYPE, STR_INT_MAP*>(tp, map));
    }
    map->insert(std::pair<std::string, int>(name, 1));
}

//显示文件统计。
void ResourceManager::showFileStatInfo()
{
    std::map<FILE_TYPE, STR_INT_MAP*>::iterator iter = _fileStatMap->begin();
    for (; iter != _fileStatMap->end(); iter++)
    {
        STR_INT_MAP::iterator siIter = iter->second->begin();
        for (; siIter != iter->second->end(); siIter++)
        {
            CCLOG("GreenHand: %s", siIter->first.c_str());
        }
        CCLOG("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
    }
}

//与LayerTag对应的LayerName
const char* LAYER_NAME[]=
{
    "sg_battleRevivelayer = 0",
    "sg_LayerNull",
    "sg_chatinputbox",
    "sg_playerInfoLayer",
    "sg_conloadingLayer",
    "sg_introLayer",
    "sg_countryLayer",
    "sg_cgMovieLayer",
    "sg_countryInfoLayer",
    "sg_countrysInfo",
    "sg_welComeLayer",//登陆
    "sg_registerLayer",//注册
    "sg_changepassword", //修改密码
    "sg_modifypassword",
    "sg_serverListLayer",//服务器列表
    "sg_firstLayer",
    "sg_mainLayer",
    "sg_loadLayer",
    "sg_battleMatchLayer",
    "sg_battleLayer",
    "sg_battleFieldLayer",//hkfn
    "sg_battleMainLayer", //天梯赛界面
    "sg_battlePreLayer",  //战前场景Loading
    
    "sg_batttleLoadingLayer",  //战斗loading  added by cgp
    "sg_sliceEffectLayer",    //切图层
    
    "sg_barrackslayer",
    "sg_showAllLayer",
    "sg_taskLayer",//当日任务
    "sg_liandanLayer",//炼丹炉界面
    "sg_jitianLayer",//祭天界面
    "sg_drinkWineLayer",//喝酒界面
    "sg_getsalaryLayer",//领工资界面
    "sg_inviterLayer",
    "sg_inviteeLayer",
    "sg_yuekaRewardLayer",
    "sg_regLotteryLayer",//拜财神
    "sg_bindLayer",//绑定帐号
    "sg_levelUpGetGold",//升级领元宝
    "sg_rewardsLayer",//领奖界面
    "sg_landingRewardLayer",//九宫格
    "sg_embattleLayer",//布阵
    "sg_generalsLayer",//武将列表界面
    "sg_generalInfoLayer",//武将详细信息界面
    "sg_generalInfoScrollLayer",//武将详情信息滑动界面
    "sg_equipsLayer",//装备列表界面
    "sg_soldiersLayer",//士兵列表界面
    "sg_skillCardsLayer",//被动技素材卡界面
    "sg_strengsoldier",
    "sg_chooseSkillCard",
    "sg_skillcardInfoLayer",
    "sg_equipInfoLayer",
    "sg_soldierInfoLayer",
    "sg_propInfoLayer",
    "sg_propsLayer",//道具列表界面
    "sg_stuffLayer",
    "sg_equipstrengLayer",
    "sg_strengLayer",
    "sg_loginLayer",
    "sg_listLoginLayer",
    "sg_advanceLayer",
    "sg_advancetexiao",
    "sg_shopLayer",
    "sg_vipGiftLayer",
    "sg_storeLayer",
    "sg_exchangCoin",
    "sg_storyLayer",      // 故事模式列表
    "sg_storySectionLayer", // 故事模式-场景详细列表
    "sg_storyHelpLayer",  // 寻求好友帮忙界面
    "sg_activityLayer",   // 每日活动界面
    "sg_activityDetail",  // 详细活动界面
    "sg_fhelpSortLayer",  // 寻求好友-排序界面
    "sg_pvpMainLayer",    // 讨伐战界面
    "sg_rankLayer",       // 排行榜界面
    "sg_creditCardLayer", // 积分兑换界面
    "sg_uiguideLayer",    // UI教学
    "sg_limitGuideLayer",    // 功能教学
    "sg_storyspeakLayer", // 故事章节对话
    "sg_blackLayer",
    "sg_expandLayer",
    "sg_arrangeLayer",
    "sg_soulCardLayer",
    "sg_helpLayer",
    "sg_helpBook",
    "sg_visitLayer",
    "sg_tongQueLayer",
    "sg_allCardsLayer",
    "sg_cardsLayer",
    "sg_vipInfoLayer",
    "sg_sortLayer",          //排序界面
    "sg_friendLayer",         //好友主界面
    "sg_myFriend",            //我的好友
    "sg_obtainTili",          //领取体力
    "sg_friendRequest",       //好友请求
    "sg_searchFriend",        //搜索好友
    "sg_expandFriend",        //扩充好友位置
    "sg_inviteFriend",        //邀请好友
    "sg_blackFriend",         //黑名单界面
    "sg_searchFriendFinish",  //搜索好友界面
    "sg_chatLayer",           //聊天界面
    "sg_setLayer",
    "sg_inviteNum",
    "sg_test",
    "sg_helpinfolayer",
    "sg_updateLayer",         //更新界面
    "sg_mailLayer",           //邮件界面
    "sg_friendMail",          //好友邮件
    "sg_systemMail",          //系统邮件
    "sg_GMMail",              //GM邮件
    "sg_systemMailContent",   //官方邮件内容
    "sg_friendMailContent",   //好友邮件内容
    "sg_visitShowCard",
    "sg_announcementLayer",   //公告界面
    "sg_battleSpeedLayer",    //战斗计算速度
    "sg_fightrewardLayer",    //战斗结算界面
    "sg_levelUpLayer",         //角色升级界面
    "sg_announcemsg",
    "sg_creamLayer",			//精英副本界面
    "sg_autoguideLayer",
    "sg_fightguideLayer",
    "sg_changeroundLayer",
    "sg_fillupLayer",
    "sg_messageLayer",
    "sg_totalLoginReward", //累计登陆界面
    "sg_herolayer", //战斗界面主体，新增
    "sg_shoujingzhou",        //守荆州
    "sg_sjzrank",             //守荆州排名
    "sg_soulExchangeLayer", //军魂换礼UI
    "sg_reward_subLayer",
    "sg_rewarPropBox",
    "sg_rewarCoinBox",
    "sg_otherPlayerInfo",
    "sg_giftInfoBox",
    "sg_glodChangeCoin",
    "sg_generalBox",
    "sg_mainScene",
    "sg_baseBox",
    "sg_firstAdvanceBox",
    "sg_advancechooselayer",
    "sg_otherPlayerInfoBox",
    "sg_piecesLayer",//武将碎片
    "sg_equipPiecesLayer",//装备碎片
    "sg_attackListLayer",
    "sg_battleHelpLayer",
    "sg_bindSuccessBox",
    "sg_bMatchAlertBox",
    "sg_bMatchEndBox",
    "sg_bMatchStartBox",
    "sg_cantVisitBox",
    "sg_yangXinBox",
    "sg_chatInputBox",
    "sg_coinRewardBox",
    "sg_creditCardBox",
    "sg_creditCardCell",
    "sg_embattleBox",
    "sg_emBattleBox",
    "sg_emBattleItem",
    "sg_exchangeEquipBox",
    "sg_fateInfoLayer",
    "sg_firstStrengBox",
    "sg_generalEquipBox",
    "sg_commonInfoBox",
    "sg_goldChangeCoin",
    "sg_nickNameBox",
    "sg_popWord",
    "sg_propRewardBox",
    "sg_registerBox",
    "sg_registerSuccess",
    "sg_rewardLdBox",
    "sg_shareGameBox",
    "sg_rewardSubLayer",
    "sg_sellBox",
    "sg_serverListNewServerBox",
    "sg_skillInfoBox",
    "sg_strengBox",
    "sg_ttMainLayer",
    "sg_battleLeaderSkillBox",
    "sg_overBattleBox",
    "sg_cgmovelayer",
    "sg_complain",///系统投诉 建议 客服留言主页面
    "sg_liuyan",//留言界面
    "sg_chatwithgm",
    "sg_chaatwithgminputbox",////追加发言输入框口
    "sg_exchangeCodeLayer",
    "sg_findHelplayer",
    "sg_jingzhouprocess",
    "sg_rank",  //综合排行榜
    "sg_batchselllayer",			//批量卖出页面
    "sg_junxianlayer",
    "sg_pvpeverydaylayer",
    "sg_consumablelayer",			//消耗品页面
    "sg_seasonlistlayer",
    "sg_pvplastlistlayer",
    "sg_mainTasklayer",
    "sg_comsumableInfoLayer", //消耗品详情页面
    "sg_luaactivity",  //活动页面
    "sg_bossbattledetailLayer",//boos战详情
    "sg_bossbattleranklayer",//boss战的排行榜
    "sg_goldshoptodysppricelayer",//元宝商城今日特价页面
    "sg_goldshoplimittimelayer",//限制出售
    "sg_goldshopnormallayer",//商城普通
    "sg_buygoodsbox",
    "sg_bossBattleJoinInLayer",
    "sg_bossBattleLayer",
    "sg_bossBattleNjoinInlayer",
    "sg_piecesinfolayer",
    "sg_bossRewardLayer",
    "sg_alltasklayer",
    "sg_strengEffLayer",//强化效果页面
    "sg_mallLayer",//test用。by：zyc  新版商店。
    "sg_lastLayer", //因商城外部多处有直接跳转至具体商店接口，返回按钮需要返回原来的layer。添加lastLayer用以记录，从哪里进入商城，以便返回原处。
    "sg_lootWikiMainLayer ", //图鉴来源
    "sg_lootWiKiLayer",
    "sg_cardItem",//为了使用ResourceManager
    "sg_mailGiftPopBox", //新增独立boxTag
    "sg_showHeadIconLayer", //新增独立layer
    //为统一资源管理，boxTag统一纳入LayerTag
    "sg_box_null",
    "sg_box_alertTag",
    "sg_box_registerTag",
    "sg_box_loginTag",
    "sg_box_strengTag",//强化武将弹框
    "sg_box_advanceTag",//强化武将弹框
    "sg_box_sortTag",
    "sg_box_canVisitTag",
    "sg_box_canAdvanceTag",
    "sg_box_exchangeEquipTag",
    "sg_box_firstAdvanceTag",
    "sg_box_noExpStrengTag",
    "sg_box_rewardLdTag",
    "sg_box_sellBoxTag",
    "sg_box_skillInfo",
    "sg_box_yangxingTag",
    "sg_box_modification",
    "sg_box_changeAccount",
    "sg_box_sellTag",
    "sg_box_equip",
    "sg_box_otherPlayerInfo",
    "sg_box_btrewardTag",
    "sg_box_matchStart",
    "sg_box_matchAlert",
    "sg_box_matchEnd",
    "sg_box_quickfight",
    "sg_box_overbattle",
    "sg_box_help",
    "sg_box_battleGuide",
    "sg_box_serverList",
    "sg_box_uplevel",
    "sg_box_general",
    "sg_box_input",
    "sg_box_backFight",
    "sg_box_fillUp",
    "sg_box_rewordCoins",
    "sg_box_shareGame",
    "sg_box_nikeNameBox",
    "sg_box_connTip",
    "sg_box_confirm",
    "sg_box_batchSell",//批量卖出
    "sg_box_consumableSell",// 消耗品卖出
    "sg_box_rank",
    "sg_box_sellconfirm",//卖出消耗品二次确认
    "sg_box_detial",//礼包详情
    "sg_box_buygoods",//元宝商城购买商品
    "sg_box_shade",
    "sg_box_shade2",
    "sg_box_cargoDetail", //zyc ..商品详情
    "sg_box_lootWikiMain",     //zyc ..图鉴来源
    "sg_box_autoEquip",
    "sg_box_battleCallFriend",
    "sg_box_battleHelp",
    "sg_box_battleRoundPrompt",
    "sg_box_changeNickName",
    "sg_box_changeOfficer",
    "sg_box_consumableSellBox",
    "sg_box_consumableSellConfirmBox",
    "sg_box_creditCostBox",
    "sg_box_embattleBox",
    "sg_box_exchangeEquipBox",
    "sg_box_expandBox",
    "sg_box_forbidChat",
    "sg_box_friendCountFull",
    "sg_box_generalinfoEffect",
    "sg_box_mallGiftPopBox",
    "sg_box_noExpStrengBox",
    "sg_box_qFightBox",
    "sg_box_rankDetailLayer",
    "sg_box_soldierinfoEffect",
    "sg_box_soldierListBox",
    "sg_box_sortBox",
    "sg_box_strengBox",
    "sg_box_strengLvlUpBox",
    "sg_box_tipsBox",
    "sg_box_storyRewardBox",
    "sg_box_actGiftInfoBox",
    "sg_box_giftInfoBox",
    "sg_box_pvpGiftInfoBox",
    "sg_box_equipBox",
    "sg_box_strengEff",
    "sg_box_announcementLayer",
    "sg_box_battleLeaderSkillBox",
    "sg_box_battleReviveLayer",
    "sg_box_battleRoundPromptLayer",
    
    "sg_box_battleRoundSurrenderLayer",  //added by cgp
    "sg_box_bindBox",
    "sg_box_bindSuccessBox",
    "sg_box_bossTips",
    "sg_box_countryInfoLayer",
    "sg_box_creamBattleBox",
    "sg_box_firstStrengBox",
    "sg_box_generalEquip",
    "sg_box_soldierList",
    "sg_box_strengEffectLayer",
    "sg_box_updateClientBox",
    //MM: 针对新手引导新增的layerTag
    "sg_fourGuideLayer",
    "sg_fightDescLayer",
    "sg_pvpFightRewardLayer",
    "sg_generalsBagLayer", //背包-》武将入口
    "sg_piecesBagLayer",
    "sg_materialLayer",//材料
    "sg_equipAdvancedLayer",//装备转生
    "sg_advancedEffectLayer",
    "sg_monthlySignLayer", //月签奖励界面。
    "sg_areanLayer",//竞技场界面
    "sg_vsLayer",//点击对战进入的那个界面
    "sg_developingOfficerLayer",//武将培养
    "sg_specialBattleLayer",//特殊副本战斗layer
    "sg_specialBattleSecionLayer", //特殊副本进入节列表
    "sg_arenaFightRewardLayer",
    "sg_fightOverNoticeBox" , //除pvp外，战斗结算页面新弹出框提示 2014-12-25
    "sg_adventureLayer", //奇遇界面
    "sg_exoticMerchantLayer", //西域商人界面
    "cgp_stringAdvLayer",    //强化进阶界面
    "SG_LAYER_TAG_TOTAL_NUM" //@NOTICE! 所有格新增LayerTag请在这个枚举值之前填写！
};

//通过layerTag获得资源所属的Layer的名称，方便鉴别。
const char* ResourceManager::getResLayerNameByTag(LayerTag tag)
{
    if (tag >= sg_battleRevivelayer && tag <= SG_LAYER_TAG_TOTAL_NUM)
    {
        return LAYER_NAME[tag];
    }
    else
    {
        CCAssert(false, "Not Valid LayerTag Oops!");
    }
}
