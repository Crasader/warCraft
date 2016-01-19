//
//  SGBattleMatching.h
//  GameSanGuo
//  玩家匹配界面
//  Created by gengshengcai on 12-12-20.
//
//

#ifndef __GameSanGuo__SGBattleMatchLayer__
#define __GameSanGuo__SGBattleMatchLayer__

#include "SGBaseLayer.h"
#include "CCSpriterX.h"

class SGBattleMatchLayer : public SGBaseLayer
{
private:
    void requestMatch();
    void requestPve();
    void showTest();
    
    void actJia();
    void actJian();

public:
    SGBattleMatchLayer();
    ~SGBattleMatchLayer();
    static SGBattleMatchLayer *create(const char *fileName, LayerTag tag_);
    virtual void initMsg();

    
private:
    CCSpriterX *sp;
    CCSpriterX *spa;
    int actNum;
    
};

#endif /* defined(__GameSanGuo__SGBattleMatching__) */
