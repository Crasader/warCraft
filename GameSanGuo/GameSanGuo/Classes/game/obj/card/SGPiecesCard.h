//
//  SGPiecesCard.h
//  GameSanGuo
//
//  Created by bugcode on 13-12-2.
//
//

#ifndef __GameSanGuo__SGPiecesCard__
#define __GameSanGuo__SGPiecesCard__

#include "SGBaseMilitaryCard.h"

class SGPiecesCard : public SGBaseMilitaryCard
{
public:
    virtual const char * getClassName(){return "SGPiecesCard";};
	CC_SYNTHESIZE(int, _piecesCurNum, PiecesCurNum);
	CC_SYNTHESIZE(int, _callCardId, CallCardId);
	CC_SYNTHESIZE(int, _canUseCommon, CanUseCommon);//废弃  2014-8-17
	CC_SYNTHESIZE(int, _maxNum, MaxNum);
	CC_SYNTHESIZE(int, _summonNeed, SummonNeed);
	CC_SYNTHESIZE_RETAIN(CCString *, _piecesDesc, PiecesDesc);
	CC_SYNTHESIZE_RETAIN(CCString *, _piecesName, PiecesName);
	CC_SYNTHESIZE(int, _officerProtoId, OfficerProtoId);
    SGPiecesCard();
    ~SGPiecesCard();
};

#endif /* defined(__GameSanGuo__SGPiecesCard__) */
