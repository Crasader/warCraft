//
//  SGLevelUpLayer.cpp
//  GameSanGuo
//
//  Created by lizi on 13-5-22.
//
//

#include "SGLevelUpLayer.h"
#include "SGMainLayer.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGFriend.h"
#include "SGGeneralBox.h"
#include "SGStoryRewardBox.h"
#include "ResourceManager.h"
#include "PlotModule.pb.h"
#include "SGGuideManager.h"
#include "AppMacros.h"
#include "SGStringConfig.h"
#include "SGFightRewardLayer.h"

SGLevelUpLayer::SGLevelUpLayer(void)
:_type(0),
_index(0),
isFirstTouch(true),
bgLevel(0),
eff(NULL),
frame(NULL),
spCon(NULL)
{

}
SGLevelUpLayer::~SGLevelUpLayer(void)
{
    CCLOG("~SGLevelUpLayer");
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_levelUpLayer);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_FRIEND_PLAYERINFO);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CREAM_STORY);
}

SGLevelUpLayer* SGLevelUpLayer::create()
{
    SGLevelUpLayer *box = new SGLevelUpLayer();
    if (box && box->init(NULL, sg_levelUpLayer)) {
        box->initView();
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

void SGLevelUpLayer::initView()
{
    
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_levelUpLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_levelUpLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer3.plist", RES_TYPE_LAYER_UI, sg_levelUpLayer);
    
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_FRIEND_PLAYERINFO, this,
																  callfuncO_selector(SGLevelUpLayer::getFriendOfficerListener));
	
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_CREAM_STORY, this, callfuncO_selector(SGLevelUpLayer::showCreamSceneLayer));
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    //CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    
    int currLev = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    SGPlayerDataModel *playPreInfo = SGStaticDataManager::shareStatic()->getPlayerByLevel(HelpFriendPlayLev);
    SGPlayerDataModel *playCurrInfo = SGStaticDataManager::shareStatic()->getPlayerByLevel(currLev);
    
     ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_levelUpLayer);
    ResourceManager::sharedInstance()->bindTexture("battle/battle_win_eff.plist", RES_TYPE_LAYER_UI, sg_levelUpLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/uplevel_mm_n.plist", RES_TYPE_LAYER_UI, sg_levelUpLayer);

    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("storyBg.png");
    
    // init the layer background
    CCRect rect = CCRectMake(0, 0,bgImg->getContentSize().width, bgImg->getContentSize().height);
    bgImg->setTextureRect(rect);

    bgImg->cocos2d::CCNode::setScaleX(size.width / bgImg->getContentSize().width * 1.01);
    bgImg->setScaleY(size.height / bgImg->getContentSize().height * 1.01);
    bgImg->setPosition(SGLayout::getPoint(kMiddleCenter));

    this->addChild(bgImg, -5);
    
    CCSprite *bgColorImg = CCSprite::createWithSpriteFrameName("uplevel_colorbg_n.png");
    bgColorImg->setPosition(ccpAdd(upCenter, ccp(0, -bgColorImg->getContentSize().height*.5f)));
    this->addChild(bgColorImg, -4);
    
	//升级了 图
    bgLevel = CCSprite::createWithSpriteFrameName("uplevel_clevel_n.png");
    bgLevel->setPosition(ccpAdd(upCenter, ccp(0, -bgLevel->getContentSize().height*.15f)));
	bgLevel->setVisible(false);
    this->addChild(bgLevel, -1);
	
	//光圈特效
	eff = CCSprite::createWithSpriteFrameName("battle_win_eff.png");
	eff->setPosition(bgLevel->getPosition());
	eff->setVisible(false);
	this->addChild(eff, -4);
	
    
    // init the handsome girl
    CCSprite *spMM = CCSprite::createWithSpriteFrameName("uplevel_mm_n.png");
    spMM->setPosition(ccp(size.width*.5f, size.height-spMM->getContentSize().height*.5f-175));
    spMM->setVisible(false);
    this->addChild(spMM, -2);
    
    // init the level rect
    frame = CCScale9Sprite::createWithSpriteFrameName("uplevel_fontbg_n.png");
    frame->setOpacity(235);
	frame->setVisible(false);
    this->addChild(frame, -1);
    frame->setPosition(ccpAdd(upCenter, ccp(0, -frame->getContentSize().height*.5f-475)));

	
	int left_shift = 0;
	int up_shift = 0;
	//ipad适配
	if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width || CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
	{
		left_shift = -70;
		up_shift = 115;
	}
	else if (CCEGLView::sharedOpenGLView()->getFrameSize().height == 960 && CCEGLView::sharedOpenGLView()->getFrameSize().width == 640)//iphone4 或者 iphone4s
	{
		up_shift = 180;
	}
    
    // 上框线框
    float posX = frame->getPositionX()-frame->getContentSize().width*.51f;
    float posY = frame->getPositionY()+frame->getContentSize().height*.53f;
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setFlipY(true);
    titlecenter->setAnchorPoint(ccp(0.5f, 1));
    titlecenter->setPosition(ccp(frame->getPositionX() + left_shift, frame->getPositionY() - 100 + up_shift));
    frame->addChild(titlecenter);
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setFlipY(true);
    title_bg_l->setAnchorPoint(ccp(0, 1));
    title_bg_l->setPosition(ccp(posX - 30 + left_shift, frame->getPositionY() - 100 + up_shift));
    frame->addChild(title_bg_l);
    posX = frame->getPositionX()+frame->getContentSize().width*.51f;
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setFlipY(true);
    title_bg_r->setPosition(ccp(posX - 30 + left_shift, frame->getPositionY() - 100 + up_shift));
    title_bg_r->setAnchorPoint(ccp(1, 1));
    frame->addChild(title_bg_r);
    
    // 下框线框
    posX = frame->getPositionX()-frame->getContentSize().width*.51f;
    posY = frame->getPositionY()-frame->getContentSize().height*.53f;
    titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5f, 0));
    titlecenter->setPosition(ccp(frame->getPositionX() + left_shift, frame->getPositionY() - frame->getContentSize().height - 120 + up_shift));
    frame->addChild(titlecenter);
    title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccp(posX - 30 + left_shift, frame->getPositionY() - frame->getContentSize().height - 120 + up_shift));
    frame->addChild(title_bg_l);
    posX = frame->getPositionX()+frame->getContentSize().width*.51f;
    title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
	title_bg_r->setAnchorPoint(ccp(1, 0));
	title_bg_r->setPosition(ccp(posX - 30 + left_shift, frame->getPositionY() - frame->getContentSize().height - 120 + up_shift));
    frame->addChild(title_bg_r);
    
    // 升级信息
    CCSprite *spLevelFont = CCSprite::createWithSpriteFrameName("uplevel_flevel_n.png");
    spLevelFont->setAnchorPoint(ccp(0, 0.5f));
    spLevelFont->setPosition(ccpAdd(frame->getPosition(), ccp(-240 + left_shift, -170 + up_shift)));
    frame->addChild(spLevelFont);
    
    const char* levCh = CCString::createWithFormat("%d", HelpFriendPlayLev)->getCString();
    CCLabelAtlas *labelLeft = CCLabelAtlas::create(levCh, "sanguobigpic/uplevel_lnum_n.png", 45, 48, '0');
    labelLeft->setAnchorPoint(ccp(0, 0.5f));
    labelLeft->setPosition(ccpAdd(spLevelFont->getPosition(), ccp(spLevelFont->getContentSize().width, 0)));
    frame->addChild(labelLeft);
	
    CCSprite *labelTag = CCSprite::createWithSpriteFrameName("uplevel_btip.png");
    labelTag->setAnchorPoint(ccp(0, 0.5f));
    labelTag->setPosition(ccpAdd(labelLeft->getPosition(), ccp(labelLeft->getContentSize().width, 0)));
    frame->addChild(labelTag);
    levCh = CCString::createWithFormat("%d", currLev)->getCString();
    CCLabelAtlas *labelRight = CCLabelAtlas::create(levCh, "sanguobigpic/uplevel_rnum_n.png", 62, 67, '0');
    labelRight->setAnchorPoint(ccp(0, 0.5f));
    labelRight->setPosition(ccpAdd(labelTag->getPosition(), ccp(labelTag->getContentSize().width, 0)));
    frame->addChild(labelRight);
    
    CCSprite *slineU = CCSprite::createWithSpriteFrameName("uplevel_fontline_n.png");
    slineU->setPosition(ccpAdd(frame->getPosition(), ccp(left_shift, -210 + up_shift)));
    frame->addChild(slineU);
    CCSprite *slineD = CCSprite::createWithSpriteFrameName("uplevel_fontline_n.png");
    slineD->setPosition(ccpAdd(frame->getPosition(), ccp(left_shift, -390 + up_shift)));
    frame->addChild(slineD);
    ////////////统御力加成
    
    int addgoven=SGPlayerInfo::sharePlayerInfo()->getAddPlayerGovering();
    ////////////
    // old data
//    int iVar1 = playPreInfo->getPlayerBaseFriendSize();
    int iVar2 = playPreInfo->getPlayerOfficerMax();
    int iVar3 = playPreInfo->getPlayerPower();
    int iVar4 = playPreInfo->getPlayerGoverning()+addgoven;
    // currlev data
//    int iVar5 = playCurrInfo->getPlayerBaseFriendSize();
    int iVar6 = playCurrInfo->getPlayerOfficerMax();
    int iVar7 = playCurrInfo->getPlayerPower();
    int iVar8 = playCurrInfo->getPlayerGoverning()+addgoven;
    // 文字Label
    int culev[3] = {iVar2, iVar3, iVar4};
    int uplev[3] = {iVar6, iVar7, iVar8};
    const char *desc[4] = {str_GeneralBox_str7, str_GeneralBox_str8, str_GeneralBox_str9};
    float fontSize = FONTSIZE(16);
    const char* fontName = FONT_BOXINFO;
    float discX = frame->getContentSize().width;
    for (int ii = 0; ii < 3; ii++) {
        SGCCLabelTTF *label = SGCCLabelTTF::create(desc[ii], fontName, fontSize);
        label->setAnchorPoint(ccp(0, 0.5f));
        label->setPosition(ccpAdd(frame->getPosition(), ccp(-frame->getContentSize().width*.37f - 20 + left_shift, -230-43.75f*ii + up_shift - 20)));
        frame->addChild(label);
        
        const char *cnum = CCString::createWithFormat("%d", culev[ii])->getCString();
        SGCCLabelTTF *levC = SGCCLabelTTF::create(cnum, fontName, fontSize);
        levC->setAnchorPoint(ccp(1, 0.5f));
        levC->setPosition(ccpAdd(label->getPosition(), ccp(discX*.43f, 0)));
        frame->addChild(levC);
        
        cnum = CCString::createWithFormat("%d", uplev[ii])->getCString();
        SGCCLabelTTF *levL = SGCCLabelTTF::create(cnum, fontName, fontSize);
        levL->setAnchorPoint(ccp(1, 0.5f));
        levL->setPosition(ccpAdd(label->getPosition(), ccp(discX*.7f, 0)));
        frame->addChild(levL);
        if (culev[ii] < uplev[ii]) levL->setInsideColor(ccGREEN);
        
        CCSprite *sline = CCSprite::createWithSpriteFrameName("uplevel_stip.png");
        sline->setPosition(ccpAdd(levC->getPosition(), ccp(discX*.11f, 0)));
        frame->addChild(sline);
    }
    
    // 体力回满提示
    CCSprite *spPower = CCSprite::createWithSpriteFrameName("uplevel_powerbg_n.png");
    spPower->setPosition(ccpAdd(frame->getPosition(), ccp(-20 + left_shift, -437 + up_shift)));
    frame->addChild(spPower);
    
    SGCCLabelTTF *levL = SGCCLabelTTF::create(CCString::createWithFormat(str_GeneralBox_str10)->getCString(), fontName, fontSize);
    levL->setPosition(spPower->getPosition());
    frame->addChild(levL);
    
    CCLabelTTF *grantPower = CCLabelTTF::create(CCString::createWithFormat("%d",playCurrInfo->getGrantPower())->getCString(), fontName, fontSize);
    grantPower->setPosition(ccpAdd(levL->getPosition(), ccp(-levL->getContentSize().width/2 + fontSize * 6.8 , 4)) );
    frame->addChild(grantPower);
    grantPower->setColor(ccGREEN);
    
    spCon = CCSprite::createWithSpriteFrameName("uplevel_font_con.png");
    spCon->setPosition(ccpAdd(frame->getPosition(), ccp(0, -237)));
    this->addChild(spCon, 1, 99);
    spCon->setVisible(false);
    
    this->setTouchEnabled(false);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//招待页面动画
	//设置MM的位置为屏幕左侧
	spMM->setPosition(ccp(-500, size.height-spMM->getContentSize().height*.5f-175));
	CCMoveTo *mmMo = CCMoveTo::create(0.5, ccp(size.width*.5f, size.height-spMM->getContentSize().height*.5f-175));
	CCSequence *mmSeq = CCSequence::create(mmMo,CCCallFuncN::create(this, callfuncN_selector(SGLevelUpLayer::setLevel)), NULL);
	spMM->runAction(mmSeq);
	
	//升级了图开始缩放
	bgLevel->setScale(6.0);
	CCScaleTo *levelUpToNorm = CCScaleTo::create(0.8, 1.0);
	CCSequence *levelSeq = CCSequence::create(levelUpToNorm, CCCallFuncN::create(this, callfuncN_selector(SGLevelUpLayer::setEff)), CCDelayTime::create(0.2),CCCallFuncN::create(this, callfuncN_selector(SGLevelUpLayer::setFrame)),CCDelayTime::create(0.2),  NULL);
	bgLevel->runAction(levelSeq);
	
	//光圈旋转
	CCRotateBy *rota = CCRotateBy::create(5.6, 360);
	CCRepeatForever *rotaFor = CCRepeatForever::create(rota);
	eff->runAction(rotaFor);
	
	//详情左侧滑入
	frame->setPosition(ccpAdd(upCenter, ccp(-490, -frame->getContentSize().height*.5f-475)));
	CCMoveTo *frameMove = CCMoveTo::create(1.3, ccpAdd(upCenter, ccp(20, -frame->getContentSize().height*.5f-475)));
	CCMoveTo *frameMove1 = CCMoveTo::create(0.1, ccpAdd(upCenter, ccp(-15, -frame->getContentSize().height*.5f-475)));
	CCMoveTo *frameMove2 = CCMoveTo::create(0.1, ccpAdd(upCenter, ccp(10, -frame->getContentSize().height*.5f-475)));
	CCMoveTo *frameMove3 = CCMoveTo::create(0.06, ccpAdd(upCenter, ccp(0, -frame->getContentSize().height*.5f-475)));
	
	CCCallFunc *continuePress = CCCallFunc::create(this, callfunc_selector(SGLevelUpLayer::spConRunaction));
	CCSequence *frameSeq = CCSequence::create(frameMove,frameMove1, frameMove2, frameMove3, CCDelayTime::create(0.3),continuePress, NULL);
	frame->runAction(frameSeq);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
}
void SGLevelUpLayer::setLevel(CCNode *n)
{
	this->bgLevel->setVisible(true);
}
void SGLevelUpLayer::setEff(CCNode *n)
{
	this->eff->setVisible(true);
}
void SGLevelUpLayer::setFrame(CCNode *n)
{
	this->frame->setVisible(true);
}
//未播放完动画前，不允许点击跳过
void SGLevelUpLayer::spConRunaction(CCNode *n)
{
	this->setVisible(true);
	spCon->runAction(CCRepeatForever::create(CCBlink::create(1.0f, 2)));
	this->setTouchEnabled(true);
}

void SGLevelUpLayer::getFriendOfficerListener(CCObject *sender)
{
	SGMainManager *sGMainManager = SGMainManager::shareMain();
	SGSocketRequest *req = (SGSocketRequest *)sender;
	if (req)
	{
		main::FriendInfoResponse *friRes = (main::FriendInfoResponse *)req->m_msg;
		if (friRes)
		{
			SGGamePlayerInfo *playerInfo = SGFriend::getSGGamePlayerInfo(friRes->role());
            HelpFriendRoleName=playerInfo->getNickName()->getCString();
            HelpFriendItemLevel=playerInfo->getLevel();
            HelpFriendItemId=playerInfo->getLordid();
            HelpFriendItemLevel=playerInfo->getLevel();
			SGOfficerCard *officer = playerInfo->getLordOfficer();
			officer->retain();
			
			// 此处获得好友武将卡的信息
			SGGeneralBox *box = SGGeneralBox::create(NULL, 1, officer);
			box->setLdIndex(_index);
			sGMainManager->closeBox();
			sGMainManager->showBox(box);
		}
	}
}

void SGLevelUpLayer::confirmAction()
{
    HelpFriendPlayLev = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    //    DDLog("cc========================touch");
    if (SGFriend::isFriendOfPlayer(HelpFriendRoleId) != SGFRIEND_FRIEND && HelpFriendRoleId > 0) { //非好友，添加好友
		
		SGMainManager::shareMain()->getFriendOfficerInfo();//发服务器请求
		
//        SGGeneralBox *box = SGGeneralBox::create(NULL, 1);
//        box->setLdIndex(_index);
//        SGMainManager::shareMain()->closeBox();
//        SGMainManager::shareMain()->showBox(box);
        return;
    }
    if (_index >= 0) { // 开启炼丹炉
        SGGeneralBox *box = SGGeneralBox::create(NULL, 2);
        box->setLdIndex(_index);
        SGMainManager::shareMain()->closeBox();
        SGMainManager::shareMain()->showBox(box);
        return;
    }
    
//    SGStoryRewardBox::gobackToStoryLayer();
    SGFightRewardLayer::gobackToStoryLayer();
}

void SGLevelUpLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
}

bool SGLevelUpLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isFirstTouch == true) {
        isFirstTouch = false;
        return true;
    }
    return false;
}

void SGLevelUpLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCSprite *spCon = (CCSprite *)this->getChildByTag(99);
    spCon->stopAllActions();
    spCon->setVisible(true);
    
    if (HelpFriendPlayLev == 11) { // 升到10级提示领元宝和打PVP
        this->confirmAction();
    }else {
        this->confirmAction();
    }
}

void SGLevelUpLayer::showSceneLayer(CCObject *sender)
{
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId <= guide_tag_10 || guideId == guide_tag_30) return;
    
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PlotListResponse *response = (main::PlotListResponse *)req->m_msg;
    if (response) {
        int storyId = response->storyid();
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
        SGStoryBean *story = NULL;
        if (array == NULL) { //战斗中断
            story = SGStaticDataManager::shareStatic()->getStoryDataById(1);
        }else {
            for (int ii = 0; ii < array->count(); ii++) {
                story = (SGStoryBean *)array->objectAtIndex(ii);
                if (storyId == story->getStoryId()) {
                    break;
                }
            }
        }
        if (story ==NULL) {
            return;
        }
        story->setStoryStars(response->star());
        story->setMaxStar(response->maxstar());
        story->setCanReward(response->canreward());
        story->setRewardId(response->rewardid());
        if (response->state() == 0) {
            // story 场景为空，开启第一个场景
            int size = response->plotmodel_size();
            if (size > 0) {
                for (int ii = 0; ii < size; ii++) {
                    main::PlotModel model = response->plotmodel(ii);
                    SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                    if (scene == NULL) {
                        continue;
                    }else {
                        scene->setVisible(true);
                        scene->setRank(model.rank());
                        scene->setOpenState(model.openstate());
                        scene->setPower(model.power());
                        scene->setZhandou(model.zhandou());
                        scene->setExp(model.exp());
                        scene->setGold(model.gold());
                        int itemNum = model.itemids_size();
                        scene->itemIds.clear();
                        for (int jj =0;jj<itemNum; jj++) {
                            scene->itemIds.push_back(model.itemids(jj));
                        }
                    }
                }
            }
        }else if (response->state() == 1) {
            // 初始化 场景数值章节
            int size = response->plotmodel_size();
            if (size > 0) {
                int maxSceneId = 0;
                for (int ii = 0; ii < size; ii++) {
                    main::PlotModel model = response->plotmodel(ii);
                    
                    CCLOG("scene ID=================%d", model.plotid());
                    SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                    if (scene == NULL) {
                        continue;
                    }else {
                        if (maxSceneId < model.plotid()) {
                            maxSceneId = model.plotid();
                        }
                        scene->setVisible(true);
                        scene->setRank(model.rank());
                        scene->setOpenState(model.openstate());
                        scene->setPower(model.power());
                        scene->setZhandou(model.zhandou());
                        scene->setExp(model.exp());
                        scene->setGold(model.gold());
                        int itemNum = model.itemids_size();
                        scene->itemIds.clear();
                        for (int jj =0;jj<itemNum; jj++) {
                            scene->itemIds.push_back(model.itemids(jj));
                        }
                    }
                }
                
            }
        }
        else
        {
            return;
        }
        if (response->plotaskid() == 0 || story->getStoryId()==1 || story->getStoryId()==5) {
            SGMainManager::shareMain()->m_trackIsPve = true;
            SGMainManager::shareMain()->showStorySectionLayer(story);
            
            if (SGGuideManager::shareManager()->getLimitGuideSize() > 0)
            {
                SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
                SGBaseLayer *nowLayer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
                nowLayer->setIsCanTouch(false);
            }
            
        }else { // 有故事对话
            //SGStoryBean *bean = SGStaticDataManager::shareStatic()->getStoryDataById(1);
            SGStorySceneBean *scene = (SGStorySceneBean *)story->getStorySections()->objectAtIndex(0);
            SGGuideManager::shareManager()->showStorySpeaker(scene->getPlotId(), this, story);////序章问题
        }
    }
}


void SGLevelUpLayer::showCreamSceneLayer(CCObject * sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::BossPlotListResponse *response = (main::BossPlotListResponse *)req->m_msg;
    CCLOG("scene=================%d", response->bossplotmodel_size());
    if (response) {
        int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
        //精英副本
		
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
        for (int ii = 0; ii < array->count(); ii++) {
            SGStoryBean *story = (SGStoryBean *)array->objectAtIndex(ii);
            if (story->getSceneBeanById(plotId) != NULL) {
                
                int size = response->bossplotmodel_size();
                if (size > 0) {
                    for (int iii = 0; iii < size; iii++) {
                        main::BossPlotModel model = response->bossplotmodel(iii);
                        CCLOG("scene ID=================%d", model.plotid());
                        SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                        if (scene == NULL) {
                            continue;
                        }else {
                            scene->setVisible(true);
                            //scene->setRank(model.ra);
                            scene->setPower(model.power());
                            scene->setGold(model.gold());
                            scene->setExp(model.exp());
                            scene->setZhandou(model.zhandou());
                            scene->setRank(model.rank());
                            scene->setCreamCanJoin(model.canjoin());
                            
                            //数据存储在playerinfo中
                            SGPlayerInfo::sharePlayerInfo()->setCreamData(model.plotid(), model.canjoincount(), model.canjoin());

                            
                            scene->setCreamDayTimes(model.canjoincount());
                            scene->setCreamBuyTimes(model.canbuycount());
                            scene->setBuyPrice(model.price());
                            scene->setBuyCompTimes(model.buycount());// 已经购买次数
                            scene->setDropInfo(model.exp(), model.gold());
                            int itemNum = model.itemids_size();
                            scene->itemIds.clear();
                            for (int jj =0;jj<itemNum; jj++) {
                                scene->itemIds.push_back(model.itemids(jj));
                            }
                            
                        }
                        SGPlayerInfo::sharePlayerInfo()->setMaxBossPlotId(model.plotid());
                    }
                }
                SGMainManager::shareMain()->m_trackIsPve = false;
                SGMainManager::shareMain()->showStorySectionLayer(story);
                
                
                break;
            }
        }
        
        
    }
    
}