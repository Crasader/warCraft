//
//  SGBossBattleRankListLayer.h
//  GameSanGuo
//
//  Created by bugcode on 14-2-25.
//
//

#ifndef __GameSanGuo__SGBossBattleRankListLayer__
#define __GameSanGuo__SGBossBattleRankListLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "SGBossBattleDetailLayer.h"

class SGBossBattleRankListLayer : public SGBaseTableLayer
{
public:
	SGBossBattleRankListLayer();
	~SGBossBattleRankListLayer();
	
	static SGBossBattleRankListLayer *create(CCArray *data, SGKillBossInfo *killInfo, int enterType = 0);
	void initView();
	void initData(CCArray *data);
	void backHandler();
	void selfRank(int value ,int ranking);//显示自己排名
	void checkEmbattle(CCObject *sender);
	
	SNSTableViewCellItem *tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
	unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
	void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
private:
	int enterType;//预留
	
	SGCCLabelTTF *ranklabel;
    SGCCLabelTTF *shengwang;
    SGCCLabelTTF *rank;
    SGCCLabelTTF *ranknum;
    SGCCLabelTTF *name;
    int selectindex;
    int playerplayerid;
	int ranktypetag;
    int countrytypetag;
	
	std::string beatBossPlayer;//打败boss的玩家名字
	int beatMinute;//所用的分钟数
	int hurtBlood;//伤害血量
	bool isHaveBossKiller;//是否有boss的最终击杀者
	int selfDamage;//自己的击杀值

};

#endif /* defined(__GameSanGuo__SGBossBattleRankListLayer__) */
