//
//  SGPropInfoLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-19.
//
//

#ifndef __GameSanGuo__SGPropInfoLayer__
#define __GameSanGuo__SGPropInfoLayer__

#include "SGBaseStuffLayer.h"
#include "SGPropsCard.h"
class SGPropInfoLayer : public SGBaseStuffLayer
{
 
public:
    static SGPropInfoLayer *create(SGPropsCard *card,int type,int currentPage=1000);
    SGPropInfoLayer();
    ~SGPropInfoLayer();
private:
    void initView();
    void backHandler();
    void protectHandler();
    void disprotectHandler();
    void sellHandler();
    void getHandler();
    void disprotectListener(CCObject *obj);
    virtual void showBoxCall(CCObject *obj);
    void showLayerByTag(LayerTag layerTag);
    void propprotectListener(CCObject *obj);
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    
    virtual void onExit();

private:
    SGButton *sellBtn;
    int enterType;
    SGPropsCard *_card;    
    SGButton *protectBtn;
    float btnwidth;
    float btnwid;
    CCSprite *lock;
    int _currentPage;
    
};


#endif /* defined(__GameSanGuo__SGPropInfoLayer__) */
