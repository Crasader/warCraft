

#include "LootPiece.h"


#ifndef __GameSanGuo__LootEquip__
#define __GameSanGuo__LootEquip__

class LootEquip : public CCObject
{
    CC_SYNTHESIZE(int , _protoType , ProtoType);
    CC_SYNTHESIZE(int , _protoId , ProtoId);
    CC_SYNTHESIZE_RETAIN(CCArray * , _lootPieceArray , LootPieceArray);
public:
    CREATE_FUNC(LootEquip);
    inline bool init(){    return true; }
    
    ~LootEquip()
    {
        _lootPieceArray->release();
    }
private:
    LootEquip() : _protoId(0) , _protoType(0)
    {
        _lootPieceArray = CCArray::create();
        _lootPieceArray->retain();
    }
};





#endif // -------end.