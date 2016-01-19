//
//  SGMaterialLayer.h
//  GameSanGuo
//
//  Created by bugcode on 14-10-22.
//
//

#ifndef __GameSanGuo__SGMaterialLayer__
#define __GameSanGuo__SGMaterialLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "SGSortLayer.h"
#include "SGCCLabelTTF.h"
#include "SGOfficerCard.h"
#include "SGCardItem.h"
#include "SGBaseBox.h"
#include "ResourceManager.h"
#include <algorithm>


class SGMaterialLayer : public SGBaseTableLayer, public SNSScrollViewPageDelegate
{
public:
    SGMaterialLayer();
    ~SGMaterialLayer();
    static SGMaterialLayer *create();
    
	virtual void initView();
	virtual void refreshPage(int type);
    
    //tableview相关
    SNSTableViewCellItem *tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
	unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
	void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    void backHandler(CCObject *obj);

private:
    SGCCLabelTTF *pageLabel;
    //选择的item的索引，相对于_allData。
    int selectIndex;
};



#endif /* defined(__GameSanGuo__SGMaterialLayer__) */
