//
//  SGChatWithGM.h
//  GameSanGuo
//
//  Created by kanata on 14-1-8.
//
//

#ifndef __GameSanGuo__SGChatWithGM__
#define __GameSanGuo__SGChatWithGM__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "cocos-ext.h"
#include "SGChatMessageCenter.h"
#include "SGCCLabelTTF.h"
#include "SGMenu.h"
#include  "SGComplainCell.h"

USING_NS_CC_EXT;

class SGChatWithGM : public SGBaseLayer{
    
    
private:
    CCArray*_array;
    
    CCArray*temptemp;
    
    SGButton *buttonSend;
    int checkid;
    //滑动控件
    CCScrollView *scrollView;
    CCLayer *scrollViewContainer;
    SGCCLabelTTF*fontlabel;
    
    
  
    
    
private:
    void constructView();
    //按钮点击
    void buttonClickGoback();
    void buttonClickSend();
    
    /* 计算一个字符串占的区域大小 */
    CCSize getSizeByCharBuffer(const char *charBuffer);
    //获取一个聊天气泡
    CCSprite* getBubbleByChatMessage(QADetail *detail);
    void constructScrollView();
    void setIsCanTouch(bool isTouch);
    
public:
    
    SGChatWithGM();
    ~SGChatWithGM();
    static SGChatWithGM* create(CCArray *array,int idid);
     void send(CCArray*arr);
    
     CC_SYNTHESIZE(QADetail*, m_qatemp, qatemp);

};


#endif /* defined(__GameSanGuo__SGChatWithGM__) */
