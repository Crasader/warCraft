//
//  SGBasetableBox.cpp
//  GameSanGuo
// 具有列表功能的弹框
//  Created by gengshengcai on 13-1-11.
//
//

#include "SGBasetableBox.h"
#include "SimpleAudioEngine.h"
SGBaseTableBox::SGBaseTableBox()
:datas(NULL),
tableViewHeight(100),
tableViewColumns(1),
tableView(NULL)
{
    
}
SGBaseTableBox::~SGBaseTableBox()
{
    //    CC_SAFE_RELEASE(datas);
}
bool SGBaseTableBox::init(SGBoxDelegate *dg, BoxTag bt, CCSize size, bool isHaveBottom,bool isHaveBg,bool useDefaultBg)
{
    if (!SGBaseBox::init(dg, bt, size , isHaveBottom,isHaveBg,CCSizeMake(0, 0), useDefaultBg))
    {
        return false;
    }
    CCLOG("initTableView");
    tableView = SNSTableView::create(CCRectMake(0, 0, 600, 600), TableViewTypeVertical);
	tableView->setDatasource(this);
	tableView->setDelegate(this);
    tableView->setShowSlider(false);
//	tableView->setPosition(ccpAdd(SGLayout::getPoint(kMiddleLeft), ccp(0, -tableView->getContentSize().height/2)));
    this->addChild(tableView,1);
    
    return true;
}
SNSTableViewCellItem* SGBaseTableBox::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    return NULL;
}

unsigned int SGBaseTableBox::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
	return datas->count();
}

float SGBaseTableBox::tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	return tableViewHeight;
}

void SGBaseTableBox::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    
}

unsigned int SGBaseTableBox::tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section)
{
	return tableViewColumns;
}

