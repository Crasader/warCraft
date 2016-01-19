//
//  SGHeadIconItem.h
//  GameSanGuo
//
//  Created by 赤霄 on 13-12-30.
//
//

#ifndef __GameSanGuo__SGHeadIconItem__
#define __GameSanGuo__SGHeadIconItem__

#include <iostream>
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SGMailManager.h"

class  SGShowHeadIconLayer;
USING_NS_CC;
class SGHeadIconItem: public SNSTableViewCellItem{


private:
    SGCCLabelTTF *label_name;
    SGCCLabelTTF *label_count;
    CCSprite *main_sprite;
    CCSprite *kuang_sprite;
    CCSprite *country_sprite;
    
   public:
    SGHeadIconItem();
   ~SGHeadIconItem();
    
    static SGHeadIconItem *create();
    void initView(HeadInfo headInfo);

};





#endif /* defined(__GameSanGuo__SGHeadIconItem__) */
