//
//  SGStorySpeakerLayer.cpp
//  GameSanGuo
//
//  Created by lizi on 13-5-19.
//
//

#include "SGStorySpeakerLayer.h"
#include "SGStaticDataManager.h"
#include "SGMainManager.h"
#include "SGGuideManager.h"
#include "SGTestSwitch.h"
#include "SGBattleManager.h"
#include "ResourceManager.h"
SGStorySpeakerLayer::SGStorySpeakerLayer(void)
{
    _wordIndex = 0;
    _datas = NULL;
    _delegateTouch = NULL;
    
    label = NULL;
    speaker = NULL;
    bboard = NULL;
    spriteBlackbg = NULL;
}
SGStorySpeakerLayer::~SGStorySpeakerLayer(void)
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_storyspeakLayer);
    CC_SAFE_DELETE(_datas);
}

SGStorySpeakerLayer* SGStorySpeakerLayer::create()
{
    SGStorySpeakerLayer *model = new SGStorySpeakerLayer();
    if (model && model->init(NULL, sg_storyspeakLayer)) {
        model->autorelease();
        model->initView();
        return model;
    }
    CC_SAFE_DELETE(model);
    return NULL;
}

void SGStorySpeakerLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, false);
}

void SGStorySpeakerLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist", RES_TYPE_LAYER_UI, sg_storyspeakLayer);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_storyspeakLayer, LIM_PNG_AS_PNG);
    
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    spriteBlackbg = CCSprite::createWithSpriteFrameName("public_blackBoard.png");

    spriteBlackbg->setPosition(SGLayout::getPoint(kMiddleCenter));
    spriteBlackbg->setScaleX(size.width/spriteBlackbg->getContentSize().width);
    spriteBlackbg->setScaleY(size.height/spriteBlackbg->getContentSize().height);
    this->addChild(spriteBlackbg, -1);
    
    float bottomH = skewing(162.0f/2);
    this->setTouchEnabled(false);
    
    // init black board
    bboard = CCSprite::createWithSpriteFrameName("guide_board_new.png");

    float posH = bboard->getContentSize().height;
    bboard->setScaleX(size.width/bboard->getContentSize().width);
    bboard->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH+posH/2)));
    this->addChild(bboard, 1);
    
    // init speaker
//    speaker = CCSprite::create("public/public_guide_mm.png");
    speaker = CCSprite::createWithSpriteFrameName("public_guide_mm.png");

    posH += speaker->getContentSize().height/2;
    float spWid = speaker->getContentSize().width;
    speaker->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(-spWid*0.32f, bottomH+posH)));
    this->addChild(speaker);
    
    // init label
    label = CCLabelTTF::create("", FONT_PENGYOU , FONTSIZE(14));
    CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
    label->setDimensions(CCSizeMake(tsize.width-skewing(15), tsize.height-skewing(15)));
    label->setPosition(bboard->getPosition());
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    this->addChild(label, 2);
}

void SGStorySpeakerLayer::setData(int plotId)
{
    SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
    CCArray *array = manager->getArrayById(plotId);
    _datas = CCArray::create();
    _datas->retain();
    if (array != NULL) {
        for (int ii = 0; ii < array->count(); ii++) {
            DDLog("%d--%d--------------%s", ii, array->count(), array->objectAtIndex(ii));
            _datas->addObject(array->objectAtIndex(ii));
        }
        if (_datas != NULL) {
            if (!_datas->count())return;
            CCString *word = ((CCString *)_datas->objectAtIndex(0));
            label->setString(word->getCString());
        }
    }
    else
    {
        ccTouchEnded(NULL,NULL);
    }
}

bool SGStorySpeakerLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void SGStorySpeakerLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (_datas != NULL) {
        _wordIndex += 1;
        int count = _datas->count();
        
        if (_wordIndex < count) {
            CCString *str = (CCString *)_datas->objectAtIndex(_wordIndex);
            label->setString(str->getCString());
        }else {
            SGGuideManager::shareManager()->closeSpeaker();
        }
    }
}