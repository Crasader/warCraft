//
//  SGActivityData.cpp
//  GameSanGuo
//
//  Created by zenghui on 14-2-21.
//
//

#include "SGActivityData.h"

SGActivityData::SGActivityData(void):m_luaActivityId(0),m_luaName(""),m_btnName(""),m_notice(false)
{

}

SGActivityData::~SGActivityData(void)
{

}


SGActivityData* SGActivityData::create(int luaActivityId,const ::std::string& luaName,const ::std::string& btnName, bool notice)
{
    SGActivityData *activityData = new SGActivityData();
    if (activityData) {
        activityData->setLuaActivityId(luaActivityId);
        activityData->setLuaName(luaName);
        activityData->setBtnName(btnName);
        activityData->setNotice(notice);//设置活动是不是显示
        activityData->autorelease();
        activityData->initData();
        return activityData;
    }
    CC_SAFE_DELETE(activityData);
    return NULL;
}
void SGActivityData::initData()
{
    m_labelInfo.clear();
    m_btnInfo.clear();
    m_ItemInfo.clear();
    m_imageInfo.clear();
}

void SGActivityData::addLabelInfo(int labelId,int labelValue,ccColor4B color)
{
    SGLabelInfo labelInfo(labelId, labelValue,color);
    m_labelInfo.push_back(labelInfo);
}

void SGActivityData::addBtnInfo(int btnId,std::string fontStr,ccColor4B fontColor,std::string bgStr,std::string fgStr,ccColor4B imageColor,int btnState)
{
    SGBtnInfo btnInfo(btnId, fontStr,fontColor,bgStr,fgStr,imageColor,btnState);
    m_btnInfo.push_back(btnInfo);
}

void SGActivityData::addItemInfo(int itemType,int itemId,int itemCount,int itemLevel,int itemBtId)
{
    SGItemInfo itemInfo(itemType, itemId,itemCount,itemLevel,itemBtId);
    m_ItemInfo.push_back(itemInfo);
}
void SGActivityData::addLabelStringInfo(int labelId, std::string labelValue,ccColor4B color)
{
    SGLabelStringInfo itemInfo(labelId, labelValue, color);
    m_LabelStringInfo.push_back(itemInfo);
}
void SGActivityData::addImageInfo(int imageId, std::string imageValue,ccColor4B color)
{
    SGImageInfo itemInfo(imageId, imageValue, color);
    m_imageInfo.push_back(itemInfo);
}
int SGActivityData::getLabelValueById(int labelId)
{
    int size = m_labelInfo.size();
    for (int i= 0; i< size; i++)
    {
        if (m_labelInfo[i].getLabelId() == labelId) {
            return m_labelInfo[i].getLabelValue();
        }
    }
    return 0;
}

int SGActivityData::getBtnStateById(int btnId)
{
    int size = m_btnInfo.size();
    for (int i= 0; i< size; i++)
    {
        if (m_btnInfo[i].getBtnId() == btnId) {
            return m_btnInfo[i].getBtnState();
        }
    }
    return 0;
}

void SGActivityData::setBtnStateById(int btnId,int btnState)
{
    int size = m_btnInfo.size();
    for (int i= 0; i< size; i++)
    {
        if (m_btnInfo[i].getBtnId() == btnId) {
            m_btnInfo[i].setBtnState(btnState);
        }
    }
}

SGItemInfo *SGActivityData::getItemInfo(int itemId,int itemType)
{
    int size = m_ItemInfo.size();
    for (int i= 0; i< size; i++)
    {
        if (m_ItemInfo[i].getItemId() == itemId && m_ItemInfo[i].getItemType() == itemType) {
            return &m_ItemInfo[i];
        }
    }
    return NULL;
}
int SGActivityData::getLabelNum()
{
    return m_labelInfo.size();
}
int SGActivityData::getBtnNum()
{
    return m_btnInfo.size();
}
int SGActivityData::getItemNum()
{
    return m_ItemInfo.size();
}
int SGActivityData::getLabelStringnum()
{
    return m_LabelStringInfo.size();
}

int SGActivityData::getImagenum()
{
    return m_imageInfo.size();
}

SGActivityUpdateData::SGActivityUpdateData(void):m_updatetime(0),m_label(NULL),m_activityIndex(0)
{
    m_label=NULL;
    m_updatetime=0;
}
SGActivityUpdateData::~SGActivityUpdateData(void)
{
    m_updatetime=0;
    m_label=NULL;
}


SGActivityUpdateData* SGActivityUpdateData::create(int updatetime,CCLabelTTF* label,int index)
{
    SGActivityUpdateData *activityData = new SGActivityUpdateData();
    if (activityData) {
        activityData->setActivityUpdateTime(updatetime);
        activityData->setLabel(label);
        activityData->setActivityIndex(index);
        activityData->autorelease();
        return activityData;
    }
    CC_SAFE_DELETE(activityData);
    return NULL;

}


SGItemCondition::SGItemCondition():
m_cardId(0),
m_cardlevel(0),
m_cardNum(0),
m_cardState(0),
m_cardtype(0)
{
   
}

SGItemResourt::SGItemResourt()
:m_cardtype(0),
m_cardNum(0),
m_cardlevel(0),
m_cardId(0)
{

}

SGACtivityBar::SGACtivityBar()
:m_barbtnstate(0),
m_description(0),
m_barId(-1)
{
    m_itemresourt.clear();
    m_itemcondition.clear();
}

SGActivityLuaTemplateData::SGActivityLuaTemplateData()
:m_luaActivityId(0),
m_btntype(0),
m_templatetype(0),
m_luaName(""),
m_btnName(""),
m_titleName(""),
m_infoName("")
{
    m_item.clear();
}


SGActivityLuaTemplateData::~SGActivityLuaTemplateData()
{}

SGActivityLuaTemplateData* SGActivityLuaTemplateData::create(int luaActivityId, int btnid, int templatetype, const std::string &luaName, const std::string &btnName, const std::string &titleName, const std::string &infoName)
{
    SGActivityLuaTemplateData *templatedate = new SGActivityLuaTemplateData();
    if (templatedate)
    {
        templatedate->setLuaActivityId(luaActivityId);
        templatedate->setBtnId(btnid);
        templatedate->setTemplateType(templatetype);
        templatedate->setLuaName(luaName);
        templatedate->setBtnName(btnName);
        templatedate->setTitleName(titleName);
        templatedate->setInfoName(infoName);
        
        return templatedate;
   }
    CC_SAFE_DELETE(templatedate);
    return NULL;


}
