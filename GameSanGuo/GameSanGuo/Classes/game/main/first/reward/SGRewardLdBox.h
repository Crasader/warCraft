//
//  SGRewardLdBox.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-6-6.
//
//

#ifndef __GameSanGuo__SGRewardLdBox__
#define __GameSanGuo__SGRewardLdBox__
#include "SGBaseBox.h"
#include "SGBaseMilitaryCard.h"
class SGRewardLdBox : public SGBaseBox
{
private:
    virtual void boxCloseWithOutCallBack();
    int entertype;
public:
    SGRewardLdBox();
    static SGRewardLdBox *create(SGBoxDelegate *bdg,SGBaseMilitaryCard *card,int type);
    bool init(SGBoxDelegate *bdg,SGBaseMilitaryCard *card);
    ~SGRewardLdBox();
};


#endif /* defined(__GameSanGuo__SGRewardLdBox__) */
