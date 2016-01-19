//
//  SGBattleScoreLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-20.
//
//

#ifndef __GameSanGuo__SGBattleScoreLayer__
#define __GameSanGuo__SGBattleScoreLayer__

#include "SGBaseTableLayer.h"
#include "SGCreditCardModel.h"

class SGBattleScoreLayer : public SGBaseTableLayer
{
public:
    SGBattleScoreLayer(void);
    ~SGBattleScoreLayer(void);
    
    static SGBattleScoreLayer *create(CCArray *array,int type =0);
    void setSelecIndex(int index); //设置选中按钮
    SNSTableView *getTableView();
    
    void getInfoByIndex(int index);
    
private:
    SGCCLabelTTF *Count;
    int nJifenMax;//积分上限
    void initView();
    void initDatas(CCArray *array);
    void initButtons(float posY);
    void sortHandler(); // 排序按钮事件
    void backHandler(); // 返回按钮事件
    
    void flushData(CCObject *sender);
    void chgCreditCard(CCObject *sender);
    void getInfoAction(CCNode *sender);
    
public:
    int _osIndex; // 旧的道具
    int _selectIndex; // 选中的按钮
    int _selectCell;  // 选中的道具
    int entertype;    //返回类型值
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
//    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
//    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath, CCPoint position);
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath, int btnTag);

};

#endif /* defined(__GameSanGuo__SGBattleScoreLayer__) */
