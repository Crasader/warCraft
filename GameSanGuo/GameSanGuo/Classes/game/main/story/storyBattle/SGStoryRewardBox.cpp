//
//  SGStoryRewardBox.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-7.
//
//

#include "SGStoryRewardBox.h"
#include "SGStoryRewardItem.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
#include "SGStaticDataManager.h"
#include "SGAIManager.h"
#include "SGBattleManager.h"
#include "SGPvpMainLayer.h"
#include "SGHeroLayer.h"
#include "SGMainManager.h"
#include "SGFriend.h"
#include "SGGeneralBox.h"
#include "SGCantAdvanceBox.h"
#include "SGTestSwitch.h"
#include "SimpleAudioEngine.h"
#include "PlotModule.pb.h"
#include "ResourceManager.h"
#include "SGGuideManager.h"
#include "SGStringConfig.h"
#include "SGFightOverNoticeBox.h"
#include "SGFightRewardLayer.h"
SGStoryRewardBox::SGStoryRewardBox(void) : ldanCount(-1)
{
    ;
}

SGStoryRewardBox::~SGStoryRewardBox(void)
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_storyRewardBox);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PLAYERINFO);
}

SGStoryRewardBox* SGStoryRewardBox::create(SGBoxDelegate *delegate, SGStoryReward *reward)
{
    SGStoryRewardBox *box = new SGStoryRewardBox();
    if (box && box->init(delegate, box_btrewardTag, CCSizeMake(skewing(250), skewing(365)), false)) {
        box->initView(reward);
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

void SGStoryRewardBox::initView(SGStoryReward *reward)
{
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_PLAYERINFO, this,
																  callfuncO_selector(SGStoryRewardBox::getFriendOfficerListener));
	
    float kh = 50;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    ldanCount = reward->getLianDan();
    
    this->datas =  CCArray::create();
    this->datas->retain();
    for (int ii = 0; ii < reward->getItems()->count(); ii++) {
        CCObject *obj = reward->getItems()->objectAtIndex(ii);
        this->datas->addObject(obj);
    }
	
	
	ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_box_storyRewardBox);
	ResourceManager::sharedInstance()->bindTexture("sgfindhelplayer/sgfindhelplayer.plist", RES_TYPE_LAYER_UI, sg_box_storyRewardBox);
	ResourceManager::sharedInstance()->bindTexture("sgfightrewardlayer/sgfightrewardlayer.plist", RES_TYPE_LAYER_UI, sg_box_storyRewardBox);
	ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_box_storyRewardBox, LIM_PNG_AS_PNG);

	CCSprite *title = CCSprite::createWithSpriteFrameName("font_pvpover.png");
    title->setPosition(ccpAdd(center, ccp(0, skewing(183-kh))));
    title->setPosition(ccpAdd(center, ccp(0, this->boxSize.height*.5 -55)));
    this->addChild(title);
    
    float kwidth = -this->getContentSize().width + skewing(210.0f);
    const char* fontName = FONT_BOXINFO;
    float fontSize = FONTSIZE(12);
    
    int rate = reward->getRating();
    std::string suffix = "";
    if (rate == 5) suffix = "0%";
    if (rate == 4) suffix = "15%";
    if (rate == 3) suffix = "16%~60%";
    if (rate == 2) suffix = "51%~85%";
    if (rate == 1) suffix = str_OverBattleBox_str11;
    
    float posY = skewing(-30);
    CCString *string = CCString::createWithFormat(str_OverBattleBox_str12, (reward->getHpRate()*100));
    if (reward->getHpRate()==0)
        string = CCString::create(str_OverBattleBox_str13);
    SGCCLabelTTF *labelTTF = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    labelTTF->setAnchorPoint(ccp(0, 0.5f));
    labelTTF->setPosition(ccpAdd(title->getPosition(), ccp(kwidth, posY)));
    this->addChild(labelTTF);
    
    string = CCString::create(str_OverBattleBox_str14);
    labelTTF = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    labelTTF->setAnchorPoint(ccp(0, 0.5f));
    labelTTF->setPosition(ccpAdd(title->getPosition(), ccp(skewing(10), posY)));
    this->addChild(labelTTF);
    CCString *fname = CCString::createWithFormat("Icon_win%d.png", rate);
    CCSprite *winSprite = CCSprite::create(fname->getCString());
    winSprite->setPosition(ccpAdd(title->getPosition(), ccp(skewing(80), posY)));
    this->addChild(winSprite);
    
    SGPlayerInfo *playInfo = SGPlayerInfo::sharePlayerInfo();
    //int currExp = playInfo->getPlayerExp();
    //int currLevel = playInfo->getPlayerLevel();
    
//    int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
//    int modelExp = 0;
//    SGStoryBean *bean = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(plotId);
//    if (bean != NULL) {
//        modelExp = bean->getSceneBeanById(plotId)->getDropExp();
//    }
//    int extExp = reward->getExp() - modelExp;
    if (rate == 0) { // 战败
        string = CCString::createWithFormat(str_OverBattleBox_str15, (reward->getExp()+reward->getExtExp()));
    }else {
        string = CCString::createWithFormat(str_OverBattleBox_str16, reward->getExp(), reward->getExtExp());
    }
    
    labelTTF = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    labelTTF->setAnchorPoint(ccp(0, 0.5f));
    labelTTF->setPosition(ccpAdd(center, ccp(kwidth, skewing(137-kh))));
    this->addChild(labelTTF);
    
    // 加经验－升级
    bool isLevelUp = updateLevel(reward->getExp()+reward->getExtExp());
    
    int currLevel = playInfo->getPlayerLevel();
    int currExp = playInfo->getPlayerExp();
    int cMaxExp = SGStaticDataManager::shareStatic()->getPlayerByLevel(currLevel)->getPlayerExp();
    
    string = CCString::createWithFormat("Lv%d", currLevel);
    labelTTF = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    labelTTF->setAnchorPoint(ccp(0, 0.5f));
    labelTTF->setPosition(ccpAdd(center, ccp(kwidth, skewing(114-kh))));
    this->addChild(labelTTF);
    
    CCSprite *proTimerBg =CCSprite::createWithSpriteFrameName("progress.png");
    proTimerBg->setPosition(ccpAdd(center, ccp(skewing(20), skewing(114-kh))));
    proTimerBg->setScaleX(0.9f);
    this->addChild(proTimerBg);
    CCProgressTimer *proTimer = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("progress1.png"));
    proTimer->setType(kCCProgressTimerTypeBar);
    proTimer->setScaleX(proTimerBg->getScaleX());
    proTimer->setMidpoint(ccp(0,0));
    proTimer->setBarChangeRate(ccp(1, 0));
    proTimer->setPosition(proTimerBg->getPosition());
    this->addChild(proTimer);
    float rates = (float)currExp/cMaxExp;
    proTimer->setPercentage(rates*100);
    
    string = CCString::createWithFormat("%d/%d", currExp, cMaxExp);
    labelTTF = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    labelTTF->setPosition(proTimer->getPosition());
    this->addChild(labelTTF);
    
    CCSprite *redImg = CCSprite::createWithSpriteFrameName("bigredbg.png");
    redImg->setScaleX((this->boxSize.width-2)/redImg->getContentSize().width);
    redImg->setScaleY(skewing(50)/redImg->getContentSize().height);
    redImg->setPosition(ccpAdd(center, ccp(0, skewing(127-kh))));
    this->addChild(redImg, -1);
    
    string = CCString::createWithFormat(str_OverBattleBox_str17, reward->getCoin());
    labelTTF = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    labelTTF->setAnchorPoint(ccp(0, 0.5f));
    labelTTF->setPosition(ccpAdd(center, ccp(kwidth, skewing(90-kh))));
    this->addChild(labelTTF);
    int currCoins = playInfo->getPlayerCoins();
    if (!isLevelUp) playInfo->setPlayerCoins((currCoins+reward->getCoin()));
    
    string = CCString::createWithFormat(str_OverBattleBox_str18, reward->getJexp());
    labelTTF = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    labelTTF->setAnchorPoint(ccp(0, 0.5f));
    labelTTF->setPosition(ccpAdd(center, ccp(kwidth, skewing(75-kh))));
    this->addChild(labelTTF);
//    int currJexp = playInfo->getplayerAward();
//    if (!isLevelUp) playInfo->setplayerAward((currJexp+reward->getJexp()));
    
    string = CCString::createWithFormat(str_OverBattleBox_str19, reward->getGold());
    labelTTF = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    labelTTF->setAnchorPoint(ccp(0, 0.5f));
    labelTTF->setPosition(ccpAdd(center, ccp(kwidth, skewing(60-kh))));
    this->addChild(labelTTF);
    int currGolds = playInfo->getPlayerGold();
    if (!isLevelUp) playInfo->setPlayerGold((currGolds+reward->getGold()));
    
    string = CCString::create(str_OverBattleBox_str20);
    labelTTF = SGCCLabelTTF::create(string->getCString(), fontName, fontSize);
    labelTTF->setAnchorPoint(ccp(0, 0.5f));
    labelTTF->setPosition(ccpAdd(center, ccp(skewing(35), skewing(60-kh))));
    labelTTF->setPosition(ccpAdd(center, ccp(kwidth, skewing(35-kh))));
    this->addChild(labelTTF);
    
    //CCSprite *bottomBg = CCSprite::createWithSpriteFrameName("public_bj_2.png", CCRectMake(1, 0, this->boxSize.width-2, skewing(40)));
    CCSprite *bottomBg = CCSprite::createWithSpriteFrameName("public_bj_2.png");
    bottomBg->setPosition(ccpAdd(center, ccp(0, -this->boxSize.height/2+bottomBg->getContentSize().height/2)));
    this->addChild(bottomBg, -1);
    
    // table的上下金边
    //CCSprite *lineUp = CCSprite::createWithSpriteFrameName("bigredbg.png", CCRectMake(0, 0, redImg->getContentSize().width, skewing(1.0f)));
    CCSprite *lineUp = CCSprite::createWithSpriteFrameName("bigredbg.png");
    lineUp->setScaleX(this->boxSize.width/lineUp->getContentSize().width);
    lineUp->setPosition(ccpAdd(center, ccp(0, skewing(50-kh))));
    this->addChild(lineUp, -1);
    //CCSprite *lineDown = CCSprite::createWithSpriteFrameName("bigredbg.png", CCRectMake(0, 0, redImg->getContentSize().width, skewing(1.0f)));
    CCSprite *lineDown = CCSprite::createWithSpriteFrameName("bigredbg.png");
    lineDown->setScaleX(lineUp->getScaleX());
    lineDown->setPosition(ccpAdd(center, ccp(0, -this->boxSize.height/2+bottomBg->getContentSize().height)));
    this->addChild(lineDown, -1);
    //table的背景图片
    float tbgH = lineUp->getPositionY()-lineDown->getPositionY();
    //CCSprite *tableBg = CCSprite::createWithSpriteFrameName("storyBg.png", CCRectMake(0, 340, 768, 876-340));
    CCSprite *tableBg = CCSprite::createWithSpriteFrameName("storyBg.png");
    tableBg->setScaleX(this->boxSize.width/tableBg->getContentSize().width);
    tableBg->setScaleY((tbgH-2)/tableBg->getContentSize().height);
    tableBg->setPosition(ccpAdd(center, ccp(0, -this->boxSize.height/2+bottomBg->getContentSize().height+tbgH/2)));
    this->addChild(tableBg, -1);

    SGButton *okBtn = SGButton::create("box_btnbg.png",
                                       "public_font_queding.png",
                                       this,
                                       menu_selector(SGStoryRewardBox::confHandler),
                                       ccp(0, 0),
                                       false,
                                       true);
    this->addBtn(okBtn);
    okBtn->setPosition(ccpAdd(center, ccp(0, -this->boxSize.height/2+bottomBg->getContentSize().height/2-10)));
    
    float boxW = this->boxSize.width - skewing(20);
    float boxH = this->boxSize.height;
    tableView->setFrame(CCRectMake(0, 0, boxW, tbgH-skewing(25)));
    tableView->setPosition(ccpAdd(center, ccp(-boxW/2, -boxH/2+bottomBg->getContentSize().height)));
    tableViewHeight = skewing(57);
}
//收取好友信息监听
void SGStoryRewardBox::getFriendOfficerListener(CCObject *sender)
{
	SGMainManager *sGMainManager = SGMainManager::shareMain();
	SGSocketRequest *req = (SGSocketRequest *)sender;
	if (req)
	{
		main::FriendInfoResponse *friRes = (main::FriendInfoResponse *)req->m_msg;
		if (friRes)
		{
			SGGamePlayerInfo *playerInfo = SGFriend::getSGGamePlayerInfo(friRes->role());
			SGOfficerCard *officer = playerInfo->getLordOfficer();
			officer->retain();
			
            HelpFriendRoleName=playerInfo->getNickName()->getCString();
            HelpFriendItemLevel=playerInfo->getLevel();
            HelpFriendItemId=playerInfo->getLordid();
            HelpFriendItemLevel=playerInfo->getLevel();
			// 此处获得好友武将卡的信息
			SGGeneralBox *box = SGGeneralBox::create(NULL, 1, officer);
			box->setLdIndex(ldanCount);
			sGMainManager->closeBox();
			sGMainManager->showBox(box);
		}
	}
	
}

// confirm action
void SGStoryRewardBox::confHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    // 先弹是否升级界面
    // 如果没升级，弹是否添加援军为好友
    // 如果没升级，也不需添加好友，弹是否进入炼丹炉
    // 如果以上界面都没有，那么重新进入关卡界面
    
    
    int currLev = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
	
    if (currLev > HelpFriendPlayLev) { //升级了,升级界面
        SGGeneralBox *box = SGGeneralBox::create(deletage, 3);
        box->setLdIndex(ldanCount);
        SGMainManager::shareMain()->closeBox();
        SGMainManager::shareMain()->showBox(box,false);
        return;
    }
	 
    if (SGAIManager::shareManager()->isInitPve && SGFriend::isFriendOfPlayer(HelpFriendRoleId) != SGFRIEND_FRIEND && HelpFriendRoleId > 0) { //非好友，添加好友
        
		SGMainManager::shareMain()->getFriendOfficerInfo();//
		
		
//		SGGeneralBox *box = SGGeneralBox::create(deletage, 1);
//        box->setLdIndex(ldanCount);
//        SGMainManager::shareMain()->closeBox();
//        SGMainManager::shareMain()->showBox(box);
        return;
    }
    
    //是否西域商人
//    if(m_isShowExoticMerchant)
//    {
//        SGBaseBox * box = SGFightOverNoticeBox::create(NULL, 3);
//        SGMainManager::shareMain()->closeBox();//???也许是防止同时出现2个box.
//        SGMainManager::shareMain()->showBox(box);
//        return ;
//    }

    if (ldanCount >= 0) { // 开启炼丹炉
//        SGGeneralBox *box = SGGeneralBox::create(deletage, 2);
        SGFightOverNoticeBox * box = SGFightOverNoticeBox::create(NULL, 4);
        box->setLdIndex(ldanCount);
        SGMainManager::shareMain()->closeBox();
        SGMainManager::shareMain()->showBox(box);
        return;
    }
    
    // 如果上面这些box都没有，直接返回到原来的界面
//    this->gobackToStoryLayer();
    
    SGFightRewardLayer::gobackToStoryLayer();
}

void SGStoryRewardBox::delayShowPvpMain()
{
    this->unschedule(schedule_selector(SGStoryRewardBox::delayShowPvpMain));
    SGMainManager::shareMain()->showPvpMainLayer();
}

SNSTableViewCellItem* SGStoryRewardBox::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
	SGStoryRewardItem *item = (SGStoryRewardItem *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    if (item == NULL) {
        item = SGStoryRewardItem::create(1, 0, sg_box_storyRewardBox);
    }
    if (index < datas->count()) {
        SGStoryRdItem *model = (SGStoryRdItem *)datas->objectAtIndex(index);
        item->setData(model->getItemId(), model->getItemTag(), model->getItemType());
    }else {
        item->setVisible(false);
    }
    return item;
}

unsigned int SGStoryRewardBox::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    return datas->count();
}

void SGStoryRewardBox::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    
    EFFECT_PLAY(MUSIC_BTN);
    // 显示卡牌信息
}

unsigned int SGStoryRewardBox::tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section)
{
    // 4 列
    return 4;
}

// add cards to bag
void SGStoryRewardBox::addCardsToBag()
{
    SGPlayerInfo *playInfo = SGPlayerInfo::sharePlayerInfo();
    int size = this->datas->count();
    // 0 武将 1 装备 2 士兵 3 道具
    for(int ii= 0; ii < size; ii++) {
        SGStoryRdItem *item = (SGStoryRdItem *)this->datas->objectAtIndex(ii);
        int itemType = item->getItemType();
        int itemId = item->getItemId();
        CCLog("xx---%d------------------%d", itemType, itemId);
        if (itemType == 0) { // 0 武将卡
            SGOfficerDataModel *model = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
            
            SGOfficerCard *card = new SGOfficerCard();
            card->autorelease();
            card->setOfficerName(CCString::create(model->getOfficerName()->getCString()));
            playInfo->addOfficerCard(card);
        }else if (itemType == 1) { // 1 装备卡
            SGEquipmentDataModel *model = SGStaticDataManager::shareStatic()->getEquipById(itemId);
            
            SGEquipCard *card = new SGEquipCard();
            card->autorelease();
            card->setOfficerName(CCString::create(model->getEquipName()->getCString()));
            playInfo->addEquipCard(card);
        }else if (itemType == 2) { // 2 士兵卡
            SGSoldierDataModel *model = SGStaticDataManager::shareStatic()->getSoliderById(itemId);
            
            SGSoldierCard *card = new SGSoldierCard();
            card->autorelease();
            card->setOfficerName(CCString::create(model->getSoldierName()->getCString()));
            playInfo->addSoldierCard(card);
        }else if (itemType == 3) { // 3 道具卡
            SGPropsDataModel *model = SGStaticDataManager::shareStatic()->getPropsById(itemId);
            
            SGPropsCard *card = new SGPropsCard();
            card->autorelease();
            card->setOfficerName(CCString::create(model->getPropsName()->getCString()));
            playInfo->addPropCard(card);
        }
    }
}

// -------------全局函数---------------
// --------------start----------------
bool updateLevel(int exp)
{
    // 加经验以及升级问题
    SGPlayerInfo *playInfo = SGPlayerInfo::sharePlayerInfo();
    int playExp = playInfo->getPlayerExp() + exp;
    DDLog("tt---------------%d, %d, %d, %d", exp, playExp, playInfo->getPlayerLevel(), playLev);
    if (HelpFriendPlayLev == playInfo->getPlayerLevel())
    { // 不升级，直接加经验
        //setPlayerExp()应该只在服务器推送、更新角色信息时调用。
        //modified by:zyc.
        //playInfo->setPlayerExp(playExp);
        return false;
    }
    else
    { // 升级
        // 升级 已有方法做处理
        // playLev = playInfo->getPlayerLevel();
        return true;
    }
}


// 武将卡信息设置
SGBaseMilitaryCard* createOfficerCardWithInfo(main::OfficerCardItemProto proto)
{
    SGOfficerCard *officerCard = new SGOfficerCard();
    officerCard->autorelease();
    CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
    SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(proto.itemid());
    int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(proto.itemid());
    
//    if (temp->getOfficerExpType()->isEqual(CCString::create("A"))) {
//        type = 0;
//    }
//    else if (temp->getOfficerExpType()->isEqual(CCString::create("B"))) {
//        type = 1;
//    }
//    else {
//        type = 2;
//    }
    SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1);
    
    officerCard->setSsid(proto.ssid());
    officerCard->setItemId(proto.itemid());
    officerCard->setCurrExp(proto.currexp());
    officerCard->setLordId(proto.lordskillid());
    officerCard->setIsProt(proto.isprotection());
//    officerCard->setIsExped(proto.isexpedition());
    officerCard->setCurrLevel(proto.currlevel());
    
    //武将攻防血速差异性，这几个属性改由服务器发送
    officerCard->setRealAtk(proto.atk());
    officerCard->setRealDef(proto.def());
    officerCard->setRealMorale(proto.hp());
    officerCard->setRealSpeed(proto.sp());
    
    if (temp->getOfficerNextId()) {
        officerCard->setCost(temp->getOfficerCost());
        
//        CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//        int len = arr->count();
//        for (int j=0; j < len; j++) {
//            officerCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//        }
    }
    officerCard->setExpRate(temp->getOfficerExpRate());
    officerCard->setMaxStar(temp->getOfficerNextId());
    if (expdata != NULL) {
        officerCard->setMaxExp(expdata->getExpValue());
    }
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
    //转生等级
    officerCard->setUpgradelevel(temp->getUpgradelevel());
    //转生最大星级
    officerCard->setUpgradestar(temp->getUpgradestar());
    //武将计最大等级
    officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
    //主将计最大等级
    officerCard->setLordMaxLevel(temp->getLordMaxLevel());
    //性别和缘分
    officerCard->setGender((OfficerGender) temp->getOfficerGender());
    officerCard->setFateList(temp->getOfficerFateList());
    //转生次数
    officerCard->setAdNum(temp->getAdvNum());
    //武将原型ID
    officerCard->setProtoId(temp->getOfficerProtoId());
    //速度修正系数
    officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
	
	officerCard->setHeadSuffixNum(temp->getIconId());//12.05

    if (strcmp(temp->getOfficerGeneralsSkill()->getCString(), "0") &&temp->getOfficerGeneralsSkill()->length()>3)
    {
        std::string aaa(temp->getOfficerGeneralsSkill()->getCString(),3,temp->getOfficerGeneralsSkill()->length() - 3);
        
        std::string generalskill(temp->getOfficerGeneralsSkill()->getCString(),3,temp->getOfficerGeneralsSkill()->length() - 3);
        officerCard->setOfficerSkil(atoi(generalskill.c_str()));
    }
    if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
    {
        std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
        officerCard->setLordSkill(atoi(lordskill.c_str()));
    }
    
    return officerCard;
}

// 装备卡信息设置
SGBaseMilitaryCard* createEquipCardWithInfo(main::EquipmentCardItemProto cardInfo)
{
    CCLog("2m-----------------%d", cardInfo.ssid());
    SGEquipCard *equipCard = new SGEquipCard();
    equipCard->autorelease();
    CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
    SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardInfo.itemid());
    int type = SGStaticDataManager::shareStatic()->getStrengEquipType(cardInfo.itemid());
    
//    if (temp->getEquipExpType()->isEqual(CCString::create("A"))) {
//        type = 0;
//    }
//    else if (temp->getEquipExpType()->isEqual(CCString::create("B"))) {
//        type = 1;
//    }
//    else {
//        type = 2;
//    }
    
    SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel()+1);
    
    equipCard->setSsid(cardInfo.ssid());
    equipCard->setItemId(cardInfo.itemid());
    //装备原型ID
    equipCard->setProtoId(temp->getEquipProtoId());
    //            equipCard->setSetNum(cardInfo.setnum());
    equipCard->setCurrLevel(cardInfo.currlevel());
    //equipCard->setCurrExp(cardInfo.currexp());
    equipCard->setOfficerCardId(cardInfo.officercarditemid());
    equipCard->setIsProt(cardInfo.isprotection());
    if (cardInfo.officercarditemid() >0)
    {
        SGOfficerCard *officer = SGPlayerInfo::sharePlayerInfo()->getOfficerCard(cardInfo.officercarditemid());
        officer->addEquip(equipCard);
    }
    //转生等级
    equipCard->setUpgradelevel(temp->getUpgradelevel());
    //转生最大星级
    equipCard->setUpgradestar(temp->getUpgradestar());
    //转生次数
    equipCard->setAdNum(temp->getAdvNum());
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
        
//        CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//        int len = arr->count();
//        for (int j=0; j < len; j++) {
//            equipCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//        }
    }
    
    return equipCard;
}

// 道具卡信息设置
SGBaseMilitaryCard* createPropCardWithInfo(main::PropsCardItemProto cardInfo)
{
    SGPropsCard *propCard = new SGPropsCard();
    propCard->autorelease();
    
    SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardInfo.itemid());//plist索引从1开始
    
    propCard->setSsid(cardInfo.ssid());
    propCard->setItemId(cardInfo.itemid());
    propCard->setIsProt(cardInfo.isprotection());
    
    propCard->setSell(prop->getpropsSell());
    propCard->setValue(prop->getpropsValue());
    propCard->setRace(prop->getPropsCountry());
    propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
    propCard->setType(prop->getPropsPropsType());
    propCard->setItemType(prop->getPropsType());
    propCard->setCurrStar(prop->getPropsCurrStarLevel());
    propCard->setHeadSuffixNum(prop->getIconId());//12.05
    
    return propCard;
}
//碎片卡信息设置
SGBaseMilitaryCard *createPiecesCardWithInfo(main::OfficerFragmentCardItemProto cardInfo)
{
	SGPiecesCard *piecesCard = new SGPiecesCard();
	piecesCard->autorelease();
	
	SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(cardInfo.itemid());
	
	piecesCard->setSsid(cardInfo.ssid());
	piecesCard->setItemId(cardInfo.itemid());
	piecesCard->setPiecesCurNum(cardInfo.curritemnumber());
	
	piecesCard->setSell(pieces->getPiecesPrice());//价格
	piecesCard->setOfficerName(pieces->getPiecesName());//名称
	piecesCard->setPiecesName(pieces->getPiecesName());
	piecesCard->setItemType(pieces->getPiecesType());//类型
	piecesCard->setCurrLevel(pieces->getStarLevel());//星级
	piecesCard->setPiecesDesc(pieces->getPiecesDesc());//碎片描述
	piecesCard->setCanUseCommon(pieces->getCommonPieces());//可使用的通用碎片数量
	piecesCard->setCallCardId(pieces->getCallingCardId());//设置碎片合成后召唤的武将id
	piecesCard->setMaxNum(pieces->getMaxPiecesNum());//碎片最大数量
	piecesCard->setSummonNeed(pieces->getcallNeedPieces());//召唤所需要的碎片的数量
    piecesCard->setCurrStar(pieces->getStarLevel());

	return piecesCard;
}

//构建掉落消耗品信息
SGBaseMilitaryCard *createConsumeCardWithInfo(main::ConsumableCardItemProto cardInfo)
{
	SGConsumableCard *consumeCard = new SGConsumableCard();
	consumeCard->autorelease();
	
	SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(cardInfo.itemid());
	
	consumeCard->setCurrNum(cardInfo.curritemnumber());
	consumeCard->setSsid(cardInfo.ssid());
	consumeCard->setItemId(cardInfo.itemid());
	
	consumeCard->setSell(consumeModel->getConsumePrice());
	consumeCard->setconsumableDesc(consumeModel->getConsumeDesc());
	consumeCard->setCanUseDriving(consumeModel->getCanUseDriving());
	consumeCard->setCurrStar(consumeModel->getConsumeStarLvl());
	consumeCard->setOfficerName(consumeModel->getConsumeName());
	consumeCard->setMoneyType(consumeModel->getMoneyType());
	consumeCard->setItemType(consumeModel->getConsumeType());
	consumeCard->setHeadSuffixNum(consumeModel->getIconId());
	consumeCard->setLimitAndNum(consumeModel->getLimitAndNum());
	consumeCard->setPicId(consumeModel->getConsumePicId());
	
	return consumeCard;
}

//构建掉落材料信息
SGBaseMilitaryCard *createMaterialCardWithInfo(main::MaterialItemProto cardInfo)
{
	SGMaterialCard *materialCard = new SGMaterialCard();
	materialCard->autorelease();
	
	SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(cardInfo.itemid());
	
    materialCard->setSsid(cardInfo.ssid());
    materialCard->setMaterialCurrNum(cardInfo.itemnum());
    materialCard->setItemId(cardInfo.itemid());
    
    materialCard->setHeadSuffixNum(materialModel->getMaterialIconId());
    materialCard->setMaterialBorderColor(materialModel->getMaterialBorderColor());
    materialCard->setMaterialPileLimit(materialModel->getMaterialPileLimit());
    materialCard->setOfficerName(materialModel->getMaterialName());
    materialCard->setSell(materialModel->getMaterialSoldPrice());
	
	return materialCard;
}



SGBaseMilitaryCard *createEmbattleSoldierCardWithInfo(main::SoldierCardItemProto cardInfo)
{
   
    SGSoldierCard *soldierCard = new SGSoldierCard();
    CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
    SGSoldierDataModel *temp = SGStaticDataManager::shareStatic()->getSoliderById(cardInfo.itemid());
    SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(EXPTYPE_Z, cardInfo.currlevel()+1);
    
    soldierCard->setMaxLevel(temp->getSoldierMaxLevel());
    soldierCard->setSsid(cardInfo.ssid());
    soldierCard->setItemId(cardInfo.itemid());
    soldierCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
    soldierCard->setMaxExp(expdata->getExpValue());
    soldierCard->setOfficerName(temp->getSoldierName());
    soldierCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
    soldierCard->setRound(temp->getSoldierRound());
    soldierCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
    soldierCard->setCurrLevel(cardInfo.currlevel());
    soldierCard->setCurrStar(temp->getSoldierStarLevel());
    soldierCard->setCurrExp(cardInfo.currexp());
    //            soldierCard->setItemType(proto.itemtype());
    soldierCard->setsoldiertalentskill(temp->getTalentSkill());//士兵增加
    soldierCard->setsoldiergrowskill(temp->getGrowSkill());
    
    return soldierCard;





}


















SGBaseMilitaryCard* getCardWithIdAndType(int itemId, int type)
{
    // 0 officer 1 equip 2 soldier 3 prop
    
    SGBaseMilitaryCard *card = NULL;
    SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
    DDLog("item id = %d--------------type = %d", itemId, type);
    if (type == 6) {
        SGOfficerDataModel *oCard = manager->getOfficerById(itemId);
        SGOfficerCard *officerCard = new SGOfficerCard;
       
        officerCard->setOfficerName(oCard->getOfficerName());
        officerCard->setMaxLevel(oCard->getOfficerMaxLevel());
        officerCard->setCurrExp(0);
        officerCard->setCurrStar(oCard->getOfficerCurrStarLevel());
        officerCard->setIsProt(0);
//        officerCard->setIsExped(0);
        officerCard->setRace(oCard->getOfficerRace());
        officerCard->setItemId(oCard->getOfficerId());
        
        if (oCard->getOfficerGeneralsSkill()&&!oCard->getOfficerGeneralsSkill()->isEqual(CCString::create(""))) {
            officerCard->setOfficerSkil(manager->getAnyoneIdByBeanId(oCard->getOfficerGeneralsSkill()->getCString()));
        }
        if (oCard->getOfficerLordSkill() &&!oCard->getOfficerLordSkill()->isEqual(CCString::create(""))) {
            int lordskill = manager->getAnyoneIdByBeanId(oCard->getOfficerLordSkill()->getCString());
            officerCard->setLordSkill(lordskill);
        }
        
        officerCard->setAtk(oCard->getOfficerBaseAtk());
        officerCard->setDef(oCard->getOfficerBaseDef());
        officerCard->setRound(oCard->getOfficerRound());
        officerCard->setSpeed(oCard->getOfficerBaseSpeed());
        officerCard->setMorale(oCard->getOfficerMorale());
        officerCard->setGoverning(oCard->getOfficerGoverning());
        //转生等级
        officerCard->setUpgradelevel(oCard->getUpgradelevel());
        //转生最大星级
        officerCard->setUpgradestar(oCard->getUpgradestar());
        //武将计最大等级
        officerCard->setSkillMaxLevel(oCard->getSkillMaxLevel());
        //主将计最大等级
        officerCard->setLordMaxLevel(oCard->getLordMaxLevel());
        //性别和缘分
        officerCard->setGender((OfficerGender) oCard->getOfficerGender());
        officerCard->setFateList(oCard->getOfficerFateList());
        //武将原型ID
        officerCard->setProtoId(oCard->getOfficerProtoId());
        //速度修正系数
        officerCard->setSpeedFactor(oCard->getOfficerSpeedFactor());
		//转生次数
        officerCard->setAdNum(oCard->getAdvNum());
		officerCard->setHeadSuffixNum(oCard->getIconId());//12.05
        card = officerCard;
    }
    if (type == 7) {
        SGEquipCard *equipCard = new SGEquipCard;
        SGEquipmentDataModel *eCard = manager->getEquipById(itemId);
        //装备原型ID
        equipCard->setProtoId(eCard->getEquipProtoId());
        equipCard->setCurrStar(eCard->getEquipCurrStarLevel());
        equipCard->setCurrExp(0);
        equipCard->setOfficerCardId(0);
        equipCard->setItemType(eCard->getEquipType());
        equipCard->setIsProt(0);
        equipCard->setOfficerName(CCString::create(eCard->getEquipName()->getCString()));
        equipCard->setItemId(itemId);
        equipCard->setMorale(eCard->getEquipHP());
        equipCard->setAtk(eCard->getEquipmentBaseAtk());
        equipCard->setDef(eCard->getEquipBaseDef());
        equipCard->setRound(eCard->getEquipRound());
        equipCard->setSpeed(eCard->getEquipBaseSpeed());
        equipCard->setGoverning(eCard->getEquipGoverning());
        equipCard->setMaxLevel(eCard->getEquipMaxLevel());
        //转生等级
        equipCard->setUpgradelevel(eCard->getUpgradelevel());
        //转生次数
        equipCard->setAdNum(eCard->getAdvNum());
        //转生最大星级
        equipCard->setUpgradestar(eCard->getUpgradestar());
		
		equipCard->setHeadSuffixNum(eCard->getIconId());//12.06
		
        equipCard->autorelease();
        card = equipCard;
    }
    if (type == 9) {
        SGPropsDataModel *prop = manager->getPropsById(itemId);
        SGPropsCard *propCard = new SGPropsCard;
        propCard->setType(prop->getPropsPropsType());
        propCard->setItemType(prop->getPropsType());
        propCard->setIsProt(0);
        propCard->setCurrStar(prop->getPropsCurrStarLevel());
        propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
        propCard->setItemId(prop->getPropsId());
		propCard->setHeadSuffixNum(prop->getIconId());//12.05
		
        propCard->autorelease();
        card = propCard;
    }
    if(type == 10)
    {
        SGPiecesCard *piecesCard = new SGPiecesCard();
        SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(itemId);
        piecesCard->setItemId(itemId);//碎片id
        piecesCard->setSell(pieces->getPiecesPrice());//价格
        piecesCard->setOfficerName(pieces->getPiecesName());//名称
        piecesCard->setPiecesName(pieces->getPiecesName());
        piecesCard->setItemType(pieces->getPiecesType());//类型
        piecesCard->setCurrLevel(pieces->getStarLevel());//星级
        piecesCard->setPiecesDesc(pieces->getPiecesDesc());//碎片描述
        piecesCard->setCanUseCommon(pieces->getCommonPieces());//可使用的通用碎片数量
        piecesCard->setCallCardId(pieces->getCallingCardId());//设置碎片合成后召唤的武将id
        piecesCard->setMaxNum(pieces->getMaxPiecesNum());//碎片最大数量
        piecesCard->setSummonNeed(pieces->getcallNeedPieces());//召唤所需要的碎片的数量
        piecesCard->setCurrStar(pieces->getStarLevel());

        card = piecesCard;
        piecesCard->autorelease();


    }
    if (type == 15)
    {
        SGConsumableCard *consumeCard = new SGConsumableCard();
        SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(itemId);
        consumeCard->setItemId(itemId);
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
        card = consumeCard;
        consumeCard->autorelease();
        

    }
    
    return card;
}

// draw stars
CCSprite *spriteWithStars(int stars)
{


    CCSprite *spriteStar = CCSprite::create();
    spriteStar->setTextureRect(CCRectMake(0, 0, skewing(15), skewing(15)));
    spriteStar->setOpacity(0);
    for (int ii = 0; ii < MAXSTAR; ii++) {
        CCSprite *sprite = CCSprite::createWithSpriteFrameName("xx.png");
        float sWidth = sprite->getContentSize().width;
        float sHeigth = sprite->getContentSize().height;
        sprite->setPosition(ccp((sWidth-skewing(3))*ii + sWidth/2-skewing(3), sHeigth/2-skewing(3)));
        spriteStar->addChild(sprite,1,ii);
        if (ii >= stars) {
            sprite->setVisible(false);
        }
    }
    return spriteStar;
}

// -------------全局函数---------------
// ---------------end-----------------
