//
//  SGAlertBox.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-2-4.
//
//

#include "SGAlertBox.h"
SGAlertBox *SGAlertBox::create(SGBoxDelegate *delegate, const char *msg)
{
    SGAlertBox *alertBox = new SGAlertBox();
    if (alertBox->init(delegate, msg))
    {
        alertBox->autorelease();
        return alertBox;
    }
    CC_SAFE_DELETE(alertBox);
    return NULL;
}

bool SGAlertBox::init(SGBoxDelegate *delegate, const char *msg)
{
    if (!SGBaseBox::init(delegate, box_alertTag, CCSizeMake(460, 400),true))
    {
        return false;
    }
    CCLabelTTF *msgLable = CCLabelTTF::create(msg,
                                              "Courier New",
                                              FONTSIZE(14),
                                              CCSizeMake(360, 140),
                                              kCCTextAlignmentCenter);
    this->addChild(msgLable,10);
    msgLable->setPosition(ccp(screenwidth * .5, screenheight *.5));
    return true;
}