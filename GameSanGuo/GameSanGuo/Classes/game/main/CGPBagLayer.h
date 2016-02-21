//
//  CGPBagLayer.h
//  GameSanGuo
//
//  Created by caoguoping on 15/12/10.
//
//

#ifndef __GameSanGuo__CGPBagLayer__
#define __GameSanGuo__CGPBagLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GlobalConfig.h"
#include "ShiBing.h"
#include "SGUIHelper.h"
#include "SGCCLabelTTF.h"
#include "SGLayout.h"
#include "SGBaseLayer.h"
#include "SGEmbattleBox.h"
USING_NS_CC_EXT;
USING_NS_CC;
using namespace cocos2d::ui;

typedef enum
{
    E_equip  = 0,
    E_prop,
    E_material,
    E_fragments,
    E_bagTagMax
}E_bagTag;

class CGPBagLayer : public SGBaseLayer
{
public:
    CGPBagLayer();
    ~CGPBagLayer();
    static CGPBagLayer*  create(SGBaseMilitaryCard *card = nullptr, int type = 0,int enter = 1, bool isOnlyOfficer = false,ERI *info = nullptr, int currentPage = 1);
    virtual bool init(SGBaseMilitaryCard *card,int type,bool isOnlyOfficer);
    void initView();
    void btnCallItemSelect(CCObject* pSender, TouchEventType type);    //选中一个物品
    void btnCallTabSwitch(CCObject* , TouchEventType);
    void showBagEquips();
    void showBagProps();
    void showBagMaterial();
    void showBagFragments();
    
//    void btnCallBack(CCObject* pSender, TouchEventType type);   //进阶和强化的返回按钮
//    void btnCallAdvOk (CCObject* pSender, TouchEventType type);   //进阶按钮
//    void btnCallStrengOk(CCObject* pSender, TouchEventType type);   //强化按钮
//    void btnCallStrengCancel(CCObject* pSender, TouchEventType type);   //强化取消按钮
//    void btnCallAddExp(CCObject* pSender, TouchEventType type);   //加经验按钮
//    
//    void updateStrengthExp(float dt);
//    void getAttrAdd(int* atk, int* def, int* mor, int* speed, int curLvl, int nextLvl);
//    void updateAttrOne(bool isLevelUp = 1);   //1:升级,  0:进阶
//    void showOfficerBg(int quality);
    
public:
    

    CCArray* mBagEquips;
    CCArray* mBagMaterials;
    CCArray* mBagProps;
    CCArray* mBagFragments;
    
    Layout*      panelBag                  ;
    ListView*    mListBag;
    E_bagTag   mNowTabTag;
    
    
    LoadingBar*  mBarSoul                     ;
    LoadingBar*  mBarFragments                ;

    SGCCLabelTTF  *mTabNames[E_bagTagMax] ;
    Button        *mTabBtns[E_bagTagMax];
    
    SGCCLabelTTF  *mLevelName ;
    
    SGCCLabelTTF  *mLabelLevel,  *mLabelLevelNew ;   //星级和等级公用一个label
    SGCCLabelTTF  *mLabelSoulNum;
    SGCCLabelTTF  *mLabelSoulBar;   //强化进度条上的数字  5/10
    SGCCLabelTTF  *mLabelFragments;   //进阶进度条上的数字
    ImageView*   m_officerBg[4];       //绿， 蓝， 黄 , 橙
    ImageView*   mStar;
    
    int mSoulNum;  //经验数量。
    
    SGBaseMilitaryCard *_card;
    int isfromfirst;
    int _currentPage ;
    int cardType;
    int mWhich;
    ERI *EmbattleInfo;//布阵界面返回信息
    bool onlyOfficer;
    
    int strengLimitLevel;

    int originLvl;  //初次网络
    int currLvl;
    int currStar;
    int mAtkNet;
    int mDefNet;
    int mMorNet;
    int mSpdNet;
    
    
    
    int strengTimes;
    bool expCom;   //连按经验条
    bool needUpdate;
    
    
    int pieceHasNum;
    int pieceNeedNum;
    bool m_canAdcance;
    
    

    
};

#endif /* defined(__GameSanGuo__CGPBagLayer__) */
