//
//  SGShowHeadIconLayer.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 13-12-30.
//
//

#include "SGShowHeadIconLayer.h"


SGShowHeadIconLayer::SGShowHeadIconLayer() : tableViewWidth(0)
{

    _data=NULL;
    _colomNum=0;
    
    
}
SGShowHeadIconLayer::~SGShowHeadIconLayer(){
    
    CCLog("~SGShowHeadIconLayer");
    
}

SGShowHeadIconLayer *  SGShowHeadIconLayer::create(CCArray *data,int colomNum,CCSize size){

    SGShowHeadIconLayer *layer=new SGShowHeadIconLayer();
    if(layer && layer->init(NULL, sg_showHeadIconLayer)){
//        
        layer->tableViewHeight=size.height;
        layer->tableViewWidth=size.width;
        layer->_colomNum=colomNum;
        layer->_data=CCArray ::create();
        layer->_data->retain();
        for (int ii = 0; ii < data->count(); ii++) {
            CCObject *obj = data->objectAtIndex(ii);
            layer->_data->addObject(obj);
        }
        layer->viewDidLoad();
        layer->autorelease();
        return layer;
        
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void SGShowHeadIconLayer::viewDidLoad(){

    float svHigh = skewingY(tableViewHeight);
    this->datas=_data;
    
    this->tableView->setFrame(CCRectMake(0,0, SGLayout::getSkewing(tableViewWidth), svHigh-20));
    tableViewColumns=_colomNum;
    this->setAnchorPoint(ccp(0.5, 0));
    tableView->setDown(10);
    
    
    
}
SNSTableViewCellItem *SGShowHeadIconLayer ::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath){
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    
	SGHeadIconItem *item = (SGHeadIconItem *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (item == NULL) {
        item = SGHeadIconItem::create();
    }
    if (index<datas->count()) {
        SGMailSystemAccessoryDataModel *access=(SGMailSystemAccessoryDataModel *)this->datas->objectAtIndex(index);
        HeadInfo headInfo=SGMailManager::getAccessoryInfoByData(access);
        item->initView(headInfo);
    }else {
        item->setVisible(false);
    }
    
    return item;

}

float SGShowHeadIconLayer :: tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath){
    return 170;
}
unsigned int SGShowHeadIconLayer ::tableViewAllItemsNumber(SNSTableView* tableView, int section){
    return datas->count();

}
