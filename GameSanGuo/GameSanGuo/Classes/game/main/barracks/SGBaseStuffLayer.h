//
//  SGBaseStuffLayer.h
//  GameSanGuo
// 显示信息的主类
//  Created by gengshengcai on 13-1-10.
//
//

#ifndef __GameSanGuo__SGBaseStuffLayer__
#define __GameSanGuo__SGBaseStuffLayer__

#include  "SGBaseLayer.h"
#include "SGBaseMilitaryCard.h"

//当前最大星星数量
#define HELLO_MAX_STAR 10   //modified by cgp ,before is 10//

enum stuffType
{
    stuff_lvl = 100,
    stuff_exp,
    stuff_atk,
    stuff_def,
    stuff_round,
    stuff_speed,
    stuff_mor,
    stuff_gov,
	stuff_area,//武将是几格
    stuff_equipLevel//几格
};

class SGTouchLayer;

class SGBaseStuffLayer : public SGBaseLayer
{
public:
    SGCCLabelTTF *atk_add;
    SGCCLabelTTF *def_add;
    SGCCLabelTTF *round_add;
    SGCCLabelTTF *speed_add;
    SGCCLabelTTF *mor_add;
    SGCCLabelTTF * m_fightStrengthLab;
    SGCCLabelTTF * m_fightStrengthNum;
    int leftTimes;
    CCCallFuncN *_call;
    //最后一个星星的位置
    CCPoint pos;
    //记录的存放星星的层
    SGTouchLayer* starFrame;
    //记录6个星星
    CCSprite* starList[HELLO_MAX_STAR];
    //记录6个空星星
    CCSprite* starList2[HELLO_MAX_STAR];

    //记录武将名称的标题栏文字
    SGCCLabelTTF* labelTitle;
    /////////////////////////////经验条所需数据
    int exp;
    int max;
    int currexp;
    //经验
    SGCCLabelTTF* labelExp;
    ////////////////////////////
public:
    CC_SYNTHESIZE(bool, m_break, breakAction);
protected:
    
    virtual void backHandler();
    //    武将名字
    void setTitle(const char *title_ ,bool isGenerInfo=false , int PinZhi=0);
    //    星级
    void setstar(int star,int max = 0);
    //战斗力
    void setOfficerFightStrength(int fightStrength);
    //    数据库ID
    void setSsid(int iSsid);
    //    图片背景
    void setItemID(const char *cItemId,bool isFrame=false);
    //   卡片类型
    void setCardType(int type);
    //   卡牌图片
    void setCardItem(const char *ItemId);
    CCRect frameSize;
    CCSprite *title_bg;
    //横杆
    CCSprite *titleBgStick;
   
    void callBack(CCNode*node);
    //该方法变为定制
    virtual void progressCallback(CCNode*node,SGBaseMilitaryCard *card);
public:
    void updateLabel(stuffType type,int value);
    SGBaseStuffLayer();
    ~SGBaseStuffLayer();
    bool init(const char *fileName_, LayerTag tag_, bool isSpriteFrame = true);
    //该方法变为定制
    virtual void addLabel(stuffType type, CCPoint point);
    //该方法变为定制。anchorType为适应不同界面的文字锚点而定，默认0为(1,0.5)，1为(0,0.5)     //qianvalue为解决强化士兵和装备设的值  大小为强化前等级值   strengtype为解决判断士兵装备强
    // 化状态， 默认0表示，1表示经过了强化  ，解决强化士兵和装备等级数字问题   例子：假如强化前5级 强后10级 强化后开始进入页面应该显示  5级 10/100 ，现在默认显示10级 10/100
    //ismax表明现在是否满级 （用于装备  士兵信息界面）
    virtual void addValueLabel(stuffType type, int value1, float move,int value2 = 0, int anchorType = 0,int qianvalue=0 ,int strengtype=0,bool ismax=false);
    virtual void initMsg();
    //该方法变为定制
    virtual void setlabelValue(stuffType type, int value,int value2=0);
//    void labAnimation(stuffType type,CCDictionary *data);
//    void lableAction(CCNode *node, CCDictionary *data);
    //该方法变为定制
    virtual void labAnimation(stuffType type,int change);
    void lableAction(CCNode *node, int change);
    //该方法变为定制
    virtual void progressAction(CCProgressTimer *m_progress,int times,SGBaseMilitaryCard *card,CCCallFuncN* call);
    void setadvancestar();
    void playact(CCNode *node);
    void midCallBack(CCNode*node, int change);
};
    
#endif /* defined(__GameSanGuo__SGBaseStuffLayer__) */
