//
//  SGPlayerInfo.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-5.
//
//

#ifndef __GameSanGuo__SGPlayerInfo__
#define __GameSanGuo__SGPlayerInfo__
#include "cocos2d.h"
#include "SGOfficerCard.h"
#include "SGSoldierCard.h"
#include "SGEquipCard.h"
#include "SGPropsCard.h"
#include "SGGamePlayer.h"
#include "SGVipGifts.h"
#include "SGGamePlayerInfo.h"
#include "SGRankPlayer.h"
#include "SGLottery9ViewItem.h"
#include "SGSjzData.h"
#include "SGPiecesCard.h"
#include "SGConsumableCard.h"
#include "SGMaterialCard.h"
//#include "SGBattleManager.h"
#include  "SGQAConcent.h"
#include  "RoleModule.pb.h"
#include "SGMainTaskDataModel.h"
#include "SGGiftPower.h"
#define  LIANDANSTARTINDEX 9
#define  LAST4PLOTID  12
#define  SUPERGUIDPLOTID  16
#define  MAXPLOTID  "maxPlotId"
USING_NS_CC;
#include "native/CCAlertViewDelegate.h"
#include "native/CCNative.h"
USING_NS_CC_EXT;

typedef struct LimitGuide
{
    LimitGuide(int type,int value, int startId ,const std::string& name)
    {
        
        triggerType = type;
        triggerValue = value;
        triggerId = startId;
        triggerName = name;
    };
    int triggerType;
    int triggerValue;
    int triggerId;
    std::string triggerName;
}limitGuideData;

enum LimitId
{
    limitXianshiFuben = 1,
    limitJinyinFuben,
    limitPvpBattle,
    limitShouJinZhou,
    limitBossBattle,
    limitEquipGuide,
    limitOpenBox,
    limitStrenghWujianJi,
    limitStrenghSoilder,
    limitPicesGuide,
    limitSpecailBattle = 17, //幻境
    limitArena = 18, //竞技场
    limit_vs = 19,//vs layer 入口限制
};
class SGPlayerInfo : public CCObject
{
private:
    
    CC_SYNTHESIZE_RETAIN(CCString *, searchfriendcode, Searchfriendcode);// 搜索id
    CC_SYNTHESIZE_RETAIN(CCString *, invitationCode, InvitationCode);// 邀请码
    //  角色ID
    CC_SYNTHESIZE(int, playerRoleId, PlayerRoleId);
//  名字
    CC_SYNTHESIZE_RETAIN(CCString*, playerName, PlayerName);
//  昵称
    CC_SYNTHESIZE_RETAIN(CCString*, nickName, NickName);
//  好友上限
    CC_SYNTHESIZE(int, friendSize, FriendSize);
//  好友扩充次数
    CC_SYNTHESIZE(int, expandCount, ExpandCount);
//  数据库ID
    CC_SYNTHESIZE_RETAIN(CCString*, playerSsid , PlayerSsid);
//  级别
    CC_SYNTHESIZE(int, playerLevel, PlayerLevel);
//  铜
    CC_SYNTHESIZE(int, playerCoins, PlayerCoins);
    CC_SYNTHESIZE(int, playerJewels, PlayerJewels);
//  银
//    CC_SYNTHESIZE(int, playerSilver, PlayerSilver);
//  炼丹秘方
    CC_SYNTHESIZE(int, playerliandan, Playerliandan);
//  商城金子
    CC_SYNTHESIZE(int, playerGold, PlayerGold);
    //征服点 代币。
    CC_SYNTHESIZE(int, arenaScore, PlayerArenaScore);
    
//    统御力
    CC_SYNTHESIZE(int, playerGovering, PlayerGovering);
//     lk统御力
    CC_SYNTHESIZE(int, AddPlayerGovering, AddPlayerGovering);
//    国家
    CC_SYNTHESIZE(int, playerCountry, PlayerCountry);
//    玩家头像
    CC_SYNTHESIZE(int, playerImageID, PlayerImageID);
//    体力
    CC_SYNTHESIZE(int, playerPower, PlayerPower);
//    背包大小
    CC_SYNTHESIZE(int, playerBagSize, PlayerBagSize);
//    玩家昵称大小
    CC_SYNTHESIZE(int, playerModifyCount, PlayerModifyCount);
//    玩家经验
    CC_SYNTHESIZE(int, playerExp, PlayerExp);
    //士兵id
//    CC_SYNTHESIZE(int, g_redSoldierId, RedSoldierId);
//    CC_SYNTHESIZE(int, g_GreenSoldierId, GreenSoldierId);
//    CC_SYNTHESIZE(int, g_BlueSoldierId, BlueSoldierId);
    //是否能免费抽奖
    CC_SYNTHESIZE(int, m_freevisit, Freevisit);
    //小喇叭数量
//    CC_SYNTHESIZE(int, speakerCount, SpeakerCount);
    //军功
//    CC_SYNTHESIZE(int, award, playerAward);    
    //军粮
    CC_SYNTHESIZE(int, pvpPower, PvpPower);
    //体力剩余时间
    CC_SYNTHESIZE(int, pveLeftTime, PveLeftTime);
    //体力周 期时间
    CC_SYNTHESIZE(int, pveCircleTime, PveCircleTime);
    //军粮剩余时间
    CC_SYNTHESIZE(int, pvpLeftTime, PvpLeftTime);
    //军粮周期时间
    CC_SYNTHESIZE(int, pvpCircleTime, PvpCircleTime);
    //个人排行信息
    CC_SYNTHESIZE_RETAIN(SGRankPlayer*, pvpRank, PvpRank);
    //故事模式信息
    CC_SYNTHESIZE_RETAIN(CCArray*, _storyData, StoryData);
    //活动信息
    CC_SYNTHESIZE_RETAIN(CCArray*, _activityData, ActivityData);
    //能否九宫格抽奖
    CC_SYNTHESIZE(int, m_isCan, is9Show);
    //连续登陆天数
    CC_SYNTHESIZE(int, m_loginday, LoginDays);
    //九宫格抽奖次数
    CC_SYNTHESIZE(int, m_chance, LoginChance);
    //最后进入的场景ID
    CC_SYNTHESIZE(int, _plotId, PlotId); 
    CC_SYNTHESIZE(int, m_Victory, Victory);
    //天梯赛积分
    CC_SYNTHESIZE(int, _pvpCredit, PvpCredit);
    //新手教学ID
    CC_SYNTHESIZE(int, _guideId, GuideId);
    //是否第一次拿到四格武将
    CC_SYNTHESIZE(int, _isFirstFour, IsFirstFour);
    //vip等级
    CC_SYNTHESIZE(int, m_viplvl, playerVipLevel);
    //pvp等级
    CC_SYNTHESIZE(int, m_pvplvl, playerPvpLevel);
    CC_SYNTHESIZE(int , m_combatValue, combatValue);//战斗力
    //pvp积分
    CC_SYNTHESIZE(int, m_pvpnum, playerPvpNum);
    CC_SYNTHESIZE(bool, m_HasPveFight, HasPveFight);
    CC_SYNTHESIZE(int, m_MilitaryRank, MilitaryRank);   //军衔
    CC_SYNTHESIZE_RETAIN(CCString*, m_MilitaryRankName, MilitaryRankName);
    CC_SYNTHESIZE(int , m_battleType, BattleType);
    CC_SYNTHESIZE(int, m_pvpWinPvp, PvpWinPvp);
    CC_SYNTHESIZE(int, m_winPvp, WinPvp);
    CC_SYNTHESIZE(bool,m_canRewardDayTask,CanRewardDayTask);
    
    CC_SYNTHESIZE(int,m_cdkeyneed,CDKeyNeed);//是否需要激活码 1需要,0不需要
    CC_SYNTHESIZE(bool, loginAwardGold, LoginAwardGold);
   //是否显示vip
    CC_SYNTHESIZE(int, isshowvip, Showvip);
    CC_SYNTHESIZE(bool, isCgBattle, IsCgBattle);

    //是否进入天梯匹配
    CC_SYNTHESIZE(bool, ispvp, Ispvp);
    
    //功能开启等级限制
    CC_SYNTHESIZE(int, fubenLevelLimit, FubenLevelLimit);
    CC_SYNTHESIZE(int, shouJinZhouLimit, ShouJinZhouLimit);
    CC_SYNTHESIZE(int, netbattleLimit, NetbattleLimit);//pvp等级限制(现在重新启用)
    CC_SYNTHESIZE(int, superFubenLimit, SuperFubenLimit);//boss战等级限制(现在重新启用)
    CC_SYNTHESIZE(int, pvPRewardRate, PvPRewardRate);//pvp战奖励倍率
    CC_SYNTHESIZE(int, skipFightCount, SkipFightCount);//扫荡次数
    
    CC_SYNTHESIZE(int, wait, _Wait);
    
    CC_SYNTHESIZE(int , m_idid,checkid); //系统投诉存储所查看问题的id
    CC_SYNTHESIZE(int, m_ismaxlevel, ismaxlevel);    ////判断守荆州下一关是不是最终关    1代表没下一关  0代表有
    CC_SYNTHESIZE(int , m_index,index);
    CC_SYNTHESIZE(int, m_chatTime, chatTime);
    CC_SYNTHESIZE(int, m_saodangType, saodangType);//大于0就是扫荡,1 是普通单扫,2普通10连扫,3是精英单扫,4是精英2连扫
    CC_SYNTHESIZE(bool, m_isActiviNotAccess, IsActiviNotAccess);
    
    
    CCArray *_officerCards;
    CCArray *_soldierCards;
    CCArray *_equipCards;
    CCArray *_propCards;
	CCArray *_piecesCards;//碎片卡牌
	CCArray *_piecesCommon;//通用碎片
	CCArray *_consumCards;//消耗品道具
	CCArray *_materialCards;//材料表
    
    
    CCArray *_serverList;
    
    CCArray *_lotteryItems;
    CCArray *_rewardNotice;
    CCArray *_rewardOtherPlayer;

    CCArray *_arrayFriend;  //好友列表
    CCArray *_arrayBlack;   //黑名单
    CCArray *_arrayApply;   //好友请求
    
    CC_SYNTHESIZE_READONLY(CCArray *,_friendPowerList, FriendPowerList);
    /*   
     *      好友角色信息缓存
     *   存放查看过的好友的信息
     *   当点击好友头像时，首先检查数据里是否有好友的信息
     *   如果有，就直接用缓存的信息，如果没有，再请求服务器
     *   暂时还没用到
     */
    
    CCArray *_arrayFriendInfoCache;

    CCArray *_arrayFriendMails; //好友邮件
    CCArray *_arraySystemMails;      //系统邮件
    CCArray *_arrayGMMails;           //GM邮件
    CCArray *_arrayAnnoun;   //公告
    CCArray *_arrayQAcontent;   //QA内容
    //////保存上次提交问题的时间
    CC_SYNTHESIZE(long, m_oldtime, oldtime);
   // 保存上次对问题添加描述时间  sgchatwithgm界面
    CC_SYNTHESIZE(long, m_withgmoldtime, withgmoldtime);
    
    // 是否已充值
    CC_SYNTHESIZE(bool, m_haveCharged, HaveCharged);
    
    // 是否最强装备提示
    CC_SYNTHESIZE(bool, m_bestEquip, BestEquip);
    
    // 是否需要验证
    CC_SYNTHESIZE(bool, m_isNeedVeriFy, IsNeedVeriFy);
    
    //每次登陆上来精英副本的数据字典
    CCDictionary *bossPlotPlayTimesDic;

    
private:
    SGPlayerInfo();
    ~SGPlayerInfo();
public:
    static SGPlayerInfo* sharePlayerInfo(void);    
    
    void addOfficerCard(SGOfficerCard *card);
    void addSoldierCard(SGSoldierCard *card);
    void addEquipCard(SGEquipCard *card);
    void addPropCard(SGPropsCard *card);
    void addFriend(SGGamePlayer *player);
    void addFriendPower(SGGiftPower *player);
    void addBlack(SGGamePlayer *player);
    ////kanata
    void addQAContent(SGQAConcent *content);
    ////
    void addApply(SGGamePlayer *player);
    void add9Item(SGLottery9ViewItem *item);
    void addServerList(CCArray *array);
    void addrewardNotice(CCObject* object);
	
	void addPiecesCard(SGPiecesCard *card);//添加碎片卡牌
	void addPiecesCommonCard(SGPiecesCard *card);//添加能用碎片
	void addConsumableCard(SGConsumableCard *card);//添加消耗品卡
	void addMaterialCard(SGMaterialCard *card);//添加材料卡牌

    //普通关卡相关。
    int  getMaxPlotId();
	void setMaxPlotId(int maxId,bool foceSet =false);
    //精英关卡相关。
    void setMaxBossPlotId(int maxId);
    int getMaxBossPlotId();
    
    bool checkIsSkipDayPlaying();
    void resetSendFriendTili();
        //获得领奖状态
    CCArray* getrewardNotice();
    //获得拜财神中奖信息
    CCArray* getrewardRegPlayers();
    void setRewardRegPlayers(CCArray *bscArr);
    CCArray* getServerList();
    SGOfficerCard *getOfficerCard(int sid);
    SGEquipCard *getEquipCard(int sid);
    SGSoldierCard *getSoldierCard(int sid);
    SGPropsCard *getPropCardB(int ssid);
	SGPiecesCard *getPiecesCardById(int ssid);//通过ssid获取碎片
	SGConsumableCard *getConsumableCardById(int ssid);//根据ssid获取对应的消耗品
	SGMaterialCard *getMaterialCardById(int ssid);//根据id获取材料卡牌
	SGMaterialCard *getMaterialCardByItemId(int itemId);//根据itemId获取卡牌

    CCArray *getOfficerCards();
    void removeOfficerCardById(int ssid);
    void removeSoldierCardById(int ssid);
    void removeEquipCardById(int ssid);
    void removePropCardById(int ssid);
	void removePiecesCardById(int ssid);//根据ssid删除碎片卡牌
	void removeConsumableCardById(int ssid);//根据ssid删除消耗品
    void removeMaterialCardById(int ssid);//根据ssid移除包里的材料卡牌
    
    //得到是否上场的武将
    CCArray *getShowOfficerCards(bool isshow = true);
    CCArray *getSoldierCards();
    CCArray *getEquipCards();
    CCArray *getPropCards();
	CCArray *getPiecesCards();//  获取碎片
	CCArray *getPiecesCommonCards();//获取通用碎片
	CCArray *getConsumableCards();//获取消耗品数组
    
    CCArray *getMaterialCards();//获取材料数组
    
    CCArray *get9Items();
    CCArray *getAnnouns();
    CCDictionary *msgstyleDic;
    
    //获取好友列表
    CCArray *getFriendList();
    
    //获取黑名单列表
    CCArray *getBlackList();
    
    //获取好友请求列表
    CCArray *getApplyList();
    
    //获取好友邮件列表
    CCArray *getFriendMails();
    
    //获取系统邮件
    CCArray *getSystemMails();
    
    //获取GM邮件
    CCArray *getGMMails();
    
    //获取QA信息
     CCArray *getQAContent();
    
    CCArray *SetQAContent();
    
    void addheight(float ss);
    float getheight();
    void addbtmhgt(float ss);
    float getbtmhgt();
    
    //通过角色id从角色缓存里获取角色信息,如果找不到，就返回为空
    SGGamePlayerInfo *getGamePlayerInfoById(int roleId);

    //获得布阵的武将
    CCArray *getEmbattleOfficers();
    
    //更新故事模式关卡信息
    void updateStoryData(int plotId, int rank);
    //判断当前场景是否是最后一关
    bool isLastSceneOfStory(int plotId);
    
    //获取当前场景ID
    int getCurrentSceneId();
    //获取当前故事ID
    int getCurrentStoryId();
    
    //获取卡牌总量
    int getTotalCardsCount();
    //---------------------------------
    // 更改一些货币类的数量，直接传入正负值即可
    //---------------------------------
    //更改铜钱数量
    void changeCoinsCount(int count);
    //更改金子数量
    void changeGoldCount(int count);
    //更改小喇叭数量
//    void changeSpeakerCount(int count);
    //更改炼丹秘方的数量
    void changeLiandanCount(int count);
    //更改军功数量
//    void changeAwardCount(int count);
    
    CCDictionary* getmsgstyleDic();
    
    int checkStartLimitGuide(int checkType,int checkPlotId = 0);//1等级  2闯关  3每日任务
    int getLimitLevelById(LimitId triggerId);
    bool canBreakLimitById(LimitId triggerId);
   
    //守荆州数据存储
    CC_SYNTHESIZE_RETAIN(SGSjzData*, _sjzData, SjzData);
    
    //获取和存储精英副本次数
    void setBossPlotPlayTimesDic(std::string &storyIdAndPlotIdAsKey, int residuTimes);
    int getBossPlotPlayTimesDic(std::string &storyIdAndPlotIdAsKey);
     //任务数组存储
private:
    CCDictionary *mainTaskDict;
    CCArray *taskGroupArray;
public:
    CCArray * getSubTaskByGroupName(const char *taskName);
    void addSubTask(SGMainTaskDataModel *dataModel);
    void removeSubTask(SGMainTaskDataModel *dataModel);
    const  char * getGroupNamebyId(int groupId);
    CCArray *getTaskGroup();
    CCDictionary *getMainTaskDict();
 
    /////存储排行榜相关信息
    CC_SYNTHESIZE(int, ranktype, rankType);
    CC_SYNTHESIZE(int, nation, playerNation);
    CC_SYNTHESIZE(int, playerid, playerID);
    CC_SYNTHESIZE(int, starnum, starnum);
    CC_SYNTHESIZE(bool, isshowed, isshowed);
 
    CC_SYNTHESIZE(int, powerGiftTimes, PowerGiftTimes);//今天还能领的赠送体力次数
    
    //当角色等级小于指定等级时 首页只显示布阵红点提示。
    CC_SYNTHESIZE(int, m_showPlayerNoticeLimitLev, ShowPlayerNoticeLimitLev);
    //等级提示2：高于该等级不出现武将强化、装备强化红点
    CC_SYNTHESIZE(int , m_showNoticeLimitLev2, ShowNoticeLimitLev2);
    //出现武将强化的最小等级差
    CC_SYNTHESIZE(int,m_officerStrengLevBigger , OfficerStrengLevBigger);
    //出现装备强化最小等级差值
    CC_SYNTHESIZE(int, m_equipStrengLevBigger, EquipStrengLevBigger);
    //背包中经验丹数量多少时添加武将强化红点提示  数据从controlvalue中读取。
    CC_SYNTHESIZE(int, m_expCardNumCanShowNotice, ExpCardNumCanShowNotice);
    //拥有多少金钱且武将身上装备未达到上限，添加装备强化红点提示 从controlvalue中读取。
    CC_SYNTHESIZE(int , m_coinNumCanShowNotice, CoinNumCanShowNotice);
    //武将培养红点提示对应 培养丹数量限制
    CC_SYNTHESIZE(int, m_devPelletLimitCnt, DevPelletLimitCnt);
    //武将培养红点提示对应 潜力点数量限制
    CC_SYNTHESIZE(int, m_devPointLimitCnt, DevPointLimitCnt);
    //武将培养红点提示对应 武将等级限制
    CC_SYNTHESIZE(int, m_devOfficerLimitLev, DevOfficerLimitLev);
    //武将培养红点提示对应 武将主公限制
    CC_SYNTHESIZE(int, m_devOfficerLimitPlayerLev, DevOfficerLimitPlayerLev);

    //boss 战
public:
    CC_SYNTHESIZE(bool, isBossBattleTime, isBossBattleTime);
    
public:
    std::vector<limitGuideData> m_limitGuideTags;
    
    //军魂
    CC_SYNTHESIZE(int ,_junHun,JunHun);
    
private:
    //保存精英闯关《plotId ，《剩余次数 , 是否可参见》 可参加总次数数据表中。
    std::map<int , std::vector<int> > m_creamPlotIdNum;
    //当前已打过的精英闯关boosplotid。
    int _maxBossPlotId;
    //当前已打过的普通关卡plotid。
    int maxPlotId;
public:
    void setCreamData(int plotId , int canJoinCount  , int isCanJoin);
    int getCreamCanJoinCount(int plotId);
    bool getCreamIsCanJoin(int plotId);
    
    bool isGuideStory(int type, int storyId);
    
    
    
    //判断是否时引导
    bool isGuid();
    
    //是否满足士兵强化的条件。
    bool canSoldierStreng();
    
    //判断碎片召唤时 判断是否已经拥有对应的武将
    bool alreadyHasTheOfficer(int officerProtoId);
    
private:
    //解锁武将卡槽对应的最小角色等级
    std::vector<int> m_unlockAtMinLevVec;
    void analyseUnlockLev();
public:
    //返回某个卡槽解锁的具体等级提示 1开始索引 1-10
    int getTipAtLevel(int index);
    
    //背包中物品物品数量    用来判断是否达到上限
    //武将 士兵 装备 道具
    int getGoodsNumInBag();
    
    /*
     *返回背包中某个物品的数量
     *args: 物品类型 -- 物品原型id
     */
    int ownItemCount(SGBaseItemType itemType, int itemId);
};

#endif /* defined(__GameSanGuo__SGPlayerInfo__) */
