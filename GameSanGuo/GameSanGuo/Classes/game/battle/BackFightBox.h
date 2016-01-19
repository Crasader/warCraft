//
//  BackFightBox.h
//  GameSanGuo
//
//  Created by geng on 13-7-29.
//
//

#ifndef __GameSanGuo__BackFightBox__
#define __GameSanGuo__BackFightBox__
#include "SGBaseBox.h"
#include "SGHTTPDelegate.h"

class BackFightBox : public SGBaseBox
{
private:
//    //这个参数废弃了，改为采用最终决定战斗类型的BattleType。
//    int from;
public:
    //确定
    void btnYesHandler(SGButton *sender);
    
    //取消，对守荆州有二次确认。
    void btnNoHandler(SGButton *sender);
    ~BackFightBox();
public:
    static BackFightBox *create(SGBoxDelegate *target);
    void initView();
    
    //向上的回调，二次确认时也可以触发。
    void callbackUpward(int tag);
};
#endif /* defined(__GameSanGuo__BackFightBox__) */
