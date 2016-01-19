//
//  SGFightRewardLayer.h
//  GameSanGuo
//
//  Created by lizi on 13-7-11.
//
//

#ifndef __GameSanGuo__SGFightRewardLayer__
#define __GameSanGuo__SGFightRewardLayer__

#include "SGBaseTableLayer.h"
#include "SGStoryReward.h"
#include "PvpFight.pb.h"
#include "SGBaseMilitaryCard.h"
#include "SGMainLayer.h"
#include "SGPlayerInfo.h"
#include "SGScrollViewSubview.h"

class SGFightRewardLayer : public SGBaseTableLayer
{
private:
    SGCCLabelTTF *labelExp;
    SGCCLabelTTF *labelExpA;
    SGCCLabelTTF *labelCoin;
    SGCCLabelTTF *labelGold;
//    SGCCLabelTTF *labelJun;
    SGCCLabelTTF *labelRate;
    ////////////
    SGButton*dhzl;//////守荆州继续闯关，待会再来按钮
    SGButton*jxcg;
    /////////////
    //新增type，1表示为天梯战进入，2表示守荆州进入。
    //现已废弃，BattleManager里面有getBattleType来标识本次战斗的类型，这里的enterType无需使用了。
    int entertype;
    ////////////
    CCSprite *conSprite;
    CCSprite *winSprite;
    CCProgressTimer *proTimer;
    
    // 防止多次调用
    int currCoin;
    int currGold;
    int currExps;
    int currExps2;
    SGPlayerInfo *playInfo;
    SGMainLayer *mainLayer;
    
    // 经验及升级相关
    int cMaxExp1;
    int cMaxExp2;
    int cMaxExpTmp;
    
    //这是当前显示的左侧经验值
    int _addExp;
    
    int _addCoin;
    int _addGold;
    int _addJun;
    
    int _addExp1;

    int _totalExp;
    int _totalExp2;
    
    int _totalCoin;
    int _totalGold;
    int _totalJun;
    
    bool _isFirstTouch;
    bool _isFinishStar;
    bool  _sjzcannottouch;
    //////
    CC_SYNTHESIZE(bool, m_jump, jump);//守荆州连续跳过 屏幕可触问题
    
	int soulNum;// 军魂数量
	
    /////
    
    int upLevel;
    int currLevel;
    
    CCSprite *star1;
    CCSprite *star2;
    CCSprite *star3;
    CCScrollView * m_rewardScrollView;
    SGScrollViewSubview * scrollview;
    int rateNum;
    CC_SYNTHESIZE(int, m_reqPvpMain, ReqPvpMain);
    
    bool m_isShowExoticMerchant;
    
public:
    SGFightRewardLayer(void);
    ~SGFightRewardLayer(void);
	
	
	void sendContinue();
	void continueResp(CCObject *sender);
    
    static SGFightRewardLayer* create(SGStoryReward *reward,int type = 1 , bool isShowExoticMerchant = false);
    static void gobackToStoryLayer(); //回到原来的界面
    
    void runRateSp(int rate); // 显示战斗评价
    void runRateSP1(cocos2d::CCObject *obj,int num);
    void finishShowStar();
    void runExpLabel();
    void runCoinLabel();
    void runGoldLabel();
    void runJunLabel();
    
    void setAllLabels();
    void fuckLayer();
    void setStar(int rate);
    ///////专门用来处理守荆州奖励页面中  点击继续闯关点击跳过战斗出现的问题
    void  updateAnimation(SGStoryReward *reward);
    void showTheStorySectionListener(CCObject * sender);//显示精英节列表
    void stopRequestPvpMain();
private:
    void initView(SGStoryReward *reward);
    void confHandler();
    void showSceneLayer(CCObject *sender);
    
    //获取最新活动副本数据
    void activityListener(CCObject *sender);
    
    //守荆州-继续闯关
    void continuechuangguan(CCObject*obj);//////sjz
    
    //守荆州-待会再来
    void waitfornext(CCObject*obj);//////sjz
	void getFriendOfficerListener(CCObject *sender);
    
    //守荆州-军魂换礼
    void exchangeHandler(CCObject*obj);
    
    void  sjztongguan();
    //显示分享按钮
    void showShareLayer();
    void continuefight();
    //响应分享按钮点击
    void onClickShareJinZhouButton();
    void onClickShareButton();
   
    void delayShowPvpMain();
    
public:

    int ldanCount;
private:
    bool isMoved;
    
    //持有奖励消息，setAllLabel需要使用。
    SGStoryReward* _rewardData;
    
public:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
	// 返回tableView的列数
	virtual unsigned int tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section);
    
protected:
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
};

#endif /* defined(__GameSanGuo__SGFightRewardLayer__) */
