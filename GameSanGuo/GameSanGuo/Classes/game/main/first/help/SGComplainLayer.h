//
//  SGComplainLayer.h
//  GameSanGuo
//
//  Created by kanata on 14-1-6.
//
//

#ifndef __GameSanGuo__SGComplainLayer__
#define __GameSanGuo__SGComplainLayer__

#include <iostream>
#include "SGBaseTableLayer.h"



class SGComplainLayer:public SGBaseTableLayer
{
public:
      CC_SYNTHESIZE(int, _id, idid);
     // CC_SYNTHESIZE(int , m_idid,checkid); //存储所查看问题的id


public:
    SGComplainLayer();
    ~SGComplainLayer();
    static SGComplainLayer *create(CCArray*array=NULL);
    void initView();
    void backHandler();
    void newquestion();
    void updatedata(int id);
    
    void deletesuccess(CCObject*obj);
    void checkqaresponse(CCObject*obj);
    

protected:
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);


private:
    CCArray*_array;






};
#endif /* defined(__GameSanGuo__SGComplainLayer__) */