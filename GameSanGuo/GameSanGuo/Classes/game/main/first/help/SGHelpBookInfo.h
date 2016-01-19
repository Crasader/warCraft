//
//  SGHelpBookInfo.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-8-7.
//
//

#ifndef __GameSanGuo__SGHelpBookInfo__
#define __GameSanGuo__SGHelpBookInfo__

#include "cocos2d.h"
#include "SGBaseLayer.h"
#include "cocos-ext.h"
#include "SGHelpTitleDataModel.h"
USING_NS_CC;
USING_NS_CC_EXT;
class SGHelpBookInfo : public SGBaseLayer
{
private:
    CCScrollView *scrollView;
    int type;
private:
    void initview(SGHelpTitleDataModel *mode);
    void buttonClickGoback();
    void showSeasonMainLayer(CCObject *sender);
public:
    static SGHelpBookInfo *create(SGHelpTitleDataModel *mode,int entertype = 0);
    SGHelpBookInfo();
    ~SGHelpBookInfo();
};

#endif /* defined(__GameSanGuo__SGHelpBookInfo__) */
