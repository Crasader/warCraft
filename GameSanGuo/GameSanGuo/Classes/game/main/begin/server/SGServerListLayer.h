//
//  SGServerListLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-4.
//
//

#ifndef __GameSanGuo__SGServerListLayer__
#define __GameSanGuo__SGServerListLayer__

//#include "SGBaseTableLayer.h"
#include "SGBaseTableBox.h"
#include "SGServerItem.h"
#include "SGHTTPDelegate.h"
#include "cocos-ext.h"
#include "SGCCLabelTTF.h"
#include "SGWelComeLayer.h"

using namespace cocos2d::extension;
using namespace cocos2d;

enum SGServerListLayerComeFrom {
    
    SGServerListLayerComeFromFirst = 0,    //在游戏登陆的时候进入服务器
    SGServerListLayerComeFromGame = 1  //在游戏中进入服务器
};

class SGServerListLayer : public SGBaseTableBox//, public SGHTTPDelegate
{
private:
    
//    SGWelComeLayer *_delegate;
    SGButton *backBtn;
    
    void initSomeThing();
    void buttonClickGoback();
private:
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
public:
    SGServerListLayerComeFrom startFrom;
    
    SGServerListLayer();
    ~SGServerListLayer();
    
//    static SGServerListLayer *create(CCArray *serverList,SGWelComeLayer *welcome, SGServerListLayerComeFrom comeFrom);
//    bool initWithSomeVar(CCArray *serverList,SGWelComeLayer *welcome, SGServerListLayerComeFrom comeFrom);
    
    static SGServerListLayer *create(SGBoxDelegate *delegate,CCArray *serverList,SGServerListLayerComeFrom comeFrom);
    virtual bool init(SGBoxDelegate *delegate,CCArray *serverList,SGServerListLayerComeFrom comeFrom);
};
#endif /* defined(__GameSanGuo__SGServerListLayer__) */
