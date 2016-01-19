//
//  SGTaskLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-18.
//
//

#ifndef __GameSanGuo__SGTaskLayer__
#define __GameSanGuo__SGTaskLayer__

#include "SGBaseLayer.h"
#include "cocos-ext.h"
#include "SGMenu.h"

USING_NS_CC_EXT;
class SGMainTaskLayer;
class SGTaskLayer : public SGBaseLayer
{
private:
    CCSize s;
    int tag;
    int height ;
    CCScrollView *scroll;
    CCLayer *scrollViewContainer;
//    CCScrollView *boxscroll;
//    CCLayer *boxscrollViewContainer;
    CCArray *pxData;
    ////
    CCArray*arr;
    SGCCLabelTTF *font1;
    CCSprite *dayNotice;
    int canGetRewardNum;
public:
    SGMenu *taskMenu;
    
private:
    void initView(CCArray *array);
    void backHandler();
    void showGetSalary();
    void showWine();
    void showHope();
    void confirmHandler(CCNode *sender);
    void salaryEnterListener(CCObject *obj);
    void recoverEnterListener(CCObject *obj);
    void godEnterListener(CCObject *obj);
    void linghandle(CCObject *obj);
    void statusEnterListener(CCObject*obj);
    void showlingstatus();
    void hide(CCObject*obj);
    void show(CCObject*obj);
    
    virtual void showBoxCall(CCObject *obj);

    
public:
    SGTaskLayer();
    ~SGTaskLayer();
    static SGTaskLayer *create(CCArray *array);
    
    void showMenuItemMultiMove(CCMenu *menu);
    void showTaskBlank();
    void showFirstTask();

     //主线任务
private:
    SGMainTaskLayer *mainTaskLayer;
    bool isShowMainTaskLayer;
public:
    void createMainTaskLayer();
    void showMainTaskLayer();
    void showDayTaskLayer();
    void setMainTaskVisible(bool isVisible);
    
};


#endif /* defined(__GameSanGuo__SGTaskLayer__) */
