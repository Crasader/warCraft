//
//  SGExchangeEquipBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-9.
//
//

#ifndef __GameSanGuo__SGExchangeEquipBox__
#define __GameSanGuo__SGExchangeEquipBox__

#include "SGBaseBox.h"
#include "SGOfficerCard.h"
class SGExchangeEquipBox : public SGBaseBox
{
private:
    SGOfficerCard *officer;
    SGOfficerCard *before;
    SGEquipCard *equip;
    SGBoxDelegate *_dele;
    int enterType;//入口类型
private:
    void confirmHandler();
    void showOfficerInfo();
    void wearEquipListener(CCObject *obj);
    virtual void boxCloseWithOutCallBack();
    void boxCloseWithOutCallBack2();
public:
    SGExchangeEquipBox();
    ~SGExchangeEquipBox();
    static SGExchangeEquipBox *create(SGBoxDelegate *bdg, SGOfficerCard *card,SGEquipCard *card_,int enterType=0);
    bool init(SGBoxDelegate *bdg, SGOfficerCard *card,SGEquipCard *card_);
};


#endif /* defined(__GameSanGuo__SGExchangeEquipBox__) */
