//
//  SGFHelpSortLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-29.
//
//

#ifndef __GameSanGuo__SGFHelpSortLayer__
#define __GameSanGuo__SGFHelpSortLayer__

#include "SGBaseSortLayer.h"

class SGFHelpSortLayer : public SGBaseSortLayer
{
public:
    SGFHelpSortLayer(void);
    ~SGFHelpSortLayer(void);

    static SGFHelpSortLayer* create(CCArray *datas, SGBaseSortLayerDelegate *callBack, SGBoxDelegate *canTouch, CCPoint position);
    void initView(CCArray *datas, SGBaseSortLayerDelegate *callBack, SGBoxDelegate *canTouch , CCPoint position);
    
    // sort actions
    virtual void sortAction(int index);
};

#endif /* defined(__GameSanGuo__SGFHelpSortLayer__) */
