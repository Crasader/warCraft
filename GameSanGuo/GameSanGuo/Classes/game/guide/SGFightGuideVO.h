//
//  SGFightGuideVO.h
//  GameSanGuo
//
//  Created by geng on 13-5-13.
//
//

#ifndef __GameSanGuo__SGFightGuideVO__
#define __GameSanGuo__SGFightGuideVO__

#include "SGGuideVO.h"
#include <vector>

class SGFightGuideVO : public SGGuideVO
{
public:
    SGFightGuideVO() : fg_Step(0), fg_Place(0), fg_from(0), fg_touch(0), fg_Level(NULL), fg_Data(NULL)
    {};
    
    CC_SYNTHESIZE(int, fg_Step, Step);
    CC_SYNTHESIZE(int, fg_Place, Place);
    CC_SYNTHESIZE(int, fg_from, From);
    CC_SYNTHESIZE(int, fg_touch, Touch);
    CC_SYNTHESIZE_RETAIN(CCString*, fg_Level, Level);
    CC_SYNTHESIZE_RETAIN(CCString *, fg_Data, Data);
    CC_SYNTHESIZE(CCPoint, fg_Point, Point);
    
    std::vector<int> tips; //显示的Tip,"12"类型
    
public:
    static SGFightGuideVO *create(int mid, int mtype, int mtag);
    bool init(int mid, int mtype, int mtag);
};
#endif /* defined(__GameSanGuo__SGFightGuideVO__) */
