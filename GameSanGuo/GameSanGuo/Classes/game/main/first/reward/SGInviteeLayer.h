//
//  SGInviteeLayer.h
//  GameSanGuo
//
//  Created by zenghui on 13-10-10.
//
//

#ifndef __GameSanGuo__SGInviteeLayer__
#define __GameSanGuo__SGInviteeLayer__

#include "SGBaseLayer.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class SGInviteeLayer : public SGBaseLayer ,public CCEditBoxDelegate
{
private:
    CCEditBox *m_pTextField;
    SGButton *btn;
private:
    void initView();
    void backHandler();
    void confirmHandler();
    void inviteeGoldListener(cocos2d::CCObject *obj);
    
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
public:
    SGInviteeLayer();
    ~SGInviteeLayer();
    static SGInviteeLayer *create();
    
    
};

#endif /* defined(__GameSanGuo__SGInviteeLayer__) */
