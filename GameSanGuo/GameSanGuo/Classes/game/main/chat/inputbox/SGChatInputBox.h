//
//  SGChatInputBox.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-2.
//
//

#ifndef __GameSanGuo__SGChatInputBox__
#define __GameSanGuo__SGChatInputBox__

#include <iostream>
#include "SGBaseBox.h"
#include "SGCCTextFieldTTF.h"
//#include "SGBoxDelegate.h"

class SGChatInputBox : public SGBaseBox
{
private:
//    SGBoxDelegate *_delegate;
    SGCCTextFieldTTF *m_pTextField;
//    CCMenu *menu;
//    SGButton *buttonSend;
//    SGButton *buttonCancel;
    
//    CCEditBox *m_pTextField;
    void initview();
    
    //按钮点击
    void sendMsgConfirm();
    void sendMsgCancel();
    void buttonClickSend();
    void buttonClickCancel();
    void textFiledDidhide(CCObject *obj);
    
public:
    SGChatInputBox();
    ~SGChatInputBox();
    static SGChatInputBox *create(SGBoxDelegate *delegate);
    
};

#endif /* defined(__GameSanGuo__SGChatInputBox__) */
