//
//  SGCountryLayer.h
//  GameSanGuo
// 选择国家界面
//  Created by gengshengcai on 13-1-10.
//
//

#ifndef __GameSanGuo__SGCountryLayer__
#define __GameSanGuo__SGCountryLayer__

#include "SGBaseLayer.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class SGCountryLayer : public SGBaseLayer
{
private:
    void selectCountry(int countryid);
    void constructView();

    void confirmHandler();
    void weiHandler();
    void shuHandler();
    void wuHandler();
    //切入效果
    void showeffect(); //效果总入口
    void wei();  //魏国动作
    void shu(); //蜀国动作
    void wu();  //吴国动作
    void qun(); //群英动作
    void lowerlabelaction();//下方动作
    //上方动作们
    void uplabelaction1();
    void uplabelaction2();
    void uplabelaction3();
    
    
    
    void staraction();//星星动作
    void setmiss(CCObject*obj);//设置星星不可见
    void  bgaction();//黑色背景动作
    
    
    
    
    
    
public:
    SGCountryLayer();
    ~SGCountryLayer();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
    static SGCountryLayer *create();
    
    
    
    
private:
    SGButton *buttonWei; //魏
    SGButton *buttonShu; //蜀
    SGButton *buttonWu;  //吴
    CCSprite *spriteOther; //群英
    CCSize s;
    CCSprite*hoshi;
    float mmmm;
    
    
};
#endif /* defined(__GameSanGuo__SGCountryLayer__) */
