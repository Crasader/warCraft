//
//  SGGetSalaryLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-16.
//
//

#ifndef __GameSanGuo__SGGetSalaryLayer__
#define __GameSanGuo__SGGetSalaryLayer__

#include "SGBaseLayer.h"
#include "SGCCLabelTTF.h"
class SGGetSalaryLayer : public SGBaseLayer
{
private:
    SGCCLabelTTF *lab_iscanget;
    SGCCLabelTTF *lab_iscanget1;
    SGButton *btn;
    SGButton *btn1;
    int m_type;
    int nowTime;
    int nowTime1;
    bool isShow;//显示上面领薪
    bool isShowPvP;//显示下面的领薪信息
    
private:
    void initView(int a,int b,int c,int d,int e,int f,int g );
    void backHandler();
    void getCoins();
    void getCoins1();
    void updateTime();
    void updateTime1();
    void getCoinListener(cocos2d::CCObject *obj);
    void salaryEnterListener(cocos2d::CCObject *obj);
public:
    SGGetSalaryLayer();
    ~SGGetSalaryLayer();
    static SGGetSalaryLayer *create(int a,int b,int c,int d,int e,int f,int g);
    
};
#endif /* defined(__GameSanGuo__SGGetSalaryLayer__) */
