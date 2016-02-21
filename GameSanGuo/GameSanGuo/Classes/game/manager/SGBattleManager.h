//
//  SGBattleManager.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-20.
//
//

#ifndef __GameSanGuo__SGBattleManager__
#define __GameSanGuo__SGBattleManager__

#include "cocos2d.h"
#include "SGBattleLayer.h"
#include "SGSoldier.h"
#include "../../../libs/extensions/native/CCAlertViewDelegate.h"
#include "../../../libs/extensions/native/CCNative.h"

#include "FightBaseModule.pb.h"   //removed by cgp

#define BATTLESID       "BattleSid"
USING_NS_CC;
USING_NS_CC_EXT;
//当前战斗类型，由战斗初始化消息343带下。
enum BATTLE_TYPE
{
    BT_PEACE = 0, //和平
    BT_ADVENTURE = 101, //闯关
    BT_SURVIVAL = 102, //守荆州
    BT_LADDER = 201, //天梯
    BT_ARENA = 103, //竞技场
};


class SGBattleManager : public CCObject, public CCAlertViewDelegate
{
    
public:
    void pveListenerDelay();
    main::PveStarResponse  mBattleData;
    CCSprite*   battleBg;
    CC_SYNTHESIZE(int, randomMapId, RandomMapId)
    
private:
    void logSbData(SGSBObj *obj);
    void connSuccess(CCObject *obj);
    void loginSuccess(CCObject *obj);
    
    //移动监听
    void moveListener(CCObject *obj);
    //删除监听
    void deleteListener(CCObject *obj);
    //补兵监听
    void fillUnitListener(CCObject *obj);

    int clientCheckControlRound();
    void clientGiveUpHandler();
    void giveUpListener(CCObject *obj);
    
    void gameoverListener(CCObject *obj);
    
    
    void validationListener(CCObject *obj);
    
    void overSettlement(CCObject *obj);
    void runAwayResponse(CCObject *obj);

    void fightReviveResponse(CCObject *obj);
    SGHeroLayer* origFightFixMapResponse(void *fixmapResponse);
    void fightFixMapResponse(CCObject *obj);
    // PVE剧情loading
    void pveLoadingListener(CCObject *obj);
    
    void clientPveContinueInitHeroLayer(CCDictionary * totalSbDics,bool isMyhero);
    void clientPveContinueInitBattleSbs(CCDictionary * totalSbDics,bool isMyhero);
    void clientPveContinueInitAttackList(CCDictionary * totalSbDics,bool isMyhero);
    void clientPveContinueInitBackupSbs(CCDictionary * totalSbDics,bool isMyhero);
    SGHeroLayer * clientPveContinueDeal(void *fixmapResponse);
    void pveContinuListener(CCObject *obj);
    void reviveResponse(CCObject *obj);
    void QiangZhiFillUnitResponse(CCObject *obj);
    void pveListener(CCObject *obj);

    
    
    void addHisBackUpSb(SGSBObj *battleSb);
    void addMyBackUpSb(SGSBObj *battleSb);
    SGSBObj* getBackUpSbById(int colorId);
    SGSBObj* getBackUpHeroByIndex(int index);
    sgShiBingType getBackUpHeroTypeByIndex(int index);
    void addPveBackHero(void *sr,bool needFriendly = true);
    int getBackUpHeroCount();
    void clientSaveSbData(SGBattleMap * battleMap ,CCDictionary *totalSbDics,bool IsMe);
    void clientSaveAttackData(SGBattleMap * battleMap ,CCDictionary *totalSbDics,bool IsMe);
    void clientSaveHeroData(SGHeroLayer * heroLayer ,CCDictionary *totalSbDics,bool IsMe);
    void clientSaveBattleData(SGHeroLayer * myLayer ,SGHeroLayer*enemyLayer);
    void writeDicData(CCDictionary *sbDic,SGSBObj *shiBing,const std::string key);
    void clientSaveBackupSbs(CCDictionary *totalSbDics);
public:
    void pveStarts();
    void pveContinuStart(int back);
    void pvpStarts();
    
    void clientfillFriend();
    int getFriendSid();
    const char* getClientSaveBattleDataName();
    
    virtual void alertViewClickedButtonAtIndex(int buttonIndex);
public:
    //监听匹配玩家
    void matchListener(CCObject *obj);
    //pvp场景loading
    void pvpLoadingListener(CCObject *sender);
    
    void matchListener1(CCObject *obj);
public:
    //是否是热座模式
    bool isHotSeat;
    bool isGuide;
    bool isContinue;
    bool isWin;
    bool isMatchSucc;
    bool isSecond;
    main::PveStarResponse  mbattleData;
    
public:
    static SGBattleManager* sharedBattle(void);
    SGBattleManager();
~SGBattleManager();
    CC_SYNTHESIZE(bool, isLoadingReady, IsLoadingReady)   //检测战斗loading是否完成
    
    SGBattleLayer *getBattleLayer();
    //得到地图layer my：是否得到对方地图
    SGBattleMap *getBattleMap(bool enemy = false);
    SGHeroLayer *getHeroLayer(bool isMyLayer);
    void pveStart(int storyId, int plotId, int roleId);
    
    //战斗结束
    void battleOver();
    void guideBattleOver();
    //初始监听
    void initNotification();
    //匹配请求
    void matchRequest();
    // 移动请求
    void moveRequest(int fromx, int tox);
    // 删除请求
    void deleteRequest(int x,int y);
    //补兵请求
    void fillUnitRequest();
    //放弃回合请
    void giveupRequest();
    //游戏结束
    void gameoverRequet();
    //地图验证请求
//    void validationRequet(std::string str);
    void validationRequet(CCArray *array);
    void clientFillHeroLoader(CCArray *sbs,const int &list);
    void clientFillSbLoader(SGBattleMap * battleMap,CCArray *sbs,int fillUnitCont,const int& randList,const int& randColor);
    bool clientFillUnitLoader(CCArray *sbs);
    void clientFillUnit();
    void startFillFriend();
    //放在了（batlelayr中了）显示谁先出手界面
  //  void showFirstActionLayer(int mySpeed,int enemySpeed);
    //对方掉线
    void lostConnection();
    void runAwayRequest();
    bool detectIsHaveItem(CCArray *arr, int  nid);
    
//    void cooperationFillRequest();
    void fightLogRequest(const std::string &str);
    void fightReviveRequest(int revive);
    void fightFixMapRequest();
    void reviveRequest();
    void QiangZhiFillUnitRequest();
    
    void showWaiteBox();//显示等待对方进入
    void PveCancelWaiteListener(cocos2d::CCObject *obj);
    
public:
    void openReceive();
    void closeReceive();
    
    void initMyMap();
    void initHisMap();
    void dealCgPveListener(cocos2d::CCObject *obj);
private:
    //战斗主界面
    SGBattleLayer *mainBattleLayer;
    HeroObj *myHeroObj;    //城墙、血量基础等信息
    HeroObj *enemyHeroObj;
    CCArray *himsbs;   //包含英雄
    CCArray *mysbs;    //包含英雄
    
    //被收藏的士兵和英雄
    CCArray *himBackupSbs;
    CCArray *myBackupsbs;
    CCArray *himBackupHeros;
    CCArray *myBackupHeros;
    
    SGSBObj *cooTempOfficer;
    
    CC_SYNTHESIZE(bool ,m_isBattlePve,IsBattlePve);//这个是判断该场战斗是否应该与服务器验证,默认false是需要验证(pvp 守荊州,一些验证的引导关卡),
    
    //变量来识
    bool m_canTouchAutoBattleBtn;

    //CC_SYNTHESIZE(bool, m_canTouchAutoBattleBtn, CanTouchAutoBattleBtn);
    void setCanTouchAutoBattleBtn(bool canTouch);
    bool getCanTouchAutoBattleBtn();
public:
    bool pveLoadMoveFinish;
    
    //本次战斗的类型。
    CC_PROPERTY(BATTLE_TYPE, _battleType, BattleType);
    
    //守荆州的进度，第几波，只有守荆州战斗情况下有效。
    CC_SYNTHESIZE_READONLY(int, _sjzLevel, SjzLevel);
    
    //守荆州相关BGN>>
    
    //开始守城请求。
    void sjzChallengeRequest();
    
    //开始守城响应。
    void sjzChallengeResponse(CCObject* obj);
    
    //尝试进入战斗req。 isJump:true则跳过本场战斗，false则不跳过，若如此以后都不能跳过了。
    void sjzStartRequest(bool isJump = true);
    
    //尝试进入战斗rsp。
    void sjzStartResponse(CCObject* obj);
    
    //守荆州战败后战绩统计信息
    void sjzFinalStatResponse(CCObject* obj);
    
    //<<守荆州相关END
public:
    void startCountingForHelp(bool counting=true);
    
    //MM&ZY: 如果是需要验证的战斗,则需要回合驱动标志为true,才能执行某些动作,目前针对sgherolayer放里面的所有按钮.
    bool getCanDoBattleOperation();
    
private:
    int startFiveRound;
    
    /*1510号消息响应 pvp战斗奖励*/
    void pvpFightRewardResponse(CCObject *);
    void arenaFightRewardListen(CCObject * obj);
};
#endif /* defined(__GameSanGuo__SGBattleManager__) */
