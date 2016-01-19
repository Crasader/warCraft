//
//  SGServerListLoginGame.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-1-28.
//
//

#ifndef __GameSanGuo__SGServerListLoginGame__
#define __GameSanGuo__SGServerListLoginGame__

#include <iostream>
#include "SGBaseLayer.h"
#include "SGBaseBox.h"
#include "SGServerListLayer.h"
#include "cocos-ext.h"
#include "SGHTTPDelegate.h"

using namespace cocos2d::extension;
using namespace cocos2d;

class SGServerListLoginGame :public SGBaseLayer ,public CCEditBoxDelegate,public SGHTTPDelegate {
    
private:
    CCEditBox *editBoxAccount;
    CCEditBox *editBoxPassword;
    bool isAccount;
private:
    void buttonClickLoginGame();
    void buttonClickGoBack();
    void constructView();
//    void processFlag(int first , int secend);
    void registerHandler();
    void passwordHandler();
    void bindHandler();
    void cleanEditText(CCEditBox *editBox);
    void showBoxCall(cocos2d::CCObject *obj);
    void setIsCanTouch(bool isTouch);
protected:
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//
//    //ccnode
//    virtual void onEnter();
//    virtual void onExit();


public:
    SGServerListLoginGame();
    ~SGServerListLoginGame();
    void setIsNewAccount(bool newAccount);
    virtual void requestFinished(int msgid, cocos2d::CCObject *data);
    virtual void requestFailed();
    static SGServerListLoginGame* create();
};

#endif /* defined(__GameSanGuo__SGServerListLoginGame__) */
