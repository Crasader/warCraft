//
//  SGHeroSkill7.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill7.h"
#include "SGSkillManager.h"

SGHeroSkill7::SGHeroSkill7()
{
    
}

SGHeroSkill7::~SGHeroSkill7()
{
    
}

//对敌方阵营随机5个位置进行伤害，总额为主将攻击力的100%
void SGHeroSkill7::activateSkill_20006(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, CCPoint point, int skillId)
{
    if (!heroLayer1 || !heroLayer)
    {
        return ;
    }

    SGBattleMap *opponentBattleMap = heroLayer1->getBattleMap();
    if (!opponentBattleMap)
    {
        return ;
    }
    SGGridBase *grid = opponentBattleMap->myGrids[(int)point.y][(int)point.x];
    if (!grid)
    {
        return ;
    }
    ShiBing * sb = opponentBattleMap->getShiBingWithGrid(grid);
    if (!sb)
    {
        return ;
    }
    bool a = !heroLayer->isme;
    CCString *name = NULL;
    CCString *name1 = NULL;
    if (skillId == skill_id_20026 ||
        skillId == skill_id_20027 ||
        skillId == skill_id_20086 ||
        skillId == skill_id_20096) {
        name = CCString::create("animationFile/yinsi.plist");
        name1 = CCString::create("animationFile/yinsi.scml");
    }else{
        name = CCString::create("animationFile/ll.plist");
        name1 = CCString::create("animationFile/ll.scml");
    }
    
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(name->getCString());
    ResourceManager::sharedInstance()->bindTexture(name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
    CCSpriterX *fermEffect = CCSpriterX::create(name1->getCString(), true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(false);
    //    fermEffect->setPosition(GameConfig::getGridPoint(a ? -((int)point.y + 1) : (int)point.y, a ? mapList - 1 - (int)point.x :(int)point.x));
    fermEffect->setPosition(GameConfig::getGridPoint((int)point.y, (int)point.x, a));
    fermEffect->play();
    if (heroLayer->isme) {
        heroLayer->addChild(fermEffect, 100);
    }else{
        heroLayer1->addChild(fermEffect, 100);
    }
    
    
    if (!grid) {
        return;
    }
    
    bool isDe = false;
    CCLOG(">>>>>%f",value1);
	//掉血效果显示,数值是技能攻击的数值
	heroLayer->showBattleLabel(value1, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());

    if (grid->getStype() == knormal)
    {
        CCLOG("散兵");
        isDe = true;
        
    }
    else if (grid->getStype() == kattack)
    {
        CCLOG("攻击阵列");
        SGAttackList *attackList = opponentBattleMap->getAttackListByGrid(grid);
        if (attackList)
        {
            if (value1 >= attackList->getAp())
            {
                int newValue = attackList->getAp();
                attackList->heroSkillSubAp(newValue);
                
                CCLOG("被打死");
                isDe = true;
            }
            else
            {
                //显示掉血效果
                attackList->heroSkillSubAp(value1);
                CCLOG("未被打死");
                float newValue = attackList->getAp() - value1;
                attackList->setAp(newValue);
                //造成伤害,播放音效
                if (value1 > 0)
                {
                    attackList->playEffectWithType(kAttackEffect);
                }
                
                //攻击后血量减少显示
                attackList->drawXueTiaoProportion();
                attackList->changAPLabel();
                
                CCLOG("val1 = %f, da = %d, max = %d", value1, attackList->damageNum, attackList->getMax_Ap());

            }
        }
    }
    else if(grid ->getStype() == kdefend)
    {
        if (!sb)
        {
            return ;
        }
        CCLOG("防御");
        if (value1 >= sb->getDef())
        {
            CCLOG("被打死");
            isDe = true;
        }
        else
        {
            CCLOG("未被打死");
            sb->setDef(sb->getDef()- value1);
        }
    }
    if (isDe) {
        int a = (fermEffect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM);
        CCDelayTime *time = CCDelayTime::create(a);
        CCCallFuncND *call = CCCallFuncND::create(SGSkillManager::shareSkill(), callfuncND_selector(SGSkillManager::sbSkill_20005), (void*)(a* 10000));
        sb->runAction(CCSequence::create(time, call, NULL));
    }
    
}