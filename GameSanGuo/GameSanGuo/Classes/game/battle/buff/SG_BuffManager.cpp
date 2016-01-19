//
//  SG_BuffManager.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-17.
//
//

#include "SG_BuffManager.h"
#include "SG_BuffID1.h"
#include "SG_BuffID2.h"
#include "SG_BuffID3.h"
#include "SG_BuffID4.h"

SG_BuffManager::SG_BuffManager()
:m_bfBase(NULL)
{

}

SG_BuffManager::~SG_BuffManager()
{
    
}
//buff的管理器
SG_BuffManager* SG_BuffManager::initBuffManager( SG_SkillsBase *skbase, SGAttackList* attact)
{
    SG_BuffManager* buffmanager = new SG_BuffManager();
    if (buffmanager && buffmanager->init()) {
        CCDictionary *data = CCDictionary::create();
        data->setObject(CCString::createWithFormat("%d",attact->getBuffId()), "BuffId");
        data->setObject(CCString::createWithFormat("%d",attact->getBuffType()), "BuffType");
        data->setObject(CCString::createWithFormat("%d",attact->getBuffRound()), "BuffRound");
        data->setObject(CCString::createWithFormat("%f",attact->getBuffAfftribute()), "BuffAfftribute");
        data->setObject(CCString::createWithFormat("%d",attact->getEffectType()), "EffectType");
        buffmanager->setBuffType( skbase, data);
        buffmanager->autorelease();
        return buffmanager;
    }
    CC_SAFE_DELETE(buffmanager);
    return NULL;
}

//根据服务器发过来的数据修正buff相关数据
SG_BuffManager* SG_BuffManager::repariBuffData(cocos2d::CCDictionary *data)
{
    SG_BuffManager* buffmanager = new SG_BuffManager();
    if (buffmanager && buffmanager->init()) {
        buffmanager->setBuffType( NULL, data);
        buffmanager->autorelease();
        return buffmanager;
    }
    CC_SAFE_DELETE(buffmanager);
    return NULL;
}
//buff类型
SG_BuffBase* SG_BuffManager::setBuffType( SG_SkillsBase *skbase, CCDictionary* data)
{
    
    int BuffId = ((CCString*)data->objectForKey("BuffId"))->intValue();
    int BuffType = ((CCString*)data->objectForKey("BuffType"))->intValue();
    int BuffRound = ((CCString*)data->objectForKey("BuffRound"))->intValue();
    float BuffAfftribute = ((CCString*)data->objectForKey("BuffAfftribute"))->floatValue();
    //int EffectType = ((CCString*)data->objectForKey("EffectType"))->intValue();
    
    switch (BuffId) {
        case BUFFID4:
        case BUFFID5:
        {
            SG_BuffID1* bf = new SG_BuffID1(skbase);
            bf->setBuffID(BuffId);
            bf->m_buffType = BuffType;            bf->setRoundNum(BuffRound);
            bf->setAfftributeNum(BuffAfftribute);
//            bf->setEffectType(EffectType);
            bf->setEffectType(4);
            setbfBase((SG_BuffBase*)bf);
        }
            break;
        case BUFFID2:
        {
            
        }
            break;
        case BUFFID3:
        {
        }
            break;

        case BUFFID6:
        case BUFFID7:
        {
            SG_BuffID2* bf = new SG_BuffID2(skbase);
            bf->setBuffID(BuffId);
            bf->m_buffType = BuffType;
            bf->setRoundNum(BuffRound);
            bf->setAfftributeNum(BuffAfftribute);
//            bf->setEffectType(EffectType);
            bf->setEffectType(6);
            setbfBase((SG_BuffBase*)bf);
        }
            break;
        case BUFFID8:
        {
            SG_BuffID3* bf = new SG_BuffID3(skbase);
            bf->setBuffID(BuffId);
            bf->m_buffType = BuffType;
            bf->setRoundNum(BuffRound);
            bf->setAfftributeNum(BuffAfftribute);
//            bf->setEffectType(EffectType);
            bf->setEffectType(8);
            setbfBase((SG_BuffBase*)bf);
        }
            break;
        case BUFFID9:
        case BUFFID10:
        case BUFFID11:
        {
            SG_BuffID4* bf = new SG_BuffID4(skbase);
            bf->setBuffID(BuffId);
            bf->m_buffType = BuffType;
            bf->setRoundNum(BuffRound);
            bf->setAfftributeNum(BuffAfftribute);
            //            bf->setEffectType(EffectType);
            bf->setEffectType(8);
            setbfBase((SG_BuffBase*)bf);
        }
            break;
        default:
            setbfBase(NULL);
            break;
    }
    
    if (getbfBase()) {
        SG_BuffBase *bf = getbfBase();
        bf->autorelease();
    }

    return getbfBase();
}
