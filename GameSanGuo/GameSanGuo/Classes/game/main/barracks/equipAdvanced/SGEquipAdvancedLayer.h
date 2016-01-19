//
//  SGEquipAdvancedLayer.h
//  GameSanGuo
//
//  Created by bugcode on 14-10-22.
//
//

#ifndef __GameSanGuo__SGEquipAdvancedLayer__
#define __GameSanGuo__SGEquipAdvancedLayer__

#include "SGBaseStuffLayer.h"
#include "SGBaseMilitaryCard.h"
#include "cocos2d.h"

USING_NS_CC;

class SGEquipAdvancedLayer : public SGBaseLayer
{
public:
    SGEquipAdvancedLayer();
    ~SGEquipAdvancedLayer();
    
    static SGEquipAdvancedLayer *create(SGEquipCard *willAdvancedCard, int enterType);
    
    void initView();
    void backHandler(CCObject *obj);
    void showWillAdvanceCardInfo(cocos2d::CCPoint pos, SGBaseMilitaryCard *card);
    void showAfterAdvanceCardInfo(CCPoint pos,SGBaseMilitaryCard *card);
    //下方的材料横条 @param, 数组里存放转生所需要的材料的datamodel
    CCLayerColor *getMaterialLayer(CCArray *materialModelArray, int *needNumArray);
    //点击开始转生
    void startAdvaced(CCObject *obj);
    //转生回调
    void advancedListener(CCObject *sender);
    CCArray *getMaterialDependency(int *needNum);
    void pressHeadIcon(CCObject *obj);
private:
    //待转生武将
    SGEquipCard *willAdvancedEquipCard;
    //转生后的武将
    SGEquipCard *afterAdvacedEquipCard;
    //转生按钮
    SGButton *quipAdvancedBtn;
    //
    CCDictionary *willAdvancedDict;
    //标记是否能转生
    bool canAdvancedEquip;
    
    //材料来源
    void sourceCallback(CCObject * obj);
    
    //0=装备页面自身，1=武将页面的已穿戴装备，2=武将页面->装备选择界面->任意装备
    int _enterType;
};


#endif /* defined(__GameSanGuo__SGEquipAdvancedLayer__) */
