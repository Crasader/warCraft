//
//  SGLandingReward.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-8.
//
//

#include "SGLandingReward.h"
#include "SGBaseBox.h"
#include "cocos-ext.h"
#include "SGPlayerInfo.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "LotteryModule.pb.h"
#include "SGStaticDataManager.h"
#include "SGOfficerCard.h"
#include "SGEquipCard.h"
#include "SGPropsCard.h"
#include "SGLottery9ViewItem.h"
#include "SGAnnouncementLayer.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#define BGCOLOR ccc3(100,100,100)
SGLandingReward::SGLandingReward()
:deletage(NULL),
currChance(0),
state(0),
tempBg(NULL),
tempBtn(NULL),
AllBtn(NULL)
,btntag(-1),
uncard(NULL),
enterGame(NULL),
tempTag(0),
logindays(0),
chances(0),
label1(NULL),
tagArray(NULL)
{
    for (int i = 0; i<9; i++)
    {
        chanceTag[i] = -1;
    }
}
SGLandingReward::~SGLandingReward()
{
    CCLOG("~SGLandingReward");
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_VISIT_9LOTTERY);
    CC_SAFE_RELEASE(tagArray);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_landingRewardLayer);
}

SGLandingReward *SGLandingReward::create(SGBoxDelegate *dg)
{
    SGLandingReward *rewardLayer = new SGLandingReward();
    if (rewardLayer && rewardLayer->init(NULL, sg_landingRewardLayer))
    {
        rewardLayer->initView(dg);
        rewardLayer->autorelease();
        return rewardLayer;
    }
    CC_SAFE_DELETE(rewardLayer);
    return NULL;
}
void SGLandingReward::initView(SGBoxDelegate *dg)
{
    ResourceManager::sharedInstance()->bindTexture("sglandingreward/sglandingreward.plist", RES_TYPE_LAYER_UI, sg_landingRewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_landingRewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_landingRewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist",RES_TYPE_LAYER_UI ,sg_landingRewardLayer);
    
    //为了后面能用铜钱的icon，这里提前把铜钱icon对应的plist加载了上来。
    SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_landingRewardLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_VISIT_9LOTTERY,
                              this,
                              callfuncO_selector(SGLandingReward::lotteryListener));   
    deletage = dg;
    CCLayerColor *bgLayer  =CCLayerColor::create(ccc4(0, 0, 0, 220));
    this->addChild(bgLayer,-3);
    
//    CCLabelTTF *test = CCLabelTTF::create("123", "", FONTSIZE(13));
//    this->addChild(test);
//    test->setPosition(SGLayout::getPoint(kMiddleCenter));
    this->setTouchEnabled(true);
    
    CCSprite *bg = CCSprite::createWithSpriteFrameName("reward_bg.png");
    this->addChild(bg);
    bg->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 35)));
    
    CCSprite *redbg = CCSprite::createWithSpriteFrameName("reward_tiao.png");
    this->addChild(redbg);
    redbg->setPosition(ccpAdd(bg->getPosition(), ccp(0, bg->getContentSize().height/2 +2 + redbg->getContentSize().height/2)));
    
    CCSprite *title = CCSprite::createWithSpriteFrameName("font_landreward.png");
    this->addChild(title);
    title->setPosition(redbg->getPosition());
   
    
    enterGame = SGButton::create("public_kuang_hui.png", "font_enterGame.png", this, menu_selector(SGLandingReward::boxCloseWithOutCallBack),CCPointZero,false,true);
    this->addBtn(enterGame);
    enterGame->setPosition(ccpAdd(bg->getPosition(), ccp(0, -bg->getContentSize().height/2 - enterGame->getContentSize().height*.7)));
    
    logindays = SGPlayerInfo::sharePlayerInfo()->getLoginDays();
    chances =SGPlayerInfo::sharePlayerInfo()->getLoginChance();
    //chances=1;
    
    label1 = SGCCLabelTTF::create(CCString::createWithFormat(str_LandingReward_str1,logindays,currChance,chances)->getCString(), FONT_BOXINFO, 26);
    this->addChild(label1);
    label1->setPosition(ccpAdd(bg->getPosition(), ccp(0, bg->getContentSize().height*.445)));
    
    tagArray = CCArray::create();
    tagArray->retain();
    for (int i = 0; i<9; i++)
    {
        uncard = SGButton::create("pai_B.png", NULL, this, menu_selector(SGLandingReward::confirmHandler),CCPointZero,false,true);
        this->addBtn(uncard);
        uncard->setPosition(ccpAdd(bg->getPosition(), ccp(-bg->getContentSize().width*.31 + bg->getContentSize().width*.31 *(i%3), 15 + bg->getContentSize().height*.25 - bg->getContentSize().height*.25*(i/3))));
        uncard->setTag(i);
        
        tagArray->addObject(uncard);   

    }    
    
    
    //SGCCLabelTTF *label10 = SGCCLabelTTF::create("10", FONT_BOXINFO, 26);
    //SGCCLabelTTF *label15 = SGCCLabelTTF::create("15", FONT_BOXINFO, 26);
    //SGCCLabelTTF *label20 = SGCCLabelTTF::create("20", FONT_BOXINFO, 26);
    //SGCCLabelTTF *label25 = SGCCLabelTTF::create("25", FONT_BOXINFO, 26);
    //SGCCLabelTTF *label_1 = SGCCLabelTTF::create("、", FONT_BOXINFO,26);
    //SGCCLabelTTF *label_2 = SGCCLabelTTF::create("、", FONT_BOXINFO, 26);
    //SGCCLabelTTF *label_3 = SGCCLabelTTF::create("、", FONT_BOXINFO, 26);
    //SGCCLabelTTF *label3 = SGCCLabelTTF::create(str_LandingReward_str2,FONT_BOXINFO, 26);
    
    //2014-6-20 修改，（修改要求：去掉连续登陆5、10.。额外奖励的功能和提示，by：zyc）该处为文字 “可额外获得”
    //修改后只有文字  “可以翻牌”
    SGCCLabelTTF *label4 = SGCCLabelTTF::create(str_LandingReward_str3, FONT_BOXINFO, 26);
    SGCCLabelTTF * label16 = SGCCLabelTTF::create(str_LandingReward_str8,FONT_BOXINFO,26);
    
    //this->addChild(label10);
    //this->addChild(label15);
    //this->addChild(label20);
    //this->addChild(label25);
    //this->addChild(label_1);
    //this->addChild(label_2);
    //this->addChild(label_3);
    //this->addChild(label3);
    this->addChild(label4);
    this->addChild(label16);
    label4->setAnchorPoint(ccp(0.5, 0.5));
    label16->setAnchorPoint(ccp(0.5,0.5));
    
    
    //CCSprite *sprite_yb = CCSprite::createWithSpriteFrameName("store_yb.png");
    //this->addChild(sprite_yb);
    
    
    //2014-6-20 修改，（修改要求：去掉连续登陆5、10.。额外奖励的功能和提示，by：zyc），该处为数字
    //SGCCLabelTTF *font_yb = SGCCLabelTTF::create("X20",FONT_BOXINFO, 26);
    //this->addChild(font_yb);
    
    
    
    
    
    SGCCLabelTTF *label2 = SGCCLabelTTF::create(str_LandingReward_str4, FONT_BOXINFO, 26);
    this->addChild(label2);
    label2->setAnchorPoint(ccp(1, 0.5));
    
    //label15->setPosition(ccpAdd(bg->getPosition(), ccp(0, -bg->getContentSize().height*0.41)));
    
    //label_1->setPosition(ccpAdd(label15->getPosition(), ccp(-label15->getContentSize().width/2, 0)));
    //label10->setPosition(ccpAdd(label_1->getPosition(), ccp(-label_1->getContentSize().width/2 - label10->getContentSize().width/2, 0)));
    //label2->setPosition(ccpAdd(label10->getPosition(), ccp(-label10->getContentSize().width/2 - label_1->getContentSize().width*0.4, 0)));
    //label_2->setPosition(ccpAdd(label15->getPosition(), ccp(label15->getContentSize().width/2 + label_2->getContentSize().width/2, 0)));
    //label20->setPosition(ccpAdd(label_2->getPosition(), ccp(label20->getContentSize().width/2, 0)));
    //label_3->setPosition(ccpAdd(label20->getPosition(), ccp(label_3->getContentSize().width/2+label20->getContentSize().width/2, 0)));
    //label25->setPosition(ccpAdd(label_3->getPosition(), ccp(label25->getContentSize().width/2, 0)));
    //label3->setPosition(ccpAdd(label25->getPosition(), ccp(label25->getContentSize().width/2 + label3->getContentSize().width/2, 0)));
    
    
    
    //label4->setPosition(ccpAdd(label10->getPosition(), ccp(26 *2, -label4->getContentSize().height*1.2)));
    label4->setPosition(ccpAdd(bg->getPosition(), ccp(0, -bg->getContentSize().height*0.41)));
    label16->setPosition(ccpAdd(bg->getPosition(), ccp(0,-bg->getContentSize().height*0.44)));
    
//    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
//    for (int i = 0; i<9; i++)
//    {
//        //main::Lottery9ViewItem item = msg.lotteryitems(i);
//        SGLottery9ViewItem *lottery9ViewItem = new SGLottery9ViewItem();
//        lottery9ViewItem->setItemIDItem(4059);
//        lottery9ViewItem->setItemTypeItem(7);
//        lottery9ViewItem->setItemNum(6);
//        playerInfo->add9Item(lottery9ViewItem);
//        lottery9ViewItem->autorelease();
//        
//    }
    
    //删除  元宝 图片
    //sprite_yb->setPosition(ccpAdd(label4->getPosition(), ccp(sprite_yb->getContentSize().width/2, 0)));
    //删除奖励书记  “x20”
    //font_yb->setPosition(ccpAdd(sprite_yb->getPosition(), ccp(sprite_yb->getContentSize().width/2 + font_yb->getContentSize().width/2, 0)));
    
    
/*
    if (logindays>9 && logindays<15) {
        label10->setColor(ccGREEN);
    }else if(logindays>14 && logindays<20)
    {
        label15->setColor(ccGREEN);
    }else if(logindays>19 && logindays<25)
    {
        label20->setColor(ccGREEN);
    }else if(logindays>24)
    {
        label25->setColor(ccGREEN);
    }
 */


    btntag = -1;

}
void SGLandingReward::lotteryListener(cocos2d::CCObject *obj)
{
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        if (btntag < 0)
        {
            return;
        }
        main::Lottery9PurchResponse *response = (main::Lottery9PurchResponse *)sr->m_msg;
        currChance++;
        label1->setString(CCString::createWithFormat(str_LandingReward_str1,logindays,currChance,chances)->getCString());
        SGButton *btn = (SGButton *)menu->getChildByTag(btntag);
        tagArray->removeObject(btn);

        btn->setEnabled(false);
        tempBg = CCSprite::createWithSpriteFrameName("pai_Z.png");
        tempBg->setPosition(btn->getPosition());
        tempBtn = btn;
        
        if (currChance >= SGPlayerInfo::sharePlayerInfo()->getLoginChance())
        {
            for (int i = 0; i < tagArray->count(); i++)
            {
                SGButton *btn = (SGButton *)tagArray->objectAtIndex(i);
                btn->setEnabled(false);
            }            
            CCAction *actionFunc1 =  CCSequence::create(CCDelayTime::create(1),CCCallFunc::create(this, callfunc_selector(SGLandingReward::showCard)),NULL);
            this->runAction(actionFunc1);
        }
        else
        {
            btntag = -1;
        }
        
        
        //6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌 1:铜钱 0:金子 3:军功 5:炼丹秘方 4:小喇叭 10:碎片 11军魂 12体力 13军粮 14天梯积分 15消耗品

        //数量
        int itemNum = response->itemnum();
        //类型
        int itemType = response->itemtype();
        
        switch (itemType)
        {
            case 1:
            {
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat(str_LandingReward_str5,itemNum)->getCString(), FONT_BOXINFO, 26);
                CCString *str = CCString::createWithFormat("equipPinzhi_%d_0.png",3);
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(str->getCString());
                tempBg->addChild(kuang,3);
                kuang->setPosition(ccp(tempBg->getContentSize().width/2, label->getContentSize().height*.5 +tempBg->getContentSize().height/2-3.7));
                
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                tempBg->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                SGMainManager::shareMain() -> addHeadIconPlistByNum(9999,sg_landingRewardLayer);
                
                CCSprite *coin = CCSprite::createWithSpriteFrameName("head9999.png");
                tempBg->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                
                tempBg->addChild(label,2);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
//                playerInfo->setPlayerCoins(playerInfo->getPlayerCoins() + response->itemnum());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);
            }
                break;
            case 2:
            {
                
            }
                break;
            case 3:
            {
                
            }
                break;
            case 4:
            {
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat(str_LandingReward_str6,itemNum)->getCString(), FONT_BOXINFO, 20);
                CCSprite *kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                tempBg->addChild(kuang,3);
                kuang->setPosition(ccp(tempBg->getContentSize().width/2, label->getContentSize().height*.5 +tempBg->getContentSize().height/2-3.7));
                
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                tempBg->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                SGMainManager::shareMain() -> addHeadIconPlistByNum(9998,sg_landingRewardLayer);
                
                CCSprite *coin = CCSprite::createWithSpriteFrameName("head9998.png");
                tempBg->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                tempBg->addChild(label,2);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
//                playerInfo->setSpeakerCount(playerInfo->getSpeakerCount() + itemNum);
                coin->setScale(1.1);
                kuang->setScale(1.1);
                kuang->setFlipX(true);
                coin->setFlipX(true);
                //label->setFlipX(true);
            }
                break;
            case 5:
            {
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat(str_LandingReward_str7,itemNum)->getCString(), FONT_BOXINFO, 20);
                CCString *str = CCString::createWithFormat("equipPinzhi_%d_0.png",3);
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(str->getCString());
                tempBg->addChild(kuang,3);
                kuang->setPosition(ccp(tempBg->getContentSize().width/2, label->getContentSize().height*.5 +tempBg->getContentSize().height/2-3.7));
                
                SGMainManager::shareMain() -> addHeadIconPlistByNum(9996,sg_landingRewardLayer);
                
                CCSprite *coin = CCSprite::createWithSpriteFrameName("head9996.png");
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                tempBg->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                tempBg->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                tempBg->addChild(label,2);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
//                playerInfo->setPlayerliandan(playerInfo->getPlayerliandan() + itemNum);
                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);

            }
                break;
            case 6:
            {
                int itemId = response->itemid();
                SGOfficerDataModel *officer = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
                SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
               
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat("%s",officer->getOfficerName()->getCString())->getCString(), FONT_BOXINFO, 20);
                int starlvl = officer->getOfficerCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
                tempBg->addChild(kuang,3);
                kuang->setPosition(ccp(tempBg->getContentSize().width/2, label->getContentSize().height*.5 +tempBg->getContentSize().height/2-3.7));
                
                //if (str_1->isEqual(CCString::create("SGOfficerCard")))
                {
                    kuang->removeChildByTag(30);
                    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setFlipX(true);
                    countryName->setPosition(ccp(kuang->getContentSize().width-kuang->getContentSize().width*0.18,kuang->getContentSize().height - countryName->getContentSize().height));
                    kuang->addChild(countryName,30,30);
                    
                    if (officer->getAdvNum() > 0) {
                        SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",officer->getAdvNum())->getCString(), FONT_PANGWA, 24);
                        advNum->setInsideColor(ccGREEN);
                        advNum->setAnchorPoint(ccp(1,1));
                        advNum->setFlipX(true);
                        advNum->ignoreAnchorPointForPosition(false);
                        advNum->setPosition(ccp(kuang->getContentSize().width*0.4,kuang->getContentSize().height - advNum->getContentSize().height));
                        kuang->addChild(advNum,40);
                    }
                    
                }

                SGMainManager::shareMain() -> addHeadIconPlistByNum(temp->getIconId(),sg_landingRewardLayer);
                CCSprite *coin = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                tempBg->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                tempBg->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                tempBg->addChild(label,2);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
                
                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);
            }
                break;
            case 7:
            {
                int itemId = response->itemid();
                
                SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(itemId);
                
                CCString *str = CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi());
                
                SGMainManager::shareMain()->addHeadIconPlistByNum(temp->getIconId(), sg_landingRewardLayer);
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat("%s",temp->getEquipName()->getCString())->getCString(), FONT_BOXINFO, 20);
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(str->getCString());
                tempBg->addChild(kuang,3);
                kuang->setPosition(ccp(tempBg->getContentSize().width/2, label->getContentSize().height*.5 +tempBg->getContentSize().height/2-3.7));
                
                SGMainManager::shareMain() -> addEquipPng(temp->getIconId(), sg_landingRewardLayer);
                CCSprite *coin = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",temp->getIconId())->getCString());
                
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                tempBg->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                tempBg->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                tempBg->addChild(label,2);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
                
                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);

            }
                break;
            case 8:
            {
                
            }
                break;
            case 9:
            {
                int itemId = response->itemid();

                SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(itemId);
                
                
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat("%s",prop->getPropsName()->getCString())->getCString(), FONT_BOXINFO, 20);
                CCString *str = CCString::createWithFormat("equipPinzhi_%d_0.png",prop->getPropsCurrStarLevel());
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(str->getCString());
                tempBg->addChild(kuang,3);
                kuang->setPosition(ccp(tempBg->getContentSize().width/2, label->getContentSize().height*.5 +tempBg->getContentSize().height/2-3.7));

                SGMainManager::shareMain() -> addHeadIconPlistByNum(prop->getIconId(), sg_landingRewardLayer);
                CCSprite *coin = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",prop->getIconId())->getCString());
                
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                tempBg->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                tempBg->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                tempBg->addChild(label,2);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
                
                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);

            }
                break;
            case 10:
            {
                int itemId = response->itemid();
                CCString *str_kuang = NULL;
                CCSprite *countryName = NULL;
                SGStaticDataManager *staticDataManager = SGStaticDataManager::shareStatic();
                SGPiecesDataModel *pieces = staticDataManager->getPiecesDictById(itemId);
                
                SGCCLabelTTF *label = SGCCLabelTTF::create(pieces->getPiecesName()->getCString(), FONT_BOXINFO, 20);
                
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
                    
                    countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",
                                                                                                  temp->getOfficerRace()))->getCString());
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
                SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_landingRewardLayer);
                
                
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(str_kuang->getCString());
                tempBg->addChild(kuang,3);
                kuang->setPosition(ccp(tempBg->getContentSize().width/2, label->getContentSize().height*.5 +tempBg->getContentSize().height/2-3.7));
                
                CCSprite *piecesCard = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",iconId)->getCString());
                
                
                if (pieces->getCallingCardId() != -1)//不是通用碎片，加上蒙版
                {
                    CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
                    mask->setPosition(ccp(piecesCard->getContentSize().width / 2, piecesCard->getContentSize().height / 2));
                    piecesCard->addChild(mask);
                }
                
                if (countryName)
                {
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setFlipX(true);
                    countryName->setPosition(ccp(kuang->getContentSize().width-kuang->getContentSize().width*0.18,kuang->getContentSize().height -
                                                 countryName->getContentSize().height));
                    kuang->addChild(countryName,30,30);
                }
                
                tempBg->addChild(piecesCard,2);
                piecesCard->setPosition(kuang->getPosition());
                piecesCard->setScale(1.1);
                kuang->setScale(1.1);
                tempBg->addChild(label,2);
                
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
                
                kuang->setFlipX(true);
                piecesCard->setFlipX(true);
                label->setFlipX(true);
            }
                break;
            case 11:
            {
                
            }
                break;
            case 12:
            {
                
            }
                break;
            case 13:
            {
                
            }
                break;
            case 14:
            {
                
            }
                break;
            case 15:
            {
                int itemId = response->itemid();
                
                SGConsumableDataModel *consume = SGStaticDataManager::shareStatic()->getConsumableById(itemId);
                
                
                
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat("%s",consume->getConsumeName()->getCString())->getCString(), FONT_BOXINFO, 20);
                CCString *str = CCString::createWithFormat("equipPinzhi_%d_0.png",consume->getConsumeStarLvl());
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(str->getCString());
                tempBg->addChild(kuang,3);
                kuang->setPosition(ccp(tempBg->getContentSize().width/2, label->getContentSize().height*.5 +tempBg->getContentSize().height/2-3.7));
                
                SGMainManager::shareMain() -> addHeadIconPlistByNum(consume->getIconId(), sg_landingRewardLayer);
                CCSprite *coin = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",consume->getIconId())->getCString());
                
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                tempBg->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                tempBg->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                tempBg->addChild(label,2);
               label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
                
                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);
            }
                break;
            case 17://材料
            {
                int itemId = response->itemid();
                SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(itemId);
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat("%s", materialModel->getMaterialName()->getCString())->getCString(), FONT_BOXINFO, 20);
                int starLvl = materialModel->getMaterialBorderColor();
                if (starLvl > 1)
                {
                    starLvl -= 1;
                }
                CCString *str = CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl);
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(str->getCString());
                tempBg->addChild(kuang, 3);
                kuang->setPosition(ccpAdd(ccp(tempBg->getContentSize().width / 2, label->getContentSize().height / 2
                                          + tempBg->getContentSize().height / 2 - 3.7), ccp(0, 0)));
                
                SGMainManager::shareMain() -> addHeadIconPlistByNum(materialModel->getMaterialIconId(), sg_landingRewardLayer);
                CCSprite *coin = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",
                                                                                                materialModel->getMaterialIconId())->getCString());
                
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                tempBg->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                tempBg->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                tempBg->addChild(label,2);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
                
                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);
                
            }
                break;
                
            default:
                break;
        }
        
    }
    
    this->addChild(tempBg);
    tempBg->setVisible(false);

    CCAction *actionFunc1 =  CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(SGLandingReward::testFlipCardCallFunc3)),
                                                CCDelayTime::create(0.6),CCCallFuncN::create(this, callfuncN_selector(SGLandingReward::testFlipCardCallFunc4)),
                                                CCDelayTime::create(0.5),NULL);
    CCOrbitCamera *actionFlip1 = CCOrbitCamera::create(1, 1, 0, 0, -180, 0, 0);
    
    tempBtn->runAction(CCSpawn::create((CCFiniteTimeAction *)actionFunc1,(CCFiniteTimeAction *)actionFlip1,NULL));
    
    CCAction *actionFunc2 =  CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(SGLandingReward::testFlipCardCallFunc4)),
                                                CCDelayTime::create(0.6),CCCallFuncN::create(this, callfuncN_selector(SGLandingReward::testFlipCardCallFunc3)),
                                                CCDelayTime::create(0.5),NULL);
    CCOrbitCamera *actionFlip2 = CCOrbitCamera::create(1, 1, 0, 0, -180, 0, 0);
    CCAction* action6=CCSequence::create(CCSpawn::create((CCFiniteTimeAction *)actionFunc2,(CCFiniteTimeAction *)actionFlip2,NULL),
                                         CCCallFuncN::create(this, callfuncN_selector(SGLandingReward::testeffect)),NULL);
    tempBg->runAction(action6);
    
    menu->setEnabled(true);
}
bool SGLandingReward::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    //拦截了除了确定按钮和CCScrollView的点击事件
    CCPoint touchPt = pTouch->getLocation();
    if(enterGame->boundingBox().containsPoint(touchPt))
    {
        if (currChance == chances && state == 1) {

            enterGame->selected();
        }
        
        return true;
    }
    for (int i = 0; i<tagArray->count(); i++)
    {
        SGButton *a = (SGButton *)tagArray->objectAtIndex(i);

       if( i!= btntag && a->boundingBox().containsPoint(touchPt))
       {       
         a->selected();
         return true;
       }
    }
    
    return true;
        
}


void SGLandingReward::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPt = pTouch->getLocation();
    for (int i = 0; i<9; i++)
    {
    SGButton *a = (SGButton *)menu->getChildByTag(i);
       if(a->boundingBox().containsPoint(touchPt))
       {
        if(a->isSelected())
          {
            a->unselected();
            a->activate();
           }
        }
    }
    
    if(enterGame->boundingBox().containsPoint(touchPt))
    {
        if(enterGame->isSelected())
        {
            enterGame->unselected();
            enterGame->activate();
        }
    }
}
void SGLandingReward::onEnter()
{
    SGBaseLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX + 1, true);//与cantAdvancebox区分
}

void SGLandingReward::onExit()
{
    SGBaseLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
void SGLandingReward::confirmHandler(CCNode *sender)
{
    EFFECT_PLAY(MUSIC_34);
    if (menu->isEnabled())
    {
        menu->setEnabled(false);
        if (currChance < SGPlayerInfo::sharePlayerInfo()->getLoginChance())
        {
            btntag = sender->getTag();
            
            
            main::Lottery9PurchRequest *request = new main::Lottery9PurchRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_VISIT_9LOTTERY, request);
        }
    }

//    SGSocketRequest *sr = new SGSocketRequest();
//    main::Lottery9PurchResponse *response = new main::Lottery9PurchResponse ();
//    response->set_itemid(1833);
//    response->set_itemnum(1);
//    response->set_itemtype(6);
//    tag= 1;
//    sr->m_msg = response;
//    lotteryListener((CCObject*)response);
}

 
void SGLandingReward::sortLandingButton(){
   
    if (btntag < 0) {
        return;
    }
    int tagi= btntag %3;
    int tagj= btntag /3 ;
    int tagi1=0;
     
    if (tagi>1) {
        tagi1= btntag-1;
    }else
    {
        tagi1+=1;
        
    }

    
    int  exchange1=-1;
    int  exchange2=-1;
    
    
    for (int i = 0; i < tagArray->count(); i++) {
         SGButton *AllBtn = (SGButton *)tagArray->objectAtIndex(i);
        
        int targetI=AllBtn->getTag()%3;
        int targetJ=AllBtn->getTag()/3;
        
        
        if ((targetI-1==tagi && targetJ==tagj) || (targetI+1==tagi && targetJ==tagj) ||(targetI==tagi && targetJ-1==tagj)||(targetI==tagi && targetJ+1==tagj)  ) {
            
            exchange1=i;
            srand((unsigned)time(NULL));
            
            float randNum = rand() % 10;
 
            if (randNum>6) {
                continue;
            }else
                break;
        }else if((targetI-1==tagi1 && targetJ==tagj) || (targetI+1==tagi1 && targetJ==tagj) ||(targetI==tagi1 && targetJ-1==tagj)||(targetI==tagi1 && targetJ+1==tagj) )
        {
            exchange2=i;
            
        }
            
    }
    if (exchange1!=-1) {
        tagArray->exchangeObjectAtIndex(0, exchange1);
    }else if (exchange2!=-1)
    {
        tagArray->exchangeObjectAtIndex(0, exchange2);
    }else
    {
        tagArray->exchangeObjectAtIndex(0, 4);
    }
    
}
void SGLandingReward::showCard()
{
    sortLandingButton();
    
//    EFFECT_PLAY(MUSIC_34);
    state = 1;
    enterGame->setImage("reward_btnbg.png");
    for (int i = 0; i < tagArray->count(); i++)
    {
        SGButton *AllBtn = (SGButton *)tagArray->objectAtIndex(i);
        
        CCSprite* AllBG = CCSprite::createWithSpriteFrameName("pai_Z.png");
        AllBG->setPosition(AllBtn->getPosition());
        SGLottery9ViewItem* item = (SGLottery9ViewItem *)SGPlayerInfo::sharePlayerInfo()->get9Items()->objectAtIndex(8-i);
        
        int itemType = item->getItemTypeItem();
        int itemId = item->getItemIDItem();
        int itemNum = item->getItemNum();
        
        
        switch (itemType)
        {
            case 1:
            {
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat(str_LandingReward_str5,itemNum)->getCString(), FONT_BOXINFO,20);
                CCSprite *kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                AllBG->addChild(kuang,3);
                kuang->setPosition(ccp(AllBG->getContentSize().width/2, label->getContentSize().height*.5 +AllBG->getContentSize().height/2-3.7));
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                AllBG->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                
                CCSprite *coin = CCSprite::createWithSpriteFrameName("head9999.png");
                AllBG->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                
                AllBG->addChild(label,2);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
                
                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);
            }
                break;
            case 2:
            {
                
            }
                break;
            case 3:
            {
                
            }
                break;
            case 4:
            {
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat(str_LandingReward_str6,itemNum)->getCString(), FONT_BOXINFO, 20);
                CCSprite *kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_5_0.png");
                AllBG->addChild(kuang,3);
                kuang->setPosition(ccp(AllBG->getContentSize().width/2, label->getContentSize().height*.5 +AllBG->getContentSize().height/2-3.7));
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                AllBG->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                CCSprite *coin = CCSprite::createWithSpriteFrameName("head9998.png");
                AllBG->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                AllBG->addChild(label,2);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));

                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);
            }
                break;
            case 5:
            {
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat(str_LandingReward_str7,itemNum)->getCString(), FONT_BOXINFO,20);
                CCSprite *kuang = CCSprite::createWithSpriteFrameName("equipPinzhi_3_0.png");
                AllBG->addChild(kuang,3);
                kuang->setPosition(ccp(AllBG->getContentSize().width/2, label->getContentSize().height*.5 +AllBG->getContentSize().height/2-3.7));
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                AllBG->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                CCSprite *coin = CCSprite::createWithSpriteFrameName("head9996.png");
                AllBG->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);

                AllBG->addChild(label,2);
               label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));

                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);

            }
                break;
            case 6:
            {
                SGOfficerDataModel *officer = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat("%s",officer->getOfficerName()->getCString())->getCString(), FONT_BOXINFO, 20);
                int starlvl = officer->getOfficerCurrStarLevel();
                if (starlvl>1) {
                    starlvl-=1;
                }
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
                AllBG->addChild(kuang,3);
                kuang->setPosition(ccp(AllBG->getContentSize().width/2, label->getContentSize().height*.5 +AllBG->getContentSize().height/2-3.7));
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                AllBG->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                SGMainManager::shareMain() -> addHeadIconPlistByNum(officer->getIconId(),sg_landingRewardLayer);
                
                //if (str_1->isEqual(CCString::create("SGOfficerCard")))
                {
                    kuang->removeChildByTag(30);
                    CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",
                                                                                                            officer->getOfficerRace()))->getCString());
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setFlipX(true);
                    countryName->setPosition(ccp(kuang->getContentSize().width-kuang->getContentSize().width*0.18,kuang->getContentSize().height -
                                                 countryName->getContentSize().height));
                    kuang->addChild(countryName,30,30);
                    
                    if (officer->getAdvNum() > 0) {
                        SGCCLabelTTF *advNum = SGCCLabelTTF::create(CCString::createWithFormat("+%d",officer->getAdvNum())->getCString(), FONT_PANGWA, 24);
                        advNum->setInsideColor(ccGREEN);
                        advNum->setAnchorPoint(ccp(1,1));
                        advNum->ignoreAnchorPointForPosition(false);
                        advNum->setFlipX(true);
                        advNum->setPosition(ccp(kuang->getContentSize().width*0.4,kuang->getContentSize().height - advNum->getContentSize().height));
                        kuang->addChild(advNum,40);
                    }
                }
                
                CCSprite *coin = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",officer->getIconId())->getCString());
                AllBG->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                AllBG->addChild(label,2);
                
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));

                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);
            }
                break;
            case 7:
            {
                SGEquipmentDataModel *equip = SGStaticDataManager::shareStatic()->getEquipById(itemId);
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat("%s",equip->getEquipName()->getCString())->getCString(), FONT_BOXINFO, 20);
                
                CCString *str = CCString::createWithFormat("equipPinzhi_%d_0.png",equip->getEquipStarPinzhi());
                
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(str->getCString());
                AllBG->addChild(kuang,3);
                kuang->setPosition(ccp(AllBG->getContentSize().width/2, label->getContentSize().height*.5 +AllBG->getContentSize().height/2-3.7));
                SGMainManager::shareMain() -> addHeadIconPlistByNum(equip->getIconId(),sg_landingRewardLayer);
                
                CCSprite *coin = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",equip->getIconId())->getCString());
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                AllBG->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                AllBG->addChild(coin,2);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                AllBG->addChild(label,2);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));

                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);
            }
                break;
            case 8:
            {
                
            }
                break;
            case 9:
            {
                
                SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(itemId);
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat("%s",prop->getPropsName()->getCString())->getCString(), FONT_BOXINFO, 20);
                CCString *str = CCString::createWithFormat("equipPinzhi_%d_0.png",prop->getPropsCurrStarLevel());
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(str->getCString());                AllBG->addChild(kuang,4);
                kuang->setPosition(ccp(AllBG->getContentSize().width/2, label->getContentSize().height*.5 +AllBG->getContentSize().height/2-3.7));
                SGMainManager::shareMain() -> addHeadIconPlistByNum(prop->getIconId(),sg_landingRewardLayer);
                
                CCSprite *coin = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",prop->getIconId())->getCString());
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                AllBG->addChild(bg,2);
                bg->setPosition(kuang->getPosition());

                AllBG->addChild(coin,3);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                AllBG->addChild(label,3);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
                
                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);

            }
                break;
            case 10:
            {
                CCString *str_kuang = NULL;
                CCSprite *countryName = NULL;
                SGStaticDataManager *staticDataManager = SGStaticDataManager::shareStatic();
                SGPiecesDataModel *pieces = staticDataManager->getPiecesDictById(itemId);
                
                SGCCLabelTTF *label = SGCCLabelTTF::create(pieces->getPiecesName()->getCString(), FONT_BOXINFO, 20);
                
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
                    
                    
                     countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",
                                                                                                            temp->getOfficerRace()))->getCString());

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
                SGMainManager::shareMain()->addHeadIconPlistByNum(iconId,sg_landingRewardLayer);
                
                
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(str_kuang->getCString());
                AllBG->addChild(kuang,3);
                kuang->setPosition(ccp(AllBG->getContentSize().width/2, label->getContentSize().height*.5 +AllBG->getContentSize().height/2-3.7));
                
                CCSprite *piecesCard = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",iconId)->getCString());
                
                
                if (pieces->getCallingCardId() != -1)//不是通用碎片，加上蒙版
                {
                    CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
                    mask->setPosition(ccp(piecesCard->getContentSize().width / 2, piecesCard->getContentSize().height / 2));
                    piecesCard->addChild(mask);
                }
                
                if (countryName)
                {
                    countryName->setAnchorPoint(ccp(1,1));
                    countryName->ignoreAnchorPointForPosition(false);
                    countryName->setFlipX(true);
                    countryName->setPosition(ccp(kuang->getContentSize().width-kuang->getContentSize().width*0.18,kuang->getContentSize().height -
                                                 countryName->getContentSize().height));
                    kuang->addChild(countryName,30,30);
                }
                
                
                AllBG->addChild(piecesCard,2);
                piecesCard->setPosition(kuang->getPosition());
                piecesCard->setScale(1.1);
                kuang->setScale(1.1);
                AllBG->addChild(label,2);
                
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
                
                kuang->setFlipX(true);
                piecesCard->setFlipX(true);
                label->setFlipX(true);
                
                
            }
                break;
            case 11:
            {
                
            }
                break;
            case 12:
            {
                
            }
                break;
            case 13:
            {
                
            }
                break;
            case 14:
            {
                
            }
                break;
            case 15:
            {
                
                SGConsumableDataModel *consume = SGStaticDataManager::shareStatic()->getConsumableById(itemId);
                
                SGCCLabelTTF *label = SGCCLabelTTF::create(CCString::createWithFormat("%s",consume->getConsumeName()->getCString())->getCString(), FONT_BOXINFO, 20);
                CCString *str = CCString::createWithFormat("equipPinzhi_%d_0.png",consume->getConsumeStarLvl());
                CCSprite *kuang = CCSprite::createWithSpriteFrameName(str->getCString());
                AllBG->addChild(kuang,4);
                kuang->setPosition(ccp(AllBG->getContentSize().width/2, label->getContentSize().height*.5 +AllBG->getContentSize().height/2-3.7));
                SGMainManager::shareMain() -> addHeadIconPlistByNum(consume->getIconId(),sg_landingRewardLayer);
                
                CCSprite *coin = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("head%d.png",consume->getIconId())->getCString());
                CCSprite *bg = CCSprite::createWithSpriteFrameName("card_bj.png");
                AllBG->addChild(bg,2);
                bg->setPosition(kuang->getPosition());
                
                AllBG->addChild(coin,3);
                coin->setPosition(kuang->getPosition());
                coin->setScale(1.1);
                kuang->setScale(1.1);
                AllBG->addChild(label,3);
                label->setPosition(ccpAdd(kuang->getPosition(), ccp(0,  - kuang->getContentSize().height/2 +3)));
                kuang->setFlipX(true);
                coin->setFlipX(true);
                label->setFlipX(true);
            }
                break;
                
            default:
                break;
        }
        
        //6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌 1:铜钱 0:金子 3:军功 5:炼丹秘方 4:小喇叭 10:碎片 11军魂 12体力 13军粮 14天梯积分 15消耗品

        this->addChild(AllBG,10);
        AllBG->setVisible(false);
        CCAction *actionFunc1 =  CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(SGLandingReward::testFlipCardCallFunc3)),
                                                    CCDelayTime::create(0.6),CCCallFuncN::create(this, callfuncN_selector(SGLandingReward::testFlipCardCallFunc4)),
                                                    CCDelayTime::create(0.5),NULL);
        CCOrbitCamera *actionFlip1 = CCOrbitCamera::create(1, 1, 0, 0, -180, 0, 0);
        
        AllBtn->runAction(CCSpawn::create((CCFiniteTimeAction *)actionFunc1,(CCFiniteTimeAction *)actionFlip1,NULL));
        
        CCAction *actionFunc2 =  CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(SGLandingReward::testFlipCardCallFunc4)),
                                                    CCDelayTime::create(0.6),CCCallFuncN::create(this, callfuncN_selector(SGLandingReward::testFlipCardCallFunc3)),
                                                    CCDelayTime::create(0.5),NULL);
        CCOrbitCamera *actionFlip2 = CCOrbitCamera::create(1, 1, 0, 0, -180, 0, 0);
        AllBG->runAction(CCSpawn::create((CCFiniteTimeAction *)actionFunc2,(CCFiniteTimeAction *)actionFlip2,NULL));

        }
    tagArray->removeAllObjects();
}
void SGLandingReward::testFlipCardCallFunc1()
{
    tempBtn->setVisible(true);
    tempBg->setVisible(false);

}
void SGLandingReward::testFlipCardCallFunc2()
{
    tempBtn->setVisible(false);
    tempBg->setVisible(true);
}

void SGLandingReward::testFlipCardCallFunc3(CCNode *node)
{
    
    CCLOG("testFlipCardCallFunc3::tempTag:%d",tempTag);
    node->setVisible(true);
  
}
void SGLandingReward::testFlipCardCallFunc4(CCNode *node)
{
    CCLOG("testFlipCardCallFunc4::tempTag:%d",tempTag);
    node->setVisible(false);
}

void SGLandingReward::boxCloseWithOutCallBack()
{    
    EFFECT_PLAY(MUSIC_BTN);
    if(deletage){
        deletage->setIsCanTouch(true);
    }
    main::AnnouncementRequest *request = new main::AnnouncementRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_ANNOUCEMENT_MESSAGE, request);
    SGPlayerInfo::sharePlayerInfo()->setis9Show(0);
    this->removeFromParentAndCleanup(true);
}

void SGLandingReward::testeffect(CCNode *node)
{
  
    CCNode *n=(CCNode*)node;
    ResourceManager::sharedInstance()->bindTexture("animationFile/short.plist", RES_TYPE_LAYER_UI, sg_landingRewardLayer);
    CCSpriterX *newRewardEffect = CCSpriterX::create("animationFile/short.scml", true, true);
    newRewardEffect->setanimaID(0);
    newRewardEffect->setisloop(true);
    newRewardEffect->setPosition(n->getPosition());
    newRewardEffect->setScale(0.95);
    this->addChild(newRewardEffect);
    newRewardEffect->play();
 
 
}












