//
//  SGEmbattleItem.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-21.
//
//

#include "SGEmbattleItem.h"
#include "SGPlayerInfo.h"
#include "SGEmbattleLayer.h"
#include "SGShowAllLayer.h"
#include "SGStaticDataManager.h"
#include "SGMainManager.h"
#include "SGShowString.h"
#include "ResourceManager.h"

#include "SGTeamgroup.h"
#include "SGDrawCards.h"
SGEmbattleItem::SGEmbattleItem():
isRock(false),
timer(0),
emtype(0),
isSelect(0),
officerCard(NULL),
index(0),
_islord(0),
face(NULL),
isCanMove(false),
delegate(NULL),
namebg(NULL),
namelabel(NULL)
{}
SGEmbattleItem::~SGEmbattleItem()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_emBattleItem);

}
SGEmbattleItem *SGEmbattleItem::create(MoveDelegate *dg, SGOfficerCard *card, int em,int islord)
{
    SGEmbattleItem *item = new SGEmbattleItem();
    item->emtype=em;
    item->_islord = islord;
    if (item && item->init(dg, card,0))
    {
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return NULL;
}
SGEmbattleItem *SGEmbattleItem::create(MoveDelegate *dg, int index_,int em)
{
    SGEmbattleItem *item = new SGEmbattleItem();
    item->emtype = em;
    
    if (item && item->init(dg, NULL,index_))
    {
        item->index = index_;
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return NULL;
}
bool SGEmbattleItem::init(MoveDelegate *dg, SGOfficerCard *card,int a)
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    namelabel=NULL;
    
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_emBattleItem);
	ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_emBattleItem, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer2.plist", RES_TYPE_LAYER_UI, sg_emBattleItem, LIM_PNG_AS_PNG);
   
     officerCard = card;
    delegate = dg;
    timer = 0;
    this->setTouchEnabled(true);    

    SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());

    if (officerCard && officerCard->getRace())
    {
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(officerCard->getItemId());
        int starlvl = temp->getOfficerCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        
        face = CCSprite::createWithSpriteFrameName("card_bj.png");
        CCSprite* kkkk = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl)->getCString());
        //if (str_1->isEqual(CCString::create("SGOfficerCard")))
        kkkk->removeChildByTag(30);
        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",officerCard->getRace()))->getCString());
        countryName->setAnchorPoint(ccp(1,1));
        countryName->ignoreAnchorPointForPosition(false);
        countryName->setPosition(ccp(kkkk->getContentSize().width*0.35,kkkk->getContentSize().height - countryName->getContentSize().height));
        countryName->setVisible(false);   //cgpUI
        kkkk->addChild(countryName,30,30);
        SGCCLabelTTF *advancenumber = NULL;
        if (officerCard->getAdNum()>0)
        {
            advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d" , officerCard->getAdNum())->getCString(), FONT_PANGWA, 24,ccGREEN);
            advancenumber->setAnchorPoint(ccp(1,1));
            advancenumber->setPosition(ccpAdd(kkkk->getContentSize(),ccp(-advancenumber->getContentSize().width + 15, -advancenumber->getContentSize().height + 15)  ));
            kkkk->addChild(advancenumber,50,250);
        }
        kkkk->setPosition(ccp(face->getContentSize().width*.5f, face->getContentSize().height*.5f));
        face->addChild(kkkk,1);
        
        if(0==_islord && officerCard->canShowNotice())
        {
            CCSprite * notice = CCSprite::createWithSpriteFrameName("publc_notice.png");
            notice->setPosition(ccpAdd(kkkk->getContentSize() , ccp(-25,-25)) );
            kkkk->addChild(notice , 50);
        }
        

    }
    else if(a <= player->getPlayerOfficerMax())
    {
        face = CCSprite::createWithSpriteFrameName("embattle_huilan.png");
        CCSprite *font = CCSprite::createWithSpriteFrameName("embattle_font_kong.png");
        face->addChild(font);
        font->setPosition(ccp(face->getContentSize().width*.5f, face->getContentSize().height*.5f));
        CCSprite *bt = CCSprite::createWithSpriteFrameName("embattle_kuang_op.png");
        face->addChild(bt,-1);
        bt->setPosition(ccp(face->getContentSize().width*.5f, 3));
        
    }
    else if(a > player->getPlayerOfficerMax())
    {
        face = CCSprite::createWithSpriteFrameName("embattle_lan.png");
        CCSprite *bt = CCSprite::createWithSpriteFrameName("embattle_kuang_op.png");
        face->addChild(bt,-1);
        bt->setPosition(ccp(face->getContentSize().width*.5f, 3));
        CCSprite *sprsuo = CCSprite::createWithSpriteFrameName("embattle_suo.png");
        face->addChild(sprsuo,10);
        sprsuo->setAnchorPoint(ccp(1, 1));
        sprsuo->setPosition(ccp(face->getContentSize().width-3, face->getContentSize().height-3));

        CCSprite *num = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("embattle_num%d.png",a)->getCString());
            face->addChild(num);
            num->setPosition(ccp(face->getContentSize().width*.5f, face->getContentSize().height*.5f));
    
    }

    this->addChild(face,1);
    //cgpUI
    namebg = CCSprite::createWithSpriteFrameName("name_bg.png");
    namebg->setPosition(ccpAdd(face->getPosition(),ccp(20,-70)));

    if (!_islord)
    {
        namebg->setScaleX(0.6);
        namebg->setScaleY(0.8);
        namebg->setPosition(ccpAdd(face->getPosition(),ccp(0,-70)));

    }
    namebg->setVisible(false);
    this->addChild(namebg);
    
    if (card)
    {
        officerCard->retain();
        officerCard->autorelease();
        char spriteName[256] = {0};
		
		//12.05
		SGMainManager::shareMain()->addHeadIconPlistByNum(officerCard->getHeadSuffixNum(),sg_emBattleItem);
        sprintf(spriteName, "head%d.png",officerCard->getHeadSuffixNum());
        

        namelabel = CCLabelTTF::create(card->getOfficerName()->getCString(), FONT_BOXINFO, 20);
        namelabel->setPosition(namebg->getPosition());
        if (_islord)
           namelabel->setPosition(ccpAdd(namebg->getPosition(),ccp(-20,0)));
        this->addChild(namelabel);
        
        SGCCLabelTTF *level = SGCCLabelTTF::create(CCString::createWithFormat("Lv:%d",card->getCurrLevel())->getCString(), FONT_PANGWA, 23);
        level->setAnchorPoint(ccp(0.5,0));
		level->setPosition(ccpAdd(face->getPosition(),ccp(face->getContentSize().width/2,level->getContentSize().height*0 - 8)));
        face->addChild(level);
        level->setZOrder(3);
	
        CCSprite *aa1 = CCSprite::createWithSpriteFrameName(spriteName);
        face->addChild(aa1,0, 321);
        aa1->setPosition(ccp(face->getContentSize().width*.5f, face->getContentSize().height*.5f));
        
        index = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,card);
       
        this->setIsLord(SGTeamgroup::shareTeamgroupInfo()->islord(emtype,card));
        
        
    }
//    if(index == 1)
//    {
//        CCSize size = this->getContentSize();
//        CCLayerColor * layer = CCLayerColor::create(ccc4(0, 255, 0, 100), size.width, size.height);
//        layer->ignoreAnchorPointForPosition(false);
//        layer->setAnchorPoint(ccp(0, 0));
//        layer->setPosition(0, 0);
//        this->addChild(layer , 1000);
//    }
    return true;
}
float SGEmbattleItem::getWidth()
{
    return face->getContentSize().width;
}
float SGEmbattleItem::getHeight()
{
    return face->getContentSize().height;
}
void SGEmbattleItem::addTimer()
{
    
    timer++;
    CCLOG("两秒:%d", timer);
    if (timer >= 5)
    {
         this->unschedule(schedule_selector(SGEmbattleItem::addTimer));
        
        isCanMove= true;
        firstPoint = face->getPosition();
        if (delegate)
        {           
            delegate->setIsSelect(true);
            
            ///在移动的时候有一个灰色的底还放在那里
//            CCSprite * sprite = SGDrawCards::drawallcards(officerCard->getItemId(),6,sg_emBattleItem);
            CCSprite * sprite = SGDrawCards::createNormalSprite(officerCard->getItemId(),6,sg_emBattleItem);
            sprite->setPosition(ccpAdd(this->getPosition(),ccp(face->getContentSize().width*0, face->getContentSize().height*0)));
            CCSprite *cardbg = CCSprite::createWithSpriteFrameName("card_bj.png");
            cardbg->setPosition(ccp(sprite->getContentSize().width/2,sprite->getContentSize().height/2));//蒙黑的图
            sprite->addChild(cardbg,100);
            CCNode *node = this->getParent();
            node->addChild(sprite);
            sprite->setTag(10000);
            namelabel->setColor(ccGRAY);
            

            node->reorderChild(this, 1);
            this->startRock();
        }
    }
}
void SGEmbattleItem::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
void SGEmbattleItem::startRock()
{
	if (this->getChildByTag(321))
	{
		this->removeChildByTag(321);
	}
	
    if (isRock) {
        return;
    }
    isRock = true;
    CCScaleTo *scale1 = CCScaleTo::create(.1, 1.5);
    CCScaleTo *scale2 = CCScaleTo::create(.1, 1);
    face->runAction(CCRepeatForever::create((CCActionInterval *)CCSequence::create(scale1, scale2,NULL)));
}
void SGEmbattleItem::stopRock()
{
    isRock = false;
    face->stopAllActions();
    face->setScale(1);
}
bool SGEmbattleItem::testIsCanMove(CCPoint point)
{
    SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
    CCPoint facePoint = face->getPosition();    
    facePoint = convertToWorldSpace(facePoint);
    if ( point.x <(facePoint.x+face->getContentSize().width *.5)
        && point.x >(facePoint.x - face->getContentSize().width *.5 )
        && point.y > (facePoint.y - face->getContentSize().height *.5)
        && point.y < (facePoint.y + face->getContentSize().height *.5) && index <= player->getPlayerOfficerMax())
    {
        return true;
    }
    return false;
}
bool SGEmbattleItem::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    
    SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
    CCPoint pt = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    if(!SGShowAllLayer::scrollviewRect.equals(CCRectZero))
    {
        if(!SGShowAllLayer::scrollviewRect.containsPoint(pt))
        {//展开状态的矩形区域
            return true;
        }
    }
    
    if(!SGEmbattleLayer::scrollviewRect.equals(CCRectZero))
    {
        if(!SGEmbattleLayer::scrollviewRect.containsPoint(pt))
        {//如果点击区域在scrollview内部
            return true;
        }
    }
    
    CCPoint point = convertTouchToNodeSpace(pTouch);
    CCRect faceRect = CCRectMake(face->getPosition().x - face->getContentSize().width * .5,
                                 face->getPosition().y - face->getContentSize().height * .5,
                                 face->getContentSize().width,
                                 face->getContentSize().height);
    if (faceRect.containsPoint(point) &&index <= player->getPlayerOfficerMax())
    {
        CCLOG("中:%d",index);
        isSelect = true;
		CCLayerColor *bl = CCLayerColor::create(ccc4(0, 0, 0, 150), 100,100);//100 阴影效果的宽高
		bl->setPosition(face->getPosition());
		bl->setAnchorPoint(ccp(0.5, 0.5));
		bl->ignoreAnchorPointForPosition(false);
		this->addChild(bl, 10, 321);
		
        if (officerCard && _islord==0)//主将框不让动
            this->schedule(schedule_selector(SGEmbattleItem::addTimer),.1);
    }
    else if (faceRect.containsPoint(point) &&index > player->getPlayerOfficerMax())
    {
//        SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
//        SGMainManager::shareMain()->showMessage(player->getPlayerUnlockTip()->getCString());
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_EmbattleLayer_str23, SGPlayerInfo::sharePlayerInfo()->getTipAtLevel(index) )->getCString() );
    }
       return true;
}
void SGEmbattleItem::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    CCPoint point = convertTouchToNodeSpace(pTouch);
    isCanMove= false;
   
    this->unschedule(schedule_selector(SGEmbattleItem::addTimer));
    face->setPosition(firstPoint);
    this->stopRock();
    //
    if (delegate && isSelect)
    {
		this->removeChildByTag(321);
        if (timer <2)
        {
           delegate->selectedIndex(this);
        }
        
        CCNode *node = this->getParent();
        node->removeChildByTag(10000,true);//拿到加的那个临时蒙黑图
        if (namelabel) {
            namelabel->setColor(ccWHITE);
        }
        
        
        isSelect = false;
        delegate->setIsSelect(false);
        delegate->trade();
    }
     timer = 0;
}

void SGEmbattleItem::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isCanMove)
    {
        this->stopRock();
        CCPoint point = convertTouchToNodeSpace(pTouch);
        face->setPosition(point);
        
        if (delegate)
            {
                point = convertToWorldSpace(point);
                delegate->moveToPoint(point, index);
                
            }
    }
}
void SGEmbattleItem::setIsLord(bool isLord)
{
    ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer2.plist", RES_TYPE_LAYER_UI, sg_emBattleItem, LIM_PNG_AS_PNG);
    if (isLord)
    {
        CCSprite *zhu = (CCSprite::createWithSpriteFrameName("countryNamelord.png"));
        zhu->setAnchorPoint(ccp(1,1));
        zhu->ignoreAnchorPointForPosition(false);
        zhu->setPosition(ccp(face->getContentSize().width*0.45,face->getContentSize().height + zhu->getContentSize().height*0.2));
        face->addChild(zhu,30,111);
        
        CCSequence *seq = CCSequence::create(CCFadeIn::create(.5),CCDelayTime::create(2),CCFadeOut::create(.5),CCDelayTime::create(1),NULL);
        zhu->runAction(CCRepeatForever::create(seq));
    }
    else
        this->removeChildByTag(111, true);
}
SGOfficerCard *SGEmbattleItem::getOfficerCard()
{
    return officerCard;
}
