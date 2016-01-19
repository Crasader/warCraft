//
//  SGGamePlayerInfo.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-6.
//
//

#include "SGGamePlayerInfo.h"
SGGamePlayerInfo::SGGamePlayerInfo()
{
    roleId = 0;
    name = NULL;
    nickName = NULL;
    ssid = NULL;
    exp = 0;
    level = 0;
    coins = 0;
    silver = 0;
    gold = 0;
    governing = 0;
    country = 0;
    image = 0;
    power = 0;
    bagSize = 0;
    modifyNickNameCount = 0;
    redSoldierId = 0;
    greenSoldierId = 0;
    blueSoldierId = 0;
    combatValue=0;
    vipLevle=0;
    militlity=NULL;
    
    lordid = 0;
    
    pvprankname=NULL;
    prestige=0;
    enterpvpcount=0;
    pvpwincount=0;
    pvptop=0;
    hp=0;
    speed=0;
    governinglimit=0;
    
    _arrayEquipmentCardItem = CCArray::create();
    _arrayEquipmentCardItem->retain();
    _arrayOfficerCardItem = CCArray::create();
    _arrayOfficerCardItem->retain();
    _arrayPropsCardItem = CCArray::create();
    _arrayPropsCardItem->retain();
    _arraySoldierCardItem = CCArray::create();
    _arraySoldierCardItem->retain();
    
    _arrayEmbattleSoldierArray=CCArray::create();
    _arrayEmbattleSoldierArray->retain();
    
    _arraypvpOfficerArray=CCArray::create();
    _arraypvpOfficerArray->retain();
    
}

SGGamePlayerInfo::~SGGamePlayerInfo()
{
    _arrayEquipmentCardItem->release();
    _arrayOfficerCardItem->release();
    _arrayPropsCardItem->release();
    _arraySoldierCardItem->release();
    

    CC_SAFE_RELEASE(name);
    CC_SAFE_RELEASE(nickName);
    CC_SAFE_RELEASE(ssid);
    CC_SAFE_RELEASE(pvprankname);
    
    
}



CCArray *SGGamePlayerInfo::getOfficerCardItemArray()
{
    return _arrayOfficerCardItem;
}

CCArray *SGGamePlayerInfo::getEquipmentCardItemArray()
{
    return _arrayEquipmentCardItem;
}

CCArray *SGGamePlayerInfo::getSoldierCardItemArray()
{
    return _arraySoldierCardItem;
}

CCArray*SGGamePlayerInfo::getembattlesoldierArray()
{

    return  _arrayEmbattleSoldierArray;

}

CCArray*SGGamePlayerInfo::getePVPOfficerArray()
{
    
    return  _arraypvpOfficerArray;
    
}



CCArray *SGGamePlayerInfo::getPropsCardItem()
{
    return _arrayPropsCardItem;
}
//void SGGamePlayerInfo::setLordOfficer(int id)
//{
//    for (int i = 0;i<_arrayOfficerCardItem->count();i++) {
//        SGOfficerCard *card = (SGOfficerCard*)_arrayOfficerCardItem->objectAtIndex(i);
//        if (card->getSsid() == id) {
//            
//        }
//    }
//}

//获取主将
SGOfficerCard *SGGamePlayerInfo::getLordOfficer()
{

    int officerCount = _arrayOfficerCardItem->count();
    if (officerCount == 0) {
        return NULL;
    }
    for (int i = 0;i<_arrayOfficerCardItem->count(); i++) {
        SGOfficerCard *card = (SGOfficerCard*)_arrayOfficerCardItem->objectAtIndex(i);
        if (card->getSsid() == lordid) {
            return card;
        }
        
    }
    
    return NULL;

}


pvpInfo::pvpInfo()
{
    itmeid=0;
    level=0;

}
pvpInfo::~pvpInfo()
{

}
