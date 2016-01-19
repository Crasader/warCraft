//
//  SGFirstStrengBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-18.
//
//

#ifndef __GameSanGuo__SGFirstStrengBox__
#define __GameSanGuo__SGFirstStrengBox__

#include "SGBaseBox.h"
class SGFirstStrengBox : public SGBaseBox
{

private:
    void strengOfficer();
    void strengEquipment();
    void strengSoldier();
    void strengSkill();
    void strengLord();
    void boxClose();
    
public:
    SGFirstStrengBox();
    ~SGFirstStrengBox();
    static SGFirstStrengBox *create(SGBoxDelegate *bdg);
    bool init(SGBoxDelegate *bdg);
};

#endif /* defined(__GameSanGuo__SGFirstStrengBox__) */
