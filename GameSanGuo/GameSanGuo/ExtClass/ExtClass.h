//
//  ExtClass.h
//  GameSanGuo
//
//  Created by bugcode on 14-1-7.
//
//

#ifndef __GameSanGuo__ExtClass__
#define __GameSanGuo__ExtClass__

#include <iostream>

//扩展SDK，不同的SDK给出不同的实现，如初始化
class ExtClassOfSDK
{
private:
	ExtClassOfSDK();
	std::string userId;
	std::string sessionId;
	std::string nickName;
	//状态，留空，还未转换
	int channelId;
    
    //@使用中，非XDYOU的账号体系，是否已登录。
    bool isLogin_;
public:
	
	~ExtClassOfSDK();
	void initSDK();//初始化sdk平台
	static ExtClassOfSDK* sharedSDKInstance();//单例
	//充值
	/*
	 @order:订单号
	 @RMB：需要花费的钱数
	 @payDesc：支付描述,PP助手使用支付标题代替
	 @roleId: 角色ID，忽略填0
	 @channelId: 渠道ID
	 @zoneId: 分区ID，忽略填0
	 */
	void userPay(const char *order, float RMB, const char *payDesc,int channelId = 1);
    
    //@使用中，进入战斗后的细微处理。uc需要隐藏悬浮按钮。
    void battleBeginDealing();
    
    //@使用中，退出战斗后的细微处理，uc需要显示悬浮按钮。
    void battleEndDealing();
    
	//@使用中，购买成功后向游戏服务器发送消息
	void buySuccessAndSendMsg(const char *order, int channelId = 1, int isApp = 0);
	
    //@使用中，进行SDK登陆。tag=预留。
    void channelLogin(int tag = 0);
    
    //@使用中，进行SDK注册。tag=预留。
    void channelRegister(int tag = 0);
    
    //@使用中，退出游戏，部分SDK有特殊需求。
    void channelExitGame(int tag = 0);
    
    //@使用中，掉线，部分SDK有特殊需求。
    void channelStopConn(int tag = 0);
    
    //@使用中，设置TD的数据。
    void setTdData(std::string uid, std::string uname);
    
	//@使用中 是否完成SDK登陆。
	bool isLoginSDK();
    
    //@使用中 设置SDK登陆状态。
    void setIsLoginSDK(bool bl);
	
	std::string getUserId();
	std::string getSessionId();
	std::string getNickName();
	int getChannelId();
	//TB单独扩展，为了切换帐号
	void exchangeAccount();
	void enterCommunity();
	void sdkLogoutExitGame();
};


#endif /* defined(__GameSanGuo__ExtClass__) */
