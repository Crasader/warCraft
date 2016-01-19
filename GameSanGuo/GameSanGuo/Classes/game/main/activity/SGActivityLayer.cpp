//
//  SGActivityLayer.cpp
//  GameSanGuo
//
//  Created by dongboqian on 14-1-7.
//
//

#include "SGActivityLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "cocos-ext.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "SGStaticDataManager.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGActivityItem.h"
#include "SGActivitySubLayer.h"
#include "CCLuaEngine.h"
#include "SGLuaSocketSender.h"
#include "ShopModule.pb.h"
#include "SGLuaDataProto.h"
#include "LuaActivityModule.pb.h"
#include "SGActGiftInfoBox.h"
#include "SGStringConfig.h"
#include "SGCommonInfoBox.h"
#include "SGCantAdvanceBox.h"
#include "SGGuideManager.h"

SGActivityLayer::SGActivityLayer()
:reallength(0)
,_nowPage(1)
,offset(0)
,m_UpdateTimeData(NULL),
luanum(0),
lualayerdic(NULL),
_totalPage(0),
_layer(NULL),
left(NULL),
rgt(NULL),
scrollview(NULL),
scrollview2(NULL),
_allData(NULL),
_menu(NULL),
m_pActivitydata(NULL),
m_pCurrPage(NULL),
activityData(NULL),
m_pLayerCurr(NULL),
m_nTAG(0)
{
   
}
SGActivityLayer::~SGActivityLayer()
{
   ResourceManager::sharedInstance()->unBindLayerTexture(sg_luaactivity);
   SGMainManager::shareMain()->getMainLayer()->openByActivity = false;
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true);
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
//    notification->removeObserver(this, MSG_STORE_ENTER);
   
    notification->removeObserver(this, MSG_LUA_ACTIVITY_PAGE);
    notification->removeObserver(this, MSG_LUA_SET_ELEMENT);
    notification->removeObserver(this, MSG_LUA_GETGIFT);
    notification->removeObserver(this, "showFirstChagreItem");
    notification->removeObserver(this, "sendRewardReq");
    notification->removeObserver(this, "buyGrowPlan");
    notification->removeObserver(this, MSG_ACTIVITY_BUY_GROW_PLAN);
    
    
}

SGActivityLayer *SGActivityLayer::create(CCArray *array,CCObject * obj)
{
    SGActivityLayer *activityLayer = new SGActivityLayer();
    if (activityLayer && activityLayer->init(NULL, sg_luaactivity))
    {
        activityLayer->initActivityData(array);
        activityLayer->m_pActivitydata = obj;
        activityLayer->m_pActivitydata->retain();
        activityLayer->initView();
        activityLayer->autorelease();
        if (activityLayer->luanum > 0) {
             return activityLayer;
        }
    }
    CC_SAFE_DELETE(activityLayer);
    return NULL;
}
void SGActivityLayer::sendRewardReqListener(CCObject *obj)
{
    if(obj)
    {
        CCNode* layer = (CCNode*)obj;
        int tag  = layer->getTag();
        main::ActivityGetRewardRequest *request = new main::ActivityGetRewardRequest();
        SGActivityData *lua =((SGActivityData *)m_pActivitydata);
        int Id = lua->getLuaActivityId();
        request->set_btnid(tag);
        request->set_childtag(Id);
        SGSocketClient::sharedSocketClient()->send(MSG_LUA_GETGIFT, request);
    }

}
void SGActivityLayer::initMsg()
{
    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(false);
    SGNotificationCenter::sharedNotificationCenter()->addObserver("showFirstChagreItem",this,callfuncO_selector(SGActivityLayer::showFirstChagreItem));
    SGNotificationCenter::sharedNotificationCenter()->addObserver("sendRewardReq",this,callfuncO_selector(SGActivityLayer::sendRewardReqListener));
    SGNotificationCenter::sharedNotificationCenter()->addObserver("buyGrowPlan",this,callfuncO_selector(SGActivityLayer::buyGrowPlan));
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_ACTIVITY_BUY_GROW_PLAN,this,callfuncO_selector(SGActivityLayer::buyGrowPlanResponse));
}

void SGActivityLayer::showFirstChagreItem(CCObject* selectItem)
{
   
    if (selectItem == NULL) {
        return;
    }
    int tag = ((CCNode*)selectItem)->getTag();
    
    
    //int activityId = ((CCNode*)selectItem)->getParent()->getTag();
    SGBaseMilitaryCard * chatShowItem = NULL;
    int type = 0;

        SGActivityData*lua =(SGActivityData *)m_pActivitydata;
    
            int elementNum = lua->getItemNum();
            for (int k=0; k<elementNum; k++) {
                if (lua->m_ItemInfo[k].getItemBtId() == tag) {
                    switch (lua->m_ItemInfo[k].getItemType()) {
                        case 0:
                        {
                            //元宝
//                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("元宝x%d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_BOXINFO, 20);
                        }
                            break;
                        case 1:
                        {
                            //铜钱
//                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("铜钱x%d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_BOXINFO, 20);
                        }
                            break;
                        case 2:
                        {
                            //Pvp积分
//                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("Pvp积分x%d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_BOXINFO, 20);
                        }
                            break;
                        case 3:
                        {
                            //军功
//                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("军功x%d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_BOXINFO, 20);
                        }
                            break;
                        case 4:
                        {
                            //小喇叭
//                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("小喇叭x%d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_BOXINFO, 20);
                        }
                            break;
                        case 5:
                        {
                            //炼丹秘方
//                            SGCCLabelTTF *itemName = SGCCLabelTTF::create(CCString::createWithFormat("炼丹秘方x%d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_BOXINFO, 20);
                        }
                            break;
                        case 6:
                        {
                            //武将
                            SGOfficerCard *officerCard = new SGOfficerCard();
                            chatShowItem = officerCard;
                            type = 1;
                            CCDictionary *dicc = SGCardAttribute::getValue(lua->m_ItemInfo[k].getItemLevel(), lua->m_ItemInfo[k].getItemId());
                            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(lua->m_ItemInfo[k].getItemId());
                            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(lua->m_ItemInfo[k].getItemId());
//                            if (temp->getOfficerExpType()->isEqual(CCString::create("A"))) {
//                                type = 0;
//                            }
//                            else if (temp->getOfficerExpType()->isEqual(CCString::create("B"))) {
//                                type = 1;
//                            }
//                            else {
//                                type = 2;
//                            }
                            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type,lua->m_ItemInfo[k].getItemLevel()+1);
                            
                            officerCard->setItemId(lua->m_ItemInfo[k].getItemId());
                            officerCard->setExpRate(temp->getOfficerExpRate());
                            officerCard->setMaxStar(temp->getOfficerNextId());
                            officerCard->setCurrLevel(lua->m_ItemInfo[k].getItemLevel());
                            officerCard->setMaxExp(expdata->getExpValue());
                            officerCard->setCurrStar(temp->getOfficerCurrStarLevel());
                            officerCard->setOfficerName(temp->getOfficerName());
                            officerCard->setMaxLevel(temp->getOfficerMaxLevel());
                            officerCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                            officerCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                            officerCard->setRound(temp->getOfficerRound());
                            officerCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
                            officerCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                            officerCard->setGoverning(temp->getOfficerGoverning());
                            officerCard->setRace(temp->getOfficerRace());
                            officerCard->setItemType(temp->getOfficerType());
                            //转生最大星级
                            officerCard->setUpgradestar(temp->getUpgradestar());
                            //转生等级
                            officerCard->setUpgradelevel(temp->getUpgradelevel());
                            //武将计最大等级
                            officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
                            //主将计最大等级
                            officerCard->setLordMaxLevel(temp->getLordMaxLevel());
                            //性别和缘分
                            officerCard->setGender((OfficerGender) temp->getOfficerGender());
                            
                            std::vector<int> reFateList =  temp->getOfficerFateList();
                            for (int i=0; i<reFateList.size(); i++) {
                                reFateList[i] =  reFateList[i]*10 + 1;
                            }
                            officerCard->setFateList(reFateList);
                            
                            //转生次数
                            officerCard->setAdNum(temp->getAdvNum());
                            //武将原型ID
                            officerCard->setProtoId(temp->getOfficerProtoId());
                            //速度修正系数
                            officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
                            
                            officerCard->setHeadSuffixNum(temp->getIconId());//12.05
                            
                            if (strcmp(temp->getOfficerGeneralsSkill()->getCString(), "0") &&temp->getOfficerGeneralsSkill()->length()>3)
                            {
                                std::string generalskill(temp->getOfficerGeneralsSkill()->getCString(),3,temp->getOfficerGeneralsSkill()->length() - 3);
                                officerCard->setOfficerSkil(atoi(generalskill.c_str()));
                            }
                            if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
                            {
                                std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
                                officerCard->setLordSkill(atoi(lordskill.c_str()));            
                            }
    
                            officerCard->autorelease();
                        }
                            break;
                        case 7:
                        {
                            SGEquipCard *equipCard = new SGEquipCard();
                            chatShowItem = equipCard;
                            type = 2;
                            CCDictionary *dicc = SGCardAttribute::getValue(lua->m_ItemInfo[k].getItemLevel(), lua->m_ItemInfo[k].getItemId());
                            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(lua->m_ItemInfo[k].getItemId());
                            int type = SGStaticDataManager::shareStatic()->getStrengEquipType(lua->m_ItemInfo[k].getItemId());
//                            if (temp->getEquipExpType()->isEqual(CCString::create("A"))) {
//                                type = 0;
//                            }
//                            else if (temp->getEquipExpType()->isEqual(CCString::create("B"))) {
//                                type = 1;
//                            }
//                            else {
//                                type = 2;
//                            }
                            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, lua->m_ItemInfo[k].getItemLevel()+1);
                            //转生等级
                            equipCard->setUpgradelevel(temp->getUpgradelevel());
                            //转生次数
                            equipCard->setAdNum(temp->getAdvNum());
                            //转生最大星级
                            equipCard->setUpgradestar(temp->getUpgradestar());
                            //装备原型ID
                            equipCard->setProtoId(temp->getEquipProtoId());
                            equipCard->setItemId(lua->m_ItemInfo[k].getItemId());
                            //            equipCard->setSetNum(cardInfo.setnum());
                            equipCard->setCurrLevel(lua->m_ItemInfo[k].getItemLevel());
                            equipCard->setCurrExp(0);

                            equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                            equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                            equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                            equipCard->setMaxExp(expdata->getExpValue());
                            equipCard->setRound(temp->getEquipRound());
                            equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
                            equipCard->setExpRate(temp->getEquipExpRate());
                            equipCard->setCurrStar(temp->getEquipCurrStarLevel());
                            equipCard->setGoverning(temp->getEquipGoverning());
                            equipCard->setItemType(temp->getEquipType());
                            equipCard->setMaxStar(temp->getEquipNextId());
                            equipCard->setOfficerName(temp->getEquipName());
                            equipCard->setMaxLevel(temp->getEquipMaxLevel());
                            
                            equipCard->setHeadSuffixNum(temp->getIconId());//12.06
                            
                            if (temp->getEquipNextId()) {
                                equipCard->setCost(temp->getEquipCost());
                            }
                            equipCard->autorelease();

                        }
                            break;
                        case 9:
                        {
                            SGPropsCard *propCard = new SGPropsCard();
                            chatShowItem = propCard;
                            type = 4;
                            SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(lua->m_ItemInfo[k].getItemId());
                            
                            propCard->setItemId(lua->m_ItemInfo[k].getItemId());
                            propCard->setSell(prop->getpropsSell());
                            propCard->setValue(prop->getpropsValue());
                            propCard->setRace(prop->getPropsCountry());
                            propCard->setHeadSuffixNum(prop->getIconId());
                            propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
                            propCard->setType(prop->getPropsPropsType());
                            propCard->setItemType(prop->getPropsType());
                            propCard->setCurrStar(prop->getPropsCurrStarLevel());
                           
                            propCard->autorelease();
                        }
                            break;
                        case 10:
                        {
                            SGPiecesCard*piecesCard=new SGPiecesCard();
                            chatShowItem = piecesCard;
                            type = 5;
                            
                            SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(lua->m_ItemInfo[k].getItemId());
                            
                            piecesCard->setItemId(lua->m_ItemInfo[k].getItemId());
                            piecesCard->setSell(pieces->getPiecesPrice());//价格
                            piecesCard->setOfficerName(pieces->getPiecesName());//名称
                            piecesCard->setPiecesName(pieces->getPiecesName());
                            piecesCard->setItemType(pieces->getPiecesType());
                            //   piecesCard->setItemType(itemType);//类型
                            piecesCard->setCurrLevel(pieces->getStarLevel());//星级
                            piecesCard->setPiecesDesc(pieces->getPiecesDesc());//碎片描述
                            piecesCard->setCanUseCommon(pieces->getCommonPieces());//可使用的通用碎片数量
                            piecesCard->setCallCardId(pieces->getCallingCardId());//设置碎片合成后召唤的武将id
                            piecesCard->setMaxNum(pieces->getMaxPiecesNum());//碎片最大数量
                            piecesCard->setSummonNeed(pieces->getcallNeedPieces());//召唤所需要的碎片的数量
                            piecesCard->setCurrStar(pieces->getStarLevel());
                            
                            piecesCard->autorelease();
                        }
                            break;
                        case 15:
                        {
                            SGConsumableCard *consumeCard = new SGConsumableCard();
                            chatShowItem = consumeCard;
                            type = 6;
                            SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(lua->m_ItemInfo[k].getItemId());
                            
                            consumeCard->setItemId(lua->m_ItemInfo[k].getItemId());
                            consumeCard->setCurrStar(consumeModel->getConsumeStarLvl());
                            consumeCard->setSell(consumeModel->getConsumePrice());
                            consumeCard->setconsumableDesc(consumeModel->getConsumeDesc());
                            consumeCard->setCanUseDriving(consumeModel->getCanUseDriving());
                            consumeCard->setOfficerName(consumeModel->getConsumeName());
                            consumeCard->setMoneyType(consumeModel->getMoneyType());
                            consumeCard->setItemType(consumeModel->getConsumeType());
                            consumeCard->setHeadSuffixNum(consumeModel->getIconId());
                            consumeCard->setLimitAndNum(consumeModel->getLimitAndNum());
                            consumeCard->setPicId(consumeModel->getConsumePicId());
                            
                            consumeCard->autorelease();
                        }
                            break;
                        case 17://材料
                        {
                            SGMaterialCard *materialCard = new SGMaterialCard();
                            chatShowItem = materialCard;
                            type = 7;//作用未知，只是按上面递增
                            
                            SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(lua->m_ItemInfo[k].getItemId());
                            
                            materialCard->setItemId(lua->m_ItemInfo[k].getItemId());
                            materialCard->setHeadSuffixNum(materialModel->getMaterialIconId());
                            materialCard->setMaterialBorderColor(materialModel->getMaterialBorderColor());
                            materialCard->setMaterialPileLimit(materialModel->getMaterialPileLimit());
                            materialCard->setOfficerName(materialModel->getMaterialName());
                            materialCard->setSell(materialModel->getMaterialSoldPrice());
                            
                            materialCard->autorelease();
                        }
                            break;
                            
                        case -1:
                            break;
                        default:
                            break;
                    }
                    if (chatShowItem)
                    {
                        //((CCNode*)selectItem)->getParent()->setVisible(false);
                        //修改为简略弹窗。
                        
                        //如果在引导，不响应这个弹窗
                        if (SGGuideManager::shareManager()->isGuide)
                        {
                            return ;
                        }
                        
                        
                        SGMainManager::shareMain()->showCommonInfoBoxInProto(chatShowItem, this, CHATSHOWENTRYTAG, false);
                        SGMainManager::shareMain()->setChatShowPreLayer(sg_luaactivity);
//                        SGMainManager::shareMain()->openChatShowLayerByType(chatShowItem,type,sg_luaactivity); //留证勿删。
                    }
                }
            }
       

    
}

void SGActivityLayer::refreshPage(int type)
{

}

void SGActivityLayer::initActivityData(CCArray *array)
{
    luanum = 0;
    _totalPage = 0;
    if (array) {
        CC_SAFE_RELEASE(activityData);
        activityData = CCArray::create();
        activityData->retain();
        int count = array->count();
        for (int i=0; i<count; i++) {
            activityData->addObject(array->objectAtIndex(i));
        }
        luanum = count;
        
    }
   }
void SGActivityLayer::initView()
{
    ResourceManager *resourceManager = ResourceManager::sharedInstance();
    
    resourceManager->bindTexture("sgserverlistlayer/sgserverlistlayer.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);
    resourceManager->bindTexture("sgrewardlayer/sgrewardlayer.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);
    resourceManager->bindTexture("sgfirstactivitylayer/sgfirstactivitylayer.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);
    resourceManager->bindTexture("animationFile/short.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);
    resourceManager->bindTexture("sanguobigpic/barrack_bg.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();

    notification->addObserver(MSG_LUA_GETGIFT,this,callfuncO_selector(SGActivityLayer::GetGiftsListener));
    notification->addObserver(MSG_LUA_ACTIVITY_PAGE,this,callfuncO_selector(SGActivityLayer::showActivityPageListener));
    notification->addObserver(MSG_LUA_SET_ELEMENT,this,callfuncO_selector(SGActivityLayer::setActivityElementListener));
    float headhgt = SGMainManager::shareMain()->getVisitHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    SGLuaSocketSender::shareLuaSender();
    SGLuaDataProto::shareStatic();

    CCLOG("lua%d",luanum);
    
    CCSprite *gonggao_3_2 = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    gonggao_3_2->setAnchorPoint(ccp(0.5, 1));
    gonggao_3_2->setScaleX(6);
    gonggao_3_2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-47)));
    this->addChild(gonggao_3_2,2);
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 1));
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft),ccp(0,-47)));
    this->addChild(gonggao_bg_3_l,3);
    
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 1));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight),ccp(0,-47)));
    this->addChild(gonggao_bg_3_r,3);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_visitLayer);
    
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30,bg->getContentSize().width, s.height - btmhgt -45);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    bg->setTextureRect(r);
    
    if (CCDirector::sharedDirector()->getWinSize().height == 1136)
    {
        bg->setScaleY(1136 / (bg->getContentSize().height * 1.25));
    }
    
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt + 10)));
    
    
    CCScale9Sprite* kuang = CCScale9Sprite::createWithSpriteFrameName("box_kuang.png");
    kuang->setPreferredSize(CCSizeMake(SGLayout::getSkewing(SCROLLWIDE+38), 125));
    kuang->setPosition(ccpAdd(ccp(s.width*.5, s.height-headhgt), ccp(0, 0)));
    this->addChild(kuang);
    offset = s.height - btmhgt-kuang->getPreferredSize().height-50;
    
    scrollview2 = CCScrollView::create(CCSizeMake(s.width, s.height - btmhgt-kuang->getPreferredSize().height-50));
    scrollview2->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0,btmhgt)));//ccpAdd(ccp(s.width*.6, s.height-headhgt), ccp(0, 0))
    scrollview2->setDirection(kCCScrollViewDirectionVertical);
    //scrollview2->retain();
    this->addChild(scrollview2);
    //SGButton * but = SGButton::createFromLocal("an.png", str_FirstLayer_str4, this, menu_selector(SGActivityLayer::testFunction),ccp(0,3),FONT_PANGWA,ccc3(0x43,0x1f,0x0e),32,false,true);
    //this->addBtn(but);
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    resourceManager->bindTexture("sanguobigpic/serviceopenbuttom.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);
    
    std::string luaName = "";
    
    SGActivityData*lua =(SGActivityData *)m_pActivitydata;
    luaName = lua->getLuaName();
		if(isFileExist(CCString::createWithFormat("lualibs/%s",lua->getLuaName().c_str())->getCString())&&luaName!="")
        {
            
#if (PLATFORM == ANDROID)
		    CCString* pstrFileContent = CCString::createWithContentsOfFile(CCString::createWithFormat("lualibs/%s",lua->getLuaName().c_str())->getCString());
		    if (pstrFileContent)
		    {
			    pEngine->executeString(pstrFileContent->getCString());
		    }
#else
		
            pEngine->executeScriptFile(CCString::createWithFormat("lualibs/%s",lua->getLuaName().c_str())->getCString());
#endif
            //pEngine->executeScriptFile(lua->getLuaName().c_str());
            CCNode * luaLayer =  SGMainManager::shareMain()->getMainScene()->getChildByTag(lua->getLuaActivityId());
            if (luaLayer) {
                m_nTAG = luaLayer->getTag();
                setLuaLayerData(luaLayer,lua,1);
                luaLayer->removeFromParentAndCleanup(false);
                m_pCurrPage = (CCLayer *)luaLayer;
            }
      
   

            scrollview2->setContainer(m_pCurrPage);
            scrollview2->setContentSize(CCSizeMake((m_pCurrPage)->getContentSize().width,(m_pCurrPage)->getContentSize().height));
            scrollview2->setContentOffset(ccp(0,offset-(m_pCurrPage)->getContentSize().height));

        }
    scrollview = CCScrollView::create(CCSizeMake(SGLayout::getSkewing(SCROLLWIDE), 112));
    _layer = CCLayer::create();

    scrollview->setContainer(_layer);
    scrollview->ignoreAnchorPointForPosition(false);
    scrollview->setAnchorPoint(ccp(.5,.5));
    
    scrollview->setPosition(ccpAdd(kuang->getPosition(),ccp(0,0)));//ccpAdd(ccp(s.width*.6, s.height-headhgt), ccp(0, 0))
    scrollview->setDirection(kCCScrollViewDirectionHorizontal);
    this->addChild(scrollview);
    
    int width = 0;
    int countState = 0;
    int nTag = 0;
    for (int i=0;i<luanum;i++)
    {
        SGActivityBriefData*lua =(SGActivityBriefData *)activityData->objectAtIndex(i);
        countState = lua->getLuaState();
        //countState = chekCountTimeState(lua);
        std::string luaIconNmae = lua->getBtnName();
        std::string luaFileName = lua->getLuaName();
        if(isFileExist(CCString::createWithFormat("lualibs/%s",luaFileName.c_str())->getCString())&&luaIconNmae != "")
        {
            //红点
            CCSprite *notice = CCSprite::createWithSpriteFrameName("publc_notice.png");
            
            SGActivitySubLayer *sublayer = SGActivitySubLayer::create(lua->getBtnName(),ccp(width, 0),CCSizeMake(113, 113),i,this,countState);
            
            sublayer->addChild(notice, 100, 12);
            //根据服务器过来的数据确定是不是显示对应的红点
            if (lua->getNotice())
            {
                notice->setVisible(true);
            }
            else
            {
                notice->setVisible(false);
            }

            
            _totalPage++;
            _layer->addChild(sublayer,0,nTag);
            nTag++;
            sublayer->setiscantouch(true);
            sublayer->setPosition(ccp(width, 0));
            width+=110;
            notice->setPosition(ccpAdd(ccp(sublayer->getContentSize().width - notice->getContentSize().width / 4,
                                           sublayer->getContentSize().height - notice->getContentSize().height / 4), ccp(0, 0)));
            
            if (lua->getNotice())
            {
                notice->setVisible(true);
            }
            else
            {
                notice->setVisible(false);
            }
        }
        
    }
    
    
    m_pLayerCurr = (CCLayer*)_layer->getChildByTag(0);
    
    reallength = width;
    
    scrollview->setContentSize(CCSizeMake(reallength, 112));
    //    scrollview->setContentOffsetInDuration(ccp(-100,0),.5);
    
    left = CCSprite::createWithSpriteFrameName("login_jiantou.png");
    if (left) {
        this->addChild(left);
        left->setFlipX(true);
        left->setPosition(ccpAdd(kuang->getPosition(), ccp(-kuang->getContentSize().width*.5+15, 0)));
        left->setVisible(false);
        rgt = CCSprite::createWithSpriteFrameName("login_jiantou.png");
        this->addChild(rgt);
        rgt->setPosition(ccpAdd(kuang->getPosition(), ccp(kuang->getContentSize().width*.5-15, 0)));
        
        if (width<= SGLayout::getSkewing(SCROLLWIDE)) {
            rgt->setVisible(false);
        }
    }
    
    this->schedule(schedule_selector(SGActivityLayer::scrollViewDidScroll),.5f);
    int labeNum = lua->getLabelNum();
    if (labeNum>0)
    {
        this->schedule(schedule_selector(SGActivityLayer::UpdateTime),1.0f);
    }
    
    CCSprite *chatLine =  CCSprite::createWithSpriteFrameName("chatLine.png");
    chatLine->setAnchorPoint(ccp(0.5, 0.5));
    chatLine->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 154+chatLine->getContentSize().height/2)));
    chatLine->setScaleX(s.width/640);
    addChild(chatLine);
    
}
void  SGActivityLayer::testFunction()
{
    if(scrollview2->isVisible())
    {
        scrollview2->setVisible(false);
    }else{
        scrollview2->setVisible(true);
    }
}
void SGActivityLayer::setIsCanTouch(bool isTouch)
{
    scrollview->setTouchEnabled(isTouch);
    for (int i=0;i<_totalPage;i++)
    {
        SGActivitySubLayer*sublayer = (SGActivitySubLayer*)_layer->getChildByTag(i);
        sublayer->setiscantouch(isTouch);
    }
    scrollview2->setTouchEnabled(isTouch);
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    if(isTouch)
    {
        scrollview2->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0,btmhgt)));
    }
    else
    {
        scrollview2->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width,btmhgt)));
    }
    
    
}

void SGActivityLayer::getHandle(SGVipGift* gift)
{
    main::ActivityGetRewardRequest *request = new main::ActivityGetRewardRequest();
    request->set_btnid(gift->getBtnId());
    request->set_childtag(gift->getActivityId());
    SGSocketClient::sharedSocketClient()->send(MSG_LUA_GETGIFT, request);

}

void SGActivityLayer::GetGiftsListener(CCObject *obj)
{
    
   ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ActivityGetRewardResponse *response = (main::ActivityGetRewardResponse *)sr->m_msg;
//    if (sr)
//    {
//        if (response->state()==1) {
//            SG_SHOW_WINDOW(str_Get_succeed);
//        }
//        else
//        {
//            SG_SHOW_WINDOW(str_Get_fail);
//        }
//    
//    }


}



//初始化lua单个活动页面
void SGActivityLayer::setLuaLayerData(CCNode *luaLayer,SGActivityData *lua,int index)
{
    int elementNum = lua->getLabelNum();
    CCNode *luaElement = NULL;
    //label
    for (int i=0; i<elementNum; i++) {
        luaElement = luaLayer->getChildByTag(lua->m_labelInfo[i].getLabelId(),true);
        if (luaElement) {
            if (lua->m_labelInfo[i].getLabelValue()!=-1) {

                m_UpdateTimeData = SGActivityUpdateData::create((lua->m_labelInfo[i].getLabelValue()),(CCLabelTTF*)luaElement,index);
                m_UpdateTimeData->retain();
                int m = (lua->m_labelInfo[i].getLabelValue());
                int time[4];
                time[0] = m % 60;//sec
                m /= 60;
                time[1] = m % 60;//min
                m /= 60;
                time[2] = m%24;//hour
                time[3]= m/24;//day
               
                
                int tag = lua->m_labelInfo[i].getLabelId();
                CCLayer *layer = (CCLayer*)luaElement->getParent();
                for (int i=tag;i<tag+4;i++)
                {
                    CCLabelTTF* lab = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(i) );
                    SGCCLabelTTF * lab2 = dynamic_cast<SGCCLabelTTF*>(layer->getChildByTag(i) );
                    if (lab)
                    {
                        lab->setString(CCString::createWithFormat("%d",time[i-tag])->getCString());
                    }
                    else if(lab2)
                    {
                        lab2->setString(CCString::createWithFormat("%d",time[i-tag])->getCString());
                    }
                }
                //update->setActivityUpdateTime(update->getActivityUpdateTime());

            }
            if(!ccc4BEqual(lua->m_labelInfo[i].getLabelColor(),ccc4(0,0,0,0))){
                int tag = lua->m_labelInfo[i].getLabelId();
                CCLayer *layer = (CCLayer*)luaElement->getParent();
                for (int i=tag;i<tag+4;i++)
                {
                    CCLabelTTF* lab = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(i) );
                    SGCCLabelTTF * lab2 = dynamic_cast<SGCCLabelTTF*>(layer->getChildByTag(i) );
                    if (lab)
                    {
                        lab->setColor(ccc3BFrom4B(lua->m_labelInfo[i].getLabelColor()));
                    }
                    else if(lab2)
                    {
                        lab2->setInsideColor(ccc3BFrom4B(lua->m_labelInfo[i].getLabelColor()));
                    }
                }

            }
            //else
            //((CCLabelTTF*)luaElement)->setString(CCString::createWithFormat("%d",lua->m_labelInfo[i].getLabelValue())->getCString());
        }
       
    }
    //string
    elementNum = lua->getLabelStringnum();
    for (int i=0;i<elementNum;i++) {
        luaElement = luaLayer->getChildByTag(lua->m_LabelStringInfo[i].getLabelId(),true);
        if (luaElement)
        {
            
            if(lua->m_LabelStringInfo[i].getLabelString()!="")
            {
                CCLabelTTF * lab = dynamic_cast<CCLabelTTF*>(luaElement);
                SGCCLabelTTF * lab2 = dynamic_cast<SGCCLabelTTF*>(luaElement);
                if(lab)
                    lab->setString(CCString::createWithFormat("%s",lua->m_LabelStringInfo[i].getLabelString().c_str())->getCString());
                else if(lab2)
                {
                    lab2->setString(CCString::createWithFormat("%s",lua->m_LabelStringInfo[i].getLabelString().c_str())->getCString());
                }
                
            }
            if(!ccc4BEqual(lua->m_LabelStringInfo[i].getLabelColor(),ccc4(0,0,0,0)))
            {
                CCLabelTTF * lab = dynamic_cast<CCLabelTTF*>(luaElement);
                SGCCLabelTTF * lab2 = dynamic_cast<SGCCLabelTTF*>(luaElement);
                if(lab)
                   lab->setColor(ccc3BFrom4B(lua->m_LabelStringInfo[i].getLabelColor()));
                else if(lab2)
                    lab2->setInsideColor(ccc3BFrom4B(lua->m_LabelStringInfo[i].getLabelColor()));
            }
            
        }
    }
    //image
    elementNum = lua->getImagenum();
    for (int i=0;i<elementNum;i++) {
        luaElement = luaLayer->getChildByTag(lua->m_imageInfo[i].getImageId(),true);
        if (luaElement)
        {
            
            if(lua->m_imageInfo[i].getImageString()!="")
            {
                ((CCSprite*)luaElement)->setDisplayFrame(CCSprite::createWithSpriteFrameName(lua->m_imageInfo[i].getImageString().c_str())->displayFrame());
            }
            if(!ccc4BEqual(lua->m_imageInfo[i].getImageColor(),ccc4(0,0,0,0)))
            {
                ((CCSprite*)luaElement)->setColor(ccc3BFrom4B(lua->m_imageInfo[i].getImageColor()));
            }
            
        }
    }

    //btn
    elementNum = lua->getBtnNum();
    std::string imageName;
    for (int k=0; k<elementNum; k++)
    {
        CCObject * obj = luaLayer->getChildByTag(lua->m_btnInfo[k].getBtnId(),true);
        CCLayer* btnlayer = (CCLayer*)obj;
        luaElement = btnlayer->getChildByTag(lua->m_btnInfo[k].getBtnId()+2);
        if(lua->m_btnInfo[k].getBtnState() ==1)
        {
            btnlayer->setTouchEnabled(true);
        }
        else
        {
            btnlayer->setTouchEnabled(false);
        }
        
        if (luaElement)
        {
            if(lua->m_btnInfo[k].getFgStrString()!="")
            {
                ((CCSprite*)luaElement)->setDisplayFrame(CCSprite::createWithSpriteFrameName(lua->m_btnInfo[k].getFgStrString().c_str())->displayFrame());
            }
            if(!ccc4BEqual(lua->m_btnInfo[k].getImageColor(),ccc4(0,0,0,0)))
            {
                ((CCSprite*)luaElement)->setColor(ccc3BFrom4B(lua->m_btnInfo[k].getImageColor()));
            }
        }
        luaElement = btnlayer->getChildByTag(lua->m_btnInfo[k].getBtnId()+1,true);
        if (luaElement)
        {
            if(lua->m_btnInfo[k].getBgStrString()!="")
            {
                ((CCSprite*)luaElement)->setDisplayFrame(CCSprite::createWithSpriteFrameName(lua->m_btnInfo[k].getBgStrString().c_str())->displayFrame());
            }
            if(!ccc4BEqual(lua->m_btnInfo[k].getImageColor(),ccc4(0,0,0,0)))
            {
                ((CCSprite*)luaElement)->setColor(ccc3BFrom4B(lua->m_btnInfo[k].getImageColor()));
            }
        }
        luaElement = btnlayer->getChildByTag(lua->m_btnInfo[k].getBtnId()+3,true);
        if (luaElement)
        {
            
            if(lua->m_btnInfo[k].getFontStrString()!="")
            {
                CCLabelTTF * lab = dynamic_cast<CCLabelTTF*>(luaElement);
                SGCCLabelTTF * lab2 =dynamic_cast<SGCCLabelTTF*>(luaElement);
                if(lab)
                    lab->setString(CCString::createWithFormat("%s",lua->m_btnInfo[k].getFontStrString().c_str())->getCString());
                else if(lab2)
                    lab2->setString(CCString::createWithFormat("%s",lua->m_btnInfo[k].getFontStrString().c_str())->getCString());
            }
            if(!ccc4BEqual(lua->m_btnInfo[k].getFontColor(),ccc4(0,0,0,0)))
            {
                CCLabelTTF * lab = dynamic_cast<CCLabelTTF*>(luaElement);
                SGCCLabelTTF * lab2 = dynamic_cast<SGCCLabelTTF*>(luaElement);
                if(lab)
                    lab->setColor(ccc3BFrom4B(lua->m_btnInfo[k].getFontColor()));
                else if (lab2)
                    lab2->setInsideColor(ccc3BFrom4B(lua->m_btnInfo[k].getFontColor()));
            }
            
        }
    }
    //item
    elementNum = lua->getItemNum();
    CCSprite * jinkuang = NULL;
    CCSprite * itemIcon = NULL;
    SGCCLabelTTF *itemName = NULL;
    SGCCLabelTTF *itemCount = NULL;
    CCSprite * pNameBg = NULL;
    SGMainManager * sgMainManager = SGMainManager::shareMain();
    for (int k=0; k<elementNum; k++) {
        jinkuang = NULL;
        itemIcon = NULL;
        itemName = NULL;
        luaElement = luaLayer->getChildByTag(lua->m_ItemInfo[k].getItemBtId(),true);
        if (luaElement) {
            switch (lua->m_ItemInfo[k].getItemType()) {
                case BIT_INGOT://元宝
                {
                    jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_4_0.png");
                    itemIcon = CCSprite::createWithSpriteFrameName("big_yb.png");
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                    
                    if (luaLayer->getTag() == 654329) {
                    itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_yuanbao_bei,lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20);
                    }
                    else
                    itemName = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_yuanbao__,lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20);
                    
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                }
                    break;
                case BIT_GOLD://铜钱
                {
                    sgMainManager->addHeadIconPlistByNum(9999,sg_luaactivity);
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                    jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                    itemIcon = CCSprite::createWithSpriteFrameName("head9999.png");
                    itemName = SGCCLabelTTF::create(str_Format_tongMoney__, FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                }
                    break;
                case BIT_GRADE://Pvp积分
                {
                    sgMainManager->addHeadIconPlistByNum(9999,sg_luaactivity);
                    jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                    itemIcon = CCSprite::createWithSpriteFrameName("store_tianti_big.png");
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                    itemName = SGCCLabelTTF::create(str_Format_pvpScore__, FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                }
                    break;
                case BIT_FEAT://军工
                {
                    sgMainManager->addHeadIconPlistByNum(9997,sg_luaactivity);
                    jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                    itemIcon = CCSprite::createWithSpriteFrameName("head9997.png");
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                    itemName = SGCCLabelTTF::create(str_Format_jungong__, FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                }
                    break;
                case BIT_HORN://小喇叭
                {
                    sgMainManager->addHeadIconPlistByNum(9998,sg_luaactivity);
                    jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                    itemIcon = CCSprite::createWithSpriteFrameName("head9998.png");
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                    itemName = SGCCLabelTTF::create(str_Format_smalll_laba__, FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                }
                    break;
                case BIT_RECIPE://炼丹秘方
                {
                    sgMainManager->addHeadIconPlistByNum(9998,sg_luaactivity);
                    jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                    itemIcon = CCSprite::createWithSpriteFrameName("head9996.png");
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                    itemName = SGCCLabelTTF::create(str_Format_liandian__, FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                }
                    break;
                case BIT_OFFICER://武将
                {
                    SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(lua->m_ItemInfo[k].getItemId());
                    if (temp->getIconId() != -1 ) {
                        sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_luaactivity);
                        int starlvl = temp->getOfficerCurrStarLevel();
                        if (starlvl>1) {
                            starlvl-=1;
                        }
                        jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
                        itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("%s",temp->getOfficerName()->getCString())->getCString(), FONT_PANGWA, 20);
                        itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                        pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                        countryName->setAnchorPoint(ccp(1,1));
                        countryName->ignoreAnchorPointForPosition(false);
                        countryName->setPosition(ccp(jinkuang->getContentSize().width*0.35,jinkuang->getContentSize().height - countryName->getContentSize().height));
                        jinkuang->addChild(countryName,30,30);
                        
                        if (temp->getAdvNum() > 0) {
//                            CCSprite *advNum = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("+%d.png",temp->getAdvNum()))->getCString());
                            SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                            advNum->setInsideColor(ccGREEN);
                            advNum->setAnchorPoint(ccp(1,1));
                            advNum->ignoreAnchorPointForPosition(false);
                            advNum->setPosition(ccp(jinkuang->getContentSize().width *0.95 , jinkuang->getContentSize().height ));
                            jinkuang->addChild(advNum,40,40);
                        }
                    }
                    
                }
                    break;
                case BIT_EQUIP://装备
                {
                    SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(lua->m_ItemInfo[k].getItemId());
                    if (temp->getIconId() < 0) {
                        break;
                    }
                    sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_luaactivity);
                    jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString());
                    itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                    itemName = SGCCLabelTTF::create(CCString::createWithFormat("%s",temp->getEquipName()->getCString())->getCString(), FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                    if (temp->getAdvNum() > 0) {
                        SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                        advNum->setInsideColor(ccGREEN);
                        advNum->setAnchorPoint(ccp(1,1));
                        advNum->ignoreAnchorPointForPosition(false);
                        advNum->setPosition(ccp(jinkuang->getContentSize().width*0.95,jinkuang->getContentSize().height));
                        jinkuang->addChild(advNum,40,40);
                    }
                    
                }
                    break;
                case BIT_PROP://道具
                {
                    SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(lua->m_ItemInfo[k].getItemId());
                    int starlvl = temp->getPropsCurrStarLevel();
                    if (starlvl>1) {
                        starlvl-=1;
                    }
                    if (temp->getIconId() < 0) {
                        break;
                    }
                    sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_luaactivity);
                    jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString());
                    itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                    itemName = SGCCLabelTTF::create(CCString::createWithFormat("%s",temp->getPropsName()->getCString())->getCString(), FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                }
                    break;
                case BIT_PIECE://碎片
                {
                    SGPiecesDataModel *temp = SGStaticDataManager::shareStatic()->getPiecesDictById(lua->m_ItemInfo[k].getItemId());
                    
                    int starLv = temp->getStarLevel();
                    if (starLv > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
                    {
                        starLv -= 1;
                    }
                    
                    CCString *spName = NULL;
                    CCString *bgName = NULL;
                    ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_luaactivity);
                    if (temp->getCallingCardId() == -1)// 通用碎片//common_pieces_border
                    {
                        
                       // spName = CCString::createWithFormat("common_pieces_border.png");
                    }
                    else if (temp->getPiecesType() == 0)//武将碎片
                    {
                       
                        
                        SGOfficerDataModel *of = SGStaticDataManager::shareStatic()->getOfficerById(temp->getCallingCardId());
                        SGMainManager::shareMain() -> addHeadIconPlistByNum(of->getIconId(),sg_luaactivity);
                        spName = CCString::createWithFormat("head%d.png", of->getIconId());
                        int tag=of->getOfficerCurrStarLevel()   ;
                        if (tag>1) {
                            tag  =tag-1;
                        }
//                        bgName = CCString::createWithFormat("officerPinzhi_%d_0.png",tag);
                        bgName = CCString::createWithFormat("equipPinzhi_%d_0.png",starLv);
                        jinkuang = CCSprite::createWithSpriteFrameName(bgName->getCString());
                        itemIcon = CCSprite::createWithSpriteFrameName(spName->getCString());
                        pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("%s",temp->getPiecesName()->getCString())->getCString(), FONT_PANGWA, 20);
                        itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                        CCSprite *piecesMask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
                        piecesMask->setPosition(ccp(0, 0));
                        luaElement->addChild(piecesMask, 90);

                        
                        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",of->getOfficerRace()))->getCString());
                        countryName->setAnchorPoint(ccp(1,1));
                        countryName->ignoreAnchorPointForPosition(false);
                        countryName->setPosition(ccp(jinkuang->getContentSize().width*0.35,jinkuang->getContentSize().height - countryName->getContentSize().height));
                        jinkuang->addChild(countryName,30,30);
                        
                        
                        
                    }
                    else if (temp->getPiecesType() == 1)//装备碎片
                    {
                        SGEquipmentDataModel *eq = SGStaticDataManager::shareStatic()->getEquipById(temp->getCallingCardId());
                        SGMainManager::shareMain() -> addHeadIconPlistByNum(eq->getIconId(),sg_luaactivity);
                        spName = CCString::createWithFormat("head%d.png", eq->getIconId());

                        bgName = CCString::createWithFormat("equipPinzhi_%d_0.png",starLv);
                        jinkuang = CCSprite::createWithSpriteFrameName(bgName->getCString());
                        itemIcon = CCSprite::createWithSpriteFrameName(spName->getCString());
                        pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                        itemName = SGCCLabelTTF::create(CCString::createWithFormat("%s",temp->getPiecesName()->getCString())->getCString(), FONT_PANGWA, 20);
                        itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                        CCSprite *piecesMask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
                        piecesMask->setPosition(ccp(0, 0));
                        luaElement->addChild(piecesMask, 90);
                    }
                }
                    break;
                case BIT_SOUL://军魂
                {
                    sgMainManager->addHeadIconPlistByNum(9999,sg_luaactivity);
                    jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                    itemIcon = CCSprite::createWithSpriteFrameName("soul_icon.png");
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                    itemName = SGCCLabelTTF::create(str_Format_junhun__, FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                }
                    break;
               
                case BIT_CONSUMABLE://消耗品
                {

                    SGConsumableDataModel *temp = SGStaticDataManager::shareStatic()->getConsumableById(lua->m_ItemInfo[k].getItemId());
                    int starlvl = temp->getConsumeStarLvl();
                    if (starlvl>1) {
                        starlvl-=1;
                    }
                    if (temp->getIconId() < 0) {
                        break;
                    }
                    sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_luaactivity);
                    jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl)->getCString());
                    itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                    itemName = SGCCLabelTTF::create(CCString::createWithFormat("%s",temp->getConsumeName()->getCString())->getCString(), FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                }
                    break;
                case BIT_MEDAL://勋章
                {
                    sgMainManager->addHeadIconPlistByNum(9999,sg_luaactivity);
                    jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                    itemIcon = CCSprite::createWithSpriteFrameName("tongqueXunBig.png");
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                    itemName = SGCCLabelTTF::create(str_Format_xunzhang__, FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                }
                    break;
                case BIT_MATERIAL: //材料
                {
                    SGStaticDataManager* sgsdm = SGStaticDataManager::shareStatic();
                    SGMaterialDataModel* temp = sgsdm->getMaterialById(lua->m_ItemInfo[k].getItemId());
                    sgMainManager->addHeadIconPlistByNum(temp->getMaterialIconId(), sg_luaactivity);
                    
                    int starLvl = temp->getMaterialBorderColor();
                    if (starLvl > 1)
                    {
                        starLvl -= 1;
                    }
                    
                    jinkuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",starLvl)->getCString());
                    itemIcon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getMaterialIconId())->getCString());
                    itemName = SGCCLabelTTF::create(CCString::createWithFormat("%s",temp->getMaterialName()->getCString())->getCString(), FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                }
                    break;
                case BIT_JADE: //勾玉
                {
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                    jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                    itemIcon = CCSprite::createWithSpriteFrameName("big_gouyu.png");
                    itemName = SGCCLabelTTF::create(str_chinese_jade, FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                }
                    break;
                case BIT_ARENA_SCORE: //征服点
                {
                    pNameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
                    jinkuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                    itemIcon = CCSprite::createWithSpriteFrameName("arean_coin_big.png");
                    itemName = SGCCLabelTTF::create(str_arena_score, FONT_PANGWA, 20);
                    itemCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d",lua->m_ItemInfo[k].getItemCount())->getCString(), FONT_PANGWA, 20,ccc3(0x29, 0xff, 0x1b));
                }
                    break;
                case -1:
                {
                    luaElement->setVisible(false);
                }
                    break;
                default:
                    break;

         }
            if (itemIcon && jinkuang&&itemCount)
            {
                itemIcon->setPosition(ccp(0,0));
                jinkuang->setPosition(itemIcon->getPosition());
                jinkuang->setZOrder(100);
                itemName->setPosition(ccpAdd(itemIcon->getPosition(),ccp(0 , -luaElement->getContentSize().height/2-itemName->getContentSize().height/2-4)));
                itemCount->setPosition(ccpAdd(itemIcon->getPosition(),ccp(0 , -luaElement->getContentSize().height/2+10)));
                itemCount->setZOrder(101);
                pNameBg->setPosition(itemName->getPosition());
                pNameBg->setZOrder(-1);
                if (itemName)
                    luaElement->addChild(itemName);
                if (itemIcon)
                    luaElement->addChild(itemIcon);
                if (jinkuang)
                    luaElement->addChild(jinkuang);
                if(pNameBg)
                    luaElement->addChild(pNameBg);
                if(itemCount)
                    luaElement->addChild(itemCount);
            }
            

            
            
      }
    }
    //lua页面初始化完成，设置对应的红点notice是不是显示
    //subLayer存在
    if (m_pLayerCurr && !lua->getNotice())
    {
        CCSprite *notice = static_cast<CCSprite *>(m_pLayerCurr->getChildByTag(12));//notice 对应的tag
        notice->setVisible(false);
        
        for (int i = 0; i < this->activityData->count(); i++)
        {
            SGActivityBriefData *tmp = static_cast<SGActivityBriefData *>(this->activityData->objectAtIndex(i));
            if (lua->getLuaActivityId() == tmp->getLuaActivityId())
            {
                tmp->setNotice(false);
            }
        }
        //设置本地数据，
        lua->setNotice(false);
        
    }
}
bool SGActivityLayer::isFileExist(const char* pFileName)
{
    if( !pFileName ) return false;
    //std::string filePath  = "";//CCFileUtils::sharedFileUtils()->getWritablePath();
    //filePath += pFileName;
    
   // FILE *fp = fopen(filePath.c_str(),"r");
    //if(fp)
    //{
       // fclose(fp);
        //return true;
    //}
    //return false;
    
     std::string filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFileName);
    //读取的字节数，读取失败则为0
    unsigned long len = 0;
    //读取的内容
    unsigned char *data =CCFileUtils::sharedFileUtils()->getFileData(filepath.c_str(), "r", &len);
    
  
    if(len >0 && data)
    {
        delete[] data;
        return true;
    }
    return false;
}
///点击活动icon响应方法
void SGActivityLayer::BtnTouched(SGActivitySubLayer* layer)
{
    //如果有引导，不响应点击
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
    
//    (CCSprite *)menu->getChildByTag(100);
    for (int i=0;i<_totalPage;i++) {
        SGActivitySubLayer*sublayer = (SGActivitySubLayer*)_layer->getChildByTag(i);
        CCSprite* spr = (CCSprite*)sublayer->getChildByTag(100);
        spr->setVisible(false);
    }
    CCSprite* spr = (CCSprite*)layer->getChildByTag(100);
    spr->setVisible(true);
    if(layer == m_pLayerCurr)
    {
        return;
    }
    else
    {
        m_pLayerCurr = layer;
        SGActivityBriefData* activitBriefData = (SGActivityBriefData*)activityData->objectAtIndex(layer->getTag());
        //activitBriefData->getLuaActivityId()
        m_nTAG = layer->getTag();
        main::ActivityDetailRequest * request = new main::ActivityDetailRequest;
        request->set_luaid(activitBriefData->getLuaActivityId());
        SGSocketClient::sharedSocketClient()->send(MSG_LUA_ACTIVITY_PAGE, request);
    }
    
    
    
}
void SGActivityLayer::showActivityPageListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    //main::ActivityDetailResponse *response = (main::ActivityDetailResponse *)sr->m_msg;
    if (sr)
    {
        main::ActivityDetailResponse *response = (main::ActivityDetailResponse *)sr->m_msg;
        SGActivityData * activityData = NULL;
        
        int labelLen = 0;
        int btnLen = 0;
        int ItemLen = 0;
        int stringLen = 0;
        int imageLen = 0;
        
        
        
        //main::ActivityDetailResponse activityDetail= response;//activitydetail(i);
        main::ActivityBrief activityBrief = response->activitybrief();
        if(activityBrief.has_luaid()&&activityBrief.has_luaname()&&activityBrief.has_btnname())
        {
            activityData = SGActivityData::create(activityBrief.luaid(), activityBrief.luaname(), activityBrief.btnname(), activityBrief.notice());
        }
        else
        {
             activityData = SGActivityData::create(1, "", "", false);
        }
        //activityData = SGActivityData::create(response->luaid(), response->luaname(), response->btnname());
        
        //label
        labelLen = response->labelinfo_size();
        for (int la=0; la<labelLen; la++) {
            main::LabelInfo labelInfo = response->labelinfo(la);
            ccColor4B color(ccc4(0,0,0,0));
            if(labelInfo.has_fontcolor())
            {
                main::ElementColor elementColor = response->labelinfo(la).fontcolor();
                color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
            }
            int labelValue = -1;
            if(labelInfo.has_labelvalue())
            {
                labelValue = labelInfo.labelvalue();
            }
            activityData->addLabelInfo(labelInfo.labelid(), labelValue,color);
        }
        //btn
        btnLen = response->btninfo_size();
        for (int bt=0; bt<btnLen; bt++) {
            main::BtnInfo btntInfo = response->btninfo(bt);
            std::string strFont = "";
            if(btntInfo.has_fontstr())
            {
                strFont = btntInfo.fontstr();
            }
            ccColor4B fontColor (ccc4(0,0,0,0));
            if(btntInfo.has_fontcolor())
            {
                main::ElementColor elementColor = btntInfo.fontcolor();
                fontColor =  ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
            }
            std::string strBg = "";
            if(btntInfo.has_bgstr())
            {
                strBg = btntInfo.bgstr();
            }
            std::string strFg = "";
            if(btntInfo.has_fgstr())
            {
                strFg = btntInfo.fgstr();
            }
            ccColor4B imageColor (ccc4(0,0,0,0));
            if(btntInfo.has_imagecolor())
            {
                main::ElementColor elementColor = btntInfo.imagecolor();
                imageColor =  ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
            }
            int state  = 1;
            if(btntInfo.has_btnstate())
            {
                state = btntInfo.btnstate();
            }
            
            activityData->addBtnInfo(btntInfo.btnid(), strFont,fontColor,strBg,strFg,imageColor,state);
        }
        //item
        ItemLen = response->iteminfo_size();
        for (int it = 0; it<ItemLen; it++) {
            activityData->addItemInfo(response->iteminfo(it).itemtype(),
                                      response->iteminfo(it).itemid(),
                                      response->iteminfo(it).itemcount(),
                                      response->iteminfo(it).itemlevel(),
                                      response->iteminfo(it).itembtnid());
        }
        stringLen = response->labelstringinfo_size();
        for(int i = 0;i<stringLen;i++)
        {
            main::LabelStringInfo labelStringInfo = response->labelstringinfo(i);
            ccColor4B color(ccc4(0,0,0,0));
            if(labelStringInfo.has_fontcolor())
            {
                main::ElementColor elementColor = labelStringInfo.fontcolor();
                color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
            }
            std::string labelString = "";
            if(labelStringInfo.has_labelvalue())
            {
                labelString = labelStringInfo.labelvalue();
            }
            activityData->addLabelStringInfo(labelStringInfo.labelid(), labelString,color);
        }
        imageLen = response->imageinfo_size();
        for(int i = 0; i < imageLen; i++)
        {
            main::ImageInfo imageInfo = response->imageinfo(i);
            std::string imageStr = "";
            if(imageInfo.has_imagestr())
            {
                imageStr = imageInfo.imagestr();
            }
            ccColor4B color(ccc4(0,0,0,0));
            if(imageInfo.has_imagecolor())
            {
                main::ElementColor elementColor = imageInfo.imagecolor();
                color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
            }
            activityData->addImageInfo(imageInfo.imageid(), imageStr, color);
        }
        //array->addObject(activityData);
        std::string luaName = activityData->getLuaName();
        m_pActivitydata =activityData;
        m_pActivitydata->retain();
        if(isFileExist(CCString::createWithFormat("lualibs/%s",activityData->getLuaName().c_str())->getCString())&&luaName!="")
        {
            CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
        
#if (PLATFORM == ANDROID)
		    CCString* pstrFileContent = CCString::createWithContentsOfFile(CCString::createWithFormat("lualibs/%s",activityData->getLuaName().c_str())->getCString());
		    if (pstrFileContent)
		    {
			   pEngine->executeString(pstrFileContent->getCString());
		    }
#else
		
          pEngine->executeScriptFile(CCString::createWithFormat("lualibs/%s",activityData->getLuaName().c_str())->getCString());
#endif
        //pEngine->executeScriptFile(lua->getLuaName().c_str());
            CCNode * luaLayer =  SGMainManager::shareMain()->getMainScene()->getChildByTag(activityData->getLuaActivityId());
            //CCLayer *laLast = (CCLayer *)lualayerdic->objectAtIndex(0);
            m_pCurrPage->removeFromParentAndCleanup(true);
           if (luaLayer) {
                setLuaLayerData(luaLayer,activityData,m_nTAG);
                luaLayer->removeFromParentAndCleanup(false);
                m_pCurrPage =(CCLayer *)luaLayer;
                //lualayerdic->addObject(luaLayer);
               
               bool isNotice = false;
               //遍历所有的data，决定外层是不是显示
               if (this->activityData && this->activityData->data && this->activityData->data->arr)
               {
                   for (int i = 0; i < this->activityData->count(); i++)
                   {
                       SGActivityBriefData *tmp = static_cast<SGActivityBriefData *>(this->activityData->objectAtIndex(i));
                       
                       std::string luaIconNmae = tmp->getBtnName();
                       std::string luaFileName = tmp->getLuaName();
                       if(isFileExist(CCString::createWithFormat("lualibs/%s",luaFileName.c_str())->getCString())
                          && luaIconNmae != "" && tmp && tmp->getNotice())
                       {
                           //如果有一个没点开，首页红点仍显示
                           isNotice = true;
                           break;
                       }

                   }
               }
               //最终确定是不是显示红点
               SGPlayerInfo::sharePlayerInfo()->setIsActiviNotAccess(isNotice);

               
            }
            
        
            //CCLayer *la = (CCLayer *)lualayerdic->objectAtIndex(0);
            scrollview2->setContainer(m_pCurrPage,false);
            scrollview2->setContentSize(CCSizeMake(m_pCurrPage->getContentSize().width,m_pCurrPage->getContentSize().height));
            scrollview2->setContentOffset(ccp(0,offset-m_pCurrPage->getContentSize().height));
            int labeNum = activityData->getLabelNum();
            if (labeNum>0)
            {
                this->schedule(schedule_selector(SGActivityLayer::UpdateTime),1.0f);
            }
            else
            {
                 this->unschedule(schedule_selector(SGActivityLayer::UpdateTime));
            }

        }

    }

}
void SGActivityLayer::setActivityElementListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        main::ActivityOpretionResponse *response = (main::ActivityOpretionResponse *)sr->m_msg;
        int len = response->opreationlist_size();
        for(int i = 0; i < len; i++)
        {
            main::ActivityOpretion actOpreation = response->opreationlist(i);
            CCLayer *la = (CCLayer *)m_pCurrPage;
            if(la)
            {
                main::ElementType type  = actOpreation.objecttype();
                CCNode * element = la->getChildByTag(actOpreation.objectid(),true);
                if(element)
                {
                    
                    if(type == main::ET_NUMERIC_LABEL)
                    {
                        
                            if (actOpreation.has_fontstr()) {
                                
                                int m = atoi(actOpreation.fontstr().c_str());
                                int time[4];
                                time[0] = m % 60;//sec
                                m /= 60;
                                time[1] = m % 60;//min
                                m /= 60;
                                time[2] = m%24;//hour
                                time[3]= m/24;//day
                                
                                
                                int tag = actOpreation.objectid();
                                CCLayer *layer = (CCLayer*)element->getParent();
                                for (int i=tag;i<tag+4;i++)
                                {
                                    CCLabelTTF* lab = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(i));
                                    if (lab)
                                    {
                                        lab->setString(CCString::createWithFormat("%d",time[i-tag])->getCString());
                                    }
                                    else
                                    {
                                        SGCCLabelTTF * lab = dynamic_cast<SGCCLabelTTF*>(layer->getChildByTag(i));
                                        lab->setString(CCString::createWithFormat("%d",time[i-tag])->getCString());
                                    }
                                }
                                //update->setActivityUpdateTime(update->getActivityUpdateTime());
                                
                            }
                            if(actOpreation.has_fontcolor()){
                                int tag = actOpreation.objectid();
                                CCLayer *layer = (CCLayer*)element->getParent();
                                for (int i=tag;i<tag+4;i++)
                                {
                                    CCLabelTTF* lab = dynamic_cast<CCLabelTTF*>(layer->getChildByTag(i));
                                    if (lab) {
                                        main::ElementColor elementColor = actOpreation.fontcolor();
                                        ccColor4B color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
                                        lab->setColor(ccc3BFrom4B(color));
                                    }
                                    else
                                    {
                                        SGCCLabelTTF * lab = dynamic_cast<SGCCLabelTTF*>(layer->getChildByTag(i));
                                        main::ElementColor elementColor = actOpreation.fontcolor();
                                        ccColor4B color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
                                        lab->setInsideColor(ccc3BFrom4B(color));
                                    }
                                }
                                
                            }
                        
                       
                    }
                    else if(type == main::ET_TEXT_LABEL)
                    {
                        if(actOpreation.has_fontstr())
                        {
                            CCLabelTTF * lab = dynamic_cast<CCLabelTTF*>(element);
                            SGCCLabelTTF * lab2 =dynamic_cast<SGCCLabelTTF*>(element);
                            if(lab)
                                lab->setString(CCString::createWithFormat("%s",actOpreation.fontstr().c_str())->getCString());
                            else if(lab2)
                                lab2->setString(CCString::createWithFormat("%s",actOpreation.fontstr().c_str())->getCString());
                        }
                        if(actOpreation.has_fontcolor())
                        {
                            main::ElementColor elementColor = actOpreation.fontcolor();
                            ccColor4B color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
//                            ((CCLabelTTF*)element)->setColor(ccc3BFrom4B(color));
                            CCLabelTTF * lab = dynamic_cast<CCLabelTTF*>(element);
                            SGCCLabelTTF * lab2 =dynamic_cast<SGCCLabelTTF*>(element);
                            if(lab)
                                lab->setColor(ccc3BFrom4B(color));
                            else if(lab2)
                                lab2->setInsideColor(ccc3BFrom4B(color));
                        }
                    }
                    else if(type == main::ET_IMAGE)
                    {
                        if(actOpreation.has_bgstr())
                        {
                            ((CCSprite*)element)->setDisplayFrame(CCSprite::createWithSpriteFrameName(actOpreation.bgstr().c_str())->displayFrame());
                        }
                    
                        if(actOpreation.has_imagecolor())
                        {
                            main::ElementColor elementColor = actOpreation.imagecolor();
                            ccColor4B color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
                            ((CCSprite*)element)->setColor(ccc3BFrom4B(color));
                        }
                    }
                    else if(type == main::ET_BUTTON)
                    {
                        if(actOpreation.has_state())
                        {
                            int state = actOpreation.state();
                            if(state == 1)
                            {
                                ((CCLayer *)element)->setTouchEnabled(true);
                            }
                            else
                            {
                                ((CCLayer *)element)->setTouchEnabled(false);
                            }
                        }
                        CCNode * node = element->getChildByTag(actOpreation.objectid()+2,true);
                        if(node)
                        {
                            if(actOpreation.has_fgstr())
                            {
                                ((CCSprite*)node)->setDisplayFrame(CCSprite::createWithSpriteFrameName(actOpreation.fgstr().c_str())->displayFrame());
                            }
                            if(actOpreation.has_imagecolor())
                            {
                                main::ElementColor elementColor = actOpreation.imagecolor();
                                ccColor4B color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
                                ((CCSprite*)node)->setColor(ccc3BFrom4B(color));
                            }
                        }
                        node = element->getChildByTag(actOpreation.objectid()+1,true);
                        if(node)
                        {
                            if(actOpreation.has_bgstr())
                            {
                                ((CCSprite*)node)->setDisplayFrame(CCSprite::createWithSpriteFrameName(actOpreation.bgstr().c_str())->displayFrame());
                            }
                            if(actOpreation.has_imagecolor())
                            {
                                main::ElementColor elementColor = actOpreation.imagecolor();
                                ccColor4B color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
                                ((CCSprite*)node)->setColor(ccc3BFrom4B(color));
                            }
                        }
                        
                        node = element->getChildByTag(actOpreation.objectid()+3,true);
                        if(node)
                        {
                            if(actOpreation.has_fontstr())
                            {
//                                ((CCLabelTTF*)node)->setString(CCString::createWithFormat("%s",actOpreation.fontstr().c_str())->getCString());
                                CCLabelTTF * lab = dynamic_cast<CCLabelTTF*>(node);
                                SGCCLabelTTF * lab2 =dynamic_cast<SGCCLabelTTF*>(node);
                                if(lab)
                                    lab->setString(CCString::createWithFormat("%s",actOpreation.fontstr().c_str())->getCString());
                                else if(lab2)
                                    lab2->setString(CCString::createWithFormat("%s",actOpreation.fontstr().c_str())->getCString());

                            }
                            if(actOpreation.has_fontcolor())
                            {
                                main::ElementColor elementColor = actOpreation.fontcolor();
                                ccColor4B color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
//                                ((CCLabelTTF*)node)->setColor(ccc3BFrom4B(color));
                                CCLabelTTF * lab = dynamic_cast<CCLabelTTF*>(node);
                                SGCCLabelTTF * lab2 =dynamic_cast<SGCCLabelTTF*>(node);
                                if(lab)
                                    lab->setColor(ccc3BFrom4B(color));
                                else if(lab2)
                                    lab2->setInsideColor(ccc3BFrom4B(color));
                            }
                        }
                    
                    }
                    else if(type == main::ET_ITEM)
                    {
                        //暂时不做操作
                    }
                    
                    
                }
            }
            //main::ElementType type  = actOpreation.objecttype();
        }
    }
}
void SGActivityLayer::scrollViewDidScroll()
{
    //    EFFECT_PLAY(MUSIC_BTN);
    float position = scrollview->getContentOffset().x;
    left->setVisible(position<0);
    rgt->setVisible((position+reallength) >skewing(SCROLLWIDE)?true:false);
    
}



void SGActivityLayer::UpdateTime()
{
    bool closeUpdate = true;
    int m = 0;
    
        SGActivityUpdateData* update = m_UpdateTimeData;
        update->getActivityUpdateTime();
        CCLabelTTF*label = update->getLabel();
        if (update->getActivityUpdateTime() == 0)
        {
            //最后1秒，活动结束
            ((SGActivitySubLayer*) _layer->getChildByTag(update->getActivityIndex()))->updateCheckState(2);
            m = 0;
        }
        else if (update->getActivityUpdateTime() < 0)
        {
           
        }
        else
        {
            m = update->getActivityUpdateTime();
            closeUpdate = false;
        }
       
        int time[4];
        time[0] = m % 60;//sec
        m /= 60;
        time[1] = m % 60;//min
        m /= 60;
        time[2] = m%24;//hour
        time[3]= m/24;//day

        int tag = label->getTag();
        CCLayer *layer = (CCLayer*)label->getParent();
        for (int i=tag;i<tag+4;i++) {
           CCLabelTTF* lab = (CCLabelTTF*)layer->getChildByTag(i);
            if (lab) {
                lab->setString(CCString::createWithFormat("%d",time[i-tag])->getCString());
            }
        }
        update->setActivityUpdateTime(update->getActivityUpdateTime()-1);

   

    if (closeUpdate)
    {
        this->unschedule(schedule_selector(SGActivityLayer::UpdateTime));
    }

}

void SGActivityLayer::buyGrowPlan(cocos2d::CCObject * obj)
{
//    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
//    SGCCLabelTTF * label = (SGCCLabelTTF*) m_pCurrPage->getChildByTag(1 , true);
//    CCAssert(label !=NULL , "can not be null");
//    int cost = atoi( label->getString());
//    if(player->getplayerVipLevel() < 3)
//    {
//        if(player->getShowvip())
//        {
//            SGCantAdvanceBox * box = SGCantAdvanceBox::create(NULL , NULL , 1919 , 1);
//            SGMainManager::shareMain()->showBox(box);
//        }
//    }
//    else if(cost > SGPlayerInfo::sharePlayerInfo()->getPlayerGold())
//    {
//        SGCantAdvanceBox * box = SGCantAdvanceBox::create(NULL , NULL , 1919 , 2);
//        SGMainManager::shareMain()->showBox(box);
//    }
//    else
//    {
        main::ShopGrowPlanRequest *request = new main::ShopGrowPlanRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_ACTIVITY_BUY_GROW_PLAN, request);
//    }
}

void SGActivityLayer::buyGrowPlanResponse(CCObject * sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender ;
    main::ShopGrowPlanResponse *response = (main::ShopGrowPlanResponse *)request->m_msg;
    
    if (response)
    {
        if(1 == response->succ())
        {
            this->BtnTouched(dynamic_cast<SGActivitySubLayer*>(m_pLayerCurr) );
            SGCantAdvanceBox * box = SGCantAdvanceBox::create(NULL , NULL , 1919 , 3);
            SGMainManager::shareMain()->showBox(box);
        }
        else if(-1 == response->succ())
        {
            SGCantAdvanceBox * box = SGCantAdvanceBox::create(NULL , NULL , 1919 , 1);
            SGMainManager::shareMain()->showBox(box);
//            SGMainManager::shareMain()->showMessage(str_MainLayer_str12);
        }
        else if(-2==response->succ())
        {
            SGCantAdvanceBox * box = SGCantAdvanceBox::create(NULL , NULL , 1919 , 2);
            SGMainManager::shareMain()->showBox(box);
//            SGMainManager::shareMain()->showMessage(str_CantAdvance_tips46);
        }
        else if(-3 == response->succ())
        {
            SGMainManager::shareMain()->showMessage(str_CantAdvance_tips136);
        }
        else
        {
            SGMainManager::shareMain()->showMessage(str_StoreLayer_str12);
        }
    }

}


//void SGActivityLayer::showFirstLuaLayer()
//{
//    SGActivityBriefData *tmp = static_cast<SGActivityBriefData *>(this->activityData->objectAtIndex(0));
//    
//    main::ActivityDetailRequest * request = new main::ActivityDetailRequest;
//    request->set_luaid(tmp->getLuaActivityId());
//    SGSocketClient::sharedSocketClient()->send(MSG_LUA_ACTIVITY_PAGE, request);
//    
//    scheduleOnce(schedule_selector( SGActivityLayer::timerCallback ), 0.6);
//}
//
//void SGActivityLayer::timerCallback()
//{
//    m_pLayerCurr = (CCLayer*)_layer->getChildByTag(0);
//    
//    for (int i=0;i<_totalPage;i++)
//    {
//        SGActivitySubLayer*sublayer = (SGActivitySubLayer*)_layer->getChildByTag(i);
//        CCSprite* spr = (CCSprite*)sublayer->getChildByTag(100);
//        spr->setVisible(false);
//    }
//    CCSprite* spr = (CCSprite*)m_pLayerCurr->getChildByTag(100);
//    spr->setVisible(true);
//}




