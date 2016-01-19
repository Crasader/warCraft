//
//  SGSystemMailLayer.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#include "SGSystemMailLayer.h"
#include "SGMainManager.h"
#include "SGMailCell.h"
#include "SGMail.h"
#include "SGSystemMailCell.h"
#include "SGSystemMailContent.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGSystemMailLayer::SGSystemMailLayer():buttonModel(NULL)
{
    _mailArray = NULL;
    _isDeleteModel = false;
    m_pNoMailLabel = NULL;
    
    CCLOG("SGSystemMailLayer()");
}

SGSystemMailLayer::~SGSystemMailLayer()
{
    CC_SAFE_RELEASE(_mailArray);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_systemMail);
    
    CCLOG("~SGSystemMailLayer()");
}

SGSystemMailLayer* SGSystemMailLayer::create(CCArray *mailArray)
{
    SGSystemMailLayer *mail = new SGSystemMailLayer();
    
    if(mail && mail->init(NULL, sg_systemMail))
    {
        mail->_mailArray = CCArray::create();
        mail->_mailArray->retain();
        
        mail->_mailArray->initWithArray(mail->sortByTimeAndRead(mailArray));
        
        mail->viewDidLoad();
        mail->autorelease();
        return mail;
    }
    
    CC_SAFE_DELETE(mail);
    return NULL;
}

CCArray * SGSystemMailLayer::sortByTimeAndRead(CCArray *mailData){
    
    CCArray *isReadArray=CCArray::create();
    isReadArray->retain();
    isReadArray->initWithArray(searchByRead(mailData, true));
    
    
    CCArray *isNoReadArray=CCArray::create();
    isNoReadArray->retain();
    isNoReadArray->initWithArray(searchByRead(mailData, false));
    
    CCArray *allObject=CCArray::create();
    allObject->retain();
    
    CCArray *isRead=CCArray::create();
    isRead->retain();
    isRead=sortBytime(isReadArray);
    
    CCArray *isNoRead=CCArray::create();
    isNoRead->retain();
    isNoRead=sortBytime(isNoReadArray);
    
    for (int i=0; i<isNoRead->count(); i++) {
       CCObject *obj = isNoRead->objectAtIndex(i);
        allObject->addObject(obj);
        
    }
    
    for (int i=0; i<isRead->count(); i++) {
        CCObject *obj = isRead->objectAtIndex(i);
        allObject->addObject(obj);
        
    }
    
    
    isReadArray->release();
    isRead->release();
    isNoRead->release();
    isNoReadArray->release();
    return allObject    ;
  
    

}
CCArray * SGSystemMailLayer::sortBytime(CCArray *mailData){
    int count=mailData->count();
    for (int i=0; i<count; i++) {
        for (int j=i+1; j<count; j++) {
            SGMailSystemDataModel *mode1=(SGMailSystemDataModel *)mailData->objectAtIndex(i);
            SGMailSystemDataModel *mode2=(SGMailSystemDataModel *)mailData->objectAtIndex(j);
            if (mode1->getSendTime()<mode2->getSendTime()) {
                mailData->exchangeObjectAtIndex(i, j);
            }
            
        }
    }
    return mailData;
}

CCArray *SGSystemMailLayer ::searchByRead(CCArray *mailData,bool isRead){

    CCArray *readArray=CCArray::create();
    readArray->retain();
    CCObject *obj;
    CCARRAY_FOREACH(mailData, obj){
        SGMailSystemDataModel *mode=(SGMailSystemDataModel *)obj;
        if (mode->getIsRead()==isRead) {
            readArray->addObject(obj);
        }
    
    }
    return readArray;
}

SNSTableViewCellItem* SGSystemMailLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    SGSystemMailCell *cell = (SGSystemMailCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    SGMailSystemDataModel *data = (SGMailSystemDataModel*)_mailArray->objectAtIndex(indexPath->getRow());
    if(NULL == cell)
    {
        cell = SGSystemMailCell::create(_isDeleteModel,data);
    }else
    {
        cell->update(_isDeleteModel,data);
    }
    
    return cell;
}

void SGSystemMailLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGMailSystemDataModel *mail = (SGMailSystemDataModel*)_mailArray->objectAtIndex(indexPath->getRow());
    SGSystemMailContent *content = SGSystemMailContent::create(mail);
    SGMainManager::shareMain()->showLayer(content);
}

void SGSystemMailLayer::viewDidLoad()
{
    this->datas = _mailArray;

    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_systemMail);
    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_systemMail);
    /************************页面布局********************************/
    CCPoint upLeft = SGLayout::getPoint(kUpLeft);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    //星形背景
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_systemMail);
    CCSprite *spriteBackground = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30+150+72,spriteBackground->getContentSize().width, SGLayout::getSkewingY(480)-47-150-72-61-101);
    //spriteBackground->cocos2d::CCNode::setScale(r.size.width / spriteBackground->getContentSize().width, r.size.height / spriteBackground->getContentSize().height);
    spriteBackground->setTextureRect(r);
    spriteBackground->setScaleX(s.width/spriteBackground->getContentSize().width * 1.01);
    spriteBackground->setAnchorPoint(ccp(0.5, 1));
    spriteBackground->setPosition(ccpAdd(upCenter, ccp(0, -47-150-72)));
    this->addChild(spriteBackground);
    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -headhgt- 10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(s.width/2,  -headhgt- 10)));
    this->addChild(title_bg_r,10);
    float h = SGMainManager::shareMain()->getTotleHdHgt();
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -h + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    //标题
//    CCSprite *spriteTitle = CCSprite::createWithSpriteFrameName("mail_titile_5.png");
    SGCCLabelTTF *spriteTitle = SGCCLabelTTF::create(str_SystemMailLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//spriteTitle->setColor(ccc3(0xff, 0x95, 0x0c));
    spriteTitle->setPosition(ccpAdd(upCenter, ccp(0,  -h+65/2)));
    

  
    // spriteTitle->setPosition(ccpAdd(upCenter, ccp(0, -s.height*0.23)));

    this->addChild(spriteTitle);
    
    //五张图片的尺寸   -79-150-72-56-101-30
//    SGButton *buttonBack = SGButton::create("chatbtn.png", "public_font_fanhui.png", this, menu_selector(SGSystemMailLayer::buttonClickGoBack),CCPointZero,false,true);
	SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGSystemMailLayer::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	buttonBack->setScale(1.05);
	
    buttonBack->setAnchorPoint(ccp(0, 0.5));
//    buttonBack->setPosition(ccpAdd(upLeft, ccp(0, -47-150-72/2)));
    buttonBack->setPosition(ccpAdd(upLeft, ccp(0, -h+65/2)));
    addBtn(buttonBack);
    
    //模式按钮 friend/friend_deleteModel.png删除 friend/friend_sendModel.png发送
    buttonModel = SGButton::create("chatbtn.png", "friend_deleteModel.png", this, menu_selector(SGSystemMailLayer::buttonClickModelChange),CCPointZero,false,true);
    buttonModel->setAnchorPoint(ccp(1, 0.5));
    buttonModel->setPosition(ccp(SGLayout::getSkewing(320) ,buttonBack->getPositionY()));
    addBtn(buttonModel);
    
    //tableview
    this->tableView->setFrame(CCRectMake(0,101+56, SGLayout::getSkewing(320), SGLayout::getSkewingY(480)-47-150-72-56-101));
    this->tableView->setZOrder(999);
    tableViewHeight = 110;
    tableView->setDown(-25);
    
    //没有收到任何消息
    m_pNoMailLabel = SGCCLabelTTF::create(str_SystemMailLayer_str2, FONT_BOXINFO, 28);
    m_pNoMailLabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -44)));
    addChild(m_pNoMailLabel);
    m_pNoMailLabel->setVisible(this->datas->count() == 0);
    
}

void SGSystemMailLayer::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMail *mail = SGMail::create();
    SGMainManager::shareMain()->showLayer(mail);
}

void SGSystemMailLayer::buttonClickModelChange()
{
    EFFECT_PLAY(MUSIC_BTN);
    //点击就切换图片
     ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_systemMail);
    if(_isDeleteModel)
    {
        buttonModel->setFontImage(CCSprite::createWithSpriteFrameName("friend_deleteModel.png")->displayFrame());
    }else
    {
        buttonModel->setFontImage(CCSprite::createWithSpriteFrameName("friend_sendModel.png")->displayFrame());
    }
    
    _isDeleteModel = !_isDeleteModel;
    tableView->reloadData();
}

void SGSystemMailLayer::deleteMailListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        printf("error deleteMailListener !!! \n");
        return;
    }
    
    main::DeleteMailResponse *response = (main::DeleteMailResponse*)request->m_msg;
    
    if(1 == response->state())
    {
        SG_SHOW_WINDOW(str_FriendMailLayer_str3);
    }else{
        SG_SHOW_WINDOW(str_FriendMailLayer_str4);
    }
}

void SGSystemMailLayer::recieveReloadUpdate(SGMailSystemDataModel* model)
{
    if (datas->containsObject(model)) {
        datas->removeObject(model);
        tableView->reloadData();
    }
    m_pNoMailLabel->setVisible(this->datas->count() == 0);
}
