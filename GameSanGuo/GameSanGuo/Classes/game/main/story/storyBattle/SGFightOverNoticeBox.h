/*
 *战斗失败弹出提示框优化
 *2014-12-25
 */

#ifndef __GameSanGuo__SGFightOverNoticeBox__
#define __GameSanGuo__SGFightOverNoticeBox__

#include "SGBaseBox.h"
#include "fightOverNoticeDataManager.h"

class SGFightRewardLayer;

class SGFightOverNoticeBox : public SGBaseBox
{
public:
    /*boxState: 1=战斗失败 2=战斗未三星 3=西域商人 4=炼丹秘境*/
    static SGFightOverNoticeBox * create(SGFightRewardLayer * , int boxState);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    //请参考fightrewardlayer中炼丹处 showbox。
    void setLdIndex(int index){m_ldIndex = index ; }
    
    void boxClose();
    
    ~SGFightOverNoticeBox();
private:
    SGFightOverNoticeBox();
    
    void initView();
    
    /*读取配置表数据*/
    void getRealViewData();
    
    /*根据按钮去往id返回函数地址*/
    SEL_MenuHandler getFuncAddres(int );
    
    void openLianDanAction(CCObject *sender);
    void liandanListener(CCObject *obj);
    
    int m_boxState;
    
    bool m_firstTouch;
    
    int m_ldIndex;
    
    SGFightRewardLayer * m_delegate;
    
    FightOverNoticeData * m_fightOverNoticeData;
    
    FightOverNoticeBtnData * m_fightOverNoticeBtnA;
    FightOverNoticeBtnData * m_fightOverNoticeBtnB;
    
private:
    //各种去往…按钮的回调
    void gotoCharge(CCObject* obj);
    void gotoStreng(CCObject* obj);
    void gotoVisit(CCObject* obj);
    void gotoOfficerList(CCObject* obj);
    void gotoVipInfo(CCObject* obj);
    void gotoRewardLayer(CCObject* obj);
    void gotoWestShop(CCObject* obj);
};








#endif 


//-----------------file end.
