//
//  SGMainScene.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-20.
//
//


#define UPDATE_LOADING 1


#ifndef __GameSanGuo__SGMainScene__
#define __GameSanGuo__SGMainScene__

#include "cocos2d.h"
#include "SGBattleMatchLayer.h"
#include "native/CCAlertViewDelegate.h"
#include "native/CCNative.h"
#include "SGBaseBox.h"
#include "RoleModule.pb.h"
#include "SGMessageLayer.h"
#include "SGGamePlayer.h"
#include "SGFriendManager.h"
#include "SGPlatFormIos.h"
#include "SGCreditCardBox.h"
#include "SGMailManager.h"
#include "SGBoxDelegate.h"
#include "SGQAConcent.h"
#include "GMQAModule.pb.h"
#include "SGGiftPower.h"

class SGConnectLoading;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;


#define       DATA_LOADING 0

class SGMainScene :public CCScene, public CCAlertViewDelegate, public SGBoxDelegate, public CCKeypadDelegate
{
public:
    LayerTag nowLayerTag;
    bool nowIsCocostudio;   //存储上一个界面是否是cocostudio
    
    BoxTag nowBoxTag;
    BoxTag oldBoxTag;
    SGPlatFormIos *plat;
    CCLabelTTF *usedMemory;
    CCLabelTTF *availableMemory;
    CCSprite *pvpMatch; //pvp匹配
    Layout*   mCocosLayers[SG_LAYER_TAG_TOTAL_NUM - cgp_strengAdvLayer];
  
    TouchGroup* m_pUIGroup;     //cgp cocos
    virtual void onEnter();
   
    
private:
    SGMainScene();
    //提示消息layer
    SGMessageLayer *message;
    
    //小貂蝉加载动画layer
    SGConnectLoading* conLayer_;
    
    //好友消息管理器
    SGFriendManager *friendManager;
//    SGMailManager *mailManager;
    //监听长连接是否成功
    void socketConnSuccess();
    //监听长连接是否失败
    void socketConnFail();
    //接收数据错误
    void socketRecvError();
    //接收数据中断
    void socketRecvBreak();
    void upDateMemory();
    
    void socketRegListener(CCObject *obj);
    void createRoleListener(CCObject *obj);
    void rewardRegListener(CCObject *obj);
    void taskFinishListener(CCObject *obj);
    void announceListener(CCObject *obj);
    void rewardLoginListener(CCObject *obj);
    void upDateBagInfoListener(cocos2d::CCObject *obj);
	//void upDateBagInfoHandler(main::UpdateBagInfo *msg);
    void showAllTextureCache();
    void updateMailListener(CCObject *obj);
    void stopServiceMsgProcess(cocos2d::CCObject *obj);
    void shareRewardProcess(cocos2d::CCObject *obj);
    //更新主线任务信息
    void updateMainTaskListener(CCObject *obj);
    
    //boss 战开启状态
    void upDateBossStartState(CCObject *obj);
    
    void addGiftPower(CCObject *obj);
private:
    void logCache();
    void startLoading();
    void initRoleInfo(main::RoleResponse msg);
    SGGamePlayer* getSGGamePlayer(main::FriendProto _friend);
    /////
    SGQAConcent* getSGQAContent(main::QAProto content);

public:
    CREATE_FUNC(SGMainScene);
    ~SGMainScene();
    virtual bool init();
    //添加监听们
    void initNotifications();
//    void removeNowLayer();
    //添加要显示的layer
    // isBeforeDel 之前的在显示的layer 是否删除
    void addShowLayer(SGBaseLayer *layer, bool isBeforeDel = true);
    
    void showBattleLayer(SGBaseLayer* layer);   //added by cgp for loading battle
    
    void addBattleMatchLayer();
    
    virtual void alertViewClickedButtonAtIndex(int buttonIndex);
    //显示提示信息。
    void showMessage(const char *messageStr);
    //显示加载动画。
    void showLoading(bool bl);
    
    void welcome();
    
    void resetMatchVisible(CCObject *sender);
    void showPvpMatch(bool visible);
    
    void backFight();
    void pvpLoginFail();
    
    void showEffect(SGEquipCard *card,int type);

    //void upDateBagInfoHandler(main::UpdateBagInfo *msg);

    static void upDateBagInfoHandler(main::UpdateBagInfo *msg);
    
    void showCountryLayer();
    SGGiftPower* getSGGiftPower(main::PowerGiftProto _friend);
    
    /*为修正自动战斗中加速 对全局定时器的影响*/
    float m_pvpDelay;
    float m_bossDelay;
    
    //pvp快速入口定时器处理
    void OnPvpTimeOver(float t);
    //boss快速入口定时器处理
    void OnBossTimeOver(float t);
    void updateSecond();//小包更新资源
private:
    void showMenu();
    void startEditmap();
protected:
virtual void showBoxCall(CCObject *obj);
virtual void setIsCanTouch(bool isTouch);
virtual void setIsLock(bool isLock);
public:
    // The back key clicked
    virtual void keyBackClicked();
    
    // The menu key clicked. only available on wophone & android
    virtual void keyMenuClicked();
};

#endif /* defined(__GameSanGuo__SGMainScene__) */
