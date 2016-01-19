//
//  GlobalConfig.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 10/11/14.
//
//

#include "GlobalConfig.h"

/* ---------- 公用API BGN ---------- */

static GlobalConfig* _instance = NULL;

//自身。
GlobalConfig* GlobalConfig::gi()
{
    if (!_instance)
    {
        _instance = new GlobalConfig();
    }
    return _instance;
}

//获得列表与认证的地址。
std::string GlobalConfig::getAuthUrl()
{
    std::string aUrl = "";
    if (GCPM_INTERNAL_TEST_MODE == GAME_PUBLISH_MODE) //内网测试
    {
        switch (GAME_CHANNEL_TYPE)
        {
            case GCCT_TEST_IOS_IN:
            case GCCT_TEST_ADR_IN:
            default:
                aUrl = "http://192.168.0.16:8080/SanguoLoginPortal/clientServlet"; //this is real internal
//                aUrl = "http://192.168.0.143:8080/SanGuoLoginPortal/clientServlet"; //this is zzq
//                aUrl = "http://s170.xdyou.cn:8080/SanguoLoginPortal/clientServlet"; //this is uc test
                break;
        }
    }
    else if (GCPM_EXTERNAL_TEST_MODE == GAME_PUBLISH_MODE) //外网测试
    {
        switch (GAME_CHANNEL_TYPE)
        {
            case GCCT_TEST_IOS_EX:
            case GCCT_TEST_ADR_EX:
            default:
                aUrl = "http://210.65.221.98:10020/SanguoLoginPortal/clientServlet";
                break;
        }
    }
    else if (GCPM_OFFICAL_RELEASE_MODE == GAME_PUBLISH_MODE) //正式发布
    {
        switch (GAME_CHANNEL_TYPE)
        {
                //ios
            case GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE:
                // aUrl = "http://herologinios.go2play.mobi:8080/SanguoLoginPortal/clientServlet";
                 aUrl = "https://node-2.sharelittle.com/SanGuoLoginPortal/clientServlet";
                 //aUrl = "http://192.168.1.88:8080/SanGuoLoginPortal/clientServlet";
                break;
                
                //android
            case GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY:
            case GCCT_TAIWAN_GO2PLAY_ADR_THIRD:
                //aUrl = "http://herologinandriod.go2play.mobi:8080/SanguoLoginPortal/clientServlet";
                aUrl = "https://node-2.sharelittle.com/SanGuoLoginPortal/clientServlet";
                break;
            case GCCT_CHINA_XDYOU_IOS_APPLE_APPSTORE:
                aUrl = "http://s10.xdyou.cn:8080/SanguoLoginPortal/clientServlet";
                break;
            case GCCT_CHINA_XDYOU_ADR_UFO:
                aUrl = "http://s10.xdyou.cn:8080/SanguoLoginPortal/clientServlet";
                break;
            case GCCT_CHINA_UC_ADR_THIRD:
                aUrl = "http://s170.xdyou.cn:8080/SanguoLoginPortal/clientServlet";
                break;
            case GCCT_CHINA_QIHU_ADR_THIRD: //TODO: 360认证地址
                aUrl = "http://s170.xdyou.cn:8080/SanguoLoginPortal/clientServlet";
                break;
            case GCCT_CHINA_KUAIYONG_IOS_THIRD: //TODO: KUAIYONG认证地址
                aUrl = "http://s170.xdyou.cn:8080/SanguoLoginPortal/clientServlet";
                break;
        }
    }
    else
    {
        assert(1 == 0);
    }
    
    assert(aUrl.size() > 0);
    
    return aUrl;
}

////#define SDKAPIADDRESS      "http://211.151.170.4/NGAPI/"//测试 用户中心地址
//#define SDKAPIADDRESS      "http://herousercenter.go2play.mobi/NGAPI/" //台湾服

//获得用户中心的地址。
std::string GlobalConfig::getUserCenterUrl()
{
    std::string userCenterUrl = "";
    if (GCPM_INTERNAL_TEST_MODE == GAME_PUBLISH_MODE
        || GCPM_EXTERNAL_TEST_MODE == GAME_PUBLISH_MODE) //内网测试 外网测试均使用一个测试的用户中心
    {
        userCenterUrl = "http://s4.xdyou.cn/NGAPI/";
    }
    else if (GCPM_OFFICAL_RELEASE_MODE == GAME_PUBLISH_MODE) //正式发布
    {
        switch (GAME_CHANNEL_TYPE)
        {
                //台湾的正式的用户中心
            case GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE:
            case GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY:
            case GCCT_TAIWAN_GO2PLAY_ADR_THIRD:
                userCenterUrl = "http://herousercenter.go2play.mobi/NGAPI/";
                break;
                //大陆的正式的用户中心
            case GCCT_CHINA_XDYOU_IOS_APPLE_APPSTORE:
            case GCCT_CHINA_XDYOU_ADR_UFO:
            case GCCT_CHINA_UC_ADR_THIRD:
            case GCCT_CHINA_QIHU_ADR_THIRD:
            case GCCT_CHINA_KUAIYONG_IOS_THIRD:
                userCenterUrl = "http://s5.xdyou.cn/NGAPI/";
                break;
        }
    }

    //如果没有，出大事！
    assert(userCenterUrl.size() > 0);
    
    CCLOG("userCenterUrl || [%s]", userCenterUrl.c_str());
    return userCenterUrl;
}

//获得渠道代号，由于部分渠道特殊性，导致必须用方法来获取。
int GlobalConfig::getRealChannelId()
{
    if (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY || GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_THIRD) //台湾GO2PLAY的3rd和gp包都是渠道2！
    {
        return GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY;
    }
    else
    {
        return GAME_CHANNEL_TYPE;
    }
}

//获得账号类型，用以区分账号体系。某些渠道使用我方（XDYOU）的账号体系（如go2play），某些渠道使用自身的账号体系（如同步推,PP助手,UC）。
int GlobalConfig::getAccountType()
{
    return GAME_ACCOUNT_TYPE;
}

//获得游戏客户端更新的地址。
std::string GlobalConfig::getClientUpdateUrl()
{
    return _clientUpdateUrl;
}

//设置游戏客户端更新的地址。
void GlobalConfig::setClientUpdateUrl(std::string cUrl)
{
    _clientUpdateUrl = cUrl;
}

//获得客户端版本号。
std::string GlobalConfig::getClientVersion()
{
    return GAME_CLIENT_VERSION;
}

//获得本包的资源版本号。
std::string GlobalConfig::getResourceVersion()
{
    if (GCPV_MINI_PACKAGE == GAME_PACKAGE_VERSION)
    {
        return "0";
    }
    else
    {
        return GAME_RESOURCE_VERSION;
    }
}

//获得大小包标识。
int GlobalConfig::getPackageVersion()
{
    return GAME_PACKAGE_VERSION;
}

//获取对应的talkingdata的url标识
std::string GlobalConfig::getTalkingDataAppId()
{
    std::string talkingdataAppId = "";
    if (GCPM_INTERNAL_TEST_MODE == GAME_PUBLISH_MODE) //内网测试，使用pengyou的测试appId
    {
        switch (GAME_CHANNEL_TYPE)
        {
            case GCCT_TEST_IOS_IN:
                talkingdataAppId = "AADCA30D169116CF80FCAEA8EB601177";//xdyou ios test
                break;
            case GCCT_TEST_ADR_IN:
                talkingdataAppId = "AADCA30D169116CF80FCAEA8EB601177";//xdyou android test
                break;
            default:
                talkingdataAppId = "AADCA30D169116CF80FCAEA8EB601177";//xdyou android test
                break;
        }
    }
    else if (GCPM_EXTERNAL_TEST_MODE == GAME_PUBLISH_MODE) //外網測試，使用go2play的appId
    {
        switch (GAME_CHANNEL_TYPE)
        {
            case GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE:
//                talkingdataAppId = "A6F6B5566E09245EF904A6A824D9191D";//台湾go2play ios talkingdata appid
                talkingdataAppId = "8FC2C66714BD1DD4E5D947413E72B20C";//台湾go2play ios talkingdata appid
                break;
            case GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY:
            case GCCT_TAIWAN_GO2PLAY_ADR_THIRD:
//                talkingdataAppId = "191F6CA0284EFD11DCF6E8AAB81851D7";//台湾go2play android talkingdata appid
                talkingdataAppId = "C25BDD25AB8E9D2549ADF4756511DC61";//台湾go2play android talkingdata appid
                break;
            default:
                talkingdataAppId = "AADCA30D169116CF80FCAEA8EB601177";//xdyou android test
                break;
        }
    }
    
    else if (GCPM_OFFICAL_RELEASE_MODE == GAME_PUBLISH_MODE) //正式发布 使用其他的appid
    {
        switch (GAME_CHANNEL_TYPE)
        {
            case GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE:
//                talkingdataAppId = "A6F6B5566E09245EF904A6A824D9191D";//台湾go2play ios talkingdata appid
                talkingdataAppId = "8FC2C66714BD1DD4E5D947413E72B20C";//台湾go2play ios talkingdata appid
                break;
            case GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY:
            case GCCT_TAIWAN_GO2PLAY_ADR_THIRD:
//                talkingdataAppId = "191F6CA0284EFD11DCF6E8AAB81851D7";//台湾go2play android talkingdata appid
                talkingdataAppId = "C25BDD25AB8E9D2549ADF4756511DC61";//台湾go2play android talkingdata appid
                break;
            case GCCT_CHINA_XDYOU_ADR_UFO: //大陆渠道 xdyou android
            case GCCT_CHINA_UC_ADR_THIRD: //大陆渠道 uc android
            case GCCT_CHINA_QIHU_ADR_THIRD: //大陆渠道 360 android
                talkingdataAppId = "C4CAB5E513C4D7059C9CB5027468E712";
                break;
            case GCCT_CHINA_XDYOU_IOS_APPLE_APPSTORE: //大陆渠道 xdyou ios
            case GCCT_CHINA_KUAIYONG_IOS_THIRD: //大陆渠道 快用 ios
                talkingdataAppId = "B1F2750406FB24595CAF7CE7D73E72E2";
                break;
        }
    }
    else//错误
    {
        assert(1 == 0);
    }
    
    assert(talkingdataAppId.size() > 0);
    
    return talkingdataAppId;
}

//返回talkingdata的统计渠道名称标识。
std::string GlobalConfig::getTalkingDataAppChannel()
{
    std::string ac = "";
    if (GCPM_INTERNAL_TEST_MODE == GAME_PUBLISH_MODE) //内网测试
    {
        switch (GAME_CHANNEL_TYPE)
        {
            case GCCT_TEST_IOS_IN:
                ac = "TEST_IOS";
                break;
            case GCCT_TEST_ADR_IN:
                ac = "TEST_ANDROID";
                break;
            default:
                ac = "TEST_IN_UFO";
                break;
        }
    }
    else if (GCPM_EXTERNAL_TEST_MODE == GAME_PUBLISH_MODE) //外网测试
    {
        switch (GAME_CHANNEL_TYPE)
        {
            case GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE: //台湾go2play ios
                ac = "TEST_GM_IOS";
                break;
            case GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY: //台湾go2play android
            case GCCT_TAIWAN_GO2PLAY_ADR_THIRD:
                ac = "TEST_GM_ANDROID";
                break;
            default:
                ac = "TEST_EX_UFO";
                break;
        }
    }
    else if (GCPM_OFFICAL_RELEASE_MODE == GAME_PUBLISH_MODE) //正式发布
    {
        switch (GAME_CHANNEL_TYPE)
        {
            case GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE: //台湾go2play ios
                ac = "GM_IOS";
                break;
            case GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY: //台湾go2play android
            case GCCT_TAIWAN_GO2PLAY_ADR_THIRD:
                ac = "GM_ANDROID";
                break;
            case GCCT_CHINA_XDYOU_ADR_UFO: //大陆xdyou android
                ac = "XDYOU_ANDROID";
                break;
            case GCCT_CHINA_UC_ADR_THIRD: //大陆uc android
                ac = "UC_ANDROID";
                break;
            case GCCT_CHINA_QIHU_ADR_THIRD: //大陆360 android
                ac = "360_ANDROID";
                break;
            case GCCT_CHINA_XDYOU_IOS_APPLE_APPSTORE: //大陆xdyou ios
                ac = "XDYOU_IOS";
                break;
            case GCCT_CHINA_KUAIYONG_IOS_THIRD:
                ac = "KUAIYONG_IOS";
                break;
            default:
                ac = "TEST_OF_UFO";
                break;
        }
    }
    else//错误
    {
        assert(1 == 0);
    }
    
    assert(ac.size() > 0);
    
    return ac;
}



/* ---------- 公用API END ---------- */



/* ---------- 定制API BGN ---------- */

//获得台湾版FB粉丝团地址。
std::string GlobalConfig::getTaiWanFbFansGroupUrl()
{
    return "https://www.facebook.com/3herotw?ref=hl";
}

//获得台湾GP版往第三方引流的地址。
std::string GlobalConfig::getTaiWanGpTo3rdLeadingUrl()
{
    return _taiwanGpTo3rdUrl;
}

//设置台湾GP版往第三方引流的地址。
void GlobalConfig::setTaiWanGpTo3rdLeadingUrl(std::string url)
{
    _taiwanGpTo3rdUrl = url;
}

//获取是不是googleplay的渠道包
bool GlobalConfig::isGoolePlayChannel()
{
    return GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY;
}

/* ---------- 定制API END ---------- */

