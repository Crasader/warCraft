//
//  SGAdvancedEffectLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-10-23.
//
//

#include "SGAdvancedEffectLayer.h"
#include "SGMainManager.h"
#include "SGShowString.h"
#include "SGStaticDataManager.h"
#include "SGEquipInfolayer.h"
#include "SGGuideManager.h"
#include "SGTools.h"

#define   CARDMOVEUPRATE    0.1
#define   SHOWCARDGAPTIME   0.09

SGAdvancedEffectLayer::SGAdvancedEffectLayer():
itemType(BIT_INGOT),
afterAdvanceItemId(-1),
beforeAdvanceItemId(-1),
beforeAdvancedItemPic(NULL),
afterAdvancedItemPic(NULL),
itemName(NULL),
successTips(NULL),
isPlayCompleted(false),
afterAdvancedCard(NULL),
m_officerFrom(-1),
adNumLabel(NULL)
{
    
}
SGAdvancedEffectLayer::~SGAdvancedEffectLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_advancedEffectLayer);
}

/*
 itemType 类型
 beforeAdvanceItemId 转生之前的itemid
 afterAdvanceItemId 转生之后的itemid
 
 之后根据itemType 在initView中做匹配处理
 
 武将召唤成功
 */

SGAdvancedEffectLayer *SGAdvancedEffectLayer::create(SGBaseItemType itemType, int beforeAdvanceItemId, int afterAdvanceItemId  , int officerFrom, int extArg)
{
    SGAdvancedEffectLayer *eff = new SGAdvancedEffectLayer();
    
    if (eff && eff->init(NULL, sg_advancedEffectLayer))
    {
        eff->itemType = itemType;
        eff->beforeAdvanceItemId = beforeAdvanceItemId;
        eff->afterAdvanceItemId = afterAdvanceItemId;
        eff->m_officerFrom = officerFrom;
        eff->_extArg = extArg;
        eff->initView();
        eff->autorelease();
        return eff;
    }
    CC_SAFE_RELEASE(eff);
    return NULL;
}


void SGAdvancedEffectLayer::initView()
{
    
    
//    ccColor3B color[4] = {ccc3(0 , 239 , 22) , ccc3(75, 147, 255) ,ccc3(215,56,255) , ccc3(249, 158, 0)};
//
//    ccColor3B titleColor;
    
    ResourceManager::sharedInstance()->bindTexture("animationFile/Lightspot.plist",RES_TYPE_LAYER_UI ,sg_advancedEffectLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/VisitArray.plist",RES_TYPE_LAYER_UI ,sg_advancedEffectLayer);
    
    int adNum = 0;
    int quality=0;
    //类型是全局通用的那个物品类型
    char beforeItemPicStr[32] = "\0";
    char afterItemPicStr[32] = "\0";
    char itemNameStr[32] = "\0";
    char itemAdNum[32] = "\0";// 小转生次次数
    //6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌 1:铜钱 0:金子 2天梯积分  3:军功 5:炼丹秘方 4:小喇叭 10:碎片 11军魂 12体力 13军粮 14号(废弃) 15消耗品 16 勋章 17 材料

    //各种类型参数
    switch (itemType)
    {
        case BIT_EQUIP://装备卡牌
        {
            SGEquipmentDataModel *beforeEquipModel = SGStaticDataManager::shareStatic()->getEquipById(beforeAdvanceItemId);
            SGEquipmentDataModel *afterEquipModel = SGStaticDataManager::shareStatic()->getEquipById(afterAdvanceItemId);
            
            int beforeId = beforeEquipModel->getIconId();
            int afterId = afterEquipModel->getIconId();
            
            SGMainManager::shareMain()->addEquipPng(beforeId, sg_advancedEffectLayer);
            SGMainManager::shareMain()->addEquipPng(afterId, sg_advancedEffectLayer);
            
            sprintf(beforeItemPicStr, "equip%d.png", beforeId);
            sprintf(afterItemPicStr, "equip%d.png", afterId);
            
            sprintf(itemNameStr, "%s", afterEquipModel->getEquipName()->getCString());
            
            adNum = afterEquipModel->getAdvNum();
            if (adNum > 0)
            {
                sprintf(itemAdNum, "+%d", adNum);
            }
            
            //装备边框颜色
            quality = afterEquipModel->getEquipCurrStarLevel();
            
//            if(quality < 3 || quality > 6)
//                titleColor = ccGREEN;
//            else
//                titleColor = color[quality - 3];
            
            
        }
            break;
        case BIT_OFFICER://武将卡牌
        {
            SGOfficerDataModel *beforeEquipModel = SGStaticDataManager::shareStatic()->getOfficerById(beforeAdvanceItemId);
            SGOfficerDataModel *afterEquipModel = SGStaticDataManager::shareStatic()->getOfficerById(afterAdvanceItemId);
            
            int beforeId = beforeEquipModel->getIconId();
            int afterId = afterEquipModel->getIconId();
            
            SGMainManager::shareMain()->addOfficerPng(beforeId, sg_advancedEffectLayer);
            SGMainManager::shareMain()->addOfficerPng(afterId, sg_advancedEffectLayer);
            
            sprintf(beforeItemPicStr, "officer_%d.png", beforeId);
            sprintf(afterItemPicStr, "officer_%d.png", afterId);
            
            sprintf(itemNameStr, "%s", afterEquipModel->getOfficerName()->getCString());
            
            adNum = afterEquipModel->getAdvNum();
            if (adNum > 0)
            {
                sprintf(itemAdNum, "+%d", adNum);
            }
            
            //装备边框颜色
            quality = afterEquipModel->getOfficerCurrStarLevel();
            
//            if(quality < 3 || quality > 6)
//                titleColor = ccGREEN;
//            else
//                titleColor = color[quality - 3];
            
            break;
        }
            
        default:
            break;
    }
    
    //构造sprite
    beforeAdvancedItemPic = CCSprite::createWithSpriteFrameName(beforeItemPicStr);
    afterAdvancedItemPic = CCSprite::createWithSpriteFrameName(afterItemPicStr);
    itemName = SGCCLabelTTF::create(itemNameStr, FONT_PANGWA, 35, SGTools::getColorByQuality(quality));
    if (adNum)
    {
        adNumLabel = SGCCLabelTTF::create(itemAdNum, FONT_PANGWA, 34, ccGREEN);
    }
    
    
    
    //黑背景
    CCLayerColor *blackBg = CCLayerColor::create(ccc4(0, 0, 0, 255), CCDirector::sharedDirector()->getWinSize().width,
                                                 CCDirector::sharedDirector()->getWinSize().height);
    this->addChild(blackBg, 0);
    blackBg->setPosition(ccp(0, 0));
    blackBg->addChild(beforeAdvancedItemPic, 1);
    blackBg->addChild(itemName, 1);
    
    beforeAdvancedItemPic->setPosition(ccpAdd(blackBg->getPosition(), ccp(blackBg->getContentSize().width / 2,
                                                                          blackBg->getContentSize().height / 2)));
    itemName->setPosition(ccpAdd(beforeAdvancedItemPic->getPosition(), ccp(0, beforeAdvancedItemPic->getContentSize().height / 2
                                                                           + itemName->getContentSize().height + 20)));
    if (adNumLabel)
    {
        blackBg->addChild(adNumLabel, 1);
        adNumLabel->setPosition(ccpAdd(itemName->getPosition(), ccp(itemName->getContentSize().width / 2 + 10 + adNumLabel->getContentSize().width, 0)));
    }
    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //添加动画 法阵旋转
    CCSpriterX *visitArray = CCSpriterX::create("animationFile/Array.scml",true,true);
    visitArray->setisloop(true);
    visitArray->play();
    visitArray->setanimaID(0);
    blackBg->addChild(visitArray,1);
    visitArray->setPosition(ccp(winSize.width/2, winSize.height*(0.8-0.28) - 50));
    //法阵上升粒子
    CCSpriterX *lightSpot = CCSpriterX::create("animationFile/Lightspot.scml",true,true);
    lightSpot->setisloop(true);
    lightSpot->play();
    lightSpot->setanimaID(0);
    blackBg->addChild(lightSpot,50);
    lightSpot->setPosition(ccp(winSize.width/2, winSize.height*(0.8-0.2)));
    
        //构造动画
    CCCallFuncN *setPicVisiable = CCCallFuncN::create(this,
                                                      callfuncN_selector(SGAdvancedEffectLayer::setPictureVisiable));
    CCCallFuncN *setTipsVisiable = CCCallFuncN::create(this,
                                                       callfuncN_selector(SGAdvancedEffectLayer::setSuccessTipsVisiable));
    CCCallFuncN *setComplete = CCCallFuncN::create(this,
                                                   callfuncN_selector(SGAdvancedEffectLayer::setisPlayCompleted));
    
    afterAdvancedItemPic->setScale(5);
    blackBg->addChild(afterAdvancedItemPic, 2);
    afterAdvancedItemPic->setPosition(beforeAdvancedItemPic->getPosition());
    
    CCString * str = NULL;
    int fSize = 36;
    if(m_officerFrom == 1)
        str = CCString::create(str_adv_eff_1);
    else if (2 == m_officerFrom)
        str = CCString::create(str_adv_eff_2);
    else if(-1 == m_officerFrom)
        str = CCString::create(str_adv_eff_1);
    else if (3 == m_officerFrom)
    {
        str = CCString::createWithFormat(str_adv_eff_3, _extArg);
        fSize = 26;
    }
    else
    {
        //默认只显示 『转生成功』
        str = CCString::create(str_adv_eff_1);
    }
    successTips = SGCCLabelTTF::create(str->getCString(), FONT_PANGWA, fSize);
    blackBg->addChild(successTips, 3);
    successTips->setPosition(ccpAdd(afterAdvancedItemPic->getPosition(), ccp(0, -afterAdvancedItemPic->getContentSize().height / 2
                                                                             - successTips->getContentSize().height - 100)));
    successTips->setVisible(false);


    
    CCSequence *seq = CCSequence::create(CCDelayTime::create(0.1),setPicVisiable,
                                         CCScaleTo::create(SHOWCARDGAPTIME*2,0.89),
                                         CCScaleTo::create(SHOWCARDGAPTIME,1.13),
                                         CCScaleTo::create(SHOWCARDGAPTIME,1.06),
                                         CCScaleTo::create(SHOWCARDGAPTIME,1), CCDelayTime::create(0.15),
                                         setTipsVisiable, CCDelayTime::create(0.15), setComplete, NULL);
    
    afterAdvancedItemPic->runAction(seq);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -255, true);
}

void SGAdvancedEffectLayer::setPictureVisiable(CCObject *obj)
{
    beforeAdvancedItemPic->setVisible(false);
}
void SGAdvancedEffectLayer::setisPlayCompleted(CCObject *obj)
{
    isPlayCompleted = true;
}
void SGAdvancedEffectLayer::setSuccessTipsVisiable(CCObject *obj)
{
    successTips->setVisible(true);
}
bool SGAdvancedEffectLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!isPlayCompleted)
    {
        return false;
    }
    else
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

        //不同的返回处理
        switch (itemType)
        {
            case BIT_EQUIP://装备卡牌
            {
                if (m_officerFrom == 4 && !SGGuideManager::shareManager()->isGuide)
                {
                    //@非常重要：其实运行到这里，SGGeneralInfoLayer还存在，所以这里即使oid是0导致的SGOfficerCard为NULL，也没有关系，这是不合理的业务出现想要的结果，出现问题再议。
                    int oid = SGPlayerInfo::sharePlayerInfo()->getEquipCard(SGMainManager::shareMain()->getAfterAdvancedSsid())->getOfficerCardId();
                    SGOfficerCard* ocard =  SGPlayerInfo::sharePlayerInfo()->getOfficerCard(oid);
                    SGMainManager::shareMain()->showGeneralInfoLayer(ocard, 3,NULL,true,NULL,12);
                }
                else
                {
                    SGEquipCard *card = SGPlayerInfo::sharePlayerInfo()->getEquipCard(SGMainManager::shareMain()->getAfterAdvancedSsid());
                    SGMainManager::shareMain()->showEquipInfoLayer(card, 0);
                    if (SGGuideManager::shareManager()->isGuide)
                    {
                        SGGuideManager::shareManager()->startLimitGuide(-11);
                    }
                }
            }
                break;
            case BIT_OFFICER://武將卡牌
            {
                if (m_officerFrom == 3) //碎裂不做返回处理，直接回到上一个页面。
                {
                    this->removeFromParent();
                }
                else
                {
                    SGOfficerCard * card = SGPlayerInfo::sharePlayerInfo()->getOfficerCard(SGMainManager::shareMain()->getAfterAdvancedSsid());
                    SGMainManager::shareMain()->showGeneralInfoLayer(card, 911);
                    if (SGGuideManager::shareManager()->isGuide)
                    {
                        SGGuideManager::shareManager()->startLimitGuide(-11);
                    }
                }
            }

            default:
                break;
        }
        return true;
    }
}


