//
//  SGRankDetailCell.h
//  GameSanGuo
//
//  Created by kanata on 14-2-12.
//
//

#ifndef __GameSanGuo__SGRankDetailCell__
#define __GameSanGuo__SGRankDetailCell__

#include <iostream>
#include "SNSTableView.h"
#include "SGRankDetailLayer.h"
class SGRankDetailCell: public SNSTableViewCellItem
{
public:
    SGRankDetailCell();
    ~SGRankDetailCell();
    
    static SGRankDetailCell* create();
    void  initView();
    void  setData(DPI*detail);
    void enter();
private:

    CCArray*datas;
    CCSprite*  item;
    CCSprite*  kuang;
    SGCCLabelTTF*  name;
    SGCCLabelTTF*  level;
    SGButton* btn;
};


#endif /* defined(__GameSanGuo__SGRankDetailCell__) */
