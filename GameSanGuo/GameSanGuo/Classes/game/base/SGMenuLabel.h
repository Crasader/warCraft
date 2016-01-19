//
//  SGMenuLabel.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-5-8.
//
//

#ifndef __GameSanGuo__SGMenuLabel__
#define __GameSanGuo__SGMenuLabel__
#include "cocos2d.h"
#include "SGCCLabelTTF.h"
USING_NS_CC;
class SGMenuLabel : public CCMenuItemLabel
{
public:
    static SGMenuLabel *create(CCNode*label, CCObject* target, SEL_MenuHandler selector);

    bool init(CCNode*label, CCObject* target, SEL_MenuHandler selector);
    void setLabel(CCNode* var);
};

#endif /* defined(__GameSanGuo__SGMenuLabel__) */
