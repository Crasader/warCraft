//
//  SGSpecialBattleSecionCell.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-12-12.
//
//

#include "SGSpecialBattleSecionCell.h"
#include "SGShowRewardBox.h"
#include "SpecialBattleModule.pb.h"


SGSpecialBattleSecionCell::SGSpecialBattleSecionCell():
openCondition(NULL),
bgId(-1),
sectionName(NULL),
cellData(NULL),
greyMask(NULL),
bgButton(NULL)
{
    
}

SGSpecialBattleSecionCell::~SGSpecialBattleSecionCell()
{
    
}


SGSpecialBattleSecionCell *SGSpecialBattleSecionCell::create(SpecialBattleSecData *cellData)
{
    SGSpecialBattleSecionCell *spCell = new SGSpecialBattleSecionCell();
    
    if (spCell && spCell->init())
    {
        spCell->autorelease();
        spCell->cellData = cellData;
        spCell->initView();
        
        return spCell;
    }
    CC_SAFE_DELETE(spCell);
    return NULL;
}

void SGSpecialBattleSecionCell::initView()
{
    if (!cellData)
    {
        return ;
    }
    
    
    //章名称
    sectionName = SGCCLabelTTF::create(cellData->sectionName.c_str(), FONT_PANGWA, 28);
    sectionName->setAnchorPoint(ccp(0, 0.5));
    this->addChild(sectionName, 10);
    sectionName->setPosition(ccpAdd(ccp(-bgSize.width/2+ZoomInX(20), 2), ccp(-20, sectionName->getContentSize().height * 1.6)));

    SGMainManager::shareMain()->addHeadIconPlistByNum(514, sg_specialBattleSecionLayer);
    
    //掉落预览按钮 //255	198	11
    SGButton *dropListPreviewBtn = SGButton::createFromLocal("head514.png", "掉落物品", this,
                                                             menu_selector(SGSpecialBattleSecionCell::dropListPreviewWindow), ccp(0, -30), FONT_PANGWA, ccc3(255, 198, 11), 24);
    this->addBtn(dropListPreviewBtn);
    this->addChild(dropListPreviewBtn, 10);
    dropListPreviewBtn->setPosition(ccpAdd(sectionName->getPosition(), ccp(dropListPreviewBtn->getContentSize().width / 2,
                                                                           -sectionName->getContentSize().height - dropListPreviewBtn->getContentSize().height / 1.5)));
    
    //‘战’按钮
    //如果开启才显示对应的战字按钮
    battleBtn = SGButton::create("mainBtnZhan_bg.png", NULL, this, menu_selector(SGSpecialBattleSecionCell::startBattle));
    this->addBtn(battleBtn);
    this->addChild(battleBtn, 10);
    battleBtn->setPosition(ccpAdd(dropListPreviewBtn->getPosition(), ccp(bgSize.width - battleBtn->getContentSize().width - 20, 0)));
    
    if (!cellData->canJoin)
    {
        battleBtn->setVisible(false);

    }
    
    //条件提示 //255	 204 12
    openCondition = SGCCLabelTTF::create(cellData->conditionTips.c_str(), FONT_PANGWA, 19);
    openCondition->setAnchorPoint(ccp(0, 0.5));
    openCondition->setInsideColor(ccc3(255, 204, 12));
    this->addChild(openCondition, 10);
    openCondition->setPosition(ccpAdd(ccp(battleBtn->getPositionX(), sectionName->getPositionY()), ccp(-battleBtn->getContentSize().width * 1.8, battleBtn->getContentSize().height * .6f)));
    
    //如果可参加， 显示建议战斗力
    if (cellData->canJoin)
    {
        int suggestPower = cellData->suggestPower;
        openCondition->setString(CCString::createWithFormat(str_SpecialBattleSuggestPower, suggestPower)->getCString());
    }
    
    int iconId = cellData->bgIconId;
    SGMainManager::shareMain()->addSpHeadBgPlistByNum(iconId, sg_specialBattleSecionLayer);
    
    //重置底图（武将cell大图背景）
    bgButton = SGButton::create("sp_battle_sec_bg.png", CCString::createWithFormat("sphead%d.png", iconId)->getCString(),
                                          this, NULL, CCPointZero, false, true);
    bgButton->setEnabled(false);
    btn = bgButton;
    
    //加上蒙灰
    greyMask = CCSprite::createWithSpriteFrameName("sp_battle_sec_mask.png");
    greyMask->setOpacity(150);
    btn->addChild(greyMask, 1);
    greyMask->setPosition(ccpAdd(ccp(btn->getContentSize().width / 2, btn->getContentSize().height / 2), ccp(0, 0)));
    greyMask->setVisible(!cellData->canJoin);
    
    
    this->addChild(btn, 0);
}

//滑动更新cell
void SGSpecialBattleSecionCell::updateSpCell(SpecialBattleSecData *cellData)
{
    if (!cellData)
    {
        return ;
    }
    this->cellData = cellData;
    
    //更新节名称
    sectionName->setString(cellData->sectionName.c_str());
    //战字按钮显示与否
    battleBtn->setVisible(cellData->canJoin);
    battleBtn->setEnabled(cellData->canJoin);
    //条件
    if (cellData->canJoin)
    {
        int suggestPower = cellData->suggestPower;
        openCondition->setString(CCString::createWithFormat(str_SpecialBattleSuggestPower, suggestPower)->getCString());
    }
    else
    {
        openCondition->setString(cellData->conditionTips.c_str());
    }
    //底图改变
    int iconId = cellData->bgIconId;
    SGMainManager::shareMain()->addSpHeadBgPlistByNum(iconId, sg_specialBattleSecionLayer);
    
    CCSprite *newBg = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("sphead%d.png", iconId)->getCString());
    
    bgButton->setFontImage(newBg->displayFrame());
    
    greyMask->setVisible(!cellData->canJoin);
    
}


//掉落预览弹窗
void SGSpecialBattleSecionCell::dropListPreviewWindow(CCObject *obj)
{
    CCLOG("Need Pop Window");
    
    SGMailSystemDataModel *systemMailDrop = new SGMailSystemDataModel();
    systemMailDrop->autorelease();
    
    for (int i = 0; i < cellData->dropVec.size(); i++)
    {
        DropList list = cellData->dropVec.at(i);
        SGMailSystemAccessoryDataModel *acc = new SGMailSystemAccessoryDataModel();
        acc->autorelease();
        
        acc->setItemId(list.itemId);
        acc->setType(list.itemType);
        //预览不显示数量
        acc->setCount(0);
        
        //把附件添加进邮件的数组里面
        systemMailDrop->getAccessoryArray()->addObject(acc);
    }
    
    //显示掉落预览的窗口，使用邮件里
    SGShowRewardBox *rewardBox = SGShowRewardBox::create(NULL, systemMailDrop->getAccessoryArray(), 4, SPBATTLE_PREVIEW);
    SGMainManager::shareMain()->showBox(rewardBox);
    
}

//点击战字按钮
void SGSpecialBattleSecionCell::startBattle(CCObject *obj)
{
    if(!cellData->canJoin)
    {
        return ;
    }
    //如果勾玉到上限
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerJewels() > 10000)
    {
        SG_SHOW_WINDOW(str_SpecilaBattleLimit);
        return;
    }
    
    
    main::SpecialBattleCheckFightRequest *fightReq = new main::SpecialBattleCheckFightRequest;
    fightReq->set_plotid(cellData->plotId);
    //全局记下特殊战斗的plotid
    SGMainManager::shareMain()->setSpBattlePlotId(cellData->plotId);
    SGSocketClient::sharedSocketClient()->send(MSG_SPECIAL_BATTLE_ENTRY, fightReq);
}








