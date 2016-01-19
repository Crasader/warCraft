//
//  SGMonthlySignItem.h
//  GameSanGuo
//
//  Created by Fygo Woo on 12/16/14.
//
//

#ifndef __GameSanGuo__SGMonthlySignItem__
#define __GameSanGuo__SGMonthlySignItem__

#include "SNSTableViewCellItem.h"
#include "RewardModule.pb.h"
#include "ItemModule.pb.h"

class CCSpriterX;

class SGMonthlySignItem : public SNSTableViewCellItem
{
//function
public:
    //ctor
    SGMonthlySignItem();
    
    //dtor
    ~SGMonthlySignItem();
    
    //static create
    static SGMonthlySignItem* create(main::SigninCell* sc);
    
    //加载视图
    void initView();
    
    //滑动时更新来源
    void updateItem(main::SigninCell* sc);
    
private:
    //点击。
    void onClicked(CCObject* sender);
    
    //校准基础底板。整合create与update的相同过程。
    void regulateFrame();
    
    //校准其他元素。整合create与update的相同过程。
    void regulateElements();
    
//variable
private:
    //数据
    main::SigninCell* scData_;
    
    //底板
    CCSprite* frame_;
    
    //角标背板
    CCSprite* markTag_;
    
    //角标文本
    SGCCLabelTTF* markLabel_;
    
    //蒙版（已领取）
    CCLayerColor* mask_;
    
    //对勾
    CCSprite* tick_;
    
    //按钮
    SGButton* button_;
    
    //NB特效
    CCSpriterX* effect_;
    
    //按钮容器
    CCLayer* container_;
};

#endif /* defined(__GameSanGuo__SGMonthlySignItem__) */
