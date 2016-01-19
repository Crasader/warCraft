//
//  SGStrengBox.h
//  GameSanGuo
// 强化确定弹框
//  Created by gengshengcai on 13-1-8.
//
//

#ifndef __GameSanGuo__SGStrengBox__
#define __GameSanGuo__SGStrengBox__

#include "SGBaseBox.h"
#include "SGOfficerCard.h"
class SGStrengBox : public SGBaseBox
{
private:
    SGBoxDelegate *delegate;
    SGBaseMilitaryCard *_card;
//    SGOfficerCard *officerCard;
    CCArray *_cardArray;
    CCArray *arrayLabel;
    int coins;
private:
    void confirmHandler();    
    void showGenerInfo();
    void strengOfficerHandler();
    void strengOfficerListener(CCObject *obj);
    void strengEquipListener(CCObject *obj);
    void strengSoldierListener(CCObject *obj);

public:
    SGStrengBox();
    ~SGStrengBox();
    static SGStrengBox *create(SGBoxDelegate *bdg, SGBaseMilitaryCard *card , CCArray *cardArray,int needCoins);
    bool init(SGBoxDelegate *bdg, SGBaseMilitaryCard *card);
};
#endif /* defined(__GameSanGuo__SGStrengBox__) */
