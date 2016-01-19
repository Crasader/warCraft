//
//  SGExchangeCodeLayer.h
//  GameSanGuo
//
//  Created by 赤霄 on 14-1-6.
//
//

#ifndef __GameSanGuo__SGExchangeCodeLayer__
#define __GameSanGuo__SGExchangeCodeLayer__

#include <iostream>
#include "SGBaseLayer.h"
#include "SGMainManager.h"
#include "RewardModule.pb.h"

class SGExchangeCodeLayer:public  SGBaseLayer,CCEditBoxDelegate {
    
    SGExchangeCodeLayer();
    ~SGExchangeCodeLayer();
    CCEditBox *editNikeName;
    CCString *_desprition;
    CCScale9Sprite *spriteBackground1;
public:
    static SGExchangeCodeLayer *create(CCString *desprition);
    void initView();
    void backHandler();
    void exchangeCode();
    void exchangeListener(CCObject *obj);
    
    virtual void showBoxCall(CCObject *obj);
   
    
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    
    
   
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    
    
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
   
    virtual void editBoxReturn(CCEditBox* editBox);
};

#endif /* defined(__GameSanGuo__SGExchangeCodeLayer__) */
