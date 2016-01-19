//
//  TestData.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-22.
//
//

#ifndef __GameSanGuo__TestData__
#define __GameSanGuo__TestData__

#include "cocos2d.h"
#include "GameConfig.h"
using namespace cocos2d;
class Index : public CCObject
{
public:
    int i;
    int j;
};
class BData :public cocos2d::CCObject
{
public:
    
    CC_SYNTHESIZE(int, p_mSid,Sid);
    CC_SYNTHESIZE(int, P_mColorId, ColorId);
    CC_SYNTHESIZE(int, p_mtype, Type);
    CC_SYNTHESIZE(GameIndex, p_index, Index);
//    CC_SYNTHESIZE_RETAIN(CCArray*, pIndexs, Indexs);
//    int sid;
//    int colorid;
};
class TestData :public cocos2d::CCObject
{
public:
    static TestData* shareData(void);
    static cocos2d::CCArray *getIndexArray();
    static BData *get(int i,int c, int t, GameIndex index);
    static CCArray *getMapArray();
    static Index* getIndex(int i, int j);
    void addData(int i, int c, int t, int inedxi, int indexj);
    void removeData(int indexi,int indexj);
    CCArray *getMapData();
private:
    CCArray *mapArray;
};
#endif /* defined(__GameSanGuo__TestData__) */
