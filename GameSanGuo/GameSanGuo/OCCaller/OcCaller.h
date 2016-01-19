//
//  OcCaller.h
//  GameSanGuo
//
//  Created by bugcode on 14-5-12.
//
//

#ifndef __GameSanGuo__OcCaller__
#define __GameSanGuo__OcCaller__


#include "PYUserSDKLib.h"
#include "SdkReqCallBack.h"
#include "GameIapStore.h"
#include <mach/mach.h>
#include <Foundation/Foundation.h>
#include "Reachability.h"
#include "native/CCNative.h"
#include "PlatformAdp.h"
#include "GlobalConfig.h"

#include "cocos2d.h"
#include <iostream>
#include "SGRootViewController.h"

//#define   GO2PLAYSDKADDRESS     "http://test.generalmobi.com/qiwiwallet/"
//#define   GO2PLAYGAMEID         45
//#define   GO2PLAYKEY            "570c303aa5d911e3a2c1782bcb337be1"

#define   GO2PLAYGAMEID         86
#define   GO2PLAYKEY            "6a792beca0d34c4d8026a0c1e202e8f2"

#define   GO2PLAYPAYTYPE        "APPLE"
#define   GO2PLAYCCY            "TWD"   //"CNY"
#define   GO2PLAYCOMMENT        "TEST"
#define   GO2PLAYCOUNTRY        "TW"    //"CN"
/*
 
 内部的OC的调用都映射到此类,工程内部全部使用c++调用,外围适配类使用oc相关调用
 
 */

class OcCaller : public PlatformAdp
{
private:
	OcCaller();
	id m_SdkReqCallBack;
	GameIapStore *iap;

public:
	static OcCaller *getInstance();
	/*
		外部的oc相关调用
	*/
	//SGCantAdvanceBox SDK调用
	virtual void 	pyUserSDKCallReq();
	
	//SGChangePasswordLayer SDK改密码调用
	virtual void 	pyUserSDKChangePasswordReq(const char *userName, const char *oldPassword, const char *newPassword);
	
	//SGHelpLayer SDK登陆调用
	virtual void 	pyUserSDKLoginReq(const char *userName, const char *password);
	virtual void    pyUserThirdPartySDKLoginReq(const char *userName, const char *password);
	//SGMainManager SDK成员
	virtual void 	SDKReqAlloc();
	virtual id		getSdkReqDelegate();
	virtual void 	startSDK();
    virtual void    startHasOffers();
    
	virtual void 	setSdkReqLevel(int lvl);
	virtual void 	trackSDKReward(long moneyNum,const char *reason);
	virtual void 	trackSDKPurchase(const char *item,int num ,long price);
	virtual void 	trackSDKItemUse(const char *item,int num);
	virtual void 	trackSDKMsnBegin(const char *storyInfo);
	virtual void 	trackSDKMsnCompleted(const char *storyInfo);
	virtual void 	trackSDKMsnFailed(const char *storyInfo, const char *cause);
	virtual void 	trackSDKTaskBegin(const char *taskInfo);
	virtual void 	trackSDKTaskCompleted(const char *taskInfo);
	virtual void 	onChargeSuccessSDK(std::string& userId, long time, int randNum);

	
	//SGRegisterBox		SDK注册
	virtual void 	registerUserSDK(const char *userName, const char *password);
	virtual void 	setAccountSDK(std::string& userId, const char *userName);
	
	//SGServerlistNewServerBox
	virtual void 	okLoginSDK();
	
	//SGStoreLayer
	virtual void 	onChargeSDK(std::string& userId, long time, int randNum, const char *ipadId, int money, int virtualMoney);
	virtual void 	gameIapStoreAlloc();
	virtual bool	isIapInstanceExist();
	virtual void 	iapInstnceRelease();
	virtual void 	iapBuyPayMent(int state);
	virtual void 	iapBuyItem(const char *info);

	//GameConfig getudit
	virtual std::string getUdid();
	virtual virtual void  prohibitLockScreen();

	virtual NetStatus checkNetWorkStatus();

	virtual const char *longRangePath(const char *name, std::string filePath);
	virtual void  saveScreenImage(std::string fileFullPath);
	virtual cocos2d::ccColor3B colorWithHexString(const char *hexString);
	virtual const char *bundleVersion();
	void  logMemInfo();
	bool memoryInfo(vm_statistics_data_t *vmStats);
	virtual bool isContainEmoji(const char *str);
	virtual virtual void  delLog();
	virtual virtual void  writeLogToXML(const char *logInfo);
	virtual virtual void  showPopWindow(const char *showInfo);
	virtual double getAvaliableMem();
	virtual double usedMem();
	virtual const char *getTime();
	
	//platformIOS 里面推送消息,出现网络等待的图等
	
	void createController(SGRootViewController **root);
	void releaseController(SGRootViewController **root);
	
    void showIOSConnection(SGRootViewController **root, bool isshow = true);
    void removeIOSConnection(SGRootViewController **root);
    //推送好友消息
    void pushIOSFriendMessage(SGRootViewController **root, const char* name);
    //推送体力回满
    void pushIOSPowerFull(SGRootViewController **root, int timeLeft);
    //推送活动
    void pushIOSActivities(SGRootViewController **root);
	
	//ios下的richlabel
	CCSprite* getIOSFomLabel(const char* cstr, const cocos2d::CCSize &size, const char* fntName, float fontSize,
							 cocos2d::ccColor3B color = cocos2d::ccRED, cocos2d::ccColor3B defaultInColor = cocos2d::ccWHITE,
							 cocos2d::ccColor3B defaultOutColor = cocos2d::ccBLACK);

	void setString(const char *cstr, CCSprite *sprite, CCNode *parent);
	
    void sendCreatInvoice(const char *usrName,float payAmount,int playerId);
	void sendInvoiceStatus(int billId);
    
    void closeFBlogin(int closeValue = 0);
    
    //打开指定URL。
    virtual void openUrl(std::string url);
    
    //加入gocpa SDK
    void addGoCPASDK();
    
    //初始化AF的sdk
    void  initAppsFlysWithAppid();
    //启动AF
    void startAppsFlys();
    //统计AF登陆跟踪
    void appsFlyerLoginTrack(std::string info);
    
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
	void createFBSession();
    void showFBlogin();
    void startFBShare(int shareInfo,std::string &sharePicture );
#endif
};

#endif /* defined(__GameSanGuo__OcCaller__) */
