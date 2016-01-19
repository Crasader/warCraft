//
//  SGActivityItem.cpp
//  GameSanGuo
//
//  Created by dongboqian on 14-1-7.
//
//

#include "SGActivityItem.h"
#include "ResourceManager.h"
#include "CCLuaEngine.h"
#include "SGMainManager.h"

SGActivityItem::SGActivityItem():delegate(NULL),
tableview(NULL)
{

}


SGActivityItem::~SGActivityItem()
{
    //ResourceManager::sharedInstance()->unBindLayerTexture(sg_activityLayer);
}



SGActivityItem* SGActivityItem::create(SGActiveItemDelegate *del, SNSTableView* table,SGActivityData *lua)
{
	SGActivityItem *instance = new SGActivityItem();
	if (instance &&instance->init()) {
        instance->delegate=del;
        instance->tableview=table;
        instance->setView(lua);
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}


void SGActivityItem::setView(SGActivityData* lua)
{

}

void SGActivityItem::update()
{

}