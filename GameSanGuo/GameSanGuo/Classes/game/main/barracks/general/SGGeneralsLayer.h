//
//  SGGeneralsLayer.h
//  GameSanGuo
//   武将界面
//  Created by gengshengcai on 13-1-8.
//
//

#ifndef __GameSanGuo__SGGeneralsLayer__
#define __GameSanGuo__SGGeneralsLayer__

#include "SGBaseTableLayer.h"
#include "SGSortLayer.h"
#include "SGCCLabelTTF.h"
#include "SGOfficerCard.h"
#include "SGCardItem.h"
class SGGeneralsLayer : public SGBaseTableLayer,public SGSortLayerDelegate, public SNSScrollViewPageDelegate,public SGCardItemDelegate
{
    
    private :
    int _currentRow;
private:
    SGCCLabelTTF * m_pDrawsLabel;
    SGCCLabelTTF *pageLabel;
    SGButton *buttonHaveUpGerneral;
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
//    virtual void registerWithTouchDispatcher();
//    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual  void onEnter();

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
    SGGeneralsLayer();
    ~SGGeneralsLayer();
    static SGGeneralsLayer *create(int type,int x=0,int currentPage=1000,int currentRow=0);
    virtual void initMsg();
    void initView();
    
    CC_SYNTHESIZE_RETAIN(CCString*, m_name, S_name);
};
#endif /* defined(__GameSanGuo__SGGeneralsLayer__) */
