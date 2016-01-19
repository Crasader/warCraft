//
//  SGBaseTableLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-11.
//
//

#include "SGBaseTableLayer.h"
#include "SimpleAudioEngine.h"

SGBaseTableLayer::SGBaseTableLayer()
:datas(NULL),
tableViewHeight(110),
tableViewColumns(1),
boxRect(CCRectMake(0, 0, 600, 600)),
tableView(NULL)
{

}
SGBaseTableLayer::~SGBaseTableLayer()
{
//    CC_SAFE_RELEASE(datas);
    
    
}
bool SGBaseTableLayer::init(const char *fileName_, LayerTag tag_, bool isSpriteFrame)
{
    if (!SGBaseLayer::init(fileName_, tag_, isSpriteFrame))
    {
        return false;
    }
    
    tableView = SNSTableView::create(boxRect, TableViewTypeVertical);
	tableView->setDatasource(this);
	tableView->setDelegate(this);
    tableView->setShowSlider(false);
    this->addChild(tableView);
    
    return true;
}
void SGBaseTableLayer::initView()
{
    
}
SNSTableViewCellItem* SGBaseTableLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    return NULL;
}

unsigned int SGBaseTableLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
	return datas->count();
}

float SGBaseTableLayer::tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	return tableViewHeight;
}

void SGBaseTableLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
}
void SGBaseTableLayer::tableViewSelectItem(SNSTableView *tableView, SNSIndexPath *indexPath, cocos2d::CCPoint position)
{
}
unsigned int SGBaseTableLayer::tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section)
{
	return tableViewColumns;
}
void SGBaseTableLayer::setIsCanTouch(bool isTouch)
{
    SGBaseLayer::setIsCanTouch(isTouch);
    tableView->setIsTouchEnabled(isTouch);
}


void SGBaseTableLayer::hideScrollItemSingleMove()
{
    if (tableView == NULL) {
        return;
    }
    int rowCount = tableView->getRowCount();
    // 显示初始动作
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    for (int ii = 0; ii < rowCount; ii++) {
        SNSTableViewCell *cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
            
        if (cell != NULL) {
            CCAction *action = CCSequence::create(CCDelayTime::create(0.15f*(ii/10.0f+1)),
                                                                    CCMoveBy::create(0.15f, ccp(size.width, 0)),
                                                                     NULL);
            cell->runAction(action);
        }
    }
}


void SGBaseTableLayer::hideScrollItemScale()
{
    if (tableView == NULL) {
        return;
    }
    int rowCount = tableView->getRowCount();
    CCAction *action1 = NULL;
    CCAction *action2 = NULL;
    for (int ii = 0; ii < rowCount; ii++) {
        SNSTableViewCell *cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
        if (cell != NULL) {
            action1 = CCScaleTo::create(ITEMACTIONTIME, 0.01);
            action2 = CCFadeOut::create(ITEMACTIONTIME);
            cell->runAction(action1);
            cell->runAction(action2);
        }
    }
}

void SGBaseTableLayer::hideScrollItemMultiMove()
{
//    EFFECT_PLAY(MUSIC_ITEM);
    if (tableView == NULL) {
        return;
    }
    bool isSingleRow = true;
    CCAction *action = NULL;
    SNSTableViewCell *cell = NULL;
    int rowCount = tableView->getRowCount();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = CCPointZero;
    
    
    for (int ii = 0; ii < rowCount; ii++) {
        cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
        if (cell != NULL)
        {
            pos = cell->getPosition();
            if (isSingleRow)
            {
                isSingleRow = false;
                action = //CCSequence::create(CCDelayTime::create(ITEMACTIONTIME*(ii/10.0f+1)),
                CCMoveBy::create(ITEMACTIONTIME, ccp(-size.width, 0));
                // NULL);
            }
            else{
                isSingleRow = true;
                action = //CCSequence::create(CCDelayTime::create(ITEMACTIONTIME*(ii/10.0f+1)),
                CCMoveBy::create(ITEMACTIONTIME, ccp(size.width, 0));
                //NULL);
            }
            cell->runAction(action);
        }
    }
}