

#include "LootPiece.h"


#ifndef __GameSanGuo__LootOfficer__
#define __GameSanGuo__LootOfficer__

class LootOfficer : public CCObject
{
    CC_SYNTHESIZE(int , _protoType , ProtoType);
    CC_SYNTHESIZE(int , _protoId , ProtoId);
    CC_SYNTHESIZE_RETAIN(CCArray * , _lootPieceArray , LootPieceArray);
public:
    CREATE_FUNC(LootOfficer);
    inline bool init(){    return true; }
    
    ~LootOfficer()
    {
        _lootPieceArray->release();
    }
private:
    LootOfficer() : _protoType(0) , _protoId(0)
    {
        _lootPieceArray = CCArray::create();
        _lootPieceArray->retain();
    }
};





#endif // -------end.