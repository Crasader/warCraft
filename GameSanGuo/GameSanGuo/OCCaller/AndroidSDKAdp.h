
//
// 	2014-6-25 by bugcode
//	Jni桥接类, 由c++调用java的sdk接口
//

#ifndef _ANDROID_SDK_ADP_H_
#define _ANDROID_SDK_ADP_H_

#include "PlatformAdp.h"	//for PLATFORM Macro
#include <string>
#include "cocos2d.h"
#include "TDCCTalkingDataGA.h"
#include "TDCCVirtualCurrency.h"
#include "TDCCItem.h"
#include "TDCCMission.h"
#include "TDCCAccount.h"


using namespace cocos2d;

#if (PLATFORM == ANDROID)
#include "platform/android/jni/JniHelper.h" //for Jni call
#include "GlobalConfig.h"


class AndroidSDKAdp : public PlatformAdp
{
private:
	AndroidSDKAdp();
	TDCCAccount *account;
public:
	~AndroidSDKAdp();
	static AndroidSDKAdp *getInstance();

	//通过jni调用java方法的sdk的函数
	TDCCAccount *getAccount();

	//sdk是否启动
	bool sdkIsStarted();
	//获取余额
	int getCurrentBalance();
	//充值
	//充值ID与充值金额
	void charge(std::string chargeId, int payment, int serverId, std::string accountId, int roleId);
	//注册账号
	std::string registerUser(std::string userName, std::string password);
	//登陆账号
	std::string login(std::string userName, std::string password);
	//FB登陆g2p版
	void fbLoginGo2play();
	//FB登出g2p版
	void fbLogoutGo2play();
	//登陆游戏
	void loginGame(std::string uname, std::string pwd);
	//快速登陆游戏
	void fastLoginGame();

	//检测用户名
	bool checkUser(std::string userName);

	//facebook 登陆
	bool fbLogin();
	//facebook 登出
	void fbLogout();
	//facebook 分享
    void fbShareGame(std::string imageName, std::string message);
    //创建登陆FB的session
    void callFBCreateSession();
    
    //appflyer登陆跟踪
    void callAppsFlyerLoginTracker(std::string userId);

    //获取android的包名， 在不同的发布渠道下
    std::string getPackageName(const char *name);


	/*
	 注释中的文件名是对应的文件中的调用
	 */

	//SGCantAdvanceBox SDK调用
	virtual void	pyUserSDKCallReq(){};

	//SGChangePasswordLayer SDK改密码调用
	//朋游SDK 密码修改
	virtual void	pyUserSDKChangePasswordReq(const char *userName, const char *oldPassword, const char *newPassword){};

	//SGHelpLayer SDK登陆调用
	//朋游SDK调用
	virtual void	pyUserSDKLoginReq(const char *userName, const char *password){};

	//SGMainManager SDK成员
	virtual void	SDKReqAlloc(){};
	virtual OBJ_TYPE	getSdkReqDelegate(){};
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
	virtual void	registerUserSDK(const char *userName, const char *password){};
	//设置帐户
	virtual void	setAccountSDK(std::string userId, const char *userName);


	//SGServerlistNewServerBox
	//sdk登陆成功
	virtual void	okLoginSDK(){};


	//SGStoreLayer
	//付费
	virtual void	onChargeSDK(std::string &userId, long time, int randNum, const char *ipadId, int money, int virtualMoney);
	virtual void	gameIapStoreAlloc(){};
	virtual bool	isIapInstanceExist(){};
	//释放
	virtual void	iapInstnceRelease(){};
	//购买
	virtual void	iapBuyPayMent(int state){};
	//苹果商店购买
	virtual void	iapBuyItem(const char *info){};

	//获取udid
	virtual std::string getUdid(){};
	// 锁屏
	virtual void prohibitLockScreen(){};
	//检测网络状态
	virtual NetStatus checkNetWorkStatus(){};
	//获取文件完整路径
	virtual const char *longRangePath(const char *name, std::string filePath){};
	//保存屏幕截图
	virtual void saveScreenImage(std::string fileFullPath){};
	//高亮指定文本
	virtual cocos2d::ccColor3B colorWithHexString(const char *hexString){};
	//版本
	virtual const char *bundleVersion(){};
	//是否包含emoji表情
	virtual bool isContainEmoji(const char *str){};
	//删除log
	virtual void delLog(){};
	//将log写入xml(IOS下为plist)
	virtual void writeLogToXML(const char *logInfo){};
	//弹出窗口
	virtual void showPopWindow(const char *showInfo){};
	//获取可用内存
	virtual double getAvaliableMem(){};
	//使用的内存容量
	virtual double usedMem(){};
	//获取当前精确时间
	virtual const char *getTime(){};

    //进入google play
    void enterGooglePlay();

    //打开指定URL。
    virtual void openUrl(std::string url);

};

#endif















#endif


