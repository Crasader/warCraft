//
//  SGBattleFieldBaseItem.h
//  GameSanGuo
//
//  Created by wwl on 13-10-8.
//
//

#ifndef __GameSanGuo__SGBattleFieldBaseItem__
#define __GameSanGuo__SGBattleFieldBaseItem__

#include <iostream>
#include "SNSTableViewCellItem.h"


class SGBattleFieldBaseItem : public SNSTableViewCellItem {
    
public:
    SGBattleFieldBaseItem();
    ~SGBattleFieldBaseItem();
    
    static SGBattleFieldBaseItem* create();
    bool init();
    
    CCSize bgSize;
};



#endif /* defined(__GameSanGuo__SGBattleFieldBaseItem__) */
