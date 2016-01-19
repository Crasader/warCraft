//
//  SGPiecesDataModel.cpp
//  GameSanGuo
//
//  Created by bugcode on 13-11-29.
//
//

#include "SGPiecesDataModel.h"

SGPiecesDataModel::SGPiecesDataModel():
_callingCardId(-1),
_callNeedPieces(-1),
_commonPieces(-1),
_piecesName(NULL),
_piecesDesc(NULL),
_piecesPrice(0.0f),
_piecesType(-1),
_starLevel(-1)
, piecesId(0)
, maxPiecesNum(0)
,_piecesJokeInfo(NULL)
{
	
}

SGPiecesDataModel::~SGPiecesDataModel()
{
	CC_SAFE_RELEASE(_piecesDesc);
	CC_SAFE_RELEASE(_piecesName);
    CC_SAFE_RELEASE(_piecesJokeInfo);

}