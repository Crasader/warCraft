//
//  MYtextfieldttf.h
//  GameSanGuo
//
//  Created by kanata on 14-1-10.
//
//

#ifndef __GameSanGuo__MYtextfieldttf__
#define __GameSanGuo__MYtextfieldttf__


#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#define  TEXTFIELDHIDE "TextHideEvent"
USING_NS_CC;
USING_NS_CC_EXT;

class MYtextfieldttf : public CCTextFieldTTF,public CCTouchDelegate
{
private:
    void viewDidLoad();
    
    int characterCount(char *str);
    
protected:
    CCPoint m_positionPos;
    //设置文字长度
    CC_SYNTHESIZE(int, m_nLength,Length);
    bool m_needShow;
    CC_SYNTHESIZE(int, keyType, KType);
    CC_SYNTHESIZE(int, _isclose, close);////及时关闭键盘
    
public:
  
public:
    
    //init
    MYtextfieldttf();
    ~MYtextfieldttf();
    static MYtextfieldttf * createWithPlaceHolder(CCPoint position,const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    
    //cctouchdelegate
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    //ccnode
    virtual void onEnter();
    virtual void onExit();
    
    //IMG
    virtual void insertText(const char * text, int len);
    
    virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info);
    //virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info);
    virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info);
    
    virtual void setString(const char *text){CCTextFieldTTF::setString(text);}
    virtual const char* getString(void){return CCTextFieldTTF::getString();}
    
    
    virtual bool canAttachWithIME(){return CCTextFieldTTF::canAttachWithIME();}
    virtual bool canDetachWithIME(){return CCTextFieldTTF::canDetachWithIME();}
    virtual void deleteBackward(){CCTextFieldTTF::deleteBackward();}
    virtual const char * getContentText(){return CCTextFieldTTF::getContentText();}
    virtual bool attachWithIME(){return CCTextFieldTTF::attachWithIME();}
    virtual bool detachWithIME(){return CCTextFieldTTF::detachWithIME();}
};



#endif /* defined(__GameSanGuo__MYtextfieldttf__) */
