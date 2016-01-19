//
//  SGBattleFieldLayer.h
//  GameSanGuo
//
//  Created by wwl on 13-10-8.
//
//

#ifndef __GameSanGuo__SGBattleFieldLayer__
#define __GameSanGuo__SGBattleFieldLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "SGBattleFieldItem.h"

class  SGBattleFieldLayer : public SGBaseTableLayer ,public SGStorySectionDelegate {
    
public:
    
    SGBattleFieldLayer();
    ~SGBattleFieldLayer();
    static SGBattleFieldLayer* create();
    
    void initData();
    void initView();

    void backButton(CCObject *sender);
    
    void creatSectionInfoView(SGBattleFieldItem *section);
    void gotoMatchRequest();

    void showSeasonMainLayer(CCObject *sender);
    void moveSectiontableViewDown(bool isDown);
    void hideSectiontableView();
protected:
// 根据indexPath返回tableView的cell
virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
// 返回tableView的cell数
virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
// 每个item的点击事件
virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);

    
virtual void entrySectionHandler(SGBattleFieldItem *section);
virtual void showSectionInfoHandler(SGBattleFieldItem *section);
    
    void setIsAct();

    
public:
    int pointer;    // 选中的场景
    int indexPointer;// 
    float lastMoveDis; //上次选中场景时移动的距离
    SNSScrollView *sectiontableView;
    bool isact;
};

#endif /* defined(__GameSanGuo__SGBattleFieldLayer__) */
