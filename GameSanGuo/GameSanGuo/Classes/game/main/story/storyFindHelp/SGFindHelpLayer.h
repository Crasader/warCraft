//
//  SGFindHelpLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#ifndef __GameSanGuo__SGGetHelpLayer__
#define __GameSanGuo__SGGetHelpLayer__

#include "SGBaseTableLayer.h"
#include "SGBaseSortLayer.h"
#include "SGStoryBean.h"
#include "SGFindHelpCell.h"

class SGFindHelpLayer : public SGBaseTableLayer, public SGBaseSortLayerDelegate,public Itembtndelegate
{
public:
    SGFindHelpLayer(void);
    ~SGFindHelpLayer(void);
    static SGFindHelpLayer *create(int plotId = 1, bool model = true);
    void setStoryAndSceneId(int sID, int dID);
    void backHandler(CCObject *sender);
    void sortHandler(CCObject *sender);
    void initDatas(CCArray *helpers); // 初始化数据
    void showPlayerInfoListener(cocos2d::CCObject *obj);
    void gotoFightingLayer(int index);
    int getRoleId();
    void setRoleID(int roleID);
    virtual void setIsCanTouch(bool isTouch);
    ////////////////////
    virtual void itemselect(SNSIndexPath* indexpath);
    //////////////////////
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath, int btnTag);
//    void buttonClickHeadPic();
private:
    void initView(bool model = true);
    void showRewardBox(CCObject *sender);
protected:
    // 根据indexPath返回tableView的cell
    virtual SGFindHelpCell* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);

    // SGBaseSortLayerDelegate action
    virtual void sortLayerCallBack(CCArray *data);
    
private:
    bool _isTouch;
    int roleID;
    int _plotId;  // 故事的场景主ID
    bool _isStoryModel; // 故事模式还是活动模式，区分返回事件

    int _selectIndex;   // 选定的cell的index
 
    int _selectRoleId;  // 选中的好友ID
    
    SGButton *sortBtn;
    SGStoryBean *storyBean;
public:
    bool isBattle_Rend;
};

#endif /* defined(__GameSanGuo__SGFindHelpLayer__) */
