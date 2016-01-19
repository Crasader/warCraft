//
//  SGBattleFieldItem.h
//  GameSanGuo
//
//  Created by wwl on 13-10-8.
//
//

#ifndef __GameSanGuo__SGBattleFieldItem__
#define __GameSanGuo__SGBattleFieldItem__

#include <iostream>
#include "SGStoryBaseCell.h"
#include "SGBattleFieldBaseItem.h"
#include "SGBattleFieldData.h"

class SGStorySectionDelegate;
class SGBattleFieldItem : public SGBattleFieldBaseItem {
    
    
public:
    SGBattleFieldItem();
    ~SGBattleFieldItem();
    
    static SGBattleFieldItem* create( SGStorySectionDelegate *_delegate, SGBattleFieldData *_data);
    void initItem();
    

void entrySectionHandler();
void showSectionInfoHandler();

public:
    SGBattleFieldData *data;
    SGStorySectionDelegate *delegate;
};


class SGStorySectionDelegate
{
public:
    virtual void entrySectionHandler(SGBattleFieldItem *giftItem){};
    virtual void showSectionInfoHandler(SGBattleFieldItem *giftItem){};
};
#endif /* defined(__GameSanGuo__SGBattleFieldItem__) */
