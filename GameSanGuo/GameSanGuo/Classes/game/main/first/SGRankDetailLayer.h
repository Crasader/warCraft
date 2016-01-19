//
//  SGRankDetailLayer.h
//  GameSanGuo
//
//  Created by kanata on 14-2-12.
//
//

#ifndef __GameSanGuo__SGRankDetailLayer__
#define __GameSanGuo__SGRankDetailLayer__

#include <iostream>
#include "SGBaseBox.h"
// detail palyer info structure
class  DPI:public CCObject
{

public:
    DPI();
    ~DPI();
    
public:
    CC_SYNTHESIZE(int , m_gmid, gmid);
    CC_SYNTHESIZE(int , m_playid, playerid);
   // CC_SYNTHESIZE(int , m_gmid, gmid);
    CC_SYNTHESIZE_RETAIN(CCString* , m_name, time);

    
};






class SGRankDetailLayer : public SGBaseBox
{
public:
    SGRankDetailLayer();
    ~SGRankDetailLayer();
    static SGRankDetailLayer *create(SGGamePlayerInfo*player,int type);
    
private:
    CCArray*officedarray;
    CCArray*soldierarray;
    SGGamePlayerInfo*_player;
    int entertype;////0为跨服，非0为本服
    int countrymm;
    
private:
    void initView(SGGamePlayerInfo*player,int type);
    void boxClose(CCObject*obj);
    void initDatas(CCArray*arr);
    void showinfo(CCNode*node);
    void showcountryinfo();
    void showgeneralinfo(CCNode*node);
    void showSoldierinfo(CCNode*node);
    void setCanSee(int type);
    void setCanNotSee(int type);

    
private:
    
};

#endif /* defined(__GameSanGuo__SGRankDetailLayer__) */
