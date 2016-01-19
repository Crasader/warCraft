//
//  SG_SBSkillsID1.cpp
//  GameSanGuo
//
//  Created by wwl on 14-2-25.
//
//

#include "SG_SBSkillsID1.h"

SG_SBSkillsID1::SG_SBSkillsID1()
{
    setHeroLayer();
}


SG_SBSkillsID1::~SG_SBSkillsID1()
{
    
}

void SG_SBSkillsID1::activateSkills1_1(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index )
{
    GPCCLOG("SG_SBSkillsID1::activateSkills1_1");
    if (num == 0) return;
    SGHeroLayer *hero = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        hero = getenemyHero();
    }else{
        hero = getmyHero();
    }
    hero->showEffectLayer(3, true, index);
    
    attackList->setAp(attackList->getAp() + (num * attackList->getAfftributeNum()));
    attackList->changeAP(attackList->getAp());
}

bool SG_SBSkillsID1::activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    int blood = attackList->getAp() * attackList->getAfftributeNum1();
	
	//--- 二格兵吸血技能,如果有空城计buff,则不吸血
	bool  haveKongChengji = false;
	
	CCArray *bufs = NULL;
	if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
	{
		bufs = getenemyHero()->getBuffs();
	}
	else
	{
		bufs = getmyHero()->getBuffs();
	}
	
	CCObject *obj = NULL;
	CCDictionary *buff = NULL;
	CCARRAY_FOREACH(bufs, obj)
	{
		buff = (CCDictionary *)obj;
		int buffId = ((CCString *)buff->objectForKey("buffId"))->intValue();
		if (buffId == 1001)
		{
			int round = ((CCString *)buff->objectForKey("round"))->intValue();
			if (round > 0)
			{
				haveKongChengji = true;
				break;
			}
		}
		if (buffId == 1010 || buffId == 1012)// 虚空盾,幽冥盾
		{
			int defValue = ((CCString*)buff->objectForKey("value1"))->intValue();
		    int resideDefValue = defValue - attackList->getAp();//墙剩余的血与攻击力
			int round = ((CCString *)buff->objectForKey("round"))->intValue();
			//if (round > 0)
			{
				if (resideDefValue > 0)//如果防御墙还能抵御,就没有加血效果
				{
					haveKongChengji = true;
				}
				else//如果墙破了,造成伤血,则进行吸血
				{
					haveKongChengji = false;
					blood = abs(resideDefValue * attackList->getAfftributeNum1());
				}
			}
		}
	}
	
	if (!haveKongChengji)
	{
	//-- above
		if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
		{
			getmyHero()->changBloodNum(blood, true, true);
		}else{
			getenemyHero()->changBloodNum(blood, true, true);
		}
	}
    
    return false;
}