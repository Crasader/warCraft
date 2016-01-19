//
//   背包--》武将界面
//
//

#ifndef __GameSanGuo__SGGeneralsBagLayer__
#define __GameSanGuo__SGGeneralsBagLayer__

#include "SGBaseTableLayer.h"
#include "SGSortLayer.h"
#include "SGCCLabelTTF.h"
#include "SGOfficerCard.h"
#include "SGCardItem.h"
class SGGeneralsBagLayer : public SGBaseTableLayer,public SGSortLayerDelegate, public SNSScrollViewPageDelegate,public SGCardItemDelegate
{
    
    private :
    int _currentRow;
private:
    SGCCLabelTTF * m_pDrawsLabel;
    SGCCLabelTTF *pageLabel;
    SGButton *range;
    CCArray *canUP;
    CCArray *_allData;
    CCArray *_sortData;
    int selectId;
    int enterType;
    int swichType;
    int x_;
    int stars[5];
private:
	//批量卖出
	//void batchSell();
    void backHandler();
    void sortHandler();
    void advanceHandler(SGOfficerCard *card);
    void strengHandler(SGOfficerCard *card);
    void buttonClickCanUpStar();
    int getCanUpStarGeneralCount();
    void initSomeProcess();
    virtual void sortLayerCallBack(CCArray *cardArray);
    ///////
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);

protected: 
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);   
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 设置翻页
    virtual void refreshPage(int type);
   
public:
    void openStrengView(int index);
    void openAdvanceView(int index);
    void itemselectByIndex(int index);
public:
    SGGeneralsBagLayer();
    ~SGGeneralsBagLayer();
    static SGGeneralsBagLayer *create(int type,int x=0,int currentPage=1000,int currentRow=0);
    virtual void initMsg();
    void initView();
    
    CC_SYNTHESIZE_RETAIN(CCString*, m_name, S_name);
	
private:
	void officerBtnClicked();
	void pieceBtnClicked();
    bool officerCanSummon();
};
#endif /* defined(__GameSanGuo__SGGeneralsBagLayer__) */
