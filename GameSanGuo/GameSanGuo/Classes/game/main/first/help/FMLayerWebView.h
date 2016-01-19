//
//  FMLayerWebView.h
//  test1
//
//  Created by 陈雪龙 on 13-4-25.
//
//

//

//  FMLayerWebView.h

//  WebViewDemo

//

//  Created by Yanghui Liu on 12-6-5.

//  Copyright (c) 2012年 FMinutes company. All rights reserved.

//



#ifndef WebViewDemo_FMLayerWebView_h

#define WebViewDemo_FMLayerWebView_h



#include "CCCommon.h"

#include "cocos2d.h"

USING_NS_CC;

class FMLayerWebView : public CCLayer{
    
public:
    
    FMLayerWebView();
    
    ~FMLayerWebView();
    
    virtual bool init();
    
//    CREATE_FUNC(FMLayerWebView);
    static FMLayerWebView *create(CCString *str);

    void webViewDidFinishLoad(int height);
    
    void onBackbuttonClick();
    
private:
    
    int mWebViewLoadCounter;
    
};


#endif


