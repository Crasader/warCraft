#include "SGMallCell.h"
#include "SGMainManager.h"
#include "SGMallCargoDetail.h"
#include "SGMallLayer.h"

SGMallCell::SGMallCell():
_cargoInfo(NULL),
_mallLayer(NULL),
btn(NULL),
_sellOut(NULL),
_contentLayer(NULL)
{
    
}


SGMallCell::~SGMallCell()
{
    
}


SGMallCell * SGMallCell::create(SGMallLayer * mallLayer,CargoInformation * cargoInfo)
{
    if ( cargoInfo == NULL)
    {
        return NULL;
    }
    
    SGMallCell * pMallCell = new SGMallCell();
    if ( pMallCell && pMallCell->init() )
    {
        pMallCell->initView(mallLayer ,cargoInfo);
        pMallCell->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pMallCell);
        pMallCell = NULL;
    }
    return pMallCell;
}


void SGMallCell::initView(SGMallLayer * mallLayer , CargoInformation * cargoInfo)
{
    _cargoInfo = cargoInfo;
    _mallLayer = mallLayer;
    
    _contentLayer = CCLayer::create();
    
    //底板
    btn = SGButton::create("mall_goodsBorderDark.png", NULL, this, menu_selector(SGMallCell::onClicked ) );
    btn->setCascadeColorEnabled(true);
    _contentLayer->addChild(btn);
    addBtn(btn);
    //this->setAnchorPoint(ccp(0.5f, 0.5f));
    
    // title
    //SGCCLabelTTF * labelTitle = SGCCLabelTTF::create(cargoInfo->name.c_str(), FONT_PANGWA, 26);
    SGCCLabelTTF * labelTitle = SGMallUtil::createLabelTitleByCargoId(_cargoInfo->itemKind, _cargoInfo->itemId );
    labelTitle->setCascadeColorEnabled(true);
    labelTitle->setPosition(ccp(btn->getPositionX()  , btn->getPositionY() + btn->getContentSize().height/2 - 30    ));
    _contentLayer->addChild(labelTitle);
    

    
    //具体商品的图片
    //CCSprite * goods = CCSprite::createWithSpriteFrameName("countryIcon_1.png");
    CCSprite * goods = SGMallUtil::createSpriteByCargoId(_cargoInfo->itemId, _cargoInfo->itemKind,_cargoInfo->itemCount , true);
    goods->setCascadeColorEnabled(true);
    goods->setPosition(btn->getPosition());
    _contentLayer->addChild(goods);

    //售出图片
    _sellOut = CCSprite::createWithSpriteFrameName("sellOut.png");
    //_sellOut->setCascadeColorEnabled(true);
    _sellOut->setPosition( btn->getPosition() );
    _sellOut->setVisible(false);
    this->addChild(_sellOut,100);
    
    //物品代币方式
    CCSprite * money = SGMallUtil::createSpriteByPayType(_cargoInfo->payKind);
    money->setCascadeColorEnabled(true);
    money->setPosition(ccp(-50, -70));
    _contentLayer->addChild(money);
    
    char temp[50];
    // oldPrice
//    sprintf(temp, "%d",cargoInfo->oldPrice);
//    SGCCLabelTTF * oldPrice = SGCCLabelTTF::create(temp, FONT_PANGWA, 26);
//    oldPrice->setPosition(money->getPosition() + ccp(50, 0) );
//    this->addChild(oldPrice);
    
    // nowPrice
    sprintf(temp, "%d",cargoInfo->currentPrice ); //显示需消费总金额
    SGCCLabelTTF * nowPrice = SGCCLabelTTF::create(temp, FONT_PANGWA, 26);
    nowPrice->setCascadeColorEnabled(true);
    nowPrice->setPosition(money->getPosition() + ccp(55, 0));
    _contentLayer->addChild(nowPrice);
    
    //判断商品的状态
    switch (_cargoInfo->cargoState)
    {
        case 0: //正常可购买状态
            //this->setGray(true);
            break;
            
        case 1:   // 1 售出，2，限制购买中
            this->setGray(true);
            break;
        case 2:
            this->setGray(true);
            break;
        default:
            break;
    }
    this->addChild(_contentLayer);
}

//设置本控件为灰色/原色。
void SGMallCell::setGray(bool isGray)
{
    CCArray* chds = _contentLayer->getChildren();
    ccColor3B color = isGray ? ccc3(90,90,90) : ccc3(255,255,255);
    bool isTouchEnable= isGray ? false : true;
    CCObject* obj = NULL;
    CCRGBAProtocol* rgba = NULL;

    CCARRAY_FOREACH(chds, obj)
    {
        rgba = dynamic_cast<CCRGBAProtocol*>(obj);
        if (rgba != NULL)
        {
            rgba->setColor(color);
        }

    }
    btn->setEnabled(isTouchEnable);
    
    //加灰  = 打上售完图片
    _sellOut->setVisible(true);
}


void SGMallCell::onClicked(cocos2d::CCObject *pSend)
{
    //服务器返回上次购买消息后   才可以继续点击
    if ( !_mallLayer->_isBuyResponsed)
    {
        return ;
    }
    SGBaseBox * cargoDetail = SGMallCargoDetail::create(_cargoInfo,_mallLayer->_mallData->_mallId);
    SGMainManager::shareMain()->showBox(cargoDetail);
    
    //记录点击的面板
    SGMallLayer::_buyMallCell = this;
}




