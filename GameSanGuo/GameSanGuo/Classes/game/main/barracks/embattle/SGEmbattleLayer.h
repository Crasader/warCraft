//
//  SGEmbattleLayer.h
//  GameSanGuo
// 布阵界面
//  Created by 陈雪龙 on 13-1-4.
//
//

#ifndef __GameSanGuo__SGEmbattleLayer__
#define __GameSanGuo__SGEmbattleLayer__

#include "SGBaseTableLayer.h"
#include "SGBarracksLayer.h"
#include "cocos2d.h"
#include "SGOfficerCard.h"
#include "SGEmbattleItem.h"
#include "SGCCLabelTTF.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
class SGEmbattleLayer : public SGBaseLayer,  public MoveDelegate {
private:
    int colorId;
    int enterType;
    int totleGovern;//当前统御力
    
    CCArray *cards;
    CCArray *soldiers;
    SNSScrollView *scrollView;
    bool isCanCrade;
    int tradeIndex;
    int selectIndex;
    int sid;      
    int selectSoldier;    
    CCSprite *titleBg;
    CCSprite *officerframe;
    SGButton *em1;
    SGButton *em2;
    SGButton *em3;
    SGButton *pqsz;
    SGButton *zqzb;
    SGButton *cksb;
    SGCCLabelTTF *yqy;
    int emtype;
    SGEmbattleItem *lorditem;
  
    CCSprite *m_noticeEquip;
    CCSprite *m_noticeSoldier;
    
private:
    //显示要
    void showEmbattleBox(SGOfficerCard * card);
    //设置上场士兵
    void setSolider(int sender);
    void updateView();
    void showAll();
    void goback();
    void exchangPositionListener(CCObject *obj);
    void exchangembattleListener(CCObject *obj);
    void setLordOfficerListener(CCObject *obj);
    void autoAllEquipListener(CCObject *obj);//全部换装
    void dispatch();
    void embattle1();
    void embattle2();
    void embattle3();
    void equiphandler(cocos2d::CCObject*obj);
    void soldiercheck(cocos2d::CCObject*obj);
    
    SGCCLabelTTF *zongSqi;
    SGCCLabelTTF *zongSdu;
    SGCCLabelTTF *jineng;
    SGCCLabelTTF *tongyuli;
    SGCCLabelTTF *zhandouli;
    SGCCLabelTTF *lordskillinfo;
public:
    static CCRect scrollviewRect;  //scrollview的区域
    void updateBestEquipBt(bool isVisible);
    void updateNoticeSoldierBt(bool isVisible);//强化士兵通知设置
    void updateEmbattle(int type=0); //type=1 显示添加属性效果
    static SGEmbattleLayer *create(int type,int emtype);
    virtual void initMsg();
    virtual void initView();
    void callBackToSetSoldierSkin(CCSprite *spriteSkin);
    
    SGEmbattleItem* getItemByIndex(int index);
     virtual void selectedIndex(SGEmbattleItem *item);
    void showEmbattleGenerinfo(SGOfficerCard *_card,int islord=0);
    
    
public:
    void  showAddAttributeEffect(int blood,int speed,int combatle,int gov);
public:
    CC_SYNTHESIZE(int , comBatle, combattle);
    CC_SYNTHESIZE(int , m_speed, speedvalue);
    CC_SYNTHESIZE(int , m_moral, morValue);
    CC_SYNTHESIZE(int , m_gov, govValue);
    CCArray *cardArray;
    
private:
    void  removeEffectLabel();
    void showEffectLabel();
    public:
	//新开启武将空位的光效
	 void addBlinkEffect(CCNode *node);
	
public:
    SGEmbattleLayer();
    ~SGEmbattleLayer();
    virtual void showBoxCall(CCObject *obj);
    virtual void setIsCanTouch(bool isTouch);
protected:
    virtual void moveToPoint(CCPoint point, int index);
    virtual void moveToLordItemPoint(CCPoint point, int index);
    virtual void setIsSelect(bool isSelect);
    virtual bool trade();
    
    //覆盖父类的方法，控制点击区域
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher(void);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

private:
    int canUsePosition;
    void initUsePosition();
};


#endif /* defined(__GameSanGuo__SGEmbattleLayer__) */
