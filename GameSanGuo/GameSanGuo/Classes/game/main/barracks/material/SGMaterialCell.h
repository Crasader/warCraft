//
//  SGMaterialCell.h
//  GameSanGuo
//
//  Created by bugcode on 14-10-22.
//
//

#ifndef __GameSanGuo__SGMaterialCell__
#define __GameSanGuo__SGMaterialCell__

#include <iostream>
#include "SGStoryBaseCell.h"
#include "SGMaterialCard.h"

class SGMaterialCell : public SGStoryBaseCell
{
public:
    SGMaterialCell();
    ~SGMaterialCell();
    static SGMaterialCell *create(SGMaterialCard *card);
    
    void initView();
    void updateCell(SGMaterialCard *card);
    
private:
    SGMaterialCard *materialCard;
    CCSprite *materialIcon;//材料的icon
    CCSprite *materialBorder;//材料的边框
    SGCCLabelTTF *materialName;//材料的名称
    SGCCLabelTTF *materialNum;//材料的数量
};


#endif /* defined(__GameSanGuo__SGMaterialCell__) */
