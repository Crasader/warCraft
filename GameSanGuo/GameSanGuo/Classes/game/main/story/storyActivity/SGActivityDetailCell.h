//
//  SGActivityDetailCell.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-3.
//
//

#ifndef __GameSanGuo__SGActivityDetailCell__
#define __GameSanGuo__SGActivityDetailCell__

#include "SGStoryBaseCell.h"

class SGActivityDetailCell : public SGStoryBaseCell
{
private:
    CCSprite *spriteIsPass;
    CCSprite *spriteActName;
    
    CCLabelTTF *labelDesc;
    
    bool _isPassLevel;
    
    void viewDidLoad();
    
public:
    SGActivityDetailCell();
    ~SGActivityDetailCell();
    static SGActivityDetailCell* create();
    
    void setData(int index);
};

#endif /* defined(__GameSanGuo__SGActivityDetailCell__) */
