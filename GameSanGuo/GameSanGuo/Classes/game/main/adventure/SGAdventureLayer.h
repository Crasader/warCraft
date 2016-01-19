//
//  SGAdventureLayer.h
//  GameSanGuo
//
//  Created by Fygo Woo on 12/25/14.
//
//

#ifndef __GameSanGuo__SGAdventureLayer__
#define __GameSanGuo__SGAdventureLayer__

#include "SGBaseLayer.h"
#include "SGAdventureItem.h"

//奇遇模块类型。添加请在此。
enum SGAdventureType
{
    SGAT_GIANT = -1, //巨大
    
    SGAT_EXOTIC_MERCHANT, //西域商人
    
    SGAT_APPLE, //苹果
};

//奇遇模块ICON，与有效类型数量一样。添加也请在此。
static const char* ADVENTURE_ITEM_ICON[SGAT_APPLE] =
{
    "sgavn_icon_em.png",
};

/*
 * 奇遇页面，可能包含各种单独的“奇遇”模块。
 */
class SGAdventureLayer : public SGBaseLayer, public SGAdventureDelegate
{
//function
public:
    //ctor
    SGAdventureLayer();
    
    //dtor
    ~SGAdventureLayer();
    
    //create
    static SGAdventureLayer* create();
    
    //iv
    bool initView();
    
    //显示第几项模块。
    void showPageByIndex(SGAdventureType at, void* data = NULL);
    
    //点击了index的条目。
    virtual void selectHandler(int index);
    
    //修改触控。
    virtual void setIsCanTouch(bool isTouch);
    
    //触发红点逻辑。
    void handleNotices();
    
//variable
private:
    //顶部滑框。
    CCScrollView* scView_;
    
    //顶部滑框容器。
    CCLayerColor* scContainer_;
    
    //滑框的所有条目。
    SGAdventureItem* advItemArray_[SGAT_APPLE];
    
    //当前奇遇类型。
    SGAdventureType curAdvType_;
    
    //当前奇遇页面。
    SGBaseLayer* curAdvLayer_;
};


#endif /* defined(__GameSanGuo__SGAdventureLayer__) */
