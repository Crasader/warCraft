//
//  SGSortLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-14.
//
//

#ifndef __GameSanGuo__SGSortLayer__
#define __GameSanGuo__SGSortLayer__



#include "SGBaseBox.h"
#include "SGBaseLayer.h"
#include "SGSortBox.h"

//排序类型
enum sortType
{
    
    sort_star = 0,//星级高
    sort_level,//等级高
    sort_ap ,//攻击高
    sort_def,//防御高   
    sort_speed,//速度高
    sort_morale,//血量高
    sort_round,//蓄力快
    sort_newgot//新获得
};

class SGSortLayer : public SGBaseLayer
{
private:
    
    CCArray *_arrayCard;
    SGSortLayerDelegate *_delegateCallBack;
    SGBoxDelegate *_delegateCannotTouch;
    CCPoint _sortButtonCenter;
    
    void buttonSortCancel();
    void sortCard(sortType type);
   
    void buttonClickSort_1();
    void buttonClickSort_2();
    void buttonClickSort_3();
    void buttonClickSort_4();
    void buttonClickSort_5();
    void buttonClickSort_6();
    void buttonClickSort_7();
    
    
private:
    void sortTypeHandler();
public:
    static SGSortLayer *sortCards(CCArray *array, int type);

    static SGSortLayer *create(CCArray *arrayCard ,SGSortLayerDelegate *callBack ,SGBoxDelegate *isCanTouch ,CCPoint sortButtonCenter);
    void initView();
    SGSortLayer();
    ~SGSortLayer();
};

#endif /* defined(__GameSanGuo__SGSortLayer__) */
