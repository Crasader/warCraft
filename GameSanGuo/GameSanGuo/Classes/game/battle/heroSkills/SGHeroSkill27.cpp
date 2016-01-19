//
//  SGHeroSkill27.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill27.h"
#include "SGSkillManager.h"

SGHeroSkill27::SGHeroSkill27()
{
    
}

SGHeroSkill27::~SGHeroSkill27()
{
    
}

// 在我方底线放置一排防御墙，可以抵御下回合75%伤害，
//（上限为武将攻击力）如果没有受到伤害，将以主将攻击力25%的伤害进行全列攻击
void SGHeroSkill27::activateSkill_20063(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, int roleid, int buffid, int round, float value1, float value2, float value3)
{
    CCDictionary *buff = CCDictionary::create();
    
    buff->setObject(heroLayer, "hero");
    buff->setObject(heroLayer1, "hero1");
    buff->setObject(CCString::createWithFormat("%d",roleid), "roleid");
    buff->setObject(CCString::createWithFormat("%d",buffid), "buffId");//
    buff->setObject(CCString::createWithFormat("%d",round), "round");
    buff->setObject(CCString::createWithFormat("%f",value1), "value1");
    if (buffid == 1012) {//虚空盾
        if (heroLayer &&
           value2 > 0) {
            heroLayer->changBloodNum(value2, true, true);
        }

    }else{
        buff->setObject(CCString::createWithFormat("%f",value2), "value2");
    }
    
    heroLayer->addBuff(buff);
    
    bool isme = true;
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId() != roleid)
    {
        isme = false;
    }
    
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animationFile/dq.plist");
	ResourceManager::sharedInstance()->bindTexture("animationFile/dq.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);

    for (int i = 0; i < mapList; i++) {
        
        
        CCSpriterX *fermEffect = CCSpriterX::create("animationFile/dq.scml", true, true);
        fermEffect->setanimaID(0);
        fermEffect->setisloop(true);
        CCPoint pos = GameConfig::getGridPoint(isme ? mapRow : -mapRow,   abs(isme ? -i : (mapList - 1 - i)),true);
        fermEffect->setPosition(ccp(pos.x, isme ? heroLayer->uibgY : (screenheight - heroLayer->uibgY)));
        fermEffect->play();
        heroLayer->addChild(fermEffect, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG + i);
        
    }
    
}

