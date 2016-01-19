//
//  SGHeroSkill8.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill8.h"

SGHeroSkill8::SGHeroSkill8()
{
    
}

SGHeroSkill8::~SGHeroSkill8()
{
    
}
//空城计
void SGHeroSkill8::activateSkill_20007(SGHeroLayer *heroLayer, float value1, int value2, int buffid)
{
    CCLOG("value:%f",value1);
    CCLOG("round:%d",value2);
    CCLOG("buffid:%d",buffid);
    CCDictionary *buff = CCDictionary::create();
    buff->setObject(CCString::createWithFormat("%f",value1), "value");
    buff->setObject(CCString::createWithFormat("%d",value2), "round");
    buff->setObject(CCString::createWithFormat("%d",buffid), "buffId");
    
    heroLayer->addBuff(buff);
    heroLayer->showEffectLayer(0, false);
    
    CCArray *ar = CCArray::create();
    for (int i = 0; i < mapRow; i++) {
        for (int j = 0; j < mapList; j++) {
            SGGridBase *g = heroLayer->battleMap->myGrids[i][j];
            if (g &&
                g->getStype() == knormal &&
                g->getSbType() == ksbone) {//把单个兵加到待移除数组中去
                ShiBing *sb = heroLayer->battleMap->getShiBingWithGrid(g);
                if(sb)
                    ar->addObject(sb);
            }
        }
    }
    
    heroLayer->battleMap->removeSbAr(ar);//移除放到待移除数组里的士兵

    
    heroLayer->battleMap->refreshMapGrid(false);
    
}
