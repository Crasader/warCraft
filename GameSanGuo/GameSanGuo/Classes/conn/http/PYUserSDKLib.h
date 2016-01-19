//
//  PYUserSDKLib.h
//  PYUserSDKLib
//
//  Created by pengyou on 13-8-15.
//  Copyright (c) 2013年 pengyou. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "ASIHTTPRequest.h"

@interface PYUserSDKLib : NSObject{
    
}

/**
 *注册userRegister
 *@UserName：用户名
 *@Password：密码
 *@GameId：游戏编号
 *@ChannelId：渠道编号
 *@SubChannelId：子渠道编号
 *@OsVersion：操作系统版本信息
 *@OsType：操作系统类型
 *@PhoneNum：联系电话
 *@CallBackDelegate：回调的Delegate实例
 *@FinishSelector：回调实例的方法
 *@FailSelector：回调实例的方法
 **/
- (void)userRegister:(NSString*)UserName Password:(NSString*)Password GameId:(NSString*)GameId ChannelId:(NSString*)ChannelId SubChannelId:(NSString*)SubChannelId OsVersion:(NSString*)OsVersion OsType:(NSString*)OsType PhoneNum:(NSString*)PhoneNum CallBackDelegate:(id)CallBackDelegate FinishSelector:(SEL)finishSelector FailSelector:(SEL)failSelector;

/**
 *登陆userLogin
 *@UserName：用户名
 *@Password：密码
 *@GameId：游戏编号
 *@ChannelId：渠道编号
 *@SubChannelId：子渠道编号
 *@OsVersion：操作系统版本信息
 *@OsType：操作系统类型
 *@CallBackDelegate：回调的Delegate实例
 *@FinishSelector：回调实例的方法
 *@FailSelector：回调实例的方法
 **/
-(void) userLogin:(NSString*)UserName Password:(NSString*)Password GameId:(NSString*)GameId ChannelId:(NSString*)ChannelId SubChannelId:(NSString*)SubChannelId OsVersion:(NSString*)OsVersion OsType:(NSString*)OsType CallBackDelegate:(id)CallBackDelegate FinishSelector:(SEL)finishSelector FailSelector:(SEL)failSelector;

/**
 *退出userLogout
 *@UserId：用户id
 *@GameId：游戏编号
 *@ChannelId：渠道编号
 *@SubChannelId：子渠道编号
 *@OsVersion：操作系统版本信息
 *@OsType：操作系统类型
 *@CallBackDelegate：回调的Delegate实例
 *@FinishSelector：回调实例的方法
 *@FailSelector：回调实例的方法
 **/
-(void) userLogout:(NSString*)UserId GameId:(NSString*)GameId ChannelId:(NSString*)ChannelId SubChannelId:(NSString*)SubChannelId OsVersion:(NSString*)OsVersion OsType:(NSString*)OsType CallBackDelegate:(id)CallBackDelegate FinishSelector:(SEL)finishSelector FailSelector:(SEL)failSelector;

/**
 *修改密码modifyPassword
 *@UserName：用户名
 *@Password：密码
 *@GameId：游戏编号
 *@ChannelId：渠道编号
 *@SubChannelId：子渠道编号
 *@OsVersion：操作系统版本信息
 *@OsType：操作系统类型
 *@NewPassword：新密码
 *@CallBackDelegate：回调的Delegate实例
 *@FinishSelector：回调实例的方法
 *@FailSelector：回调实例的方法
 **/
-(void) modifyPassword:(NSString*)UserName Password:(NSString*)Password GameId:(NSString*)GameId ChannelId:(NSString*)ChannelId SubChannelId:(NSString*)SubChannelId OsVersion:(NSString*)OsVersion OsType:(NSString*)OsType NewPassword:(NSString*)NewPassword CallBackDelegate:(id)CallBackDelegate FinishSelector:(SEL)finishSelector FailSelector:(SEL)failSelector;

/**
 *一键登陆oKLogin
 *@GameId：游戏编号
 *@ChannelId：渠道编号
 *@SubChannelId：子渠道编号
 *@OsVersion：操作系统版本信息
 *@OsType：操作系统类型
 *@CallBackDelegate：回调的Delegate实例
 *@FinishSelector：回调实例的方法
 *@FailSelector：回调实例的方法
 **/
-(void) oKLogin:(NSString*)GameId ChannelId:(NSString*)ChannelId SubChannelId:(NSString*)SubChannelId OsVersion:(NSString*)OsVersion OsType:(NSString*)OsType CallBackDelegate:(id)CallBackDelegate FinishSelector:(SEL)finishSelector FailSelector:(SEL)failSelector;

/**
 *检测用户是否存在checkUserName
 *@UserName：用户名
 *@CallBackDelegate：回调的Delegate实例
 *@FinishSelector：回调实例的方法
 *@FailSelector：回调实例的方法
 **/
-(void) checkUserName:(NSString*)UserName CallBackDelegate:(id)CallBackDelegate FinishSelector:(SEL)finishSelector FailSelector:(SEL)failSelector;

/**
 *激活游戏activite
 *@GameId：游戏编号
 *@ChannelId：渠道编号
 *@SubChannelId：子渠道编号
 *@OsVersion：操作系统版本信息
 *@OsType：操作系统类型
 *@CallBackDelegate：回调的Delegate实例
 *@FinishSelector：回调实例的方法
 *@FailSelector：回调实例的方法
 **/
-(void) activite:(NSString*)GameId ChannelId:(NSString*)ChannelId SubChannelId:(NSString*)SubChannelId OsVersion:(NSString*)OsVersion OsType:(NSString*)OsType CallBackDelegate:(id)CallBackDelegate FinishSelector:(SEL)finishSelector FailSelector:(SEL)failSelector;

/**
 *消除游客账号destroyGuest
 *@GameId：游戏编号
 *@ChannelId：渠道编号
 *@SubChannelId：子渠道编号
 *@OsVersion：操作系统版本信息
 *@OsType：操作系统类型
 *@CallBackDelegate：回调的Delegate实例
 *@FinishSelector：回调实例的方法
 *@FailSelector：回调实例的方法
 **/
-(void) destroyGuest:(NSString*)GameId ChannelId:(NSString*)ChannelId CallBackDelegate:(id)CallBackDelegate FinishSelector:(SEL)finishSelector FailSelector:(SEL)failSelector;




/**
 *第三方登陆userThirdPartyLogin
 *@UserName：用户名
 *@Password：密码
 *@GameId：游戏编号
 *@ChannelId：渠道编号
 *@SubChannelId：子渠道编号
 *@OsVersion：操作系统版本信息
 *@OsType：操作系统类型
 *@CallBackDelegate：回调的Delegate实例
 *@FinishSelector：回调实例的方法
 *@FailSelector：回调实例的方法
 **/
-(void) userThirdPartyLogin:(NSString*)UserName Password:(NSString*)Password GameId:(NSString*)GameId ChannelId:(NSString*)ChannelId SubChannelId:(NSString*)SubChannelId OsVersion:(NSString*)OsVersion OsType:(NSString*)OsType  CallBackDelegate:(id)CallBackDelegate FinishSelector:(SEL)finishSelector FailSelector:(SEL)failSelector;
/**
 *获得单件sharedUserSDKLib
 *返回单件实例
 **/
+ (PYUserSDKLib*)sharedUserSDKLib;

/**
 *Key校验md5Encrypt
 *@inPutText：需要加密字串
 *返回加密字串
 ***/
-(NSString *) md5Encrypt:(NSString *)inPutText;

-(bool) generateUserKey:(NSString *)gameId ChannelId:(NSString*)ChannelId;

-(NSString *) getIDKC:(NSString*)GameId ChannelId:(NSString*)ChannelId ;

@end
