#include "SGMallCargoDetail.h"
#include "SGButton.h"
#include "SGMallData.h"
#include "SGShowString.h"
#include "SGMainManager.h"
#include "RoleModule.pb.h"
#include "SGMallLayer.h"

SGMallCargoDetail::SGMallCargoDetail() :
_cargoInfo(NULL),
_shopId(0)
{
    
}

SGMallCargoDetail::~SGMallCargoDetail()
{
    
}

SGMallCargoDetail * SGMallCargoDetail::create(CargoInformation * cargoInfo,int shopId)
{
    if (NULL == cargoInfo)
    {
        return NULL;
    }
    
    SGMallCargoDetail * pCargoDetail = new SGMallCargoDetail();
    if(pCargoDetail && pCargoDetail->init(NULL,box_cargoDetail,CCSize(510,520),false,false )  )
    {
        pCargoDetail->initView(cargoInfo,shopId);
        pCargoDetail->autorelease();
    }
    else
        CC_SAFE_DELETE(pCargoDetail);
    return pCargoDetail;
}


void SGMallCargoDetail::initView(CargoInformation * cargoInfo,int shopId)
{
    _cargoInfo = cargoInfo;
    _shopId = shopId;
    
    // 整个屏幕呈半透明状态
    CCLayerColor * layerColor = CCLayerColor::create(ccc4(0,0,0 , 180));
    this->addChild(layerColor , -10);
    
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    //底板
    CCSprite * spriteBg = CCSprite::createWithSpriteFrameName("mall_goodsDetailsDlgBorder.png");

    spriteBg->setPosition(center);
    this->addChild(spriteBg);
    
    CCSize sizeBg = spriteBg->getContentSize();
    
    //cargo descriptor with text
    SGCCLabelTTF * cargoDes = SGMallUtil::createLabelTitleByCargoId(cargoInfo->itemKind, cargoInfo->itemId,30,true);
    //缘分武将四个字
    SGCCLabelTTF * equipFate = NULL;
    if(cargoInfo->itemKind == 7)
    {
        equipFate = SGCCLabelTTF::create(str_cib_fate_officer,  FONT_BOXINFO  , 30 , COLOR_YELLOW);
        equipFate->setAnchorPoint(ccp(0, 0));
        equipFate->setPosition(ccp(45, 300));
        spriteBg->addChild(equipFate);
        
        cargoDes->setPosition(ccp(sizeBg.width/2 +5, sizeBg.height/2 -  equipFate->getContentSize().height));
        cargoDes->setInsideColor(ccWHITE);
    }
    else
        cargoDes->setPosition(ccp(sizeBg.width/2 , sizeBg.height/2));
    spriteBg->addChild(cargoDes);
    
    //cargo icon
    CCSprite * goods = SGMallUtil::createSpriteByCargoId(_cargoInfo->itemId, _cargoInfo->itemKind,_cargoInfo->itemCount);
    goods->setAnchorPoint(ccp(0.5f, 0.5f));
    goods->setPosition(ccp( 40 + goods->getContentSize().width/2 ,sizeBg.height - goods->getContentSize().height/2 -40 ) ) ;
    spriteBg->addChild(goods);
    //cargo name
    SGCCLabelTTF * cargoName = SGMallUtil::createLabelTitleByCargoId(cargoInfo->itemKind, cargoInfo->itemId , 34);
    cargoName->setAnchorPoint(ccp(0, 0.5f));
    cargoName->setPosition( ccp(20 + goods->getPositionX() + goods->getContentSize().width/2, sizeBg.height - cargoName->getContentSize().height/2 - 30 ) );
    spriteBg->addChild(cargoName);
    

    //背包已有该物品数量文本
    int num = SGMallUtil::getNumByCargoIdAndType(_cargoInfo->itemKind, _cargoInfo->itemId);
    SGCCLabelTTF * hasNumText = SGCCLabelTTF::create(str_MallLayer_str14, FONT_LISHU, 30);
    hasNumText->setAnchorPoint(ccp(0, 0.5f));
    hasNumText->setPosition(ccp( 20 + goods->getPositionX() + goods->getContentSize().width/2, goods->getPositionY() - goods->getContentSize().height/2 + hasNumText->getContentSize().height/2));
    spriteBg->addChild(hasNumText);
    //已有数量个数
    SGCCLabelTTF* hasNum = SGCCLabelTTF::create(CCString::createWithFormat("%d",num)->getCString(), FONT_LISHU, 30,CCSizeMake(100, 36),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter , COLOR_RED);
    hasNum->setPosition(ccp( 1.35 *hasNumText->getContentSize().width + hasNumText->getPositionX(), goods->getPositionY() - goods->getContentSize().height/2 + hasNumText->getContentSize().height/2));
    spriteBg->addChild(hasNum);
    
    
    //购买数量文本
    SGCCLabelTTF * buyNumText = SGCCLabelTTF::create(str_MallLayer_str15 , FONT_PANGWA, 32);
    buyNumText->setPosition(ccp(0.3*sizeBg.width,0.25*sizeBg.height) );
    spriteBg->addChild(buyNumText);
    
    //购买个数
    SGCCLabelTTF * buyNum = SGCCLabelTTF::create(CCString::createWithFormat("%d",_cargoInfo->itemCount)->getCString(), FONT_LISHU,32 , COLOR_RED);
    buyNum->setPosition(ccp(  0.33*sizeBg.width, 0.25*sizeBg.height));
    spriteBg->addChild(buyNum);
    
    //购买需要金钱图标和总额
    CCSprite * payKind = SGMallUtil::createSpriteByPayType(_cargoInfo->payKind);
    payKind->setPosition(ccp( 0.57 *sizeBg.width, 0.25*sizeBg.height ));
    spriteBg->addChild(payKind,10);
    
    SGCCLabelTTF * needMoney = SGCCLabelTTF::create(CCString::createWithFormat("%d",_cargoInfo->currentPrice)->getCString(), FONT_PANGWA, 26, COLOR_GREEN);
    needMoney->setPosition(ccp(0.7*sizeBg.width,0.25*sizeBg.height) );
    spriteBg->addChild(needMoney,10);
    
    //金额墨水图标
    CCSprite * spriteInk = 	CCSprite::createWithSpriteFrameName("name_bg.png");
    spriteInk->setPosition(ccp(0.68 * sizeBg.width , 0.25 * sizeBg.height));
    spriteBg->addChild(spriteInk);
    
    //购买按钮 ，此时需要发出购买的消息
    SGButton * buyButton = SGButton::createFromLocal("reward_btn_redbg.png", str_MallLayer_str2, this, menu_selector(SGMallCargoDetail::onBuyClicked ),ccp(0,3),FONT_PANGWA,ccWHITE,32);
    buyButton->setPosition(ccp(spriteBg->getPositionX() - 0.75*buyButton->getContentSize().width , spriteBg->getPositionY() - sizeBg.height/2 + buyButton->getContentSize().height/2 + 20));
    addBtn(buyButton);
    
    //取消，删除该界面
    SGButton * cancelBtn = SGButton::createFromLocal("reward_btn_redbg.png", str_MallLayer_str3, this, menu_selector(SGMallCargoDetail::onCancelClicked ),ccp(0,3),FONT_PANGWA,ccWHITE,32);
    cancelBtn->setPosition(ccp(spriteBg->getPositionX() + 0.75*cancelBtn->getContentSize().width , spriteBg->getPositionY() - sizeBg.height/2 + cancelBtn->getContentSize().height/2 + 20) );
    addBtn(cancelBtn);
    
}


void SGMallCargoDetail::onBuyClicked(cocos2d::CCObject * pSend)
{
    CCLog("buy button clicked.");
    
    switch (_cargoInfo->cargoState)
    {
        case 0:
        {
            //保存购买物品的cargoinfomation指针 ，购买成功时修改状态
            SGMallLayer::_cargoInfoBuySuccess = _cargoInfo;
            main::CargoBuyRequest *request = new main::CargoBuyRequest;
            request->set_cargoid(_cargoInfo->cargoId);
            request->set_shopid(_shopId);
            SGSocketClient::sharedSocketClient()->send(MSG_CARGO_TRY_TO_BUY, request);
            
            
            //服务器返回上次购买消息后   才可以继续点击
            if (SGMallLayer::_isBuyResponsed)
            {
                SGMallLayer::_isBuyResponsed = false ;
            }
        }
            break;

        case 1:
        {
            SGMainManager::shareMain()->showMessage(str_MallLayer_str19);
        }
            break;
        case 2:
        {
            
        }
            SGMainManager::shareMain()->showMessage(str_MallLayer_str19);
            break;
        default:
        {
            
        }
            break;
    }
    
    //关闭窗口   恢复触摸。
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true,false);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(true);
    }
    this->boxCloseWithOutCallBack();
}

void SGMallCargoDetail::onCancelClicked(cocos2d::CCObject * pSend)
{
    CCLog("cancel button clicked.");
    this->boxClose();
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true,false);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(true);
    }

}
