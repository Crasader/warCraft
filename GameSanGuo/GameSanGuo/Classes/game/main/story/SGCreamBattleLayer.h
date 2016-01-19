//
//  SGCreamBattleLayer.h
//  GameSanGuo
//
//  Created by bugcode on 13-10-28.
//
//

#ifndef __GameSanGuo__SGCreamBattleLayer__
#define __GameSanGuo__SGCreamBattleLayer__


#include "SGBaseTableLayer.h"
#include "SGBaseBox.h"
class  SGCreamMainCellDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};
class SGCreamBattleLayer : public SGBaseTableLayer,public SGCreamMainCellDelegate
{
public:
    SGCreamBattleLayer(void);
    ~SGCreamBattleLayer(void);
    
    static SGCreamBattleLayer *create(CCArray *array);
    
    void checkVisible();
    void showSceneLayer(CCObject *sender);
private:
    void initView();
    void initDatas(CCArray *array);
    
    void showStoryScene();
    /////
    void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
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


#endif /* defined(__GameSanGuo__SGCreamBattleLayer__) */
