//
//  SGComplainCell.cpp
//  GameSanGuo
//
//  Created by kanata on 14-1-6.
//
//

#include "SGComplainCell.h"
#include "ResourceManager.h"
#include "SGCantAdvanceBox.h"
#include "SGMainManager.h"
#include  "SGChatWithGM.h"
#include "SGComplainLayer.h"
#include "SGTools.h"
#include "SGSystemMailCell.h"
#include "SGStringConfig.h"


QADetail::QADetail()
:typetype(0),
idid(0)
{
    m_gmid=-1;
    m_time=NULL;
    m_content=NULL;
}
QADetail::~QADetail()
{


}









SGComplainCell::SGComplainCell()
{
    _data=NULL;
}
SGComplainCell::~SGComplainCell()
{
    //ResourceManager::sharedInstance()->unBindLayerTexture(sg_complain);
}


SGComplainCell* SGComplainCell::create(SGQAConcent*data)
{
    SGComplainCell *cell = new SGComplainCell();
    if (cell && cell->init()) {
        cell->initBackground();
        cell->initView(data);
        cell->autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;

}
void SGComplainCell::initBackground()
{
    spriteBg = CCSprite::createWithSpriteFrameName("server_BlueBG.png");
    spriteBg->setScaleX(skewing(300)/spriteBg->getContentSize().width);
    spriteBg->setScaleY(skewing(65)/spriteBg->getContentSize().height);
    this->addChild(spriteBg, -10);
    spriteBg->setVisible(false);
    /////
    bgSize=CCSizeMake(skewing(300), skewing(65));
    float viewWid = bgSize.width;
  
    
    
    
    CCScale9Sprite *frame2=CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    
    frame2->setPreferredSize(CCSizeMake(skewing(300), skewing(55)));
    this->addChild(frame2);
    frame2->setPosition(spriteBg->getPosition());
    
    ////
    CCSprite *spriteBorder = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    // spriteBorder->setPreferredSize(CCSizeMake(skewing(300), skewing(55)));
    
    
    spriteBorder->setScaleX(frame2->getContentSize().width/spriteBorder->getContentSize().width);
    spriteBorder->setScaleY(frame2->getContentSize().height/spriteBorder->getContentSize().height);
    
    
    spriteBorder->setPosition(spriteBg->getPosition());
    this->addChild(spriteBorder, -10);
    
    
    CCSprite *guang2l = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2l, -1);
    guang2l->setAnchorPoint(ccp(0, 1));
    CCSprite *guang2r = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2r, -1);
    guang2r->setFlipX(true);
    guang2r->setAnchorPoint(ccp(1, 1));
    
    CCSprite *guang2m = CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
    this->addChild(guang2m, -1);
    guang2m->setScaleX((viewWid-120)/guang2m->getContentSize().width);
    guang2m->setAnchorPoint(ccp(0.5, 1));
    guang2m->setPosition(ccpAdd(frame2->getPosition(), ccp(0, frame2->getContentSize().height*.5)));
    guang2r->setPosition(ccpAdd(guang2m->getPosition(), ccp(viewWid*.5f, 0)));
    guang2l->setPosition(ccpAdd(guang2m->getPosition(), ccp(-viewWid*.5f, 0)));
}



void SGComplainCell::initView(SGQAConcent*data)
{
    
    _data=data;

    std::string contentString;
    contentString.append(SGSystemMailCell::getFirstTenWords(data->getTitle()->getCString(), 10));
   
    
    
    qtitle=SGCCLabelTTF::create(contentString.c_str(), FONT_BOXINFO, FONTSIZE(13) , ccc3(0xff, 0xc2, 0x00));
//    qtitle=SGCCLabelTTF::create(data->getTitle()->getCString(), FONT_BOXINFO, FONTSIZE(13));
    qtitle->setAnchorPoint(ccp(0, 0.5));
    qtitle->setPosition(ccpAdd(spriteBg->getPosition(), ccp(-spriteBg->getContentSize().width*0.5,spriteBg->getContentSize().height*0.2)));
    //modify by:zyc. merge into create.
    //qtitle->setColor(ccc3(0xff, 0xc2, 0x00));
    this->addChild(qtitle );
    
    ////时间
   
   SGCCLabelTTF *timetime=SGCCLabelTTF::create(data->getcreateDate()->getCString(), FONT_BOXINFO, FONTSIZE(13));
    timetime->setPosition(ccpAdd(qtitle->getPosition(), ccp(0, -spriteBg->getContentSize().height*0.4)));
    timetime->setAnchorPoint(ccp(0, 0.5));
    this->addChild(timetime);
    
    //删除按钮
    SGButton *shanchubtn =SGButton::createFromLocal("btn_fenbg.png", str_Delete, this,menu_selector(SGComplainCell::deletefun),CCPointZero,FONT_PANGWA,ccWHITE);
    shanchubtn->setPosition(ccpAdd(spriteBg->getPosition(), ccp(spriteBg->getContentSize().width*0.2, 0)));
    shanchubtn->setScale(1.1);
    this->addBtn(shanchubtn);
    this->addChild(shanchubtn);
    shanchubtn->setTag(_data->getID());
    
    //查看按钮
    SGButton *chakanbtn =SGButton::createFromLocal("btn_fenbg.png", str_SeasonListLayer_str7, this,menu_selector(SGComplainCell::lookfun),CCPointZero,FONT_PANGWA,ccWHITE);
    chakanbtn->setPosition(ccpAdd(spriteBg->getPosition(), ccp(spriteBg->getContentSize().width*0.42, 0)));
    chakanbtn->setScale(1.1);
    this->addBtn(chakanbtn);
    this->addChild(chakanbtn);
    chakanbtn->setTag(_data->getID()+20000);//动态tag 解决与删除按钮tag冲突
    //叹号
    
    
    CCSprite *spr_notice =  CCSprite::createWithSpriteFrameName("publc_notice.png");
    spr_notice->setPosition(ccpAdd(chakanbtn->getPosition(), ccp(chakanbtn->getContentSize().width*.37, chakanbtn->getContentSize().height*.38)));
    this->addChild(spr_notice);
    
    
    if(_data->getState())
    {
    spr_notice->setVisible(false);
    }
    else
    {
        spr_notice->setVisible(true);
    }
    
    
}


void SGComplainCell::deletefun(CCNode*node)
{
    int tag=node->getTag();
    SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 64, tag);
    SGMainManager::shareMain()->showBox(cantadvanceBox);

}



void SGComplainCell::lookfun(CCNode*node)
{
    int tagtag=node->getTag()-20000;
    ///存储id
 

    /////////////////////////////////////////////////////本地刷新客服列表查看状态
    
    CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getQAContent();

    for(int i=0;i<temp->count();i++)
    {
        SGQAConcent * qa = (SGQAConcent*)temp->objectAtIndex(i);
        
        if(qa->getID()==tagtag)
        {
            qa->setState(1); ////手动设置查看状态  貌似1是已查看
        }
    }
    
    
    
    
    
    
    
    ///////////////////////////////////////////////////////
     SGPlayerInfo *playInfo = SGPlayerInfo::sharePlayerInfo();
    
    playInfo->setcheckid(tagtag);
    
    
    
    //////////////////////////////////////////////////////
    
    main::CheckQARequest*request=new main::CheckQARequest();
    request->set_id(tagtag);
    
    SGSocketClient::sharedSocketClient()->send(MSG_CHECKQA, request);
    
}

