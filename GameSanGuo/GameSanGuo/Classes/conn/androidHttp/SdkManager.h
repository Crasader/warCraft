//
//  SdkManager.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-26.
//
//

#ifndef __GameSanGuo__SdkManager__
#define __GameSanGuo__SdkManager__

#include "XdObject.h"
#include <string>


#define HTTPLENGHT 200

class SdkHandlerDelegate : public XdObject
{
public:
    virtual void registFinished(char *bufferchar){};
    virtual void loginFinished(char *bufferchar){};
    virtual void logoutFinished(char *bufferchar){};
    virtual void passwordFinished(char *bufferchar){};
    virtual void fastLoginFinished(char *bufferchar){};
    virtual void destroyGuestFinished(char *bufferchar){};
    virtual void checkUserName(char *bufferchar){};
    virtual void activite(char *bufferchar){};
    virtual void requestFailed(){};
    virtual void thirdLoginFinished(char *bufferchar){};
};

class SdkManager : public XdObject
{
public:
    SdkManager();
    static SdkManager *shareSdkManager();
    void setSdkHandle(SdkHandlerDelegate * sdkHandler);
    
    bool isInternetConnectionAvailable(void);
    void initUserInfo(const std::string &gameId,const std::string &channelId,const std::string &subChannelId,const std::string &OsVersion,const std::string &osType);
    void setExtInfo(const std::string &phoneNumber,const std::string &province,const std::string &city);
    std::string getUsrMac();
    

    /**
     *注册userRegister
     *@UserName：用户名
     *@Password：密码
     **/
    void userRegister(const std::string &userName,const std::string &password);
   
    /**
     *登陆userLogin
     *@UserName：用户名
     *@Password：密码
     **/
    void userLogin(const std::string &userName,const std::string &password);
    
    /**
     *退出userLogout
     *@UserId：用户id
     **/
    void userLogout(const std::string userId);

    /**
     *修改密码modifyPassword
     *@UserName：用户名
     *@Password：密码
     *@NewPassword：新密码
     **/
    void modifyPassword(const std::string userName,const std::string password,const std::string newPassword);

    /**
     *一键登陆oKLogin
     **/
    void oKLogin();
    
    /**
     *检测用户是否存在checkUserName
     *@UserName：用户名

     **/
    void checkUserName(const std::string userName);

    /**
     *激活游戏activite
     **/
    void activite();
    
    /**
     *消除游客帐号destroyGuest
     **/
    void destroyGuest();
    
    
    /**
     *由facebook登陆android对应的调用
     **/
    void androidFacebookLogin(std::string userName, std::string password);
    
     /*
      android的第三方登陆
      */
    void userThirdPartyLogin(const std::string &userName, const std::string &password);

    
    
    void sendHttpMsg(char ctag, const std::string & httpUrl, const std::string data);
    
    void requestFinishHandler(XdObject *sender, void *data);
    
private:
    std::string putInUserInfo();
    
    std::string putInExtInfo();
    
    std::string putInMd5Key(std::string &originStr);
    
    void fastLoginFinished(char *bufferchar);
    
    void loginFinished(char *bufferchar);

    std::string m_gameId;
    std::string m_channelId;
    std::string m_subChannelId;
    std::string m_OsVersion;
    std::string m_osType;
    
    std::string m_phoneNumber;
    std::string m_province;
    std::string m_city;
    
    std::string m_mac;

    SdkHandlerDelegate * m_sdkHandlerDelegate;
};

#endif /* defined(__GameSanGuo__SdkManager__) */
