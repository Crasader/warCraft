//
//   武将培养
//
//2014-12-17

#ifndef __GameSanGuo__SGDevelopingOfficerLayer__
#define __GameSanGuo__SGDevelopingOfficerLayer__

#include "SGBaseLayer.h"

class SGButton;
class SGCCLabelTTF;
class SGOfficerCard;
class SGTouchLayer;

struct PreviewAttribute
{
    /*附加属性*/
    int a[4];
    
    PreviewAttribute()
    {
        memset(a, 0, 4*sizeof(int));
    }
};

struct NowAttribute
{
    /*附加属性*/
    int a[4];
    /*最大附加*/
    int b[4];
    
    NowAttribute()
    {
        memset(a,0,4*sizeof(int));
        memset(b,0,4*sizeof(int));
    }
};

struct DevePointInfo
{
    /*当前点数 、 已增加点数、最大可增加*/
    int a[3];
    DevePointInfo()
    {
        memset(a, 0, 3*sizeof(int));
    }
};

struct DeveCost
{
    int normal_material;
    int normal_coin;
    int careful_material;
    int careful_gold;
    DeveCost()
    {
        normal_material=0;
        normal_coin=0;
        careful_material=0;
        careful_gold=0;
    }
};

struct DevelopingOfficerData
{
    bool isSaved; /*是否培养之后未保存，只有在刚进入该页面时使用该字段*/
    
    /*普通培养 和 精心培养需要的材料数量 和 金钱*/
    DeveCost deveCost;
    
    PreviewAttribute preview;
    NowAttribute nowAttribute;
    DevePointInfo pointInfo;
    
    DevelopingOfficerData():isSaved(true)
    {
        
    }
    ~DevelopingOfficerData()
    {
        
    }
};


class SGDevelopingOfficerLayer : public SGBaseLayer
{
public:
    /*析构时del DevelopingOfficerData */
    static SGDevelopingOfficerLayer * create(const DevelopingOfficerData & , SGOfficerCard * , int generinfoType);
    
    /*弹出框关闭后刷新潜力点*/
    virtual void refreshView();
    
    /*培养一次 十一的消息响应*/
    void developingResponse(CCObject *);
    /*保存培养消息响应*/
    void saveDevelopingResponse(CCObject *);
        
    ~SGDevelopingOfficerLayer();
protected:
    SGDevelopingOfficerLayer();
    
private:
    /*武将数据*/
    PreviewAttribute m_preview;
    NowAttribute m_nowAttribute;
    DevePointInfo m_pointInfo;
    DeveCost m_deveCost;
    /*武将装备对攻防血速的加成*/
    int equipsAdd[4];
    /**/
    SGOfficerCard * m_card;
    
    //用于返回generinfolayer的type
    int m_generinfoType;
    
    /*false=培养页面 ， true=培养结果预览页面（保存页面）*/
    bool m_state;
    
    /*培养模式：1=普通 2=精心*/
    int m_developingModel;
    
    /**/
    void initView();
    
    /*培养结果预览页面、未开始培养 展示增加属性lab*/
    void createLabels(CCNode *  , CCPoint , float delayPosY);
    
    /*在本页操作后刷新页面*/
    void refreshAfterOperator();
    
    /*刷新攻防血速*/
    void refreshAttribute();
    
    /*培养一次、十次的回调*/
    void deveOnce();
    void deveTen();
    void sendDevelopingRequest(int num);
    
    /*保存、取消培养预览的回调*/
    void saveCallback();
    void cancelCallback();
    
    /*选择普通、精心模式的回调*/
    void choseNormalCallback(CCObject *);
    void choseCarefunCallback(CCObject *);
    
    /*增加武将潜力点的回调*/
    void addDevelopingPointCallback();
    void showAddBox();
    
    /*规则按钮的回调*/
    void ruleCallback();
    
    /*操作之后更改按钮的状态*/
    void setBtnEnabled(bool enable);
    
    /*返回武将详情*/
    void backHandler();
    
    /*动画*/
    void playAnimate();
    
    /*点击培养丹*/
    void showComsumeInfo();
    
    /*本页面所有触摸*/
    void setIsCanTouch(bool enable);
private:
    //防止多次点击
    bool m_beginning;
    
    //培养丹id
    int m_pei_yang_dan_id;
    //表示培养模式的亮点
    CCSprite* lightPoint;
    
    SGButton * m_onceBtn;
    SGButton * m_tenBtn;
    SGButton * m_saveBtn;
    SGButton * m_cancelBtn;
    
    SGCCLabelTTF * m_developingPointLab;
    SGCCLabelTTF * m_coinLab;/*拥有*/
    SGCCLabelTTF * m_goldLab;
    SGCCLabelTTF * m_coinNeedLab;/*培养需要*/
    SGCCLabelTTF * m_goldNeedLab;
    SGCCLabelTTF * m_materialNeedLab;
    SGCCLabelTTF * m_materialNeedLab2;
    
    SGTouchLayer * m_touchLayer;//回调
};
#endif

//----------------------------file end.