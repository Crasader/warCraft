//
//  SGRegisterBox.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-6.
//
//

//-----------------------------------------------
// 1 注册帐号
// 0 绑定帐号
//-----------------------------------------------


#ifndef __GameSanGuo__SGRegisterBox__
#define __GameSanGuo__SGRegisterBox__

#include "cocos-ext.h"
#include "SGHttpClient.h"
#include "SGBaseBox.h"

#define FLAG_NODISPLAY  0
#define FLAG_OK         1
#define FLAG_ERROR      2

#define ACCOUNT_REGISTER 1
#define ACCOUNT_BIND     0

USING_NS_CC_EXT;

class SGRegisterBox : public SGBaseBox, public CCEditBoxDelegate, public SGHTTPDelegate
{
private:
    CCEditBox* m_pEditName;
    CCEditBox* m_pEditPassword;
    CCEditBox* m_pEditPasswordAgain;
    
    CCLabelTTF *labelAccountError;
    CCLabelTTF *labelPasswordError;
    CCLabelTTF *labelPasswordAgainError;
    
    CCSprite *spriteEditIsError_1;
    CCSprite *spriteEditIsError_2;
    CCSprite *spriteEditIsError_3;
    
    int EnterType;
    SGBoxDelegate *_target;
    
    virtual void constructView();
    bool checkInputStringChar(const char *inputString);
    void processErrorState(int firstFlag , int scendFlag ,int thirdFlag);

    
    //button click
    void registerHandler();
    void buttonClickGoBack();

    //edit box
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
public:
    SGRegisterBox();
    ~SGRegisterBox();
    //http
    virtual void requestFinished(int msgid, CCObject *data);
    virtual void requestFailed();
    
    static SGRegisterBox *create(SGBoxDelegate *dg,int type);
};
#endif /* defined(__GameSanGuo__SGRegisterBox__) */
