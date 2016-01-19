//
//  SGShowHeadIconLayer.h
//  GameSanGuo
//
//  Created by 赤霄 on 13-12-30.
//
//

#ifndef __GameSanGuo__SGShowHeadIconLayer__
#define __GameSanGuo__SGShowHeadIconLayer__

#include <iostream>
#include "cocos2d.h"
#include "SGHeadIconItem.h"
#include "cocos-ext.h"
#include "SGMailSystemDataModel.h"
#include "SGStaticDataManager.h"
#include "SGMainManager.h"
#include "SGBaseTableLayer.h"
USING_NS_CC_EXT;

  
class SGShowHeadIconLayer:public SGBaseTableLayer {
private:
    
    int _colomNum;
    CCArray *_data;
    void viewDidLoad();
//    int tableViewHeight;
    int tableViewWidth;
 
public:
    SGShowHeadIconLayer();
    ~ SGShowHeadIconLayer();
    static SGShowHeadIconLayer *create(CCArray *data,int colomNum,CCSize size);//type=1是box，0是layer
    
    //tableview delegate
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    virtual  float  tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath);
    
   };



#endif /* defined(__GameSanGuo__SGShowHeadIconLayer__) */
