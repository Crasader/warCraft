//
//  SGTongQueLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 2014-2-18.
//
//

#ifndef __GameSanGuo__SGTongQueLayer__
#define __GameSanGuo__SGTongQueLayer__

#include "SGBaseLayer.h"
#include "SGBaseMilitaryCard.h"
#include "SGYangXinBox.h"

class SGTongQueLayer : public SGBaseLayer
{
public:
    SGTongQueLayer();
    ~SGTongQueLayer();
    static SGTongQueLayer *create(int spendMedal,int spendGold,int medal,int spendCoins,int refreshTime,int blessValue, std::vector<SGItemInfo> &tongqueItem, SGItemInfo &yangxing);
    void refreshGold();
    virtual void showBoxCall(CCObject *obj);
protected:
    void initView();
    void showDobkCostInfo();
    void selectTongQueCard(CCObject* obj);
    void showYangXinItem(CCObject* obj);
    void tongQueDobkSend(CCObject* obj);
    void tongQueRefreshSend(CCObject* obj);
    void tongQueDobkListener(CCObject* obj);
    void tongQueRefreshListener(CCObject* obj);
    
    void createTongQueCard(CCPoint tongQuePos);
    void startMoveHaHa(float dt);
    void dealGetDobkItem();
    void startRefreshTimeHaHa(float dt);
    void refreshTongQuePercetage();
    void initData(int spendMedal,int spendGold,int medal,int spendCoins,int refreshTime,int blessValue, std::vector<SGItemInfo> &tongqueItem, SGItemInfo &yangxing);
    void showtongqueItemDetail(SGItemInfo & tongqueItemInfo);
    void tongQueYangXinSend();
    void tongQueYangXinListener(CCObject* obj);
    void setYangXinStatus();
    void backHandler(CCObject * obj);
    void yangxinRotation();
    void setDobkLimit(float dt);
private:
    SGCCLabelTTF *havegold;
    SGCCLabelTTF *havejg;
    SGCCLabelTTF *yangxinLabel;
    CCProgressTimer *updateProgress;//进度条
    SGButton *tognqueDokbBt;
    SGButton *guangBioa;
   
    std::vector<CCPoint> movePosList;
    SGCCLabelTTF *refeshTimeCount;
    SGCCLabelTTF *dokbCostLabel;
    SGCCLabelTTF *yongqiCount;
    int nXunZhangMax;//勋章上限
    SGCCLabelTTF *refeshCostCount;
    SGCCLabelTTF *refeshCost;
    CCSprite *rewardEffect;
    CCSprite * guangBioaGuan;
    SGButton *yangxindian;
    SGButton *refreshBtn;
    CCSprite *m_dokbFlag;
    CCSprite * refeshFlag;
    float moveHaHaTime;
    int refreshTimeCount;
    CCNode* gotItem;
    int wid;
    int startPosId ;
    int stopPosId;
    bool canDobk;
    
    int m_spendMedal ;//当前翻牌所需要的勋章数目
    int m_spendGold ;//消耗元宝数
    int m_medal ;//当前勋章数
    int m_spendCoins ;//刷新消耗铜钱数
    int m_blessValue ;//祝福值
    CCPoint itemsPos;
    std::vector<SGItemInfo> m_tongqueItem;
    SGItemInfo m_yangxingItem;
    bool dobkTimeLimit;
};

#endif /* defined(__GameSanGuo__SGTongQueLayer__) */
