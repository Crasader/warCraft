//
//  SGShowJingZhouRankCell.h
//  GameSanGuo
//
//  Created by kanata on 13-11-14.
//
//

#ifndef __GameSanGuo__SGShowJingZhouRankCell__
#define __GameSanGuo__SGShowJingZhouRankCell__

#include <iostream>
#include "SNSTableViewCellItem.h"

#include "SGRankPlayer.h"
#include "SGShowJingZhouRankPlayerLayer.h"
class SGShowJingZhouRankCell : public SNSTableViewCellItem
{
public:
    SGShowJingZhouRankCell();
    virtual ~SGShowJingZhouRankCell();
    
    static SGShowJingZhouRankCell* create();
    //void resetBgImg(int index);
    void setData(SGShowJingZhouRankPlayerLayer *player);
    
private:
    void initView();
    void initBackground();
    CCSize bgSize;
    CCSprite *spriteBg;
    CCSprite  *spriteIconBg;
    CCSprite *spriteIcon;   // 头像
    SGCCLabelTTF *labelNickName;  // 昵称
    SGCCLabelTTF *labelRank;      // 排名
    SGCCLabelTTF *labelboshu;     // 最高波数
    SGCCLabelTTF *labelshanghai;     // 造成伤害
	
	
	// cell上的元宝显示
	CCSprite *goldSpr;
	CCSprite *goldBorderSpr;
	CCSprite *goldBg;
	//元宝数量
	SGCCLabelTTF *goldCount;
	//数量背景
	CCSprite *goldCountBg;
   
};


#endif /* defined(__GameSanGuo__SGShowJingZhouRankCell__) */
