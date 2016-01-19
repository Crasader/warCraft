//
//  SGCrditCardCell.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-26.
//
//

#include "SGCreditCardCell.h"
#include "SGStaticDataManager.h"
#include "SGGeneralInfoLayer.h"
#include "SGStoryRewardBox.h"
#include "ResourceManager.h"
#include "SGDrawCards.h"
#include "SGStringConfig.h"

SGCreditCardCell::SGCreditCardCell(void)
:spriteIcon(NULL),     // 卡牌头像
spriteIconselect(NULL),    // 卡牌头选中
headIcon(NULL),       // 可点击头像 武将／装备／道具
labelOriCount(NULL), // 原价
labelSLevel(NULL),   // 卡牌星级
spriteStar(NULL),      // 主将星级
labelName(NULL),      // 主将星级
labelCount(NULL),     // 道具消耗类个数
labelCanNum(NULL),// 可兑换次数
labelCLevel(NULL),     // 所需等级
labelCost(NULL),       // 兑换所需积分
redline(NULL),
spriteBackground(NULL),
bgSize(CCSizeMake(0, 0)),
itemType(0),
itemId(0),  // ItemId, 通过Id显示Item信息
cardType(-1)// 卡牌类型 // 0 officer 1 equip 2 soldier 3 prop
{

}
SGCreditCardCell::~SGCreditCardCell(void)
{

}

SGCreditCardCell* SGCreditCardCell::create()
{
    SGCreditCardCell *cell = new SGCreditCardCell();
    
    if (cell && cell->init()) {
        cell->viewDidLoad();
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

bool SGCreditCardCell::init()
{
    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_creditCardCell);
    
    if (!SNSTableViewCellItem::init()) {
        return false;
    }

    spriteBackground = CCSprite::createWithSpriteFrameName("upwar_itembg.png");
    bgbgbg = spriteBackground;

    this->addChild(spriteBackground, -10);
    
    bgSize = spriteBackground->getContentSize();
    
    return true;
}

void SGCreditCardCell::viewDidLoad()
{
//    float dx = 0;
    float fontSize = 24;
    const char *fontName = FONT_BOXINFO;
    
    CCSprite *ss = CCSprite::createWithSpriteFrameName("head1001.png");
    CCSprite *ss2 = CCSprite::createWithSpriteFrameName("head1001.png");
    
    headIcon = SGButton::createFromSprite(ss,ss2, this,
                                menu_selector(SGCreditCardCell::showGeneralInfo), ccp(0, 0), false, true, true);
    headIcon->setPosition(ccp(-bgSize.width/2+headIcon->getContentSize().width/2+25, bgSize.height/2-headIcon->getContentSize().height/2-60));
    headIcon->setEnabled(false);
    headIcon->setVisible(false);
    CCMenu *menu = CCMenu::create(headIcon, NULL);
    menu->setPosition(ccp(0, 0));
    this->addChild(menu, 10);
    this->addBtn(headIcon);
    headIcon->setTag(11);

    
    // 星级 暂时用不到
    labelSLevel = SGCCLabelTTF::create("4 ★", fontName, FONTSIZE(16) , ccGRAY);
    labelSLevel->setPosition(headIcon->getPosition());
    //modify by:zyc. merge into create.
    //labelSLevel->setColor(ccGRAY);
    this->addChild(labelSLevel, 11);
    labelSLevel->setVisible(false);
    
    labelName = SGCCLabelTTF::create(str_CreditCardCellstr1,FONT_PANGWA, 26);
//    labelName->setAnchorPoint(ccp(0, 0.5f));
    labelName->setPosition(ccpAdd(spriteBackground->getPosition(),ccp(0, spriteBackground->getContentSize().height*.35)));
    this->addChild(labelName);
//    ccp(spriteIconBg->getContentSize().width*.55, spriteIconBg->getContentSize().height*.4 - 10))
    // 消耗类道具 个数
    labelCount = SGCCLabelTTF::create(str_CreditCardCellstr2, fontName, fontSize);
    labelCount->setAnchorPoint(ccp(0, 0.5f));
    labelCount->setPosition(ccpAdd(labelName->getPosition(), ccp(-10, -35)));
    this->addChild(labelCount);
    labelCount->setVisible(false);
    
    labelCLevel = SGCCLabelTTF::create(str_CreditCardCellstr3, fontName, 21 , ccc3(0xe7, 0xc4, 0x6a));
//    labelCLevel->setAnchorPoint(ccp(1, 0.5f));
    labelCLevel->setPosition(ccpAdd(spriteBackground->getPosition(), ccp(0, -spriteBackground->getContentSize().height*.4)));
    
    this->addChild(labelCLevel);
    
    labelCost = SGCCLabelTTF::create(str_CreditCardCellstr4, fontName, fontSize , ccYELLOW);
    labelCost->setAnchorPoint(ccp(0, 0.5f));
    labelCost->setPosition(ccpAdd(labelName->getPosition(), ccp(-10, -70)));
    //modify by:zyc. merge into create.
    //labelCost->setColor(ccYELLOW);
    this->addChild(labelCost);
    
    labelOriCount = SGCCLabelTTF::create("LV", fontName, 24);
    labelOriCount->setAnchorPoint(ccp(0, 0.5f));
    labelOriCount->setPosition(ccpAdd(labelCost->getPosition(), ccp(0, 30)));
    this->addChild(labelOriCount);
    labelOriCount->setVisible(false);
    
    labelCanNum = SGCCLabelTTF::create(str_CreditCardCellstr5, fontName, 17);
//    labelCanNum->setAnchorPoint(ccp(1, 0.5f));
    labelCanNum->setPosition(ccpAdd(labelName->getPosition(), ccp(50, -115)));
    this->addChild(labelCanNum);
    
    
//    redline = CCSprite::create("store/store_hongxian.png",CCRect(0, 2, 130, 2));
    redline = CCSprite::createWithSpriteFrameName("store_hongxian.png");
    redline->setAnchorPoint(ccp(0, 0.5));
    this->addChild(redline,1);
    redline->setPosition(ccpAdd(labelOriCount->getPosition(), ccp(-5, -22)));
    redline->setVisible(false);
}


// reset all the cell's infomation
void SGCreditCardCell::setData(SGCreditCardModel* model)
{
    CCString *string = NULL;
    itemType = model->getModelType();
    itemId = model->getitemId();
    
    CCString *cstring = CCString::createWithFormat(str_CreditCardCellstr6, model->getDisCount());
    labelOriCount->setVisible(false);
    int var1 = model->getpvpLev();
    int var2 = model->getCurrCount();
    int var3 = model->getMaxCout();
    int var4 = model->getCostScore();
    
    if (var4 > model->getDisCount()) {
        labelOriCount->setVisible(true);
        labelOriCount->setString(CCString::createWithFormat(str_CreditCardCellstr7, var4)->getCString());
        labelCost->setString(CCString::createWithFormat(str_CreditCardCellstr8, model->getDisCount())->getCString());
        
        labelCost->setPosition(ccpAdd(labelName->getPosition(), ccp(-10, -87)));
        labelOriCount->setPosition(ccpAdd(labelCost->getPosition(), ccp(0, 25)));
        redline->setVisible(true);
        CCSprite *font = CCSprite::createWithSpriteFrameName("upwar_youhui.png");
        this->addChild(font);
        font->setPosition(ccpAdd(spriteBackground->getPosition(), ccp(spriteBackground->getContentSize().width*.35-5, spriteBackground->getContentSize().height*.45)));
        
    }else
    {
        labelCost->setString(cstring->getCString());
        labelCost->setPosition(ccpAdd(labelName->getPosition(), ccp(-10, -70)));
        redline->setVisible(false);
    }
    
    cstring = CCString::createWithFormat(str_CreditCardCellstr9, var1);
    labelCLevel->setString(cstring->getCString());
    cstring = CCString::createWithFormat(str_CreditCardCellstr10, var2, var3);
    labelCanNum->setString(cstring->getCString());
    SGMainManager::shareMain() -> addHeadIconPlistByNum(9001,sg_creditCardCell);
   
    if (itemType >=0 && itemType<=5) {
        labelCount->setVisible(true);
        string = CCString::createWithFormat(str_CreditCardCellstr11, itemId);
        labelCount->setString(string->getCString());
        int drawitemid =0;
//        labelName->setPosition(ccp(posX1, skewing(9)));
//        const char *bgName = "";
        if (itemType == 1) {
            labelName->setString(str_Tong_money);
//            bgName = "head9999.png";
            drawitemid =9999;
        } else if (itemType == 3) {
            labelName->setString(str_jungong);
//            bgName = "head9997.png";
            drawitemid =9997;
        } else if (itemType == 4) {
            labelName->setString(str_Small_laba);
//            bgName = "head9998.png";
            drawitemid = 9998;
        } else if (itemType == 5) {
            labelName->setString(str_liandianmifang);
//            bgName = "head9996.png";
            drawitemid = 9996;
        } else if (itemType == 0) {
            labelName->setString(str_yuanbao);
//            bgName = "big_yb.png";
            drawitemid =10000;
        }
        spriteIcon = SGDrawCards::drawcoinetccard(drawitemid, sg_creditCardCell);
        spriteIconselect = SGDrawCards::drawcoinetccard(drawitemid, sg_creditCardCell);
        headIcon ->init(spriteIcon,spriteIconselect,this,menu_selector(SGCreditCardCell::showGeneralInfo),ccp(0,0), false,true,true);

    }else {
        int bgTag = 0;
        int starLev = 0;
        int maxstar = 0;
        const char *cardName = "";
        SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
//        要加 modelType
        /*modelType
         1 SGEquipmentDataModel
         2 SGSoldierDataModel
         3 SGPropsDataModel
         0 SGOfficerDataModel
         4 SGPiecesDataModel
         5 SGConsumableDataModel
         */
        cardType = model->getModelType();
        switch (model->getModelType()) {
            case 6:
            {
//                cardType = 0;
                SGOfficerDataModel *model = manager->getOfficerById(itemId);
//                bgTag = model->getOfficerRace();
                starLev = model->getOfficerCurrStarLevel();
//                maxstar = model->getUpgradestar();
                cardName = model->getOfficerName()->getCString();
//                advancenumber = model->getAdvNum();
                spriteIcon = SGDrawCards::drawofficercard(itemId, sg_creditCardCell);
                spriteIconselect = SGDrawCards::drawofficercard(itemId, sg_creditCardCell);
                
            }
                break;
            case 7:
            {
//                cardType = 1;
                SGEquipmentDataModel *model = manager->getEquipById(itemId);
//                bgTag = model->getEquipType();
                starLev = model->getEquipCurrStarLevel();
//                maxstar = model->getUpgradestar();
                cardName = model->getEquipName()->getCString();
//                advancenumber = model->getAdvNum();
                spriteIcon = SGDrawCards::drawequipcard(itemId,sg_creditCardCell);
                spriteIconselect = SGDrawCards::drawequipcard(itemId,sg_creditCardCell);

            }
                break;
            case 8:
            {
//                cardType = 2;
                SGSoldierDataModel *model = manager->getSoliderById(itemId);
                bgTag = model->getSoldierRace();
                cardName = model->getSoldierName()->getCString();

            }
                break;
            case 9:
            {
//                cardType = 3;
                SGPropsDataModel *model = manager->getPropsById(itemId);
//                bgTag = model->getPropsCountry();
                starLev = model->getPropsCurrStarLevel();
                cardName = model->getPropsName()->getCString();
                spriteIcon = SGDrawCards::drawpropcard(itemId,sg_creditCardCell);
                spriteIconselect = SGDrawCards::drawpropcard(itemId,sg_creditCardCell);
            }
                break;
            
            case 10:
            {
//                cardType = 4;
                SGPiecesDataModel *model = manager->getPiecesDictById(itemId);
//                bgTag = model->getPropsCountry();
//                starLev = model->getPropsCurrStarLevel();
                cardName = model->getPiecesName()->getCString();
                spriteIcon = SGDrawCards::drawpiececard(itemId,sg_creditCardCell);
                spriteIconselect = SGDrawCards::drawpiececard(itemId,sg_creditCardCell);
				starLev = model->getStarLevel();
            }
                break;
            case 15:
            {
//                cardType=5;
                SGConsumableDataModel *model = manager->getConsumableById(itemId);
                cardName = model->getConsumeName()->getCString();
                starLev = model->getConsumeStarLvl();
                spriteIcon = SGDrawCards::drawconsumecard(itemId,sg_creditCardCell);
                spriteIconselect = SGDrawCards::drawconsumecard(itemId,sg_creditCardCell);

            }

            default:
                break;
        }
          this->drawStars(starLev,maxstar);
        
//        CCString *bgName = NULL; // Icon background
//        if (cardType == 0) { // 0 officer
//           spriteIcon = SGDrawCards::drawofficercard(itemId, sg_creditCardCell);
//           spriteIconselect = SGDrawCards::drawofficercard(itemId, sg_creditCardCell);
//        }else if (cardType == 1) { // 1 equip
//            spriteIcon = SGDrawCards::drawequipcard(itemId,sg_creditCardCell);
//            spriteIconselect = SGDrawCards::drawequipcard(itemId,sg_creditCardCell);
//        }else if (cardType == 2) { // 2 soldier 积分兑换离没士兵卡
////            spName = CCString::createWithFormat("%d-%d.png", bgTag, 3);
////            //int contry = SGPlayerInfo::sharePlayerInfo()->getPlayerCountry();
////            bgName = CCString::createWithFormat("equipPinzhi_1_0.png");
//            
//        }else if (cardType == 3) { // 3 prop
//            spriteIcon = SGDrawCards::drawpropcard(itemId,sg_creditCardCell);
//            spriteIconselect = SGDrawCards::drawpropcard(itemId,sg_creditCardCell);
//        }
//        else if (cardType==4)
//        {
//            spriteIcon = SGDrawCards::drawpiececard(itemId,sg_creditCardCell);
//            spriteIconselect = SGDrawCards::drawpiececard(itemId,sg_creditCardCell);
//        }
//        else if (cardType==5)
//        {
//            spriteIcon = SGDrawCards::drawconsumecard(itemId,sg_creditCardCell);
//            spriteIconselect = SGDrawCards::drawconsumecard(itemId,sg_creditCardCell);
//        }
//        if (bgName) {
//            CCSpriteFrame *frame = CCSprite::createWithSpriteFrameName(bgName->getCString())->displayFrame();
//            headIcon->setFontImage(frame);
//        }
//
//        spriteIcon->setVisible(false);
//        spriteIconBg->setVisible(false);
		
		if (spriteIcon && spriteIconselect)
		{
			headIcon->init(spriteIcon,spriteIconselect,this,menu_selector(SGCreditCardCell::showGeneralInfo),ccp(0,0), false,true,true);
			headIcon->setVisible(true);
			headIcon->setEnabled(true);
		}

        
        labelCount->setVisible(false);
        labelName->setString(cardName);
    }
}

void SGCreditCardCell::showGeneralInfo()
{
    EFFECT_PLAY(MUSIC_BTN);
    // 0 officer 1 equip 2 soldier 3 prop
    SGBaseMilitaryCard *_card = getCardWithIdAndType(itemId, cardType);
   // CCLog("%s",_card->getClassName());
    if (_card != NULL) {
        CCString *str_ = CCString::create(_card->getClassName());
        if (str_->isEqual(CCString::create("SGPropsCard"))) {
            SGPropsCard *card = (SGPropsCard*)_card;
            if (card->getType() == 4) {
                SGMainManager::shareMain()->showSkillCardInfoLayer(_card, -2,false);
                
            }else{
//
            SGMainManager::shareMain()->showPropInfoLayer((SGPropsCard *)_card, 10, false);
            }
        }else if(str_->isEqual(CCString::create("SGOfficerCard")) ) {
            SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard *)_card, 20,NULL,false);
        }else if(str_->isEqual(CCString::create("SGEquipCard"))){
            SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)_card, 10, false);
        }
    else if(str_->isEqual(CCString::create("SGConsumableCard")))
    {
        SGMainManager::shareMain()->showConsumableInfoLayer((SGConsumableCard *)_card, false,10);
    }
    else if(str_->isEqual(CCString::create("SGPiecesCard")))
    {
        SGMainManager::shareMain()->showPiecesInfoLayer((SGPiecesCard *)_card, 10,false);
    }
    }
}

void SGCreditCardCell::drawStars(int count,int star)
{
    if (spriteStar) {
//        spriteStar->removeAllChildrenWithCleanup(true);
        spriteStar->removeFromParentAndCleanup(true);
        spriteStar = NULL;
    }
    spriteStar = spriteWithStars(count);
    spriteStar->setScale(.7f);
    spriteStar->setAnchorPoint(ccp(0, 0.5f));
    spriteStar->setPosition(ccpAdd(labelName->getPosition(), ccp(-10, -32)));
    this->addChild(spriteStar);
    for (int ii = count; ii < star; ii++) {
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("bluestar.png");
        
        sprite->setPosition(ccpAdd(spriteStar->getChildByTag(ii)->getPosition(), ccp(0,-1)));
                            
        spriteStar->addChild(sprite);
    }

}
void SGCreditCardCell::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    //CCPoint touchPt = pTouch->getLocation();
}
