//
//  SGBattleHelpLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-20.
//
//

#ifndef __GameSanGuo__SGBattleHelpLayer__
#define __GameSanGuo__SGBattleHelpLayer__

#include "SGBaseBox.h"

/**
 ** 战斗帮助界面
 **/

class SGBattleHelpBox : public SGBaseBox
{
public:
    SGBattleHelpBox(void);
    ~SGBattleHelpBox(void);
    
    static SGBattleHelpBox* create(SGBoxDelegate *delegate);
    
private:
    void initView();
    void backHandler(); // 返回事件
    void resetMainTouch(bool touch); // 重置地层Layer的响应
    
    
private:
    SGBoxDelegate *t_delegate;
};

#endif /* defined(__GameSanGuo__SGBattleHelpLayer__) */
