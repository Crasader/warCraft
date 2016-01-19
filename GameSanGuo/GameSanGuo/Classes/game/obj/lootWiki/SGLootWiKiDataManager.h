//SGLootWiKiDataManager.h
//by:zyc 2014-7-8
//管理图鉴数据，目前只有武将 和 装备
//

#include "cocos2d.h"
//#include "LootOfficer.h"
//#include "LootPiece.h"
//#include "LootEquip.h"
USING_NS_CC;
using namespace std;


#ifndef __GameSanGuo__SGLootWiKiDataManager__
#define __GameSanGuo__SGLootWiKiDataManager__

class HaveSource : public CCObject
{
    CC_SYNTHESIZE(int , _sourceType , SourceType);
    CC_SYNTHESIZE(int , _sourcePara , SourcePara);
    CC_SYNTHESIZE(std::string , _des1 , Des1);
    CC_SYNTHESIZE(std::string , _des2 , Des2);
    CC_SYNTHESIZE(int , priority, Priority)
public:
    CREATE_FUNC(HaveSource);
    inline bool init(){ return true;}
};

class PieceLootInfo : public CCObject
{
    CC_SYNTHESIZE(int , _pieceId , PieceId);
    CC_SYNTHESIZE_RETAIN(CCArray * , _haveSource , HaveSource);
public:
    CREATE_FUNC(PieceLootInfo);
    inline bool init(){ return true;}
    
    PieceLootInfo():_haveSource(NULL)
    {
        _haveSource = CCArray::create();
        _haveSource->retain();
    }
};



class SGLootWiKiDataManager
{
public:
    //ctor
    ~SGLootWiKiDataManager();
    //singleton model
    static SGLootWiKiDataManager * shareSGLootWiKiDataManager();
    
    //protoid 获取对应的lootpieceArray
    CCArray * getLootPieceArray(int pieceId);
private:
    //ctor
    SGLootWiKiDataManager();
    //抽取数据 , 只在构造函数中调用一次
    void formatData();
    // static this
    static SGLootWiKiDataManager * _self;
    
    //unknown
    CCDictionary * _dict;
    
    CCDictionary * m_materialDict;
    
    
    //武将详情
    CCArray * _allLootInfo;
};











#endif   //----------end.