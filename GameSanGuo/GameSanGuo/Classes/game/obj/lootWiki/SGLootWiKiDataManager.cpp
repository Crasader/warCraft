#include "SGLootWiKiDataManager.h"
#include "SGStaticDataManager.h"

#include <string>

//初始化
SGLootWiKiDataManager * SGLootWiKiDataManager::_self = NULL;


SGLootWiKiDataManager::SGLootWiKiDataManager() : _dict(NULL)
{
    _allLootInfo = CCArray::create();
    _allLootInfo->retain();
}

SGLootWiKiDataManager::~SGLootWiKiDataManager()
{
    _allLootInfo->release();    
}

SGLootWiKiDataManager *  SGLootWiKiDataManager :: shareSGLootWiKiDataManager()
{
    if( NULL == _self )
    {
        _self = new SGLootWiKiDataManager();
        _self->_dict = SGStaticDataManager::shareStatic()->getLootWiKiDict();
        _self->m_materialDict = SGStaticDataManager::shareStatic()->getMaterialLootWiKiDict();
        _self->formatData();
    }
    return _self;
}


void SGLootWiKiDataManager::formatData()
{
    CCDictElement * ele;
    CCDictionary * row;
    
    HaveSource * haveSource = NULL;
    PieceLootInfo * pieceLootInfo = NULL;
    
    bool existSource = false;
    
    CCDICT_FOREACH(_dict , ele)
    {
        row = (CCDictionary *)ele->getObject() ;
        int  pieceId = ((CCString*) row->objectForKey("pieceId"))->intValue();
        int souceType     = ((CCString*) row->objectForKey("lootType"))->intValue();
        int sourcePara       = ((CCString*) row->objectForKey("lootParam"))->intValue();
        string des1 = ((CCString*)row->objectForKey("lootDesc1") )->m_sString;
        CCString* desCs2 = (CCString*)row->objectForKey("lootDesc2");
        string des2 = (desCs2 != NULL) ? desCs2->m_sString : "-1";
        int priority = ((CCString*) row->objectForKey("lootPriority"))->intValue();
        
        haveSource  = HaveSource::create();
        
        haveSource->setSourceType(souceType);
        haveSource->setSourcePara(sourcePara);
        haveSource->setDes1(des1);
        haveSource->setDes2(des2);
        haveSource->setPriority(priority);
        
        for(int i=0 ; i<_allLootInfo->count() ; ++i)
        {
            PieceLootInfo * temp = (PieceLootInfo*)_allLootInfo->objectAtIndex(i);
            if(temp->getPieceId() == pieceId)
            {
                temp->getHaveSource()->addObject(haveSource);
                existSource = true;
                break;
            }
        }
        if( !existSource)
        {
            pieceLootInfo = PieceLootInfo::create();
            pieceLootInfo->setPieceId(pieceId);
            pieceLootInfo->getHaveSource()->addObject(haveSource);
            
            _allLootInfo->addObject(pieceLootInfo);
        }
        existSource = false;
    }
    
    //装备来源
    haveSource = NULL;
    pieceLootInfo = NULL;
    existSource = false;

    CCDICT_FOREACH(m_materialDict , ele)
    {
        row = (CCDictionary *)ele->getObject() ;
        int  pieceId = ((CCString*) row->objectForKey("materialId"))->intValue();
        int souceType     = ((CCString*) row->objectForKey("lootType"))->intValue();
        int sourcePara       = ((CCString*) row->objectForKey("lootParam"))->intValue();
        string des1 = ((CCString*)row->objectForKey("lootDesc1") )->m_sString;
        CCString* desCs2 = (CCString*)row->objectForKey("lootDesc2");
        string des2 = (desCs2 != NULL) ? desCs2->m_sString : "-1";
        int priority = ((CCString*) row->objectForKey("lootPriority"))->intValue();
        
        haveSource  = HaveSource::create();
        
        haveSource->setSourceType(souceType);
        haveSource->setSourcePara(sourcePara);
        haveSource->setDes1(des1);
        haveSource->setDes2(des2);
        haveSource->setPriority(priority);
        
        for(int i=0 ; i<_allLootInfo->count() ; ++i)
        {
            PieceLootInfo * temp = (PieceLootInfo*)_allLootInfo->objectAtIndex(i);
            if(temp->getPieceId() == pieceId)
            {
                temp->getHaveSource()->addObject(haveSource);
                existSource = true;
                break;
            }
        }
        if( !existSource)
        {
            pieceLootInfo = PieceLootInfo::create();
            pieceLootInfo->setPieceId(pieceId);
            pieceLootInfo->getHaveSource()->addObject(haveSource);
            
            _allLootInfo->addObject(pieceLootInfo);
        }
        existSource = false;
    }

}


CCArray *  SGLootWiKiDataManager::getLootPieceArray(int pieceId)
{
    CCArray * ret = NULL;
    for(int i = 0 ; i < _allLootInfo->count() ; ++i)
    {
        PieceLootInfo * temp = (PieceLootInfo * )_allLootInfo->objectAtIndex(i);
        if(pieceId == temp->getPieceId())
        {
            ret  = temp->getHaveSource();
            break;
        }
    }
    return ret;
}





















/*
void SGLootWiKiDataManager::formatData()
{
    CCDictElement * ele;
    CCDictionary * row;
    
    bool isEquipFirst = true;
    bool isOfficerFirst = true;
    
    HavingSource * havingSource = NULL;
    LootPiece * lootPiece = NULL;
    LootOfficer * lootOfficer = NULL;
    LootEquip * lootEquip = NULL;
    CCDICT_FOREACH(_dict , ele)
    {
        row = (CCDictionary *)ele->getObject() ;
        
        //
        
        int protoTypeTemp     = ((CCString*) row->objectForKey("protoType"))->intValue();
        int protoIdTemp       = ((CCString*) row->objectForKey("protoId"))->intValue();
        int pieceIdTemp       = ((CCString*) row->objectForKey("pieceId"))->intValue();
        int piecePriorityTemp = ((CCString*) row->objectForKey("piecePriority"))->intValue();
        int lootTypeTemp      = ((CCString*) row->objectForKey("lootType"))->intValue();
        int paramTemp         = ((CCString*) row->objectForKey("lootParam"))->intValue();
        string descriptorTemp = ((CCString*) row->objectForKey("lootDesc"))->m_sString;
        int priorityTemp      = ((CCString*) row->objectForKey("lootPriority"))->intValue();
        
        havingSource = HavingSource::create();
        havingSource->setLootType(lootTypeTemp);
        havingSource->setLootParam(paramTemp);
        havingSource->setLootDesc(descriptorTemp);
        havingSource->setLootPriority(priorityTemp);
        
        
        
        switch ( protoTypeTemp)
        {
            case 6:
            {
                if (lootPiece && !isOfficerFirst)
                {
                    bool isExistPiece = false;
                    for (int j=0 ; j<_lootOfficerArray->count() ; ++j) //查找是否已经含有碎片id
                    {
                        LootOfficer * lootOfficerTemp = (LootOfficer*)_lootOfficerArray->objectAtIndex(j);
                        for (int i=0; i< lootOfficerTemp->getLootPieceArray()->count() ; ++i)
                        {
                            LootPiece * temp = (LootPiece *)lootOfficerTemp->getLootPieceArray()->objectAtIndex(i);
                            if ( temp->getPieceId() == pieceIdTemp)
                            {
                                temp->getHavingSourceArray()->addObject(havingSource);
                                isExistPiece = true;
                                break;
                            }
                        }
                    }
                    if ( isExistPiece)
                        break;
                    if ( ! isExistPiece)
                    {
                        lootPiece = LootPiece::create();
                        lootPiece->getHavingSourceArray()->addObject(havingSource);
                        lootPiece->setPieceId(pieceIdTemp);
                        lootPiece->setPiecePriority(piecePriorityTemp);
                    }
                }
                else//第一次时lootPiece=null.
                {
                    lootPiece = LootPiece::create();
                    lootPiece->getHavingSourceArray()->addObject(havingSource);
                    lootPiece->setPieceId(pieceIdTemp);
                    lootPiece->setPiecePriority(piecePriorityTemp);
                }
                
                isOfficerFirst = false;
                
                bool isExistOfficer = false;
                for ( int i = 0 ; i < _lootOfficerArray->count() ; ++i)
                {
                    LootOfficer *temp = (LootOfficer *)_lootOfficerArray->objectAtIndex(i);
                    if (temp->getProtoId() == protoIdTemp )
                    {
                        temp->getLootPieceArray()->addObject(lootPiece);
                        isExistOfficer = true;
                        break;
                    }
                }
                if (! isExistOfficer )
                {
                    lootOfficer = LootOfficer::create();
                    lootOfficer->setProtoType(6);
                    lootOfficer->setProtoId(protoIdTemp);
                    lootOfficer->getLootPieceArray()->addObject(lootPiece);
                    
                    _lootOfficerArray->addObject(lootOfficer);
                }
            
                break;
            }
            case 7:
            {
                if (lootPiece && !isEquipFirst)
                {
                    bool isExistPiece = false;
                    for ( int j =0 ; j < _lootEquipArray->count() ; j++)
                    {
                        LootEquip * lootEquipTemp = (LootEquip *)_lootEquipArray->objectAtIndex(j);
                        for (int i=0; i< lootEquipTemp->getLootPieceArray()->count() ; ++i)
                        {
                            LootPiece * temp = (LootPiece *)lootEquipTemp->getLootPieceArray()->objectAtIndex(i);
                            if ( temp->getPieceId() == pieceIdTemp)
                            {
                                temp->getHavingSourceArray()->addObject(havingSource);
                                isExistPiece = true;
                                break;
                            }
                        }
                    }
                    if ( isExistPiece)
                        break;
                    if ( ! isExistPiece)
                    {
                        lootPiece = LootPiece::create();
                        lootPiece->getHavingSourceArray()->addObject(havingSource);
                        lootPiece->setPieceId(pieceIdTemp);
                        lootPiece->setPiecePriority(piecePriorityTemp);
                    }
                }
                else//第一次时lootPiece=null.
                {
                    lootPiece = LootPiece::create();
                    lootPiece->getHavingSourceArray()->addObject(havingSource);
                    lootPiece->setPieceId(pieceIdTemp);
                    lootPiece->setPiecePriority(piecePriorityTemp);
                }
                
                isEquipFirst = false;
                
                
                bool isExistEquip = false;
                for ( int i = 0 ; i < _lootEquipArray->count() ; ++i)
                {
                    LootEquip *temp = (LootEquip *)_lootEquipArray->objectAtIndex(i);
                    if (temp->getProtoId() == protoIdTemp )
                    {
                        temp->getLootPieceArray()->addObject(lootPiece);
                        isExistEquip = true;
                        break;
                    }
                }
                if (! isExistEquip )
                {
                    lootEquip = LootEquip::create();
                    lootEquip->setProtoType(7);
                    lootEquip->setProtoId(protoIdTemp);
                    lootEquip->getLootPieceArray()->addObject(lootPiece);
                    
                    _lootEquipArray->addObject(lootEquip);
                }
                
                break;
            }

//            case 7:
//            {
//                if ( lastProtoId == protoIdTemp ) //物品不变
//                {
//                    lootEquip->getLootPieceArray()->addObject(lootPiece);
//                }
//                else
//                {
//                    if ( lootEquip ) //跳过第一次
//                        _lootOfficerArray->addObject(lootEquip);
//                    lootEquip = LootEquip::create();
//                    lootEquip->setProtoType(7);
//                    lootEquip->setProtoId(protoIdTemp);
//                    
//                    lastProtoId = protoIdTemp;
//                }
//                
//                break;
//            }
            default:
                CCAssert(false, "数据源错误.");
        }
    }
}


CCArray * SGLootWiKiDataManager::getLootPieceArray(int protoType, int protoId)
{
    CCArray * ret = NULL;
    switch (protoType)
    {
        case 6: //武将
            for (int i =0 ; i < _lootOfficerArray->count() ; ++i )
            {
                LootOfficer * p = (LootOfficer *)_lootOfficerArray->objectAtIndex(i);
                if ( protoId == p->getProtoId() )
                {
                    ret = p->getLootPieceArray();
                    break;
                }
            }
            break;
        case 7: //装备
            for (int i =0 ; i < _lootEquipArray->count() ; ++i )
            {
                LootEquip * p = (LootEquip *)_lootEquipArray->objectAtIndex(i);
                if ( protoId == p->getProtoId() )
                {
                    ret = p->getLootPieceArray();
                    break;
                }
            }
            break;
        default:
            CCAssert(false, "数据来源错误.");
    }
    
    return ret;
}
*/



