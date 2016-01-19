//
//  SGFHelpSortLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-29.
//
//

#include "SGFHelpSortLayer.h"
#include "SGPveHelper.h"
#include <functional>
#include "ResourceManager.h"

using namespace std;

//排序类型
enum fHelpSortType
{
    fhelp_sort_level = 0,//等级高
    fhelp_sort_star,    //星级高
    fhelp_sort_speed,   //速度高
    fhelp_sort_morale,  //血量高
};

SGFHelpSortLayer::SGFHelpSortLayer(void)
{
    
}
SGFHelpSortLayer::~SGFHelpSortLayer(void)
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_fhelpSortLayer);
}

SGFHelpSortLayer* SGFHelpSortLayer::create(CCArray *datas, SGBaseSortLayerDelegate *callBack, SGBoxDelegate *canTouch, CCPoint position)
{
    SGFHelpSortLayer *sortLayer = new SGFHelpSortLayer();
    if (sortLayer && sortLayer->init(NULL, sg_fhelpSortLayer)) {
        
        sortLayer->initView(datas, callBack, canTouch, position);
        sortLayer->autorelease();
        return sortLayer;
    }
    CC_SAFE_DELETE(sortLayer);
    return NULL;
}

void SGFHelpSortLayer::initView(CCArray *datas, SGBaseSortLayerDelegate *callBack, SGBoxDelegate *canTouch, CCPoint position)
{
    vector<int> btnTags;  // used for init sort button tag
    vector<string> btnFiles; // used for init sort button image files
    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("barrack/barrack3.plist");
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_fhelpSortLayer, LIM_PNG_AS_PNG);
    
    for (int ii = 0; ii < 4; ii++) {
        CCString *bName = CCString::createWithFormat("sort_%d.png", ii+1);
        btnFiles.push_back(bName->getCString());
        btnTags.push_back(fHelpSortType(ii));
    }
    SGBaseSortLayer::initView(datas, btnFiles, btnTags,  callBack, canTouch, position);
}

static bool compareLevel(const void* oA, const void* oB)
{
    SGPveHelper *obj1 = (SGPveHelper *)oA;
    SGPveHelper *obj2 = (SGPveHelper *)oB;
    return obj1->getItemLevel() < obj2->getItemLevel();
}

static bool compareStars(const void* oA, const void* oB)
{
    SGPveHelper *obj1 = (SGPveHelper *)oA;
    SGPveHelper *obj2 = (SGPveHelper *)oB;
    return obj1->getItemStar() < obj2->getItemStar();
}


void SGFHelpSortLayer::sortAction(int index)
{
    switch (index) {
        case fhelp_sort_level:
        {
            this->doSort(compareStars);
        }   break;
        case fhelp_sort_star:
        {
            this->doSort(compareLevel);
        }   break;
        case fhelp_sort_speed:
        {
            this->doSort(compareLevel);
        }   break;
        case fhelp_sort_morale:
        {
            this->doSort(compareLevel);
        }   break;
        default:
            break;
    }
}

