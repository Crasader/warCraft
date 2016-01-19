//
//  SGPropsLayer.h
//  GameSanGuo
//  道具界面
//  Created by gengshengcai on 13-1-10.
//
//

#ifndef __GameSanGuo__SGPropsLayer__
#define __GameSanGuo__SGPropsLayer__

#include "SGBaseTableLayer.h"
#include "SGSortLayer.h"

class SGPropsLayer : public SGBaseTableLayer , public SGSortLayerDelegate, public SNSScrollViewPageDelegate
{
private:
    int currentPage;
private:
    
    SGCCLabelTTF *pageLabel;
//    void cardInfoListener(CCObject *obj);
    void backHandler();
    void buttonClickSort();
    virtual void sortLayerCallBack(CCArray *cardArray);
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);    
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 设置翻页
    virtual void refreshPage(int type);
public:
    SGPropsLayer();
    ~SGPropsLayer();
    static SGPropsLayer *create(int currentPage=1000);
    void initView();
};
#endif /* defined(__GameSanGuo__SGPropsLayer__) */
