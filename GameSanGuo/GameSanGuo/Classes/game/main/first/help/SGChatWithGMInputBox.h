//
//  SGChatWithGMInputBox.h
//  GameSanGuo
//
//  Created by kanata on 14-1-9.
//
//

#ifndef __GameSanGuo__SGChatWithGMInputBox__
#define __GameSanGuo__SGChatWithGMInputBox__

#include <iostream>
#include "SGBaseBox.h"
#include "SGCCTextFieldTTF.h"
//#include "SGBoxDelegate.h"

class SGChatWithGMInputBox : public SGBaseBox
{
private:

    SGCCTextFieldTTF *m_pTextField;
    void initview();
    void buttonClickSend();
    void buttonClickCancel();
    void sendconfirm(CCObject*obj);
    void callback();
    void checkqaresponse(CCObject*obj);
    long getCurrentTime();
private:
    int checkid;
    CCArray*array;
    
    
public:
    SGChatWithGMInputBox();
    ~SGChatWithGMInputBox();
    static SGChatWithGMInputBox *create(SGBoxDelegate *delegate,int idid,CCArray*arr);
    
};
#endif /* defined(__GameSanGuo__SGChatWithGMInputBox__) */
