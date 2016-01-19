//
//  SGAIBu.cpp
//  GameSanGuo
//
//  Created by geng on 13-3-28.
//
//

#include "SGAIBu.h"
#include "SGHeroLayer.h"
#include "SGSkillManager.h"

bool SGAIBu::bu(SGBattleMap *battleMap)
{
    SGHeroLayer *heroLayer = (SGHeroLayer *)battleMap->getParent();
    
    CCArray *buffs = heroLayer->getBuffs();
    if (buffs)
    {
        int round = 0;
        CCDictionary *buff = NULL;
        CCObject *obj = NULL;
        
        CCARRAY_FOREACH(buffs, obj)
        {
            buff = (CCDictionary *)obj;
            int buffId = ((CCString *)buff->objectForKey("buffId"))->intValue();
            switch (buffId)
            {
                case 1000:
                case 1002:
                case 1003:
                {
                    round = ((CCString *)buff->objectForKey("round"))->intValue();
                }
                    break;
                default:
                    break;
            }
            
        }
        CCLog("$阻止对方增兵的buff:%d",round);
        if (round>0)
        {
            CCLog("$有阻止对方增兵的buff:%d",round);
            return false;
        }
    }
    
    
    
    int maxnum = heroLayer->max_soldierNum;
    int soldier =  heroLayer->getSoldier();
    
    if (soldier == 0)
    {
        return false;
    }
    bool isFillUp = false;
    int num = 0;
    int num2 = 0;
    int num4 = 0;
    for (int i = 0; i<mapRow; i++)
    {
        for (int j = 0; j<mapList; j++)
        {
            SGGridBase *grid = battleMap->myGrids[i][j];
            if (grid &&
                grid->getStype() == knormal)
            {
                num++;
                if (grid->getSbType() == ksbtwo)
                {
                    num2++;
                }
                if (grid->getSbType() == ksbfour)
                {
                    num4++;
                }
            }
        }
    }
    
//    //如果回合数为3就补兵
//    if ((heroLayer->roundNum) >2) {
//        CCLog("$回合数大于2,%d",heroLayer->roundNum);
//        return true;
//    }
    
    
    num = num-num2*.5 - num4*0.25;
    CCLog("$maxnum:%d",maxnum);
    CCLog("$num:%d",num);
    CCLog("$soldier:%d",soldier);
    
    
//    if (soldier>0) {
//        return true;
//    }
    if (num <=maxnum/2 && soldier>0)
    {
        CCLog("$bubing");
        return true;
    }
    
//    num = 0;
//    SGHeroLayer *heroLayer = (SGHeroLayer *)battleMap->getParent();
    for (int j = 0; j<mapList-1; j++)
    {
        for (int i =mapRow-1;i>0; i--)
        {
            SGGridBase *grid = battleMap->myGrids[i][j];
            if (grid && grid->getSbType() == ksbfour &&
                grid->getStype() == knormal)
            {
               num = SGAIBu::findNum(battleMap, grid->getSaveColorID(), j);
                if (num<4 &&!isFillUp)
                {
                    isFillUp = true;
//                    return true;
                }
            }
        }
    }
    return isFillUp;
}
int SGAIBu::findNum(SGBattleMap *battleMap, int color, int noj)
{
    int num = 0;
    for (int j = 0; j<mapList; j++)
    {
        if (j != noj && j!=(noj +1))
        {
            for (int i = mapRow-1; i>0; i--)
            {
                SGGridBase *grid = battleMap->myGrids[i][j];
                if (grid)
                {
                    if (grid->getStype() == knormal &&
                        grid->getSbType() == ksbone &&
                        grid->getSaveColorID() == color)
                    {
                        num++;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    return num;
}