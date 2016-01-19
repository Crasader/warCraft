//
//  SGHeroSkill4.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill4.h"
#include "SGSkillManager.h"

SGHeroSkill4::SGHeroSkill4()
{
    
}

SGHeroSkill4::~SGHeroSkill4()
{
    
}

void SGHeroSkill4::activateSkill_20003(SGHeroLayer *heroLayer, int value1, int value2, int roleid)
{
    CCDictionary *buff = CCDictionary::create();
    buff->setObject(CCString::createWithFormat("%d",value1), "round");
    buff->setObject(CCString::createWithFormat("%d",value2), "buffId");
    heroLayer->addBuff(buff);
    
    //FIXME: 位置尚未适配分辨率
//    bool isme = true;
    CCPoint pos = ccp(screenwidth - 116, screenheight - 100); //对方位置
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() != roleid)
    {
        //isme = false;
       pos = ccp(screenwidth - 116, 5); //我的位置
    }
    
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animationFile/jzan.plist");
    ResourceManager::sharedInstance()->bindTexture("animationFile/jzan.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    
    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/jzan.scml", true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(false);
    
    fermEffect->setPosition(pos);
    
    heroLayer->addChild(fermEffect, FORBID_EFFECT_TAG * 10, FORBID_EFFECT_TAG * 10);
    fermEffect->play();
    
    
    
    CCSpriterX *fermEffect1 = CCSpriterX::create("animationFile/jzan_000.scml", true, true);
    fermEffect1->setanimaID(0);
    fermEffect1->setisloop(true);
    fermEffect1->setPosition(pos);
    heroLayer->addChild(fermEffect1, FORBID_EFFECT_TAG * 20, FORBID_EFFECT_TAG * 20);
    fermEffect1->play();
    
    float a = fermEffect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM;
    CCHide *hide = CCHide::create();
    CCDelayTime *time = CCDelayTime::create(a);
    CCShow *show = CCShow::create();
    fermEffect1->runAction(CCSequence::create(hide, time, show, NULL));
}
