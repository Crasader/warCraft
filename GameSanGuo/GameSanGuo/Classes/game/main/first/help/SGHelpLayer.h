//
//  SGHelpLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-7.
//
//

#ifndef __GameSanGuo__SGHelpLayer__
#define __GameSanGuo__SGHelpLayer__
#include "SGBaseTableLayer.h"
#include "SGHttpClient.h"
#include "SGHTTPDelegate.h"
#include "SGBarracksLayer.h"
class SGHelpLayer : public SGBaseTableLayer , public SGHTTPDelegate,public LabelItemDelegate
{
private:
    void showShareBox();//分享游戏
    void initView();
    void showCards();
    void playerSet();
    void help();
    void invite();
    void advice();
    void band();
    void change();
    void backHandler();
    void enterCardInfo(CCObject *obj);
    
    void itemselect(SNSTableView* tableView, SNSIndexPath* indexPath);
	//显示SDK中的用户中心或者社区中心
	void enterUserCommunity();
    //进入好友功能。
    void showFriendFunc();
private:
    int  itemNum;
public:
    SGHelpLayer();
    ~SGHelpLayer();
    static SGHelpLayer *create();
    
    virtual void requestFinished(int msgid, cocos2d::CCObject *data);
    virtual void requestFailed();

    virtual void showBoxCall(CCObject *obj);
	
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    virtual void setIsCanTouch(bool isTouch);
    
};
#endif /* defined(__GameSanGuo__SGHelpLayer__) */
