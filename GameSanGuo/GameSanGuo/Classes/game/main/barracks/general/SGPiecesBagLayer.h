

#ifndef __GameSanGuo__SGPiecesBagLayer__
#define __GameSanGuo__SGPiecesBagLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SGCCLabelTTF.h"
#include "SGPiecesBagCell.h"

class SGPiecesBagLayer : public SGBaseTableLayer, public SGPiecesBagDelegate, public SNSScrollViewPageDelegate
{
public:
	SGPiecesBagLayer();
	~SGPiecesBagLayer();
	void backHandler();//返回
	static SGPiecesBagLayer *create();
	virtual void initView();
	void useCommonPieces();//使用通用碎片回调
	
	SNSTableViewCellItem *tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
	unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
	void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
public:
	bool isUseCommon;//是否使用通用
	int piecesCommonTotalNum;//通用碎片的数量
	virtual void summonPieces(SGPiecesBagCell *piecesItem);
    virtual void pieceClickCallback(SGPiecesBagCell *piecesItem);
	virtual void onEnter();
	void summonPiecesListener(CCObject *sender);//召唤监听
	void refreshPage(int type);
    
    void resortPiecesCard();
    inline void setSelectIndex(int index){selectIndex = index;}
    inline void virtualPressBtn(){this->pieceClickCallback(NULL);}
    
private:
	int selectIndex;//选择的item
	SGPiecesCard *card;//召唤的武将卡
	SGCCLabelTTF *commonCount;//能用碎片数量
	SGCCLabelTTF *pageLabel;
	
	CCArray *_allData;
private:
	void officerBtnClicked();
	void pieceBtnClicked();
};












#endif /* defined(__GameSanGuo__SGPiecesBagLayer__) */
