//
//  SGObtainTili.h
//  GameSanGuo
//
//  Created by zenghui on 14-6-3.
//
//

#ifndef __GameSanGuo__SGObtainTili__
#define __GameSanGuo__SGObtainTili__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "SGFriendBase.h"


class SGObtainTili :public SGFriendBase{
    
private:
    
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    
private:
    SGButton *buttonSend;
    CCArray *_arrayFriend;
  
    void constructView();

    void quickEatTili();
    void eatGiftPowerListener(CCObject *obj);
    void eatAllGiftPowerListener(CCObject *obj);
    int m_obtainCount;
    SGGiftPower *eatedGiftPower;
    SGCCLabelTTF *obtainCountMsg;
public:
    SGObtainTili();
    ~SGObtainTili();
    static SGObtainTili* create(CCArray *arrayFriend);

    virtual void giftCellCallBack(SGGiftPower *player , bool type);
    virtual void updateData();
};



#endif /* defined(__GameSanGuo__SGObtainTili__) */
