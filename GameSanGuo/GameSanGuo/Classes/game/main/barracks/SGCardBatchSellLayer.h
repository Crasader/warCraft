//
//  SGCardBatchSellLayer.h
//  GameSanGuo
//
//  Created by bugcode on 14-2-11.
//
//

#ifndef __GameSanGuo__SGCardBatchSellLayer__
#define __GameSanGuo__SGCardBatchSellLayer__

#include <iostream>

#include "SGBaseTableLayer.h"
#include "SGSortLayer.h"
#include "SGCCLabelTTF.h"
#include "SGOfficerCard.h"
#include "SGCardItem.h"

class SGCardBatchSellLayer : public SGBaseTableLayer,public SGSortLayerDelegate, public SNSScrollViewPageDelegate,public SGCardItemDelegate
{
private:
	int selectItem[300][30];//对应每页选择的item，行是每一页的item，列是页数
	int selectNum;
	int curRow;
	int enterType;
	int currPage;
	int totalMoney;//选中之后卡牌加起来的钱数
    int m_ntwoStarCount;
    int m_nthreeStarCount;
	SGButton *range;//排序按钮
	SGCCLabelTTF *pageLabel;//页数显示
	SGCCLabelTTF *selCardCount;// 选择卡牌数量label
        SGCCLabelTTF *nextLine1;
	SGCCLabelTTF *curCount;//当前容量
	SGCCLabelTTF *afterSoldCount;//卖出之后
	SGCCLabelTTF *countChange;//军营容量变化
	SGCCLabelTTF *coinsCount;//卡牌值多少钱
	SGButton *batchSellButton;//卖出按钮
	CCArray *isSelectArray;//已选择卡牌的数组
	bool isPressAutoSell;//是否自动卖出弹框中退出
	
    
    CCSprite *batch2StarShow;
    CCSprite *batch3StarShow;
    CCSprite *batch4StarShow;
    
	enum CardType {OFFICER=6, EQUIP, SKILL = 9};
	
	virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
	virtual void sortLayerCallBack(CCArray *cardArray);
	
	//初始化各卡牌数组
	void initDatas(CardType ct);
	
	void sortHandler();//排序回调
    void autoSelectCard();//自动选卡
	void autoSellCard();//自动卖出的按钮回调
	void batchSellCard(); //选中之后卖出按钮
    void changeSellInfo(int sellCount);
	void changeSelectCount(bool initShowSelect = true); // 共选中多少卡牌
	int computeTotal();//计算共有多少卡牌
	//int computeSellMoney(SGBaseMilitaryCard *_card);//算出卡牌钱数
    void updateSelectStatus();
    void select2StarKind();
    void select3StarKind();
    void select4StarKind();

public:
	SGCardBatchSellLayer();
	~SGCardBatchSellLayer();
	/*	enterType:来自于哪个页面,为初始化data卡牌数据
	 
	 */
	void backHandler();//返回
	static SGCardBatchSellLayer *create(int enterType, int curPage = 1000, int curRow = 0);
	virtual void initMsg();
    void initView();
	bool getIsPressAuto();
	void setIsPressAuto(bool isPress);
	void clearSelectedArray(bool clearCount = true);
	inline int getItemType(){return enterType;}
    
    int getAutoSellStars();
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 设置翻页
    virtual void refreshPage(int type);
};

//二次确认框
class SGBatchSellBox : public SGBaseBox
{
public:
	SGBatchSellBox();
	~SGBatchSellBox();
	
	static SGBatchSellBox *create(SGBoxDelegate *del, CCArray *array, int total = 0);
	void initView(int total);
	void sellConfirm();
	void sellCancel();
	void autoSellConfirmListener(CCObject *sender);
	void sellConfirmListener(CCObject *obj);
private:
	CCScrollView *m_scrollview;//滑动区，显示已经选择的武将
	CCArray *selectedCard;//选择武将的数组传递过来
	SGBoxDelegate *del;
};

//scrollview中的item
class SGTableItem : public CCLayer
{
public:
	SGTableItem();
	~SGTableItem();
	static SGTableItem *create(SGBaseMilitaryCard *card);
	void initView(SGBaseMilitaryCard *card);
};

//二次确认框
class SGBatchSellBox_x : public SGBaseBox
{
public:
	SGBatchSellBox_x();
	~SGBatchSellBox_x();
	
	static SGBatchSellBox_x *create(SGBoxDelegate *del, CCArray *array, int total = 0, int twoStar = 0, int  threeStar= 0 );
	void initView(int total,int twoStar , int  threeStar );
	void sellConfirm();
	void sellCancel();
	void autoSellConfirmListener(CCObject *sender);
	void sellConfirmListener(CCObject *obj);
private:
	CCArray *selectedCard;//选择武将的数组传递过来
	SGBoxDelegate *del;
};

//scrollview中的item




#endif /* defined(__GameSanGuo__SGCardBatchSellLayer__) */
