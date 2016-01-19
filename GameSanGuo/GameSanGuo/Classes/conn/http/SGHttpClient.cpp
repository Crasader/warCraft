//
//  SGHttpClient.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-26.
//
//

#include "SGHttpClient.h"
#include "SGMsgId.h"
#include "SGRegisterSuccess.h"
#include "SGPlayerInfo.h"
#include "SGMainManager.h"
#include "SGConnectTipBox.h"
#include "PlatformAdp.h"
#include "GlobalConfig.h"
#if (PLATFORM == ANDROID)
#include <netinet/in.h>
#endif

static SGHttpClient *shareInter = NULL;

SGHttpClient::SGHttpClient():
_delegate(NULL)
{
}
SGHttpClient *SGHttpClient::shareHttpClient()
{
    if (!shareInter)
    {
        shareInter = new SGHttpClient();
    }
    return shareInter;
}
bool SGHttpClient::isInternetConnectionAvailable(void)
{
	//    return [[Reachability_objc reachabilityForInternetConnection] currentReachabilityStatus] != NotReachable;
    return true;
}

void SGHttpClient::postData(SGRequestData *data,bool isShowCon)
{
    if ( isShowCon )
        SGMainManager::shareMain()->getMainScene()->plat->showConnection();
    
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(GlobalConfig::gi()->getAuthUrl().c_str());
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setRequestData(data->getRequestData(), data->getRequestLength());
    CCHttpClient::getInstance()->setTimeoutForConnect(20);
    int _tag = 0;
    char ctag = data->getMsgId();
    
    memcpy(&_tag, &ctag, 1);
    
    char requestTag[1];
    sprintf(requestTag, "%d",_tag);
    request->setTag(requestTag);
    
    request->setResponseCallback(this, callfuncND_selector(SGHttpClient::requestFinishHandler));
    CCHttpClient::getInstance()->send(request);
    request->release();
}
void SGHttpClient::login(SGHTTPDelegate *delegate, const char* name,const char * pas, short sid)
{
	/**/
    CCLOG("SGHttpClient::login");
    _delegate = delegate;
    
    char msgId = MSG_HTTP_LOGIN;
	
    SGRequestData *pd = new SGRequestData();
    pd->setMsgId(msgId);
    //pd->writeShort(accountId);
    pd->writeString(name);
    pd->writeString(pas);
    pd->writeShort(sid);
    this->postData(pd);
    
    delete pd;
}
void SGHttpClient::fastLogin(SGHTTPDelegate *delegate, bool isFirst)
{
    _delegate = delegate;
	
    SGRequestData *pd = new SGRequestData();
    pd->setMsgId(MSG_HTTP_FAST_LOGIN);
    pd->writeString(GameConfig::getUdit().c_str());
    this->postData(pd);
    
    delete pd;
}

void SGHttpClient::bind(SGHTTPDelegate *delegate,const char*account,const char*password1,const char*password2)
{
    _delegate = delegate;
    
	//SGRequestData *pd = new SGRequestData();
	
    std::string un = CCUserDefault::sharedUserDefault()->getStringForKey("username");
    std::string pw = CCUserDefault::sharedUserDefault()->getStringForKey("password");
    
    SGRequestData *re = new SGRequestData();
    re->setMsgId(MSG_HTTP_BING);
    
    //当前用户名密码
    re->writeString(GameConfig::getUdit().c_str());
    re->writeString(GameConfig::getUdit().c_str());
    
    //新用户名密码
    re->writeString(account);
    re->writeString(password1);
    re->writeString(password2);
    
    this->postData(re);
    
    delete re;
}

void SGHttpClient::password(SGHTTPDelegate *delegate,const char*account,const char*oldPassword,const char* newPassword)
{
    _delegate = delegate;
    
    SGRequestData *re = new SGRequestData();
    re->setMsgId(MSG_HTTP_PASSWORD);
    re->writeString(account);
    re->writeString(oldPassword);
    re->writeString(newPassword);
    
    this->postData(re);
    
    delete re;
    
}


void SGHttpClient::goRegister(SGHTTPDelegate *delegate, const char*un, const char *pw)
{
    _delegate = delegate;
    
    char msgId = MSG_HTTP_REGISTER;
    
    SGRequestData *pd = new SGRequestData();
    pd->setMsgId(msgId);
    pd->writeString(un);
    pd->writeString(pw);
    
    this->postData(pd);
    
    delete pd;
}
void SGHttpClient::getServerList(SGHTTPDelegate *delegate,std::string &userId,int channelId)
{
    _delegate = delegate;
    char msgId = MSg_HTTP_SERVERLIST;
    short channelIds = (short)channelId;
    
    SGRequestData *pd = new SGRequestData();
    pd->setMsgId(msgId);
    
    pd->writeString(userId.c_str());
    
    //channle
    pd->writeShort(channelIds);
    
    //longitype
    //pd->writeShort(loginTypes);
    
    this->postData(pd);
    delete pd;
}

void SGHttpClient::getlastestServer(SGHTTPDelegate *delegate)
{
    _delegate = delegate;
    char msgId = MSg_HTTP_LASTESTSERVER;
    SGRequestData *re = new SGRequestData();
    re->setMsgId(msgId);
    this->postData(re);
    delete re;
}

void SGHttpClient::getlastestServer(char *buffer)
{
    //获取最新服务器 18
    //字符串 服务器ip string
    //端口post short
    //服务器id short
    //服务器名字 string
    //服务器当前状态具体描述 string
	
    CCDictionary *dict = CCDictionary::create();
    SGResponseData *response = new SGResponseData(buffer);
	
    CCString *serverIp = CCString::create(response->readString());
    CCString *serverPost = CCString::createWithFormat("%d",response->readShort());
    CCString *serverId = CCString::createWithFormat("%d",response->readShort());
    CCString *serverIsNew = CCString::createWithFormat("%d",response->readShort());
    CCString *serverName = CCString::create(response->readString());
    CCString *serverDes = CCString::create(response->readString());
    CCString *serverShowId = CCString::createWithFormat("%d",response->readShort());
    dict->setObject(serverIp, "serverIp");
    dict->setObject(serverPost, "serverPost");
    dict->setObject(serverId, "serverId");
    dict->setObject(serverIsNew, "serverIsNew");
    dict->setObject(serverName, "serverName");
    dict->setObject(serverDes, "serverDes");
    //MM: 添加server的显示ID。
    dict->setObject(serverShowId, "serverShowId");
    delete response;
    if(_delegate)
    {
        _delegate->requestFinished(MSg_HTTP_LASTESTSERVER, dict);
    }
    
}

void SGHttpClient::fastLoginListener(char *buffer)
{
    CCDictionary *loginDict = CCDictionary::create();
    SGResponseData *rData = new SGResponseData(buffer);
    char code = rData->readChar();
	
    CCLOG("http code :%d",code);
    if (code == 0)
    {
        if (_delegate)
        {
            _delegate->requestFailed();
        }
        delete rData;
        return;
    }
    /*
	 const char *un = rData->readString();
	 CCString *username = CCString::create(un);
	 loginDict->setObject(username, "username");
	 delete []un;
	 
	 const char *psw = rData->readString();
	 CCString *password = CCString::create(psw);
	 loginDict->setObject(password, "password");
	 delete []psw;
	 
	 delete rData;
	 */
    if (_delegate)
    {
        _delegate->requestFinished(MSG_HTTP_FAST_LOGIN,loginDict);
    }
    delete rData;
}
void SGHttpClient::loginListener(char *buffer)
{
    CCDictionary *loginDict = CCDictionary::create();
    SGResponseData *rData = new SGResponseData(buffer);
	
    int code = rData->readChar();
    CCLOG("http code :%d",code);
    
    if (code == 0)
    {
        int tip = rData->readChar();
        CCString *tipString = CCString::create(CCString::createWithFormat("%d",tip)->getCString());
        loginDict->setObject(tipString, "error");
        if (_delegate)
        {
            _delegate->requestFinished(MSG_HTTP_LOGIN_ERROR,loginDict);
            delete rData;
            return;
        }
    }
    
	//    const char *randUserName = rData->readString();
	//    CCString *userName = CCString::create(randUserName);
	//    loginDict->setObject(userName, "randUserName");
    
    if (_delegate)
    {
        _delegate->requestFinished(MSG_HTTP_LOGIN,loginDict);
    }
    delete rData;
}
void SGHttpClient::serverListListener(char *buffer)
{
    CCLOG("下行：服务器列表,数据长度 %ld",sizeof(buffer));
    
    CCArray *serverList = CCArray::create();
    
    SGResponseData *rData = new SGResponseData(buffer);
    if(rData->readChar()==0){
        CCLOG("获取服务器列表失败");
        delete rData;
        return;
    }
    
    short serverNum = rData->readShort();
    for (int i = 0; i<serverNum; i++)
    {
        CCDictionary *serverDict = CCDictionary::create();
        
        //服务器Id
        short serverId = rData->readShort();
        serverDict->setObject(CCString::createWithFormat("%d", serverId), "serverId");
        //是否为新服
        short serverIsNew = rData->readShort();
        serverDict->setObject(CCString::createWithFormat("%d",serverIsNew), "serverIsNew");
		
        //服务器名称
        const char *serverName = rData->readString();
        serverDict->setObject(CCString::create(serverName), "serverName");
        
        //服务器ip地址
        const char *serverIp = rData->readString();
        serverDict->setObject(CCString::create(serverIp), "serverIp");
        
        //服务器端口号
        short serverPost = rData->readShort();
        serverDict->setObject(CCString::createWithFormat("%d", serverPost), "serverPost");
        
        //服务器状态
        char serverState = rData->readChar();
        serverDict->setObject(CCString::createWithFormat("%d", serverState), "serverState");
        
        //是否有角色
        char isHasRole = rData->readChar();
        serverDict->setObject(CCString::createWithFormat("%d", isHasRole), "isHasRole");
        //0.没有角色 1.有角色
        if (isHasRole == 1)
        {
            //昵称
            const char * roleNikeName = rData->readString();
            serverDict->setObject(CCString::create(roleNikeName), "roleNikeName");
			
            //等级
            short  roleLevel = rData->readShort();
            serverDict->setObject(CCString::createWithFormat("%d", roleLevel), "roleLevel");
            
            //国家
            short countryId = rData->readShort();
            serverDict->setObject(CCString::createWithFormat("%d", countryId), "countryId");
        }
        //add by:zyc.服务器详细的状态信息。
        const char * serverDes = rData->readString();
        serverDict->setObject(CCString::create(serverDes), "serverDes");
        
        //MM: serverShowId显示其为几区，与前面的serverId不同，这里只是显示。例如serverId为2009，serverShowId可能为9。
        short serverShowId = rData->readShort();
        serverDict->setObject(CCString::createWithFormat("%d", serverShowId), "serverShowId");
        
        //添加到服务器列表。
        serverList->addObject(serverDict);
        SGPlayerInfo::sharePlayerInfo()->addServerList(serverList);
        
    }
    delete rData;
    if (_delegate)
    {
        _delegate->requestFinished(MSg_HTTP_SERVERLIST, serverList);
    }
}

void SGHttpClient::registerListener(char *buffer)
{
    SGResponseData *rData = new SGResponseData(buffer);
    char ch = rData->readChar();
    //1成功 0失败
    CCString *string = CCString::createWithFormat("%d",ch);
    if(_delegate)
    {
        _delegate->requestFinished(MSG_HTTP_REGISTER, string);
    }
    delete rData;
}

void SGHttpClient::bindListener(char *buffer)
{
    SGResponseData *rData = new SGResponseData(buffer);
    
    char ch = rData->readChar();
    short type = rData->readShort();
    
    CCString *string1 = CCString::createWithFormat("%d",ch);
    CCString *string2 = CCString::createWithFormat("%d",type);
    CCDictionary *dict = CCDictionary::create();
    dict->setObject(string1, "string1");
    dict->setObject(string2, "string2");
	
    delete rData;
    if(_delegate)
    {
        _delegate->requestFinished(MSG_HTTP_BING, dict);
    }
}

void SGHttpClient::passwordListener(char *buffer)
{
    SGResponseData *rData = new SGResponseData(buffer);
    
    CCDictionary *dict = CCDictionary::create();
    char ch = rData->readChar();
    CCString *isSuccess = CCString::createWithFormat("%d",ch);
    short f = rData->readShort();
    CCString *flag = CCString::createWithFormat("%d",f);
    
    dict->setObject(isSuccess, "isSccess");
    dict->setObject(flag, "flag");
    
    delete rData;
    if(_delegate)
    {
        _delegate->requestFinished(MSG_HTTP_PASSWORD, dict);
    }
    
    /*
	 1没有帐号
	 2新旧密码不相同
	 3没修改成功
	 */
}



void SGHttpClient::requestFinishHandler(CCNode *sender, void *data)
{
    CCHttpResponse *response = (CCHttpResponse*)data;
    
    if (!response)
    {
        return;
    }
    
    int statusCode = response->getResponseCode();
    CCLog("response code: %d", statusCode);
    if (!response->isSucceed())
    {
        SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
		//        弹框
        if (_delegate)
        {
            _delegate->requestFailed();
        }
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        return;
    }
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        const char *responseTag = response->getHttpRequest()->getTag();
        CCLog("%s completed", responseTag);
        
        int rt = atoi(responseTag);
        std::vector<char> *buffer = response->getResponseData();
        int bufferSize = buffer->size();
        char *bufferchar = new char[bufferSize];
        std::copy(buffer->begin(), buffer->end(), bufferchar);
		
        switch (rt)
        {
            case MSG_HTTP_FAST_LOGIN:
            {
                this->fastLoginListener(bufferchar);
                break;
            }
            case MSG_HTTP_LOGIN:
            {
                this->loginListener(bufferchar);
                break;
            }
            case MSG_HTTP_REGISTER:
            {
                this->registerListener(bufferchar);
                break;
            }
            case MSg_HTTP_SERVERLIST:
            {
                this->serverListListener(bufferchar);
                break;
            }
            case MSG_HTTP_BING:
            {
                bindListener(bufferchar);
                break;
            }
            case MSG_HTTP_PASSWORD:
            {
                passwordListener(bufferchar);
                break;
            }
            case MSg_HTTP_LASTESTSERVER:
            {
                getlastestServer(bufferchar);
                break;
            }
            case MSG_HTTP_UPDATE_SERVER_STATE:
            {
                updateSelectServerResponse(bufferchar);
            }
                break;
            case MSG_HTTP_GET_CHANNEL_USER_ID:
            {
                rspGetChannelUserId(bufferchar);
            }
                break;
            default:
                break;
        }
		
    }
    CCUserDefault::sharedUserDefault()->setBoolForKey("pvp_tran", false);
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
	
}

SGRequestData::SGRequestData()
:toAdd(0),
requestData(NULL),
msgId(0)
{
    requestData = new unsigned char[HTTPLENGHT];
}
SGRequestData::~SGRequestData()
{
    CCLOG("~SGRequestData");
    CC_SAFE_DELETE_ARRAY(requestData);
}
char *SGRequestData::getRequestData()
{
    return (char *)requestData;
}
int SGRequestData::getRequestLength()
{
    return toAdd;
}
void SGRequestData::setMsgId(char msgId_)
{
    msgId = msgId_;
    memcpy(requestData, &msgId, 1);
    toAdd += 1;
}
char SGRequestData::getMsgId()
{
    return msgId;
}
void SGRequestData::writeShort(short s)
{
    short h_s = htons(s);
    memcpy(requestData+toAdd, &h_s, 2);
    toAdd +=2;
}
void SGRequestData::writeString(const char *str)
{
    unsigned short sl = strlen(str);
    this->writeShort(sl);
    memcpy(requestData+toAdd, str, sl);
    toAdd+= sl;
}
SGResponseData::SGResponseData(char *data)
:toAdd(0)
{
    responseData = data;
    
}
SGResponseData::~SGResponseData()
{
    CCLOG("~SGResponseData");
    CC_SAFE_DELETE_ARRAY(responseData);
}
char SGResponseData::readChar()
{
    char charnum = 0;
    memset(&charnum, 0, 1);
    memcpy(&charnum, responseData + toAdd, 1);
    toAdd+=1;
    return charnum;
}
const char * SGResponseData::readString()
{
    short strlen = this->readShort();
    char *str = new char[strlen+1];
    memset(str, 0, strlen);
    memcpy(str, responseData+toAdd, strlen);
    str[strlen] = '\0';
    toAdd+= strlen;
    return str;
}
short SGResponseData::readShort()
{
    short shortnum = 0;
    memcpy(&shortnum, responseData + toAdd, 2);
    shortnum = htons(shortnum);
    toAdd += 2;
    return shortnum;
}


void SGHttpClient::sendStartApp()
{
    
    char msgId = MSg_HTTP_OPENAPP;
    SGRequestData *re = new SGRequestData();
    re->setMsgId(msgId);
    re->writeString(GameConfig::getUdit().c_str());
    this->postData(re , false);
    delete re;

}

void SGHttpClient::updateSelectServer(SGHTTPDelegate *delegate , short serverId)
{
    _delegate = delegate;
    
    char msgId = MSG_HTTP_UPDATE_SERVER_STATE;
    SGRequestData *re = new SGRequestData();
    re->setMsgId(msgId);
    re->writeShort(serverId);
    this->postData(re , true);
    delete re;
}

void SGHttpClient::updateSelectServerResponse(char *buf)
{
    //获取最新服务器 20
    //字符串 服务器ip string
    //端口post short
    //服务器id short
    //服务器名字 string
    //服务器当前状态具体描述 string
	
    CCDictionary *dict = CCDictionary::create();
    SGResponseData *response = new SGResponseData(buf);
	
    CCString *serverIp = CCString::create(response->readString());
    CCString *serverPost = CCString::createWithFormat("%d",response->readShort());
    CCString *serverId = CCString::createWithFormat("%d",response->readShort());
    CCString *serverIsNew = CCString::createWithFormat("%d",response->readShort());
    CCString *serverName = CCString::create(response->readString());
    CCString *serverState = CCString::createWithFormat("%d"  , response->readChar() );
    CCString *serverDes = CCString::create(response->readString());
    CCString *serverShowId = CCString::createWithFormat("%d",response->readShort());
    dict->setObject(serverIp, "serverIp");
    dict->setObject(serverPost, "serverPost");
    dict->setObject(serverId, "serverId");
    dict->setObject(serverIsNew, "serverIsNew");
    dict->setObject(serverName, "serverName");
    dict->setObject(serverState, "serverState");
    dict->setObject(serverDes, "serverDes");
    //MM: 添加server的显示ID。
    dict->setObject(serverShowId, "serverShowId");
    delete response;
    if(_delegate)
    {
        _delegate->requestFinished(MSG_HTTP_UPDATE_SERVER_STATE, dict);
    }
   
}

//与服务器交互，获取user_id
void SGHttpClient::reqGetChannelUserId(SGHTTPDelegate* delegate, const char* sid)
{
    _delegate = delegate;
    
    char msgId = MSG_HTTP_GET_CHANNEL_USER_ID;
    
    SGRequestData *pd = new SGRequestData();
    pd->setMsgId(msgId);
    pd->writeShort(GAME_CHANNEL_TYPE);
    pd->writeString(sid);
    
    this->postData(pd);
    
    delete pd;
}

//获取user_id回应
void SGHttpClient::rspGetChannelUserId(char* buffer)
{
    SGResponseData *rData = new SGResponseData(buffer);
    char code = rData->readChar();
	
    CCLOG("http code :%d",code);
    if (code == 0)
    {
        if (_delegate)
        {
            _delegate->requestFailed();
        }
        delete rData;
        return;
    }
    
    CCDictionary *dict = CCDictionary::create();
    
    const char* sdkAiPsz = rData->readString();
    const char* sdkCPsz = rData->readString();
    const char* sdkNnPsz = rData->readString();
    dict->setObject(CCString::create(sdkAiPsz), "sdkAccountId");
    dict->setObject(CCString::create(sdkCPsz), "sdkCreator");
    dict->setObject(CCString::create(sdkNnPsz), "sdkNickName");
    CCLOG("rspGetChannelUserId: ai=%s, c=%s, nn=%s", sdkAiPsz, sdkCPsz, sdkNnPsz);
    
    if (_delegate)
    {
        _delegate->requestFinished(MSG_HTTP_GET_CHANNEL_USER_ID, dict);
    }
    delete rData;
}

