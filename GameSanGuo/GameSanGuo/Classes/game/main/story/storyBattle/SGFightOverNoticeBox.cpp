#include "SGFightOverNoticeBox.h"
#include "SGFightRewardLayer.h"
#include "SGMainManager.h"
#include "RewardModule.pb.h"
#include "SGFirstStrengBox.h"
#include "ShopModule.pb.h"
#include "SGStoryRewardBox.h"
#include <map>
using std::map;


SGFightOverNoticeBox::SGFightOverNoticeBox()
: m_boxState(-1)
, m_firstTouch(true)
{
    ;
}

SGFightOverNoticeBox::~SGFightOverNoticeBox()
{
    //rm msg
    SGNotificationCenter * notif = SGNotificationCenter::sharedNotificationCenter();
    notif->removeObserver(this, MSG_REWARD_LIANDANENTER);
    //rm resource
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_fightOverNoticeBox);
}

/*boxState: 1=战斗失败 2=战斗未三星 3=西域商人 4=炼丹秘境*/
//sg_fightOverNoticeBox 绑定资源的tag
//box_fightOverNoticeBox boxTag
//需要读取2个数据表 fightOverNotice.xls  fightOverNoticeBtn.xls
SGFightOverNoticeBox * SGFightOverNoticeBox::create(SGFightRewardLayer * dele, int boxState)
{
    SGFightOverNoticeBox * ret = new SGFightOverNoticeBox;
    if(ret && ret->init(dele, box_fightOverNoticeBox , CCSize(640 , 500) ,false , false) )
    {
        ret->autorelease();
        ret->m_delegate = dele;
        ret->m_boxState = boxState;
        ret->initView();
        
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void SGFightOverNoticeBox::initView()
{
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(false , true);
    
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    menu->setTouchMode(kCCTouchesOneByOne);
    menu->setTouchEnabled(true);
    
    //------------msg
    SGNotificationCenter * notif = SGNotificationCenter::sharedNotificationCenter();
    notif->addObserver(MSG_REWARD_LIANDANENTER , this , callfuncO_selector(SGFightOverNoticeBox::openLianDanAction));
    
    
    //compute data
    this->getRealViewData();
    
    //------------resource
    int index = atoi(m_fightOverNoticeData->bgFileName.c_str());
    CCString * str = CCString::createWithFormat("sgFightOverNoticeBox/sgFightOverNoticeBox_%d.plist" , (index-1)/3 + 1);
    ResourceManager::sharedInstance()->bindTexture(str->getCString(), RES_TYPE_LAYER_UI, sg_fightOverNoticeBox);
    
    //------------ui
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //蒙黑板
    ccColor4B color;
    if(-1 == m_fightOverNoticeData->bgColor[0])
        color = ccc4(10, 10, 10, 220);
    else
        color = ccc4(m_fightOverNoticeData->bgColor[0], m_fightOverNoticeData->bgColor[1], m_fightOverNoticeData->bgColor[2], 200);
    CCLayerColor * colorLayer = CCLayerColor::create(color, winSize.width, winSize.height);
    colorLayer->setPosition(CCPointZero);
    colorLayer->setTouchEnabled(false);
    this->addChild(colorLayer , -10);
    //bg
    CCSprite * bg = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("fightOverBoxBg_%s.png", m_fightOverNoticeData->bgFileName.c_str() )->getCString());
    bg->setScale(1.05);
    bg->setPosition(winSize/2);
    this->addChild(bg , 0 ,19);
    
    //按钮
    SGButton* btnA = SGButton::createFromLocal("an.png", m_fightOverNoticeBtnA->btnText.c_str(), this, getFuncAddres(m_fightOverNoticeBtnA->gotoId) , CCPointZero , FONT_PANGWA , ccWHITE , 32 ) ;
    btnA->setAnchorPoint(ccp(0.5 ,1));
    this->addBtn(btnA);
    
    SGButton * btnB = NULL;
    if(m_fightOverNoticeBtnB) //2个按钮
    {
        btnB = SGButton::createFromLocal("an.png", m_fightOverNoticeBtnB->btnText.c_str(), this, getFuncAddres(m_fightOverNoticeBtnB->gotoId) , CCPointZero , FONT_PANGWA , ccWHITE , 32 );
        btnB->setAnchorPoint(ccp(0.5, 1));
        btnB->setPosition(ccp(winSize.width*0.5 + 100, bg->getPositionY() - bg->getContentSize().height*0.5*bg->getScaleY() ));
        this->addBtn(btnB);
        
        btnA->setPosition(winSize.width*0.5 - 100 , btnB->getPositionY());
        
    }
    else//1个
    {
        btnA->setPosition(ccp(winSize.width*0.5, bg->getPositionY() - bg->getContentSize().height*0.5*bg->getScaleY()));
    }
}

void SGFightOverNoticeBox::getRealViewData()
{
    SGFightOverNoticeDataManager * m = SGFightOverNoticeDataManager::getInstance();
    vector<FightOverNoticeData*> vec = m->getFightOverNoticeData(m_boxState);
    
    int sum = 0;
    for(int i=0; i<vec.size();i++)
    {
        sum += vec[i]->weight;
    }
    time_t nouse;
    int i=0 ;
    nouse = time(NULL);
    srand(nouse);

    int rd = rand()%sum + 1;
    int temp =0;
    i=0;
    for( ; i<vec.size() ; i++)
    {
        temp += vec[i]->weight;
        if(temp >= rd)
            break;
    }
    CCAssert(i < vec.size(), "fatal error. wrong weight random.");
    m_fightOverNoticeData = vec[i];
    m_fightOverNoticeBtnA = m->getFightOverNoticeBtnData(m_fightOverNoticeData->buttonA);
    m_fightOverNoticeBtnB = m->getFightOverNoticeBtnData(m_fightOverNoticeData->buttonB);
    
    //统计  随机的正确性
    /*
    SGFightOverNoticeDataManager * m = SGFightOverNoticeDataManager::getInstance();
    vector<FightOverNoticeData*> vec = m->getFightOverNoticeData(m_boxState);
    
    int sum = 0;
    for(int i=0; i<vec.size();i++)
    {
        sum += vec[i]->weight;
    }
    time_t nouse;
    int i=0 ;
    nouse = time(NULL);
    srand(nouse);
    FILE * fp = fopen("/Users/pengyou/Desktop/random.txt", "w+");
    fprintf(fp, "\n\n\n\n\nsum = %d \n" , sum);
    map<int,int> myMap;
    map<int , int >::iterator iter;
    for(int count =0;count <10000000 ; ++count)
    {
        int rd = rand()%sum + 1;
        int temp =0;
        i=0;
        for( ; i<vec.size() ; i++)
        {
            temp += vec[i]->weight;
            if(temp >= rd)
                break;
        }
        CCAssert(i < vec.size(), "fatal error. wrong weight random.");
        //fprintf(fp, "count = %d  , srand() = %ld  ,  rd = %d  ,  select=%d , weight = %d \n", count ,nouse , rd , i ,vec[i]->weight);
        iter = myMap.find(vec[i]->weight);
        if( iter != myMap.end())
            iter->second++;
        else
            myMap.insert(std::pair<int , int>(vec[i]->weight, 1));
    }
    iter = myMap.begin();
    for(;iter != myMap.end();++iter)
    {
        fprintf(fp, "weight = %d  ,  count = %d\n" , iter->first , iter->second);
    }
   
    fclose(fp);
     
    m_fightOverNoticeData = vec[i];
    m_fightOverNoticeBtnA = m->getFightOverNoticeBtnData(m_fightOverNoticeData->buttonA);
    m_fightOverNoticeBtnB = m->getFightOverNoticeBtnData(m_fightOverNoticeData->buttonB);
     */
   
}

void SGFightOverNoticeBox::openLianDanAction(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    main::RewardLianDanTimeRequest *request = new main::RewardLianDanTimeRequest();
    request->set_index(m_ldIndex);
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_LIANDANENTER, request);
}

void SGFightOverNoticeBox::liandanListener(CCObject *obj)
{
    this->boxClose();
    SGSocketRequest *req = (SGSocketRequest *)obj;
    if (req)
    {
        main::RewardLianDanTimeResponse *response = (main::RewardLianDanTimeResponse *)req->m_msg;
        int time = response->time();
        SGMainManager::shareMain()->showLiandanLayer(time, m_ldIndex);
    }
}
void SGFightOverNoticeBox::gotoCharge(CCObject* obj)
{
    this->boxClose();
    SGMainManager::shareMain()->getMainLayer()->onClickAddYuanbao();
}

void SGFightOverNoticeBox::gotoOfficerList(CCObject* obj)
{
    this->boxClose();
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showGeneralsBagLayer(0);
}

void SGFightOverNoticeBox::gotoRewardLayer(CCObject* obj)
{
    this->boxClose();
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showRewardsLayer();
}

void SGFightOverNoticeBox::gotoStreng(CCObject* obj)
{
    this->boxClose();
    EFFECT_PLAY(MUSIC_BTN);
    SGFirstStrengBox *strengBox = SGFirstStrengBox::create(NULL);
    SGMainManager::shareMain()->showBox(strengBox);
}

void SGFightOverNoticeBox::gotoVipInfo(CCObject* obj)
{
    this->boxClose();
    main::ShopVipGiftBagEntryRequest *request = new main::ShopVipGiftBagEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_VIPGIFT_BAGS, request);
}

void SGFightOverNoticeBox::gotoVisit(CCObject* obj)
{
    this->boxClose();
    SGMainManager::shareMain()->getMainLayer()->visitHandler();
}

void SGFightOverNoticeBox::gotoWestShop(CCObject* obj)
{
    this->boxClose();
    CCLog("去往西域即将开启，敬请期待。");
    //奇遇界面，默认进入西域商人。
    SGMainManager::shareMain()->exoMechMainInfoRequest();
}

bool SGFightOverNoticeBox::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCSprite * bg = dynamic_cast<CCSprite*>( getChildByTag(19) );
    if( bg->boundingBox().containsPoint( pTouch->getLocationInView() ) )
        return true;
    if(m_firstTouch)
    {
        m_firstTouch = false;
        return true;
    }
    this->boxClose();
    SGPlayerInfo::sharePlayerInfo()->setisshowed(true);
    SGFightRewardLayer::gobackToStoryLayer();
    
    return true;
}

void SGFightOverNoticeBox::boxClose()
{
    SGBaseBox::boxClose();
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
    {
        mainLayer->setIsCanTouch(true,true);
    }
}

/*根据按钮去往id返回函数地址*/
SEL_MenuHandler SGFightOverNoticeBox::getFuncAddres(int gotoId)
{
    switch(gotoId)
    {
        case 1:
            return menu_selector(SGFightOverNoticeBox::gotoCharge);
        case 2:
            return menu_selector(SGFightOverNoticeBox::gotoStreng);
        case 3:
            return menu_selector(SGFightOverNoticeBox::gotoVisit);
        case 4:
            return menu_selector(SGFightOverNoticeBox::gotoOfficerList);
        case 5:
            return menu_selector(SGFightOverNoticeBox::gotoVipInfo);
        case 6:
            return menu_selector(SGFightOverNoticeBox::gotoWestShop);
        case 7:
            return menu_selector(SGFightOverNoticeBox::gotoRewardLayer);
        default:
            assert(1==0);
    }
}


//-----------------------------------file end.
