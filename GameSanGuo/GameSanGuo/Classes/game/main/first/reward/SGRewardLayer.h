//
//  SGRewardLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-15.
//
//

#ifndef __GameSanGuo__SGRewardLayer__
#define __GameSanGuo__SGRewardLayer__

#include "SGBaseTableLayer.h"
enum SGRewardItemType
{
    Reward_YueKa = 1,//月卡
    Reward_OpenServer,//开服奖励
    Reward_MonthlySign,//月签奖励
    Reward_OtherLianDan, //各种炼丹，共5种。
    Reward_LvlUpGold,//1升级领金子
    Reward_Caishen ,//2拜财神
    Reward_Salary ,//领工资
    Reward_Wine ,//煮酒论英雄
    Reward_Hope ,//祭天
    Reward_Inviter ,//呼朋引伴
    Reward_Invitee ,//分羹一杯
    Reward_Bind ,//绑定账号奖励
    Reward_ExchangeCode ,//兑换码
    
};

class itemObj : public CCObject
{
public:
    CC_SYNTHESIZE(int, m_itemid,item);
    CC_SYNTHESIZE(int, m_state, state);
    CC_SYNTHESIZE(int, m_count, count);
    CC_SYNTHESIZE(int, m_ishave, ishave);
    itemObj();
    ~itemObj();
};

////////
class SGRewardItemDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};
////////////
class SGRewardLayer : public SGBaseTableLayer,public SGRewardItemDelegate
{
private:
    SNSTableView *m_tableView;
    CCArray *_array;
    int select;
private:
    void initView();
    void backHandler();
    void showGetLvlUpGold();
    void showGetSalary();
    void showYueKaReward();
    void showWine();
    void showHope();
    void showliandan();
    void showcaishen();
//    void lvlUpEnterListener(CCObject *obj);
    void salaryEnterListener(CCObject *obj);
    void recoverEnterListener(CCObject *obj);
   
    void liandanEnterListener(CCObject *obj);
    void regEnterListener(CCObject *obj);
    void showInviter();
    void showInvitee();
    void showBind();
    void inviterListener(cocos2d::CCObject *obj);
    void bindListener(cocos2d::CCObject *obj);
    void showExchangeCode();
    void showExchangeCodeListener(CCObject *obj);
    void yueKaEnterListener(cocos2d::CCObject *obj);
    void showLoginReward();
    void showLoginRewardListener(CCObject *obj);
    
    //月签奖励相关。
    void showMonthlySignRequest();
    void showMonthlySignResponse(CCObject* obj);

    /////////
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
    /////////
public:
    SGRewardLayer();
    ~SGRewardLayer();
    static SGRewardLayer *create();
    void hideInviteeItem();
    void hideBindItem();
    void godEnterListener(CCObject *obj);
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
};

#endif /* defined(__GameSanGuo__SGRewardLayer__) */
