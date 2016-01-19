//
//  SGSortBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-24.
//
//

#ifndef __GameSanGuo__SGSortBox__
#define __GameSanGuo__SGSortBox__

#include "SGBaseBox.h"
#include "SGBaseMilitaryCard.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

class SGSortLayerDelegate{
    
public:
    virtual void sortLayerCallBack(CCArray *cardArray) = 0;
};
//
////排序类型
//enum sortType
//{
//    
//    sort_star = 1,//星级高
//    sort_level = 2,//等级高
//    sort_ap ,//攻击高
//    sort_def,//防御高
//    sort_speed,//速度高
//    sort_morale,//血量高
//    sort_round,//蓄力快
//    sort_newgot//新获得
//};

class SGSortBox : public SGBaseBox
{
private:
    CCSprite *down;
    CCSprite *up;
    CCArray *_arrayCard;
    int isfromstreng;
    CCScrollView *boxscroll;    
    int cardType;
    SGBoxDelegate *_delegate;
    SGSortLayerDelegate *_delegateCallBack;
    CCLayer *boxscrollViewContainer;
    int type;
protected:
private:
    void updateTime();
    void confirmHandler();    
    void buttonSortCancel();
    void buttonClickSort(CCNode *sender);
    
public:
    SGSortBox();
    ~SGSortBox();
    static SGSortBox *sortCards(CCArray *array, int type, int isstreng=0 ,bool needSave = true);
    static SGSortBox *updateBtn(SGButton *btn,int entertype);
    static SGSortBox *create(SGBoxDelegate *bdg,SGSortLayerDelegate *callBack ,CCArray *arrayCard,int isstreng = 0,int entertype = 0);
    
    bool init(SGBoxDelegate *bdg,CCArray *arrayCard,int isstreng = 0);
    
//    virtual void scrollViewDidScroll(CCScrollView* view);
    
    // 获取主将所在位置
    static int getIndexByType(int ssid, int sType, int iType);
    
    virtual void boxClose();
};

#endif /* defined(__GameSanGuo__SGSortBox__) */
