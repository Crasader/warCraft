//
//  SGShouJingZhouRank.h
//  GameSanGuo
//
//  Created by kanata on 13-11-14.
//
//

#ifndef __GameSanGuo__SGShouJingZhouRank__
#define __GameSanGuo__SGShouJingZhouRank__

#include "SGBaseTableLayer.h"

#endif /* defined(__GameSanGuo__SGShouJingZhouRank__) */
class SGShouJingZhouRank:public SGBaseTableLayer
{
public:
    SGShouJingZhouRank();
    ~SGShouJingZhouRank();
     void setSelecIndex(int index); //设置选中按钮

//    static SGShouJingZhouRank *create(CCArray *array,int num);
    static SGShouJingZhouRank *create(CCArray *array, int pageItemsNum, int pageMax, int nowRank, int coins, int rewardNum, int time);
private:
    void initDatas(CCArray *array, int pageItemsNum, int pageMax, int nowRank, int coins, int rewardNum, int time);
    void initView();
    void backHandler(CCObject*obj);
	
	//刷新倒计时显示
	void updateTimeInfo();
	
	CCLabelTTF *tickTime;//倒计时显示的label
    int type;
    int newindex;
    int _selectIndex; //选中的按钮
    
    //当前页数（动态）
    int _pageCur;
    
    //每页显示多少条目
    int _pageItemsNum;
    
    //本次最多显示多少条目
    int _pageTotalNum;
    
    //最大页数
    int _pageMaxReal;
    
    //当前排名
    int _rankCur;
    
    //次日可领奖励
    int _rewardCoins;
    
    //追加刷新本页面
    void flushData(CCObject* obj);
	
	//排名前多少可得奖励
	int _rewardNum;
	
	//领奖剩余时间
	int _residueTime;
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
};