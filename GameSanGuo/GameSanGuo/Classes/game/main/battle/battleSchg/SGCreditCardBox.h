//
//  SGCreditCardBox.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-27.
//
//

#ifndef __GameSanGuo__SGCreditCardBox__
#define __GameSanGuo__SGCreditCardBox__

#include "SGBaseBox.h"

class SGCreditCostBox : public SGBaseBox
{
public:
    SGCreditCostBox(void);
    ~SGCreditCostBox(void);
    
    static SGCreditCostBox* create(SGBoxDelegate *delegate, int sid);
    void initWithInfo(int score, int cost);
    
private:
    void sureAction();
    void cancelAction();
    void resetMainTouch(bool touch); // 重置地层Layer的响应
    
private:
    int sendId;
    SGBoxDelegate *t_delegate;
};


class SGCantMessageBox : public CCLayer
{    
private:
    CCLayerColor *spriteBackground;
    SGCCLabelTTF *labelContent;
public:
    SGCantMessageBox();
    ~SGCantMessageBox();
    
    virtual bool init();
    static SGCantMessageBox* create();
    
    void showMessage(const char *content);
    void dismiss();
};

#endif /* defined(__GameSanGuo__SGCreditCardBox__) */
