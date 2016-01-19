//
//  SGSkillCard.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-6-17.
//
//

#ifndef __GameSanGuo__SGSkillCard__
#define __GameSanGuo__SGSkillCard__

#include "SGBaseMilitaryCard.h"
class SGSkillCard : public SGBaseMilitaryCard
{
public:
    virtual const char * getClassName(){return "SGSkillCard";};
    SGSkillCard();
    ~SGSkillCard();
};


#endif /* defined(__GameSanGuo__SGSkillCard__) */
