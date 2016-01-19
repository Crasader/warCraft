//
//  SGScrollLabel.h
//  TestScrollView
//
//  Created by 江南岸 on 13-2-16.
//
//

#ifndef __TestScrollView__SGScrollLabel__
#define __TestScrollView__SGScrollLabel__

#include <iostream>
#include "cocos-ext.h"
using namespace cocos2d;

class SGScrollLabel : public CCLayerColor  {
private:
    
    CCArray *_arrayLabel;
    CCArray *_arrayString;
    CCSize _viewSize;
    
    void constructView();
    int getFirstRowWordCount();

public:
    
    ~SGScrollLabel();
    SGScrollLabel();
    static SGScrollLabel *create(CCArray *arrayString , CCSize viewSize);
    void callFunction();


};

#endif /* defined(__TestScrollView__SGScrollLabel__) */
