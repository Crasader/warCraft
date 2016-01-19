//
//  SGChangePassWordLayer.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-7-4.
//
//

#ifndef __GameSanGuo__SGChangePassWordLayer__
#define __GameSanGuo__SGChangePassWordLayer__

#define FLAG_NODISPLAY 0
#define FLAG_OK 1
#define FLAG_ERROR 2

#include <iostream>
#include "SGBaseLayer.h"
#include "cocos-ext.h"
#include "SGHTTPDelegate.h"
#include "SGHttpClient.h"

using namespace cocos2d::extension;
using namespace cocos2d;

class SGChangePassWordLayer :public SGBaseLayer,public SGHTTPDelegate //,public CCEditBoxDelegate
{
    
public:
    SGChangePassWordLayer();
    ~SGChangePassWordLayer();
    
    static SGChangePassWordLayer* create();
    
    virtual void requestFinished(int msgid, cocos2d::CCObject *data);
    virtual void requestFailed(){};
    
    
private:
    CCEditBox *m_userName;
    CCEditBox *m_pEditName;
    CCEditBox *m_pEditPassword;
    CCEditBox *m_pEditPasswordAgain;
    
//    CCLabelTTF *labelOldPassword = NULL;
//    CCLabelTTF *labelNewPassword = NULL;
//    CCLabelTTF *labelNewPasswordAgain = NULL;
//    CCLabelTTF *labelOldPasswordError = NULL;
//    CCLabelTTF *labelTip = NULL;
//    CCLabelTTF *labelNewPasswordAgianError = NULL;
//    
//    CCSprite *spriteFlag_1;
//    CCSprite *spriteFlag_2;
//    CCSprite *spriteFlag_3;
    
    
    void constructView();
    void buttonClickGoBack();
    void buttonClickOK();
//    void processFlag(int first , int secend , int third);
    
    
    
    //CCEditBoxDelegate
    //    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    //    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    //    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    //    virtual void editBoxReturn(CCEditBox* editBox);
    
};

#endif /* defined(__GameSanGuo__SGChangePassWordLayer__) */
