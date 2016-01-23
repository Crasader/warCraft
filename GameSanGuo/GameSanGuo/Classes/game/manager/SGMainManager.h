//
//  SGMainManager.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-20.
//
//
#define SG_SHOW_WINDOW(_xxxx_) SGMainManager::shareMain()->showMessage(_xxxx_)


#ifndef __GameSanGuo__SGMainManager__
#define __GameSanGuo__SGMainManager__

#include "cocos2d.h"
#include "SGMainScene.h"
#include "SGBaseLayer.h"
#include "SGBaseBox.h"
#include "SGOfficerCard.h"
#include "SGSoldierCard.h"
#include "SGEquipCard.h"
#include "SGPropsCard.h"
#include "SGStoryBean.h"
#include "SGMainLayer.h"
#include "SGVisitLayer.h"
#include "SGTeamgroup.h"
#include "SGTotalLoginRewardLayer.h"
#include "ResourceManager.h"
#include "SGLayout.h"
#include "SGEmbattleBox.h"
#include "SGExchangeCodeLayer.h"
#include "SGBaseStuffLayer.h"
#include "SGActivityData.h"
#include "SGCommonInfoBox.h"
#include "SGBossBattleLayer.h"
#include "SGBossBattleJoinInLayer.h"
#include "SGBossBattleDetailLayer.h"
#include "PlatformAdp.h"
#include "SGSpecialBattleLayer.h"
#include "SGAreanLayer.h"
#include "SGMallLayer.h"
//由globalconfig中配置，留档。

//#define USEFACEBOOK 1 //andriod使用
//#define XDBUYPLAYCHANNEL    101
//#define GO2PLAYCHANNEL    102
//#define IOSBUYCHANNEL   GO2PLAYCHANNEL //当台湾正式发布时候走go2play

#define CHATSHOWENTRYTAG   1111

#define VISIT_ENTER     111

USING_NS_CC;

class SGMainManager : public CCObject
{
public:  //全局
    CC_SYNTHESIZE(bool , isAdv,  IsAdv)     //0:streng, 1:adv
    
    
private:
    CCString *serverIp;
    short serverPost;
    SGMainScene *mainScene;
    
    //设置本页面之前的Pre页面的tag，以前用于记录进入聊天之前的页面，现在也用于SGActivityLayer与物品详情之间的回溯，大雾。
    CC_SYNTHESIZE(int, m_chatShowPreLayer, ChatShowPreLayer);
private:
    SGMainManager();
    //id _mSdkReqCallBack;
public:
    
    //全局通用提示消息条
    void showCommonMessage(CCObject *);
    
    // 开始的几关路过援军
    void skipFindHelp(CCArray *helpers, int plotId, bool storyModel);
    
	// sdk 购买成功后向服务器发送消息
	//购买成功后向游戏服务器发送消息
	void buySuccessAndSendMsg(const char *order, int channelId = 1, int isApp = 0);
    
    // android 调用这个方法像服务器发送充值成功消息。bId为订单ID，bNum为充值金额。
    void chargeSuccessForAndroid(const char* bId, int bNum);
	
    static SGMainManager* shareMain(void);
    
    void  initNotification();
    
    void setServerInfo(CCString *ip, short post);
    CCString *getServerIp();
    short getServerPost();
    void startGameCGPCallfunc();
    void startGame();
    SGBaseLayer *getNowShowLayer();
    void showLayer(SGBaseLayer *layer, bool isBeforeDel = true);
    
	//新手玩家游戏进度
	void sendNewPlayerProcess(GUIDE_STEP step, int ext = 0);
	
    void showLoadLayer(bool isshow);
    //显示主界面背景
    void showMainLayer();
    void creatMainLayerForGuide();
    void updataUserMsg();
    //显示首页
    void showFirstLayer();

    //显示 军营
    void showBarrackslayer(bool isBeforeDel = true);
    //显示武将列表类
    void showGeneralsLayer(int type,bool isBeforeDel = true,int x=0,int currentPage=1000,int currentRow=0);
    //显示背包武将列表类
    void showGeneralsBagLayer(int type,bool isBeforeDel = true,int x=0,int currentPage=1000,int currentRow=0);

    //显示士兵列表类
    void showSoldierslayer(bool isBeforeDel = true,int x=0);
    //显示道具列表
    void showPropsLayer(bool isBeforeDel = true,int currentPage=1000);
    //显示道具信息
    void showPropInfoLayer(SGPropsCard* card,int type,bool isBeforeDel = true,int currentPage=1000);
    //显示被动技素材卡列表
    void showSkillCardsLayer(int type,bool isBeforeDel = true );
    //显示武将计素材卡信息
    void showSkillCardInfoLayer(SGBaseMilitaryCard* card,int type,bool isBeforeDel = true,bool state = false);
    //军衔buff界面
    void showJunxianLayer(bool isBeforeDel=true);
    //显示武将信息
    void removeGeneralInfoLayerForNew();
    void showGeneralInfoLayer(SGOfficerCard* card,int type,ERI *info = NULL, bool isBeforeDel = true,SGOfficerCard *officer = NULL,int isfromfirst = 0,CCDictionary *dic = NULL,int currentPage=1000,int currentRow=0);
    //显示士兵信息
    void showSoldierInfoLayer(SGSoldierCard* card,int type, bool isBeforeDel = true,SGSoldierCard *before = NULL, bool isfirst = false,CCDictionary *dic = NULL);
    //显示装备列表类
    void showEquipsLayer(SGOfficerCard *card,int equiptype,int enter,bool isBeforeDel = true,int x=0 ,int currentPage=1000,int currentRow=0);
    //显示装备信息
    void showEquipInfoLayer(SGEquipCard* card,int type,bool isBeforeDel = true,SGEquipCard *before = NULL,bool isfirst = false,CCDictionary *dic = NULL,int currentPage=1000,int currentRow=1);
    //显示战斗匹配界面
    void showBattleMatchLayer();
    //显示 战斗主界面
    void showBattleLayer(SGBaseLayer *battleLayer,bool isPveContinue = false);
	
    void showCgBattleLayer(SGBaseLayer *battleLayer);
	//显示卡牌碎片界面
	void showPiecesLayer(bool isBeforeDel = true);

	//显示道具界面
	void showConsumeLayer(bool isBeforeDel = true);
	
    //显示材料界面
    void showMaterialLayer(bool isBeforeDel = true);
    
    //显示特殊副本（幻境）
    void showSpecialBattleLayer(CCArray *data);
    
    
    //显示 弹框
    void showBox(SGBaseBox *bb,bool isAction = true);
    void showAlertBox(SGBoxDelegate *delegate, const char *msg);
    //关闭弹框
    void closeBox();    
    //显示 布阵
    void showEmbattleLayer(int type,int emtype= SGTeamgroup::shareTeamgroupInfo()->getiscurrent(),bool isBeforeDel = true);
    //显示 扩建军营
    void showExpandLayer(bool isBeforeDel = true);
    //显示 布阵展开
    void showAllLayer(bool isBeforeDel = true);   
    //显示 故事模式界面
    void showStoryMainLayer(CCArray *array = NULL, bool isBeforeDel = true);
	
	//显示精英副本界面//10.28//george
	void showCreamBattleLayer(CCArray *array = NULL, bool isBeforeDel = true);
	
    //显示 场景列表界面
    void showStorySectionLayer(SGStoryBean* storyBean, int index = 0);
    //显示 积分兑换界面
    void showChgCreditCardLayer(CCArray *array, int index,int type =0);
    //显示 寻求好友帮助节目
    void showFindHelpLayer(int plotId, CCArray* helpers, bool storyModel = true);
    //显示 活动列表界面
    void showActivityLayer(CCArray *array = NULL, bool isBeforeDel = true);
    //显示PVP主界面
    void showPvpMainLayer();
    //显示排行信息
    void showRankLayer(CCArray *data, int num,int index);
    //显示九宫格界面
    void showRewardLayer(SGBoxDelegate *dg);
    //显示 冰魂卡信息
    void showSoulCardInfoLayer(SGPropsCard *card,int enterType = 0,bool delBefore = true);
    //显示强化界面
	
    void showStrengLayer(SGBaseMilitaryCard *card, int type, int enter = 0,bool isOfficerOnly = false, int currentPage=1000, ERI *info = NULL);
    
    
    //显示强化进阶界面, added by cgp
    void showStrengAdvLayer(SGBaseMilitaryCard *card, int type, int enter = 0,bool isOfficerOnly = false, int currentPage=1000, ERI *info = NULL);
    
    
    //显示进阶界面
    void showAdvanceLayer(SGBaseMilitaryCard *card, int type,int isfirst = 0,CCArray *array = NULL, ERI *info = NULL);
    
    //显示装备进阶的layer
    void showEquipAdvancedLayer(SGEquipCard *card, bool isBeforeDel = true, int enterType = 0);
    
    void showAdvancechooseLayer(SGBaseMilitaryCard *card, int type,bool enter,bool isOfficerOnly,int currentPage,CCArray *array);
    void showblack();
    void hideblack();
    //添加额外的层，一般用于一次性的全屏UI，但是又不想影响MainScene的nowLayerTag。tips：游戏里所有的box的zorder为90，guide的zorder为101。
    void addShowLayer(SGBaseLayer *layer , bool isBeforeDel = true);
    //显示商城
    void showShopLayer();
    //显示充值界面
    void showStoreLayer(int curr,int next,int currrmb,int maxrmb,CCArray *ary,int type,CCArray *str,bool isBeforeDel = true);
    //显示兑换界面
    void showExchangLayer(int lvl,int curr,int max,int gold,int coin);
    //显示VIP信息界面
//    void showVipInfoLayer(int viplvl,int type = 0);
//    提示
    void showConnectTip(SGNetworkErrorType tp);
    //显示帮助
    void showHelpLayer(bool isBeforeDel = true);
    //显示玩家信息
    void showPlayerInfo();
    //更新布阵
    void updateEmbattleInfo();
    //显示图鉴
    void showAllCards(CCArray *dict);
    //显示图鉴列表
    void showCardsInfo(CCArray *array,int type,bool isBeforeDel = true);
    //显示设置
    void showSetLayer();
    //显示帮助手册
    void showHelpBook();
    //填写邀请码
    void showInviteNumLayer();
    //显示礼包界面
    void showGiftsLayer(int vipLvl,CCArray *_array,int curr,int max);
    //显示探访界面
    void showVisitLayer(int visitHuo,int a,int b,int c,int d,int j,std::vector<VisitActivity>& visitActivityList ,const char*lotteryName  = NULL, const char * activityLotteryName = NULL);
    //显示抽奖结果界面
    void showVisitCards(CCArray *array,CCArray *huoArray,SGBaseMilitaryCard *comboCard,SGBaseMilitaryCard *progressCard,bool useGoldDoor,bool isBeforeDel = true, SGPropsCard *buyPropCard = NULL);
    //显示聊天界面
    void showChatLayer(bool isBeforeDel = true);
    //显示国家特性
    void showCountriesLayer(int type=0,bool isBeforeDel=true,int country=-10);
    //显示领奖界面
    void showRewardsLayer(bool willReload = false);
    //显示喝酒回体力界面
    void showDrinkWineLayer(int power,int iscan);
    //显示升级领金子
    void showLvlUpGetGold(CCArray *array);
    //显示领俸禄
    void showSalaryLayer(int a,int b,int c,int d,int e,int f,int g);
    //显示祭天
    void showJitianLayer(int current, int max,int cando,CCArray*array);
    //显示炼丹炉界面
    void showLiandanLayer(int time,int index);
    //显示拜财神
    void showRegLayer(int chance,int time,int cost,CCArray *bcsArr);
    //显示绑定页面
    void showBindLayer(CCArray *array,int type,CCArray *array2,int type2,const char * mail);
    //显示当日任务
    void showTaskLayer(CCArray *array);
    void showAllTaskLayer(CCArray *array);//kanata
    //显示进阶特效
//    void showAdvanceTexiao(int itemid , SGBaseMilitaryCard *card_ ,int type,bool isfirst =false);
    void showAdvanceTexiao(int itemid , SGBaseMilitaryCard *card_ ,int type,int star,bool isfirst =false);
    //显示选择强化被动技界面
    void showChooseSkillLayer();
    //显示登陆界面
    void showloginLayer();
    //显示登陆首页
    void showwelcomelayer(bool conn = false);
    //显示注册帐号
    void showregisterLayer(int type,bool beforeDel = true);
    //显示修改密码
    void showChangePassWord();
    //显示投诉建议主页面
    void showcomplainlayer();
    //显示留言界面
    void showliuyanlayer();
    //赛季奖励
    void showSeasonListLayer(int count,CCArray *ary,bool isBeforeDel = true);
    //上届榜单
    void showlastListLayer(int count,CCArray *ary,bool isBeforeDel = true);
    
    void showTongQueLayer(int spendMedal,int spendGold,int medal,int spendCoins,int refreshTime,int blessValue, std::vector<SGItemInfo> &tongqueItem, SGItemInfo &yangxing);
	
	//显示强化效果页面
	//void showStrengEffectLayer(SGBaseMilitaryCard *card, int enterType, int successLvl);
    
    float getBottomHeight();
    float getHeadHeight();
    float getTotleHdHgt();
    float getVisitHgt();
    float fivebuttonhight();
    //显示消息
    void showMessage(const char *messageStr);
    //显示装备动画
    void showEffect(SGEquipCard *card,int type);
    void showTotalRewardLayer(main::LoginAwardListResponse &response,SGBoxDelegate *dg);//显示积累登陆奖励
    
    //获取mainscene
    SGMainScene *getMainScene();
    
    //获取m_pUIGroup
    TouchGroup* getTouchGroup();
    
    //获取mainLayer
    SGMainLayer* getMainLayer();
    
    void showTestLayer(CCLayer *layer);
    
    void showPveFinish();
	
	//精英副本章节
	void showCreamSectionLayer(SGStoryBean* storyBean);
    //显示竞技场界面
    void showAreanLayer(CCArray * array, SGAreanBaseInfoData * baseData,SGAreanItemInfoData * itemData,AreanLayerType type);
    //显示对战界面
    void showVsLayer();
    //发送到竞技场的消息
    void goToArenaLayerRequest();
    
    //显示特殊副章详情
    void showSpecialBattleSectionInfoLayer(CCArray *sArray, SpecialBattleData *chapInfo);
    
    
private:
    //本次守荆州是否去军魂商店。
    bool _shallSjzGoToSoulMall;
    
public:
    //发送获取守荆州详情消息。如果为true，则会直接去军魂商店，如果为false，则去守荆州首页。大部分情况，不需要去商店。
    void sendGetSjzInfoRequest(bool shallGoToSoulMall = false);
    //MM: 守荆州详情消息统一处理。
    void sjzInfoResponse(CCObject *obj);
    //show出守荆州界面。
    void showshoujingzhou();
    //守荆州排名
	void showsjzrank(CCArray *data, int pageItemsNum, int pageMax, int nowRank, int coins, int reward, int time);
    void showExchangeCodeLayer(CCString *desprition);//显示兑换码
     
    //按序号添加headIcon、offcer、equip、prop资源
    void addVSHeadPlistByNum(int numId,LayerTag layerTag);
    void addHeadIconPlistByNum(int numId,LayerTag layerTag);
    void addOfficerPng(int numId,LayerTag layerTag);
    void addEquipPng(int numId,LayerTag layerTag);
    void addPropPng(int numId,LayerTag layerTag);
    void addSkillPlistByNum(int numId,LayerTag layerTag);
    //加载特殊战斗节列表中的底图
    void addSpHeadBgPlistByNum(int numId, LayerTag lTag);
    //加载章节列表中上部banner的底畋
    void addSpTopHeadBgPlistByNum(int numId, LayerTag lTag);
    
    
    void showInviterLayer( CCString* inviteCode,int inviteNum,int inviteMax,CCArray *array);
    void showInviteeLayer();
    void showYueKaRewardLayer(CCArray *array);
    //FIXME: 刷新详情界面的六大属性，暂时使用，杯具。
    void refreshGilOfficerInfo();
    
    //显示军魂换礼界面。
    void showSoulExchangeLayer(int et);
    //显示活动界面
    void requestFirstActivityLayer();
    
    void showFirstActivityLayer(CCArray *array,CCObject * obj);
    
    //id getSdkdelegate();
    void doRegist(int state,std::string& accountId,const char* loginKey);
    void doFBLogin();
    void dealFbBindFail();
    void doLogin(std::string& accountId,const char* loginKey, int channelId = 1, int subChannelId = 0, const char *nickName = "\0");
    void doFastLogin(std::string& accountId,const char* loginKey);
    void doPassword(int state,int flag);
 
    void trackLevel(int level);
    void trackReward(long moneyNum,CCString *reason);
    void trackPurchase(CCString *item,int num ,long price);
    void trackItemUse(CCString *item,int num );
    void trackMsnBegin(int sceneId);
    void trackMsnCompleted(int sceneId,bool isSpot = true);
    void trackMsnFailed(int sceneId,CCString *cause);
    void trackTaskBegin(CCString *taskName);
    void trackTaskCompleted(CCString *taskName);
    
    void loginIn();
	
	//显示批量卖出页面
	void showBatchSellLayer(int enter = 0, bool isBeforeDel = true);
	
    
    void showpvpEverydayLayer(int count ,CCArray *arrary,bool isBeforeDel = true);//天梯每日奖励

    
    void showjingzhouprocesslayer(CCArray *array);
    void showRankLayerLayer(CCArray *array,int value ,int ranking,int maxitemnum);

    
    void openChatShowLayerByType(SGBaseMilitaryCard *showChatItem,int showType,int nowLayersTag);
    void closeChatShowLayer();

	
	void refreshCreamBattleList(int storyId = 20000);//向服务器请求最新的精英副本数据
	// 显示boss战
  
    void showBossBattleLayer(BossBattleData *bossData,BossReward *storyReward);
    void showBossBattleJoinInLayer(BossBattleJoinData *data);
 
	void showBossBattleDetailLayer(SGBossBattleData *bossData, int enterType, bool del);
	void showBossBattleRankLayer(CCArray *rankData, SGKillBossInfo *killInfo, int enterType = 0, bool del = true);
	void refreshBossInfo(SGBossStateData *data);
	void freshBuffInfo(int v1, int v2, int v3);
 
	void showConsumableInfoLayer(SGConsumableCard *card, bool isBeforeDel = true, int enterType = 0, int whichShop = 3,int currentPage = 0);//显示消耗品详情
	void getFriendOfficerInfo();
  
	
	
    void showPiecesInfoLayer(SGPiecesCard* card,int type,bool isBeforeDel);
	void sendGetLastBossRankList();//发送boos战排行消息
    
    //显示天梯进阶赛统一用这个了，其他的删除了，有错勿怪。。。。。。
    void showPvpMatchLayer();
    void gotoMatchRequest();
    void showSeasonMainLayer(CCObject *sender);
	
	//设置布阵武将空位提醒
	void setEmbattleKey(bool val);
	//获取布阵武将空位提醒
	bool getEmbattleKey();
    
    //西域商人总览req
    void exoMechMainInfoRequest();
    //西域商人总览resp
    void exoMechMainInfoListener(CCObject* obj);
	
#if (PLATFORM == ANDROID)
	
	//android 登陆回调
	void loginReceive(CCObject *sender);
	//android 注册回调
	void registerReceive(CCObject *sender);
	//android 修改密码回调
	void modifyPassword(CCObject *sender);
	//android 快速登陆
	void fastLogin(CCObject *sender);
    
    //android facebook 从java获取登陆用户名和用户id
    void fbUserData(const char *userName, const char *userId);
    
    // 第三方登陆
    void thirdLoginReceive(CCObject *sender);
    //登陆或者注册一系列操作的错误提示
    void androidHttpShowTip(CCObject *sender);
    
    //根据分享的id设置返回要分享的字符串
    std::string getShareInfoFromShareId(int shareId);
    
    //facebook绑定失败通知
    void androidBindFacebookFailed(CCObject *sender);
#endif
	
	inline void setUserName(std::string userName){this->userName = userName;}
	inline void setPassword(std::string password){this->password = password;}
	inline void setIsExist(bool isEx){this->isExist = isEx;}
	
	inline std::string getUserName(){return this->userName;}
	inline std::string getPassword(){return this->password;}
	inline bool getIsExist(){return this->isExist;}
    
#if (PLATFORM == IOS)
    
    void setBuyData(const char *data);
    
    void sendCreatInvoice(float amount,const char * buyString);
    
    void sendInvoiceRequst();
    
    void doCreatInvoice(int state,int billId);

    void doReceiveInvoice(int state);
    
    void doInvoiceFail();
	

    CC_SYNTHESIZE_READONLY(int, m_billId, BillId);
    
    CC_SYNTHESIZE_READONLY(std::string, m_buyData, BuyData);
    
    CC_SYNTHESIZE_READONLY(std::string, m_buyString, BuyString);


#endif   
    void shareGameById(int shareId,int shareInfo,std::string &sharePicture );
    void doFBShare(int shareInfo,std::string &sharePicture );
    void finishFBShare();
    void shareFbDidFail();
    void setFBSessionState(bool sessionState);
    void doCloseFBlogin(int fbValue);
    void clearFBUserData(int fbValue = -1);
    void setFBUserData(const char * userId,const char* loginKey,const char *userName,bool setFbState = false);
    void doUpdateNewViewForFB();
    void updateNewViewForFB();
    inline std::string getServerName(){return this->servername;}
	inline int getServerId(){return this->serverid;}
    
    void dealIosOKLogin();
    void dealIosLogin(std::string &un,std::string &pw);
    void doCreatFBSession();
    void cleanAccount();
public:
    bool m_trackIsPve;
    bool haveChatShowLayer;
    CC_SYNTHESIZE(int, m_isFBLogin, IsFBLogin);
    CC_SYNTHESIZE(bool, isFbAutologin, IsFbAutologin);
    
    CC_SYNTHESIZE(bool, isBossKilled, IsBossKilled);
    CC_SYNTHESIZE(std::string, bosskillMsg, BosskillMsg);
private:
	bool isExist;
	std::string userName;//用于记录登陆状态
	std::string password;
    std::string servername;
    int serverid;
    int m_shareId;
public:
    //当接收到返回的商品列表消息，进入SGMallLayer.
    void showMallLayer(CCObject *);
    void sendEnterMallLayerRequest(ShopGUID shopId , ShopEnterType type=SHOP_LAYER );

    std::string m_sharePicture;

    
    //聊天按钮的显示和隐藏
	static void setChatBtnVisiable(bool isvisble);
    


	CC_SYNTHESIZE_READONLY(bool, m_facebookSessionState, FacebookSessionState)
    
    //战斗相关的统计处理 然后发送消息    actionType 意思是1=点击按钮  2=移动士兵 3=删除士兵
    static void preProcessDataInFightSceneForSend(int actionType);
    
    //发送用户点击记录消息
    static void sendOperatorRecord(int stepId , bool isShowCircle = false);
    
    //vip特权页面返回按钮 不知道从哪里来。加一字段 ，true=来自商城vip礼包。false=储值页面 ， 默认为false。
    //by:zyc
    CC_SYNTHESIZE(bool , _fromShopLayer ,FromShopLayer );
    
    //筛选可以用来武将转生的材料数量。
//    static void setCanAdvPartNum(SGOfficerCard * );
    
    //DEBUG黄点的测试
    void yellowTest();
    
    //快速开启去往“竞技场”途径的CIB。
    void showCommonInfoBoxGoToArena(SGBaseItemType type, int itemId, SGBoxDelegate* boxDeg = NULL,  int otherEnterType = -1, bool otherIsBeforeDel = true, int myEnterType = -1);
    
    //快速开启原型级物品信息BOX，直接BOX版本。
    void showCommonInfoBoxInProto(SGBaseMilitaryCard* bmc, SGBoxDelegate* boxDeg = NULL, int otherEnterType = -1, bool otherIsBeforeDel = true, int myEnterType = -1);
    
    //快速开启原型物品信息BOX，需要物品的TYPE/ID。
    void showCommonInfoBoxInProto(SGBaseItemType type, int itemId, SGBoxDelegate* boxDeg = NULL, int otherEnterType = -1, bool otherIsBeforeDel = true, int myEnterType = -1);

    //获取当前的渠道是哪个，台湾，或者大陆 //true 为TW， false不是台湾
    bool getCurrentChannelIsTW();
    
    //显示战场里的操作提示章节数（目前是前十章都提示）
    int getShowFriendlyTipsChapId();
    
    //记录二个装备itemId值
    CC_SYNTHESIZE(int, m_willAdvancedItemId, WillAdvancedItemId);
    CC_SYNTHESIZE(int, m_afterAdvancedItemId, AfterAdvancedItemId);
    CC_SYNTHESIZE(int, m_afterAdvancedSsid, AfterAdvancedSsid);
    
    CC_SYNTHESIZE(std::string, m_plistName, PlistName);
public:
    //是否从来源页面进入闯关 每次使用完成 置false
    static bool sourceToStory;
    static int static_plotId;
    
    
    //记录第二章是不是只有一个节
    CC_SYNTHESIZE(int, _chap2Only1, Chap2Only1);
    
    std::string getPlistWithChannel();
    
    
    
    //类似全局变量，记录幻境中将要去的节id与章id
    CC_SYNTHESIZE(int, _spBattleChapId, SpBattleChapId);
    CC_SYNTHESIZE(int, _spBattlePlotId, SpBattlePlotId);
    
};

#endif /* defined(__GameSanGuo__SGMainManager__) */
