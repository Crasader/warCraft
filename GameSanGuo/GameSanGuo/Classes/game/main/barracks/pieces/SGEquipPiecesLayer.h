//
//  SGEquipPiecesLayer.h
//  GameSanGuo
//
//  Created by bugcode on 13-11-27.
//
//

#ifndef __GameSanGuo__SGEquipPiecesLayer__
#define __GameSanGuo__SGEquipPiecesLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SGCCLabelTTF.h"
#include "SGPiecesCell.h"

class SGEquipPiecesLayer : public SGBaseTableLayer, public SGPiecesDelegate, public SNSScrollViewPageDelegate
{
public:
	SGEquipPiecesLayer();
	~SGEquipPiecesLayer();
	void backHandler();//返回
	static SGEquipPiecesLayer *create();
	virtual void initView();
	void showEquipPiecesLayer(bool isBeforeDel = true);
	void useCommonPieces();//选择通用button
	
	SNSTableViewCellItem *tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
	unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
	void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
	void refreshPage(int type);
public:
	bool isUseCommon;//是否使用能用碎片
	int piecesTotalCommon;//通用碎片数量
	void summonPieces(SGPiecesCell *piecesItem);
	void summonPiecesListener(CCObject *sender);//召唤监听
    void showPiecesinfo(SGPiecesCell *piecesItem);
    void resortPiecesCard();
private:
	int selectIndex;//选择的item
	SGPiecesCard *card;//召唤的武将卡
	CCLabelTTF *commonCount;//能用碎片数量
	
	SGCCLabelTTF *pageLabel;
	CCArray *_allData;
};


#endif /* defined(__GameSanGuo__SGEquipPiecesLayer__) */
