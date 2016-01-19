//
//  SGActivityItem.h
//  GameSanGuo
//
//  Created by dongboqian on 14-1-7.
//
//

#ifndef __GameSanGuo__SGActivityItem__
#define __GameSanGuo__SGActivityItem__

#include "SNSTableViewCellItem.h"
#include "SGActivityLayer.h"
#include "SGStaticDataManager.h"
#include "SGActivityData.h"

class SGActivityItem : public SNSTableViewCellItem
{
//public:
//    CREATE_FUNC(SGActivityItem);
public:
    void update();
    SGActivityItem();
    ~SGActivityItem();
	void setView(SGActivityData* lua);
	static SGActivityItem* create(SGActiveItemDelegate*del,SNSTableView*table,SGActivityData *lua);


private:
    SGActiveItemDelegate *delegate;
    SNSTableView *tableview;
};

#endif /* defined(__GameSanGuo__SGActivityItem__) */
