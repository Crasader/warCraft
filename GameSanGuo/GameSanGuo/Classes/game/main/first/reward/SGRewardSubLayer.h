//
//  SGRewardSubLayer.h
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-29.
//
//

#ifndef __GameSanGuo__SGRewardSubLayer__
#define __GameSanGuo__SGRewardSubLayer__

#include <iostream>
#include "cocos-ext.h"
#include "SGScrollViewSubview.h"
#include "SGBaseLayer.h"
#include "SGTotalLoginRewardLayer.h"
#include "SGStaticDataManager.h"
#include "SGMenu.h"
USING_NS_CC;
USING_NS_CC_EXT;
class SGRewardSubLayer:public SGBaseLayer
{
private:
    CCSize _viewSize;
    void constructView();
    int index;
    int officerID;
    int dayNum;
    
    int currentDay;//当前是第几天
    int isFinish;//是否完成
    int leftday;//剩余天数
    
    SGCCLabelTTF *day1_label;
    SGCCLabelTTF *day2_label;
    SGCCLabelTTF *day3_label;
    SGCCLabelTTF *day4_label;
   
    
    SGMenu *menuNew;
//    LoginRewardObj *obj;
    
    int showType;
    CCArray *allObject;
    
//    virtual void onEnter();
//    virtual void onExit();

public:
    SGRewardSubLayer();
   ~SGRewardSubLayer();
     SGButton *lq_btn;
    void showRewardInfo(CCObject *sender);
    void getRewardHandle();
    virtual void selectSubview();
    static SGRewardSubLayer *create(int leftday,int currentDay,int isFinish,CCArray *allObj,CCPoint position,CCSize viewSize);
    
    
    void showGenerOrEquipInfo(LoginRewardObj *obj);
    
//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};
#endif /* defined(__GameSanGuo__SGRewardSubLayer__) */
