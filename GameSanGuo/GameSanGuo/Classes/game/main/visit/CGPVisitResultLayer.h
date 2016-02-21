//
//  CGPVisitResult.h
//  GameSanGuo
//
//  Created by caoguoping on 16/1/25.
//
//

#ifndef __GameSanGuo__CGPVisitResult__
#define __GameSanGuo__CGPVisitResult__

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

class CGPVisitResultLayer : public SGBaseLayer
{
public:
    CGPVisitResultLayer();
    ~CGPVisitResultLayer();
    
    static CGPVisitResultLayer*  create(CCArray *array, int type,int enter, bool isOnlyOfficer,ERI *info, int currentPage);
    virtual bool init(CCArray*  array,int type,bool isOnlyOfficer);
    void initView(CCArray*  array);
    void btnCallBack(CCObject* pSender, TouchEventType type);    //返回按钮
    void callVisitResult();
    void showStarts(int starLevel, int maxStarLevel);
    
    void updateAnimation(float dt);
    
public:
    
    Action_Sequnce  currentAction;
    SGBaseMilitaryCard*   mOfficer;
    CCSprite*  starList2[10];
    
    
    CCSkeletonAnimation*  mOpenDoor;

    
    Layout*   mPanelVisitResult;
    ImageView  *mImageBack;
    ImageView  *mImageAttr;
    ImageView  *mImageType;
    ImageView  *mImageAtk;
    ImageView  *mImageDef;
    ImageView  *mImageHp;
    ImageView  *mImageSpeed;
    ImageView  *mImageHeroLight;
    SGCCLabelTTF  *mHeroName;
    SGCCLabelTTF  *mLabelAtk;
    SGCCLabelTTF  *mLabelDef;
    SGCCLabelTTF  *mLabelHp;
    SGCCLabelTTF  *mLabelSpeed;
    SGCCLabelTTF  *mLabelMainSkillName;
    SGCCLabelTTF  *mLabelMainSkillRound;
    SGCCLabelTTF  *mLabelMainSkillDesc;
    SGCCLabelTTF  *mLabelPassiveSkillName;
    SGCCLabelTTF  *mLabelPassiveSkillRound;
    SGCCLabelTTF  *mLabelPassiveSkillDesc;

    Button *btnMainSkill;
    Button *btnPassiveSkill;
    Label  *mainSkillLabel;
    Label  *passiveSkillLabel;
    
    CCSprite*  mHeroIcon;
    CCSkeletonAnimation   *mSpine;
    
    int tag;
    
    
    
    
    
    
    
    
    
    
};








#endif /* defined(__GameSanGuo__CGPVisitResult__) */
