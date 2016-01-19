//
//  SGEquipCard.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#include "SGEquipCard.h"
#include "SGStaticDataManager.h"
#include "Utils.h"
SGEquipCard::SGEquipCard()
:g_speed(0),
needCards(NULL)
, g_hp(0)
, g_governing(0)
, g_SetNum(0)
, g_OfficerCardId(0)
,m_materialDependencyId(-1)
,m_fateDesc(NULL)
{
}
SGEquipCard::~SGEquipCard()
{
    CC_SAFE_RELEASE(needCards);
    CC_SAFE_RELEASE(m_fateDesc);
}
void SGEquipCard::addReqitemId(int itemId)
{
    if (!needCards) {
        needCards = CCArray::create();
        needCards->retain();
    }
    needCards->addObject(CCString::createWithFormat("%d",itemId));
}
CCArray *SGEquipCard::getRequitemIds()
{
    return needCards;
}


bool SGEquipCard::selfCanAdv()
{
    bool ret = true;
    
    bool flag = true;

    //读取将要转生这个装备卡牌
    SGEquipmentDataModel *model = SGStaticDataManager::shareStatic()->getEquipById(getItemId());
    
    //装备的需要材料组id
    int materialDependId = model->getMaterialDependencyId();
    
    
    //根据装备datamodel里的MaterialDependency获取依赖材料的数据
    //=-1标示已经到最顶级
    if(materialDependId ==-1)
        return false;
    SGMaterialDependencyDataModel *depend = SGStaticDataManager::shareStatic()->getMaterialDependencyById(materialDependId);
    CCString *condition = depend->getMaterialDependCondition();
    //每个条件 materialId:materialNum
    CCArray *conditionGroup = split(condition->getCString(), ";");
    
    
    //切分每个分组条件，将内部的条件再次进行切分
    for (int i = 0; i < conditionGroup->count(); i++)
    {
        CCString *temp = static_cast<CCString *>(conditionGroup->objectAtIndex(i));
        //冒号切分
        CCArray *tempConditon = split(temp->getCString(), ",");
        
        //需要的材料id
        int materialId = static_cast<CCString *>(tempConditon->objectAtIndex(0))->intValue();
        //需要的材料的数量
        int needMaterialNum = static_cast<CCString *>(tempConditon->objectAtIndex(1))->intValue();
        
        SGMaterialCard *materialCard = SGPlayerInfo::sharePlayerInfo()->getMaterialCardByItemId(materialId);
        if(materialCard)
        {
            if(materialCard->getMaterialCurrNum() < needMaterialNum)
            {
                ret = false;
                break;
            }
        }
        else{
            ret = false;
            break;
        }
     }
    
    return ret;
    
}

bool SGEquipCard::selfShowStrengNotice()
{
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    return player->getPlayerLevel() < player->getShowNoticeLimitLev2()  && player->getPlayerCoins() >= player->getCoinNumCanShowNotice() && (player->getPlayerLevel() - getCurrLevel())>= player->getEquipStrengLevBigger() ;
}


