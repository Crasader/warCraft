//
//  SGFirstAdvanceBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-18.
//
//

#ifndef __GameSanGuo__SGFirstAdvanceBox__
#define __GameSanGuo__SGFirstAdvanceBox__

#include "SGBaseBox.h"
class SGFirstAdvanceBox : public SGBaseBox
{
    
private:
    void advanceOfficer();
    void advanceEquipment();
    void boxClose();
    
public:
    SGFirstAdvanceBox();
    ~SGFirstAdvanceBox();
    static SGFirstAdvanceBox *create(SGBoxDelegate *bdg);
    bool init(SGBoxDelegate *bdg);
};

#endif /* defined(__GameSanGuo__SGFirstAdvanceBox__) */
