//
//  SGActivityLayer.h
//  GameSanGuo
//
//  Created by dongboqian on 14-1-7.
//
//

#ifndef __GameSanGuo__SGActivityLayer__
#define __GameSanGuo__SGActivityLayer__

#include "SGBaseTableLayer.h"
#include "SGActivitySubLayer.h"
#include "SGActivityData.h"

#define SCROLLWIDE 275


class SGActiveItemDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};

static inline bool ccc4BEqual(ccColor4B a, ccColor4B b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}
static inline ccColor3B ccc3BFrom4B(ccColor4B a)
{
    return ccc3(a.r, a.g , a.b);
}
class SGActivityLayer : public SGBaseLayer, public  SGActiveItemDelegate, public SNSScrollViewPageDelegate, public SGActivitySubItem
//class SGActivityLayer : public SGBaseLayer
{
public:
    static SGActivityLayer *create(CCArray *array,CCObject * obj);
    SGActivityLayer();
    ~SGActivityLayer();
    void getHandle(SGVipGift* gift);
    void setIsCanTouch(bool isTouch);
    void  testFunction();
    
    //未储值用户在活动页面前往储值时显示第一个lua页面
//    void showFirstLuaLayer();
    
private:
    void initView();
    void initActivityData(CCArray *array);
    int luanum;
    CCObject * m_pActivitydata;
    int offset;
    CCArray *lualayerdic;
    int _totalPage;
    int _nowPage;
    int reallength;
    CCLayer *_layer;
    CCLayer *m_pLayerCurr;//sulayer
    CCSprite *left;
    CCSprite *rgt;
    CCScrollView* scrollview;
    CCScrollView* scrollview2;
//    void touchbutton(SGButton* sender);
    void refreshPage(int type);
    void scrollViewDidScroll();
    void UpdateTime();
    CCArray *_allData;
    CCMenu * _menu;
    void BtnTouched(SGActivitySubLayer* layer);
    //已整合到mainLayer中
//    void storeEnterListener(CCObject *obj);
    void GetGiftsListener(CCObject *obj);
    bool isFileExist(const char* pFileName);
    CCArray *activityData;
    
    SGActivityUpdateData* m_UpdateTimeData;

    //void showboxa(CCObject *obj);
    
    void setLuaLayerData(CCNode *luaLayer,SGActivityData *lua,int index);
    void showFirstChagreItem(CCObject* selectItem);
    void showActivityPageListener(CCObject *obj);
    void setActivityElementListener(CCObject *obj);//设置lua元素属性
    void sendRewardReqListener(CCObject *obj);//领取按钮监听
    CCLayer *m_pCurrPage;//当前页
    int m_nTAG;//当前活动按钮tag
   
protected:

    virtual void initMsg();
    void buyGrowPlan(CCObject *);
    void buyGrowPlanResponse(CCObject *);
    
//    void timerCallback();
};

#endif /* defined(__GameSanGuo__SGActivityLayer__) */
