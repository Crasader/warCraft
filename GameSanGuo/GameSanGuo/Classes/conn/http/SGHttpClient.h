//
//  SGHttpClient.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-26.
//
//

#ifndef __GameSanGuo__SGHttpClient__
#define __GameSanGuo__SGHttpClient__

#include "cocos2d.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpClient.h"
#include "SGHTTPDelegate.h"


//不再由这里指定，统一由GlobalConfig里配置，这里留底参考 ￥_$

//内网测试 1
//外网测试 2
//台湾ios正式 3
//台湾andirod正式 4
//内地正式 5
//#define WHICH_SERVER    4
//
//#if (WHICH_SERVER == 1)
//
//    #define HTTPURL "http://192.168.0.16:8080/SanguoLoginPortal/clientServlet"//内网测试
////    #define HTTPURL "http://192.168.0.95:8080/SanGuoLoginPortal/clientServlet"//PVP测试
//    #define OWNTALKDATA  1
//
//#elif (WHICH_SERVER == 2)
//
//    #define HTTPURL "http://210.65.221.98:10020/SanguoLoginPortal/clientServlet"//外网测试
//    #define OWNTALKDATA  2
//
//#elif (WHICH_SERVER == 3)
//
//    #define HTTPURL "http://herologinios.go2play.mobi:8080/SanguoLoginPortal/clientServlet"//台湾ios
//    #define OWNTALKDATA  2
//
//#elif (WHICH_SERVER == 4)
//
//    #define HTTPURL "http://herologinandriod.go2play.mobi:8080/SanguoLoginPortal/clientServlet"//台湾andirod
//    #define OWNTALKDATA  2
//
//#elif (WHICH_SERVER == 5)
//
//    #define HTTPURL "http://s10.xdyou.cn:8080/SanguoLoginPortal/clientServlet"//内地app正式
//    #define OWNTALKDATA  1
//
//#else//没指定的情况默认是内网测试
//
//    #define HTTPURL "http://192.168.0.16:8080/SanguoLoginPortal/clientServlet"//内网测试
//    #define OWNTALKDATA  1
//
//#endif


#define HTTPLENGHT 200
USING_NS_CC;
USING_NS_CC_EXT;
struct LoginData
{
    char *name;
    char *password;
};

class SGRequestData
{
public:
    SGRequestData();
    ~SGRequestData();
    
    char * getRequestData();
    int getRequestLength();
    char getMsgId();
    void setMsgId(char msgId_);
    void writeString(const char *str);
    void writeShort(short s);
private:
    int toAdd;
    char msgId;
    unsigned char *requestData;
};
class SGResponseData
{
private:
    int toAdd;
    char *responseData;
public:
    SGResponseData(char *data);
    ~SGResponseData();
    char readChar();
    const char* readString();
    short readShort();
};
class SGHttpClient : public CCObject
{
private:
    SGHTTPDelegate *_delegate;
private:
    
    SGHttpClient();
    void postData(SGRequestData *data , bool isShowCon=true);
    
    void requestFinishHandler(CCNode *sender, void *data);
    
    void fastLoginListener(char *buffer);
    void loginListener(char *buffer);
    void serverListListener(char *buffer);
    void registerListener(char *buffer);
    void bindListener(char *buffer);
    void passwordListener(char *buffer);
    void getlastestServer(char *buffer);

public:
    static SGHttpClient *shareHttpClient();
    bool isInternetConnectionAvailable(void);
    void fastLogin(SGHTTPDelegate *delegate, bool isFirst );
    void login(SGHTTPDelegate *delegate, const char* name,const char * pas, short sid);
    void goRegister(SGHTTPDelegate *delegate, const char*un, const char *pw);
    void getServerList(SGHTTPDelegate *deletage,std::string &userId,int channelId);
    void getData();
    void bind(SGHTTPDelegate *delegate,const char*account,const char*password1,const char*password2);
    void password(SGHTTPDelegate *delegate,const char*account,const char*oldPassword,const char* newPassword);
    void getlastestServer(SGHTTPDelegate *delegate);
    //每次打开游戏app 即发送此消息  用于服务器信息统计。
    void sendStartApp();
    //点击进入游戏时，发送消息20号 更新选中的服务器状态
    void updateSelectServer(SGHTTPDelegate  *, short serverId);
    //收到20号消息的响应
    void updateSelectServerResponse(char * buf);
    
    //与服务器交互，获取user_id。sid=从合作方sdk取得的标识。
    void reqGetChannelUserId(SGHTTPDelegate* delegate, const char* sid);
    
    //获取user_id回应。主要是存入回执中的userId(accountId)等参数。
    void rspGetChannelUserId(char* buffer);
};

#endif /* defined(__GameSanGuo__SGHttpClient__) */
