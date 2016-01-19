//
//  SGAreanLayer.h  ...无语了 arena 拼写错误就不改了  见谅见谅!!!!
//  GameSanGuo
//  竞技场主layer
//  Created by angel on 14-12-18.
//
//

#ifndef __GameSanGuo__SGAreanLayer__
#define __GameSanGuo__SGAreanLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "CCScrollView.h"
#include "SGMenu.h"
#include "SGAreanItemInfo.h"
#include "SGAreanItemFightLog.h"

typedef enum {
    
    Arean_qun,//群雄逐鹿
    Arean_log,//征战之路
    Arean_zhu//诸侯割据
    
} AreanLayerType;//展示的layer类型

class SGAreanBaseInfoData;

class SGAreanLayer: public SGBaseTableLayer
{
public:
    SGAreanLayer();
    ~SGAreanLayer();
    static SGAreanLayer * create(CCArray * arrayData,SGAreanBaseInfoData *baseInfoData, SGAreanItemInfoData * itemData,AreanLayerType type);
    void lookPlayerInfoListen(CCObject * obj);
    void enterFightListen(CCObject * obj);
    void hideMyselfInfoItem();
    void showMyselfinfoItem();
    
 protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    virtual void setIsCanTouch(bool isTouch);
    
    void onClickLuanShiStoreButton();
    void  onClickMyZhuJiangIconButton();
    void onClickGetRewardButton();
    void getArenaRewardListen(CCObject * obj);
    void buyArenaFightCountListen(CCObject * obj);
    
    void setCdTime();
    void updateCdTime();
    void setRewardTime();
    void updateRewardTime();
    
    CCString * calcTime(int time);
    
    //三大功能按钮响应方法
    void onClickButtonQun();
    void onClickButtonzhu();
    void onClickButtonLog();
    
    
    void onClickButtonRule();//规则按钮响应
    
   
    
   
    
    virtual void refreshView();
    
    void resetMyselfInfo();
public:
    void setMyselfData(SGAreanItemInfoData * myselfData);
    void resetBaseInfo();
    void initDatas(CCArray * arrayDatas);
    void setBaseInfoData( SGAreanBaseInfoData * baseInfnData);
    void updateAreanLayer();
    void setAreanLayerType(AreanLayerType type){
        m_areanLayerType  = type;
    }
private:
    
    
    void initView();
private:
    SGAreanBaseInfoData * m_pBaseInfoData;
    SGAreanItemInfoData * m_pMySelfData;
    AreanLayerType m_areanLayerType;
    CCSprite * m_pMyselfInfoItem;
    CCSprite * m_pBaseInfnBg;
    SGCCLabelTTF * m_pLabelMyName;
    SGCCLabelTTF * m_pLabelMyRank;
    SGCCLabelTTF * m_pLabelMyRankReward;
    SGButton * m_pZhujiangIcon;
    SGButton * m_pButtonLuanShi;
    
    SGCCLabelTTF * m_pLabelMyAreanCoinSum;//自己拥有的竞技场币数量
    SGCCLabelTTF * m_pLabelChallengCount;//挑战次数
    SGCCLabelTTF * m_pLabelLastTime;//剩余发奖时间
    SGCCLabelTTF * m_pLabelDescribInfo;//一句描述
    SGCCLabelTTF * m_pLabelCdTime;//战败后的冷却时间
    SGCCLabelTTF * m_pCdTime;
    SGCCLabelTTF * m_pLabelCanFight;//可挑战提示
    
    SGButton * m_pButtonQun;//群雄逐鹿按鈕
    SGButton * m_pButtonLog;//征战之路按钮
    SGButton * m_pButtonzhu;//诸侯割据按钮
    
    SGCCLabelTTF * m_pLabelQun;
    SGCCLabelTTF * m_pLabelLog;
    SGCCLabelTTF * m_pLabelZhu;
    
    SGButton * m_pButtonGetReward;
    
    CCSprite * m_prewardIcon;
    SGCCLabelTTF * m_plabelCanReward;
    SGButton * m_pbuttonRule ;
    
    
    
    
    
    
};

class SGAreanBaseInfoData: public CCObject
{
public:
    SGAreanBaseInfoData();
    ~SGAreanBaseInfoData();
    static SGAreanBaseInfoData * creat(int arenaCoin , int challengeCount, int maxCount, CCString * des, int rewardTime, int cdTime,int  canReward);
    CC_SYNTHESIZE(int ,m_areanCoin,AreanCoin);//竞技场货币数
    CC_SYNTHESIZE(int,m_challengeCount, ChallengeCount);//挑战次数
    CC_SYNTHESIZE(int,m_maxChallengeCount, MaxChallengeCount);//最大挑战次数
    CC_SYNTHESIZE( CCString *,  m_describInfo, DescribInfo);//一句描述信息交代奖励发放时间
    CC_SYNTHESIZE( int , m_rewardTime, RewardTime);//发放奖励倒计时
    CC_SYNTHESIZE( int , m_cdTime, CdTime);//冷却倒计时
    CC_SYNTHESIZE( int , m_canReward,  CanReward);//当前能够领取的累积的奖励
    
    
};



#endif /* defined(__GameSanGuo__SGAreanLayer__) */
