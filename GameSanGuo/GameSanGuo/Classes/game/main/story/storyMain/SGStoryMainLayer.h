//
//  SGStoryMainLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#ifndef __GameSanGuo__SGStoryMainLayer__
#define __GameSanGuo__SGStoryMainLayer__

#include "SGBaseTableLayer.h"

class SGStoryMainCellDelegate
{
public:
    virtual   void itemselect(SNSTableView* tableView, SNSIndexPath* indexPath){};
};
class SGStoryMainLayer : public SGBaseTableLayer,public SGStoryMainCellDelegate
{
public:
    SGStoryMainLayer(void);
    ~SGStoryMainLayer(void);
    
    static SGStoryMainLayer *create(CCArray *array);
    
    void checkVisible();
    
private:
    void initView();
    void initDatas(CCArray *array);
    void activityHandler();
    void limitMenuTouch();
    void resetMenuTouch();
    void showSceneLayer(CCObject *sender);
    void showStoryScene();
    /////////
    void itemselect(SNSTableView* tableView, SNSIndexPath* indexPath);
//    ////////
private:
    int selectIndex;
    int showIndex;
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
};

#endif /* defined(__GameSanGuo__SGStoryMainLayer__) */
