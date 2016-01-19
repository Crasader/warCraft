//
//  FMUIWebViewBridge.m
//  test1
//
//  Created by 陈雪龙 on 13-4-25.
//
//

//

//  FMUIWebViewBridge.cpp

//  WebViewDemo

//

//  Created by Yanghui Liu on 12-6-5.

//  Copyright (c) 2012年 FMinutes company. All rights reserved.

//

#import "FMUIWebViewBridge.h"

#import "EAGLView.h"

@implementation FMUIWebViewBridge

- (id)init{
    
    self = [super init];
    
    if (self) {
        
        // init code here.
        
    }
    
    return self;
    
}

- (void)dealloc{
    
    [mBackButton release];
    
    [mToolbar release];
    
    [mWebView release];
    
    [mView release];

    [super dealloc];
    
}

-(void) setLayerWebView : (FMLayerWebView*) iLayerWebView URLString:(const char*) urlString{
    
    mLayerWebView = iLayerWebView;
    
    cocos2d::CCSize size = mLayerWebView-> getContentSize();
    
    mView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 300 , size.height)];
    mLayerWebView->setPosition(CCPointZero);
    // create webView
    
    //Bottom size
    
    //int wBottomMargin = size.height*0.10;
    
    //int wWebViewHeight = size.height - wBottomMargin;
    
    mWebView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, 550, 200)];
//    [mWebView setBounds:CGRectMake(0, 0, 300, 200)];
    [mWebView setScalesPageToFit:NO];

    mWebView.delegate = self;
    
    NSString *urlBase = [NSString stringWithCString:urlString encoding:NSUTF8StringEncoding];
    
    [mWebView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlBase ]]];
    
    [mWebView setUserInteractionEnabled:NO]; //don't let the user scroll while things are
    
    //create a tool bar for the bottom of the screen to hold the back button
    
//    mToolbar = [UIToolbar new];
//    
//    [mToolbar setFrame:CGRectMake(0, wWebViewHeight, size.width, wBottomMargin)];
//    
//    mToolbar.barStyle = UIBarStyleBlackOpaque;
    
    //Create a button
    
//    mBackButton = [[UIBarButtonItem alloc] initWithTitle:@"Back"
//                   
//                                                   style: UIBarButtonItemStyleDone
//                   
//                                                  target: self
//                   
//                                                  action:@selector(backClicked:)];
//    
//    [mToolbar setItems:[NSArray arrayWithObjects:mBackButton,nil] animated:YES];
//    
//    [mView addSubview:mToolbar];
    
    //[mToolbar release];
    
    // add the webView to the view
    [mWebView setBackgroundColor:[UIColor clearColor]];
    [mWebView setOpaque:NO];
    [mView addSubview:mWebView];
    [mView setCenter:CGPointMake(150, 680)];
    [[EAGLView sharedEGLView] addSubview:mView];
    
}

- (void)webViewDidStartLoad:(UIWebView *)thisWebView {
    
}
- (void)webViewDidFinishLoad:(UIWebView *)webView{
    
    [mWebView setUserInteractionEnabled:NO];
    CGFloat webViewHeight = 0.0f;
    if (webView.subviews.count > 0)
    {
        UIView *scrollerView = [webView.subviews objectAtIndex:0];//为什么要取第一个？
        if (scrollerView.subviews.count > 0)
        {
            UIView *webDocView = scrollerView.subviews.lastObject;
            if ([webDocView isKindOfClass:[NSClassFromString(@"UIWebDocumentView") class]])
            {
                webViewHeight = webDocView.frame.size.height;//获取文档的高度
                webView.frame= webDocView.frame; //更新UIWebView 的高度
                
                [mWebView setFrame:CGRectMake(0, 0, 485, webViewHeight)];
//                aScrollView.contentSize=CGSizeMake(485, 240+webViewHeight);//调整底层的contensize 的大小
            }
        }
    }
    mLayerWebView->webViewDidFinishLoad(webViewHeight);

    
}



- (void)webView:(UIWebView *)thisWebView didFailLoadWithError:(NSError *)error {
    
    if ([error code] != -999 && error != NULL) { //error -999 happens when the user clicks on something before it's done loading.
        
        
        
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Network Error" message:@"Unable to load the page. Please keep network connection."
                              
                                                       delegate:self cancelButtonTitle:nil otherButtonTitles:@"OK", nil];
        
        [alert show];
        
        [alert release];
        
        
        
    }
    
}



//-(void) backClicked:(id)sender {
//    
//    mWebView.delegate = nil; //keep the webview from firing off any extra messages
//    
//    //remove items from the Superview...just to make sure they're gone
//    
//    [mToolbar removeFromSuperview];
//    
//    [mWebView removeFromSuperview];
//    
//    [mView removeFromSuperview];
//    
//    mLayerWebView->onBackbuttonClick();
//    
//}
//
-(void)removeMyself
{
    mWebView.delegate = nil; //keep the webview from firing off any extra messages
    
    //remove items from the Superview...just to make sure they're gone
    
    [mToolbar removeFromSuperview];
    
    [mWebView removeFromSuperview];
    
    [mView removeFromSuperview];

}

@end
