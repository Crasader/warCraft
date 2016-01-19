//
//  SGAllTaskLayer.h
//  GameSanGuo
//
//  Created by kanata on 14-3-27.
//
//

#ifndef __GameSanGuo__SGAllTaskLayer__
#define __GameSanGuo__SGAllTaskLayer__

#include <iostream>


#include "cocos-ext.h"
#include "SGMenu.h"
#include "SGBaseTableLayer.h"
#include "SGStorySectionLayer.h"
#include "SGAllTaskCell.h"

USING_NS_CC_EXT;
class SGAllTaskLayer : public SGBaseTableLayer,public SGTaskDelegate
{
private:

public:
    SGMenu *taskMenu;
    
private:
    void initView();
    void backHandler();
    void typeSelect(CCNode*node);
    void taskListListener(cocos2d::CCObject *obj);
    void linghandle(CCObject*obj);
    void getRewardListener(CCObject *obj);
    void updateview();
    void refreshTaskCell();
    CCArray*  sortarray(CCArray*arr);
    CCArray* sortmainichijanaiarray(CCArray*arr);//非每日排序
    
    void gogogo(CCNode*node);
    
    
    void showHope();
    void godEnterListener(cocos2d::CCObject *obj);
    void showGetSalary();
    void salaryEnterListener(cocos2d::CCObject *obj);
    void setspvisible(int tag,bool mieru);
    void settitle();
    void setspunvisible(int tag,bool mieru);
    
  
public:
    SGAllTaskLayer();
    ~SGAllTaskLayer();
    static SGAllTaskLayer *create(CCArray *array);
     void firstShow();
    //可以领奖且领 等待消息返回后 开启上方 和 领 的触摸。
    static bool m_buttonCanTouch;
protected:
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
    virtual void entrySectionHandler(SGAllTaskCell *section,int index);
    virtual void showSectionInfoHandler(SGAllTaskCell *section,int index);
    virtual void showBoxCall(CCObject *obj);
    
    //////
    
    void creatSectionInfoView(SGAllTaskCell *section);
    void moveSectiontableViewDown(bool isDown);
    void hideSectiontableView();
    void resetMovingSectiontableView();
   
    //需要解决一些跟新手引导/任务界面相关的触控问题。
    void setIsCanTouch(bool isTouch);

    //内部sectiontableView的menu，与setIsCanTouch配合解决触控问题。
    CCMenu* _sectionMenu;
    
private:
    CCArray*_array;
    NewScrollView *sectiontableView;
    bool isMovingSection;
    SGAllTaskCell *m_lastShowSection;
    float lastMoveDis;
    int selectIndex;    // 选中的场景
    int lastSelectIndex; //上次选中的场景
    CCSize size;
    CCSize bgSize;
    
    int typetype;
    int allid;//各种ID
    int num;//每日完成数量
    int numnum;//非每日完成数量
    SGCCLabelTTF *title;
    bool isshow;
    
    //点击每日奖励的回调   根据点击按钮的tag 确定物品的类型
    void onRewardGoodsClicked(CCObject *);
    //对应的button
    SGButton * reward_sprite;
    //点击物品的ItemId
    int m_itemId;
    int m_itemType;
//    CCSprite *kuang_sprite;
//    CCSprite *reward_sprite;
//    CCLabelTTF *task_Name;
//    CCLabelTTF *reward_count;
//    CCLabelTTF *reward_name;
//    CCLabelTTF *task_description;
//    
//    CCLabelTTF *task_progress;
//    CCLabelTTF *task_finish;
//    SGButton*go;
    //bool canTouchTitle;
};

#endif /* defined(__GameSanGuo__SGAllTaskLayer__) */
