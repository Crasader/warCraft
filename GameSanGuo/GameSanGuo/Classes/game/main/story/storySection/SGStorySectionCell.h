//
//  SGStorySectionCell.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#ifndef __GameSanGuo__SGStorySectionCell__
#define __GameSanGuo__SGStorySectionCell__

#include "SGStoryBaseCell.h"
#include "SGStorySceneBean.h"
#include "SGVipGiftLayer.h"

#define SECTIONCELLEFFECTAG  -1001
#define HAND_EFFECT 9999
#define STARWARDTAG     -1111
class SGStorySectionDelegate;
class SGStorySectionCell : public SGStoryBaseCell
{
private:
	CCArray *data;//放精英已开启的所有副本storyBeanScence
    CCSprite *spriteIsPass;
    CCSprite *spriteStar;
    
    SGCCLabelTTF *labelSceneName;
    
	SGCCLabelTTF *needStarInfo;
    
	SGCCLabelTTF *joinTime;//精英副本参加时间
    
    bool _isPassLevel;
    
    void viewDidLoad();
    SGStorySceneBean* storyBean;
    SGStorySectionDelegate *delegate;
    
	//void buyCreamListener(CCObject *sender);
	bool isFromBuy;//是否是购买后直接进入战斗
public:
	CCMenu *menu;
	SGButton *backBtn;
	SGButton *_bigBGBtn;
///////////
	
///////////    /
	
	virtual void onEnter();
	
    SGStorySectionCell();
    ~SGStorySectionCell();
    void showSectionInfoHandler();
    void entrySectionHandler();
    static SGStorySectionCell* create();
    static SGStorySectionCell* create(SGStorySectionDelegate *delegate_, SGStorySceneBean* bean, CCArray *data = NULL);
    void setData(SGStorySceneBean* bean, CCArray *data = NULL);
    SGStorySceneBean *getGift();
};

class SGStorySectionDelegate
{
public:
    virtual void entrySectionHandler(SGStorySectionCell *giftItem){};
    virtual void showSectionInfoHandler(SGStorySectionCell *giftItem){};
	virtual int getSelectIndex();
	virtual void gotoPurchPower();
};


class SGCreamBattleBox : public SGBaseBox
{
private:
	
public:
	SGCreamBattleBox();
	~SGCreamBattleBox();
	
	static SGCreamBattleBox *create(SGBoxDelegate *del = NULL, int buyCompTimes = 0, int price = 0, int storyId = 20000,
									bool havechance=true,int canbuytimes=1,int willaddcount=1, int nVip = 0);
	void initView();
	
	void pressOKButton();
	void pressCancelButton();
	virtual void boxClose();
    void closebox();
private:
	int buyCompTimes;//已经购买的次数
	int price;//此次购买的价格
	int storyId;// 故事ID
    bool _havechance;
    int  canBuyTimes; //最大购买次数
    int  willAddCount;//增加次数
	int nextVip;//升到下一个vip
};




#endif /* defined(__GameSanGuo__SGStorySectionCell__) */
