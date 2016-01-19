#include "cocos2d.h"
#include "CCPlatformMacros.h"
#include "HavingSource.h"
using namespace cocos2d;

#ifndef __GameSanGuo__LootPiece__
#define __GameSanGuo__LootPiece__

class LootPiece : public CCObject
{
    CC_SYNTHESIZE(int , _pieceId , PieceId);
    CC_SYNTHESIZE(int , _piecePriority , PiecePriority);
    //CC_SYNTHESIZE_RETAIN(HavingSource * , _havingSource , HavingSourcePtr)
    CC_SYNTHESIZE_RETAIN(CCArray * , _havingSourceArray , HavingSourceArray);
public:
    CREATE_FUNC(LootPiece);
    inline bool init(){    return true; }
    
    ~LootPiece()
    {
        _havingSourceArray->release();
    }
private:
    LootPiece() : _pieceId(0) , _piecePriority(0)
    {
        _havingSourceArray = CCArray::create();
        _havingSourceArray->retain();
    }
};



#endif //-------end.