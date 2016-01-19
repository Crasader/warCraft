//
//  SGSoulCardInfoLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-10.
//
//

#ifndef __GameSanGuo__SGSoulCardInfoLayer__
#define __GameSanGuo__SGSoulCardInfoLayer__

#include "SGBaseStuffLayer.h"
#include "SGPropsCard.h"
class SGSoulCardInfoLayer : public SGBaseStuffLayer
{
private:
    SGPropsCard *_card;
    SGButton *protectBtn;
    CCSprite *hd;
    float btnwidth;
    float btnwid;
    CCSprite *lock;
    int enterType;
public:
    static SGSoulCardInfoLayer *create(SGPropsCard *card,int enterType);
    SGSoulCardInfoLayer();
    ~SGSoulCardInfoLayer();
    virtual void showBoxCall(CCObject *obj);
private:
    void initView();
    void backHandler();
    void protectHandler();
    void disProtectHandler();
    void sellHandler();
    void disprotectListener(CCObject *obj);
    void propprotectListener(CCObject *obj);

};
#endif /* defined(__GameSanGuo__SGSoulCardInfoLayer__) */
