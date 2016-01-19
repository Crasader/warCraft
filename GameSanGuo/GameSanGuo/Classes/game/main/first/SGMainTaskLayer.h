//
//  SGMainTaskLayer.h
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-14.
//
//

#ifndef __GameSanGuo__SGMainTaskLayer__
#define __GameSanGuo__SGMainTaskLayer__

#include <iostream>
#include "cocos2d.h"
#include "SGBaseTableLayer.h"
#include "SGMainTaskCell.h"
#include "SGMainTaskDataModel.h"
#include "SGStorySectionLayer.h"




class SGMainTaskSubLayerDelegate;
class SGMainTaskSubLayer:public CCLayer {
private:
    SGMainTaskDataModel *_dataModel;
    SGMainTaskSubLayerDelegate *_delegate;
public:
    SGMainTaskSubLayer();
    ~SGMainTaskSubLayer();
    
    static  SGMainTaskSubLayer *create( SGMainTaskSubLayerDelegate *delegate);
    void initMsg();
    
    CCSize bgSize;
    
    int questId;
    
    
    CCSprite *kuang_sprite;
    CCSprite *reward_sprite;
    CCLabelTTF *task_Name;
    CCLabelTTF *reward_count;
    CCLabelTTF *reward_name;
    CCLabelTTF *task_description;
    
    CCLabelTTF *task_progress;
    CCLabelTTF *task_finish;
    SGButton *button;
    
    void getRewardHandle();
   
    void updateTaskSubLayer(SGMainTaskDataModel *dataModel);
    
   
    
};

class SGMainTaskSubLayerDelegate {
public:
    virtual void updateMainTaskSubLayer(SGMainTaskSubLayer *subLayer){};
};
class SGMainTaskLayer:public SGBaseTableLayer ,public SGMainTaskLayerDelegate,public  SGMainTaskSubLayerDelegate{

public:
    SGMainTaskLayer(void);
    ~SGMainTaskLayer(void);
    static SGMainTaskLayer *create(CCArray* data);
    
    virtual void entrySectionHandler(SGMainTaskCell *Item);
    virtual void showSectionInfoHandler(SGMainTaskCell *Item);
    
    virtual void updateMainTaskSubLayer(SGMainTaskSubLayer *subLayer);
private:
    int selectIndex;    // 选中的场景
    int lastSelectIndex; //上次选中的场景
    float lastMoveDis; //上次选中场景时移动的距离
    NewScrollView *sectiontableView;
     CCLayer *sectionInfoView;
    float infoViewPosY;
    SGMainTaskCell *m_lastShowSection;
    float lastShowHeight;
    bool isSectionVisible;//点击的时候section是否可见
    CCArray *subTaskArray;
    
    bool isUpdate;
    bool ishasValue;
public:
     void showIsFinishTask();
    int  getTaskFinishIndex();
    void showRunAction();
    void openSubSection(SGMainTaskCell *section);
    void closeSubSection(SGMainTaskCell *section);
private:
    void initView();
    
    //展开列表的操作
    
    void createSectionInfoView(SGMainTaskCell *section);
    void moveSectiontableViewDown(bool isDown);
    void hideSectiontableView();
    void resetMovingSectiontableView();
    bool isMovingSection;
    void dealHideSingleMove();
    
     void getRewardListener(CCObject *obj);
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);

};






#endif /* defined(__GameSanGuo__SGMainTaskLayer__) */
