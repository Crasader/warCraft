//
//  SGCountrysInfo.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-12.
//
//

#ifndef __GameSanGuo__SGCountrysInfo__
#define __GameSanGuo__SGCountrysInfo__

#include "SGBaseLayer.h"
class SGCountrysInfo :public SGBaseLayer
{
private:
    SGButton *btn_wei;
    SGButton *btn_shu;
    SGButton *btn_wu;
    
    SGCCLabelTTF* label_1;
    SGCCLabelTTF*label_2 ;
    SGCCLabelTTF*label_3 ;
    
    SGCCLabelTTF *countrytitle;
    int entertype;
    int _countrytype;

private:
    void backHandler();
    void showInfo(CCNode * psender);
    void initView();
public:
    SGCountrysInfo();
    ~SGCountrysInfo();
    
    static SGCountrysInfo *create(int type,int countrytype=-10);
};


#endif /* defined(__GameSanGuo__SGCountrysInfo__) */
