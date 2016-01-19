//
//  SGEquipCard.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#ifndef __GameSanGuo__SGEquipCard__
#define __GameSanGuo__SGEquipCard__

#include "SGBaseMilitaryCard.h"

class SGEquipCard : public SGBaseMilitaryCard
{
private:
    CCArray *needCards;
public:
    virtual const char * getClassName(){return "SGEquipCard";};
//    //原型ID
//    CC_SYNTHESIZE(int, m_ProtoId, ProtoId);
    //血量
    CC_SYNTHESIZE(int, g_hp, HP);
    //    速度
    CC_SYNTHESIZE(float, g_speed,Speed);
    //    统御力
    CC_SYNTHESIZE(int, g_governing,Governing);
    //套装号
    CC_SYNTHESIZE(int, g_SetNum, SetNum);
    // 所属武将id
    CC_SYNTHESIZE(int, g_OfficerCardId, OfficerCardId);
    
    //转生需要材料组的id
    CC_SYNTHESIZE(int, m_materialDependencyId, MaterialDependencyId);
    
    //与装备相关的武将缘分描述
    CC_SYNTHESIZE_RETAIN(CCString *, m_fateDesc, FateDesc);
    //该装备是否可转生。
    bool selfCanAdv();
    //是否显示强化提示
    bool selfShowStrengNotice();
    
    SGEquipCard();
    ~SGEquipCard();
    static SGEquipCard *proto2card();
    void addReqitemId(int itemId);
    CCArray* getRequitemIds();
};
#endif /* defined(__GameSanGuo__SGEquipCard__) */
