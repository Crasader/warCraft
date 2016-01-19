//
//  SGCantAdvanceBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-21.
//
//

#ifndef __GameSanGuo__SGCantAdvanceBox__
#define __GameSanGuo__SGCantAdvanceBox__

#include "SGBaseBox.h"
#include "SGOfficerCard.h"
#include "SGBaseMilitaryCard.h"

class SGCantAdvanceBox : public SGBaseBox
{
private:
    int need;
    SGBaseMilitaryCard *_card;
    SGBoxDelegate *_dele;
    int enterType;
    CCLabelTTF *warn;
    
    CCObject *_data;
    int neenGold; // 购买体力或军粮需要的金子
    
    SGButton *ok;
    SGButton *cancel;
    SGButton *store;
    const char *mailaddress;
    
    
private:
    void upAccountHandler();
    void cleanAccountHandler();
    void strengConfirmHandler();
    void strengsecondconfirm();
    void showAddGold();
    void expandBarrack();
    void managerBarrack();
    void getFullPvEPower();
    void getFullPvPPower();
    void noCoinsHandler();
    void okHandler();
    void cancelHandler();
    void enterVisitHandler();
    void liandanHandler();
    void strengHandler();
    void visitHandler();
    void cancelFromReward();
    void onlyclosebox();
    void showConGuide();
    void reConnect();
    void onClickShareButton();//boss战分享按钮响应
    //守荆州跳过关卡
    void sjzJumpGo();
    
    //守荆州手动闯关
    void sjzRushGo();
    
    //守荆州战斗二次确认-放弃
    void sjzBattleGiveUp();
    
    //守荆州战斗二次确认-继续
    void sjzBattleCarryOn();
    
    //取回军魂消费确认-yes
    void withdrawSoulCostYes();
    
    //取回军魂消费确认-no
    void withdrawSoulCostNo();
    
    //取回军魂充值确认-yes
    void withdrawSoulRechargeYes();
    
    //取回军魂充值确认-no
    void withdrawSoulRechargeNo();
    ///守荆州判断进入次数时  如果是消耗VIP进入次数时的消费确认
    void chargeconfirm();
    //投诉意见确认删除
    void confirmdelete();
    //直接进入pvp战斗页面
    void confirmJumptoPvp();
    //直接进入boss战斗页面
    void confirmJumptoBoss();
    void confirmReboot();
    void confirmBindEmai();
    //购买竞技场挑战次数
    void buyArenaFightCount();
    
    
    //商城刷新二次确认
    void shopYes();
    void shopCancel();
    //go to bind account layer direct from bind pop box
    void gotoBindAccountLayer();
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

private:
    void fullPvEPowerListener(CCObject *sender);
    void fullPvPPowerListener(CCObject *sender);
    void storeEnterListener(CCObject *obj);
    void exchangeEnterListener(cocos2d::CCObject *obj);
    
    void gotoChargeLayer();
    void boxCloseAtActivityLayer();
    //綁定游客页面  使用老账号登陆 确认
    void useOldAccountOk();
    void useOldAccountCanel();
public:
    SGCantAdvanceBox();
    ~SGCantAdvanceBox();
    
    virtual void boxClose();
    virtual void boxCloseWithOutCallBack();
    
    static SGCantAdvanceBox *create(SGBoxDelegate *bdg, SGBaseMilitaryCard *card,int type,int Coins, int power = 120, CCObject *obj=NULL,int Golds=0,const char *showMessage="");
    bool init(SGBoxDelegate *bdg, SGBaseMilitaryCard *card,int type,int Coins,int power = 120, CCObject *obj=NULL,int Golds=0,const char *showMessage="");
};

#endif /* defined(__GameSanGuo__SGCantAdvanceBox__) */
