//
//  SGFateInfoLayer.h
//  GameSanGuo
//
//  Created by Fygo Woo on 11/17/13.
//
//

#ifndef GameSanGuo_SGFateInfoLayer_h
#define GameSanGuo_SGFateInfoLayer_h

#include "SGBaseTableLayer.h"

class SGFateInfoItem;
class SGOfficerCard;

class SGFateInfoLayer : public SGBaseTableLayer
{
public: //method
    //构造
    SGFateInfoLayer();
    
    //析构
    ~SGFateInfoLayer();
    
    //创建
    static SGFateInfoLayer* create(SGOfficerCard* card, int enterType = 0);
    
    //获得节点数据
    void initData();
    
    //初始化视图
    void initView();
    
    //确定按钮的回调
    void confirmHandler();
    
    //武将卡对象
    CC_SYNTHESIZE(SGOfficerCard*, m_card, Card);
    
protected: //method
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    //定义tableView每一行或列的高度
   // virtual float tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath);
    
private: //method
    //触控开关
    void setTouchPad(bool bl);
    int enterType;

    
public: //member
    
protected: //member
    
private: //member
    
};

#endif
