//
//  SGActivityData.h
//  GameSanGuo
//
//  Created by zenghui on 14-2-21.
//
//

#ifndef __GameSanGuo__SGActivityData__
#define __GameSanGuo__SGActivityData__

#include "cocos2d.h"

USING_NS_CC;

class SGLabelInfo
{
    CC_SYNTHESIZE(int, m_labelId, LabelId);
    CC_SYNTHESIZE(int, m_labelValue, LabelValue);
    CC_SYNTHESIZE(ccColor4B, m_labelColor, LabelColor);

public:
    SGLabelInfo(int labelId,int labelValue,ccColor4B color)
    {
        m_labelId = labelId;
        m_labelValue = labelValue;
        m_labelColor = color;
    };
};
class SGLabelStringInfo
{
    CC_SYNTHESIZE(int, m_labelId, LabelId);
    CC_SYNTHESIZE(std::string, m_labelString, LabelString);
    CC_SYNTHESIZE(ccColor4B, m_labelColor, LabelColor);
    
public:
    SGLabelStringInfo(int labelId,std::string labelValue,ccColor4B color)
    {
        m_labelId = labelId;
        m_labelString = labelValue;
        m_labelColor = color;
    };
};


class SGBtnInfo
{
    CC_SYNTHESIZE(int, m_btnId, BtnId);
    CC_SYNTHESIZE(std::string, m_fontStr, FontStrString);
    CC_SYNTHESIZE(ccColor4B, m_fontColor, FontColor);
    CC_SYNTHESIZE(std::string, m_bgStr, BgStrString);
    CC_SYNTHESIZE(std::string, m_fgStr, FgStrString);
    CC_SYNTHESIZE(ccColor4B, m_imageColor, ImageColor);
    CC_SYNTHESIZE(int, m_btnState, BtnState);
    
public:
    SGBtnInfo(int btnId,std::string fontStr,ccColor4B fontColor,std::string bgStr,std::string fgStr,ccColor4B imageColor,int btnState)
    {
        m_btnId = btnId;
        m_fontStr = fontStr;
        m_fontColor = fontColor;
        m_bgStr = bgStr;
        m_fgStr = fgStr;
        m_imageColor = imageColor;
        m_btnState = btnState;
    };
};
class SGImageInfo
{
    CC_SYNTHESIZE(int, m_imageId, ImageId);
    CC_SYNTHESIZE(std::string, m_imageString, ImageString);
    CC_SYNTHESIZE(ccColor4B, m_imageColor, ImageColor);
    
public:
    SGImageInfo(int imageId,std::string imagelValue,ccColor4B color)
    {
        m_imageId = imageId;
        m_imageString = imagelValue;
        m_imageColor = color;
    };
};


class SGItemInfo
{
    CC_SYNTHESIZE(int, m_itemType, ItemType);
    CC_SYNTHESIZE(int, m_itemId, ItemId);
    CC_SYNTHESIZE(int, m_itemCount, ItemCount);
    CC_SYNTHESIZE(int, m_itemLevel, ItemLevel);
    CC_SYNTHESIZE(int, m_itemBtId, ItemBtId);
public:
    SGItemInfo():
    m_itemType(0),
    m_itemId (0),
    m_itemCount (0),
    m_itemLevel(0),
    m_itemBtId (0)
    {
        m_itemType = 0;
        m_itemId = 0;
        m_itemCount = 0;
        m_itemLevel = 0;
        m_itemBtId = 0;
    }
    SGItemInfo(int itemType,int itemId,int itemCount,int itemLevel,int itemBtId)
    {
        m_itemType = itemType;
        m_itemId = itemId;
        m_itemCount = itemCount;
        m_itemLevel = itemLevel;
        m_itemBtId = itemBtId;
    };
};

class SGActivityBriefData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(int, m_luaActivityId, LuaActivityId);
    CC_SYNTHESIZE(int, m_luaState, LuaState);
    CC_SYNTHESIZE(std::string, m_luaName, LuaName);
    CC_SYNTHESIZE(std::string, m_btnName, BtnName);
    CC_SYNTHESIZE(bool, m_notice, Notice);
    static SGActivityBriefData* create(int luaActivityId, int nState,const std::string& luaName,const std::string& btnName, bool notice)
    {
        SGActivityBriefData *activityData = new SGActivityBriefData();
        if (activityData) {
            activityData->setLuaActivityId(luaActivityId);
            activityData->setLuaState(nState);
            activityData->setLuaName(luaName);
            activityData->setBtnName(btnName);
            activityData->setNotice(notice);
            activityData->autorelease();
            
            return activityData;
        }
        CC_SAFE_DELETE(activityData);
        return NULL;
}
};
class SGActivityData : public cocos2d::CCObject
{
    //关卡是否显示
    CC_SYNTHESIZE(int, m_luaActivityId, LuaActivityId);
    CC_SYNTHESIZE(std::string, m_luaName, LuaName);
    CC_SYNTHESIZE(std::string, m_btnName, BtnName);
    CC_SYNTHESIZE(bool, m_notice, Notice);//活动页面是不是显示对应的红点
public:
    SGActivityData(void);
    ~SGActivityData(void);
    
    static SGActivityData* create(int luaActivityId, const std::string& luaName,const std::string& btnName, bool notice);
    void addLabelInfo(int labelId,int labelValue,ccColor4B color);
    void addBtnInfo(int btnId,std::string fontStr,ccColor4B fontColor,std::string bgStr,std::string fgStr,ccColor4B imageColor,int btnState);
    void addItemInfo(int itemType,int itemId,int itemCount,int itemLevel,int itemBtId);
    void addLabelStringInfo(int labelId,std::string labelValue,ccColor4B color);
    void addImageInfo(int imageId, std::string imageValue,ccColor4B color);
    int getLabelValueById(int labelId);
    int getBtnStateById(int btnId);
    void setBtnStateById(int btnId,int btnState);
    SGItemInfo *getItemInfo(int itemId,int itemType);
    void initData();
    
    int getLabelNum();
    int getBtnNum();
    int getItemNum();
    int getLabelStringnum();
    int getImagenum();
    
    std::vector<SGLabelInfo> m_labelInfo;
    std::vector<SGBtnInfo> m_btnInfo;
    std::vector<SGItemInfo> m_ItemInfo;
    std::vector<SGLabelStringInfo> m_LabelStringInfo;
    std::vector<SGImageInfo> m_imageInfo;
};

class SGActivityUpdateData : public cocos2d::CCObject
{
    //关卡是否显示
    CC_SYNTHESIZE(int, m_updatetime, ActivityUpdateTime);
    CC_SYNTHESIZE(CCLabelTTF*, m_label, Label);
    CC_SYNTHESIZE(int, m_activityIndex, ActivityIndex);
public:
    SGActivityUpdateData(void);
    ~SGActivityUpdateData(void);
    
    static SGActivityUpdateData* create(int updatetime,CCLabelTTF* label,int index);
};



class SGItemCondition : public CCObject
{
    
CC_SYNTHESIZE(int, m_cardtype, Cardtype);
CC_SYNTHESIZE(int, m_cardId,CardId);
CC_SYNTHESIZE(int, m_cardlevel, CardLevel);
CC_SYNTHESIZE(int, m_cardNum, CardNum);
CC_SYNTHESIZE(int, m_cardState, CardState);
public:
    SGItemCondition();
    
};

class SGItemResourt : public CCObject
{
    CC_SYNTHESIZE(int, m_cardtype, Cardtype);
    CC_SYNTHESIZE(int, m_cardId,CardId);
    CC_SYNTHESIZE(int, m_cardlevel, CardLevel);
    CC_SYNTHESIZE(int, m_cardNum, CardNum);
public:
    SGItemResourt();
};




class SGACtivityBar : public cocos2d::CCObject
{
    CC_SYNTHESIZE(int, m_barId, BarId);
    CC_SYNTHESIZE(int, m_barbtnstate, BarBtnState);
    std::vector<SGItemCondition> m_itemcondition;
    std::vector<SGItemResourt> m_itemresourt;
    CC_SYNTHESIZE(std::string, m_description, Description);
public:
    SGACtivityBar();
};



class SGActivityLuaTemplateData
{
    
    CC_SYNTHESIZE(int, m_luaActivityId, LuaActivityId);
    CC_SYNTHESIZE(int, m_btntype, BtnId);
    CC_SYNTHESIZE(int, m_templatetype, TemplateType);
    CC_SYNTHESIZE(std::string, m_luaName, LuaName);
    CC_SYNTHESIZE(std::string, m_btnName, BtnName);
    CC_SYNTHESIZE(std::string, m_titleName, TitleName);
    CC_SYNTHESIZE(std::string, m_infoName, InfoName);
    std::vector<SGACtivityBar> m_item;
public:
    SGActivityLuaTemplateData(void);
    ~SGActivityLuaTemplateData(void);
    static SGActivityLuaTemplateData* create(int luaActivityId,int btnid, int templatetype, const std::string& luaName,const std::string& btnName,const std::string& titleName,const std::string& infoName);
};




#endif /* defined(__GameSanGuo__SGActivityData__) */
