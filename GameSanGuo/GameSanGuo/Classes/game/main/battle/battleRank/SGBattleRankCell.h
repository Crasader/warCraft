//
//  SGBattleRankCell.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-16.
//
//

#ifndef __GameSanGuo__SGBattleRankCell__
#define __GameSanGuo__SGBattleRankCell__

#include "SNSTableViewCellItem.h"
#include "SGRankPlayer.h"

class SGBattleRankCell : public SNSTableViewCellItem
{
public:
    SGBattleRankCell(void);
    virtual ~SGBattleRankCell(void);
    
    static SGBattleRankCell* create();
    void resetBgImg(int index);
    void setData(SGRankPlayer* player);
    
private:
    void initView();
    void initBackground();
    
private:
    CCSize bgSize;
    CCSprite *spriteBg;
    
    CCSprite *spriteIcon;   // 头像
    CCSprite *spriteIconBg;
    SGCCLabelTTF *labelNickName;  // 昵称
    SGCCLabelTTF *labelRank;      // 排名
    SGCCLabelTTF *labelFTime;     // 参战次数
    SGCCLabelTTF *labelLevel;     // 天梯等级
    SGCCLabelTTF *labelWinR;      // 胜率
};

#endif /* defined(__GameSanGuo__SGBattleRankCell__) */
