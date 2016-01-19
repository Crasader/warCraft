//
//  FMLayerWebView.cpp
//  test1
//
//  Created by 陈雪龙 on 13-4-25.
//
//

//

//  FMLayerWebView.mm

//  WebViewDemo

//

//  Created by Yanghui Liu on 12-6-5.

//  Copyright (c) 2012年 FMinutes company. All rights reserved.

//



#include "FMLayerWebView.h"

#include "FMUIWebViewBridge.h"



static FMUIWebViewBridge *g_FMUIWebViewBridge=nil;



FMLayerWebView::FMLayerWebView(){
    
}

FMLayerWebView::~FMLayerWebView()
{
    
    [g_FMUIWebViewBridge release];
    this->removeFromParentAndCleanup(true);
}

FMLayerWebView* FMLayerWebView::create(CCString *str)
{
    FMLayerWebView *Layer = new FMLayerWebView();
   
    if (Layer && Layer->init())
    {
        Layer->autorelease();
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}


void FMLayerWebView::webViewDidFinishLoad(int height)
{
//    this->setContentSize(CCSize(this->getContentSize().width, height));
}
void FMLayerWebView::onBackbuttonClick(){
    
    [g_FMUIWebViewBridge removeMyself];
}

bool FMLayerWebView::init(){
    
    if ( !CCLayer::init() ){
        
        return false;
    }
    g_FMUIWebViewBridge = [[FMUIWebViewBridge alloc] init];
//    [g_FMUIWebViewBridge setLayerWebView : this URLString:"http://sg-gate.gamebean.com:8084/help/help_iphone/"];

    [g_FMUIWebViewBridge setLayerWebView : this URLString:"http://117.79.91.57:8084/help/2.html"];
    return true;
}
