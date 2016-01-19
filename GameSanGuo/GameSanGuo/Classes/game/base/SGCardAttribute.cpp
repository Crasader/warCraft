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
    if (itemid > 1000 && itemid<3000) {
        SGOfficerDataModel *officer = SGStaticDataManager::shareStatic()->getOfficerById(itemid);
        float oridef = (float)(officer->getOfficerBaseDef()*officer->getOfficerGrowValue()*0.1*currlvl + officer->getOfficerBaseDef());
        int def = oridef+0.5;
//        int def = oridef;
        float oriatk = (float)(oridef*officer->getOfficerRound() *officer->getOfficerAtkRate());
        int atk = oriatk+0.5;
//        int atk = oriatk;
        float orimor = (float)(oriatk*officer->getOfficerMorRate());
        int mor = orimor+0.5;
//        int mor = orimor;
        //速度新增系数控制，当系数完成乘法后后再进位
        int speed = (float) ( ( oriatk + oridef + orimor) / 20 * officer->getOfficerSpeedFactor()+.5);
//        if (SGPlayerInfo::sharePlayerInfo()->getPlayerCountry()==3) {
//            if (officer->getOfficerRace()==3) {
//                mor = orimor*WU_RATE+.5;
//            }
//            else
//            {
//                mor = orimor*OTHER_RATE+.5;
//            }
//        }

        dic->setObject(CCString::createWithFormat("%d",def), "def");
        dic->setObject(CCString::createWithFormat("%d",atk), "atk");
        dic->setObject(CCString::createWithFormat("%d",mor), "mor");
        dic->setObject(CCString::createWithFormat("%d",speed), "speed");
    }else if( itemid>3999 &&itemid <6000)
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
        //CCLOG("%f",orimor);
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
    else if( itemid>2999 &&itemid <4000)
    {
        SGStaticDataManager* sdm = SGStaticDataManager::shareStatic();
        float soldiertype = (float) sdm->getSoldierGrowProperty(itemid, currlvl) / 10000; //10000为百分比基底。
        
        //old code, 0.5 totally.
        /*if (currlvl>0 && currlvl <100) {
            soldiertype = 0.5;
        }else if(currlvl>99 && currlvl <115)
        {
            soldiertype = 0.5;
        }else
        {
            soldiertype = 0.5;
        }*/
            
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
 

        

//        dic->setObject(CCString::createWithFormat("%f",speed), "speed");
    }
    return dic;
}
CCArray *SGCardAttribute::spliceStr(const char *str)
{
    CCArray *strs = CCArray::create();
    std::string s = str;
    
    int index = 0;
    int begin = 0;
//    CCLOG("length:%d",s.length());
    while (index <s.length())
    {
        
        if (s[index] == ';')
        {
            std::string get = s.substr(begin,index-begin);
//            CCLOG("#####get:%s",get.c_str());
            CCString *str = CCString::create(get);
            strs->addObject(str);
            begin = index+1;
        }
//        CCLOG("index:%d",index);
        
        index ++;
        if (index == s.length())
        {
            std::string get = s.substr(begin,index-begin);
            CCString *str = CCString::create(get);
            strs->addObject(str);
//            CCLOG("#####get:%s",get.c_str());
        }
    }
    return strs;
}