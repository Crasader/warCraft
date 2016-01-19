/****************************************************************************
 Copyright 2012 ourpalm Inc.;
 
 modify by canglang
 
 使用本接口做http联网
 使用范例为如下所示：
 HttpClientTool ::sharedHttpClient()->HttpGet(this,"http://117.79.148.42:8080/nVz/notice",HTS(HelloWorld::HttpCompleted),HTS(HelloWorld::Httpfailed),"1");
 第一个参数传使用处的ccnode节点，
 第二个参数传欲连接的url地址
 第三个参数传连接成功的回调函数
 第四个参数传连接失败的回调函数
 第五个参数传该连接的Tag标志
 第六个参数设置本次连接的timeout时间
 
 注意：
 自定义个回调函数必须带两个参数如下例子所示：
 void HelloWorld::HttpCompleted(CCNode* _node , CCHttpResponse *data1)
 {
    if (!data)
    {
        return;
    }
 
    //数据包组成字节流
    std::vector<char> *vebuf = data->getResponseData();
    int veLen = vebuf->size();
    if ( veLen <= 0 )
    {
        return;
    }
 
    char* buf = new char[veLen];
    for (unsigned int i = 0; i < veLen; i++)
    {
        *(buf+i) = (*vebuf)[i];
    }
 }
 ****************************************************************************/
#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpRequest.h"
#include "HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define HTS httpresponse_selector 
#define TIMEOUT 10

class HttpClientTool
{
protected:
    HttpClientTool();
    static HttpClientTool* mInst;
    
public:
    virtual ~HttpClientTool();
    static HttpClientTool* sharedHttpClient();
    
public:

    void HttpPost(CCObject* _obj, const char* _url, const char* _postdata, SEL_HttpResponse _selector, const char* _tag, int _timeout = TIMEOUT, bool isshowloading = true);
    void HttpGet(CCObject* _obj, const char* _url, SEL_HttpResponse _selector, const char* _tag, int _timeout = TIMEOUT);
};

#endif //__HTTPREQUESTHTTP_H
