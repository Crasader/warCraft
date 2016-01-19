//
//  SGComplainLayer.cpp
//  GameSanGuo
//
//  Created by kanata on 14-1-6.
//
//

#include "SGComplainLayer.h"
#include "ResourceManager.h"
#include "SGMainManager.h"
#include "SGComplainCell.h"
////////////
#include "SGMainScene.h"
#include "SGChatWithGM.h"
#include "SGStringConfig.h"

SGComplainLayer::SGComplainLayer()
:_array(NULL)
{
    
    _id=0;
}
SGComplainLayer::~SGComplainLayer()
{
    CC_SAFE_RELEASE(datas);
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_complain);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_DELQA);

}
SGComplainLayer *SGComplainLayer::create(CCArray*array)
{
    SGComplainLayer *Layer = new SGComplainLayer();
    if (Layer && Layer->init(NULL, sg_complain))
    {
        Layer->_array=array;
        Layer->initView();
        Layer->autorelease();
       
        
        return Layer;
    }
    CC_SAFE_DELETE(Layer);
    return NULL;
}

void SGComplainLayer::initView()
{
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_DELQA, this, callfuncO_selector(SGComplainLayer::deletesuccess));
    
    SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
    sgnc->addObserver(MSG_CHECKQA, this, callfuncO_selector(SGComplainLayer::checkqaresponse));
    
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_complain);
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_complain);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_complain);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_complain);

    
    datas= CCArray::create();
    datas->retain();
    datas->addObjectsFromArray(_array);
    //datas=_array;
    
    
        CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    if(datas->count()==0)
    {
    
        SGCCLabelTTF *title = SGCCLabelTTF::create(str_ComplainLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
        title->setPosition(center);
        this->addChild(title);
    
    }
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGComplainLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55,title_bg->getPosition().y -title_bg->getContentSize().height*.5f));

    
    
  
    //    CCSprite *title = CCSprite::createWithSpriteFrameName("help_font_kptj.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_ComplainLayer_str2, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
   
    title->setPosition(ccp(s.width/2, backBtn->getPosition().y));
    this->addChild(title);
    ////提交新问题
    SGButton *tjxwt = SGButton::createFromLocal("reward_btn_redbg.png", str_ComplainLayer_str3, this, menu_selector(SGComplainLayer::newquestion),CCPointZero,FONT_BOXINFO,ccWHITE,28);
    tjxwt->setFontColor(ccc3(0xff,0xf2, 0x23));
    tjxwt->setOutsideColor(ccc3(0x8d,0x13, 0x13));
  
    tjxwt->setPosition(ccpAdd(ccp(0,backBtn->getPositionY()), ccp(s.width-tjxwt->getContentSize().width*0.6,  0)));
    this->addBtn(tjxwt);

    //tableview
    tableViewHeight = skewing(65);
    tableView->setFrame(CCRectMake(0, 0, s.width, s.height - headhgt - btmhgt));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -tableView->getContentSize().height - headhgt)));
    tableView->setDown(-25);
   // tableViewHeight=130;




}



void SGComplainLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGMainManager::shareMain()->showHelpLayer();
}


 SNSTableViewCellItem* SGComplainLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	SGComplainCell *item = (SGComplainCell *)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGQAConcent *data = (SGQAConcent*)datas->objectAtIndex(indexPath->getRow());

    if (item == NULL) {
        item = SGComplainCell::create(data);
    }


    else
    {
     //   cell->update(_isDeleteModel,data);
    }
 //////////////////////////////////////
    return item;

    
    
}
 void SGComplainLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
///////
}
unsigned int SGComplainLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)

{
    return datas->count();
}


void SGComplainLayer::newquestion()
{

    SGMainManager::shareMain()->showliuyanlayer();
    
}

void SGComplainLayer:: deletesuccess(CCObject*obj)
{
    
    
    SGSocketRequest *req = (SGSocketRequest *)obj;
    main::DelQAResponse*response =(main::DelQAResponse*)req->m_msg;
    
    
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
  int idid=  player->getcheckid();
    if(response->state()==1)
    {
        updatedata(idid);
         SG_SHOW_WINDOW(str_ComplainLayer_str4);
    }
    else
    {
    SG_SHOW_WINDOW(str_ComplainLayer_str5);
    }




}




void SGComplainLayer::updatedata(int id)
{
    

        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getQAContent();
        SGQAConcent *player = NULL;
        for (int ii = 0; ii < array->count(); ii++)
        {
            player = (SGQAConcent *)array->objectAtIndex(ii);
            if (player->getID() == id) break;
        }
        if (player != NULL)
        {
            SGPlayerInfo::sharePlayerInfo()->getQAContent()->removeObject(player);
            this->_array = SGPlayerInfo::sharePlayerInfo()->getQAContent();
            datas->removeAllObjects();
            datas->addObjectsFromArray(_array);
            this->tableView->reloadData();
            
        }
    
  
    
    

    }



void SGComplainLayer::checkqaresponse(CCObject*obj)
{
    //////
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_CHECKQA);
    
    
    ///////
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::CheckQAResponse *response = (main::CheckQAResponse *)sr->m_msg;
    
    CCArray *array=CCArray::create();
    for(int i=0;i< response->answer_size();i++)
    {
        main::AnswerProto proto= response->answer(i);
        //////////////////////////
        
        QADetail *detail=new QADetail();
        detail->setgmid(proto.gmid());
        
        std::string str2(proto.time());
         CCString* nn=CCString::createWithFormat("%s",str2.c_str());
        detail->settime(nn);
        
        std::string str(proto.content());
        
        CCString* ns=CCString::createWithFormat("%s",str.c_str());
        
        detail->setcontent(ns);
        
        array->addObject(detail);
        
        
        
        
    }
    /////
    SGPlayerInfo *playInfo = SGPlayerInfo::sharePlayerInfo();
    //////////////
    
    
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setupdatebattlebtnvisible();
    //////////////
    int lookid= playInfo->getcheckid();
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();

//    if (mainScene->getChildByTag(mainScene->nowLayerTag)) {
//        mainScene->removeChildByTag(mainScene->nowLayerTag, true);
//    }


    SGChatWithGM *chat = (SGChatWithGM*)mainScene->getChildByTag(sg_chatLayer);
    if(!chat)
    {
        
        //SGChatMessageCenter::shareChatMessageCenter()->getArrayCount();
        chat = SGChatWithGM::create(array,lookid);
    }
    SGMainLayer *mainLayer = (SGMainLayer*)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setPortraitMenuCanTouch(false);
//    SGButton*btn=(SGButton*)  mainLayer->getBtnByTag(54321);
//    btn->setEnabled(false);
    mainLayer->visitshow();
    mainScene->addShowLayer(chat);
    
    
}















