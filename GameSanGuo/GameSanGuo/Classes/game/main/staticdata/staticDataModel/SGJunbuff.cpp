//
//  SGJunbuff.cpp
//  GameSanGuo
//
//  Created by dongboqian on 14-2-12.
//
//

#include "SGJunbuff.h"
#include "SGStoryBean.h"
#include "SGStorySceneBean.h"
#include "SGStaticDataManager.h"

static SGJunbuff *s_sharePlayerInfo = NULL;
SGJunbuff::SGJunbuff()
: buffid(0)
, buff(NULL)
, condition(NULL)
, junName(NULL)
, m_max(0)
, m_min(0)
, m_score(0)
{
}

SGJunbuff::~SGJunbuff()
{
    
}
//SGJunbuff* SGJunbuff::shareBuffInfo(void)
//{
//    if (!s_sharePlayerInfo)
//    {
//        s_sharePlayerInfo = new SGJunbuff();
//    }
//    return s_sharePlayerInfo;
//}

