//
//  SGGamePrinceAttack.h
//  GameSanGuo
//
//  Created by wenlong w on 12-11-28.
//  2格英雄攻击阵列
//

//git test
#ifndef __GameSanGuo__SGGamePrinceAttack__
#define __GameSanGuo__SGGamePrinceAttack__

#include "SGAttackList.h"

class SGTwoAttackList : public SGAttackList
{
public:
    SGTwoAttackList();
    ~SGTwoAttackList();
    
    static SGTwoAttackList* createWithSoldier(ShiBing *sb, bool ismy);
    
    void showFormEffect();
};

#endif /* defined(__GameSanGuo__SGGamePrinceAttack__) */
