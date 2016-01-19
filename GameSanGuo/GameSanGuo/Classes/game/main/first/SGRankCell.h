//
//  SGRankCell.h
//  GameSanGuo
//
//  Created by kanata on 14-2-11.
//
//

#ifndef __GameSanGuo__SGRankCell__
#define __GameSanGuo__SGRankCell__

#include <iostream>
#include "SNSTableViewCellItem.h"

#include "SGRankAllPlayer.h"
class SGRankCell : public SNSTableViewCellItem
{
public:
    SGRankCell();
    virtual ~SGRankCell();
    
    static SGRankCell* create(int playid, int enter = 0);
    void setData(SGRankAllPlayer *player,int type,int rank);
    void sendrequest(CCNode*node);
//    void receivedetail(CCObject*obj);
    
private:
    void initView(int playid);
 
    CCSize bgSize;
    CCSprite *spriteBg;
    CCSprite  *spriteIconBg;
    CCSprite  *rankrewardpng;
    CCSprite *spriteIcon;   // 头像
    SGCCLabelTTF *labelNickName;  // 昵称
    SGCCLabelTTF *labelRank;
    SGCCLabelTTF *ranknum;
    SGCCLabelTTF *labelshanghai;
    int typetype;
    int playid;
    int nation;
    SGButton *detail;
	int enterType;
};
#endif /* defined(__GameSanGuo__SGRankCell__) */
