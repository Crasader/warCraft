//
//  SGPopWord.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-4.
//
//

#ifndef __GameSanGuo__SGPopWord__
#define __GameSanGuo__SGPopWord__

#include <iostream>
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class SGPopWord : public CCLayerColor ,public CCStandardTouchDelegate {
private:
    CCArray *_stringContent;    //数组，存放所有的文字内容
    CCLabelTTF *labelContent;   //标签
    int _wordIndex;             //文字索引
    CCSize _viewSize;           //Layer大小
    int _contentCount;          //字符总数
    float _timerDuration;       //单字弹出间隔时间
    int _maxRowWordCount;       //单行最大字数
    float _clockTick;           //计时器
    CCSprite *smallAngle;       //右下角小三角
    
    void constructView();
    void timerStep(CCTime dt);
    int getFirstRowWordCount();
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);

    
public:

    SGPopWord();
    ~SGPopWord();

    static SGPopWord *create(CCArray *arrayContent , CCSize viewSize , int contentCount , float timerDuration);
    
    static CCArray *convertCharToArray(char string[][10] , int length);
    
};




#endif /* defined(__GameSanGuo__SGPopWord__) */
