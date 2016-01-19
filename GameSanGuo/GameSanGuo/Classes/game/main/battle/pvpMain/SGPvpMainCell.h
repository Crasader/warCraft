//
//  SGPvpMainCell.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-9.
//
//

#ifndef __GameSanGuo__SGPvpMainCell__
#define __GameSanGuo__SGPvpMainCell__

#include "SNSTableViewCellItem.h"
#include "SNSTableView.h"


class SGPvpMainCellDelegate;
class SGPvpMainCell : public SNSTableViewCellItem
{
private:
    CCSprite *spriteBg;
    CCSprite *spriteLock;
    CCSprite *spriteTitle;
    SGCCLabelTTF *lableDesc;
    SGCCLabelTTF *lableTitle;
    SNSTableView* tableview;
    bool _isOpen;
    int _index;
    
    
    
    SGPvpMainCellDelegate *delegate;
    CCMenu* menu;
public:
    SGPvpMainCell();
    ~SGPvpMainCell();
    
    static SGPvpMainCell *create(SGPvpMainCellDelegate* del,SNSTableView* table);
    void viewDidLoad();
    void setData(int index);
    void buttonClick(CCObject*obj);

};

class SGPvpMainCellDelegate
{

public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};

};

#endif /* defined(__GameSanGuo__SGPvpMainCell__) */
