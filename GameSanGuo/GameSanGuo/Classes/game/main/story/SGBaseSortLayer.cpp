//
//  SGBaseSortLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-29.
//
//

#include "SGBaseSortLayer.h"
#include "SGStoryBaseCell.h"
#include "ResourceManager.h"
#include <algorithm>
using namespace std;

SGBaseSortLayer::SGBaseSortLayer(void)
{
    _datas = NULL;
    _delegate = NULL;
    _delegateTouch = NULL;
}
SGBaseSortLayer::~SGBaseSortLayer(void)
{
	if (_datas)
    {
        CC_SAFE_RELEASE(_datas);
    }
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_sortLayer);
 
}

void SGBaseSortLayer::initView(CCArray *datas, vector<string> btns, vector<int> tags, SGBaseSortLayerDelegate *delegate, SGBoxDelegate *canTouch, CCPoint position)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    _datas = datas;
    if(_datas) _datas->retain();
    
    _delegate = delegate;
    _delegateTouch = canTouch;
    _delegateTouch->setIsCanTouch(false);

 
	
	ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_sortLayer, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_sortLayer, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgtrenglayer/sgtrenglayer.plist", RES_TYPE_LAYER_UI, sg_sortLayer);
	 
	
 
    CCSprite *spriteBlackbg = CCSprite::createWithSpriteFrameName("public_blackBoard.png");
    spriteBlackbg->setPosition(SGLayout::getPoint(kMiddleCenter));
    spriteBlackbg->setScaleX(size.width/spriteBlackbg->getContentSize().width);
    spriteBlackbg->setScaleY(size.height/spriteBlackbg->getContentSize().height);
    this->addChild(spriteBlackbg);
    
    SGButton *rangeBtn = SGButton::create("chatbtn.png", "public_range.png", this,
                                       menu_selector(SGBaseSortLayer::buttonSortCancel), ccp(0, 0), false, true);
    rangeBtn->setPosition(position);
    this->addBtn(rangeBtn);
    
    // init background
    bgPosition = ccpAdd(position, ccp(rangeBtn->getContentSize().width/2, -rangeBtn->getContentSize().height/2));
    this->initBackground(btns.size());
    
    // init buttons
    this->initButtons(btns, tags);
    
    // init cancel button
    SGButton *cancelBtn = SGButton::create("public_btn_6.png", "public_font_qixiao.png", this,
                                           menu_selector(SGBaseSortLayer::buttonSortCancel), CCPointZero, false, true);
    cancelBtn->setPosition(ccpAdd(bgPosition, ccp(-bgSize.width/2, -ZoomInX(40/2)*1.6f*(btns.size()+1))));
    this->addBtn(cancelBtn);
}

void SGBaseSortLayer::initBackground(int btnCount)
{
    CCRect rect = CCRectMake(0, 0, ZoomInX(212.0f/2), (btnCount+1)/8.0f * ZoomInX(622.0f/2));
    //CCSprite *spriteBg = CCSprite::createWithSpriteFrameName("barrack_sort_bg.png", rect);
    CCSprite *spriteBg = CCSprite::createWithSpriteFrameName("barrack_sort_bg.png");
    spriteBg->setAnchorPoint(ccp(1,1));
    spriteBg->setPosition(bgPosition);
    this->addChild(spriteBg);
    bgSize = spriteBg->getContentSize();
    
    //四条线
    CCSprite *spriteLine1 = CCSprite::createWithSpriteFrameName("gonggao_xian.png"); // 上
    CCSprite *spriteLine2 = CCSprite::createWithSpriteFrameName("gonggao_xian.png"); // 下
    CCSprite *spriteLine3 = CCSprite::createWithSpriteFrameName("playerinfo_Lbian.png"); // 左
    CCSprite *spriteLine4 = CCSprite::createWithSpriteFrameName("playerinfo_Rbian.png"); // 右
    spriteLine1->setAnchorPoint(ccp(0.5, 1));
    spriteLine1->setPosition(ccpAdd(bgPosition, ccp(-bgSize.width/2, 0)));
    spriteLine1->setScaleX(bgSize.width/spriteLine1->getContentSize().width);
    spriteLine2->setAnchorPoint(ccp(0.5, 0));
    spriteLine2->setPosition(ccpAdd(bgPosition, ccp(-bgSize.width/2, -bgSize.height)));
    spriteLine2->setScaleX(bgSize.width/spriteLine2->getContentSize().width);
    spriteLine3->setAnchorPoint(ccp(0, 0.5));
    spriteLine3->setPosition(ccpAdd(bgPosition, ccp(-bgSize.width, -bgSize.height/2)));
    spriteLine3->setScaleY(bgSize.height/spriteLine3->getContentSize().height);
    spriteLine4->setAnchorPoint(ccp(1, 0.5));
    spriteLine4->setPosition(ccpAdd(bgPosition, ccp(0, -bgSize.height/2)));
    spriteLine4->setScaleY(bgSize.height/spriteLine4->getContentSize().height);
    this->addChild(spriteLine1);
    this->addChild(spriteLine2);
    this->addChild(spriteLine3);
    this->addChild(spriteLine4);
    
    //四个角
    CCSprite *spriteBG_leftAngle_1 =CCSprite::createWithSpriteFrameName("barrack_sort_jiao.png"); // 左上角
    CCSprite *spriteBG_leftAngle_2 = CCSprite::createWithSpriteFrameName("barrack_sort_jiao.png"); // 右上角
    CCSprite *spriteBG_rightAngle_1 = CCSprite::createWithSpriteFrameName("barrack_sort_jiao.png"); // 左下角
    CCSprite *spriteBG_rightAngle_2 = CCSprite::createWithSpriteFrameName("barrack_sort_jiao.png"); // 右下角
    spriteBG_leftAngle_1->setAnchorPoint(ccp(0, 0));
    spriteBG_leftAngle_1->setRotation(90);
    spriteBG_leftAngle_2->setAnchorPoint(ccp(0, 0));
    spriteBG_rightAngle_1->setAnchorPoint(ccp(0, 0));
    spriteBG_rightAngle_1->setRotation(180);
    spriteBG_rightAngle_2->setAnchorPoint(ccp(0, 0));
    spriteBG_rightAngle_2->setRotation(270);
    
    spriteBG_leftAngle_1->setPosition(ccpAdd(bgPosition, ccp(-bgSize.width, 0)));
    spriteBG_leftAngle_2->setPosition(ccpAdd(bgPosition, ccp(-bgSize.width, -bgSize.height)));
    spriteBG_rightAngle_1->setPosition(bgPosition);
    spriteBG_rightAngle_2->setPosition(ccpAdd(bgPosition, ccp(0, -bgSize.height)));
    this->addChild(spriteBG_leftAngle_1);
    this->addChild(spriteBG_leftAngle_2);
    this->addChild(spriteBG_rightAngle_1);
    this->addChild(spriteBG_rightAngle_2);
}

void SGBaseSortLayer::initButtons(vector<string> btns, vector<int> tags)
{

	//CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("barrack/1.plist");

	for (int ii = 0; ii < btns.size(); ii++) {
//        CCLOG("tt================%s", btns[ii].c_str());
        SGButton *sortBtn = SGButton::create("barrack_sort_buttonbg.png", btns[ii].c_str(), this,
                                             menu_selector(SGBaseSortLayer::buttonSortAction), CCPointZero, false, true);
        sortBtn->setPosition(ccpAdd(bgPosition, ccp(-bgSize.width/2, -ZoomInX(40/2)*1.6f*(ii+1))));
        sortBtn->setTag(tags[ii]);
        this->addBtn(sortBtn);
    }
}

void SGBaseSortLayer::buttonSortCancel()
{
    EFFECT_PLAY(MUSIC_BTN);
    _delegateTouch->setIsCanTouch(true);
    this->removeFromParentAndCleanup(true);
}

void SGBaseSortLayer::buttonSortAction(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    // do your sorting
    int index = ((CCMenuItem *)sender)->getTag();
    this->sortAction(index);
    
    _delegate->sortLayerCallBack(_datas);
    _delegateTouch->setIsCanTouch(true);
    this->removeFromParentAndCleanup(true);
}

#pragma mark - --------sort action TODO----------
// 子类实现compare定义排序规则
void SGBaseSortLayer::doSort(bool (*compare)(const void*, const void*))
{
    CCLog("==================dosort");
    if (!_datas || _datas->count()==0) {
        return;
    }
    
    std::sort(_datas->data->arr, _datas->data->arr+_datas->data->num, compare);
}

// 子类实现sortAction方法来绑定排序规则
void SGBaseSortLayer::sortAction(int index)
{
    CCLOG("tt================sort action");
}







