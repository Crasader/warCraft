//
//  SGSoulExchangeLayer.h
//  GameSanGuo
//
//  Created by Fygo Woo on 11/28/13.
//
//  @description: 军魂换礼UI界面，目前从守荆州进入。
//  @update: 本类废弃。bymm 2014.08.01
//

#ifndef __GameSanGuo__SGSoulExchangeLayer__
#define __GameSanGuo__SGSoulExchangeLayer__

#include "cocos2d.h"
#include "SGBaseLayer.h"
#include "SGSjzData.h"
#include "SurvivalModule.pb.h"


USING_NS_CC;

class SGTouchLayer;

class SeRewardTypeId:public CCObject
{
public:
    int typetype;
    int idid;
  
    
    
public:
    SeRewardTypeId();
    ~SeRewardTypeId();
    
public:
    CC_SYNTHESIZE(int , m_typetype, typetype);
    CC_SYNTHESIZE(int , m_idid, idid);
   
    
    
} ;



//军魂换礼LAYER
class SGSoulExchangeLayer : public SGBaseLayer
{
//======function!======
public:
    //构造
    SGSoulExchangeLayer();
    
    //析构
    ~SGSoulExchangeLayer();
    
    //创建
    static SGSoulExchangeLayer* create(int et);
    
    //初始化视图
    void initView();
    
    //返回按钮
    void btnBackHandler();
    
    //取回军魂按钮
    void btnWithdrawHandler();
    
    //设置enterType，进入方式。1=守荆州UI，返回守荆州UI。2=战败UI，返回主界面。-1=非法
    CC_SYNTHESIZE(int, _enterType, EnterType);
    
    //取回军魂消息send
    void sendWithdrawSoulRequest();
    
    //取回军魂消息recv
    void recvWithdrawSoulResponse(CCObject* obj);
	
	//动态更新进度标签的数字
	void updateLabelValue(float dtime);
    void updateActiveLabel(float dt);
	
protected:
    
private:
    //灌注军魂消息send
    void sendInjectSoulRequest();
    
    //灌注军魂消息recv
    void recvInjectSoulResponse(CCObject* obj);
    
    //灌注时的数字操作
    void changeSoulAmount();
    
    //显示战绩提示窗口
    void showFinalInfoNotice();
    
    //开始灌注，生成当前活动进度条/数字标签/索引
    void startInjection(SoulTag type);
    
    //终止灌注
    void stopInjection();
    /////重载终止灌注
    void stopInjection(SoulTag type);
    
    
    
    //试图领奖send，传入SoulTag区分是哪个箱子
    void sendGetRewardRequest(SoulTag type);
    
    //试图领奖recv
    void recvGetRewardResponse(CCObject* obj);
    
    //检查哪个箱子可以打开
    void checkBoxOpen();
    
    //产生三大件，在这里进行子节点的添加
    void generateInnerLayers(SoulTag type);
    ///hehehe
    void showlayerhehe();
    void haha(CCObject*obj);
    void actionall(std::vector<SeRewardTypeId*>,SoulTag type);
    void actionall1(CCObject*obj);
    void actionall2(CCObject*obj);
    void actionall3(CCObject*obj);
    void actionall4(CCObject*obj);
    void actionall5(CCObject*obj);
    void actionall6(CCObject*obj);
    void visible(CCObject*node);
    
//======member!======
public:
    
protected:
    
private:
    //当前活动的进度条
    CCProgressTimer* _activeProg;
    
    //当前活动的数字标签
    SGCCLabelTTF* _activeLabel;
    
    //当前活动的箱子当前值，点击时决定。
    int _activeAmntCur;
    
    //当前活动的箱子最大值，点击时决定。
    int _activeAmntMax;
    
    //当前活动的索引
    SoulTag _activeType;
    
    //本次消耗的军魂数量，注入后清零。
    int _soulCost;
    
    //本次拥有的军魂数量，注入后减去消耗。
    int _soulTotal;
    
    //军魂总数标签
    SGCCLabelTTF* _labelTotal;
    int nJunHunMax;//军魂上限
    
    //>>>>传奇系列
    
    //传奇触控
    SGTouchLayer* _touchS;
    
    //传奇按钮注入
    CCSprite* _imageS;
    
    //传奇进度条
    CCProgressTimer* _progS;
    
    //传奇数字标签
    SGCCLabelTTF* _labelS;
    
    //传奇箱子
    CCSprite* _boxS;
    
    //传奇点击打开
    CCSprite* _waOpenS;

    
    //传奇星星
    std::vector<CCSprite*> _starListS;
    
    //>>>>精粹系列
    
    //精粹触控
    SGTouchLayer* _touchA;
    
    //精粹按钮注入
    CCSprite* _imageA;
    
    //精粹进度条
    CCProgressTimer* _progA;
    
    //精粹数字标签
    SGCCLabelTTF* _labelA;
    
    //精粹箱子
    CCSprite* _boxA;
    
    //精粹点击打开
    CCSprite* _waOpenA;
    
    //精粹星星
    std::vector<CCSprite*> _starListA;
    
    //>>>>荆州系列
    
    //荆州触控
    SGTouchLayer* _touchB;
    
    //荆州按钮注入
    CCSprite* _imageB;
    
    //荆州进度条
    CCProgressTimer* _progB;
    
    //荆州数字标签
    SGCCLabelTTF* _labelB;
    
    //荆州箱子
    CCSprite* _boxB;
    
    //荆州点击打开
    CCSprite* _waOpenB;
    
    //荆州星星
    std::vector<CCSprite*> _starListB;
    //漩涡们
    CCSprite* eddyS_S;//S装备漩涡
    CCSprite* eddyS_A;//A装备漩涡
    CCSprite* eddyS_B;//B装备漩涡
    //光效们
    CCSprite*hikari;
    CCSprite*hikari_S;
    CCSprite*hikari_A;
    CCSprite*hikari_B;
    CCSprite*shita;
    CCSprite*ue;
    CCSprite*ue2;
    std::vector<SeRewardTypeId*>vect;
	
	
	//三个箱子对应的进度条的数字显示
	CCLabelTTF *_labelPercentS;
	CCLabelTTF *_labelPercentA;
	CCLabelTTF *_labelPercentB;
	
	CCLabelTTF *_curActiveLabel;// 当前活动的数字进度
	
	//三个标签字符动态变化
	int percentA;
	int percentB;
	int percentS;
	
    
    //控制能否继续开箱子
    CC_SYNTHESIZE(bool, _canOpenBox, CanOpenBox);
    
//触控相关
public:
    //began传奇
    bool helloBeganSSS(CCTouch *pTouch, CCEvent *pEvent);
    
    //传奇开箱子回调
    void openCofferHandlerS();
    
    //began精粹
    bool helloBeganAAA(CCTouch *pTouch, CCEvent *pEvent);
    
    //精粹开箱子回调
    void openCofferHandlerA();
    
    //began荆州
    bool helloBeganBBB(CCTouch *pTouch, CCEvent *pEvent);
    
    //荆州开箱子回调
    void openCofferHandlerB();
    
    //moved
    void helloMovedSSS(CCTouch *pTouch, CCEvent *pEvent);
    void helloMovedAAA(CCTouch *pTouch, CCEvent *pEvent);
    void helloMovedBBB(CCTouch *pTouch, CCEvent *pEvent);
    
    //ended
    void helloEndedSSS(CCTouch *pTouch, CCEvent *pEvent);
    void helloEndedAAA(CCTouch *pTouch, CCEvent *pEvent);
    void helloEndedBBB(CCTouch *pTouch, CCEvent *pEvent);
    
    //cancelled
    void helloCancelledSSS(CCTouch *pTouch, CCEvent *pEvent);
    void helloCancelledAAA(CCTouch *pTouch, CCEvent *pEvent);
    void helloCancelledBBB(CCTouch *pTouch, CCEvent *pEvent);
    
};

#endif /* defined(__GameSanGuo__SGSoulExchangeLayer__) */
