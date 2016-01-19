//
//  SGMainTaskCell.h
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-14.
//
//

#ifndef __GameSanGuo__SGMainTaskCell__
#define __GameSanGuo__SGMainTaskCell__

#include "SNSTableViewCellItem.h"
#include "GameMusicmanage.h"
 

class SGMainTaskLayerDelegate;
class SGMainTaskCell:public SNSTableViewCellItem  {
 
private:
     
    SGCCLabelTTF *titleLabel;
    CCSprite *noticeSprite;
    
    void viewDidLoad();
    SGMainTaskLayerDelegate *delegate;
public:
    CCSize bgSize;
public:
    SGMainTaskCell();
    ~SGMainTaskCell();
    
    static SGMainTaskCell *create(SGMainTaskLayerDelegate *_delegate);
    void updateInfo(CCString *_titile,bool isVisible=false);
    void updateNotice(bool isVisible);
    
    void showSectionInfoHandler();
    void entrySectionHandler();
};

class SGMainTaskLayerDelegate
{
public:
    virtual void entrySectionHandler(SGMainTaskCell *taskItem){};
    virtual void showSectionInfoHandler(SGMainTaskCell *taskItem){};
   
//    virtual void updateMainTaskLayerHandle(SGMainTaskSubLayer *taskSubLayer){};
};


#endif /* defined(__GameSanGuo__SGMainTaskCell__) */
