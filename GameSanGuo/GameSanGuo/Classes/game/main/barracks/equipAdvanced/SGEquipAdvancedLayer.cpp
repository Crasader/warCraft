//
//  SGEquipAdvancedLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-10-22.
//
//

#include "SGEquipAdvancedLayer.h"
#include "ResourceManager.h"
#include "SGShowString.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGAdvancedEffectLayer.h"
#include "SGLootWiKiMainLayer.h"
#include "SGGuideManager.h"

#define ICON_SCALE  0.8


SGEquipAdvancedLayer::SGEquipAdvancedLayer():
willAdvancedEquipCard(NULL),
afterAdvacedEquipCard(NULL),
quipAdvancedBtn(NULL),
willAdvancedDict(NULL),
canAdvancedEquip(true)
{
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true , false);
}

SGEquipAdvancedLayer::~SGEquipAdvancedLayer()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_EQUIP_ADVANCE);
}

SGEquipAdvancedLayer *SGEquipAdvancedLayer::create(SGEquipCard *willAdvancedCard, int enterType)
{
    SGEquipAdvancedLayer *equip = new SGEquipAdvancedLayer();
    if (equip && equip->init(NULL, sg_equipAdvancedLayer))
    {
        equip->willAdvancedEquipCard = willAdvancedCard;
        equip->_enterType = enterType;
        equip->initView();
        equip->autorelease();
        return equip;
    }
    CC_SAFE_RELEASE(equip);
    return NULL;
}


void SGEquipAdvancedLayer::initView()
{
    
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    //消息监听
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_EQUIP_ADVANCE, this, callfuncO_selector(SGEquipAdvancedLayer::advancedListener));
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg);
    
    CCSprite *font_title = CCSprite::createWithSpriteFrameName("barrack_font_kpzssx.png");
    font_title->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.5)));
    this->addChild(font_title);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(titlecenter,100);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_l,100);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_r,100);
    
    
    CCSprite *lanbg = CCSprite::createWithSpriteFrameName("advance_bg_lan.png");
    
    lanbg->setPosition(ccpAdd(center, ccp(0,250)));
    
    
    
    CCSprite *redbg = CCSprite::createWithSpriteFrameName("advance_bg_hong.png");
    redbg->setFlipX(true);
    this->addChild(redbg);
    redbg->setPosition(ccpAdd(center, ccp(-6, -40)));
    
    CCSprite *jiantou = CCSprite::createWithSpriteFrameName("advance_jiantou.png");
    jiantou->setPosition(ccpAdd(lanbg->getPosition(), ccp(0,-lanbg->getContentSize().height*.35)));
    this->addChild(jiantou);
    
    this->addChild(lanbg);
    
//    CCSprite *titlebg = CCSprite::createWithSpriteFrameName("barrack_title_bg.png");
//    this->addChild(titlebg);
//    titlebg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,titlebg->getContentSize().height/2)));
//    
//    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("streng_huawen.png");
//    gonggao_bg_3_l->setAnchorPoint(ccp(0.5, 1));
//    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft),ccp(0,titlebg->getContentSize().height)));
//    this->addChild(gonggao_bg_3_l,3);
//    
//    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("streng_huawen.png");
//    gonggao_bg_3_r->setFlipX(true);
//    gonggao_bg_3_r->setAnchorPoint(ccp(0.5, 1));
//    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft),ccp(s.width,titlebg->getContentSize().height)));
//    this->addChild(gonggao_bg_3_r,3);
//    
//    CCSprite *coinIcon = CCSprite::createWithSpriteFrameName("coinicon.png");
//    this->addChild(coinIcon);
//    coinIcon->setPosition(ccpAdd(titlebg->getPosition(), ccp(160, 0)));
//    
//    SGCCLabelTTF *currCoins = SGCCLabelTTF::create("----", FONT_BOXINFO, 24);
//    this->addChild(currCoins);
//    currCoins->setPosition(ccpAdd(coinIcon->getPosition(), ccp(-currCoins->getContentSize().width/2 - coinIcon->getContentSize().width/2 - 12, 0)));
//    SGCCLabelTTF *coins = SGCCLabelTTF::create(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getPlayerCoins())->getCString(), FONT_BOXINFO, FONTSIZE(12));
//    coins->setAnchorPoint(ccp(0, 0.5));
//    coins->setPosition(ccpAdd(coinIcon->getPosition(), ccp(coinIcon->getContentSize().width/2 + 8 , 0)));
//    this->addChild(coins);
//
//    
//    SGCCLabelTTF *needCoins = SGCCLabelTTF::create(str_cost_tong_money, FONT_BOXINFO, 24);
//    this->addChild(needCoins);
//    needCoins->setPosition(ccpAdd(titlebg->getPosition(), ccp(-242,0)));
//    
//    CCSprite *coinIcon1 = CCSprite::createWithSpriteFrameName("coinicon.png");
//    this->addChild(coinIcon1);
//    coinIcon1->setPosition(ccpAdd(needCoins->getPosition(), ccp(needCoins->getContentSize().width*.5 +2 +coinIcon1->getContentSize().width/2, 0)));
//    
//    SGCCLabelTTF *costCoins = SGCCLabelTTF::create(CCString::createWithFormat("%d",101)->getCString(), FONT_BOXINFO, 24);
//    costCoins->setAnchorPoint(ccp(0, 0.5));
//    this->addChild(costCoins);
//    costCoins->setPosition(ccpAdd(coinIcon1->getPosition(), ccp(coinIcon1->getContentSize().width/2 + 8, 0)));
    
    
    
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    
    CCRect re = CCRectMake(0, 30, bg->getContentSize().width, s.height  - title_bg->getContentSize().height - 112);
    bg->setTextureRect(re);
    if (CCDirector::sharedDirector()->getWinSize().height == 1136)
    {
        bg->setScaleY(1.08);
    }
    else
    {
        bg->setScaleY(1.09);
    }
    bg->setScaleX(s.width/bg->getContentSize().width * 1.02);
    
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-5, 112));
    
    char spriteName[256] = {0};
    
    
    //下一级卡牌的属性
    SGEquipmentDataModel *equipModel = SGStaticDataManager::shareStatic()->getEquipById(willAdvancedEquipCard->getItemId());
    SGEquipmentDataModel *nextEquip = SGStaticDataManager::shareStatic()->getEquipById(equipModel->getEquipNextId());
    
    //对应下一个阶数的卡牌属性
    
    afterAdvacedEquipCard = new SGEquipCard();
    afterAdvacedEquipCard->retain();
    afterAdvacedEquipCard->autorelease();
    //原型ID
    afterAdvacedEquipCard->setProtoId(nextEquip->getEquipProtoId());
    afterAdvacedEquipCard->setCurrLevel(willAdvancedEquipCard->getCurrLevel());//转生加
    afterAdvacedEquipCard->setCurrStar(nextEquip->getEquipCurrStarLevel());
    //equipCard->setCurrExp(_card->getCurrExp());//转生
    afterAdvacedEquipCard->setOfficerCardId(0);
    //转生最大星级
    afterAdvacedEquipCard->setUpgradestar(nextEquip->getUpgradestar());
    //转生等级
    afterAdvacedEquipCard->setUpgradelevel(nextEquip->getUpgradelevel());
    //转生次数
    afterAdvacedEquipCard->setAdNum(nextEquip->getAdvNum());
    afterAdvacedEquipCard->setItemType(nextEquip->getEquipType());
    afterAdvacedEquipCard->setIsProt(willAdvancedEquipCard->getIsProt());//转生
    afterAdvacedEquipCard->setOfficerName(CCString::create(nextEquip->getEquipName()->getCString()));
    afterAdvacedEquipCard->setItemId(equipModel->getEquipNextId());
    afterAdvacedEquipCard->setMorale(nextEquip->getEquipHP());
    afterAdvacedEquipCard->setAtk(nextEquip->getEquipmentBaseAtk());
    afterAdvacedEquipCard->setDef(nextEquip->getEquipBaseDef());
    afterAdvacedEquipCard->setRound(nextEquip->getEquipRound());
    afterAdvacedEquipCard->setSpeed(nextEquip->getEquipBaseSpeed());
    afterAdvacedEquipCard->setMaxLevel(nextEquip->getEquipMaxLevel());
    afterAdvacedEquipCard->setGoverning(nextEquip->getEquipGoverning());
    afterAdvacedEquipCard->setHeadSuffixNum(nextEquip->getIconId());//12.06

	//
	SGMainManager::shareMain()->addHeadIconPlistByNum(willAdvancedEquipCard->getHeadSuffixNum(),sg_equipAdvancedLayer);
    sprintf(spriteName, "head%d.png",willAdvancedEquipCard->getHeadSuffixNum());
    
    //边框
    char boder[32] = "\0";
    int starLvl = willAdvancedEquipCard->getCurrStar();
    if (starLvl > 1)
    {
        starLvl -= 1;
    }
    sprintf(boder, "equipPinzhi_%d_0.png", starLvl);
    
    
    //转生前的头像
    SGButton *current = SGButton::create(spriteName,boder, this,
                                         menu_selector(SGEquipAdvancedLayer::pressHeadIcon),
                                         ccp(0, 0),
                                         false,
                                         true);
    this->addBtn(current);
    current->setPosition(ccpAdd(lanbg->getPosition(), ccp(-lanbg->getContentSize().width*.3, 0)));
    
    CCSprite * cardBgCur = CCSprite::createWithSpriteFrameName("card_bj.png");
    cardBgCur->setScale(current->getContentSize().width / cardBgCur->getContentSize().width);
    cardBgCur->setPosition(current->getPosition());
    this->addChild(cardBgCur , lanbg->getTag()+1);
    
    //当前要转生的卡牌的对应的+1 +2 +3
//    if (willAdvancedEquipCard->getAdNum())
//    {
//        SGCCLabelTTF *willAdvancedNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d", willAdvancedEquipCard->getAdNum())->getCString(),
//                                                             FONT_PANGWA, 24, ccGREEN);
//        current->addChild(willAdvancedNum, 10);
//        willAdvancedNum->setPosition(ccpAdd(ccp(0, 0), ccp(current->getContentSize().width - willAdvancedNum->getContentSize().width / 2,
//                                                           current->getContentSize().height - willAdvancedNum->getContentSize().height / 2)));
//    }
    
    
    CCSprite *jinjieq = CCSprite::createWithSpriteFrameName("font_jinjieq.png");
    jinjieq->setAnchorPoint(ccp(0, 1));
    this->addChild(jinjieq);
    jinjieq->setPosition(ccpAdd(lanbg->getPosition(), ccp(-lanbg->getContentSize().width*.48,lanbg->getContentSize().height*.46)));
    
    CCSprite *jinjieh = CCSprite::createWithSpriteFrameName("font_jinjieh.png");
    jinjieh->setAnchorPoint(ccp(0, 1));
    this->addChild(jinjieh);
    jinjieh->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.46,redbg->getContentSize().height*.46)));
    
    
    //前置文本字号
    int nameFontSize = 26;
    int nameOffy = 7;
    int advOffy = 0;
    
    //未转生前的名字
    ccColor3B colorArr[4] = {ccc3(0 , 239 , 22) , ccc3(75, 147, 255) ,ccc3(215,56,255) , ccc3(249, 158, 0)};
    ccColor3B fontColor = ccc3(0,255,0);
    int starLev = willAdvancedEquipCard->getCurrStar();
    if(starLev<3)
        starLev = 3;
    else if(starLev>6)
        starLev=6;
    fontColor = colorArr[starLev-3];
    SGCCLabelTTF * name1 = SGCCLabelTTF::create((CCString::createWithFormat("%s", willAdvancedEquipCard->getOfficerName()->getCString())->getCString()), FONT_PANGWA, nameFontSize , fontColor);
    this->addChild(name1);
    name1->setAnchorPoint(ccp(0,1));
    name1->setPosition(ccpAdd(jinjieq->getPosition(), ccp(jinjieq->getContentSize().width + 21, nameOffy)));
    //转生前 小转生次数
    if(willAdvancedEquipCard->getAdNum() > 0)
    {
        SGCCLabelTTF * advNum = SGCCLabelTTF::create((CCString::createWithFormat("+%d", willAdvancedEquipCard->getAdNum())->getCString()), FONT_PANGWA, nameFontSize , ccGREEN);
        advNum->setAnchorPoint(ccp(0, 1));
        advNum->setPosition(ccpAdd(name1->getPosition(), ccp(name1->getContentSize().width + 6, advOffy)) );
        this->addChild(advNum);
    }
    
    //转生后的名字
    starLev = afterAdvacedEquipCard->getCurrStar();
    if(starLev<3)
        starLev = 3;
    else if(starLev>6)
        starLev=6;
    fontColor = colorArr[starLev-3];
    SGCCLabelTTF *name2 = SGCCLabelTTF::create((CCString::createWithFormat("%s", afterAdvacedEquipCard->getOfficerName()->getCString())->getCString()),FONT_PANGWA, nameFontSize, fontColor);
    this->addChild(name2);
    name2->setAnchorPoint(ccp(0,1));
    name2->setPosition(ccpAdd(jinjieh->getPosition(), ccp(jinjieh->getContentSize().width + 21, nameOffy)));
    
    //转生后 小转生次数
    if(afterAdvacedEquipCard->getAdNum() > 0)
    {
        SGCCLabelTTF * advNum2 = SGCCLabelTTF::create((CCString::createWithFormat("+%d", afterAdvacedEquipCard->getAdNum())->getCString()), FONT_PANGWA, nameFontSize , ccGREEN);
        advNum2->setAnchorPoint(ccp(0, 1));
        advNum2->setPosition(ccpAdd(name2->getPosition(), ccp(name2->getContentSize().width + 6, advOffy)) );
        this->addChild(advNum2);
    }
    
    char spriteN[256] = {0};
	
    SGMainManager::shareMain()->addHeadIconPlistByNum(afterAdvacedEquipCard->getHeadSuffixNum(),sg_equipAdvancedLayer);//12.05
    sprintf(spriteN, "head%d.png",afterAdvacedEquipCard->getHeadSuffixNum());
    
    starLvl = afterAdvacedEquipCard->getCurrStar();
    if (starLvl > 1)
    {
        starLvl -= 1;
    }
    sprintf(boder, "equipPinzhi_%d_0.png", starLvl);
    //转生后的头像
    SGButton *advance = SGButton::create(spriteN,boder, this,
                                         menu_selector(SGEquipAdvancedLayer::pressHeadIcon),
                                         ccp(0, 0),
                                         false,
                                         true);

    advance->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.288, 5)));
    this->addBtn(advance);
    advance->setScale(1.2);
    
    CCSprite * cardBgAd = CCSprite::createWithSpriteFrameName("card_bj.png");
    cardBgAd->setScale(advance->getContentSize().width / cardBgAd->getContentSize().width + 0.2);
    cardBgAd->setPosition(advance->getPosition());
    this->addChild(cardBgAd , redbg->getTag() + 1);
    
    //显示示+1 +2 +3的标签
//    if (afterAdvacedEquipCard->getAdNum())
//    {
//        SGCCLabelTTF *afterAdvancedNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d", afterAdvacedEquipCard->getAdNum())->getCString(),
//                                                              FONT_PANGWA, 24, ccGREEN);
//        advance->addChild(afterAdvancedNum, 10);
//        afterAdvancedNum->setPosition(ccpAdd(ccp(0, 0), ccp(advance->getContentSize().width - afterAdvancedNum->getContentSize().width / 2,
//                                                            advance->getContentSize().height - afterAdvancedNum->getContentSize().height / 2)));
//    }
     //显示未转生前的卡牌的信息
    this->showWillAdvanceCardInfo(ccpAdd(lanbg->getPosition(), ccp(-63, 70)), willAdvancedEquipCard);
    //显示转生后的信息
    this->showAfterAdvanceCardInfo(ccpAdd(redbg->getPosition(), ccp(-63, 70)), afterAdvacedEquipCard);
    
    SGButton* okbtn = SGButton::createFromLocal("batch_sell.png", str_EquipAdvanced_start, this,
                                                menu_selector(SGEquipAdvancedLayer::startAdvaced),CCPointZero,FONT_PANGWA,ccWHITE,28,false,true);
    
    //装备转生第8步
    int advanceTag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_EUIQP_ADVANCED, 8);
    okbtn->setTag(advanceTag);
    this->addBtn(okbtn);
//    okbtn->setPosition(ccpAdd(center, ccp(0, -398)));
//    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_24);
//    okbtn->setTag(modelTag);
    
    //返回
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this,
                                                  menu_selector(SGEquipAdvancedLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));

    //下面的材料条背景
    CCScale9Sprite *frame1 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame1->setPreferredSize(CCSizeMake(600, 150));
    frame1->setPosition(ccpAdd(center, ccp(0,-260)));
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    
    //装备转生第六步，高亮区域
    int tag = SGStaticDataManager::shareStatic()->getimitTagById(LIT_GUID_EUIQP_ADVANCED, 6);
    frame2bg->setTag(tag);
    
    
    this->addChild(frame2bg);
    this->addChild(frame1);
    frame2bg->setScaleX(frame1->getContentSize().width/frame2bg->getContentSize().width);
    frame2bg->setScaleY(frame1->getContentSize().height/frame2bg->getContentSize().height);
    frame2bg->setPosition(frame1->getPosition());
    
    
    //每个材料需要的数量
    int materialNeedNum[12] = {0};
    CCArray *materialModelArray = this->getMaterialDependency(materialNeedNum);
    //材料放置layer
    CCLayerColor *color = this->getMaterialLayer(materialModelArray, materialNeedNum);
    CCLog("anchorPoint x=%f,y=%f" , color->getAnchorPoint().x , color->getAnchorPoint().y);
//    color->setPosition(ccpAdd(frame2bg->getPosition(), ccp(-color->getContentSize().width / 2, -color->getContentSize().height / 2)));
    //color->setPosition(ccpAdd(frame2bg->getPosition(), ccp(-frame1->getContentSize().width*0.5 + color->getContentSize().width / 2, -color->getContentSize().height / 2)));
    
    color->setPosition(ccpAdd(frame2bg->getPosition(), ccp(-frame1->getContentSize().width / 2 + 20, -color->getContentSize().height / 2)));

    
    okbtn->setPosition(ccp(frame1->getPositionX() + frame1->getContentSize().width*0.5 - okbtn->getContentSize().width *0.6, color->getContentSize().height*0.5 +  color->getPositionY()));
    
    //白色动画箭头
    CCMoveTo *move = CCMoveTo::create(1.0,(ccpAdd(lanbg->getPosition(), ccp(0,-lanbg->getContentSize().height*.5))));
    CCMoveTo *move2=CCMoveTo::create(0.01,(ccpAdd(lanbg->getPosition(), ccp(0,-lanbg->getContentSize().height*.35))));
    CCRepeatForever *r = CCRepeatForever::create(CCSequence::create(move,move2,NULL));
    jiantou->runAction(r);

}

void SGEquipAdvancedLayer::pressHeadIcon(CCObject *obj)
{
    
}

CCArray *SGEquipAdvancedLayer::getMaterialDependency(int *needNum)
{
    //读取将要转生这个装备卡牌
    SGEquipmentDataModel *model = SGStaticDataManager::shareStatic()->getEquipById(willAdvancedEquipCard->getItemId());
    
    //装备的需要材料组id
    int materialDependId = model->getMaterialDependencyId();
    
    //根据装备datamodel里的MaterialDependency获取依赖材料的数据
    SGMaterialDependencyDataModel *depend = SGStaticDataManager::shareStatic()->getMaterialDependencyById(materialDependId);
    CCString *condition = depend->getMaterialDependCondition();
    //每个条件 materialId:materialNum
    CCArray *conditionGroup = split(condition->getCString(), ";");
    
    
    //需要的材料原型的的数组
    CCArray *materialModelArray = CCArray::create();
    
    //切分每个分组条件，将内部的条件再次进行切分
    for (int i = 0; i < conditionGroup->count(); i++)
    {
        CCString *temp = static_cast<CCString *>(conditionGroup->objectAtIndex(i));
        //冒号切分
        CCArray *tempConditon = split(temp->getCString(), ",");
        
        //需要的材料id
        int materialId = static_cast<CCString *>(tempConditon->objectAtIndex(0))->intValue();
        //需要的材料的数量
        int materialNum = static_cast<CCString *>(tempConditon->objectAtIndex(1))->intValue();
        //将每次材料需要的数量放到数组中，传给getMaterialLayer
        needNum[i] = materialNum;
        //根据条件组中的id，获取材料的原型
        SGMaterialDataModel *material = SGStaticDataManager::shareStatic()->getMaterialById(materialId);
        if (material)
        {
            materialModelArray->addObject(material);
        }
    }
    return materialModelArray;
}
CCLayerColor *SGEquipAdvancedLayer::getMaterialLayer(CCArray *materialModelArray, int *needNumArray)
{
    CCLayerColor *color = CCLayerColor::create(ccc4(0, 255, 0, 0), 600, 150);
    color->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(color, 20, 33);
    //框中放的几个材料
    
    CCSize colorContentSize = CCSizeZero;
    SGButton *materialIcon = NULL;
    SGMaterialDataModel *tempMaterial = NULL;
    SGCCLabelTTF *materialName = NULL;
    
    SGCCLabelTTF *selfMaterialNumLabel = NULL;
    SGCCLabelTTF *needMaterialNumLabel = NULL;
    CCSprite *cardBg = NULL;
    CCSprite *nameBg = NULL;
    
    char iconName[32] = "\0";
    char border[32] = "\0";
    CCMenu * menu = CCMenu::create();
    menu->setContentSize(color->getContentSize());
    menu->ignoreAnchorPointForPosition(false);
    menu->setAnchorPoint(ccp(0,0));
    menu->setPosition(0, 0);
    color->addChild(menu , 1, 66);
    
    
    if (materialModelArray)
    {
        for (int i = 0; i < materialModelArray->count(); i++)
        {
            tempMaterial = static_cast<SGMaterialDataModel *>(materialModelArray->objectAtIndex(i));
            
            SGMainManager::shareMain()->addHeadIconPlistByNum(tempMaterial->getMaterialIconId(), sg_equipAdvancedLayer);
            sprintf(iconName, "head%d.png", tempMaterial->getMaterialIconId());
            
            int borderColor = tempMaterial->getMaterialBorderColor();
            if (borderColor > 1)
            {
                borderColor -= 1;
            }
            
            sprintf(border, "equipPinzhi_%d_0.png", borderColor);
            
            //头像，边框，名称
            //materialBorder = CCSprite::createWithSpriteFrameName(border);
            materialIcon = SGButton::create(iconName, border, this, menu_selector(SGEquipAdvancedLayer::sourceCallback) , CCSizeZero , false , true);
            
            materialName = SGCCLabelTTF::create(tempMaterial->getMaterialName()->getCString(), FONT_PANGWA, 18);
            materialIcon->setScale(ICON_SCALE);
            //materialBorder->setScale(ICON_SCALE);
            
            
            menu->addChild(materialIcon , 2 , i);
            
            //自己拥有的材料数
            int selfMaterialNum = 0;
            
            SGMaterialCard *tmpMaterial = SGPlayerInfo::sharePlayerInfo()->getMaterialCardByItemId(tempMaterial->getMaterialId());
            if (tmpMaterial)
            {
                selfMaterialNum = tmpMaterial->getMaterialCurrNum();
            }
            
            selfMaterialNumLabel = SGCCLabelTTF::create(CCString::createWithFormat("%d", selfMaterialNum)->getCString(), FONT_PANGWA, 18);
            selfMaterialNumLabel->setAnchorPoint(ccp(0, 0.5));
            needMaterialNumLabel = SGCCLabelTTF::create(CCString::createWithFormat("/%d", needNumArray[i])->getCString(), FONT_PANGWA, 18);
            needMaterialNumLabel->setAnchorPoint(ccp(0, 0.5));
            nameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
            nameBg->setScaleX(0.7);
            nameBg->setScaleY(0.85);
            
            //某一种数量不足
            if (selfMaterialNum < needNumArray[i])
            {
                //如果有一种材料不足，就是不可转生的
                canAdvancedEquip = false;
                selfMaterialNumLabel->setInsideColor(ccRED);
            }
            cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
            cardBg->setScale(ICON_SCALE);
            
            //color->addChild(materialBorder, 1);
            color->addChild(selfMaterialNumLabel, 2);
            color->addChild(needMaterialNumLabel, 3);
            color->addChild(materialName, 3);
            color->addChild(cardBg, -1);
            color->addChild(nameBg, -1);
            
            materialIcon->setPosition(ccpAdd(ccp(0, 0), ccp(materialIcon->getContentSize().width / 2 +
                                                            5 + i *  (materialIcon->getContentSize().width + 10) + 40 - materialIcon->getContentSize().width / 2, materialIcon->getContentSize().height / 2 + 10)));
            
            colorContentSize.width += (materialIcon->getContentSize().width + 10);
            colorContentSize.height = materialIcon->getContentSize().height + 10;
            
            //materialBorder->setPosition(ccpAdd(materialIcon->getPosition(), ccp(0, 0)));
            materialName->setPosition(ccpAdd(materialIcon->getPosition(), ccp(0,-materialIcon->getContentSize().height / 2 + 15)));
            
            selfMaterialNumLabel->setPosition(ccpAdd(materialIcon->getPosition(), ccp(-selfMaterialNumLabel->getContentSize().width / 2
                                                                                      -needMaterialNumLabel->getContentSize().width / 2,
                                                                                      -selfMaterialNumLabel->getContentSize().height)));
            
            needMaterialNumLabel->setPosition(ccpAdd(selfMaterialNumLabel->getPosition(), ccp(selfMaterialNumLabel->getContentSize().width, 0)));
            
            materialName->setPosition(ccpAdd(materialIcon->getPosition(), ccp(0, -materialName->getContentSize().height * 2)));
            nameBg->setPosition(ccpAdd(materialName->getPosition(), ccp(0, -2)));
            cardBg->setPosition(ccpAdd(materialIcon->getPosition(), ccp(0, 0)));
        }
    }
    color->setContentSize(colorContentSize);
    return color;
}



//消息监听
void SGEquipAdvancedLayer::advancedListener(CCObject *sender)
{
    SGSocketRequest *receive = static_cast<SGSocketRequest *>(sender);
    if (receive)
    {
        main::EquipmentUpgradeResponse *advanceResp = static_cast<main::EquipmentUpgradeResponse *>(receive->m_msg);
        if (advanceResp)
        {
            //advanceResp->equipmentcarditemproto();
            
            bool isFailed = true;
            CCLOG("state || %d", advanceResp->state());
            if (advanceResp->state() == 1)
            {
                CCLOG("advance success");
                int aelFrom = -1;
                if (1 == _enterType || 2 == _enterType)
                {
                    aelFrom = 4; //请参考SGAdvancedEffectLayer里这个值的作用。
                }
                SGAdvancedEffectLayer *eff = SGAdvancedEffectLayer::create(BIT_EQUIP,
                                                                           SGMainManager::shareMain()->getWillAdvancedItemId(),
                                                                           SGMainManager::shareMain()->getAfterAdvancedItemId() , aelFrom);
                SGMainManager::shareMain()->getMainScene()->addShowLayer(eff);
                isFailed = false;
                
            }
            else if (advanceResp->state() == 0)
            {
                CCLOG("equipadv: equip no exist!");
            }
            else if (advanceResp->state() == -1)
            {
                CCLOG("equipadv: equip no need!");
            }
            else if (advanceResp->state() == -2)
            {
                CCLOG("equipadv: equip no level!");
            }
            else if (advanceResp->state() == -3)
            {
                CCLOG("equipadv: equip no material!");
            }
            else if (advanceResp->state() == -4)
            {
                CCLOG("equipadv: equip no money!");
            }
            
            if (isFailed)
            {
                SG_SHOW_WINDOW(str_EquipAdvanced_failed);
            }
        }
    }
    
    CCLOG("receive response");
}


//点击开始转生
void SGEquipAdvancedLayer::startAdvaced(CCObject *obj)
{
    CCLOG("start to advancing...");
    
    if (canAdvancedEquip)
    {
        SGMainManager::shareMain()->setWillAdvancedItemId(willAdvancedEquipCard->getItemId());
        SGMainManager::shareMain()->setAfterAdvancedItemId(afterAdvacedEquipCard->getItemId());
        SGMainManager::shareMain()->setAfterAdvancedSsid(willAdvancedEquipCard->getSsid());
        main::EquipmentUpgradeRequest *advancedReq = new main::EquipmentUpgradeRequest();
        advancedReq->set_ssid(willAdvancedEquipCard->getSsid());
        SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_ADVANCE, advancedReq);
    }
    else
    {
        SG_SHOW_WINDOW("材料不足！");
    }
}



void SGEquipAdvancedLayer::showWillAdvanceCardInfo(cocos2d::CCPoint pos, SGBaseMilitaryCard *card)
{
    int hgt = 30;
    int wid = 190;
    SGCCLabelTTF *atk = SGCCLabelTTF::create(str_attack, FONT_BOXINFO, 26);
    this->addChild(atk);
    atk->setAnchorPoint(ccp(0, 0.5));
    atk->setPosition(pos);
    SGCCLabelTTF *def = SGCCLabelTTF::create(str_defense, FONT_BOXINFO, 26);
    this->addChild(def);
    def->setAnchorPoint(ccp(0, 0.5));
    def->setPosition(ccpAdd(atk->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *mor = SGCCLabelTTF::create(str_hp, FONT_BOXINFO, 26);
    this->addChild(mor);
    mor->setAnchorPoint(ccp(0, 0.5));
    mor->setPosition(ccpAdd(def->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *round = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 26);
    this->addChild(round);
    round->setAnchorPoint(ccp(0, 0.5));
    round->setPosition(ccpAdd(mor->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *spd = SGCCLabelTTF::create(str_speed, FONT_BOXINFO, 26);
    this->addChild(spd);
    spd->setAnchorPoint(ccp(0, 0.5));
    spd->setPosition(ccpAdd(round->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *gov = SGCCLabelTTF::create(str_tongyuli, FONT_BOXINFO, 26);
    this->addChild(gov);
    gov->setAnchorPoint(ccp(0, 0.5));
    gov->setPosition(ccpAdd(spd->getPosition(), ccp(0, -hgt)));
    
    
    CCDictionary *dicc = SGCardAttribute::getValue(card->getCurrLevel(), card->getItemId());
    
    this->willAdvancedDict = dicc;
    
    SGCCLabelTTF *atkvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("atk"))->intValue())->getCString(),FONT_BOXINFO, 26);
    atkvalue->setAnchorPoint(ccp(1, 0.5));
    atkvalue->setPosition(ccpAdd(pos, ccp(wid, 0)));
    this->addChild(atkvalue);
    SGCCLabelTTF *defvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("def"))->intValue())->getCString(),FONT_BOXINFO, 26);
    defvalue->setAnchorPoint(ccp(1, 0.5));
    defvalue->setPosition(ccpAdd(def->getPosition(), ccp(wid, 0)));
    this->addChild(defvalue);
    SGCCLabelTTF *morvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("mor"))->intValue())->getCString(),FONT_BOXINFO, 26);
    morvalue->setAnchorPoint(ccp(1, 0.5));
    morvalue->setPosition(ccpAdd(mor->getPosition(), ccp(wid, 0)));
    this->addChild(morvalue);
    SGCCLabelTTF *roundvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",card->getRound())->getCString(),FONT_BOXINFO, 26);
    roundvalue->setAnchorPoint(ccp(1, 0.5));
    roundvalue->setPosition(ccpAdd(round->getPosition(), ccp(wid, 0)));
    this->addChild(roundvalue);
    SGCCLabelTTF *spdvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",(int)((CCString *)dicc->objectForKey("speed"))->floatValue())->getCString(),FONT_BOXINFO, 26);
    spdvalue->setAnchorPoint(ccp(1, 0.5));
    spdvalue->setPosition(ccpAdd(spd->getPosition(), ccp(wid, 0)));
    this->addChild(spdvalue);
    SGCCLabelTTF *govvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",card->getGoverning())->getCString(),FONT_BOXINFO, 26);
    govvalue->setAnchorPoint(ccp(1, 0.5));
    govvalue->setPosition(ccpAdd(gov->getPosition(), ccp(wid, 0)));
    this->addChild(govvalue);
    
}

void SGEquipAdvancedLayer::showAfterAdvanceCardInfo(CCPoint pos,SGBaseMilitaryCard *card)
{
    int hgt = 30;
    int wid = 190;
    SGCCLabelTTF *atk = SGCCLabelTTF::create(str_attack, FONT_BOXINFO, 26);
    this->addChild(atk);
    atk->setAnchorPoint(ccp(0, 0.5));
    atk->setPosition(pos);
    SGCCLabelTTF *def = SGCCLabelTTF::create(str_defense, FONT_BOXINFO, 26);
    this->addChild(def);
    def->setAnchorPoint(ccp(0, 0.5));
    def->setPosition(ccpAdd(atk->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *mor = SGCCLabelTTF::create(str_hp, FONT_BOXINFO, 26);
    this->addChild(mor);
    mor->setAnchorPoint(ccp(0, 0.5));
    mor->setPosition(ccpAdd(def->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *round = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 26);
    this->addChild(round);
    round->setAnchorPoint(ccp(0, 0.5));
    round->setPosition(ccpAdd(mor->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *spd = SGCCLabelTTF::create(str_speed, FONT_BOXINFO, 26);
    this->addChild(spd);
    spd->setAnchorPoint(ccp(0, 0.5));
    spd->setPosition(ccpAdd(round->getPosition(), ccp(0, -hgt)));
    SGCCLabelTTF *gov = SGCCLabelTTF::create(str_tongyuli, FONT_BOXINFO, 26);
    this->addChild(gov);
    gov->setAnchorPoint(ccp(0, 0.5));
    gov->setPosition(ccpAdd(spd->getPosition(), ccp(0, -hgt)));
    
    
    CCDictionary *dicc = SGCardAttribute::getValue(afterAdvacedEquipCard->getCurrLevel(), afterAdvacedEquipCard->getItemId());
    
    SGCCLabelTTF *atkvalue = SGCCLabelTTF::create(CCString::createWithFormat("(%d)",((CCString *)dicc->objectForKey("atk"))->intValue())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    atkvalue->setAnchorPoint(ccp(1, 0.5));
    atkvalue->setPosition(ccpAdd(pos, ccp(wid*0.7, 0)));
    //modify by:zyc. merge into create.
    //atkvalue->setColor(ccGREEN);
    this->addChild(atkvalue,1,10);
    ///////////攻击后的黄字
    SGCCLabelTTF *atkvaluemax = SGCCLabelTTF::create(CCString::createWithFormat("(+%d)",((CCString *)dicc->objectForKey("atk"))->intValue() - ((CCString *)willAdvancedDict->objectForKey("atk"))->intValue())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    atkvaluemax->setAnchorPoint(ccp(1, 0.5));
    atkvaluemax->setPosition(ccpAdd(pos, ccp(wid*1.5, 0)));

    this->addChild(atkvaluemax,1,100);
    SGCCLabelTTF *defvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("def"))->intValue())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    defvalue->setAnchorPoint(ccp(1, 0.5));
    defvalue->setPosition(ccpAdd(def->getPosition(), ccp(wid*0.7, 0)));

    this->addChild(defvalue,1,11);
    ////////////防御后的黄字
    SGCCLabelTTF *defvaluemax = SGCCLabelTTF::create(CCString::createWithFormat(("(+%d)"),((CCString *)dicc->objectForKey("def"))->intValue()- ((CCString *)willAdvancedDict->objectForKey("def"))->intValue())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    defvaluemax->setAnchorPoint(ccp(1, 0.5));
    defvaluemax->setPosition(ccpAdd(def->getPosition(), ccp(wid*1.5, 0)));

    this->addChild(defvaluemax,1,110);
    ///////////
    SGCCLabelTTF *morvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",((CCString *)dicc->objectForKey("mor"))->intValue())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    morvalue->setAnchorPoint(ccp(1, 0.5));
    morvalue->setPosition(ccpAdd(mor->getPosition(), ccp(wid*0.7, 0)));

    this->addChild(morvalue,1,12);
    ////////////血量后的黄字
    SGCCLabelTTF *morvaluemax = SGCCLabelTTF::create(CCString::createWithFormat("(+%d)",((CCString *)dicc->objectForKey("mor"))->intValue()- ((CCString *)willAdvancedDict->objectForKey("mor"))->intValue())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    morvaluemax->setAnchorPoint(ccp(1, 0.5));
    morvaluemax->setPosition(ccpAdd(mor->getPosition(), ccp(wid*1.5, 0)));

    this->addChild(morvaluemax,1,120);
    
    ///////////
    SGCCLabelTTF *roundvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",card->getRound())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    roundvalue->setAnchorPoint(ccp(1, 0.5));
    roundvalue->setPosition(ccpAdd(round->getPosition(), ccp(wid*0.7, 0)));

    this->addChild(roundvalue,1,13);
    //////////蓄力后的黄字
    SGCCLabelTTF *roundvaluemax = SGCCLabelTTF::create(CCString::createWithFormat("(+%d)",card->getRound() - willAdvancedEquipCard->getRound())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    roundvaluemax->setAnchorPoint(ccp(1, 0.5));
    roundvaluemax->setPosition(ccpAdd(round->getPosition(), ccp(wid*1.5, 0)));
    this->addChild(roundvaluemax,1,130);
    
    /////////
    SGCCLabelTTF *spdvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",(int)((CCString *)dicc->objectForKey("speed"))->floatValue())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    spdvalue->setAnchorPoint(ccp(1, 0.5));
    spdvalue->setPosition(ccpAdd(spd->getPosition(), ccp(wid*0.7, 0)));
    this->addChild(spdvalue,1,14);
    ///////////速度后的黄字
    SGCCLabelTTF *spdvaluemax = SGCCLabelTTF::create(CCString::createWithFormat("(+%d)",(int)((CCString *)dicc->objectForKey("speed"))->floatValue()- ((CCString *)willAdvancedDict->objectForKey("speed"))->intValue())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    spdvaluemax->setAnchorPoint(ccp(1, 0.5));
    spdvaluemax->setPosition(ccpAdd(spd->getPosition(), ccp(wid*1.5, 0)));

    this->addChild(spdvaluemax,1,140);
    
    //////////
    SGCCLabelTTF *govvalue = SGCCLabelTTF::create(CCString::createWithFormat("%d",card->getGoverning() )->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    govvalue->setAnchorPoint(ccp(1, 0.5));
    govvalue->setPosition(ccpAdd(gov->getPosition(), ccp(wid*0.7, 0)));
    this->addChild(govvalue,1,15);
    ////////统御力后的黄字
    SGCCLabelTTF *govvaluemax = SGCCLabelTTF::create(CCString::createWithFormat("(+%d)",card->getGoverning() - willAdvancedEquipCard->getGoverning())->getCString(),FONT_BOXINFO, 26 , ccGREEN);
    govvaluemax->setAnchorPoint(ccp(1, 0.5));
    govvaluemax->setPosition(ccpAdd(gov->getPosition(), ccp(wid*1.5, 0)));
    this->addChild(govvaluemax,1,150);
    
    
}

void SGEquipAdvancedLayer::backHandler(CCObject *obj)
{
    if (0 == _enterType)
    {
        SGMainManager::shareMain()->showEquipInfoLayer(willAdvancedEquipCard, 0);
    }
    else if (1 == _enterType || 2 == _enterType) //1和2可以等同处理，因为最终都是要返回“并没有删除“的SGGeneralInfoLayer！
    {
        //@非常重要：其实运行到这里，SGGeneralInfoLayer还存在，所以这里即使oid是0导致的SGOfficerCard为NULL，也没有关系，这是不合理的业务出现想要的结果，出现问题再议。
        int oid = willAdvancedEquipCard->getOfficerCardId();
        SGOfficerCard* ocard =  SGPlayerInfo::sharePlayerInfo()->getOfficerCard(oid);
        SGMainManager::shareMain()->showGeneralInfoLayer(ocard, 3,NULL,true,NULL,12);
    }
}


void SGEquipAdvancedLayer::sourceCallback(cocos2d::CCObject *obj)
{
    int needNum[12] = {0};
    CCArray *materialArray;
    //获取卡牌的转生条件
    materialArray = this->getMaterialDependency(needNum);
    
    int tag = ( (SGButton *)obj)->getTag();
    int mataerialId = ( (SGMaterialDataModel *)materialArray->objectAtIndex(tag) )->getMaterialId();
    
    
    SGLootWiKiMainLayer * lootWiKiMainLayer = SGLootWiKiMainLayer::create(mataerialId , 1);
    SGMainManager::shareMain()->showBox(lootWiKiMainLayer);
}






