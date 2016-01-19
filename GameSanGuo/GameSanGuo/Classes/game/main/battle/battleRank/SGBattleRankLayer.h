//
//  SGBattleRankLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-16.
//
//

#ifndef __GameSanGuo__SGBattleRankLayer__
#define __GameSanGuo__SGBattleRankLayer__

#include "SGBaseTableLayer.h"
#include "SGBattleRankCell.h"

class SGBattleRankLayer : public SGBaseTableLayer
{
public:
    SGBattleRankLayer(void);
    ~SGBattleRankLayer(void);
    
    static SGBattleRankLayer *create(CCArray *array,int num);
    void setSelecIndex(int index); //设置选中按钮
    
private:
    void initView();
    void initDatas(CCArray *array);
    void initButtons(float posY);
    void helpHandler(); // 帮助按钮事件
    void backHandler(); // 返回按钮事件
    void findrankhandler();
    void flushData(CCObject *sender);
    void getInfoAction(CCObject *sender);
    void reAskData();
    int newindex;
//    SGBattleRankCell *newitem;
//    SNSIndexPath * newindex;
    
private:
    int _selectIndex; // 选中的按钮
    int type;
    int number[5];
    SGCCLabelTTF *notice; // 无排名时候的提示
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual float tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath);
};

#endif /* defined(__GameSanGuo__SGBattleRankLayer__) */
