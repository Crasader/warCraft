//
//  SGLoginLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-27.
//
//

#ifndef __GameSanGuo__SGLoginLayer__
#define __GameSanGuo__SGLoginLayer__

#include "SGBaseLayer.h"
#include "SGHTTPDelegate.h"
#include "SGBoxDelegate.h"
#include "cocos-ext.h"
#include "native/CCAlertViewDelegate.h"

USING_NS_CC_EXT;

class SGWelComeLayer : public SGBaseLayer, public SGHTTPDelegate, public CCAlertViewDelegate
{
private:
    bool isstart;
    CCDictionary *m_pServerDict;
    SGButton *changeServerbtn;
    bool canStart;
    SGButton *facebookBt;
    SGButton *facebookLogOut;
    SGButton *guestBidBtn;
    std::string serverDes;
    SGButton *startBtn;
   // CCLayerColor * m_blackColor;
public:
    SGButton *registerBtn;
    CCSprite *logo;
private:
    void changeIDHandler();
    void registerHandler();
    void bindGuestHandler();
    void logOutFB();
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void showFBBindBox();
public:
    
    SGWelComeLayer();
    ~SGWelComeLayer();
    void loginstart();
    void doFacebookBind();
    void startFacebookLogin();
    void showbox();
    static SGWelComeLayer *create();
    virtual void initMsg();
    
    virtual void showBoxCall(CCObject *obj);
    virtual void alertViewClickedButtonAtIndex(int buttonIndex);
    void resetCanStart();
    void serverListCallBack(CCDictionary *serverDict);
    void refreshRegisterBtn();
    void fastStartHandler();
	//切换帐号
	void exchangeHandler();
    
    void updateViewForFB();
    void clearGust();
    void doFbBindFail();
public:
    virtual void requestFinished(int msgid, CCObject *data);
    virtual void requestFailed();

    //统一执行逻辑，主要因为SGServerListNewServerBox对登陆的流程与本UI不一样。
    void realLoginProcess();
    //点击“登陆”按钮之后的逻辑，封装后可用于SDK统一登陆。
    void loginGameProcess();
    //設置registerBtn的文本，以走順註冊/登陸邏輯。
    void setRegisterBtnText(const std::string& text);
    
    //sdk的facebook界面更新
    void updateViewFBForSdk(bool isLogin);
private:
    //点击进入游戏 后  需要经过 一定的逻辑处理   若符合条件    则  执行之
    void realLogin();
    //属于本layer的测试代码。
    void testSomething();

};
#endif /* defined(__GameSanGuo__SGLoginLayer__) */
