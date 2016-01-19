//
//  SdkManager.h
//  GameSanGuo
//
//  Created by zenghui on 14-4-2.
//
//

#ifndef __GameSanGuo__Sdkhandler__
#define __GameSanGuo__Sdkhandler__

#include "SdkManager.h"
#include "SGNotificationCenter.h"

#define REREGISTFLAG    "regist_receive"
#define LOGINFLAG       "login_receive"
#define FASTLOGINFLAG   "fastlogin_receive"
#define PASSWORDLAG     "password_receive"
#define THIRDPARTYLOGIN "third_party_login"
#define INVALID_INFO_TIP    "valid_user_name"
#define BIND_FAILED     "bind_facebook_failed"


class SdkInfoData : public CCObject
{
public:
    SdkInfoData();
    ~SdkInfoData();

    int state;
    int errorFlag;
    int accountId;
    int flag;
	
	bool isEx;
    std::string loginKey;
	std::string userName;
	std::string password;
};

class SdkHandler : public SdkHandlerDelegate
{
public:
    SdkHandler();
    ~SdkHandler();
    static SdkHandlerDelegate *shareSdkHandler();
    virtual void registFinished(char *bufferchar);
    virtual void loginFinished(char *bufferchar);
    virtual void logoutFinished(char *bufferchar);
    virtual void passwordFinished(char *bufferchar);
    virtual void fastLoginFinished(char *bufferchar);
    virtual void destroyGuestFinished(char *bufferchar);
    virtual void checkUserName(char *bufferchar);
    virtual void activite(char *bufferchar);
    virtual void requestFailed();
    virtual void thirdLoginFinished(char *bufferchar);
	
	inline void setUserName(std::string un){this->userName = un;}
	inline void setPassword(std::string pw){this->password = pw;}
	inline void setIsEx(bool isEx){this->isExist = isEx;}
private:
    //SdkInfoData * m_sdkLoginData;
	std::string userName;
	std::string password;
	bool isExist;
};


#endif /* defined(__GameSanGuo__Sdkhandler__) */
