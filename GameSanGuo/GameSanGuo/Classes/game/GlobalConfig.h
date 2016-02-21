//
//  GlobalConfig.h
//  GameSanGuo
//
//  Created by Fygo Woo on 10/11/14.
//
//  @description 全球化配置，以后可能会将所有的URL/文字/适配等都会整合到这里。

#ifndef __GameSanGuo__GlobalConfig__
#define __GameSanGuo__GlobalConfig__

#include "cocos2d.h"


#define CGPTest 0  //是否使用测试程序。 1：直接进入TestScene场景。0：正常场景
#define AllTaskOpen 0
#define LogSbData 0
#define newServerData 0

//cgpADR	
#define CGP_OTH  0
#define CGP_IOS  1
#define CGP_ADR  2



#define CGP_PLATFORM   CGP_IOS


/*
 * 脸书开关。
 * 是否开启facebook
 */
//使用facebook
#define FACEBOOK_ON     1
//不使用facebook
#define FACEBOOK_OFF    0



/*
 * 各类广告开关，历史遗留问题。
 */
#define GHOS_HASOFFER_ON 1
#define GHOS_HASOFFER_OFF 0

#define GCBS_CHARTBOOST_ON 1
#define GCBS_CHARTBOOST_OFF 0

#define GGCS_GOCPA_ON 1
#define GGCS_GOCPA_OFF 0

#define GAFS_APPSFLYER_ON 1
#define GAFS_APPSFLYER_OFF 0

/*
 * 大小包标识。
 */
//全包
#define GCPV_FULL_PACKAGE 1
//迷你包
#define GCPV_MINI_PACKAGE 0

/*
 * 发布模式。
 * 测试与正式的区分，历史原因，测试渠道的内外网和正式渠道都有1和2这两个值，还得继续细分。
 */
//内网测试
#define GCPM_INTERNAL_TEST_MODE 789
//外网测试
#define GCPM_EXTERNAL_TEST_MODE 897
//正式发布
#define GCPM_OFFICAL_RELEASE_MODE 978

/* 
 * 渠道设置。
 * 以国家和渠道名联合区分，规则：GCCT_地区_发行商_系统平台_渠道
 * ADR=android，IOS=IOS。
 */
//测试渠道>>
//内网测试IOS
#define GCCT_TEST_IOS_IN 1
//内网测试ADR
#define GCCT_TEST_ADR_IN 2
//外网测试IOS
#define GCCT_TEST_IOS_EX 1
//外网测试ADR
#define GCCT_TEST_ADR_EX 2
//正式渠道>>
//台湾 go2play IOS 苹果APPSTORE
#define GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE 1
//台湾 go2play ADR 谷歌PLAY
#define GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY 2
//台湾 go2play ADR 第三方
#define GCCT_TAIWAN_GO2PLAY_ADR_THIRD 3
//大陆 xdyou IOS 苹果APPSTORE
#define GCCT_CHINA_XDYOU_IOS_APPLE_APPSTORE 11
//大陆 xdyou ADR 未知
#define GCCT_CHINA_XDYOU_ADR_UFO 12
//大陆 uc ADR 第三方
#define GCCT_CHINA_UC_ADR_THIRD 13
//大陆 360 ADR 第三方
#define GCCT_CHINA_QIHU_ADR_THIRD 14
//大陆 快用 IOS 第三方
#define GCCT_CHINA_KUAIYONG_IOS_THIRD 15
//大陆 同步推 IOS 越狱（保留）
#define GCCT_CHINA_TONGBUTUI_IOS_THIRD 100
//大陆 PP助手 IOS 第三方（保留）
#define GCCT_CHINA_PP_IOS_THIRD 101
//泰国
#define GCCT_THAILAND 200
//火星
#define GCCT_MARS 300



/*********************                 ********************/
/* !!!!!!!!!!         发版请设置下方即可         !!!!!!!!!! */
/* !!!!!!!!!!         怕你找不到        !!!!!!!!!!!!!!!!!!!*/
/******************                 ***********************/


//发布模式
#define GAME_PUBLISH_MODE       GCPM_OFFICAL_RELEASE_MODE

//渠道代号

#if (CGP_PLATFORM == CGP_IOS)
//    #define GAME_CHANNEL_TYPE       GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE  //ios
//    //游戏版本
//    #define GAME_CLIENT_VERSION     "4.2.5"
//    //资源版本
//    #define GAME_RESOURCE_VERSION   "128"

#define GAME_CHANNEL_TYPE       GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE  //ios
//游戏版本
#define GAME_CLIENT_VERSION     "4.2.3"
//资源版本
#define GAME_RESOURCE_VERSION   "129"


#elif (CGP_PLATFORM == CGP_ADR)
    #define GAME_CHANNEL_TYPE       GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY  //android
    //游戏版本
    #define GAME_CLIENT_VERSION     "4.2.3"
    //资源版本
    #define GAME_RESOURCE_VERSION   "129"
#endif










//是否使用facebook FACEBOOK_ON = 开启 FACEBOOK_OFF = 关闭
#define FACEBOOK_OPEN_STATUS    FACEBOOK_OFF

//大小包标识（当前全部采用大包模式！勿设置成迷你包！）
#define GAME_PACKAGE_VERSION    GCPV_FULL_PACKAGE



//各类广告开关
#define GAME_HASOFFER_STATUS    GHOS_HASOFFER_OFF
#define GAME_CHARTBOOST_STATUS  GCBS_CHARTBOOST_OFF
#define GAME_GOCPA_STATUS       GGCS_GOCPA_OFF
#define GAME_APPSFLYER_STATUS   GAFS_APPSFLYER_OFF

/* !!!!!!!!!!发版请设置上方即可!!!!!!!!!! */

/*
 * 账号类型。
 * 一般来说与渠道是一一对应，但是有些渠道用的是XDYOU的账号体系。规则：GCAT_账号商
 */
//兄弟游
#define GCAT_XDYOU 1
//PP助手
#define GCAT_PP 2
//同步推
#define GCAT_TONGBUTUI 3
//UC九游
#define GCAT_UC 4
//GO2PLAY
#define GCAT_GO2PLAY 5
//360
#define GCAT_QIHU 6
//快用
#define GCAT_KUAIYONG 7

//账号类型，用以兼容以前的SDK代码，与“渠道代号”有较大的关联，但是目标不同。
//uc
#if (GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD)
    #define GAME_ACCOUNT_TYPE GCAT_UC
//360
#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_QIHU_ADR_THIRD)
    #define GAME_ACCOUNT_TYPE GCAT_QIHU
//快用
#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_KUAIYONG_IOS_THIRD)
    #define GAME_ACCOUNT_TYPE GCAT_KUAIYONG
//pp
#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_PP_IOS_THIRD)
    #define GAME_ACCOUNT_TYPE GCAT_PP
//tongbutui
#elif (GAME_CHANNEL_TYPE == GCCT_CHINA_TONGBUTUI_IOS_THIRD)
    #define GAME_ACCOUNT_TYPE GCAT_TONGBUTUI
//others
#else
    #if (GAME_PUBLISH_MODE == GCPM_INTERNAL_TEST_MODE)
        #define GAME_ACCOUNT_TYPE GCAT_XDYOU
    #elif (GAME_PUBLISH_MODE == GCPM_EXTERNAL_TEST_MODE)
        #define GAME_ACCOUNT_TYPE GCAT_GO2PLAY
    #else
        #if(GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE || GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY || GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_THIRD)
            #define GAME_ACCOUNT_TYPE GCAT_GO2PLAY
        #else
            #define GAME_ACCOUNT_TYPE GCAT_XDYOU
        #endif
    #endif
#endif

/*
 全球化配置，以后可能会将所有的URL与文字都整合到这里。
 */
class GlobalConfig
{
//公用API
public:
    //自身。
    static GlobalConfig* gi();
    
    //获得列表与认证的地址。
    std::string getAuthUrl();
    
    //获得用户中心的地址。
    std::string getUserCenterUrl();
    
    //获得渠道代号，由于部分渠道特殊性，导致必须用方法来获取。
    int getRealChannelId();
    
    //获得账号类型，用以区分账号体系。某些渠道使用我方（XDYOU）的账号体系（如go2play），某些渠道使用自身的账号体系（如同步推,PP助手,UC）。
    int getAccountType();
    
    //获得客户端版本号。
    std::string getClientVersion();
    
    //获得本包的资源版本号。
    std::string getResourceVersion();
    
    //获得大小包标识。
    int getPackageVersion();
    
    //游戏客户端更新的地址。
    CC_PROPERTY(std::string, _clientUpdateUrl, ClientUpdateUrl)
    
    //返回talkingdata的统计唯一id。
    std::string getTalkingDataAppId();
    
    //返回talkingdata的统计渠道名称标识。
    std::string getTalkingDataAppChannel();

    
//公用参数
private:
    
    
//定制API
public:
    //获得台湾版FB粉丝团地址。
    std::string getTaiWanFbFansGroupUrl();
    
    //台湾GP版往第三方引流的地址。
    CC_PROPERTY(std::string, _taiwanGpTo3rdUrl, TaiWanGpTo3rdLeadingUrl);

    //获取是不是go2play的googleplay的渠道包
    bool isGoolePlayChannel();
    
//定制参数
private:
    
};

#endif /* defined(__GameSanGuo__GlobalConfig__) */
