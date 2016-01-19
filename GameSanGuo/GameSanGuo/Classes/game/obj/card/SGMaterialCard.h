//
//  SGMaterialCard.h
//  GameSanGuo
//
//  Created by bugcode on 14-10-22.
//
//

#ifndef __GameSanGuo__SGMaterialCard__
#define __GameSanGuo__SGMaterialCard__

#include <iostream>
#include "cocos2d.h"
#include "SGBaseMilitaryCard.h"


USING_NS_CC;


class SGMaterialCard : public SGBaseMilitaryCard
{
public:

    SGMaterialCard();
	~SGMaterialCard();
	virtual const char * getClassName(){return "SGMaterialCard";};
    
    //这里与Material的dataModel相同的字段
    CC_SYNTHESIZE(int, _materialBorderColor, MaterialBorderColor );//材料icon的边框颜色
    CC_SYNTHESIZE(int, _materialPileLimit, MaterialPileLimit);//材料的堆叠上限
    //卡里面要放这个材料现在的个数
    CC_SYNTHESIZE(int, _materialCurrNum, MaterialCurrNum);
    
};

#endif /* defined(__GameSanGuo__SGMaterialCard__) */
