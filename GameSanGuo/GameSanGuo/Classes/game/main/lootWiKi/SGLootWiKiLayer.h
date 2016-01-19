//
//  SGLootWiKiLayer.h 武将和装备来源详情
//  GameSanGuo
//
//  by ： zyc  14-7-7.
//
//

#include "SGMainLayer.h"
#include "SGBaseBox.h"
#include "SGBaseTableLayer.h"
class SGLootWiKiMainLayer;

#ifndef __GameSanGuo__SGLootWiKiLayer__
#define __GameSanGuo__SGLootWiKiLayer__



class SGLootWiKiLayer : public SGBaseTableLayer , public SNSScrollViewPageDelegate
{
public:
    //dtor
    ~SGLootWiKiLayer();
    //static create method
    static SGLootWiKiLayer * create(CCArray * pieceLootInfo, SGLootWiKiMainLayer * , CCSize );
    //初始化数据
    void initView(CCArray * pieceLootInfo,SGLootWiKiMainLayer * );
    
    
    
    //根据indexPath返回tableView的cellItem
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section) { return this->datas->count(); }

    void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    //点击具体的碎片
    void itemSelect();
    
    //精英闯关消息回调
    void showCream(CCObject *);
    
    static void sendMsg();
private:
    CCArray * m_pieceLootInfo;
    //关闭按钮的回调
    void onCloseBtnClicked();
    
    
    //来源点击的回调
    void onSourceShouJingZhouClicked();
    void onSourceTaoMoClicked();
    void onSourceTianTiClicked();
    void onSourceChuangGuanClicked(int plotId);
    //精英闯关
    void onJingYingChuangGuan(int plotId);
    //副本
    void onInstance();
    SGLootWiKiMainLayer * m_lootWikiMainLayer;
    
    int m_selectIndex;
    
    //如果是闯关  记录plotId
    int m_plotId;
    
    //判断来源开启与否。
    void figureSourceOpenState();
    
    //存储每个来源开启与否的标志，开启为true，未开启为false。
    std::vector< std::pair<bool, std::string> > _sourceStateVec;
    
protected:
    //ctor
    SGLootWiKiLayer();
    
};

#endif /* end */
