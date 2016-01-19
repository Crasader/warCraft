//
//  SGTestScene.h
//  Created by caoguoping on 15-07-31.
//
//


#ifndef __GameSanGuo__SGTestScene__
#define __GameSanGuo__SGTestScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GlobalConfig.h"
#include "ShiBing.h"
#include "SGUIHelper.h"
#include "SGCCLabelTTF.h"
#include "SGLayout.h"
#include "CGPStrengAdvLayer.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace cocos2d::ui;
//typedef enum
//{
//    ACT_wait = 0,      //待机
//    aAction_1 = 1,          //准备
//    ACT_moveDown = 3,          //向下移动
//    ACT_atkDown = 4,          //向下攻击
//    ACT_moveUp = 7,          //向上移动
//    ACT_atkUp = 8,          //向上攻击
//    ACT_null = 10,         //空
//
//}Action_Sequnce;


//typedef enum
//{
//    ACT_wait = 0,           //待机
//    ACT_xuli = 1,          //蓄力
//    ACT_moveDown = 3,          //向下移动
//    ACT_atkDown = 4,          //向下攻击
//    ACT_moveUp = 7,          //向上移动
//    ACT_atkUp = 8,          //向上攻击
//    ACT_null = 10,         //空
//    
//}Action_Sequnce12;

//static char acActionNames[11][30] =
//{
//    "daiji",
//    "xuli",
//    "",    //2
//    "yidong_down",
//    "gongji_down",
//    "",
//    "", //6
//    "yidong_up",
//    "gongji_up",
//    "",   //9
//    ""  //10
//    
//    
//    
//    
//};


class SGTestScene :public CCScene
{
public:
    TouchGroup* m_pUIGroup;
    
    Button*   myButton;
    float scaleSum;
    
public:
    //void update(float deltaTime);
    virtual void onEnter();
    void  changeAction(CCObject*  obj, Action_Sequnce actSeq);
    void spineTest();
    void effectTest();
    void cocostudioTest();
    void gotoButtonOk(CCObject* pSender, TouchEventType type);
    
  
public:
    E_updateTag  mNowUpdateTag;    //0:qianghua,  1:jinjie
    Button*  mBtnUpdate[E_updateBtnMax];
    Layout*  mPanel_update[E_updateBtnMax];
    SGCCLabelTTF  *mLabelAtt[E_attributeMax],  *mLabelAttNew[E_attributeMax];    //攻防血速
    SGCCLabelTTF  *mLabelName ;
    SGCCLabelTTF  *mLabelLevel,  *mLabelLevelNew ;   //星级和等级公用一个label
    SGCCLabelTTF  *mLabelSoulNum;
    SGCCLabelTTF  *mLabelSoulBar;   //强化进度条上的数字  5/10
    SGCCLabelTTF  *mLabelFragments;   //进阶进度条上的数字
    LoadingBar*   mBarSoul;
    LoadingBar*   mBarFragments;    
    
    
    
    
    
public:
    void btnUpdate(CCObject* pSender, TouchEventType type);    //强化和进阶按钮
};

#endif /* defined(__GameSanGuo__SGMainScene__) */
