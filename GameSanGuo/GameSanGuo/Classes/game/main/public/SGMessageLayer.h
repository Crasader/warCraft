//
//  SGMessageLayer.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-20.
//
//

/**************************************
消息类，用于弹出黑色底框的消息
 
调用
SGMainManager::showMessage("消息")
就可以显示消息
 
**************************************/


#ifndef __GameSanGuo__SGMessageLayer__
#define __GameSanGuo__SGMessageLayer__

#include <iostream>
#include "cocos-ext.h"
#include "SGCCLabelTTF.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SGMessageLayer :public CCLayer {
    
private:
    CCSprite *spriteUpLine;
    CCSprite *spriteBottomLine;
    CCSprite *spriteBackground;
    SGCCLabelTTF *labelContent;
    CCSprite *guang;
    CCSprite *guang1;
    void constructView();
    void callFunction();
    void showlab();
public:
    
    SGMessageLayer();
    ~SGMessageLayer();
    
    static SGMessageLayer* create();
    
    void showMessage(const char *content);
    
    
};

#endif /* defined(__GameSanGuo__SGMessageLayer__) */
