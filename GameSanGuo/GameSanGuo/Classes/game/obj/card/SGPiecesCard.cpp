//
//  SGPiecesCard.cpp
//  GameSanGuo
//
//  Created by bugcode on 13-12-2.
//
//

#include "SGPiecesCard.h"
SGPiecesCard::SGPiecesCard():
_piecesCurNum(-1),
_callCardId(-1),
_canUseCommon(-1),
_summonNeed(-1),
_piecesDesc(NULL),
_maxNum(-1),
_piecesName(NULL)
{
	
}

SGPiecesCard::~SGPiecesCard()
{
	CC_SAFE_RELEASE(_piecesName);
	CC_SAFE_RELEASE(_piecesDesc);
}
