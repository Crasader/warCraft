//
//  SGHeroSkill6.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill6.h"
#include "SGSkillManager.h"


SGHeroSkill6::SGHeroSkill6()
{
    
}

SGHeroSkill6::~SGHeroSkill6(){
    
}




//随机杀死一个敌方未进入蓄力的武将（2格和4格均可）
//发动后，随机杀死敌方5个散兵（不会是武将）
void SGHeroSkill6::activateSkill_20005(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, CCPoint point, int roleid, int skillid)
{
    SGBattleMap *opponentBattleMap = heroLayer1->getBattleMap();
    SGGridBase *grid = opponentBattleMap->myGrids[(int)point.y][(int)point.x];
    ShiBing * sb = opponentBattleMap->getShiBingWithGrid(grid);
    if(sb)
    {
        CCString *name = NULL;
        CCString *name1 = NULL;
        if (skill_id_20005 == skillid) {
            name = CCString::create("animationFile/hfc.plist");
            name1 = CCString::create("animationFile/hfc.scml");
        }else{
            name = CCString::create("animationFile/jy.plist");
            name1 = CCString::create("animationFile/jy.scml");
        }
        
        bool me = !heroLayer->isme;
        
        CCPoint pos = GameConfig::getGridPoint((int)point.y, (int)point.x, me);
        
    //    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(name->getCString());
        ResourceManager::sharedInstance()->bindTexture(name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
        CCSpriterX *fermEffect = CCSpriterX::create(name1->getCString(), true, true);
        fermEffect->setanimaID(0);
        fermEffect->setisloop(false);
        fermEffect->setPosition(pos);
        fermEffect->play();
        
        if (me) {
            heroLayer->addChild(fermEffect);
        }else{
            heroLayer1->addChild(fermEffect);
        }
        
        
        int a = (fermEffect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM);
        
        
        CCDelayTime *time = CCDelayTime::create(a);
        CCCallFuncND *call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::sbSkill_20005), (void*)(a * 10000));
        sb->runAction(CCSequence::create(time, call, NULL));
    }
    
}

