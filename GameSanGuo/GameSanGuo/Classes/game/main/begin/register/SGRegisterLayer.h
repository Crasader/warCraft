//
//  SGRegisterLayer.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-7-3.
//
//

#ifndef __GameSanGuo__SGRegisterLayer__
#define __GameSanGuo__SGRegisterLayer__

#include "cocos-ext.h"
#include "SGHttpClient.h"
#include "SGBaseLayer.h"


#define FLAG_NODISPLAY  0
#define FLAG_OK         1
#define FLAG_ERROR      2

#define ACCOUNT_REGISTER 1
#define ACCOUNT_BIND     0

USING_NS_CC_EXT;

class SGRegisterLayer : public SGBaseLayer, public CCEditBoxDelegate, public SGHTTPDelegate
{
private:
    CCEditBox* m_pEditName;
    CCEditBox* m_pEditPassword;
    CCEditBox* m_pEditPasswordAgain;
    
    int EnterType;
    SGBoxDelegate *_target;
    
    virtual void constructView();
    bool checkInputStringChar(const char *inputString);
//    void processErrorState(int firstFlag , int scendFlag ,int thirdFlag);
    
    bool is_email_valid(const char* email);
    //button click
    void registerHandler();
    void buttonClickGoBack();
    
    //跳转到登陆页面
    void showLoginLayer(CCObject *obj);

    //edit box
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);

    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
public:

    SGRegisterLayer();
    ~SGRegisterLayer();
    static SGRegisterLayer *create(int type);
    //http
    virtual void requestFinished(int msgid, CCObject *data);
    virtual void requestFailed();
};


#endif /* defined(__GameSanGuo__SGRegisterLayer__) */
