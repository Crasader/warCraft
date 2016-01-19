//
//  SGJingZhouProcessLayer.h
//  GameSanGuo
//
//  Created by kanata on 14-2-14.
//
//

#ifndef __GameSanGuo__SGJingZhouProcessLayer__
#define __GameSanGuo__SGJingZhouProcessLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "SGShouJingZhouProcessCell.h"
#include "SGStorySectionLayer.h"


class SGJingZhouProcessLayer:public SGBaseTableLayer,public SGJingZhouProcessDelegate
{
public:
    SGJingZhouProcessLayer();
    ~SGJingZhouProcessLayer();
    static SGJingZhouProcessLayer *create(CCArray*array);
    void initView();
    void backHandler();


    virtual void entrySectionHandler(SGShouJingZhouProcessCell *section,int index);
    virtual void showSectionInfoHandler(SGShouJingZhouProcessCell *section,int index);
    
    
    //////
    void dealFirstSectionInfo();
	void setSectionFirstInfo(SGShouJingZhouProcessCell *section);
	void showFirstSectionInfo();
    void creatSectionInfoView(SGShouJingZhouProcessCell *section);
    void moveSectiontableViewDown(bool isDown);
    void hideSectiontableView();
    void resetMovingSectiontableView();
    
    
protected:
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
    
private:
     CCArray*_array;
     NewScrollView *sectiontableView;
     bool isMovingSection;
     SGShouJingZhouProcessCell *m_lastShowSection;
     float lastMoveDis;
     int selectIndex;    // 选中的场景
     int lastSelectIndex; //上次选中的场景
    
     CCSize size;
    CCSprite* info;
};
#endif /* defined(__GameSanGuo__SGJingZhouProcessLayer__) */
