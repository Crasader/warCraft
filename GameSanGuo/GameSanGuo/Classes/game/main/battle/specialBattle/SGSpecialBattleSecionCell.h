//
//  SGSpecialBattleSecionCell.h
//  GameSanGuo
//
//  Created by bugcode on 14-12-12.
//
//

#ifndef __GameSanGuo__SGSpecialBattleSecionCell__
#define __GameSanGuo__SGSpecialBattleSecionCell__

#include <iostream>
#include "SGStoryBaseCell.h"
#include "SGSpecialBattleLayer.h"


class SGSpecialBattleSecionCell : public SGStoryBaseCell
{
public:
    SGSpecialBattleSecionCell();
    ~SGSpecialBattleSecionCell();

    static SGSpecialBattleSecionCell *create(SpecialBattleSecData *cellData);
    void initView();
    
    void updateSpCell(SpecialBattleSecData *cellData);
    void dropListPreviewWindow(CCObject *obj);
    void startBattle(CCObject *obj);
    
private:
    //节背景需要武将的iconid
    int bgId;
    //每一节的名称
    SGCCLabelTTF *sectionName;
    //开启的条件显示
    SGCCLabelTTF *openCondition;
    //节数据
    SpecialBattleSecData *cellData;
    //SGButton *battleBtn
    SGButton *battleBtn;
    //大图背景
    SGButton *bgButton;
    CCSprite *greyMask;
};


#endif /* defined(__GameSanGuo__SGSpecialBattleSecionCell__) */
