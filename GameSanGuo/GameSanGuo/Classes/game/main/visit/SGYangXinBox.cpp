//
//  SGYangXinBox.cpp
//  GameSanGuo
//
//  Created zenghui
//
//

#include "SGYangXinBox.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "SGPlayerInfo.h"
#include "LotteryModule.pb.h"
#include "SGMainLayer.h"
#include "ResourceManager.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"

SGYangXinBox::SGYangXinBox():m_canGot(false)
{
}
SGYangXinBox::~SGYangXinBox()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_yangXinBox);
}
SGYangXinBox *SGYangXinBox::create(SGBoxDelegate *bdg,SGItemInfo &yangXinInfo,bool canGot)
{
    SGYangXinBox *noExpBox = new SGYangXinBox();
    if (noExpBox && noExpBox->init(bdg,yangXinInfo,canGot))
    {
        noExpBox->setTag(box_yangxingTag);
        noExpBox->autorelease();
        return noExpBox;
    }
    CC_SAFE_DELETE(noExpBox);
    return NULL;
}
bool SGYangXinBox::init(SGBoxDelegate *bdg,SGItemInfo &yangXinInfo,bool canGot)
{
 
    ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_yangXinBox, LIM_PNG_AS_PNG);
   
    ResourceManager::sharedInstance()->bindTexture(SGMainManager::shareMain()->getPlistWithChannel().c_str(), RES_TYPE_LAYER_UI, sg_yangXinBox);

    ResourceManager::sharedInstance()->bindTexture("sgvisitlayer/sgvisitlayer2.plist", RES_TYPE_LAYER_UI, sg_yangXinBox);
    ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_yangXinBox);
    
    m_canGot = canGot;

    float wid = 535;
    float hgt = 365;
    if (!SGBaseBox::init(bdg,box_yangxingTag,CCSizeMake(wid, hgt)))
    {
        return false;
    }

    SGButton *store = NULL;
    if (m_canGot) {
        store = SGButton::createFromLocal("box_btn_newbg.png", str_YangXinBox_str1, this, menu_selector(SGYangXinBox::lingQuHandler),ccp(0,3),FONT_PANGWA,ccWHITE,32);

    }
    else
    {
        store = SGButton::createFromLocal("box_btn_newbg.png", str_Ok_, this, menu_selector(SGYangXinBox::confirmHandler),ccp(0,3),FONT_PANGWA,ccWHITE,32);
    }
    this->addBtn(store);
    store->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -hgt*0.4+5)));

    SGCCLabelTTF* detail = NULL;
    if (m_canGot) {
        detail = SGCCLabelTTF::create(str_YangXinBox_str2, FONT_BOXINFO, 32);
    }
    else
    {
        detail = SGCCLabelTTF::create(str_YangXinBox_str3, FONT_BOXINFO, 32);
    }
    this->addChild(detail);
    detail->setAnchorPoint(ccp(0, 0.5f));
    detail->setPosition(ccpAdd(biao->getPosition(), ccp(-230,-60)));
    
    CCNode *yangXinItem = creatYangXinItem(yangXinInfo);
    if (yangXinItem) {
        addChild(yangXinItem);
        yangXinItem->setPosition(ccp(SGLayout::getPoint(kMiddleCenter).x, detail->getPosition().y-80));
    }
    
    SGCCLabelTTF * title = SGCCLabelTTF::create(str_YangXinBox_str4, FONT_XINGKAI, 36);
    if (title) {
        title->setPosition(biao->getPosition());
        this->addChild(title);
    }
    return true;
}

CCNode * SGYangXinBox::creatYangXinItem(SGItemInfo &yangXinInfo)
{
    SGCCLabelTTF *itemName = NULL;
    SGButton *visitCardbg = NULL;
    SGMainManager * sgMainManager = SGMainManager::shareMain();
    
    SGStaticDataManager *staticDataManager = SGStaticDataManager::shareStatic();
    CCString *nameStr = NULL;
    switch (yangXinInfo.getItemType()) {
        case 0:
        {
            visitCardbg = SGButton::create("big_yb.png", "jinkuang.png", NULL,NULL,CCPointZero,false,true);
            nameStr = CCString::createWithFormat(str_Format_yuanbao,yangXinInfo.getItemCount());
        }
            break;
        case 1:
        {
            sgMainManager->addHeadIconPlistByNum(9999,sg_tongQueLayer);
            visitCardbg = SGButton::create("head9999.png", "jinkuang.png", NULL,NULL,CCPointZero,false,true);
            nameStr = CCString::createWithFormat(str_Format_tongMoney,yangXinInfo.getItemCount());
        }
            break;
        case 2:
        {
            sgMainManager->addHeadIconPlistByNum(9999,sg_tongQueLayer);
            visitCardbg = SGButton::create("head9999.png", "jinkuang.png", NULL,NULL,CCPointZero,false,true);
            nameStr = CCString::createWithFormat(str_Format_pvpScore,yangXinInfo.getItemCount());
        }
            break;
        case 3:
        {
            sgMainManager->addHeadIconPlistByNum(9997,sg_tongQueLayer);
            visitCardbg = SGButton::create("head9997.png", "jinkuang.png", NULL,NULL,CCPointZero,false,true);
            nameStr = CCString::createWithFormat(str_Format_jungong,yangXinInfo.getItemCount());
        }
            break;
        case 4:
        {
            sgMainManager->addHeadIconPlistByNum(9998,sg_tongQueLayer);
            visitCardbg = SGButton::create("head9998.png", "jinkuang.png", NULL,NULL,CCPointZero,false,true);
            nameStr = CCString::createWithFormat(str_Format_smalll_laba,yangXinInfo.getItemCount());
        }
            break;
        case 5:
        {
            sgMainManager->addHeadIconPlistByNum(9998,sg_tongQueLayer);
            visitCardbg = SGButton::create("head9998.png", "jinkuang.png", NULL,NULL,CCPointZero,false,true);
            nameStr = CCString::createWithFormat(str_Format_liandian,yangXinInfo.getItemCount());
        }
            break;
        case 6:
        {
            SGOfficerDataModel *temp = staticDataManager->getOfficerById(yangXinInfo.getItemId());
            if (temp->getIconId() < 0) {
                break;
            }
            sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_tongQueLayer);
            int starlvl = temp->getOfficerCurrStarLevel();
            if (starlvl>1) {
                starlvl-=1;
            }
            visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),
                                           CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString(),
                                           NULL,NULL,CCPointZero,false,true);
            
            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(visitCardbg->getContentSize().width*0.28,visitCardbg->getContentSize().height - countryName->getContentSize().height*0.1));
            visitCardbg->addChild(countryName,30,30);
            
            if (temp->getAdvNum() > 0) {
                SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advNum->setInsideColor(ccGREEN);
                advNum->ignoreAnchorPointForPosition(false);
                advNum->setPosition(ccp(visitCardbg->getContentSize().width*0.8,visitCardbg->getContentSize().height - advNum->getContentSize().height/2));
                visitCardbg->addChild(advNum,40,40);
            }
            
            nameStr = CCString::createWithFormat("%sx%d",temp->getOfficerName()->getCString(),yangXinInfo.getItemCount());
            
        }
            break;
        case 7:
        {
            SGEquipmentDataModel *temp = staticDataManager->getEquipById(yangXinInfo.getItemId());
            if (temp->getIconId() < 0) {
                break;
            }
            sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_tongQueLayer);
            visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),
                                           CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString(),
                                           NULL,NULL,CCPointZero,false,true);
            
            if (temp->getAdvNum() > 0) {
                SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
                advNum->setInsideColor(ccGREEN);
                advNum->setAnchorPoint(ccp(1,1));
                advNum->ignoreAnchorPointForPosition(false);
                advNum->setPosition(ccp(visitCardbg->getContentSize().width*0.8,visitCardbg->getContentSize().height - advNum->getContentSize().height));
                visitCardbg->addChild(advNum,40,40);
            }
            
            nameStr = CCString::createWithFormat("%sx%d",temp->getEquipName()->getCString(),yangXinInfo.getItemCount());
            
        }
            break;
        case 9:
        {
            SGPropsDataModel *temp = staticDataManager->getPropsById(yangXinInfo.getItemId());
            if (temp->getIconId() < 0) {
                break;
            }
            sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_tongQueLayer);
            visitCardbg = SGButton::create("jinkuang.png",
                                           CCString::createWithFormat("head%d.png",temp->getIconId())->getCString(),
                                           NULL,NULL,CCPointZero,false,true);
            
            nameStr = CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),yangXinInfo.getItemCount());
        }
            break;
        case 10:
        {
            SGPiecesDataModel *pieces = staticDataManager->getPiecesDictById(yangXinInfo.getItemId());
            CCString *str_kuang = NULL;
            if (pieces->getCallingCardId() == -1)//如果是通用碎片
            {
                str_kuang = CCString::createWithFormat("common_pieces_border.png");
            }
            else if (pieces->getPiecesType() == 1)//装备碎片
            {
                str_kuang  = CCString::createWithFormat("equipPinzhi_%d_0.png",staticDataManager->getEquipById(pieces->getCallingCardId())->getEquipStarPinzhi());
            }
            else if (pieces->getPiecesType() == 0)//武将碎片
            {
                SGOfficerDataModel *temp = staticDataManager->getOfficerById(pieces->getCallingCardId());
                int starlvl = temp->getOfficerCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                str_kuang  = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
            }
            
            
            int iconId = 0;
            if (pieces->getPiecesType() == 0)//武将
            {
                SGOfficerDataModel *of = staticDataManager->getOfficerById(pieces->getCallingCardId());
                iconId = of->getIconId();
            }
            else if (pieces->getPiecesType() == 1)//装备
            {
                SGEquipmentDataModel *eq = staticDataManager->getEquipById(pieces->getCallingCardId());
                iconId = eq->getIconId();
            }
            if (iconId < 0) {
                break;
            }
            sgMainManager->addHeadIconPlistByNum(iconId,sg_tongQueLayer);
            
            visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png",iconId)->getCString(),str_kuang->getCString(),
                                           NULL,NULL,CCPointZero,false,true);
            if (pieces->getCallingCardId() != -1)//不是通用碎片，加上蒙版
            {
                CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
                mask->setPosition(ccp(visitCardbg->getContentSize().width / 2, visitCardbg->getContentSize().height / 2));
                visitCardbg->addChild(mask);
            }
            
            nameStr = CCString::createWithFormat("%sx%d",pieces->getPiecesName()->getCString(),yangXinInfo.getItemCount());
        }
            break;
        case 15:
        {
            SGConsumableDataModel *consume = SGStaticDataManager::shareStatic()->getConsumableById(yangXinInfo.getItemId());
            if (consume->getIconId() < 0) {
                break;
            }
            sgMainManager->addHeadIconPlistByNum(consume->getIconId(), sg_tongQueLayer);
            int starLvl = consume->getConsumeStarLvl();
            if (starLvl > 1)
                starLvl -= 1;
            visitCardbg = SGButton::create(CCString::createWithFormat("head%d.png", consume->getIconId())->getCString(),
                                           CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl)->getCString(),
                                           NULL,NULL,CCPointZero,false,true);
            
            nameStr = CCString::createWithFormat("%sx%d",consume->getConsumeName()->getCString(),yangXinInfo.getItemCount());
        }
            break;
        default:
            break;
    }
    if (visitCardbg) {
        itemName = SGCCLabelTTF::create(nameStr->getCString(), FONT_BOXINFO, 20,CCSizeMake(visitCardbg->getContentSize().width*0.9, 0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentTop , COLOR_YELLOW);
        itemName->setPosition(ccp(visitCardbg->getContentSize().width/2 , -itemName->getContentSize().height/2-4));
        //modify by:zyc. merge into create.
        //itemName->setColor(ccc3(0xff,0xf6,0x00));
        visitCardbg->addChild(itemName);
        visitCardbg->setEnabled(false);
    }
    return visitCardbg;
}

void SGYangXinBox::lingQuHandler(CCObject *obj)
{
    EFFECT_PLAY(MUSIC_BTN);
    main::TongQueYangXinRequest* req = new main::TongQueYangXinRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_TONGQUE_YANGXIN, req);
    boxClose();
}

void SGYangXinBox::confirmHandler(CCObject *obj)
{
    EFFECT_PLAY(MUSIC_BTN);
    boxClose();
}

void SGYangXinBox::boxClose()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer != NULL) {
        mainLayer->setIsCanTouch(true, false);
    }
    
    SGBaseBox::boxClose();
}

void SGYangXinBox::boxCloseWithOutCallBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer != NULL) {
        mainLayer->setIsCanTouch(true, false);
    }
    
    SGBaseBox::boxCloseWithOutCallBack();
}
