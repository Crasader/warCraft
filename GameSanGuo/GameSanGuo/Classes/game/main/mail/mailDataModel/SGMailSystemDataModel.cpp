//
//  SGMailSystemDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-5-9.
//
//



#include "SGMailSystemDataModel.h"
SGMailSystemDataModel::SGMailSystemDataModel()
{
    m_isRead = 0;
    m_isReceieve = 0;
    m_roleId = 0;
    m_sendTime = 0;
    m_ssid = 0;
    
    m_pContent = NULL;
    
    m_pAccessoryArray = CCArray::create();
    m_pAccessoryArray->retain();
}

SGMailSystemDataModel::~SGMailSystemDataModel()
{
    CC_SAFE_RELEASE(m_pAccessoryArray);
    CC_SAFE_RELEASE(m_pContent);
}

void SGMailSystemDataModel::addAccessoryToArray(cocos2d::CCObject *object)
{
    if(object)
    {
        m_pAccessoryArray->addObject(object);
    }
}

CCArray* SGMailSystemDataModel::getAccessoryArray()
{
    return m_pAccessoryArray;
}

#pragma mark - ----- SGMailSystemAccessoryDataModel -----

SGMailSystemAccessoryDataModel::SGMailSystemAccessoryDataModel()
{
    
    m_pAccessoryArray = CCArray::create();
    m_pAccessoryArray->retain();
    
    m_ssid = 0;
    m_ItemId = 0;
    m_SystemMailId = 0;
    m_Count = 0;
    m_Level = 0;
    m_Type = 0;
    
}

SGMailSystemAccessoryDataModel::~SGMailSystemAccessoryDataModel()
{
    CC_SAFE_DELETE(m_pAccessoryArray);
}

CCArray* SGMailSystemAccessoryDataModel::getAccessoryArray()
{
    return m_pAccessoryArray;
}


