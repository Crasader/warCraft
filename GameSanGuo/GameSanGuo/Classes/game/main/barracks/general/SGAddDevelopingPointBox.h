//
//   武将培养--增加潜力点box
//     资源均在layer中绑定。这里直接使用
//2014-12-17

#ifndef __GameSanGuo__SGAddDevelopingPointBox__
#define __GameSanGuo__SGAddDevelopingPointBox__

#include "SGBaseBox.h"

class SGCCLabelTTF;
class SGOfficerCard;
class SGDevelopingOfficerLayer;
struct DevePointInfo;
struct NowAttribute;
class SGAddDevelopingPointBox : public SGBaseBox
{
public:
    /*add=已经增加点数  max=上限*/
    static SGAddDevelopingPointBox * create(SGOfficerCard * card , SGDevelopingOfficerLayer *, DevePointInfo * ,NowAttribute * );
    
    
    ~SGAddDevelopingPointBox();
protected:
    SGAddDevelopingPointBox();
private:
    SGDevelopingOfficerLayer * m_delegate;
    SGOfficerCard * m_card;
    DevePointInfo * m_pointInfo;
    NowAttribute * m_nowAttribute;
    bool m_NeedRefreshLayerLab;
    
    
    void initView();
    void useOneCallback();
    void useTenCallback();
    void closeCallback();
    
    /*发送使用潜力酒的消息*/
    void sendUseRequest(int num);
    
    //使用道具换取潜力点的消息响应
    void onUseResponse(CCObject *);
    
    void refreshLabels();
    
    int getConsumeNumById(int itemId);
    
private:
    int m_itemId;
    SGCCLabelTTF * m_hasNum;
    SGCCLabelTTF * m_developingPointLab;
    SGCCLabelTTF * m_canAddPointLab;
};
#endif

//----------------------------file end.