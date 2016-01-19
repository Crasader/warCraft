//
//  SGChangeNickName.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-15.
//
//

#ifndef __GameSanGuo__SGChangeNickName__
#define __GameSanGuo__SGChangeNickName__

#include "SGBaseBox.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;

class SGChangeNickName : public SGBaseBox,  public CCEditBoxDelegate
{
private:
    int countryId;
    CCLabelTTF *label_;
    CCEditBox* m_pEditName;
    CCLabelTTF *label_Error;
private:
    void initView();
    void confirmHandler();
    void buttonClickNickName();
    void showStoreLayer();
public:
    SGChangeNickName();
    ~SGChangeNickName();
    static SGChangeNickName *create(SGBoxDelegate *dg, int countryId_);
    static int characterCount(char *str);
    
public:
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
};


#endif /* defined(__GameSanGuo__SGChangeNickName__) */
