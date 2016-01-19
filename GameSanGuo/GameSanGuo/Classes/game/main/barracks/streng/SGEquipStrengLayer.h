//
//  SGEquipStrengLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-11.
//
//

#ifndef __GameSanGuo__SGEquipStrengLayer__
#define __GameSanGuo__SGEquipStrengLayer__

#include "SGBaseStuffLayer.h"
#include "SGEquipCard.h"
#include "CCSpriterX.h"
class SGEquipStrengLayer : public SGBaseStuffLayer
{

private:

    int leftTimes;
    int isfromfirst;
    SGEquipCard *_card;
    SGButton *equipStrengBtn;
    SGButton *equipAutoStrengBtn;
    
    SGCCLabelTTF *equipName;
    SGCCLabelTTF *costCount;
    SGCCLabelTTF *totalCount;
    SGCCLabelTTF *equiplevel;
    
    SGCCLabelTTF *currAtk;
    SGCCLabelTTF *currDef;
    SGCCLabelTTF *currMor;
    SGCCLabelTTF *currSpd;
    
    SGCCLabelTTF *nextAtk;
    SGCCLabelTTF *nextDef;
    SGCCLabelTTF *nextMor;
    SGCCLabelTTF *nextSpd;
    
    CCSprite *equipLevels;
    CCSprite *bostLevel;
    CCSprite *finishLevel;
    
    CCLabelAtlas *finishNumLabel;
    CCLabelAtlas *bostNumLabel;
    
    CCSpriterX *m_fermEffect2;
    CCSpriterX *m_fermEffect3;
    
    CCSprite *equipItem;
    int enterType;
    //CCPoint pos;
    bool isback;
    bool isbreak;
    int adpt_wid ;
    int adpt_hei ;
    int shift_ipad ;
    int _currentPage;
    std::vector<int> processList;
    int equipStartLevel;
    int totalCoin;
protected:

    void initView();
    void autoEquipStrengHandler();
    void equipStrengHandler();
    void equipStrenghListener(CCObject *obj);
    void autoEquipStrenghListener(CCObject *obj);

    void backHandler();
    int getStrenghCost(int strenghLevel);
    void addCurrStatus(CCPoint startPos, float fontwid ,float heightRate);
    void addNextStatus(CCPoint startPos, float fontwid,float heightRate);
    void setEquipLevel(int costLevel);
    void setCostCount(int costLevel,bool isLastTime = true,int lastLevel = 1);
    void setCurrStatus(int level);
    void setNextStatus(int level);
    void setEquipData(int cardLevel,int sellPrice);
    void creatBostLevel(CCPoint orignPos);
    void showBostLevel();
    void showMainChat(bool isvisble);
    void createGuanquan(bool isfirst = false);
    void hideLables(bool hideFlag= false);
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    
    virtual void onExit();
public:
    
    void refreshView(SGEquipCard *card);
    SGEquipStrengLayer();
    ~SGEquipStrengLayer();
    static SGEquipStrengLayer *create(SGBaseMilitaryCard* card,int type,int isfirst,int currentPage);
};

#endif /* defined(__GameSanGuo__SGEquipStrengLayer__) */
