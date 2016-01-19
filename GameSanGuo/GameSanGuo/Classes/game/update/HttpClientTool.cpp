#include "HttpClientTool.h"
#include <string>

HttpClientTool* HttpClientTool::mInst = NULL;

HttpClientTool* HttpClientTool::sharedHttpClient()
{
    if ( !mInst )
    {
        mInst = new HttpClientTool();
    }
    
    return mInst;
}

HttpClientTool::HttpClientTool() 
{
    ;
}

HttpClientTool::~HttpClientTool()
{
    CCHttpClient::getInstance()->destroyInstance();
}

void HttpClientTool::HttpPost(CCObject* _obj, const char* _url, const char* _postdata, SEL_HttpResponse _selector, const char* _tag, int _timeout, bool isshowloading)
{
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(_url);
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setResponseCallback(_obj, _selector);
    request->setRequestData(_postdata, strlen(_postdata));
    request->setTag(_tag);
    CCHttpClient::getInstance()->setTimeoutForConnect(_timeout);
    CCHttpClient::getInstance()->send(request);
    request->release();
    
}


void HttpClientTool::HttpGet(CCObject* _obj, const char* _url, SEL_HttpResponse _selector, const char* _tag, int _timeout)
{
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(_url);
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(_obj, _selector);
    request->setTag(_tag);
    CCHttpClient::getInstance()->setTimeoutForConnect(_timeout);
    CCHttpClient::getInstance()->send(request);
    request->release();
}




