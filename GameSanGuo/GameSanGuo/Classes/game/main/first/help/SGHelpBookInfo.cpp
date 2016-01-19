//
//  SGHelpBookInfo.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-8-7.
//
//

#include "SGHelpBookInfo.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#include "SGBattleMainLayer.h"
#include "SGBattleData.h"
#include "PvpFight.pb.h"
#include "SGStringConfig.h"
SGHelpBookInfo* SGHelpBookInfo::create(SGHelpTitleDataModel *mode,int entertype)
{
    SGHelpBookInfo *info = new SGHelpBookInfo();
    
    if (info&&info->init(NULL, sg_helpinfolayer)) {
        info->type = entertype;
        info->initview(mode);
        info->autorelease();
        return info;
    }
    CC_SAFE_DELETE(info);
    return NULL;
}

SGHelpBookInfo::SGHelpBookInfo()
:type(0),
scrollView(NULL)
{
    
}

SGHelpBookInfo::~SGHelpBookInfo()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_helpinfolayer);
}

void SGHelpBookInfo::initview(SGHelpTitleDataModel *mode)
{
    
    ResourceManager::sharedInstance()->bindTexture("sgchat/sgchat.plist", RES_TYPE_LAYER_UI, sg_helpBook);

    CCArray *arr = SGStaticDataManager::shareStatic()->gethelpinfo(mode->getbeg(), mode->getend());
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    
    //标题栏
    CCSprite *spriteTopBG = CCSprite::createWithSpriteFrameName("chattitlebg.png");
    
    SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGHelpBookInfo::buttonClickGoback),CCPointZero,FONT_PANGWA,ccWHITE,36,false,true);
    buttonBack->setScale(1.05);
	SGCCLabelTTF *spriteTitle = SGCCLabelTTF::create(mode->gettitle()->getCString(), FONT_PANGWA, 36);
    spriteTopBG->setAnchorPoint(ccp(0.5, 1));
    buttonBack->setAnchorPoint(ccp(0,0.5));
    spriteTopBG->setPosition(ccpAdd(upCenter, ccp(0, -spriteTopBG->getContentSize().height+3)));
    buttonBack->setPosition(ccp(1,spriteTopBG->getPosition().y - spriteTopBG->getContentSize().height*.5));
    spriteTitle->setPosition(ccpAdd(spriteTopBG->getPosition(), ccp(0, -spriteTopBG->getContentSize().height/2)));
    addChild(spriteTopBG);
    addBtn(buttonBack);
    addChild(spriteTitle);
    
    //星形背景 -79-150-72-56-101-30
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_helpBook);
    
    //CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_helpBook);
    CCSprite *bg = CCSprite::createWithSpriteFrameName("storyBg.png");
    //CCRect r = CCRectMake(0, 30,bg->getContentSize().width,s.height - spriteTopBG->getContentSize().height - 113);
    //bg->setTextureRect(r);
    //bg->setScaleY(s.height/2 / bg->getContentSize().height);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setScaleY( 1.0f * (spriteTopBG->getPositionY() - 113) / bg->getContentSize().height);
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,113)));
    
    int hgt = 0;
    //滑动
    scrollView = CCScrollView::create(CCSizeMake(SGLayout::getSkewing(320), SGLayout::getSkewingY(480)-112-96)*.95);
    //初始化ScrollView的container
    CCLayer* scrollViewContainer = CCLayer::create();
    
    for (int i = arr->count()-1; i>=0; i--)
    {
        SGHelpTitleDataModel *modle = (SGHelpTitleDataModel*)arr->objectAtIndex(i);
        if (modle->getend() == 1 || modle->getend() == 4)
        {
            SGCCLabelTTF * lab = SGCCLabelTTF::create(modle->gettitle()->getCString(), FONT_PENGYOU, 30, CCSize(540, 0));
            lab->setAnchorPoint(ccp(0.5f, 0));
            if (modle->getend() == 4) {
                lab->setInsideColor(COLOR_YELLOW);
            }
            scrollViewContainer->addChild(lab,10);
            lab->setPosition(ccp(skewing(160), hgt));
            hgt+= lab->getContentSize().height;
        }
        else if (modle->getend() == 2)
        {
            //plist
            int pos = modle->gettitle()->m_sString.find(".");
            std::string realName = modle->gettitle()->m_sString.substr(0, pos);
            char buffer[100];
            //sprintf(buffer, "sghelplayer/singlepicture/%s.plist", realName.c_str());  //cgp_png
            sprintf(buffer, "sghelplayer/%s.plist", realName.c_str());
            ResourceManager::sharedInstance()->bindTexture(buffer, RES_TYPE_LAYER_UI, sg_helpinfolayer);
            
            //png
            CCSprite *a = CCSprite::createWithSpriteFrameName(modle->gettitle()->getCString());
            a->setAnchorPoint(ccp(0.5f, 0));
            scrollViewContainer->addChild(a);
            a->setPosition(ccp(skewing(160), hgt));
            hgt += a->getContentSize().height;
            
        }else
        {
            hgt+= 30;
        }
    }
    //把layer设置为container
    scrollView->setContainer(scrollViewContainer);
    //设置container的contentSize
    scrollView->setDirection(kCCScrollViewDirectionVertical);
    scrollViewContainer->setContentSize(CCSizeMake(skewing(320), hgt*.95));
    
    scrollView->setContentSize(CCSizeMake(skewing(320), hgt));
    
    scrollView->setContentOffset(ccp(0,-(hgt  - (skewingY(480) - 112-96-50))));
    //设置scrollView的位置
    scrollView->setPosition(ccp(0, 130));
    scrollView->unscheduleAllSelectors();
    this->addChild(scrollView);
}
void SGHelpBookInfo::buttonClickGoback()
{
    if (type == 2)
    {
        SGMainManager::shareMain()->showPvpMatchLayer();
    }
    else if (type==3)
    {
        SGMainManager::shareMain()->showshoujingzhou();//////1114
    }
    else if(type == 4)//武将培养  之前的页面切记不能删除
    {
        SGMainScene * scene = SGMainManager::shareMain()->getMainScene();
        scene->addShowLayer(dynamic_cast<SGBaseLayer*>(scene->getChildByTag(sg_developingOfficerLayer)) , true);
    }
    else if(type == 5)//竞技场
    {
        SGMainManager::shareMain()->goToArenaLayerRequest();
    }
    else
    {
    SGMainManager::shareMain()->showHelpBook();
    }
}


void SGHelpBookInfo::showSeasonMainLayer(CCObject *sender)
{
//    SGSocketRequest *request = (SGSocketRequest *)sender;
//    main::PvpEntryResponse *response = (main::PvpEntryResponse *)request->m_msg;
//    SGBattleData *battleData = NULL;
//    SGRankPlayer *selfPlayer = NULL;
//    if (response) {
//        int var1 = response->isopen();
//        int var2 = response->toppos();
//        int var3 = response->toplevel();
//        int var4 = response->fighttimes();
//        int var5 = response->score();
//        int var6 = response->joinmax();
//        int var7 = response->jointoday();
//        
//        std::string str = response->seasontime();
//        float winr = response->winratio();
//        
//        CCString *name = SGPlayerInfo::sharePlayerInfo()->getNickName();
//        int iconId = SGPlayerInfo::sharePlayerInfo()->getPlayerImageID();
//        // create(const char *name, int hid, int rank, int levl, int ftime, float rwin)
//        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, var2, var3, var4, winr);//不知道要那几个？？？
//        battleData = SGBattleData::create(str.c_str(), var1, var5, var7, var6);
//        battleData->setPlayer(selfPlayer);
//        
//        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, var2, var3, var4, winr);
//        if (selfPlayer) {
//            SGPlayerInfo::sharePlayerInfo()->setPvpRank(selfPlayer);
//            SGPlayerInfo::sharePlayerInfo()->setplayerPvpLevel(selfPlayer->getLevel());
//            SGPlayerInfo::sharePlayerInfo()->setplayerPvpNum(response->score());
//        }
//    }
//    if (battleData != NULL) {
//        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
//        SGBattleMainLayer *layer = (SGBattleMainLayer *)mainScene->getChildByTag(sg_battleMainLayer);
//        if (!layer) {
//            layer = SGBattleMainLayer::create(battleData);
//        }
//        
//        mainScene->setVisible(true);
//        mainScene->addShowLayer(layer);
//    }
}
