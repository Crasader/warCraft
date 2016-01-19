//
//  CGPStrengAdvLayer.h
//  GameSanGuo
//
//  Created by caoguoping on 15/12/10.
//
//

#ifndef __GameSanGuo__CGPStrengAdvLayer__
#define __GameSanGuo__CGPStrengAdvLayer__

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
    E_qianghua  = 0,
    E_jinjie,
    E_updateBtnMax,
    
}E_updateTag;

typedef enum
{
    E_attack = 0,
    E_defense,
    E_health,
    E_speed,
    E_attributeMax
    
}E_Attribute;



class CGPStrengAdvLayer : public SGBaseLayer
{
public:
    CGPStrengAdvLayer();
    ~CGPStrengAdvLayer();
    static CGPStrengAdvLayer*  create(SGBaseMilitaryCard *card, int type,int enter, bool isOnlyOfficer,ERI *info, int currentPage);
    void initView();
    void btnUpdate(CCObject* pSender, TouchEventType type);    //强化和进阶按钮
    
public:
    
     E_updateTag  mNowUpdateTag;                //0:qianghua,  1:jinjie
    
    Layout*      panelUpdate                  ;
    LoadingBar*  mBarSoul                     ;
    LoadingBar*  mBarFragments                ;
    Button*      mBtnUpdate[E_updateBtnMax]   ;
    Layout*      mPanel_update[E_updateBtnMax];
    SGCCLabelTTF  *mLabelAtt[E_attributeMax],  *mLabelAttNew[E_attributeMax];    //攻防血速
    SGCCLabelTTF  *mLabelName ;
    SGCCLabelTTF  *mLabelLevel,  *mLabelLevelNew ;   //星级和等级公用一个label
    SGCCLabelTTF  *mLabelSoulNum;
    SGCCLabelTTF  *mLabelSoulBar;   //强化进度条上的数字  5/10
    SGCCLabelTTF  *mLabelFragments;   //进阶进度条上的数字

    
};

#endif /* defined(__GameSanGuo__CGPStrengAdvLayer__) */
