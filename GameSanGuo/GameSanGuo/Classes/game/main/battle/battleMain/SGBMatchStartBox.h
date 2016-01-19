//
//  SGBMatchStartBox.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-9.
//
//

#ifndef __GameSanGuo__SGBMatchStartBox__
#define __GameSanGuo__SGBMatchStartBox__

/**
 * 速配对手界面
 **/

#include "SGBaseBox.h"

class SGBMatchStartBox : public SGBaseBox
{
public:
    SGBMatchStartBox(void);
    ~SGBMatchStartBox(void);
    
    static SGBMatchStartBox* create(SGBoxDelegate *delegate);
    bool init(SGBoxDelegate *delegate);
    void hideBoxAction();   // 隐藏菜单
    
private:
    void initView();
    void stopMatchAction(); // 停止速配
    void resetMainTouch(bool touch); // 重置地层Layer的响应
    
    void stopMatchRespone(CCObject *sender);
    void showTitleDot();
private:
    SGBoxDelegate *t_delegate;
    SGCCLabelTTF* titleDot1;
    SGCCLabelTTF* titleDot2;
    SGCCLabelTTF* titleDot3;
    int m_showDotCount;
};

#endif /* defined(__GameSanGuo__SGBMatchStartBox__) */
