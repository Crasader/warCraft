//
//  SGCreamMainCell.h
//  GameSanGuo
//
//  Created by bugcode on 13-10-28.
//
//

#ifndef __GameSanGuo__SGCreamMainCell__
#define __GameSanGuo__SGCreamMainCell__

#include <iostream>

#include "SGStoryBaseCell.h"
#include "SGLayout.h"
#include "SGStoryBean.h"
#include "SGCCLabelTTF.h"
#include "SGCreamBattleLayer.h"

class SGCreamMainCell : public SGStoryBaseCell
{
private:
    CCSprite *spriteIsPass;
    CCSprite *spriteRank;
    
    CCLabelTTF *labelName;
    SGCCLabelTTF *labelDisc_1;
    SGCCLabelTTF *labelDisc_2;
	void select();
	
    int _conTime;
    ///
    SGCreamMainCellDelegate * delegate;
    SNSTableView* tableview;
    
private:
    void viewDidLoad();
    void resetTime();
    
public:
    SGCreamMainCell();
    ~SGCreamMainCell();
    //static SGCreamMainCell *create();
    static SGCreamMainCell *   create(SGCreamMainCellDelegate*del,SNSTableView*tab);
    void setData(int index, SGStoryBean* bean, bool isShowNew = true);
};



#endif /* defined(__GameSanGuo__SGCreamMainCell__) */
