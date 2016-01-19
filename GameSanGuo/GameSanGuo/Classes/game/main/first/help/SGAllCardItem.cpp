//
//  SGAllCardItem.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-10.
//
//
#include "SGMainManager.h"
#include "SGAllCardItem.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#define COUNTRYNAMETAG 30
SGAllCardItem::SGAllCardItem()
:
_card(NULL),
icon(NULL),
kuang(NULL),
m_name(NULL),
m_namebg(NULL)
{
}

SGAllCardItem::~SGAllCardItem()
{
	
	//ResourceManager::sharedInstance()->unBindLayerTexture(sg_cardsLayer);

}

void SGAllCardItem::initView(SGBaseMilitaryCard *card,int num)
{

	
    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_cardsLayer);
    bgbgbg = NULL;
    
    
    if (card)
    {
        CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
        this->addChild(bg);
        icon = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",1001)->getCString());
        this->addChild(icon);
        kuang = CCSprite::createWithSpriteFrameName("jinkuang.png");
        this->addChild(kuang);
        kuang->setPosition(icon->getPosition());
        
        m_namebg = CCScale9Sprite::createWithSpriteFrameName("carditem_infobg.png");
        m_namebg->setPreferredSize(CCSize(kuang->getContentSize().width , m_namebg->getContentSize().height));
        m_namebg->setPosition(ccp(kuang->getPositionX() +1.5 , kuang->getPositionY() - kuang->getContentSize().width * 0.5 - m_namebg->getContentSize().height *0.5));
        this->addChild(m_namebg);
        
        m_name = SGCCLabelTTF::create("", FONT_BOXINFO,20);//cgp
        m_name->setPosition(m_namebg->getPosition());
        this->addChild(m_name);
        if (num != 1) {
            kuang->setColor(ccc3(100, 100, 100));
            icon->setColor(ccc3(100, 100, 100));
            m_name->setInsideColor(ccc3(100, 100, 100));
        }
    }
    else
    {
        CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
        this->addChild(bg);
        icon = CCSprite::create();
        this->addChild(icon);
        kuang = CCSprite::createWithSpriteFrameName("unknownkuang.png");
        this->addChild(kuang);
        
        m_namebg = CCScale9Sprite::createWithSpriteFrameName("carditem_infobg.png");
        m_namebg->setPreferredSize(CCSize(kuang->getContentSize().width , m_namebg->getContentSize().height));
        m_namebg->setPosition(ccp(kuang->getPositionX() + 1.5, kuang->getPositionY() - kuang->getContentSize().width * 0.5 - m_namebg->getContentSize().height * 0.5 ));
        this->addChild(m_namebg);
        m_name = SGCCLabelTTF::create("", FONT_BOXINFO,20);//cgp
        m_name->setPosition(m_namebg->getPosition());
        this->addChild(m_name);
    }
    
    updataCard(card,num);
}
SGAllCardItem* SGAllCardItem::create(int num)
{
	SGAllCardItem *instance = new SGAllCardItem();
	if (instance&&instance->init()) {
        instance->initView(NULL,num);
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}
SGAllCardItem* SGAllCardItem::create(SGBaseMilitaryCard *card,int num)
{
	SGAllCardItem *instance = new SGAllCardItem();
	if (instance &&instance->init()) {
        instance->initView(card,num);
		instance->autorelease();
		return instance;
	}
    
	CC_SAFE_DELETE(instance);
	return NULL;
}
void SGAllCardItem::updataCard(SGBaseMilitaryCard *card,int state)
{
    if (card) {
        _card = card;
        CCString *str_ = CCString::create(_card->getClassName());
        CCSprite *temp = NULL;
        CCSprite *tempk = NULL;

        SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_cardsLayer);
        
        if(str_->isEqual(CCString::create("SGOfficerCard")))
        {
            
            if (_card->getHeadSuffixNum() != -1)
            {
                temp = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",_card->getHeadSuffixNum())->getCString());//12.05
                
                SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
                int starlvl = temp->getOfficerCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                tempk = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
            }
        }
        else if(str_->isEqual(CCString::create("SGEquipCard")))
        {
			SGEquipmentDataModel *equipTest = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());//获取对应的equip的id
			temp = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",equipTest->getIconId())->getCString());//12.06
            //tempk = CCSprite::createWithSpriteFrameName(str->getCString());
            
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
            tempk = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString());
            
        }
        else
        {
            if (_card->getHeadSuffixNum()!=0) {
                temp = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",_card->getHeadSuffixNum())->getCString());//12.06
            }
            else
            {
                SGPropsDataModel *model = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
                temp = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",model->getIconId())->getCString());//12.06

            }
			
            tempk = CCSprite::createWithSpriteFrameName("equipPinzhi_4.png");
        }
        
        if (icon && temp)
        {
            icon->setDisplayFrame(temp->displayFrame());
            icon->setVisible(true);
            kuang->setDisplayFrame(tempk->displayFrame());
            m_name->setString(_card->getOfficerName()->getCString());
            m_namebg->setVisible(true);
            if (str_->isEqual(CCString::create("SGOfficerCard")))
            {
                kuang->removeChildByTag(COUNTRYNAMETAG);
                CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",_card->getRace()))->getCString());
                countryName->setAnchorPoint(ccp(1,1));
                countryName->ignoreAnchorPointForPosition(false);
                countryName->setPosition(ccp(kuang->getContentSize().width*0.35,kuang->getContentSize().height - countryName->getContentSize().height));
                kuang->addChild(countryName,COUNTRYNAMETAG,COUNTRYNAMETAG);
                
            }
            if (state != 1) {
                icon->setColor(ccc3(100, 100, 100));
                kuang->setColor(ccc3(100, 100, 100));
                m_name->setInsideColor(ccc3(100,100,100));
                if (kuang->getChildByTag(COUNTRYNAMETAG)) {
                    ((CCSprite *)kuang->getChildByTag(COUNTRYNAMETAG))->setColor(ccc3(100, 100, 100));
                }
            }
            else
            {
                icon->setColor(ccWHITE);
                kuang->setColor(ccWHITE);
                m_name->setInsideColor(ccWHITE);
                if (kuang->getChildByTag(COUNTRYNAMETAG)) {
                    ((CCSprite *)kuang->getChildByTag(COUNTRYNAMETAG))->setColor(ccWHITE);
                }
            }
        }
    }
    else
    {
        icon->setVisible(false);
        m_namebg->setVisible(false);
        m_name->setString(" ");
        CCSprite *tempk = CCSprite::createWithSpriteFrameName("unknownkuang.png");
        kuang->setDisplayFrame(tempk->displayFrame());
        kuang->setColor(ccWHITE);
        kuang->removeChildByTag(COUNTRYNAMETAG);
    }
        
}
    
