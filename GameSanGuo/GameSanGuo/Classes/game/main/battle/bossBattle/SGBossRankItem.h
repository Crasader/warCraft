//
//  SGBossRankItem.h
//  GameSanGuo
//
//  Created by bugcode on 14-2-27.
//
//

#ifndef __GameSanGuo__SGBossRankItem__
#define __GameSanGuo__SGBossRankItem__

#include "SNSTableViewCellItem.h"
#include "SGBossBattleDetailLayer.h"

class SGBossRankItem : public SNSTableViewCellItem
{
public:
	SGBossRankItem();
	~SGBossRankItem();
	
	static SGBossRankItem *create(SGBossRankListData *data, int enter = 0);
	void initView();
	void checkDetailEmbattle();
	void initData(SGBossRankListData *data, int order = 4, bool isHavaKillBoss = true);
private:
	int enter;
	CCSize bgSize;
    CCSprite *spriteBg;
    CCSprite  *spriteIconBg;
    CCSprite  *rankrewardpng;
    CCSprite *spriteIcon;   // 头像
    SGCCLabelTTF *labelNickName;  // 昵称
    SGCCLabelTTF *labelRank;
    SGCCLabelTTF *ranknum;
    SGCCLabelTTF *labelshanghai;
	SGButton *detail;
	
	SGBossRankListData *listDatas;
	
	int roleId;
};


#endif /* defined(__GameSanGuo__SGBossRankItem__) */
