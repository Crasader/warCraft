//
//  SGNoExpStrengBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-20.
//
//

#ifndef __GameSanGuo__SGNoExpStrengBox__
#define __GameSanGuo__SGNoExpStrengBox__

#include "SGBaseBox.h"
#include "SGOfficerCard.h"
class SGNoExpStrengBox : public SGBaseBox
{
private:
    void confirmHandler();
    SGOfficerCard *_card;
public:
	~SGNoExpStrengBox();
    static SGNoExpStrengBox *create(SGBoxDelegate *bdg, SGOfficerCard *card);
    bool init(SGBoxDelegate *bdg, SGOfficerCard *card);
};
#endif /* defined(__GameSanGuo__SGNoExpStrengBox__) */
