//
//  PlatformAdp.h
//  GameSanGuo
//
//  Created by bugcode on 14-5-13.
//
//

#ifndef __GameSanGuo__PlatformAdp__
#define __GameSanGuo__PlatformAdp__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

//安卓
#define ANDROID		1
//苹果
#define IOS			2
//具体哪个平台
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define PLATFORM IOS
#else //非IOS全部认为是CCObject*
#define PLATFORM ANDROID
#endif

 //自定义宏,与NetworkStatus相同,为了兼容android
typedef enum
{
	NotReach = 0,
	ReachableWiFi,
	ReachableWWAN
} NetStatus;

// sdk代理返回的对象类型,ios下是id类型,android用CCObject *代替
#if (PLATFORM == IOS)
	#define		OBJ_TYPE		id
#elif (PLATFORM == ANDROID)
	#define		OBJ_TYPE		CCObject*
#endif

//平台接口, 单纯接口,具体实现由具体平台下实现
class PlatformAdp : public CCObject
{
public:
	PlatformAdp();
	~PlatformAdp();
	
	/*
	 注释中的文件名是对应的文件中的调用
	 */
	
	//SGCantAdvanceBox SDK调用
	virtual void	pyUserSDKCallReq();
	
	//SGChangePasswordLayer SDK改密码调用
	//朋游SDK 密码修改
	virtual void	pyUserSDKChangePasswordReq(const char *userName, const char *oldPassword, const char *newPassword);
	
	//SGHelpLayer SDK登陆调用
	//朋游SDK调用
	virtual void	pyUserSDKLoginReq(const char *userName, const char *password);
	
	//SGMainManager SDK成员
	virtual void	SDKReqAlloc();
	virtual OBJ_TYPE	getSdkReqDelegate();
	//启动sdk(TalkingData)
	virtual void	startSDK();
	//设置等级
	virtual void	setSdkReqLevel(int lvl);
	//奖励领取
	virtual void	trackSDKReward(long moneyNum,const char *reason);
	//物品付费
	virtual void	trackSDKPurchase(const char *item,int num ,long price);
	//物品使用
	virtual void	trackSDKItemUse(const char *item,int num);
	//
	virtual void	trackSDKMsnBegin(const char *storyInfo);
	//
	virtual void	trackSDKMsnCompleted(const char *storyInfo);
	//
	virtual void	trackSDKMsnFailed(const char *storyInfo, const char *cause);
	//跟踪任务开始
	virtual void	trackSDKTaskBegin(const char *taskInfo);
	//跟踪任务完成情况
	virtual void	trackSDKTaskCompleted(const char *taskInfo);
	//付费成功
	virtual void	onChargeSuccessSDK(std::string &userId, long time, int randNum);
	
	
	//SGRegisterBox		SDK注册
	//通过sdk注册帐户
	virtual void	registerUserSDK(const char *userName, const char *password);
	//设置帐户
	virtual void	setAccountSDK(int userId, const char *userName);
	
	
	//SGServerlistNewServerBox
	//sdk登陆成功
	virtual void	okLoginSDK();
	
	
	//SGStoreLayer
	//付费
	virtual void	onChargeSDK(std::string &userId, long time, int randNum, const char *ipadId, int money, int virtualMoney);
	virtual void	gameIapStoreAlloc();
	virtual bool	isIapInstanceExist();
	//释放
	virtual void	iapInstnceRelease();
	//购买
	virtual void	iapBuyPayMent(int state);
	//苹果商店购买
	virtual void	iapBuyItem(const char *info);
	
	//获取udid
	virtual std::string getUdid();
	// 锁屏
	virtual void prohibitLockScreen();
	//检测网络状态
	virtual NetStatus checkNetWorkStatus();
	//获取文件完整路径
	virtual const char *longRangePath(const char *name, std::string filePath);
	//保存屏幕截图
	virtual void saveScreenImage(std::string fileFullPath);
	//高亮指定文本
	virtual cocos2d::ccColor3B colorWithHexString(const char *hexString);
	//版本
	virtual const char *bundleVersion();
	//是否包含emoji表情
	virtual bool isContainEmoji(const char *str);
	//删除log
	virtual void delLog();
	//将log写入xml(IOS下为plist)
	virtual void writeLogToXML(const char *logInfo);
	//弹出窗口
	virtual void showPopWindow(const char *showInfo);
	//获取可用内存
	virtual double getAvaliableMem();
	//使用的内存容量
	virtual double usedMem();
	//获取当前精确时间
	virtual const char *getTime();
	
    //打开指定URL。
    virtual void openUrl(std::string url);
};

#endif /* defined(__GameSanGuo__PlatformAdp__) */
