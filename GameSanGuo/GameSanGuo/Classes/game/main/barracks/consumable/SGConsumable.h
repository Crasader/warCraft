//
//  SGConsumable.h
//  GameSanGuo
//
//  Created by bugcode on 14-2-13.
//
//

#ifndef __GameSanGuo__SGConsumable__
#define __GameSanGuo__SGConsumable__

#include "SGBaseTableLayer.h"
#include "SGSortLayer.h"
#include "SGCCLabelTTF.h"
#include "SGOfficerCard.h"
#include "SGCardItem.h"
#include "SGConsumableCell.h"
#include "SGStorySectionLayer.h"
#include "SGBaseBox.h"

class SGConsumable : public SGBaseTableLayer, public SNSScrollViewPageDelegate, public SGSortLayerDelegate, public SGConsumableDelegate
{
private:
	SGButton *range;
	SGCCLabelTTF *pageLabel;
public:
	SGConsumable();
	~SGConsumable();
	static SGConsumable *create();
	virtual void initView();
	void backHandler();
	void initData();
	
	SNSTableViewCellItem *tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
	unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
	void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
	virtual void sortLayerCallBack(CCArray *cardArray);
	virtual void refreshPage(int type);
	void buttonClickSort();
	
	void sellComsuableHandler();//下拉菜单上的卖出按钮
	void showConsumeHandler();//下拉菜单上的展示按钮
	void freshView();//弹窗关闭后刷新列表
	void useConsumable(SGConsumableCard *card);
	void consumeUseListener(CCObject *sender);
	void sellConsumableCardListener(CCObject *sender);//卖出消耗品卡牌
	void showOpenedConsumableCardBox(std::vector<int> itemids, std::vector<int> nums, std::vector<int> types);//宝箱开启显示得到的物品
	// 借用storysectionlayer中的下拉效果
	//*******************************************************************//
	virtual void showConsumeInfoHandler(SGConsumableCell *consumeCell);
	void moveSectiontableViewDown(bool isDown);
	void resetMovingSectiontableView();
	void hideSectiontableView();
	void creatConsumeInfoView(SGConsumableCell *consumeCell);
	//*******************************************************************//
	inline void setMoneyNum(int num){getMoneyNum = num;}
	
private:
	int selectIndex;    // 选中的
    int lastSelectIndex; //上次选中的
    float lastMoveDis; //上次选中时移动的距离
	bool isMovingSection;
	NewScrollView *sectiontableView;
	SGConsumableCell *m_lastShowSection;
	int getMoneyNum;// 卖出最终获得的钱数
	
};


//卖出时出现的弹窗
class SGConsumableSellBox : public SGBaseBox
{
public:
	SGConsumableSellBox();
	~SGConsumableSellBox();
	static SGConsumableSellBox *create(SGConsumableCard *card = NULL, SGBoxDelegate *del = NULL);
	
	void initView();
	void sellCancelHandler();//卖出取消
	void sellConfirmHandler();//卖出确认
	
	void add1();
	void add10();
	void minus1();
	void minus10();

private:
	int sellNum;//要卖出的个数
	CCLabelTTF *sellNumLabel;//即时显示的钱数
	int sellMoney;//卖出的钱数
	SGCCLabelTTF *moneyNumLabel;//卖出钱数显示
	SGBoxDelegate *del;
	SGCCLabelTTF *money;//显示的钱数，要随时更新
	SGConsumableCard *m_card;//对应要卖出的卡牌
	
	
};

//卖出时的二次确认框
class SGConsumableSellConfirmBox : public SGBaseBox
{
public:
	SGConsumableSellConfirmBox();
	~SGConsumableSellConfirmBox();

	static SGConsumableSellConfirmBox *create(SGBoxDelegate *del = NULL, int sellNum = 0, int sellMoney = 0, CCString *name = NULL, int cardId = -1);
	void initView();
	void sellConfirmHandler();
	void sellCancelHandler();
	
private:
	SGBoxDelegate *del;
	int num;//卖出的数量
	int money;//卖出所得的钱数
	CCString *name;//名称
	int cardId;//要卖掉卡的id，是ssid
};








#endif /* defined(__GameSanGuo__SGConsumable__) */
