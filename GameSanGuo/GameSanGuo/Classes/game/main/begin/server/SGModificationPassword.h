//
//  SGModificationPassword.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-1-30.
//
//

#ifndef __GameSanGuo__SGModificationPassword__
#define __GameSanGuo__SGModificationPassword__

#define FLAG_NODISPLAY 0
#define FLAG_OK 1
#define FLAG_ERROR 2

#include <iostream>
#include "SGBaseBox.h"
#include "cocos-ext.h"
#include "SGHTTPDelegate.h"
#include "SGHttpClient.h"

using namespace cocos2d::extension;
using namespace cocos2d;

class SGModificationPassword :public SGBaseBox,public SGHTTPDelegate //,public CCEditBoxDelegate
{
    
public:
    SGModificationPassword();
    ~SGModificationPassword();
    
    static SGModificationPassword* create(SGBoxDelegate *delegate);
    
    virtual void requestFinished(int msgid, cocos2d::CCObject *data);
    virtual void requestFailed(){};

    
private:
    CCEditBox *m_userName;
    CCEditBox *m_pEditName;
    CCEditBox *m_pEditPassword;
    CCEditBox *m_pEditPasswordAgain;
    
    CCLabelTTF *labelOldPassword;
    CCLabelTTF *labelNewPassword;
    CCLabelTTF *labelNewPasswordAgain;
    CCLabelTTF *labelOldPasswordError;
    CCLabelTTF *labelTip;
    CCLabelTTF *labelNewPasswordAgianError;

    CCSprite *spriteFlag_1;
    CCSprite *spriteFlag_2;
    CCSprite *spriteFlag_3;
    
    
    void constructView();
    void buttonClickGoBack();
    void buttonClickOK();
    void processFlag(int first , int secend , int third);

    
    
    //CCEditBoxDelegate
//    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
//    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
//    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
//    virtual void editBoxReturn(CCEditBox* editBox);
    
};

#endif /* defined(__GameSanGuo__SGModificationPassword__) */
