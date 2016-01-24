//
//  SGCardAttribute.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-5-22.
//
//

#include "SGCardAttribute.h"
#include "SGStaticDataManager.h"
CCDictionary *SGCardAttribute::getValue(int currlvl, int itemid)
{
    CCDictionary *dic = CCDictionary::create();
    if (itemid > 1000 && itemid < 3000)    //武将
    {
        SGOfficerDataModel *officer = SGStaticDataManager::shareStatic()->getOfficerById(itemid);
        float baseDef = officer->getOfficerBaseDef();    //基础防御
        float oridef = baseDef * (currlvl * 0.05 + 1);
        int def = oridef + 0.5;

        float oriatk = (float)(baseDef * officer->getOfficerAtkRate() * (0.5 + currlvl * 0.015));
        int atk = oriatk + 0.5;

        float orimor = (float)(baseDef * officer->getOfficerMorRate() * (0.5 + currlvl * 0.015));
        int mor = orimor + 0.5;
        
        //GPCCLOG("orimor:%f,   baseDef :%f,  morRate: %f,   currlv1 :%d,  mor : %d", orimor, baseDef, officer->getOfficerMorRate(), currlvl, mor);

        float oriSpeed = (float)(baseDef * officer->getOfficerSpeedFactor() * (0.5 + currlvl * 0.015));
        int speed = oriSpeed + 0.5;

        dic->setObject(CCString::createWithFormat("%d",def), "def");
        dic->setObject(CCString::createWithFormat("%d",atk), "atk");
        dic->setObject(CCString::createWithFormat("%d",mor), "mor");
        dic->setObject(CCString::createWithFormat("%d",speed), "speed");
    }
    else if( itemid > 3999 && itemid < 6000)   //装备
    {
        SGEquipmentDataModel *equip = SGStaticDataManager::shareStatic()->getEquipById(itemid);
        
        float oridef = (float)(equip->getEquipBaseDef()*0.2*equip->getEquipGrowValue()*currlvl + equip->getEquipBaseDef());
        int def = oridef+.5;
        float oriatk = (float)(equip->getEquipmentBaseAtk()*0.2*equip->getEquipGrowValue()*currlvl + equip->getEquipmentBaseAtk());
        int atk = oriatk+.5;
        float orimor = (float)(equip->getEquipHP()*0.2*equip->getEquipGrowValue()*currlvl + equip->getEquipHP());
        int mor = orimor+.5;

        int speed = (float)(equip->getEquipBaseSpeed()*0.2*equip->getEquipGrowValue()*currlvl + equip->getEquipBaseSpeed()+.5);
        
        CCLOG("XXXXXXXXXXXXXX");
        CCLOG("XXXXXXXXXXXXXX");
        CCLOG("攻击%f",equip->getEquipmentBaseAtk());
        CCLOG("成长系数%f",equip->getEquipGrowValue());
        CCLOG("血量%f",equip->getEquipHP());
        CCLOG("XXXXXXXXXXXXXX");
        CCLOG("最后防御%d",def);
        CCLOG("最后攻击%d",atk);
        CCLOG("最后血量%d",mor);
        CCLOG("最后速度%d",speed);

        dic->setObject(CCString::createWithFormat("%d",def), "def");
        dic->setObject(CCString::createWithFormat("%d",atk), "atk");
        dic->setObject(CCString::createWithFormat("%d",mor), "mor");
        dic->setObject(CCString::createWithFormat("%d",speed), "speed");
    }
    else if( itemid>2999 &&itemid <4000)   //士兵
    {
        SGStaticDataManager* sdm = SGStaticDataManager::shareStatic();
        float soldiertype = (float) sdm->getSoldierGrowProperty(itemid, currlvl) / 10000; //10000为百分比基底。

        SGSoldierDataModel *soldier = sdm->getSoliderById(itemid);
        float oridef = (float)(soldier->getSoldierBaseDef()*soldier->getSoldierGrowValue()*soldiertype + soldier->getSoldierBaseDef());
        int def = oridef+.5;
        float oriatk = (float)(oridef*soldier->getSoldierRound() *soldier->getSoldierAtkRate());
        int atk = oriatk+.5;
        float orimor = (float)(oriatk*soldier->getSoldierMorRate());
        int mor = orimor+.5;

        dic->setObject(CCString::createWithFormat("%d",def), "def");
        dic->setObject(CCString::createWithFormat("%d",atk), "atk");
        dic->setObject(CCString::createWithFormat("%d",mor), "mor");

    }
    return dic;
}
CCArray *SGCardAttribute::spliceStr(const char *str)
{
    CCArray *strs = CCArray::create();
    std::string s = str;
    
    int index = 0;
    int begin = 0;
    while (index <s.length())
    {
        
        if (s[index] == ';')
        {
            std::string get = s.substr(begin,index-begin);
            CCString *str = CCString::create(get);
            strs->addObject(str);
            begin = index+1;
        }
        
        index ++;
        if (index == s.length())
        {
            std::string get = s.substr(begin,index-begin);
            CCString *str = CCString::create(get);
            strs->addObject(str);
        }
    }
    return strs;
}