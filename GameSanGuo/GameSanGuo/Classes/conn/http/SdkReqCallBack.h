//
//  SdkReqCallBack.h
//  OPUserSDK
//
//  Created by pengyou on 13-8-15.
//  Copyright (c) 2013年 OURPALM. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "GlobalConfig.h"

//由globalconfig中FACEBOOLK_OPEN_STATUS配置
//#define USEFACEBOOK 1


//#define USEHASOFFER 1
//#define USECHARTBOOST 1

#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
#import <FacebookSDK/FacebookSDK.h>
#endif

//#if (GAME_HASOFFER_STATUS == GHOS_HASOFFER_ON)
//#import <MobileAppTracker/MobileAppTracker.h>
//#import <AdSupport/AdSupport.h>
//#endif
//
//#if (GAME_HASOFFER_STATUS == GCBS_CHARTBOOST_ON)
//#import "Chartboost.h"
////#define CHARTBOOSTAPPID         @"53b617361873da4e4548b7d0"
////#define CHARTBOOSTAPPSIGNATURE  @"9046385fc3e2f340b2eecf114f7773676636063a"
//
//#define CHARTBOOSTAPPID         @"54af95bc04b0167e26cdb520"
//#define CHARTBOOSTAPPSIGNATURE  @"3b1a3119cb98dc0d214abfa2206a7396cce3baf5"
//#endif

#import "PYUserSDKLib.h"
#import "TalkingDataGA.h"

#define  PAYMENTTYPE   @"苹果官方"
#define  MONEYTTYPE   @"CNY"

@interface SdkReqCallBack : NSObject
{
    int m_ShareInfo;
}


@property (retain,readonly) NSString *ChannelId;
@property (retain,readonly) NSString *SubChannelId;
@property (retain,readonly) NSString *OsVersion;
@property (retain,readonly) NSString *OsType;

@property (retain,readonly) TDGAAccount *account;

@property (retain,readonly) NSString *SharePictureInfo;

#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
@property (strong, nonatomic) FBSession *session;
#endif

-(id)init;

- (void)registFinished:(ASIHTTPRequest *)request;

- (void)loginFinished:(ASIHTTPRequest *)request;

- (void)passwordFinished:(ASIHTTPRequest *)request;

- (void)requestFailed:(ASIHTTPRequest *)request;

- (void)destroyGuestFinished:(ASIHTTPRequest *)request;

- (void)invoiceFail:(ASIHTTPRequest *)request;

- (void)creatInvoiceFinished:(ASIHTTPRequest *)request;

- (void)receiveInvoiceFinished:(ASIHTTPRequest *)request;

#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)

- (void)creatFbSession;

- (void)startFbLogin;

- (void)updateView:(BOOL)needSetFBlogin;

- (void)logOutFBView:(int)value;

- (void)closeFBSession:(int)value;

- (void)publishStory;

- (void)postToWallFinished:(ASIHTTPRequest *)request;

- (void)getFacebookPhotoFinished:(ASIHTTPRequest *)request;

- (void)sendToPhotosFinished:(ASIHTTPRequest *)request;

- (void)requestPublishPermissions;

- (void)checkFbPermission;

- (void)loginForFBShare;

- (void)startFbShare:(int)shareInfo sharePictureInfo:(NSString *)sharePictureInfo;
#endif

#if (GAME_HASOFFER_STATUS == GHOS_HASOFFER_ON)
+ (void)onStartHasOffers;
#endif
/**
 *	@method	onStart     初始化统计实例 
 *	@param 	appId       appId           类型:NSString     应用的唯一标识，统计后台注册得到
 *  @param 	channelId   渠道名(可选)      类型:NSString     如“app store”
 */
+ (void)onStart;

/**
 *	@method	onEvent     自定义事件
 *	@param 	eventId     事件ID    类型:NSString
 *	@param 	eventData   事件参数   类型:键值对(key只支持NSString, value支持NSString和NSNumber)
 */


+ (void)onEvent:(NSString *)eventId eventData:(NSDictionary *)eventData;

/**
 *  @method setLatitude 设置经纬度
 *  @param  latitude    纬度      类型:double
 *  @param  longitude   经度      类型:double
 */
+ (void)setLatitude:(double)latitude longitude:(double)longitude;

/**
 *  @method setVerboseLogEnabled 设置记录日志  如要记录日志，应当最先调用该方法
 */
+ (void)setVerboseLogEnabled;

/**
 *	@method	setAccount  设置帐号
 *	@param 	accountId   帐号id    类型:NSString
 */
-(void)setAccount:(NSString *)accountId accountName:(NSString *)accountName accountType:(TDGAAccountType)accountType;

/**
 *	@method	setAccountName  设置帐号名称
 *	@param 	accountName     账户名称    类型:NSString
 */
- (void)setAccountName:(NSString *)accountName;

/**
 *	@method	setAccountType  设置帐号类型
 *	@param 	accountType     账户类型        类型TDGAAccountType
 */
- (void)setAccountType:(TDGAAccountType)accountType;

/**
 *	@method	setLevel    设置帐号等级
 *	@param 	level       升级之后的等级     类型:int
 */
- (void)setLevel:(int)level;

/**
 *	@method	setGender   设置性别
 *	@param 	gender      性别      类型:TDGAGender
 */
- (void)setGender:(TDGAGender)gender;

/**
 *	@method	setAge  设置年龄
 *	@param 	age     年龄      类型:int
 */
- (void)setAge:(int)age;

/**
 *	@method	setGameServer   设置区服
 *	@param  gameServer      区服      类型:NSString
 */
- (void)setGameServer:(NSString *)gameServer;

/**
 *	@method	onChargeRequst          虚拟币充值请求
 *	@param 	orderId                 订单id        类型:NSString
 *	@param 	iapId                   充值包id      类型:NSString
 *	@param 	currencyAmount          现金金额      类型:double
 *	@param 	currencyType            币种          类型:NSString
 *	@param 	virtualCurrencyAmount   虚拟币金额    类型:double
 *	@param 	paymentType             支付类型      类型:NSString
 */
+ (void)onChargeRequst:(NSString *)orderId
                 iapId:(NSString *)iapId
        currencyAmount:(double)currencyAmount
          currencyType:(NSString *)currencyType
 virtualCurrencyAmount:(double)virtualCurrencyAmount
           paymentType:(NSString *)paymentType;

/**
 *	@method	onChargeRequst          虚拟币充值请求
 *	@param 	orderId                 订单id        类型:NSString
 */
+ (void)onChargeSuccess:(NSString *)orderId;

/**
 *  @method onReward                虚拟币赠送
 *  @param  virtualCurrencyAmount   虚拟币金额         类型:double
 *  @param  reason                  赠送虚拟币的原因    类型:NSString
 */
+ (void)onReward:(double)virtualCurrencyAmount reason:(NSString *)reason;

/**
 *	@method	onPurchase  虚拟物品购买
 *	@param 	item        道具           类型:NSString
 *	@param 	number      道具个数        类型:int
 *	@param 	price       道具单价        类型:double
 */
+ (void)onPurchase:(NSString *)item itemNumber:(int)number priceInVirtualCurrency:(double)price;

/**
 *	@method	onPurchase  虚拟物品消耗
 *	@param 	item        道具           类型:NSString
 *	@param 	number      道具个数        类型:int
 */
+ (void)onUse:(NSString *)item itemNumber:(int)number;

/**
 *	@method	onBegin     开始一项任务
 *	@param 	missionId   任务名称    类型:NSString
 */
+ (void)onBegin:(NSString *)missionId;

/**
 *	@method	onCompleted 完成一项任务
 *	@param 	missionId   任务名称    类型:NSString
 */
+ (void)onCompleted:(NSString *)missionId;

/**
 *	@method	onFailed    一项任务失败
 *	@param 	missionId   任务名称    类型:NSString
 *	@param 	failedCause 失败原因    类型:NSString
 */
+ (void)onFailed:(NSString *)missionId failedCause:(NSString *)cause;

#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)


#endif

@end
