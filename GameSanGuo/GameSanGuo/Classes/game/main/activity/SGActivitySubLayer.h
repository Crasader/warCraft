//
//  SGActivitySubLayer.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-19.
//
//

#ifndef __GameSanGuo__SGActivitySubLayer__
#define __GameSanGuo__SGActivitySubLayer__

#include <iostream>
#include "cocos-ext.h"
#include "SGButton.h"
#include "SGOfficerCard.h"
#include "SGScrollViewSubview.h"
#include "SGBaseShow.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class SGActivitySubItem;

class SGActivitySubLayer :public SGScrollViewSubview{
    
private:
    CCSize _viewSize;
//    SGOfficerCard *_officerCard;
    int enterType;
    void constructView();
    std::string str;
//    int index ;
    void buttonClickOK(SGButton *sender);
    CCPoint touchBeginLocation;
    bool isMove;
    SGActivitySubItem *item;
    
    int checkCountState;//0不是开服活动，1进行中，2已经结束
     CCSprite* countStateImage;
public:
    CCMenu *me_nu;
    SGActivitySubLayer();
    ~SGActivitySubLayer();
//	void selectEmbattle();
    bool iscantouch ;
    virtual void selectSubview();
    void setiscantouch(bool iscan);
    static SGActivitySubLayer *create(const std::string&name,CCPoint position, CCSize viewSize,int type,SGActivitySubItem* item,int checkTimeState);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    void updateCheckState(int state);
};

class SGActivitySubItem
{
public:
    SGActivitySubItem(){};
    virtual void BtnTouched(SGActivitySubLayer *layer){};
};



#endif /* defined(__GameSanGuo__SGActivitySubLayer__) */
