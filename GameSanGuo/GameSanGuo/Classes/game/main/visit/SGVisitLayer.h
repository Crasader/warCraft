//
//  SGVisitLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-18.
//
//

#ifndef __GameSanGuo__SGVisitLayer__
#define __GameSanGuo__SGVisitLayer__

#include "SGBaseTableLayer.h"
#include "SGVisitItem1.h"
#include "SGVisitItem2.h"
#include "SGBaseMilitaryCard.h"
#include "CCScrollView.h"
#include "SGUIHelper.h"

typedef struct
{
    int  subType;
    long starTime;
    long endTime;
    std::string description;
}VisitActivity;

class SGVisitLayer : public SGBaseTableLayer,public SGVisitDelegate,public SGVisitDelegate2
{
public:
//    CCSkeletonAnimation*  mOpenDoor;
//    void callVisitResult();
    
    
private:
    bool _touch;
    SGBaseMilitaryCard *comboCard;
    SGBaseMilitaryCard *progressCard;
    int legofftime;
    int legeqptime;
    int advofftime;
    int adveqptime;

    int tag;
    int time1;
    int time2;
    int singleCost;
    int comboCost;
    int activitylotteryviplv;//活动探访vip限制等级
//    CCArray* _array;
    CCProgressTimer *progress;
    SGButton *visit;
    SGVisitItem1 *item1;
    SGVisitItem2 *item2;
    SGCCLabelTTF *havegold;
    SGCCLabelTTF *havejg;
    int nJunGongMxa;//军工上限
    time_t currentTime;
    CCLayer *huoDongLayer;
    SGButton *visit1;
    //SGButton *visit10;
    SGButton * huoDongVisitBtn;
    SGButton * normolVisitBtn;
    CCSprite * huoDongBg;
    CCSprite * huoDongLableBg;
    CCSprite * huoDongLable;
    CCSprite *singleVisit1;
    CCSprite *yb1;
    SGCCLabelTTF * singleVisitMoney1;

    CCScrollView *scrollview2;
    const char *lotteryName;
    const char *activityLotteryName;
    SGCCLabelTTF * qidai;
    bool canVisitHuodong;
    CCSprite *huoDong;
    bool isSongHuodong;
    bool haveRequestHuoVisit;
    bool canVisitCard;
    
    //买到的卡牌
    SGPropsCard *propBuyCard;
    
private:
    void initView();
    void backHandler();
    void buyLegListener(CCObject *obj);
    void buyLegHuoDongListener(CCObject *obj);
    VisitActivity* getVisitActivityByTyew(int subType);
    long getCurrentTime();
    void showNormalVisit(CCObject *obj);

    void requestHuoDongVisit(CCObject *obj);
    void showHuoDongVisit(CCObject *obj);
    void sendHuodong1Visit(CCObject *obj);
    void sendHuodong10Visit(CCObject *obj);
    SGBaseMilitaryCard *setCardInfo(int itemType,int cardId,int num);
    void refreshHuoDongListener(CCObject *obj);
    void limitMenuTouch();
    void resetMenuTouch();
    
public:
    SGVisitLayer();
    ~SGVisitLayer();
    virtual void buyHandler(SGVisitItem2 *visitItem);
    virtual void buyLegHandler(SGVisitItem1 *visitItem);
    static SGVisitLayer *create(int visitHuo,int a,int b,int c,int d,int j,std::vector<VisitActivity>& visitActivityList,const char*lotteryName  = NULL, const char * activityLotteryName = NULL);
    virtual void setIsCanTouch(bool isTouch);
    void setVisitActivity(std::vector<VisitActivity>& visitActivityList);
    std::vector<VisitActivity> m_visitActivityList;
    void refreshOwens();
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
    //标示是活动探访或者其他探访。 outtype=2是活动探访
    static int outType;
};

#endif /* defined(__GameSanGuo__SGVisitLayer__) */
