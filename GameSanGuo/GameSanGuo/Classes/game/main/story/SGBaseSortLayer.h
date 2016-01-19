//
//  SGBaseSortLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-29.
//
//

#ifndef __GameSanGuo__SGBaseSortLayer__
#define __GameSanGuo__SGBaseSortLayer__

#include "SGBaseBox.h"
#include "SGBaseLayer.h"
#include <vector>
#include <string>

//typedef void cc_compare (bool* )(const void*, const void*);
class SGBaseSortLayerDelegate
{
public:
    virtual void sortLayerCallBack(CCArray *data) = 0;
};


class SGBaseSortLayer : public SGBaseLayer
{
private:
    CCSize bgSize;
    CCPoint bgPosition;
    
    SGBaseSortLayerDelegate *_delegate;
    SGBoxDelegate *_delegateTouch;
    
public:
    CCArray *_datas; // 要排列的数据
public:
    SGBaseSortLayer(void);
    virtual ~SGBaseSortLayer(void);
    
    void initView(CCArray *datas, std::vector<std::string> btns, std::vector<int> tags, SGBaseSortLayerDelegate *delegate, SGBoxDelegate *canTouch, CCPoint position);
    void initBackground(int btnCount);
    void initButtons(std::vector<std::string> btns, std::vector<int> tags);
    void buttonSortCancel();
    void buttonSortAction(cocos2d::CCObject *sender);
    void doSort(bool (*compare)(const void*, const void*));
    
    // sort actions
    virtual void sortAction(int index);
};
#endif /* defined(__GameSanGuo__SGBaseSortLayer__) */
