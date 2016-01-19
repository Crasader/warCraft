//
//  SGBMacthAlertBox.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-9.
//
//

#ifndef __GameSanGuo__SGBMatchAlertBox__
#define __GameSanGuo__SGBMatchAlertBox__

/**
 * 速配成功提示界面
 **/

#include "SGBaseBox.h"

class SGBMatchAlertBox : public SGBaseBox
{
public:
    SGBMatchAlertBox(void);
    ~SGBMatchAlertBox(void);
    
    static SGBMatchAlertBox* create(SGBoxDelegate *delegate);
    bool init(SGBoxDelegate *delegate);
private:
    void initView();
    void macthGoonAction();     // 确定匹配成功
    void macthLaterAction();    // 稍后重新匹配
    void macthStopAction();     // 退出速配
    void matchAction(); // 发送速配请求
    void macthStopRespone(CCObject *sender);    // 接收服务器响应
    void resetMainTouch(bool touch); // 重置地层Layer的响应
    
    void showCounter(); // 显示倒计时
    
private:
    SGBoxDelegate *t_delegate;
    CCLabelTTF *label; //10秒倒计
    int counter;
    bool isLater;
};

#endif /* defined(__GameSanGuo__SGBMatchAlertBox__) */
