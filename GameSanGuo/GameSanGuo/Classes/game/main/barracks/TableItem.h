//
//  TableItem.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-4.
//
//

#ifndef __GameSanGuo__TableItem__
#define __GameSanGuo__TableItem__

#include "SNSTableViewCellItem.h"
class TableItem : public SNSTableViewCellItem
{
public:
    CCLabelTTF *label;
public:
    TableItem();
	virtual bool initWithSize(CCSize size);
	
	static TableItem* create(CCSize size);
};

#endif /* defined(__GameSanGuo__TableItem__) */
