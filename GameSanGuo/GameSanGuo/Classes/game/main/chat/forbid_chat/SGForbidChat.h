//
//  SGForbidChat.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-4.
//
//

#ifndef __GameSanGuo__SGForbidChat__
#define __GameSanGuo__SGForbidChat__

#include <iostream>
#include "SGBaseBox.h"

class SGForbidChat : public SGBaseBox {
private:
    void constructView();
    
    void buttonClickOK();
    void buttonClickCancel();
    
    bool isCanPay();
    //void buySpeakerListener(CCObject *object);
    void enterstorelayer(CCObject *obj);
public:
    
    SGForbidChat();
    ~SGForbidChat();
    static SGForbidChat* create(SGBoxDelegate *delegate);

};

#endif /* defined(__GameSanGuo__SGForbidChat__) */
