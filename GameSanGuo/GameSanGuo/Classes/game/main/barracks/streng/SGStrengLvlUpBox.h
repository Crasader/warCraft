//
//  SGStrengLvlUpBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-20.
//
//

#ifndef __GameSanGuo__SGStrengLvlUpBox__
#define __GameSanGuo__SGStrengLvlUpBox__

#include "SGBaseBox.h"
#include "SGOfficerCard.h"
#include "SGEquipCard.h"
class SGStrengLvlUpBox : public SGBaseBox
{
private:
    void confirmHandler();
    int cardType;
    int isfromfirst;
    SGBoxDelegate *_delegate;
 
    SGBaseMilitaryCard *new_card;
    SGBaseMilitaryCard *old_card;
public:
    SGStrengLvlUpBox();
    ~SGStrengLvlUpBox();
    static SGStrengLvlUpBox *create(SGBoxDelegate *bdg, SGBaseMilitaryCard *aftrecard,SGBaseMilitaryCard *beforecard,int type,int from = 0);
    bool init(SGBoxDelegate *bdg, SGBaseMilitaryCard *card,SGBaseMilitaryCard *beforecard,int type);
};


#endif /* defined(__GameSanGuo__SGStrengLvlUpBox__) */
