//
//  SGObtainTiliCell.h
//  GameSanGuo
//
//  Created by zenghui on 14-6-3.
//
//

#ifndef __GameSanGuo__SGObtainTiliCell__
#define __GameSanGuo__SGObtainTiliCell__

#include <iostream>
#include "SNSTableViewCell.h"
#include "SNSTableViewCellItem.h"
#include "SGButton.h"
#include "SGGiftPower.h"
#include "cocos-ext.h"
#include "SGFriendCellBase.h"

USING_NS_CC_EXT;


class SGObtainTiliCell :public SGFriendCellBase
{
private:
    SGButton *itemSendLetter;
    SGGiftPower * m_giftPower;

public:
    SGObtainTiliCell();
    ~SGObtainTiliCell();
    
    static SGObtainTiliCell* create(SGGiftPower *giftPower , SGFriendBaseDelegate *friendDelegate);
    
    void upDate(SGGiftPower *giftPower);
    void buttonClickObtainTili();
    void constructView();
    virtual void updateCellBySGamePlayer(SGGiftPower *giftPower);
};

#endif /* defined(__GameSanGuo__SGObtainTiliCell__) */
