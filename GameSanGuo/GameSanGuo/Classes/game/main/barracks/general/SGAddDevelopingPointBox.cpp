#include "SGAddDevelopingPointBox.h"
#include "SGDevelopingOfficerLayer.h"
#include "SGMainLayer.h"
#include "SGMainManager.h"
#include "SGNotificationCenter.h"
#include "SGOfficerCard.h"
#include "SGDrawCards.h"
#include "SGShowString.h"
#include "SGTools.h"
#include "Developing.pb.h"

SGAddDevelopingPointBox::SGAddDevelopingPointBox()
: m_card(NULL)
, m_delegate(NULL)
, m_pointInfo(NULL)
, m_nowAttribute(NULL)
, m_developingPointLab(NULL)
, m_canAddPointLab(NULL)
, m_hasNum(NULL)
, m_NeedRefreshLayerLab(false)
{
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(false , false);
}

SGAddDevelopingPointBox::~SGAddDevelopingPointBox()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this,MSG_ADD_DEVELOPING_POINT );
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true , false);
}

SGAddDevelopingPointBox * SGAddDevelopingPointBox::create(SGOfficerCard *card  , SGDevelopingOfficerLayer * delegate, DevePointInfo * pointInfo , NowAttribute * nowAttribute)
{
    SGAddDevelopingPointBox * ret = new SGAddDevelopingPointBox();
    if(ret && ret->init(NULL, box_addDevelopingPointBox, CCSize(600,600),false,false ))
    {
        ret->m_card = card;
        ret->m_delegate = delegate;
        ret->m_pointInfo = pointInfo;
        ret->m_nowAttribute = nowAttribute;
        ret->initView();
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void SGAddDevelopingPointBox::initView()
{
    //listen msg
    SGNotificationCenter::sharedNotificationCenter()->addObserver( MSG_ADD_DEVELOPING_POINT, this, callfuncO_selector(SGAddDevelopingPointBox::onUseResponse));
    
    //-----------------------ui
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCLayerColor * colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 200), winSize.width, winSize.height);
    colorLayer->setPosition(CCPointZero);
    colorLayer->setTouchEnabled(false);
    this->addChild(colorLayer , -10);
    
    CCScale9Sprite * outBg = CCScale9Sprite::createWithSpriteFrameName("Tips.png");
    outBg->setPreferredSize(CCSize(580 , 580));
    outBg->setPosition(winSize/2);
    this->addChild(outBg);
    
    CCScale9Sprite * inBg = CCScale9Sprite::createWithSpriteFrameName("tipsinside.png");
    inBg->setPreferredSize(CCSize(500 , 390));
    inBg->setPosition(outBg->getPosition());
    this->addChild(inBg);
    
    SGCCLabelTTF * titleText = SGCCLabelTTF::create(str_developing_officer_12, FONT_PANGWA, 40 , COLOR_LIGHT_YELLOW);
    titleText->setPosition(ccp( outBg->getContentSize().width*0.5 , outBg->getContentSize().height - titleText->getContentSize().height*0.5 - 30));
    outBg->addChild(titleText);
    
    float fixPos1 = 15;
    float fixPos2 = 30;
    
    //-------------------------上半部份背景
    CCScale9Sprite * upBg = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png");
    upBg->setPreferredSize(CCSize(470 , 160));
    upBg->setPosition(ccp(inBg->getContentSize().width*0.5 , inBg->getContentSize().height*0.5 + 5 + upBg->getContentSize().height*0.5));
    inBg->addChild(upBg);
    
    CCSprite * officer = SGDrawCards::createSimpleSprite(m_card->getItemId(), 6, sg_developingOfficerLayer);
    officer->setPosition(ccp(fixPos1 + officer->getContentSize().width*0.5, upBg->getContentSize().height*0.5));
    upBg->addChild(officer);
    
    //潜力点
    
    CCScale9Sprite * devPointBg = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png" , CCRect(7,7,86,86));
    devPointBg->setPreferredSize(CCSize(200 , 45));
    devPointBg->setAnchorPoint(ccp(0, 0.5));
    devPointBg->setPosition(ccp(officer->getContentSize().width*0.5 + officer->getPositionX() + fixPos2, upBg->getContentSize().height*0.5));
    upBg->addChild(devPointBg);
    
    m_developingPointLab = SGCCLabelTTF::create(CCString::createWithFormat(str_developing_officer_1 , m_pointInfo->a[0])->getCString(), FONT_PANGWA, 28);
    m_developingPointLab->setAnchorPoint(ccp(0, 0.5));
    m_developingPointLab->setPosition(ccp(8, devPointBg->getContentSize().height*0.5));
    devPointBg->addChild(m_developingPointLab);
    
    SGCCLabelTTF * name = SGCCLabelTTF::create(m_card->getOfficerName()->getCString(), FONT_PANGWA, 32 ,SGTools::getColorByQuality(m_card->getCurrStar()) );
    name->setAnchorPoint(ccp(0, 0.5));
    name->setPosition(ccp(devPointBg->getPositionX() , devPointBg->getPositionY() + devPointBg->getContentSize().height*0.5 + name->getContentSize().height*0.5  + 5));
    upBg->addChild(name);
    
    SGCCLabelTTF * level = SGCCLabelTTF::create(CCString::createWithFormat("Lv:%d" , m_card->getCurrLevel())->getCString(), FONT_PANGWA, 32);
    level->setPosition(ccp(name->getPosition().x + name->getContentSize().width + 20 + level->getContentSize().width*0.5, name->getPosition().y));
    upBg->addChild(level);
    
    //可增加潜力点
    CCScale9Sprite * addPointBg = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png" , CCRect(7,7,86,86));
    addPointBg->setPreferredSize(CCSize(320 , devPointBg->getContentSize().height));
    addPointBg->setAnchorPoint(ccp(0, 0.5));
    addPointBg->setPosition(ccp(devPointBg->getPositionX(), devPointBg->getPositionY() - devPointBg->getContentSize().height - 4));
    upBg->addChild(addPointBg);
    
    m_canAddPointLab = SGCCLabelTTF::create(CCString::createWithFormat(str_developing_officer_31 , m_pointInfo->a[2] - m_pointInfo->a[1] )->getCString(), FONT_PANGWA, 28);
    m_canAddPointLab->setAnchorPoint(ccp(0,0.5));
    m_canAddPointLab->setPosition(ccp(8, addPointBg->getContentSize().height*0.5));
    addPointBg->addChild(m_canAddPointLab);
    
    //------------------------下半部份背景
    CCScale9Sprite * downBg = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png");
    downBg->setPreferredSize(CCSize(470 , 160));
    downBg->setPosition(ccp(inBg->getContentSize().width*0.5 , inBg->getContentSize().height*0.5 - 5 - downBg->getContentSize().height*0.5));
    inBg->addChild(downBg);
    
    m_itemId = 352004;
    
    //道具图标
    CCSprite * prop = SGDrawCards::createSimpleSprite(m_itemId, 15, sg_developingOfficerLayer);
    prop->setPosition(officer->getPosition());
    downBg->addChild(prop);
    
    CCScale9Sprite * desBg = CCScale9Sprite::createWithSpriteFrameName("ofc_100x100.png");
    desBg->setPreferredSize(CCSize(320 , 110));
    desBg->setAnchorPoint(ccp(0, 0.5));
    desBg->setPosition(ccp( devPointBg->getPositionX(), downBg->getContentSize().height*0.5 + 15));
    downBg->addChild(desBg);
    
    SGConsumableDataModel * dataModel = SGStaticDataManager::shareStatic()->getConsumableById(m_itemId);
    int propNum = getConsumeNumById(m_itemId);
    //道具功能描述
    SGCCLabelTTF * propDes = SGCCLabelTTF::create(dataModel->getConsumeDesc()->getCString(), FONT_BOXINFO, 30  , CCSize(280 , 115) , kCCTextAlignmentLeft , kCCVerticalTextAlignmentCenter);
    propDes->setPosition(ccpAdd(desBg->getContentSize()/2  , ccp(0, 0)));
    desBg->addChild(propDes);
    
    m_hasNum = SGCCLabelTTF::create(CCString::createWithFormat(str_developing_officer_13 , propNum)->getCString(), FONT_BOXINFO, 24 , ccGREEN);
    m_hasNum->setAnchorPoint(ccp(0, 0.5));
    m_hasNum->setPosition(ccp( 5 + desBg->getPositionX(), desBg->getPositionY() - desBg->getContentSize().height*0.5 - m_hasNum->getContentSize().height*0.5 - 10));
    downBg->addChild(m_hasNum);
    
    SGButton * useOne = SGButton::createFromLocal("box_btn_newbg.png" ,str_developing_officer_15 , this, menu_selector(SGAddDevelopingPointBox::useOneCallback) , CCPointZero , FONT_PANGWA , ccWHITE , 32);
    useOne->setPosition(ccp(outBg->getPositionX(), outBg->getPositionY() - outBg->getContentSize().height*0.5 + useOne->getContentSize().height*0.5 + 20));
    addBtn(useOne);
    
    SGButton * useTen = SGButton::createFromLocal( "box_btn_newbg.png",str_developing_officer_14 , this, menu_selector(SGAddDevelopingPointBox::useTenCallback) , CCPointZero , FONT_PANGWA , ccWHITE , 32);
    useTen->setPosition(ccp(useOne->getPositionX() - 180, useOne->getPositionY()));
    addBtn(useTen);
    
    SGButton * close = SGButton::createFromLocal("box_btn_newbg.png" , str_developing_officer_16, this, menu_selector(SGAddDevelopingPointBox::closeCallback) , CCPointZero , FONT_PANGWA , ccWHITE , 32);
    close->setPosition(ccp(useOne->getPositionX() + 180, useOne->getPositionY()));
    addBtn(close);
    
}

void SGAddDevelopingPointBox::useOneCallback()
{
    this->sendUseRequest(1);
}

void SGAddDevelopingPointBox::useTenCallback()
{
    this->sendUseRequest(10);
}

void SGAddDevelopingPointBox::sendUseRequest(int num)
{
    //condition
    if( m_pointInfo->a[1] >= m_pointInfo->a[2])
    {
        SGMainManager::shareMain()->showMessage(str_developing_officer_28);
        return ;
    }

    if(getConsumeNumById(m_itemId) < num)
    {
        SGMainManager::shareMain()->showMessage(str_developing_officer_23);
        return ;
    }
    
        
    //send msg
    main::UseDevelopItemRequest * request = new main::UseDevelopItemRequest();
    request->set_ssid(m_card->getSsid());
    request->set_itemnum(num);
    SGSocketClient::sharedSocketClient()->send( MSG_ADD_DEVELOPING_POINT, request);
}

void SGAddDevelopingPointBox::onUseResponse(CCObject * sender)
{
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    if(sr)
    {
        main::UseDevelopItemResponse * res = (main::UseDevelopItemResponse*)sr->m_msg;
        if(res)
        {
            if(1==res->state())
            {
                main::DevPointDetail devPointDetail = res->pointdetail();
                m_pointInfo->a[0] = devPointDetail.devpoint();
                m_pointInfo->a[1] = devPointDetail.adddevpoint();
                m_pointInfo->a[2] = devPointDetail.maxdevpoint();
                
                main::DevOfficerDetail devOfficerDetail = res->officerdetail();
                m_nowAttribute->a[0] = devOfficerDetail.extatk();
                m_nowAttribute->a[1] = devOfficerDetail.extdef();
                m_nowAttribute->a[2] = devOfficerDetail.exthp();
                m_nowAttribute->a[3] = devOfficerDetail.extsp();
                
                m_nowAttribute->b[0] = devOfficerDetail.maxatk();
                m_nowAttribute->b[1] = devOfficerDetail.maxdef();
                m_nowAttribute->b[2] = devOfficerDetail.maxhp();
                m_nowAttribute->b[3] = devOfficerDetail.maxsp();

                m_NeedRefreshLayerLab = true;
                
                SGMainManager::shareMain()->showMessage(str_developing_officer_29);
                this->refreshLabels();
            }
            else
            {
                SGMainManager::shareMain()->showMessage(str_developing_officer_30);
            }
        }
    }
}

void SGAddDevelopingPointBox::refreshLabels()
{
    m_developingPointLab->setString(CCString::createWithFormat(str_developing_officer_1 , m_pointInfo->a[0])->getCString());
    m_canAddPointLab->setString(CCString::createWithFormat(str_developing_officer_31 , m_pointInfo->a[2] - m_pointInfo->a[1] )->getCString());
    m_hasNum->setString(CCString::createWithFormat(str_developing_officer_13 , getConsumeNumById(m_itemId))->getCString());
}

int SGAddDevelopingPointBox::getConsumeNumById(int itemId)
{
    int num =0;
    CCArray * arr = SGPlayerInfo::sharePlayerInfo()->getConsumableCards();
    for(int i=0 ; i<arr->count() ; ++i)
    {
        SGConsumableCard * temp = dynamic_cast<SGConsumableCard *>(arr->objectAtIndex(i));
        if(temp->getItemId() == itemId)
        {
            num = temp->getCurrNum();
            break;
        }
    }
    return num;
}

void SGAddDevelopingPointBox::closeCallback()
{
    //刷新潜力点
    if(m_NeedRefreshLayerLab)
        m_delegate->refreshView();
    
    SGBaseBox::boxClose();
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer)
        mainLayer->setIsCanTouch(true,false);
    if (nowLayer)
        nowLayer->setIsCanTouch(true);
}


//-------------------------------------------file end.
