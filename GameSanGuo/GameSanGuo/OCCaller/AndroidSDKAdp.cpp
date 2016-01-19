#include <iostream>
#include "AndroidSDKAdp.h"
#include "TDCCAccount.h"

#define MONEY_TYPE "CNY"
#define PAY_TYPE    "GOOGLE_WALLET"

//CLASS目录名
#define ADR_PACKAGE_NAME        "com/xdyou/sanguo/"

/*
#define ADR_PACKAGE_NAME        "com/generalmobi/hero/tw/"

//如果google play发布，使用com/generalmobi/hero/tw/包
#if (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY)//正式google play
    //定义包名字符串 for google play
    #define ADR_PACKAGE_NAME        "com/generalmobi/hero/tw/"

//如果等于第三方 渠道， android
#elif (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_THIRD)//正式第三方
    //定义包名字符串 for third package
    #define ADR_PACKAGE_NAME        "com/generalmobi/herothird/tw/"

//uc android
#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD)
	#define ADR_PACKAGE_NAME		"com/xdyou/sanguo/uc/"

//内外网测试，非正式
#elif ( GAME_CHANNEL_TYPE == GCCT_TEST_ADR_IN || GAME_CHANNEL_TYPE == GCCT_TEST_ADR_EX )
    //定义包名字符串 for 测试外网
    #define ADR_PACKAGE_NAME        "com/generalmobi/herotest/tw/"

//其他默认内外网测试
#else
	#define ADR_PACKAGE_NAME		"com/generalmobi/herotest/tw/"

#endif
*/

AndroidSDKAdp::AndroidSDKAdp()
{
	account = NULL;
}

AndroidSDKAdp::~AndroidSDKAdp()
{

}

static AndroidSDKAdp *instance = NULL;

//单例
AndroidSDKAdp *AndroidSDKAdp::getInstance()
{
	if (!instance)
	{
		instance = new AndroidSDKAdp();
	}
	return instance;
}

//获取包名路径
std::string AndroidSDKAdp::getPackageName(const char *name)
{
    std::string fileName(name);
    std::string packageName(ADR_PACKAGE_NAME + fileName);
    CCLOG("packageName == [%s]", packageName.c_str());
    return packageName;
}


//获取SDK是否启动
bool AndroidSDKAdp::sdkIsStarted()
{
	bool isStarted = false;
    
	JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("SDKCaller").c_str(), "sdkIsStarted", "()Z");

	if (isHave)
	{
		isStarted = (bool)mtdInfo.env->CallStaticBooleanMethod(mtdInfo.classID, mtdInfo.methodID);
		CCLOG("gamesanguo -- call sdkIsStarted success! [isStarted == %d]", isStarted);
	}
	else
	{
		CCLOG("gamesanguo -- [sdkIsStarted] funtion not found!");
	}
	return isStarted;
}

//获取余额
int AndroidSDKAdp::getCurrentBalance()
{
	int balance = 0;

	JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("SDKCaller").c_str(), "getCurrentBalance", "()I");
	if (isHave)
	{
		balance = (int)mtdInfo.env->CallStaticIntMethod(mtdInfo.classID, mtdInfo.methodID);
		CCLOG("go2play -- call getCurrentBalance success! [balance == %d]", balance);
	}
	else
	{
		CCLOG("go2play -- [getCurrentBalance] function not found!");
	}
	return balance;
}

//充值操作
void AndroidSDKAdp::charge(std::string chargeId, int payment, int serverId, std::string accountId, int roleId)
{
	JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("SDKCaller").c_str(), "charge", "(Ljava/lang/String;IILjava/lang/String;I)V");

	if (isHave)
	{
		jstring jniChargeId = mtdInfo.env->NewStringUTF(chargeId.c_str());
		jstring jniServerId = mtdInfo.env->NewStringUTF(accountId.c_str());
		CCLOG("go2play -- chargeId == %s -- payment == %d serverId=%d, accountId=%s, roleId=%d", chargeId.c_str(), payment, serverId, accountId.c_str(), roleId);
		mtdInfo.env->CallStaticVoidMethod(mtdInfo.classID, mtdInfo.methodID, jniChargeId, payment, serverId, jniServerId, roleId);
		CCLOG("go2play -- call charge success!");
	}
	else
	{
		CCLOG("go2play -- [charge] function not found!");
	}
}

//注册操作
std::string AndroidSDKAdp::registerUser(std::string userName, std::string password)
{
	//sdk注册成功后返回的userId
	std::string regInfo = "";

	JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("SDKCaller").c_str(), "register", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

	if (isHave)
	{
		//转换为jni的包装string
		jstring jniUserName = mtdInfo.env->NewStringUTF(userName.c_str());
		jstring jniPassword = mtdInfo.env->NewStringUTF(password.c_str());

		//调用java的注册方法
		jstring jniRegInfo = (jstring)mtdInfo.env->CallStaticObjectMethod(mtdInfo.classID, mtdInfo.methodID, jniUserName, jniPassword);

		//转为可用char*字符串
		const char *charRegString = mtdInfo.env->GetStringUTFChars(jniRegInfo, 0);

		std::string temp(charRegString);
		regInfo = temp;

		//减引用计数,防止内存泄露
		mtdInfo.env->ReleaseStringUTFChars(jniRegInfo, charRegString);

		CCLOG("samesanguo -- call register success! RegInfo == %s", regInfo.c_str());
	}
	else
	{
		CCLOG("gamesanguo -- [register] function not found!");
	}
	return regInfo;
}

//sdk登陆
std::string AndroidSDKAdp::login(std::string userName, std::string password)
{
	//sdk登陆成功后返回的userId
	std::string loginInfo = "";

	JniMethodInfo mtdInfo;
	bool isHave =JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("SDKCaller").c_str(), "login", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

	if (isHave)
	{
		//传入的用户名和密码
		jstring jniUserName = mtdInfo.env->NewStringUTF(userName.c_str());
		jstring jniPassword = mtdInfo.env->NewStringUTF(password.c_str());

		//调用sdk的登陆方法并返回, 如果登陆成功,返回当前登陆的用户名的对应的uerId
		jstring jniLoginInfo = (jstring)mtdInfo.env->CallStaticObjectMethod(mtdInfo.classID, mtdInfo.methodID, jniUserName, jniPassword);

		//转换为C形式的字符串
		const char *loginCString = mtdInfo.env->GetStringUTFChars(jniLoginInfo, 0);
		std::string temp(loginCString);
		loginInfo = temp;

		//释放刚才获取的字符kkhgk
		mtdInfo.env->ReleaseStringUTFChars(jniLoginInfo, loginCString);
	}
	else
	{
		CCLOG("gamesanguo -- [login] fuction not found!");
	}

	return loginInfo;
}

//FB登陆g2p版
void AndroidSDKAdp::fbLoginGo2play()
{
	JniMethodInfo mtdInfo;
	bool isHave =JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("SDKCaller").c_str(), "fbLogin", "()V");

	if (isHave)
	{
		//调用sdk的fb登陆方法。
		mtdInfo.env->CallStaticVoidMethod(mtdInfo.classID, mtdInfo.methodID);
	}
	else
	{
		CCLOG("gamesanguo -- [fbLogin] fuction not found!");
	}

	return;
}

//FB登出g2p版
void AndroidSDKAdp::fbLogoutGo2play()
{
	JniMethodInfo mtdInfo;
	bool isHave =JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("SDKCaller").c_str(), "fbLogout", "()V");

	if (isHave)
	{
		//调用sdk的fb登出方法。
		mtdInfo.env->CallStaticVoidMethod(mtdInfo.classID, mtdInfo.methodID);
	}
	else
	{
		CCLOG("gamesanguo -- [fbLogout] fuction not found!");
	}

	return;
}

//登陆游戏
void AndroidSDKAdp::loginGame(std::string uname, std::string pwd)
{
	JniMethodInfo mtdInfo;
	bool isHave =JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("SDKCaller").c_str(), "loginGame", "(Ljava/lang/String;Ljava/lang/String;)V");

	if (isHave)
	{
		//传入的用户名和密码。
		jstring jniUserName = mtdInfo.env->NewStringUTF(uname.c_str());
		jstring jniPassword = mtdInfo.env->NewStringUTF(pwd.c_str());

		//调用sdk的登陆方法。
		mtdInfo.env->CallStaticVoidMethod(mtdInfo.classID, mtdInfo.methodID, jniUserName, jniPassword);
	}
	else
	{
		CCLOG("gamesanguo -- [loginGame] fuction not found!");
	}

	return;
}

//快速登陆游戏
void AndroidSDKAdp::fastLoginGame()
{
	JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("SDKCaller").c_str(), "connect", "()Ljava/lang/String;");

	if (isHave)
	{
		//调用sdk中的java方法, 尝试快速登陆。
		mtdInfo.env->CallStaticObjectMethod(mtdInfo.classID, mtdInfo.methodID);
	}
	else
	{
		CCLOG("gamesanguo -- [fastLoginGame] function not found!");
	}
	return ;
}

//检测用户名是否存在
bool AndroidSDKAdp::checkUser(std::string userName)
{
	bool isExist = true;

	JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("SDKCaller").c_str(), "checkUser", "(Ljava/lang/String;)Z");

	if (isHave)
	{
		jstring jniUserName = mtdInfo.env->NewStringUTF(userName.c_str());

		//调用sdk中的java方法, 返回是是否帐号存在
		isExist = (bool)mtdInfo.env->CallStaticBooleanMethod(mtdInfo.classID, mtdInfo.methodID, jniUserName);

	}
	else
	{
		CCLOG("gamesanguo -- [checkUser] function not found!");
	}
	return isExist;
}

//facebook 相关
//facebook 登陆
bool AndroidSDKAdp::fbLogin()
{
	bool success = false;

	JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("GameSanGuo").c_str(), "callFBLogin", "()V");

	if (isHave)
	{
		//调用sdk中的java方法,
		mtdInfo.env->CallStaticBooleanMethod(mtdInfo.classID, mtdInfo.methodID);
		success = true;
	}
	else
	{
		CCLOG("gamesanguo -- [fbLogin] function not found!");
	}
	return success;
}
//facebook 登出
void AndroidSDKAdp::fbLogout()
{
	JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("GameSanGuo").c_str(), "callFBLogout", "()V");

	if (isHave)
	{
		//调用sdk中的java方法, 返回是是否帐号存在
		mtdInfo.env->CallStaticVoidMethod(mtdInfo.classID, mtdInfo.methodID);
	}
	else
	{
		CCLOG("gamesanguo -- [fbLogout] function not found!");
	}
	return ;
}
//facebook 分享
void AndroidSDKAdp::fbShareGame(std::string imageName, std::string message)
{
    JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("GameSanGuo").c_str(), "callFBShare", "(Ljava/lang/String;Ljava/lang/String;)V");
    
	if (isHave)
	{
		//调用sdk中的java方法, 返回是是否帐号存在
        jstring jstrImage = mtdInfo.env->NewStringUTF(imageName.c_str());
        jstring jstrMessage = mtdInfo.env->NewStringUTF(message.c_str());
		mtdInfo.env->CallStaticVoidMethod(mtdInfo.classID, mtdInfo.methodID, jstrImage, jstrMessage);
	}
	else
	{
		CCLOG("gamesanguo -- [callFBShare] function not found!");
	}
	return ;
}

void AndroidSDKAdp::callFBCreateSession()
{
    JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("GameSanGuo").c_str(), "callFBCreateSession", "()V");
    
	if (isHave)
	{
		//调用sdk中的java方法, 调用创建facebook 的session
		mtdInfo.env->CallStaticVoidMethod(mtdInfo.classID, mtdInfo.methodID);
	}
	else
	{
		CCLOG("gamesanguo -- [callFBCreateSession] function not found!");
	}
	return ;
}
void AndroidSDKAdp::callAppsFlyerLoginTracker(std::string userId)
{
    //appsFlyerLoginTracker
    JniMethodInfo mtdInfo;
    std::string package(getPackageName("GameSanGuo"));
    CCLOG("appFlyer == %s", package.c_str());
    bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, package.c_str(), "appsFlyerLoginTracker", "(Ljava/lang/String;)V");
    if (isHave)
    {
        jstring jniUserId = mtdInfo.env->NewStringUTF(userId.c_str());
        CCLOG("gamesanguo -- [callAppsFlyerLoginTracker] called");
        mtdInfo.env->CallStaticVoidMethod(mtdInfo.classID, mtdInfo.methodID, jniUserId);
    }
    else
    {
        CCLOG("gamesanguo -- [appsFlyerLoginTracker] function not found!");
    }
}


//for talkingdata

//启动sdk(TalkingData)
void AndroidSDKAdp::startSDK()
{
	const char *talkingDataId = "191F6CA0284EFD11DCF6E8AAB81851D7";
	const char *talkingDataChannelId = "go2play";
//	TDCCTalkingDataGA::onStart(talkingDataId, talkingDataChannelId);
}
//设置accountId
void AndroidSDKAdp::setAccountSDK(std::string userId, const char *userName)
{
	account = TDCCAccount::setAccount(userId.c_str());
    //设置区服务器
    std::string gameServer = CCUserDefault::sharedUserDefault()->getStringForKey("servername");
	account->setAccountName(userName);
    account->setGameServer(gameServer.c_str());
    account->setAccountType(TDCCAccount::kAccountRegistered);
}
//获取account对象
TDCCAccount *AndroidSDKAdp::getAccount()
{
	if (!account)
	{
		std::string userId = CCUserDefault::sharedUserDefault()->getStringForKey("userId");
		std::string userName = CCUserDefault::sharedUserDefault()->getStringForKey("username");
        //设置区服务器
        std::string gameServer = CCUserDefault::sharedUserDefault()->getStringForKey("servername");
        
		account = TDCCAccount::setAccount(userId.c_str());
		account->setAccountName(userName.c_str());
        account->setGameServer(gameServer.c_str());
	}

	return account;
}
//设置等级
void AndroidSDKAdp::setSdkReqLevel(int lvl)
{
	CCLOG("gamesanguo -- talking data set level!");
	this->getAccount()->setLevel(lvl);
}
//奖励领取
void AndroidSDKAdp::trackSDKReward(long moneyNum,const char *reason)
{
	TDCCVirtualCurrency::onReward(moneyNum, reason);
}
//物品付费
void AndroidSDKAdp::trackSDKPurchase(const char *item,int num ,long price)
{
	TDCCItem::onPurchase(item, num, price);
}
//物品使用
void AndroidSDKAdp::trackSDKItemUse(const char *item,int num)
{
	TDCCItem::onUse(item, num);
}
//
void AndroidSDKAdp::trackSDKMsnBegin(const char *storyInfo)
{
	TDCCMission::onBegin(storyInfo);
}
//
void AndroidSDKAdp::trackSDKMsnCompleted(const char *storyInfo)
{
	TDCCMission::onCompleted(storyInfo);
}
//
void AndroidSDKAdp::trackSDKMsnFailed(const char *storyInfo, const char *cause)
{
	TDCCMission::onFailed(storyInfo, cause);
}
//跟踪任务开始
void AndroidSDKAdp::trackSDKTaskBegin(const char *taskInfo)
{
	TDCCMission::onBegin(taskInfo);
}
//跟踪任务完成情况
void AndroidSDKAdp::trackSDKTaskCompleted(const char *taskInfo)
{
	TDCCMission::onCompleted(taskInfo);
}
//发送购买消息的同时，也向talkingdata发送购买消息
void  AndroidSDKAdp::onChargeSDK(std::string &userId, long time, int randNum, const char *ipadId, int money, int virtualMoney)
{
    CCString *orderId = CCString::createWithFormat("%s", userId.c_str());
    CCLOG("向talkingdata发送消费信息 订单号 [%s]", orderId->getCString());

    //TDCCVirtualCurrency::onChargeRequest(orderId->getCString(), ipadId, money, MONEY_TYPE, virtualMoney, PAY_TYPE);
    
    CCLOG("向talkingdata发送消费信息完成 [%s-%s-%d]", orderId->getCString(), ipadId, money);

}

//付费成功后talkingdata调用
void AndroidSDKAdp::onChargeSuccessSDK(std::string &userId, long time, int randNum)
{
    CCLOG("充值成功 订单号 [%s]", userId.c_str());
    //TDCCVirtualCurrency::onChargeSuccess(CCString::createWithFormat("%s", userId.c_str())->getCString());
}

//callGotoGoolePlay
void AndroidSDKAdp::enterGooglePlay()
{
    JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("GameSanGuo").c_str(), "callGotoGoolePlay", "()V");
    
	if (isHave)
	{
		//调用java， 进入googleplay的方法
        CCLOG("gamesanguo -- [enterGooglePlay] found");
		mtdInfo.env->CallStaticVoidMethod(mtdInfo.classID, mtdInfo.methodID);
	}
	else
	{
		CCLOG("gamesanguo -- [enterGooglePlay] function not found!");
	}
	return ;
}

//打开指定URL
void AndroidSDKAdp::openUrl(std::string url)
{
	JniMethodInfo mtdInfo;
	bool isHave = JniHelper::getStaticMethodInfo(mtdInfo, getPackageName("GameSanGuo").c_str(), "callOpenUrl", "(Ljava/lang/String;)V");
    
	if (isHave)
	{
		//调用java， 打开指定URL。
        CCLOG("gamesanguo -- [callOpenUrl] found");
        jstring jurl = mtdInfo.env->NewStringUTF(url.c_str());
		mtdInfo.env->CallStaticVoidMethod(mtdInfo.classID, mtdInfo.methodID, jurl);
	}
	else
	{
		CCLOG("gamesanguo -- [callOpenUrl] function not found!");
	}
	return ;
}

