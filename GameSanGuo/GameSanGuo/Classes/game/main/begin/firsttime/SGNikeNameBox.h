//
//  SGNikeNameBox.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-10.
//
//

#ifndef __GameSanGuo__SGNikeNameBox__
#define __GameSanGuo__SGNikeNameBox__

#include "SGBaseBox.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;

class SGNikeNameBox : public SGBaseBox,  public CCEditBoxDelegate
{
private:
    int countryId;
    CCEditBox* m_pEditName;
  //  CCLabelTTF *label_Error;
    CCEditBox* m_pEditNum;
private:
    void initView();
    void confirmHandler();
    void buttonClickNickName();
    //修改国家
    void changeCountryHandler(CCObject *obj);
public:
    static SGNikeNameBox *create(SGBoxDelegate *dg, int countryId_);
    static std::string Trim(const std::string &str);
    static int utf8StrLength(const char* utf8);
public:
    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(CCEditBox* editBox);
    SGNikeNameBox();
    ~SGNikeNameBox();
};
#endif /* defined(__GameSanGuo__SGNikeNameBox__) */
