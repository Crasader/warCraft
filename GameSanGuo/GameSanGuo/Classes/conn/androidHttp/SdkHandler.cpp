//
//  SdkHandler.cpp
//  GameSanGuo
//
//  Created by zenghui on 14-4-2.
//
//

#include "SdkHandler.h"
#include "libjson.h"
#include "SGMainManager.h"

static SdkHandler *shareSdkHandlerr = NULL;

SdkHandler::SdkHandler()
{
    //m_sdkLoginData = new SdkInfoData();
}
SdkHandler::~SdkHandler()
{
    //m_sdkLoginData->release();
}
SdkHandlerDelegate *SdkHandler::shareSdkHandler()
{
    CCLOG("shareSdkHandler");
    if (!shareSdkHandlerr)
    {
        shareSdkHandlerr = new SdkHandler();
    }
    return shareSdkHandlerr;
}

void SdkHandler::registFinished(char *bufferchar)
{
	CCLOG("in function:[%s], buff:[%s]", __FUNCTION__, bufferchar);
	
	
    int state = 0;
    int userId = 0;
    int flag = 0;
    const char* loginKey = NULL;
    JSONNODE *n = json_parse(bufferchar);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
		CCLOG("Start Parse Json in [%s]", __FUNCTION__);
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "STATE") == 0){
            json_int_t node_value = json_as_int(*i);
            state = node_value;
        }
        else if (strcmp(node_name, "USER_ID") == 0){
            json_int_t node_value = json_as_int(*i);
            userId = node_value;
            
        }
        else if (strcmp(node_name, "LOGIN_KEY") == 0){
            json_char *node_value = json_as_string(*i);
            loginKey = node_value;
            
        }
        else if (strcmp(node_name, "ERROR_TYPE") == 0){
            json_int_t node_value = json_as_int(*i);
            flag = node_value;
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
	
	//这里做状态判定
	if (flag == 10)
	{
        SGNotificationCenter::sharedNotificationCenter()->postNotification(INVALID_INFO_TIP,new CCInteger(10),false);
		return ;
	}
	else if (flag == 11)
	{
        SGNotificationCenter::sharedNotificationCenter()->postNotification(INVALID_INFO_TIP,new CCInteger(11),false);
		return ;
	}
	else if (flag == 12)
	{
        SGNotificationCenter::sharedNotificationCenter()->postNotification(INVALID_INFO_TIP,new CCInteger(12),false);
		return ;
	}
	
	
	CCLOG("Json parse completed! in fun : [%s]", __FUNCTION__);
	
    SdkInfoData *sdkLoginData = new SdkInfoData();
    sdkLoginData->state = state;
    sdkLoginData->accountId = userId;
    sdkLoginData->flag = flag;
    sdkLoginData->loginKey = std::string(loginKey);
	sdkLoginData->userName = userName;
	sdkLoginData->password = password;
	sdkLoginData->isEx = true;
	
	
	CCLOG("####state:[%d]m accountId : [%d], flag : [%d], loginKey:[%s]####", state, userId, flag, loginKey);
    SGNotificationCenter::sharedNotificationCenter()->postNotification(REREGISTFLAG,sdkLoginData,false);

}
void SdkHandler::loginFinished(char *bufferchar)
{
	CCLOG("in function:[%s], buff:[%s]", __FUNCTION__, bufferchar);
	
	CCLOG("userName:[%s], password:[%s]", userName.c_str(), password.c_str());
	
    int state = 0;
    int userId = 0;
    int flag = 0;
    const char* loginKey = NULL;
    JSONNODE *n = json_parse(bufferchar);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
		
		CCLOG("Start Parse Json in [%s]", __FUNCTION__);
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "STATE") == 0){
            json_int_t node_value = json_as_int(*i);
            state = node_value;
        }
        else if (strcmp(node_name, "USER_ID") == 0){
            json_int_t node_value = json_as_int(*i);
            userId = node_value;
            
        }
        else if (strcmp(node_name, "LOGIN_KEY") == 0){
            json_char *node_value = json_as_string(*i);
            loginKey = node_value;
            
        }
        else if (strcmp(node_name, "ERROR_TYPE") == 0){
            json_int_t node_value = json_as_int(*i);
            flag = node_value;
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
	
    //state为0的时候说明登入错误
	if (state == 0 && flag == 20)
	{
        CCLOG("----state:[%d], err_type:[%d]", state, flag);
        SGNotificationCenter::sharedNotificationCenter()->postNotification(INVALID_INFO_TIP,new CCInteger(20),false);
		return ;
	}
    
	
	
	CCLOG("state:[%d], err_type:[%d]", state, flag);
    if (state == 1)
	{
        CCLOG(" ====== enter if (state == 1) ====== ");
        SdkInfoData *sdkLoginData = new SdkInfoData();
        sdkLoginData->accountId = userId;
        sdkLoginData->errorFlag = flag;
        sdkLoginData->loginKey = std::string(loginKey);
		sdkLoginData->userName = this->userName;
		sdkLoginData->password = this->password;
		sdkLoginData->isEx = this->isExist;
		
		CCLOG("@@@loginKey:[%s]@@@", loginKey);
		//需要将登陆数据写入本地,在此非主线程,需要将当前数据发至主线程写入本地文件
        SGNotificationCenter::sharedNotificationCenter()->postNotification(LOGINFLAG,sdkLoginData,false);
    }

    else
    {
		CCLOG("Login Failed!");
    }
}


void SdkHandler::thirdLoginFinished(char *bufferchar)
{
	CCLOG("in function:[%s], buff:[%s]", __FUNCTION__, bufferchar);
	
	CCLOG("userName:[%s], password:[%s]", userName.c_str(), password.c_str());
	
    int state = 0;
    int userId = 0;
    int flag = 0;
    const char* loginKey = NULL;
    JSONNODE *n = json_parse(bufferchar);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
		
		CCLOG("Start Parse Json in [%s]", __FUNCTION__);
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "STATE") == 0){
            json_int_t node_value = json_as_int(*i);
            state = node_value;
        }
        else if (strcmp(node_name, "USER_ID") == 0){
            json_int_t node_value = json_as_int(*i);
            userId = node_value;
            
        }
        else if (strcmp(node_name, "LOGIN_KEY") == 0){
            json_char *node_value = json_as_string(*i);
            loginKey = node_value;
            
        }
        else if (strcmp(node_name, "ERROR_TYPE") == 0){
            json_int_t node_value = json_as_int(*i);
            flag = node_value;
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
	
	if (flag == 20)
	{
        SGNotificationCenter::sharedNotificationCenter()->postNotification(INVALID_INFO_TIP,new CCInteger(20),false);
		return ;
	}
	
	
	CCLOG("state:[%d], err_type:[%d]", state, flag);
    if (state == 1)
	{
        SdkInfoData *sdkLoginData = new SdkInfoData();
        sdkLoginData->accountId = userId;
        sdkLoginData->errorFlag = flag;
        sdkLoginData->loginKey = std::string(loginKey);
		sdkLoginData->userName = this->userName;
		sdkLoginData->password = this->password;
		sdkLoginData->isEx = this->isExist;
		
		CCLOG("loginKey:[%s]", loginKey);
		
		//需要将登陆数据写入本地,在此非主线程,需要将当前数据发至主线程写入本地文件
        SGNotificationCenter::sharedNotificationCenter()->postNotification(THIRDPARTYLOGIN,sdkLoginData,false);
		
    }
    else if (flag == 13)
    {
        SGNotificationCenter::sharedNotificationCenter()->postNotification(BIND_FAILED, false);
    }
    else
    {
		CCLOG("Login Failed!");
    }
}



void SdkHandler::logoutFinished(char *bufferchar)
{
    
}

void SdkHandler::passwordFinished(char *bufferchar)
{
	CCLOG("in function:[%s], buff:[%s]", __FUNCTION__, bufferchar);
	
    int state = 0;
    int userId = 0;
    int flag = 0;
    JSONNODE *n = json_parse(bufferchar);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
		CCLOG("Start Parse Json in [%s]", __FUNCTION__);
		
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "STATE") == 0){
            json_int_t node_value = json_as_int(*i);
            state = node_value;
        }
        else if (strcmp(node_name, "USER_ID") == 0){
            json_int_t node_value = json_as_int(*i);
            userId = node_value;
            
        }
        else if (strcmp(node_name, "LOGIN_KEY") == 0){
            json_char *node_value = json_as_string(*i);
            json_free(node_value);
            
        }
        else if (strcmp(node_name, "ERROR_TYPE") == 0){
            json_int_t node_value = json_as_int(*i);
            flag = node_value;
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
	CCLOG("state:[%d], flag:[%d]", state, flag);
    SdkInfoData *sdkLoginData = new SdkInfoData();
    sdkLoginData->state = state;
    sdkLoginData->errorFlag = flag;
    SGNotificationCenter::sharedNotificationCenter()->postNotification(PASSWORDLAG,sdkLoginData,false);
	
}

void SdkHandler::fastLoginFinished(char *bufferchar)
{
	CCLOG("in function:[%s], buff:[%s]", __FUNCTION__, bufferchar);
	
    int state = 0;
    int userId = 0;
    const char* loginKey = NULL;
    JSONNODE *n = json_parse(bufferchar);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
		CCLOG("Start Parse Json in [%s]", __FUNCTION__);
		
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "STATE") == 0){
            json_int_t node_value = json_as_int(*i);
            state = node_value;
        }
        else if (strcmp(node_name, "USER_ID") == 0){
            json_int_t node_value = json_as_int(*i);
            userId = node_value;
            
        }
        else if (strcmp(node_name, "LOGIN_KEY") == 0){
            json_char *node_value = json_as_string(*i);
            loginKey = node_value;
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
	
	CCLOG("state:[%d], userId:[%d], loginKey:[%s]", state, userId, loginKey);
    SdkInfoData *sdkLoginData = new SdkInfoData();
    sdkLoginData->state = state;
    sdkLoginData->accountId = userId;
    sdkLoginData->loginKey = std::string(loginKey);
	
    SGNotificationCenter::sharedNotificationCenter()->postNotification(FASTLOGINFLAG,sdkLoginData, false);
}

void SdkHandler::destroyGuestFinished(char *bufferchar)
{
	CCLOG("in function:[%s], buff:[%s]", __FUNCTION__, bufferchar);
	
    int state = 0;
    int userId = 0;
    int flag = 0;
    JSONNODE *n = json_parse(bufferchar);
    if (n == NULL){
        return;
    }
    JSONNODE_ITERATOR i = json_begin(n);
    while (i != json_end(n)){
		CCLOG("Start Parse Json in [%s]", __FUNCTION__);
		
        if (*i == NULL){
            break;
        }
        
        // recursively call ourselves to dig deeper into the tree
        if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE){
            break;
        }
        
        // get the node name and value as a string
        json_char *node_name = json_name(*i);
        
        // find out where to store the values
        if (strcmp(node_name, "STATE") == 0){
            json_int_t node_value = json_as_int(*i);
            state = node_value;
        }
        else if (strcmp(node_name, "USER_ID") == 0){
            json_int_t node_value = json_as_int(*i);
            userId = node_value;
            
        }
        else if (strcmp(node_name, "LOGIN_KEY") == 0){
            json_char *node_value = json_as_string(*i);
            json_free(node_value);
            
        }
        else if (strcmp(node_name, "ERROR_TYPE") == 0){
            json_int_t node_value = json_as_int(*i);
            flag = node_value;
            
        }
        // cleanup and increment the iterator
        json_free(node_name);
        ++i;
    }
	
}
void SdkHandler::requestFailed()
{
    //    SG_SHOW_WINDOW("请检查您的网络连接哦！  亲！！");
    //SGMainManager::shareMain()->showConnectTip();
}
void SdkHandler::checkUserName(char *bufferchar)
{
    
}

void SdkHandler::activite(char *bufferchar)
{
    
}


SdkInfoData::SdkInfoData()
:state(0),
errorFlag(0),
accountId(0),
flag(0),
isEx(false)
{
    
}

SdkInfoData::~SdkInfoData()
{
    
}






