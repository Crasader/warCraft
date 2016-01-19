//
//  SGPiecesLayer.h
//  GameSanGuo
//
//  Created by bugcode on 13-11-27.
//
//

#ifndef __GameSanGuo__SGPiecesLayer__
#define __GameSanGuo__SGPiecesLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SGCCLabelTTF.h"
#include "SGPiecesCell.h"

class SGPiecesLayer : public SGBaseTableLayer, public SGPiecesDelegate, public SNSScrollViewPageDelegate
{
public:
	SGPiecesLayer();
	~SGPiecesLayer();
	void backHandler();//返回
	static SGPiecesLayer *create();
	virtual void initView();
//	void showPiecesLayer(bool isBeforeDel = true);//显示本界面回调
	void useCommonPieces();//使用通用碎片回调
	
	SNSTableViewCellItem *tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
	unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
	void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
	
public:
	bool isUseCommon;//是否使用通用
	int piecesCommonTotalNum;//通用碎片的数量
	virtual void summonPieces(SGPiecesCell *piecesItem);
    virtual void showPiecesinfo(SGPiecesCell *piecesItem);
	virtual void onEnter();
	void summonPiecesListener(CCObject *sender);//召唤监听
	void refreshPage(int type);
    void showpieceinfo(SGPiecesCell*cell);
    
    void resortPiecesCard();
    
private:
	int selectIndex;//选择的item
	SGPiecesCard *card;//召唤的武将卡
	SGCCLabelTTF *commonCount;//能用碎片数量
	SGCCLabelTTF *pageLabel;
	
	CCArray *_allData;
	
};












#endif /* defined(__GameSanGuo__SGPiecesLayer__) */
