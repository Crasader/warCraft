//
//  SGMailInput.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-15.
//
//

#ifndef __GameSanGuo__SGMailInputBox__
#define __GameSanGuo__SGMailInputBox__

#include <iostream>
#include "cocos2d.h"
#include "SGCCTextFieldTTF.h"
#include "SGBoxDelegate.h"
#include "SGButton.h"

class SGMailInputBox : public CCLayer
{
private:
    SGBoxDelegate *_delegate;
    SGCCTextFieldTTF *m_pTextField;
    CCMenu *menu;
    SGButton *buttonSend;
    SGButton *buttonCancel;
    
    void viewDidLoad();
    
    //按钮点击
    void buttonClickSend();
    void buttonClickCancel();
    
public:
    SGMailInputBox();
    ~SGMailInputBox();
    static SGMailInputBox *create(SGBoxDelegate *delegate);
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
};


#endif /* defined(__GameSanGuo__SGMailInput__) */
