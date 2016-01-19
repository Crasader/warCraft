//
//  SGCountryInfoLayer.h
//  GameSanGuo
//  国家简介界面
//  Created by gengshengcai on 13-1-10.
//
//

#ifndef __GameSanGuo__SGCountryInfoLayer__
#define __GameSanGuo__SGCountryInfoLayer__

#include "SGBaseBox.h"
class SGCountryInfoLayer :public SGBaseBox
{
private:
    int countryId;
    SGBoxDelegate *_dele;
private:
    void initView();
    void confirmHandler();
    void cancelHandler();
public:
    SGCountryInfoLayer();
    ~SGCountryInfoLayer();
	void stepListener();
    
    static SGCountryInfoLayer *create(SGBoxDelegate *dele,int countryid_);
};
#endif /* defined(__GameSanGuo__SGCountryInfoLayer__) */
