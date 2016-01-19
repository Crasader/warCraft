//
//  SGEquipsLayer.h
//  GameSanGuo
// 装备界面
//  Created by gengshengcai on 13-1-11.
//
//

#ifndef __GameSanGuo__SGEquipsLayer__
#define __GameSanGuo__SGEquipsLayer__

#include "SGBaseTableLayer.h"
#include "SGSortBox.h"
#include "SGOfficerCard.h"
#include "SGEquipCard.h"
#include "SGCardItem.h"
class SGEquipsLayer : public SGBaseTableLayer ,SGSortLayerDelegate, public SNSScrollViewPageDelegate,public SGCardItemDelegate
{
private:
    SGButton *range;
    SGOfficerCard *officerCard;
//    SGEquipCard *equipCard;
    int equipType;
    int swithType;
    int entertype;
    int x_;
    CCArray *canUP;
    CCArray *equipCards;
    SGButton* buttonHaveUpGerneral;
    CCLabelTTF* m_pDrawsLabel;
    int getCanUpStarGeneralCount();
    int stars[5];
    CCArray *_sortData;
    CCArray *_allData;
    SGCCLabelTTF *pageLabel;
private:
    void backHandler();    
    virtual void sortLayerCallBack(CCArray *cardArray);
    void buttonClickSort();
    void buttonClickCanUpStar();
    void initSomeProcess();    
    void strengHandler(SGEquipCard *card);
    void advanceHandler(SGEquipCard *card);
    void initDatas(int type=0);
    ///////
    void itemselect(SNSTableView* tableView, SNSIndexPath* indexPath);
    //////
	void batchSell();
    //装备不足5件 前往购买
    void buyBtnClicked();
    void gotoJiTian();
    void onJiTianMsg(CCObject *);
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);    
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 设置翻页
    virtual void refreshPage(int type);
public:
    SGEquipsLayer();
    ~SGEquipsLayer();
    static SGEquipsLayer *create(SGOfficerCard *card,int equiptype,int type,int x=0,int currentPage=1000,int currentRow=0);
    void initView();
};
#endif /* defined(__GameSanGuo__SGEquipsLayer__) */
