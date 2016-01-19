//
//  SGSkillDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-21.
//
//

#include "SGSkillDataModel.h"
SGSkillDataModel::SGSkillDataModel()
{
    skillId = 0;
    skillInfo = NULL;
    skillName = NULL;
    beanId = NULL;
    round = 0;
    roleName = NULL;
    skillLevel = 0;
    skillClientLevel = NULL;
    skillUpId = 0;
    skillType = 0;
    skillitemid = 0;
    skillstar = 0;
    nextid=0;
    needcoin=0;
    needexp=0;
    level=0;
    haveexp=0;
    iconid = 0;
    value1 = 0;
    value2 = 0;
    effectRound = 0;
    buff = 0;
    //clientType = 0;
    //atkModeSkill = 0;
    //belongId = 0;
}

SGSkillDataModel::~SGSkillDataModel()
{
    CC_SAFE_RELEASE(skillInfo);
    CC_SAFE_RELEASE(skillName);
    CC_SAFE_RELEASE(roleName);
    CC_SAFE_RELEASE(beanId);
    CC_SAFE_RELEASE(skillClientLevel);

}