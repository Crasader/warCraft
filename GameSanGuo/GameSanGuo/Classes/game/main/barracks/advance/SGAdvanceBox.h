//
//  SGAdvanceBox.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#ifndef __GameSanGuo__SGAdvanceBox__
#define __GameSanGuo__SGAdvanceBox__

#include "SGBaseBox.h"
#include "SGOfficerCard.h"
class SGAdvanceBox : public SGBaseBox
{
private:
    SGBaseMilitaryCard *_card;
    CCArray *arrayLabel;
    CCArray *reqCards;
    int cardType;
    SGOfficerCard *officer;
    SGEquipCard *equip;
private:
    void confirmHandler();
    void showGeneralInfo();
    void advanceOfficerListener(CCObject *obj);
    void advanceEquipListener(CCObject *obj);
public:
    SGAdvanceBox();
    ~SGAdvanceBox();
    static SGAdvanceBox *create(SGBoxDelegate *bdg, SGBaseMilitaryCard *card,int type);
    bool init(SGBoxDelegate *bdg, SGBaseMilitaryCard *card);
};
#endif /* defined(__GameSanGuo__SGAdvanceBox__) */

