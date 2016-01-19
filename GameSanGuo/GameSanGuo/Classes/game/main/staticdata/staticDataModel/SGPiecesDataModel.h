//
//  SGPiecesDataModel.h
//  GameSanGuo
//
//  Created by bugcode on 13-11-29.
//
//

#ifndef __GameSanGuo__SGPiecesDataModel__
#define __GameSanGuo__SGPiecesDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 套装表 */

class SGPiecesDataModel : public CCObject
{
	//要召唤的卡牌ID
	CC_SYNTHESIZE(int, _callingCardId, CallingCardId);
	// 碎片的id
	CC_SYNTHESIZE(int, piecesId, PiecesId);
	//召唤所需要的卡牌的总数
	CC_SYNTHESIZE(int, _callNeedPieces, callNeedPieces);
	//通用碎片有数量
	CC_SYNTHESIZE(int, _commonPieces, CommonPieces);
	//碎片的名称
	CC_SYNTHESIZE_RETAIN(CCString*, _piecesName, PiecesName);
	//碎片描述
	CC_SYNTHESIZE_RETAIN(CCString *, _piecesDesc, PiecesDesc);
	//碎片价格
	CC_SYNTHESIZE(float, _piecesPrice, PiecesPrice);
	//碎片类型
	CC_SYNTHESIZE(int, _piecesType, PiecesType);
	//星级
	CC_SYNTHESIZE(int, _starLevel, StarLevel);
	//拥有最大的碎片数
	CC_SYNTHESIZE(int, maxPiecesNum, MaxPiecesNum);
    
    //对应武将原型id
    CC_SYNTHESIZE(int, _officerProtoId, OfficerProtoId);

    CC_SYNTHESIZE_RETAIN(CCString *, _piecesJokeInfo, PiecesJokeInfo);

public:
	SGPiecesDataModel();
	~SGPiecesDataModel();
    
    
};

#endif /* defined(__GameSanGuo__SGPiecesDataModel__) */
