//
//  SdkManager.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-26.
//
//
#include <stdlib.h>
#include "SdkManager.h"
#include "md5.h"
#include "XdHttpRequest.h"
#include "XdHttpResponse.h"
#include "XdHttpClient.h"
#include "cocos2d.h"
#include "SdkHandler.h"
#include "GameConfig.h"
#include "SGMainManager.h"
#include "GlobalConfig.h"

USING_NS_CC;

//由globalconfig中统一配置，留底参考
//#define SDKAPIADDRESS      "http://211.151.170.4/NGAPI/"//测试 用户中心地址
//#define SDKAPIADDRESS      "http://herousercenter.go2play.mobi/NGAPI/" //台湾服
#define IOSDEVICECODE      "IOSDeviceCode"
#define SERVICECOMPANY     "UserCenter.xdyou.com.AppIdentifier"
#define ACCOUNTID          "pengyou20131010"
#define IOSVERSIONLIMMIT   6.0



enum XdHttpFlag
{
    XD_HTTP_REGISTER = 5,
    XD_HTTP_LOGIN ,
    XD_HTTP_LOGOUT,
    XD_HTTP_MODIFY_PASSWORD ,
    XD_HTTP_OK_LOGIN ,
    XD_HTTP_CHECK_USER_NAME ,
    XD_HTTP_ACTIVATE ,
    XD_HTTP_DESTROYGUEST ,
    XD_HTTP_THIRDPARTY,
};

static SdkManager *shareInter = NULL;

SdkManager::SdkManager() :m_sdkHandlerDelegate(NULL)
{
    m_gameId = "3";
    m_channelId = "12";
    m_subChannelId = "6";
    m_OsVersion = "25";
    m_osType = "1";

    m_phoneNumber = "PhoneNum";
    m_province = "Province";
    m_city = "City";
    
    
    
    //xxx:
    m_mac = GameConfig::getUdit();
    CCLOG("m_mac in fun [%s->%s] ", __func__, m_mac.c_str());

}

SdkManager *SdkManager::shareSdkManager()
{
    if (!shareInter)
    {
        shareInter = new SdkManager();
    }
    return shareInter;
}
bool SdkManager::isInternetConnectionAvailable(void)
{
    //    return [[Reachability_objc reachabilityForInternetConnection] currentReachabilityStatus] != NotReachable;
    return true;
}
void SdkManager::setSdkHandle(SdkHandlerDelegate * sdkHandler)
{
    CCLOG("setSdkHandler -- sdkHandler: %p -- m_sdkHandlerDelegate : %p", sdkHandler, m_sdkHandlerDelegate);
    if (m_sdkHandlerDelegate != NULL) {
        m_sdkHandlerDelegate->release();
    }
    m_sdkHandlerDelegate = sdkHandler;
}

void SdkManager::initUserInfo(const std::string &gameId,const std::string &channelId,const std::string &subChannelId,const std::string &OsVersion,const std::string &osType)
{
    m_gameId = gameId;
    m_channelId = channelId;
    m_subChannelId = subChannelId;
    m_OsVersion = OsVersion;
    m_osType = osType;
}
void SdkManager::setExtInfo(const std::string &phoneNumber,const std::string &province,const std::string &city)
{
    m_phoneNumber = phoneNumber;
    m_province = province;
    m_city = city;
}

std::string SdkManager::getUsrMac()
{
    if (m_mac.compare("123") == 0) {
        m_mac = "3111232123";
    }

    m_mac = GameConfig::getUdit();
    CCLOG("m_mac in fun [%s->%s] ", __func__, m_mac.c_str());
    return m_mac;
}

std::string SdkManager::putInUserInfo()
{
    std::string userInfo = "GameId=" + m_gameId +"&";
    userInfo += "ChannelId=" + m_channelId +"&";
    userInfo += "SubChannelId=" + m_subChannelId +"&";
    userInfo += "OsVersion=" + m_OsVersion +"&";
    userInfo += "Mac=" + m_mac +"&";
    userInfo += "OsType=" + m_osType +"&" ;
    userInfo += "DeviceCode=IOSDeviceCode";
    return userInfo;
}
std::string SdkManager::putInExtInfo()
{
    std::string extInfo = "ExtInfo=" + m_province + "@" + m_city +"@" + m_phoneNumber;
    
    return extInfo;
}

std::string SdkManager::putInMd5Key(std::string &originStr)
{
    MD5 md5(originStr);
    std::string md5Key = "Key=" + md5.md5();
    return md5Key;
}

void SdkManager::userRegister(const std::string &userName,const std::string &password)
{
    char ctag = XD_HTTP_REGISTER;
    
    std::string httpUrl = GlobalConfig::gi()->getUserCenterUrl();;
    httpUrl += "REGISTER.do";
    m_mac = GameConfig::getUdit();
    std::string md5Str = userName + password + m_gameId + m_channelId + m_subChannelId + m_OsVersion + m_mac + m_osType + IOSDEVICECODE;
    
    std::string data = "act=REGISTER&" ;
    data += "UserName=" + userName +"&";
    data += "Password=" + password +"&";
    
    data += putInMd5Key(md5Str) + "&";
    data += putInUserInfo() + "&";
    data += "ExtInfo=Province@City@123456789@CardID@Name@1";

	static_cast<SdkHandler*>(m_sdkHandlerDelegate)->setUserName(userName);
	static_cast<SdkHandler*>(m_sdkHandlerDelegate)->setPassword(password);
	
    this->sendHttpMsg(ctag,httpUrl,data);
}

void SdkManager::userLogin(const std::string &userName,const std::string &password)
{
	CCLOG("data:[%s-%s][func:%s line:%d]", userName.c_str(),password.c_str(), __FUNCTION__, __LINE__);

    char ctag = XD_HTTP_LOGIN;
   
    std::string httpUrl = GlobalConfig::gi()->getUserCenterUrl();;
    httpUrl += "LOGIN.do";
    m_mac = GameConfig::getUdit();
    std::string md5Str = userName + password + m_gameId + m_channelId + m_subChannelId +
						 m_OsVersion + m_mac + m_osType + IOSDEVICECODE;
    CCLOG("go2play -- m_channelId == %s", m_channelId.c_str());

 
    std::string data = "act=LOGIN&" ;
    data += "UserName=" + userName +"&";
    data += "Password=" + password +"&";
    data += putInMd5Key(md5Str) + "&";
    data += putInUserInfo();
	
	static_cast<SdkHandler*>(m_sdkHandlerDelegate)->setUserName(userName);
	static_cast<SdkHandler*>(m_sdkHandlerDelegate)->setPassword(password);
    static_cast<SdkHandler*>(m_sdkHandlerDelegate)->setIsEx(true);

	CCLOG("data:[%s][func:%s line:%d]", data.c_str(), __FUNCTION__, __LINE__);
    this->sendHttpMsg(ctag,httpUrl,data);
}

void SdkManager::userThirdPartyLogin(const std::string &userName, const std::string &password)
{
    CCLOG("data:[%s-%s][func:%s line:%d]", userName.c_str(),password.c_str(), __FUNCTION__, __LINE__);
    
    char ctag = XD_HTTP_THIRDPARTY;
    
    std::string httpUrl = GlobalConfig::gi()->getUserCenterUrl();;
    httpUrl += "THIRDPARTY_LOGIN.do";
    m_mac = GameConfig::getUdit();
    std::string md5Str = userName + password + m_gameId + m_channelId + m_subChannelId +
    m_OsVersion + m_mac + m_osType + IOSDEVICECODE;
    
    std::string data = "act=THIRDPARTY_LOGIN&" ;
    data += "UserName=" + userName +"&";
    data += "Password=" + password +"&";
    data += putInMd5Key(md5Str) + "&";
    data += putInUserInfo();
	
	static_cast<SdkHandler*>(m_sdkHandlerDelegate)->setUserName(userName);
	static_cast<SdkHandler*>(m_sdkHandlerDelegate)->setPassword(password);
    static_cast<SdkHandler*>(m_sdkHandlerDelegate)->setIsEx(true);

	CCLOG("data:[%s][func:%s line:%d]", data.c_str(), __FUNCTION__, __LINE__);
    this->sendHttpMsg(ctag,httpUrl,data);
}

void SdkManager::userLogout(const std::string userId)
{
    char ctag = XD_HTTP_LOGOUT;
    
    std::string httpUrl = GlobalConfig::gi()->getUserCenterUrl();;
    httpUrl += "LOGOUT.do";
    m_mac = GameConfig::getUdit();
    std::string md5Str = userId + m_gameId + m_channelId + m_subChannelId + m_OsVersion + m_mac + m_osType + IOSDEVICECODE;
    
    std::string data = "act=LOGOUT&" ;
    data += "UserId=" + userId +"&";
    data += putInMd5Key(md5Str) + "&";
    data += putInUserInfo();
    
    this->sendHttpMsg(ctag,httpUrl,data);
}

void SdkManager::modifyPassword(const std::string userName,const std::string password,const std::string newPassword)
{
    char ctag = XD_HTTP_MODIFY_PASSWORD;
    
    std::string httpUrl = GlobalConfig::gi()->getUserCenterUrl();;
    httpUrl += "MODIFY_PASSWORD.do";
    
    m_mac = GameConfig::getUdit();
    std::string md5Str = userName + password + m_gameId + m_channelId + m_subChannelId + m_OsVersion + m_mac + m_osType + IOSDEVICECODE + newPassword;

    std::string data = "act=MODIFY_PASSWORD&" ;
    data += "UserName=" + userName +"&";
    data += "Password=" + password +"&";
    data += "NewPassword=" + newPassword +"&";
    
    data += putInMd5Key(md5Str) + "&";
    data += putInUserInfo();
    
    this->sendHttpMsg(ctag,httpUrl,data);
}

void SdkManager::oKLogin()
{
    char ctag = XD_HTTP_OK_LOGIN;

    std::string httpUrl = GlobalConfig::gi()->getUserCenterUrl();;
    httpUrl += "OK_LOGIN.do";
    m_mac = GameConfig::getUdit();
    CCLOG("m_mac == %s", m_mac.c_str());
    std::string md5Str =  m_gameId + m_channelId + m_subChannelId + m_OsVersion + m_mac + m_osType + IOSDEVICECODE;

    std::string data = "act=OK_LOGIN&" ;
    data += putInMd5Key(md5Str) + "&";
    data += putInUserInfo() + "&";
    data += putInExtInfo();
    //data += "Province@City@PhoneNum";
    
    this->sendHttpMsg(ctag,httpUrl,data);
}

void SdkManager::checkUserName(const std::string userName)
{

    char ctag = XD_HTTP_CHECK_USER_NAME;
    
    std::string httpUrl = GlobalConfig::gi()->getUserCenterUrl();;
    httpUrl += "CHECK_USER_NAME.do";
    
    std::string data = "act=CHECK_USER_NAME&" ;
    data += "UserName=" + userName ;

    this->sendHttpMsg(ctag,httpUrl,data);
}

void SdkManager::activite()
{
    char ctag = XD_HTTP_ACTIVATE;
    
    std::string httpUrl = GlobalConfig::gi()->getUserCenterUrl();;
    httpUrl += "ACTIVATE.do";
    m_mac = GameConfig::getUdit();
    std::string md5Str = m_gameId + m_channelId + m_subChannelId + m_OsVersion + m_mac + m_osType + IOSDEVICECODE;
    
    std::string data = "act=ACTIVATE&" ;
    data += putInMd5Key(md5Str) + "&";
    data += putInUserInfo();
    
    this->sendHttpMsg(ctag,httpUrl,data);

}

void SdkManager::destroyGuest()
{
    m_mac = GameConfig::getUdit();
    char ctag = XD_HTTP_DESTROYGUEST;
    
    std::string httpUrl = GlobalConfig::gi()->getUserCenterUrl();;
    httpUrl += "DESTROYGUEST.do";
    
    std::string md5Str =  m_mac + IOSDEVICECODE;
    
    std::string data = "act=DESTROYGUEST&" ;
     data += "Mac=" + m_mac + "&";
     data += std::string("DeviceCode=") + std::string(IOSDEVICECODE) ;
     data += putInMd5Key(md5Str) ;

    
    this->sendHttpMsg(ctag,httpUrl,data);
}

void SdkManager::sendHttpMsg(char ctag, const std::string & httpUrl, const std::string data)
{
    XdHttpRequest* request = new XdHttpRequest();
    request->setUrl(httpUrl.c_str());
    request->setRequestType(XdHttpRequest::kHttpPost);
    request->setRequestData(data.c_str(), data.length());
    XdHttpClient::getInstance()->setTimeoutForConnect(20);
    
    int _tag = 0;
    memcpy(&_tag, &ctag, 1);
    
    char requestTag[1];
    sprintf(requestTag, "%d",_tag);
    request->setTag(requestTag);
    
    request->setResponseCallback(this, xdcallfuncND_selector(SdkManager::requestFinishHandler));
    
    XdHttpClient::getInstance()->send(request);
}


void SdkManager::requestFinishHandler(XdObject *sender, void *data)
{
    XdHttpResponse *response = (XdHttpResponse*)data;
    
	
    if (!m_sdkHandlerDelegate || !response)
    {
        SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
        return;
    }
    int statusCode = response->getResponseCode();
    if (!response->isSucceed())
    {
        m_sdkHandlerDelegate->requestFailed();
        SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
        return;
    }
	CCLOG("response: [%p], m_sdkHandlerDelegate : [%p]", response, m_sdkHandlerDelegate);

    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        const char *responseTag = response->getHttpRequest()->getTag();
 
        int rt = atoi(responseTag);
        std::vector<char> *buffer = response->getResponseData();
        int bufferSize = buffer->size();
        char *bufferchar = new char[bufferSize + 1];
        std::copy(buffer->begin(), buffer->end(), bufferchar);
		bufferchar[bufferSize] = '\0';
		
        switch (rt)
        {
            case XD_HTTP_REGISTER:
            {
                m_sdkHandlerDelegate->registFinished(bufferchar);
				CCLOG("register:[%d]", __LINE__);
            }
              break;
            case XD_HTTP_LOGIN:
            {
                m_sdkHandlerDelegate->loginFinished(bufferchar);
				CCLOG("login:[%d]", __LINE__);

            }
              break;
            case XD_HTTP_LOGOUT:
            {
                m_sdkHandlerDelegate->logoutFinished(bufferchar);
				CCLOG("logout:[%d]", __LINE__);

            }
                break;
            case XD_HTTP_MODIFY_PASSWORD:
            {
                m_sdkHandlerDelegate->passwordFinished(bufferchar);
				CCLOG("password:[%d]", __LINE__);

            }
                break;
            case XD_HTTP_OK_LOGIN:
            {
                m_sdkHandlerDelegate->fastLoginFinished(bufferchar);
				CCLOG("oklogin:[%d]", __LINE__);

            }
                break;
            case XD_HTTP_CHECK_USER_NAME:
            {
                m_sdkHandlerDelegate->checkUserName(bufferchar);
				CCLOG("check_user_name:[%d]", __LINE__);

            }
                break;
            case XD_HTTP_ACTIVATE:
            {
                m_sdkHandlerDelegate->activite(bufferchar);
				CCLOG("activate:[%d]", __LINE__);

            }
                break;
            case XD_HTTP_DESTROYGUEST:
            {
                m_sdkHandlerDelegate->destroyGuestFinished(bufferchar);
				CCLOG("destroy_guest:[%d]", __LINE__);

            }
                break;
            case XD_HTTP_THIRDPARTY:
            {
                m_sdkHandlerDelegate->thirdLoginFinished(bufferchar);
                CCLOG("third_party_login:[%d]", __LINE__);
            }
                break;
            default:
                break;
        }
        delete [] bufferchar;
    }
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();

}






