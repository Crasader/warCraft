//
//  MainSceneLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-26.
//
//

#ifndef __GameSanGuo__MainSceneLayer__
#define __GameSanGuo__MainSceneLayer__

#include "SGBaseLayer.h"
#include "SGChatMessageCenter.h"
#include "SGDynamicCirculateLayer.h"
#include "SGCCLabelTTF.h"
#include "SGMailManager.h"
#include "SGCreditCardModel.h"
#include "SGHTTPDelegate.h"
#include "PlatformAdp.h"

#define      CHATBTNTAG  54321
extern int isInMyFriendList;
extern int HelpFriendPlayLev;
extern int HelpFriendRoleId;
extern const char* HelpFriendRoleName;
extern int HelpFriendItemId;
class CCSpriterX;
extern int HelpFriendItemLevel;
class SGMainLayer: public SGBaseLayer, public SGHTTPDelegate
{
public:
    CCSkeletonAnimation*  mYunCai1 ;    //云彩的spine动画
    CCSkeletonAnimation*  mYunCai2;
    void delayYuncai();
    
    SGCCLabelTTF *gold;
    SGCCLabelTTF *copper;
    CCSprite *money_bg;
    CCSprite *money_bg1;
    CCProgressTimer *expprog;
    SGCCLabelTTF *combatValue;
    SGCCLabelTTF *level;
    bool openByActivity;
    SGCCLabelTTF *chat;
    SGCCLabelTTF *chatMore;
private:
    SGButton *portrait_bg;

    SGCCLabelTTF *name;
    CCLabelTTF *experience;
    SGCCLabelTTF *tili;
    SGCCLabelTTF *jl;
    CCMenu *portraitMenu;

//    CCLayerColor *layer;

    CCSprite *gonggaobg;
    CCLabelTTF *tilicd;
    CCLabelTTF *junlcd;
    CCSprite *tilicdbg;
    CCSprite *junlcdbg;
    
    float upheight;
    float btmheight;
    float btnheight;
    float totaluphgt;
    float visitHeight;
    
    //刷新军粮体力
    int m_nPveLeftTime;
    int m_nPveCircleTime;
    int m_nPvpLeftTime;
    int m_nPvpCircleTime;
    
    // 开启新布阵格子的级数数组
	int levelArray[8];

    //布阵按钮
    SGButton * embattleBtn;
    
    
    //全服动态
    SGDynamicCirculateLayer *dynamic;
    
    //消息中心
    SGChatMessageCenter *center;
    
    //邮件
    SGMailManager *mailManager;
    
    //vip
    CCSprite *vipRank;
    int oldVipLevel;
    
    int chatShowItemSsid;
    int chatShowRoleId;
    int chatShowType;
//    CCMenu *portraitMenu;

    bool canChatShown;
    SGButton *chongzhi;
    SGButton *money_Button;
    SGButton *money_Button1;
public:
    bool m_isbattelAddFriend;
private:
    ///系统投诉
    void updatebattlebtn(CCObject*obj);
    
  
    ///
    void updatehomebtn();
    void barracksHandler();
    
    
    void addFriendListener(CCObject *obj);
    void storyListener(CCObject *sender);
    void activityListener(CCObject *sender);
    void visitListener(CCObject *obj);
    void rewardListener(CCObject *obj);
    
	void updateFriendBtn();//更新好友状态
    //通过一个int型的值，解析成时间格式的字符串，以秒为单位
    const char *getStringByTime(int leftTime);
    //把当前时间加上一段时间，返回一个时间格式的字符串
    const char *getStringFromNow(int time);
    void showAnnoucementListener(CCObject *obj);
    
    void showRewardLoginListener(CCObject *obj);
    
    void purchPvePowerEnterListener(CCObject *obj);
    void purchPvpPowerEnterListener(CCObject *obj);
    void entryFirstChargeShowListener(CCObject *obj);
	//进入boss战排行
	void entryBossRankList(CCObject *sender);
	void enterBossFightListener(CCObject *sender);
    void showBossEndRemind(CCObject *sender);
    //显示九宫格奖励登陆框
    void show9RewardLogin();
    void inviterListener(cocos2d::CCObject *obj);
public:
    
    
	
    virtual void requestFinished(int msgid, CCObject *data);
    virtual void requestFailed();
//    void storeHandler();
    void loginAwardList();
    void sendActivityHandler();
    /////系统投诉
      void  setupdatebattlebtnvisible();
    void updateLayerData();

    void showBossKilledMsg(SGBoxDelegate *bdg);
public:
	 void computeChateTime();
    void storyHandler();
    void battleHandler();
    SGMainLayer();    
    ~SGMainLayer();
    static SGMainLayer *create(const char *bgFile, LayerTag tag_);
    void updateUserMsg();    
    void setportraitMenu(bool isTouch);
    void showblack();
    void hideblack();
    void visithide();
    void visitshow();
    float getVisitHeight();
    float getBottomHeight();
    float getfivebuttonhight();
    float getHeadHeight();
    float getTotleUpHgt();
    void buttonClickFirstLayer();
    void sendMessage(CCObject *chatObject);
    void chatShowItemClickSend(CCObject *obj);
    void buttonClickChatRoom();
    void visitHandler();
    void activityHandler(); // 获取活动列表
    void showSeasonMainLayer(CCObject *sender);
    void updatePlayerInfo(CCObject *sender);
    void updatepvppve(CCObject *sender);
    void responseVSApply(CCObject *sender); // 响应天梯赛报名事件
    void responseVSReady(CCObject *sender); // 响应战前准备事件, 弹出提示对战双方的弹框
    void showBMatchBox(CCObject *sender);   // 提示天梯赛速配成功的弹框
    void flushVSerInfo(CCObject *sender);   // 获取Vser数据并刷新弹框
    void loginPvpServer();                  // 倒计结束后进入PVP服务器
    void showRewardBox(CCObject *sender);   // 战斗结束后掉落界面
    bool detectIsHaveItem(CCArray *arr, int nid);
    void showSceneLayer(CCObject *sender);
    void setIsCanTouch(bool touch, bool btn=true);
    void setPortraitMenuCanTouch(bool touch);
    int getMailSelectId();
    void setMailSelectId(int mailId);
    void updateTeam(CCObject *sender);
    void requestStory();
    void bossListener(CCObject *obj);
	void setEmbattleFlag(int lv);//根据不同的级数,确定是否显示布阵界面的问号,提示开启新的布阵格子 现在规则是每十级开一个新的格 2014-5-5 by bugcode
     //点击增加元宝按钮响应
    void onClickAddYuanbao();
    //点击增加铜钱按钮响应
    void onClickAddCopperMoney();

    //add notification
    void addNotifications();
 
    
    //特殊战斗章列表
    void specialBattleListener(CCObject *obj);
	void specialBattlePlostListener(CCObject *obj);

	//10.28
	void requestCream();
	void creamBattleHandler();
	void creamBattleListener(CCObject *s);
    
    //11.20 军魂换礼
    //军魂换礼req。 et:1=守荆州UI导入，2=战绩UI导入。
    void sjzSoulExchangeRequest(int et);
    
    //军魂换礼rsp
    void sjzSoulExchangeResponse(CCObject* obj);
    
    //2013.12.16 守荆州当日奖励，随时可能推送，只有接收，没有发送。
    void sjzDailyRewardResponse(CCObject* obj);
    
    //记录军欢换礼界面的enterType，杯具。
    int sjzSeEt ;
    
    //每5秒更新一次显示资源计数器信息
    void showRRInfo();
	
	void lvlUpEnterListener(cocos2d::CCObject *obj);
    void sendChatShowContent(CCString * itemName ,int showType,int showStarsLevel,int itemSsid);
    
    void resetChatShown();
    void updateqa(SGQAConcent *player);////
    
    void hideFirstCharge();
    void entryFirstChargeShow();
    void requestStoreLayer(CCObject *obj = NULL);
    void entryTongQueShow();
	
	void storeEnterListener(CCObject *obj);
    void exchangeCopperListener(CCObject *obj);
	
    //敲醒客户端
    void alanWakeListener(CCObject* obj);
    //武将碎裂动画
    void officerHolyCrashListener(CCObject* obj);
    
    //任务
    void taskListListener(cocos2d::CCObject *obj);
    //群雄争霸监听
    void enterAreanListener(cocos2d::CCObject *obj);
    //征战之路监听
    void enterFightRoad(cocos2d::CCObject *obj);
    //诸侯割据监听
    void enterZhuHouGeJu(cocos2d::CCObject *obj);
    
    //设置布阵红点的显隐
    void setEmbattleNoticeVisible(bool isVisible);
    
    //设置奇遇红点的显隐
    void setAdventureNoticeVisible(bool vis);
    
private:
    void showPlayerInfo();
    virtual void initMsg();
    void systemHandler();
    void friendHandler();
    void getShowItemListener(cocos2d::CCObject *object);
    void showChatItemByType(main::ViewSpectacularItemResponse *chatShowInfo);
    void showTongQueLisenter(CCObject *obj);
    void showFirstChargeGift(CCObject *obj);
    
    //pvp每日奖励
    void onPvpDayReward(CCObject *);
    
    
    CC_SYNTHESIZE_RETAIN(CCString*, m_name, S_name)
    
    
    //排行消息回调
    void rankListener(CCObject *);
    
    //vip礼包消息回调 MSG_VIPGIFT_BAGS
    void onVipGiftResponse(CCObject *);
    
    //排行按钮回调
    void onRankBtnClicked();
    
    //布阵红点
    CCSprite * m_embattleNotice;
    
    //奇遇红点
    CCSprite* m_adventureNotice;
    
    //手型动画提示
    CCSpriterX * m_hand;
public:
    //布阵回调
    void onEmbattleBtnClicked();
};

#endif /* defined(__GameSanGuo__MainSceneLayer__) */
