
//
//  SGFirstSubLayer.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-19.
//
//

#include "SGFirstSubLayer.h"
#include "SGGeneralInfoLayer.h"
#include "SGMainManager.h"
#include "SGGeneralInfoLayer.h"
#include "SGCCLabelTTF.h"
#include "SGStaticDataManager.h"
#include "SGArrangeLayer.h"
#include "SGGuideManager.h"
#include "ResourceManager.h"
#include "SGTeamgroup.h"
#include "CCScrollView.h"

SGFirstSubLayer::SGFirstSubLayer():isMove(false)
,iscantouch(true),
_viewSize(CCSizeZero),
_officerCard(NULL),
enterType(0),
index(0),
touchBeginLocation(CCPointZero)
{
    _officerCard = NULL;
}

SGFirstSubLayer::~SGFirstSubLayer()
{
// 	ResourceManager::sharedInstance()->unBindLayerTexture(sg_firstSubLayer);
	 
    if (_officerCard) {
         _officerCard->release();
    }   
}

SGFirstSubLayer* SGFirstSubLayer::create(SGOfficerCard *officerCard,CCPoint position,CCSize viewSize,int type)
{
    SGFirstSubLayer *sublayer = new SGFirstSubLayer();
    sublayer->_viewSize = viewSize;
    sublayer->enterType = type;
    sublayer->_officerCard = officerCard;
    sublayer->index = position.x;
    if (officerCard) {
        sublayer->_officerCard->retain();
    }
    
    if(sublayer && sublayer->init(position, viewSize))
    {
        sublayer->constructView();
        sublayer->autorelease();
        return sublayer;
    }
    
    CC_SAFE_DELETE(sublayer);
    return NULL;
}

void SGFirstSubLayer::constructView()
{
    //在firstLayer中添加资源
    CCSprite *bg = CCSprite::createWithSpriteFrameName("embattle_huilan_lan.png");
    this->addChild(bg,-10);
    bg->setPosition(ccp(_viewSize.width/2, _viewSize.height/2));
    
    CCString *headString = NULL;
    CCString *countryStr = NULL;
    
    
    if (enterType == 0 ||enterType == 1 )
    {
        if(!_officerCard)
        {
			SGButton *spriteButton = SGButton::create("embattle_font_kong_lan.png", NULL, this, menu_selector(SGFirstSubLayer::selectEmbattle),CCPointZero,false,true);

			CCMenu *menu2 = CCMenu::create(spriteButton, NULL);
			this->addChild(menu2, 0, 125);
			menu2->setPosition(ccp(_viewSize.width/2, _viewSize.height/2));
        }
        else
        {
            CCString *string = CCString::createWithFormat("head%d.png",_officerCard->getHeadSuffixNum());//12.05
            headString = CCString::create(string->getCString());
            SGMainManager::shareMain()->addHeadIconPlistByNum(_officerCard->getHeadSuffixNum(), sg_firstLayer);//12.05

            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_officerCard->getItemId());
            int starlvl = temp->getOfficerCurrStarLevel();
            if (starlvl>1)
            {
                starlvl-=1;
            }
            countryStr = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
            CCSprite *spriteFont = CCSprite::createWithSpriteFrameName(countryStr->getCString());
            spriteFont->setPosition(ccp(_viewSize.width/2, _viewSize.height/2));
            
            spriteFont->removeChildByTag(30);
            CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",temp->getOfficerRace()))->getCString());
            countryName->setAnchorPoint(ccp(1,1));
            countryName->ignoreAnchorPointForPosition(false);
            countryName->setPosition(ccp(spriteFont->getContentSize().width*.36 - countryName->getContentSize().width,spriteFont->getContentSize().height - countryName->getContentSize().height * 2));
            countryName->setVisible(false);    //cgpRemove
            
            //spriteFont->addChild(countryName,30,30);
            
            
            SGCCLabelTTF *lvl = SGCCLabelTTF::create(CCString::createWithFormat("Lv%d",_officerCard->getCurrLevel())->getCString(), FONT_PANGWA, 23);
            this->addChild(lvl,22);
            lvl->setAnchorPoint(ccp(0.5, 0));
            lvl->setPosition(ccpAdd(spriteFont->getPosition(), ccp(0, -spriteFont->getContentSize().height*.35 -10)));

            int id = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
            if (SGTeamgroup::shareTeamgroupInfo()->islord(id,_officerCard))
            {
                CCSprite *lord = CCSprite::createWithSpriteFrameName("font_zj_bg.png");
                this->addChild(lord,99);
                lord->setPosition(ccpAdd(spriteFont->getPosition(), ccp(0, -lord->getContentSize().height*1.2)));
                CCSprite *font = CCSprite::createWithSpriteFrameName("font_zj.png");
                this->addChild(font,100);
                font->setPosition(lord->getPosition());
                CCBlink *blink = CCBlink::create(10, 1);
                lord->runAction(CCRepeatForever::create(blink));
                CCBlink *blink1 = CCBlink::create(10, 1);
                font->runAction(CCRepeatForever::create(blink1));               
            }
			SGButton *spriteButton = SGButton::create(headString->getCString(), countryStr->getCString(), this,
                                                      menu_selector(SGFirstSubLayer::selectEmbattle),CCPointZero,false,true);
			spriteButton->addChild(countryName,30,30);
			CCMenu *menu1 = CCMenu::create();
            //头像的值
            int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_12);
            menu1->addChild(spriteButton);
            //对应的tag
            spriteButton->setTag(modelTag);
			this->addChild(menu1, 0, 124);
			menu1->setPosition(ccp(_viewSize.width/2, _viewSize.height/2));
            
            SGCCLabelTTF *advancenumber = NULL;
            if (temp->getAdvNum()>0)
            {
                advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d" , temp->getAdvNum())->getCString(),
                                                     FONT_PANGWA, 24,ccGREEN);
                advancenumber->setAnchorPoint(ccp(1,1));
                
                advancenumber->setPosition(spriteButton->getContentSize());
                spriteButton->addChild(advancenumber,50,550);
            }

            
            //如果可以转生  则显示红色叹号
            SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
            if(_officerCard->canShowNotice()  && playerInfo->getPlayerLevel() >= playerInfo->getShowPlayerNoticeLimitLev())
            {
                CCSprite * _spriteNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
                _spriteNotice->setPosition( ccpAdd(spriteButton->getPosition() , ccp(spriteButton->getContentSize().width  , spriteButton->getContentSize().height -3 ) ));
                this->addChild(_spriteNotice,1000);
            }
        }

    }
    else if(enterType == 2)
    {
        CCSprite *spriteHead = CCSprite::createWithSpriteFrameName("jinkuang.png");
        spriteHead->setPosition(ccp(_viewSize.width/2, _viewSize.height/2));
        addChild(spriteHead);
        
        CCSprite *suo = CCSprite::createWithSpriteFrameName("firstkuangSou.png");
        addChild(suo);
        suo->setPosition(spriteHead->getPosition());
        
        SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
        
		std::string str(player->getPlayerUnlockLevel()->getCString());
		str[0] = 'L';
		CCString *lvlLabelTmp = CCString::create(str);
		SGCCLabelTTF *lvLabel=SGCCLabelTTF::create(CCString::createWithFormat("%s",lvlLabelTmp->getCString())->getCString(),
                                                   FONT_BOXINFO, 34 , ccYELLOW);
        addChild(lvLabel);
        lvLabel->setPosition(ccpAdd(spriteHead->getPosition(), ccp(0, -suo->getContentSize().height/2+20)));
        
    }
    else if(enterType == 3)
    {
        SGButton *btn = SGButton::create("mainBtn_bg.png", "embattlekuang.png", this,
                                         menu_selector(SGFirstSubLayer::buttonClickOK),CCPointZero,false,true);
        bg->removeFromParent();
        CCMenu *menu = CCMenu::create(btn,NULL);
        this->addChild(menu,0,123);
        menu->setPosition(ccp(_viewSize.width/2, _viewSize.height/2));
    }

}
void SGFirstSubLayer::buttonClickOK()
{
  SGMainManager::shareMain()->showEmbattleLayer(1);
}

bool SGFirstSubLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{

    
    if (SGGuideManager::shareManager()->isGuide)
    {
        return false;
    }
    CCPoint touchLocation = pTouch->getLocation(); // Get the touch position
    touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
    CCRect bBox=boundingBox();
    
    bool  istouchIn = bBox.containsPoint(touchLocation);
    if (istouchIn && iscantouch) {
        ((CCScrollView*)getParent()->getParent())->ccTouchBegan(pTouch, pEvent);
        touchBeginLocation = pTouch->getLocationInView();
        isMove = false;
    }
    return (istouchIn && iscantouch);
}

void SGFirstSubLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
    if (!isMove) {
        CCPoint touchLocation = pTouch->getLocationInView();
        float distance = ccpDistance(touchLocation, touchBeginLocation);
        if (distance > 6) {
            isMove = true;
        }
    }
    if (isMove) {
        ((CCScrollView*)getParent()->getParent())->ccTouchMoved(pTouch, pEvent);
    }
}

void SGFirstSubLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        return ;
    }
    CCScrollView* firsteneral = (CCScrollView*)getParent()->getParent();
    if (firsteneral) {
        (firsteneral)->ccTouchEnded(pTouch, pEvent);
    }
}

// 为显示阴影效果
void SGFirstSubLayer::selectEmbattle()
{
    // 如果有背包不足的弹框，首页武将栏不能点击
    if (SGMainManager::shareMain()->getMainScene()->getChildByTag(box_canAdvanceTag))
    {
        return ;
    }
    
	int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    //新手引导修改，这里做修改
    //if (guideId < UIG_MAX1) return;
    
    if (!iscantouch || isMove) {
        return;
    }
    if (enterType == 0)
    { //可以导向武将滑动！
        //获得当前阵营武将列表大全。
        SGTeamgroup* sgtg = SGTeamgroup::shareTeamgroupInfo();
        CCArray* ofcList = sgtg->getEmbattleOfficers(sgtg->getiscurrent());
        SGGeneralInfoLayer::setMyOfficerList(ofcList);
        //打开武将详情
        SGMainManager::shareMain()->showGeneralInfoLayer(_officerCard, 3,NULL,true,NULL,11);
    }else if(enterType == 3)
    {
		//        SGMainManager::shareMain()->showEmbattleLayer(1);
    }else if(enterType == 2)
    {
        SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
        SGMainManager::shareMain()->showMessage(player->getPlayerUnlockTip()->getCString());
    }else if(enterType == 1)
    {
		//        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getEmbattleOfficers();
        int id = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
        CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(id);
        int totleGovern = 0;
        CCObject *obj = NULL;
        CCARRAY_FOREACH(array, obj)
        {
            SGOfficerCard *card = (SGOfficerCard *)obj;
			
            totleGovern += card->getGoverning();
            if (card->getOfficerEquips())
            {
                for (int i = 0; i <card->getOfficerEquips()->count(); i++)
                {
                    SGEquipCard *equip = (SGEquipCard *)card->getOfficerEquips()->objectAtIndex(i);
                    if ((card->getGoverning()+equip->getGoverning())<0) {
                        totleGovern -= card->getGoverning();
                    }
                    else{
                        totleGovern += equip->getGoverning();
                    }
                }
            }
        }
         SGPlayerDataModel *player=SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
        int aviaiblePosition=player->getPlayerOfficerMax()-array->count();
        
        int teamid = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
        SGArrangeLayer *layer = SGArrangeLayer::create(index/120 + 1,totleGovern,10,teamid,aviaiblePosition);
        layer->setComBattle(SGPlayerInfo::sharePlayerInfo()->getcombatValue());
        SGMainManager::shareMain()->showLayer(layer);
        SGMainManager::shareMain()->showblack();
	}
}

void SGFirstSubLayer::selectSubview()
{
	
}

void SGFirstSubLayer::setiscantouch(bool iscan)
{
    iscantouch = iscan;
    setTouchEnabled(iscan);
    isMove = false;
    
    if (getChildByTag(124)) {
        ((CCMenu *)getChildByTag(124))->setEnabled(iscan);
    }
    if (getChildByTag(125)) {
        ((CCMenu *)getChildByTag(125))->setEnabled(iscan);
    }
    
    if (enterType == 3 && getChildByTag(123)) {
        ((CCMenu *)getChildByTag(123))->setEnabled(iscan);
    }
}