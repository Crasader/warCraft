//
//  SGOfficerCard.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-7.
//
//  Updated by MM on 2013-11-09
//  :增加武将缘分相关属性
//
//

#include "SGOfficerCard.h"
#include "SGFateManager.h"
#include "SGPiecesCard.h"
#include "SGPiecesDataModel.h"
#include "SGStaticDataManager.h"
#include <algorithm>


SGOfficerCard::SGOfficerCard()
://g_positionId(0)
//,g_isLord(0),
//g_isexped(0),
g_advance(NULL)
,equips(NULL),
g_officerSkillId(0),
g_lordSkillId(0),
g_times(0),
g_skillExp(0),
g_lordExp(0),
g_fateList(NULL),
m_speedFactor(0),
m_advancestuffnum(0),
m_adsortnum(0)
, g_gender(OG_SHEMALE)
, m_skillmaxlevel(0)
, m_lordmaxlevel(0)
, m_crashNum(0)
, m_hasSvaedDev(true)
{
    m_realAtk = m_realDef = m_realMorale = m_realSpeed = -1;
}

//获得攻击力。不计算缘分
int SGOfficerCard::getBaseAtk() const
{
    return g_atk;
}

//获得防御力。不计算缘分
int SGOfficerCard::getBaseDef() const
{
    return g_def;
}

//获得血量值。不计算缘分
int SGOfficerCard::getBaseMorale() const
{
    return g_morale;
}

//获得速度值。不计算缘分
float SGOfficerCard::getBaseSpeed() const
{
    return g_speed;
}

//获得攻击力
int SGOfficerCard::getAtk() const
{
    if(getRealAtk()==-1)
        return getBaseAtk();
    else
        return getRealAtk();
    /*
    int addAtk = 0;
    SGFateManager* sgfm = SGFateManager::sharedManager();
    if (sgfm)
    {
        addAtk = sgfm->getAddedProperty(this, FAT_ATK);
    }
    
    return g_atk + addAtk;
     */
}

//获得防御力
int SGOfficerCard::getDef() const
{
    if(-1==getRealDef())
        return getBaseDef();
    else
        return getRealDef();
    /*
    int addDef = 0;
    SGFateManager* sgfm = SGFateManager::sharedManager();
    if (sgfm)
    {
        addDef = sgfm->getAddedProperty(this, FAT_DEF);
    }
    return g_def + addDef;
     */
}

//获得血量值
int SGOfficerCard::getMorale() const
{
    if(-1 == getRealMorale())
        return getBaseMorale();
    else
        return getRealMorale();
    /*
    int addMorale = 1;
    SGFateManager* sgfm = SGFateManager::sharedManager();
    if (sgfm)
    {
        addMorale = sgfm->getAddedProperty(this, FAT_MOR);
    }
    return g_morale + addMorale;
     */
}

//获得速度值
float SGOfficerCard::getSpeed() const
{
    if(-1 == getRealSpeed())
        return getBaseSpeed();
    else
        return getRealSpeed();
    /*
    int addSpeed = 1;
    SGFateManager* sgfm = SGFateManager::sharedManager();
    if (sgfm)
    {
        addSpeed = sgfm->getAddedProperty(this, FAT_SPD);
    }
    return g_speed + addSpeed;
     */
}

SGOfficerCard::~SGOfficerCard()
{
    CC_SAFE_RELEASE(equips);
    CC_SAFE_RELEASE(g_advance);
    CCLOG("~SGOfficerCard");
}
void SGOfficerCard::addEquip(SGEquipCard *equipCard)
{
    removeEquip(equipCard);
    if (!equips) {
        equips = CCArray::create();
        equips->retain();
    }
    equips->addObject(equipCard);
}
void SGOfficerCard::removeEquip(SGEquipCard *equipCard)
{
    if (equips) {
        equips->removeObject(equipCard);
    }
}
CCArray *SGOfficerCard::getOfficerEquips()
{
    return equips;
}
void SGOfficerCard::addReqitemId(int itemId)
{
    if (!g_advance) {
        g_advance = CCArray::create();
        g_advance->retain();
    }
    g_advance->addObject(CCString::createWithFormat("%d",itemId));
}
CCArray *SGOfficerCard::getRequitemIds()
{
    return g_advance;
}

bool SGOfficerCard::selfCanAdvance()
{
    int pieceHasNum =0;
    SGPiecesDataModel * model = SGStaticDataManager::shareStatic()->getPiecesDictByOfficerProtoId(getProtoId());
    if(model)
    {
        int itemId = model->getPiecesId();
        CCArray * array = SGPlayerInfo::sharePlayerInfo()->getPiecesCards();
        SGPiecesCard * temp;
        for(int i=0 ; i<array->count() ; ++i)
        {
            temp = (SGPiecesCard*)array->objectAtIndex(i);
            if(itemId == temp->getItemId())
            {
                pieceHasNum = temp->getPiecesCurNum();
                break;
            }
        }
    }
    int needNum =0;
    needNum = SGStaticDataManager::shareStatic()->getAdvOfficerNeedPieceNum(getCurrStar(), getAdNum() );
    
    return pieceHasNum>=needNum && 0!=getMaxStar();
}

bool SGOfficerCard::selfShowStrengthNotice()
{
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    if(player->getPlayerLevel() >= player->getShowNoticeLimitLev2())
        return false;
    if( (player->getPlayerLevel()- getCurrLevel()) < player->getOfficerStrengLevBigger())
        return false;
    CCArray *array_pro = SGPlayerInfo::sharePlayerInfo()->getPropCards();
    CCObject *obj = NULL;
    int nPropsNum = 0;
    CCARRAY_FOREACH(array_pro, obj)
    {
        SGPropsCard *card = (SGPropsCard *)obj;
        if (card  && card->getType() == 1)
        {
            nPropsNum++;
        }
    }

    
    return nPropsNum >= SGPlayerInfo::sharePlayerInfo()->getExpCardNumCanShowNotice() ;
}

bool SGOfficerCard::canShowNotice()
{
    bool ret  = false;
    //可转生
    ret = selfCanAdvance();
    //可强化
    if(selfShowStrengthNotice())
        ret = true;
    //可培养
    if(selfCanDeveloping())
        ret = true;
    if(ret)
        return ret;
    
    CCArray * allEquips = CCArray::create();
    allEquips->retain();
    CCArray * allWearEquips = CCArray::create();
    allWearEquips->retain();
    
    //装备可转生
    for(int i=9 ; i<=12 ; ++i)
    {
        if(hisEquipCanAdv(i) || hisEquipCanStreng(i))
        {
            ret = true;
            break;
        }
    }
//    CCArray * equipsArray = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
//    for(int i=0; i<equipsArray->count(); ++i)
//    {
//        SGEquipCard * card = (SGEquipCard *)equipsArray->objectAtIndex(i);
//        if(card->selfCanAdv())
//        {
//            ret = true;
//            break;
//        }
//    }

    
    //武将可穿戴装备
    CCArray * equipCards = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
    if(equipCards)
        allEquips->addObjectsFromArray(equipCards);
    
    CCArray * array = NULL;
    array = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    for(int i= 0; i<array->count() ; ++i)
    {
        SGOfficerCard * temp = (SGOfficerCard*)array->objectAtIndex(i);
        if(temp->getOfficerEquips())
            allWearEquips->addObjectsFromArray(temp->getOfficerEquips());
    }
    
    //去掉已经穿戴装备
    for(int i=0; i<allWearEquips->count();++i)
    {
        allEquips->removeObject(allWearEquips->objectAtIndex(i));
    }


    
    int intArr[] = {9,10,11,12};
    std::vector<int> vec;
    std::vector<int> vec2;
    CCArray * selfWearEquips = getOfficerEquips();
    if(selfWearEquips)
    {
        for(int j =0 ; j < selfWearEquips->count() ; j++)
        {
            // 武器：9            防具：10            饰品：11            坐骑：12
            
            SGEquipmentDataModel * model = SGStaticDataManager::shareStatic()->getEquipById(((SGEquipCard*)selfWearEquips->objectAtIndex(j))->getItemId() );
            vec.push_back(model->getEquipType());
        }

    }
    
    std::sort(vec.begin(), vec.end());
    for(int k = 0 ; k<4 ; k++)
    {
        bool flag = false;
        for(int m = 0; m<vec.size(); m++)
        {
            if(intArr[k]==vec[m])
            {
                flag = true;
                break;
            }
        }
        if(!flag)
            vec2.push_back(intArr[k]);
    }
    
    for(int k =0 ; k<allEquips->count(); ++k)
    {
        SGEquipmentDataModel * model = SGStaticDataManager::shareStatic()->getEquipById( ((SGEquipCard*)allEquips->objectAtIndex(k))->getItemId());
        int type = model->getEquipType();
        for(int m=0; m<vec2.size() ;++m)
        {
            if(type == vec2[m])
            {
                ret = true;
                break;
            }
        }
        if(ret == true)
            break;
    }
    
    allEquips->release();
    allWearEquips->release();
    
    return  ret;
}


bool SGOfficerCard::canWearEquip(int equipType)
{
    bool ret = false;
    
    CCArray * allEquips = CCArray::create();
    allEquips->retain();
    CCArray * allWearEquips = CCArray::create();
    allWearEquips->retain();
    
    CCArray * equipCards = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
    if(equipCards)
        allEquips->addObjectsFromArray(equipCards);
    
    CCArray * array = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    for(int i= 0; i<array->count() ; ++i)
    {
        SGOfficerCard * temp = (SGOfficerCard*)array->objectAtIndex(i);
        if(temp->getOfficerEquips())
            allWearEquips->addObjectsFromArray(temp->getOfficerEquips());
    }
    
    //去掉已经穿戴装备
    for(int i=0; i<allWearEquips->count();++i)
    {
        allEquips->removeObject(allWearEquips->objectAtIndex(i));
    }
    
    for(int i=0 ; i<allEquips->count(); ++i)
    {
        SGEquipCard * temp = (SGEquipCard*)allEquips->objectAtIndex(i);
        SGEquipmentDataModel * model = SGStaticDataManager::shareStatic()->getEquipById(temp->getItemId());
        if(equipType == model->getEquipType())
        {
            ret = true;
            break;
        }
    }

    
    return ret;
}


bool SGOfficerCard::isWearEquip(int equipType)
{
    bool ret=false;
    CCArray * arr = getOfficerEquips();
    if(arr)
    {
        for(int i=0; i<arr->count(); ++i)
        {
            SGEquipCard * temp = (SGEquipCard *)arr->objectAtIndex(i);
            SGEquipmentDataModel * model = SGStaticDataManager::shareStatic()->getEquipById(temp->getItemId());
            if(model->getEquipType() == equipType)
            {
                ret = true;
                break;
            }
        }

    }
    return ret;
}


bool SGOfficerCard::hisEquipCanAdv(int equipType)
{
    bool ret = false;
    CCArray * array = getOfficerEquips();
    if(array)
    {
        for(int i=0 ; i < array->count() ; ++i)
        {
            SGEquipCard * temp = (SGEquipCard*)array->objectAtIndex(i);
            SGEquipmentDataModel * model = SGStaticDataManager::shareStatic()->getEquipById(temp->getItemId());
            if(equipType == model->getEquipType())
            {
                if(temp->selfCanAdv())
                {
                    ret = true;
                    break;
                }
            }
        }
    }
    
    
    return ret;
}

bool SGOfficerCard::hisEquipCanStreng(int equipType)
{
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    if(player->getPlayerLevel() >= player->getShowNoticeLimitLev2())
        return false;
    bool ret = false;
    CCArray * array = getOfficerEquips();
    if(array)
    {
        for(int i=0 ; i < array->count() ; ++i)
        {
            SGEquipCard * temp = (SGEquipCard*)array->objectAtIndex(i);
            SGEquipmentDataModel * model = SGStaticDataManager::shareStatic()->getEquipById(temp->getItemId());
            if(equipType == model->getEquipType())
            {
               // CCLog("%d , %d , %d" ,temp->selfShowStrengNotice(), temp->getCurrLevel() < player->getPlayerLevel() , (player->getPlayerLevel() - getCurrLevel())>= player->getEquipStrengLevBigger());
                if( temp->selfShowStrengNotice() && temp->getCurrLevel() < player->getPlayerLevel()  )
                {
                    ret = true;
                    break;
                }
            }
        }
    }
    
    
    return ret;

}

bool SGOfficerCard::selfCanDeveloping()
{
    bool ret = false;
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    //CCLog("point =%d, limitPoint=%d , hasNum = %d , needNum = %d , lev = %d ,needLev = %d" , getDevPoint(), player->getDevPointLimitCnt() , player->ownItemCount(BIT_CONSUMABLE, 352003) , player->getDevPelletLimitCnt() , this->getCurrLevel() , player->getDevOfficerLimitLev());
    
    ret = getDevPoint() >= player->getDevPointLimitCnt() && player->ownItemCount(BIT_CONSUMABLE, 352003) >= player->getDevPelletLimitCnt() && this->getCurrLevel() >= player->getDevOfficerLimitLev();
    if(!m_hasSvaedDev)
        ret = true;
    return ret;
}







