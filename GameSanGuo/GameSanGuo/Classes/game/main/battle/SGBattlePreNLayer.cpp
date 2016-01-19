//
//  SGBattlePreNLayer.cpp
//  GameSanGuo
//
//  Created by lizi on 13-6-28.
//
//

#include "PlatformAdp.h"
#include "SGBattlePreNLayer.h"
#include "SGPlayerInfo.h"
#include "SGStaticDataManager.h"
#include "SGAIManager.h"
#include "SGTestSwitch.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGBattleManager.h"
#include "SGMainManager.h"
#if (PLATFORM == IOS)
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#elif (PLATFORM == ANDROID)
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif
#include "ResourceManager.h"
#include <string.h>

#if (PLATFORM == IOS)

CCHLSprite::CCHLSprite(void)
{
}
CCHLSprite::~CCHLSprite(void)
{
 
}
CCHLSprite* CCHLSprite::create(const char *fileName)
{
    CCHLSprite *pSprite = new CCHLSprite();
    if (pSprite && pSprite->initWithFile(fileName)) {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}
void CCHLSprite::setHighLight(bool highLight)
{
    ccBlendFunc func = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
    setBlendFunc(func);
}
void CCHLSprite::draw(void)
{
//    CCNode::draw();
    
    CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");
	
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Unneeded states: -
	
	bool newBlend = false;
	if( m_sBlendFunc.src != CC_BLEND_SRC || m_sBlendFunc.dst != CC_BLEND_DST ) {
		newBlend = true;
        glBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
	}
	
    
#define kQuadSize sizeof(m_sQuad.bl)
	if (m_pobTexture != NULL){
        glBindTexture(GL_TEXTURE_2D, m_pobTexture->getName());
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (true) {
        float tinting[4] = {getColor().r/255.0f, getColor().g/255.0f, getColor().b/255.0f, getOpacity()/255.0f};
        glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, tinting);
        
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB,      GL_INTERPOLATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB,         GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB,         GL_CONSTANT);
        glTexEnvi(GL_TEXTURE_ENV, GL_SRC2_RGB,         GL_CONSTANT);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB,	  GL_ONE_MINUS_SRC_ALPHA);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_SUBTRACT);
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
	long offset = (long)&m_sQuad;
    
    // vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexPointer(3, GL_FLOAT, kQuadSize, (void*) (offset + diff) );
	
	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	glColorPointer(4, GL_UNSIGNED_BYTE, kQuadSize, (void*)(offset + diff));
	
	// tex coords
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glTexCoordPointer(2, GL_FLOAT, kQuadSize, (void*)(offset + diff));
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	if( newBlend )
		glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
    
#if CC_SPRITE_DEBUG_DRAW
	CGSize s = [self contentSize];
	CGPoint vertices[4]={
		ccp(0,0),ccp(s.width,0),
		ccp(s.width,s.height),ccp(0,s.height),
	};
	ccDrawPoly(vertices, 4, YES);
#endif // CC_TEXTURENODE_DEBUG_DRAW
	
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB,      GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB,         GL_PREVIOUS);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
	
	glBlendFunc( CC_BLEND_SRC, CC_BLEND_DST );
}

#endif

#pragma mark -
#pragma mark - -----------OfficerSprite Define-----------

OfficerSprite::OfficerSprite(void)
{
}
OfficerSprite::~OfficerSprite(void)
{

}
OfficerSprite* OfficerSprite::create(const char* name, int roleId, int roleType, bool isMoff)
{
    OfficerSprite *pSprite = new OfficerSprite();
    if (pSprite && pSprite->init()) {
        
        pSprite->initViewWithInfo(name, roleId, roleType, isMoff);
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}
void OfficerSprite::initViewWithInfo(const char* name, int roleId, int level, bool isMoff)
{
    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(roleId);
    int currStarLevel = 2;
    if (data) {
        int level = data->getOfficerCurrStarLevel();
        
        if (level > currStarLevel) {
            currStarLevel = level;
        }
        
    }
    
    
    CCString *fileName = CCString::createWithFormat("load_ctyIconBbg_%d.png", currStarLevel-1);
    CCSprite *spBg = CCSprite::createWithSpriteFrameName(fileName->getCString());
//    CCSprite *spBg = CCSprite::create(fileName->getCString());//gai
    this->addChild(spBg, -1);
    
    if (data) {
        SGMainManager::shareMain() -> addVSHeadPlistByNum(data->getOfficerHead(),sg_battlePreLayer);
        fileName = CCString::createWithFormat("vs_head_%d.png", data->getOfficerHead());
        CCSprite *spMain = CCSprite::createWithSpriteFrameName(fileName->getCString());
        if (spMain) {
            this->addChild(spMain,-2);
        }
   }

    fileName = CCString::createWithFormat("load_ctyIconBbg_6.png");
    CCSprite *spbgg = CCSprite::createWithSpriteFrameName(fileName->getCString());
    this->addChild(spbgg,-3);
    
//    fileName = CCString::createWithFormat("sceneLoad/load_ctyIconSbg_%d.png", currStarLevel);
    fileName = CCString::createWithFormat("load_ctyIconsbg_%d.png", currStarLevel-1);
    CCSprite *labelBg = CCSprite::createWithSpriteFrameName(fileName->getCString());
//    CCSprite *labelBg = CCSprite::create(fileName->getCString());//gai
    float posX = spBg->getContentSize().width*.5f - labelBg->getContentSize().width*.55f;//.82
    float posY = -spBg->getContentSize().height*.5f + labelBg->getContentSize().height*.5f;//.58
    labelBg->setPosition(ccp(posX, posY));
    this->addChild(labelBg);
    CCLabelTTF *label = CCLabelTTF::create(CCString::createWithFormat("LV:%d", level)->getCString(), FONT_PANGWA, 20);//上下左右四个边和中间的数字,因为SG的不能用
    this->addChild(label);
    label->setAnchorPoint(ccp(0, 0.5));
    label->setPosition(ccp(-spBg->getContentSize().width * 0.45, spBg->getContentSize().height * 0.45+2));
    label->setColor(ccBLACK);
    CCLabelTTF *label1 = CCLabelTTF::create(CCString::createWithFormat("LV:%d", level)->getCString(), FONT_PANGWA, 20);
    this->addChild(label1);
    label1->setAnchorPoint(ccp(0, 0.5));
    label1->setPosition(ccp(-spBg->getContentSize().width * 0.45, spBg->getContentSize().height * 0.45-2));
    label1->setColor(ccBLACK);

    CCLabelTTF *label2 = CCLabelTTF::create(CCString::createWithFormat("LV:%d", level)->getCString(), FONT_PANGWA, 20);
    this->addChild(label2);
    label2->setAnchorPoint(ccp(0, 0.5));
    label2->setPosition(ccp(-spBg->getContentSize().width * 0.45+2, spBg->getContentSize().height * 0.45));
    label2->setColor(ccBLACK);

    CCLabelTTF *label3 = CCLabelTTF::create(CCString::createWithFormat("LV:%d", level)->getCString(), FONT_PANGWA, 20);
    this->addChild(label3);
    label3->setAnchorPoint(ccp(0, 0.5));
    label3->setPosition(ccp(-spBg->getContentSize().width * 0.45-2, spBg->getContentSize().height * 0.45));
    label3->setColor(ccBLACK);

    CCLabelTTF *label4 = CCLabelTTF::create(CCString::createWithFormat("LV:%d", level)->getCString(), FONT_PANGWA, 20);
    this->addChild(label4);
    label4->setAnchorPoint(ccp(0, 0.5));
    label4->setPosition(ccp(-spBg->getContentSize().width * 0.45, spBg->getContentSize().height * 0.45));

    
    if (data->getAdvNum() > 0) {//
        
        
        CCLabelTTF *advNum1 = CCLabelTTF::create(CCString::createWithFormat("+%d",data->getAdvNum())->getCString(), FONT_PANGWA, 24);
        advNum1->setColor(ccBLACK);
        advNum1->setPosition(ccp(spBg->getContentSize().width*0-30, -spBg->getContentSize().height * 0.40-15-2));
        addChild(advNum1);
        
        CCLabelTTF *advNum2 = CCLabelTTF::create(CCString::createWithFormat("+%d",data->getAdvNum())->getCString(), FONT_PANGWA, 24);
        advNum2->setColor(ccBLACK);
        advNum2->setPosition(ccp(spBg->getContentSize().width*0-30+2, -spBg->getContentSize().height * 0.40-15));
        addChild(advNum2);
        
        CCLabelTTF *advNum3 = CCLabelTTF::create(CCString::createWithFormat("+%d",data->getAdvNum())->getCString(), FONT_PANGWA, 24);
        advNum3->setColor(ccBLACK);
        advNum3->setPosition(ccp(spBg->getContentSize().width*0-30-2, -spBg->getContentSize().height * 0.40-15));
        addChild(advNum3);
        
        CCLabelTTF *advNum4 = CCLabelTTF::create(CCString::createWithFormat("+%d",data->getAdvNum())->getCString(), FONT_PANGWA, 24);
        advNum4->setColor(ccBLACK);
        advNum4->setPosition(ccp(spBg->getContentSize().width*0-30, -spBg->getContentSize().height * 0.40-15+2));
        addChild(advNum4);
        
        CCLabelTTF *advNum = CCLabelTTF::create(CCString::createWithFormat("+%d",data->getAdvNum())->getCString(), FONT_PANGWA, 24);
        advNum->setColor(ccGREEN);
        advNum->setPosition(ccp(spBg->getContentSize().width*0-30, -spBg->getContentSize().height * 0.40-15));
        addChild(advNum);
        
        
        
        
    }
    
    
//    float wid = 334 / 8.0f, hit = 218 / 2.0f;
//    if (roleId == 54 || roleId == 55) {
//        int xIndex = 0, yIndex = 0;
//        
//        strcmp(name, "芙蓉")==0 && (xIndex=0, yIndex=0);
//        strcmp(name, "吕雯")==0 && (xIndex=1, yIndex=0);
//        strcmp(name, "张梁")==0 && (xIndex=2, yIndex=0);
//        strcmp(name, "周玉")==0 && (xIndex=3, yIndex=0);
//        strcmp(name, "诸葛琪")==0 && (xIndex=4, yIndex=0);
//        strcmp(name, "鲍信")==0 && (xIndex=5, yIndex=0);
//        strcmp(name, "韩浩")==0 && (xIndex=6, yIndex=0);
//        strcmp(name, "蒋琬")==0 && (xIndex=7, yIndex=0);
//        
//        strcmp(name, "吕岱")==0 && (xIndex=0, yIndex=1);
//        strcmp(name, "罗宪")==0 && (xIndex=1, yIndex=1);
//        strcmp(name, "孙桓")==0 && (xIndex=2, yIndex=1);
//        strcmp(name, "孙礼")==0 && (xIndex=3, yIndex=1);
//        strcmp(name, "孙韶")==0 && (xIndex=4, yIndex=1);
//        strcmp(name, "孙瑜")==0 && (xIndex=5, yIndex=1);
//        strcmp(name, "王昶")==0 && (xIndex=6, yIndex=1);
//        strcmp(name, "朱治")==0 && (xIndex=7, yIndex=1);
//        DDLog("cc=====================%s---=%d==%d, %d===%d", name, roleId, xIndex, yIndex, strcmp(name, "芙蓉"));
//        CCRect rect = CCRectMake(wid*xIndex, hit*yIndex, wid, hit);
//        CCSprite *label = CCSprite::create("sceneLoad/load_ctyAllName.png", rect);
    if (data) {
        CCLabelTTF *xx = CCLabelTTF::create(data->getOfficerName()->getCString(),FONT_BOXINFO, 24,CCSizeMake(24,0),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
        xx->setColor(ccBLACK);
        if (xx->getContentSize().height>=90) {
            xx->setScaleY(90/xx->getContentSize().height);
        }
        xx->setPosition(labelBg->getPosition());
        this->addChild(xx,1);
    }

//    xx->setOutSideColor(ccWHITE);
//    xx->setInsideColor(ccBLACK);

//    xx->setContentSize(CCSize(0,labelBg->getContentSize().height));
//    CCLabelTTF::create(const char *string, const char *fontName, float fontSize, const cocos2d::CCSize &dimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment)
//    CCLabelTTF *xx = CCLabelTTF::create(data->getOfficerName()->getCString(),FONT_BOXINFO, 24,CCSizeMake(24,labelBg->getContentSize().height),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);

  
    
//    }
//    this->addChild(labelBg);
    this->setContentSize(spBg->getContentSize());
    //CCSpriteFrameCache::sharedSpriteFrameCache() -> addSpriteFramesWithFile("battle/battle.plist");

    
    if (isMoff==true) {
        CCSprite *header = CCSprite::createWithSpriteFrameName("battle_zj.png");
        posX = spBg->getContentSize().width*.5f - header->getContentSize().width*.25f;
        posY = spBg->getContentSize().height*.5f - header->getContentSize().height*.25f;
        header->setPosition(ccp(posX, posY));
        this->addChild(header);
    }
}

void OfficerSprite::setOpacity(GLubyte opacity)
{
    CCSprite::setOpacity(opacity);
    for (int ii = 0; ii < this->getChildrenCount(); ii++) {
        CCSprite *child = (CCSprite *)this->getChildren()->objectAtIndex(ii);
        child->setOpacity(opacity);
    }
}

#pragma mark -
#pragma mark - -----------SGBattlePreNLayer Define-----------

SGBattlePreNLayer::SGBattlePreNLayer(void)
{
    IconWid = 0;
    IconHig = 0;
    vsImg = NULL;
    
    headIconD = NULL;
    headIconU = NULL;
    
    selfName = NULL;
    armyName = NULL;
    selfCountry = NULL;
    armyCountry = NULL;
    
    selfCtyBg = NULL;
    armyCtyBg = NULL;
    
    selfArray = NULL;
    armyArray = NULL;
    selfLArray = NULL;
    armyLArray = NULL;
    selfBArray = NULL;
    armyBArray = NULL;
    aniArray = NULL;
    aniArrayN = NULL;
    

    
}
SGBattlePreNLayer::~SGBattlePreNLayer(void)
{
    //MM 14.11.19 这里原来为CC_SAFE_DELETE，什么情况？
    CC_SAFE_RELEASE(selfArray);
    CC_SAFE_RELEASE(armyArray);
    CC_SAFE_RELEASE(selfLArray);
    CC_SAFE_RELEASE(armyLArray);
    CC_SAFE_RELEASE(selfBArray);
    CC_SAFE_RELEASE(armyBArray);
    CC_SAFE_RELEASE(aniArrayN);
    CC_SAFE_RELEASE(aniArray);
    CC_SAFE_RELEASE(selfNumArray);
    CC_SAFE_RELEASE(armyNumArray);

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_battlePreLayer);
}
SGBattlePreNLayer* SGBattlePreNLayer::create(SGLoadData *self, SGLoadData *army)
{
    SGBattlePreNLayer *layer = new SGBattlePreNLayer();
    if (layer &&  layer->init(NULL, sg_battlePreLayer)) {
        layer->initView(self, army);
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void SGBattlePreNLayer::initBgImgs()
{

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    ResourceManager::sharedInstance()->bindTexture("sgbattlemap/battle_loadbg.plist", RES_TYPE_LAYER_UI, sg_battlePreLayer);
    
    CCSprite *bgImgUp = CCSprite::createWithSpriteFrameName("battle_loadbg.png");
    CCSprite *bgImgDown = CCSprite::createWithSpriteFrameName("battle_loadbg.png");
    if (bgImgUp) {
        bgImgUp->setScaleX(size.width/bgImgUp->getContentSize().width * 1.01);
        bgImgUp->setScaleY(size.height*.5f/bgImgUp->getContentSize().height * 1.01);
        bgImgUp->setPosition(ccp(size.width/2 , size.height*.75f));
        this->addChild(bgImgUp);
        bgImgDown->setFlipX(true);
        bgImgDown->setScaleX(size.width/bgImgDown->getContentSize().width * 1.01);
        bgImgDown->setScaleY(size.height*.5f/bgImgDown->getContentSize().height * 1.01);
        bgImgDown->setPosition(ccp(size.width/2 , size.height*.25f));
        this->addChild(bgImgDown);
    }
    
//    vsImg = CCSprite::create("sanguobigpic/battle_loadvs.png", CCRect((768-size.width)*.5, 0, size.width, 563));
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/battle_loadvs.plist", RES_TYPE_LAYER_UI, sg_battlePreLayer);

    vsImg = CCSprite::createWithSpriteFrameName("battle_loadvs.png");
//    float scaleXY = size.width/vsImg->getContentSize().width;
//    vsImg->setScaleY(0);
//    vsImg->setOpacity(0);
   // vsImg->setPosition(ccp(-size.width/2*3 , size.height/2));
    vsImg->setPosition(ccp(size.width * 0.7 , size.height*.52f));
    this->addChild(vsImg);
}

void SGBattlePreNLayer::initView(SGLoadData *self, SGLoadData *army)
{
    ResourceManager::sharedInstance()->bindTexture("sgbattlemap/sgbattlemap.plist", RES_TYPE_BATTLE_IMAGE, sg_battlePreLayer);
    ResourceManager::sharedInstance()->bindTexture("loadscene/loadscene2.plist", RES_TYPE_BATTLE_IMAGE, sg_battlePreLayer);
    ResourceManager::sharedInstance()->bindTexture("loadscene/loadscene3.plist", RES_TYPE_BATTLE_IMAGE, sg_battlePreLayer);
    ResourceManager::sharedInstance()->bindTexture("loadscene/load_dismiss.plist", RES_TYPE_BATTLE_IMAGE, sg_battlePreLayer);
    // 敌军在上，自己在下放
 
   
	this->initBgImgs();
    
    this->initArrays();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    CCString *str = NULL;
    // init player headicon
    str = CCString::createWithFormat("load_ctyIcon_%d.png", army->getRoleId());
    headIconU = CCSprite::createWithSpriteFrameName(str->getCString());
//    headIconU = CCSprite::create(str->getCString());//gai
    str = CCString::createWithFormat("load_ctyIcon_%d.png", self->getRoleId());
    headIconU->setScale(1.0);
    headIconD = CCSprite::createWithSpriteFrameName(str->getCString());
//    headIconD = CCSprite::create(str->getCString());//gai
    headIconD->setFlipX(true);
    headIconD->setScale(1.0);
    
//    std::string servername = CCUserDefault::sharedUserDefault()->getStringForKey("servername");
//    std::string serverid = CCUserDefault::sharedUserDefault()->getStringForKey("serverid");
    str = CCString::createWithFormat("%s",self->getRoleName()->getCString()/*,serverid.c_str(),servername.c_str()*/);
    selfName = CCLabelTTF::create(str->getCString(), FONT_BOXINFO, 26);
    armyName = CCLabelTTF::create(army->getRoleName()->getCString(), FONT_BOXINFO, 26);
    str = CCString::createWithFormat("load_ctyName_%d.png", self->getRoleType());
    selfCountry = CCSprite::createWithSpriteFrameName(str->getCString());//gai
//    selfCountry = CCSprite::create(str->getCString());//gai
    str = CCString::createWithFormat("load_ctyName_%d.png", army->getRoleType());
    armyCountry = CCSprite::createWithSpriteFrameName(str->getCString());//gai
//    armyCountry = CCSprite::create(str->getCString());//gai
    
    str = CCString::createWithFormat("battle_ui_bg.png");
    selfCtyBg = CCSprite::createWithSpriteFrameName(str->getCString());

    armyCtyBg = CCSprite::createWithSpriteFrameName(str->getCString());//gai
    
    float headWid = headIconU->getContentSize().width;
    float ctyWid = selfCtyBg->getContentSize().width;
    float ctyHit = selfCtyBg->getContentSize().height;
    
    CCSprite *nameUbg = CCSprite::createWithSpriteFrameName("load_ctyNameBg.png");
    nameUbg->setPosition(ccp(ctyWid*.75f, ctyHit*.5f));
    nameUbg->setScaleX(1.3);
    armyCtyBg->addChild(nameUbg);
    CCSprite *nameDbg = CCSprite::createWithSpriteFrameName("load_ctyNameBg.png");
    nameDbg->setPosition(ccp(ctyWid*.25f, ctyHit*.5f));
    nameDbg->setScaleX(1.3);
    selfCtyBg->addChild(nameDbg);

    armyName->setHorizontalAlignment(kCCTextAlignmentCenter);
    armyName->setPosition(nameUbg->getPosition());
    armyName->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    armyName->setDimensions(nameUbg->getContentSize());
    selfName->setHorizontalAlignment(kCCTextAlignmentCenter);
    selfName->setPosition(nameDbg->getPosition());
    selfName->setDimensions(nameDbg->getContentSize());
    selfName->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    
    float posyy = 344*.5f - armyCtyBg->getContentSize().height*.52f;
    selfCtyBg->setPosition(ccp(-size.width*.5f*3, size.height/2-posyy));
    armyCtyBg->setPosition(ccp(size.width*.5f*3, size.height/2+posyy));
    
    headIconU->setAnchorPoint(ccp(0.5f, 1));
    headIconD->setAnchorPoint(ccp(0.5f, 0));
    headIconU->setPosition(ccp(headWid*.48f -20, ctyHit*.75f + 100));
    headIconD->setPosition(ccp(ctyWid-headWid*.42f, ctyHit*.25f - 15));
    armyCountry->setPosition(ccp(headWid, ctyHit*.5f));//.3
    selfCountry->setPosition(ccp(ctyWid-headWid, ctyHit*.5f));//.75
    
    armyCtyBg->addChild(headIconU);
    armyCtyBg->addChild(armyName);
    armyCtyBg->addChild(armyCountry);
    
    selfCtyBg->addChild(headIconD);
    selfCtyBg->addChild(selfName);
    selfCtyBg->addChild(selfCountry);

    
    this->addChild(armyCtyBg, 1000);
    this->addChild(selfCtyBg, 1000);

    
    SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
    
    int scount = army->getSoldiers()->count();
    for (int ii = 0; ii < scount; ii++) {
        
        CCString *id = (CCString *)army->getSoldiers()->objectAtIndex(ii);
        
        int index = id->m_sString.find(":");
        CCLOG("id->m_sString.substr(0, index - 1).c_str()==%s",id->m_sString.substr(0, index).c_str());
        CCLOG("id->m_sString.substr(index + 1, id->m_sString.length()).c_str()=%s",id->m_sString.substr(index + 1, id->m_sString.length()).c_str());
        //        std::string ss = name->m_sString.substr(name->m_sString.find("1"), name->m_sString.length());
        std::string sbid = id->m_sString.substr(0, index);
        std::string level = id->m_sString.substr(index + 1, id->m_sString.length());
        CCString *str = CCString::create(sbid);
        
        CCString *str1 = CCString::create(level);
        
        SGOfficerDataModel *model = manager->getOfficerById(str->intValue());
        const char *sname = model->getOfficerName()->getCString();
        bool isMainOff = false;
        if (ii == army->getMIndex()-1)
            isMainOff = true;
        OfficerSprite *sprite = OfficerSprite::create(sname, /*model->getOfficerHead()*/str->intValue(), str1->intValue(), isMainOff);
        if (IconHig == 0 && IconWid ==0) {
            IconWid = sprite->getContentSize().width;
            IconHig = sprite->getContentSize().height;
        }
        float posXX = size.width*.5f+IconWid*.62f-IconWid*(ii%5-2);//.62f
        float posYY = size.height*.5f+IconHig*1.45;
         if (ii%2==0)
            posYY -= IconHig;
        else
            posYY += IconHig;
        CCPoint selfPos;
        if (ii<5) {
            selfPos = ccp(posXX-(ii*8), posYY);
        }
        
        else
        {
           selfPos = ccp(posXX-((ii-5)*8), posYY);
        }
        sprite->setPosition(selfPos);
        sprite->setOpacity(0);
        this->addChild(sprite);
        armyArray->addObject(sprite);
    }
    
    int acount = self->getSoldiers()->count();
    for (int ii = 0; ii < acount; ii++) {
        CCString *id = (CCString *)self->getSoldiers()->objectAtIndex(ii);
        int index = id->m_sString.find(":");
        CCLOG("id->m_sString.substr(0, index - 1).c_str()==%s",id->m_sString.substr(0, index).c_str());
        CCLOG("id->m_sString.substr(index + 1, id->m_sString.length()).c_str()=%s",id->m_sString.substr(index + 1, id->m_sString.length()).c_str());
        //        std::string ss = name->m_sString.substr(name->m_sString.find("1"), name->m_sString.length());
        std::string sbid = id->m_sString.substr(0, index).c_str();
        std::string level = id->m_sString.substr(index + 1, id->m_sString.length()).c_str();
        CCString *str = CCString::create(sbid.c_str());
        
        CCString *str1 = CCString::create(level.c_str());

        
        SGOfficerDataModel *model = manager->getOfficerById(str->intValue());
        const char *sname = model->getOfficerName()->getCString();
        bool isMainOff = false;
        if (ii == self->getMIndex()-1)
            isMainOff = true;
        float posXX = size.width*.5f-IconWid*(2-0.39f)+IconWid*(ii%5);//-.62,-.39
        float posYY = size.height*.5f-IconHig*.49f;
        
        if (ii%2==0)
            posYY -= IconHig;
        else
            posYY += IconHig;
        
        OfficerSprite *sprite = OfficerSprite::create(sname, /*model->getOfficerHead()*/str->intValue(), str1->intValue(), isMainOff);
        CCPoint armyPos;
        if (ii<5) {
            armyPos = ccp(posXX+(ii*8), posYY);
        }
        else
        {
          armyPos = ccp(posXX+((ii-5)*8), posYY);
        }
        sprite->setPosition(armyPos);
        sprite->setOpacity(0);
        
        selfArray->addObject(sprite);
    }
    for (int ii = selfArray->count()-1; ii >=0 ; ii--) {
        CCSprite *sprite = (CCSprite *)selfArray->objectAtIndex(ii);
        this->addChild(sprite);
    }
    
    this->initData(self->getRoleType(), army->getRoleType());
    
    /******************************haha it is here**************************/
    
    this->showMainIcon();     //cgp_pro
}

void SGBattlePreNLayer::initArrays()
{
    selfArray = CCArray::create();
    selfArray->retain();
    armyArray = CCArray::create();
    armyArray->retain();
    selfLArray = CCArray::create();
    selfLArray->retain();
    armyLArray = CCArray::create();
    armyLArray->retain();
    aniArray = CCArray::create();
    aniArray->retain();
    aniArrayN = CCArray::create();
    aniArrayN->retain();
    selfBArray = CCArray::create();
    selfBArray->retain();
    armyBArray = CCArray::create();
    armyBArray->retain();
    selfNumArray = CCArray::create();
    selfNumArray->retain();
    armyNumArray = CCArray::create();
    armyNumArray->retain();
}

void SGBattlePreNLayer::initData(int sType, int aType)
{
   

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite *lightSp = CCSprite::createWithSpriteFrameName("load_ctyLight.png");//gai
    float wid = lightSp->getContentSize().width*.54f;
    float high = lightSp->getContentSize().height;
    
    int scount = selfArray->count();
    if (scount > 5) scount = 5;
    for (int ii = 0; ii < scount; ii++) {
        CCSprite *lightSp = CCSprite::createWithSpriteFrameName("load_ctyLightNN.png");//gai
        
        float posX = size.width*.5f-IconWid*2+IconWid*(ii%5);
//        float posX = size.width*.5f - wid*(ii-2);
        float posY = size.height*.5f - high*.75f;
        
        lightSp->setPosition(ccp(posX, posY));
        lightSp->setOpacity(0);
        this->addChild(lightSp);
        selfLArray->addObject(lightSp);
    }
    scount = armyArray->count();
    if (scount > 5) scount = 5;
    for (int ii = 0; ii < scount; ii++) {
        CCSprite *lightSp = CCSprite::createWithSpriteFrameName("load_ctyLightNN.png");//gai
        
        float posX = size.width*.5f-IconWid*(ii%5-2);
//        float posX = size.width*.5f - wid*(ii-2);
        float posY = size.height*.5f + high*.75f;
        
        lightSp->setOpacity(0);
        lightSp->setPosition(ccp(posX, posY));
        this->addChild(lightSp);
        armyLArray->addObject(lightSp);
    }
    
    /*
     SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(roleId);
     int currStarLevel = 2;
     if (data) {
     int level = data->getOfficerCurrStarLevel();
     
     if (level > currStarLevel) {
     currStarLevel = level;
     }
     
     }
     CCString *fileName = CCString::createWithFormat("sceneLoad/load_ctyIconBbg_%d.png", currStarLevel);
     */
    
//    CCString *sName = CCString::createWithFormat("load_ctyHighBg_%d.png", sType);
//    CCString *bgName = CCString::createWithFormat("load_ctyIconBbg_%d.png", sType);//gai
    CCString *sName = CCString::createWithFormat("load_ctyHighBg_%d.png", sType);//gai
    CCString *bgName = CCString::create("load_ctyIconBbg_1.png");//gai
    for (int ii = 0; ii < 5; ii++) {
        CCSprite *lightBg = CCSprite::createWithSpriteFrameName(bgName->getCString());
        CCSprite *lightSp = CCSprite::createWithSpriteFrameName(sName->getCString());
//        CCSprite *lightBg = CCSprite::create(bgName->getCString());//gai
//        CCSprite *lightSp = CCSprite::create(sName->getCString());//gai

        float posX = size.width*.5f - wid*(ii-2);
        float posY = size.height*.5f - high*.75f;
        
        lightSp->setPosition(ccp(posX, posY-10));
        lightBg->setPosition(ccp(posX, posY-10));
        lightSp->setOpacity(0);
        lightBg->setOpacity(0);
        this->addChild(lightBg);
        this->addChild(lightSp);
        selfBArray->addObject(lightBg);
        selfBArray->addObject(lightSp);
    }
    
//    sName = CCString::createWithFormat("load_ctyHighBg_%d.png", aType);
//    bgName = CCString::createWithFormat("load_ctyIconBbg_%d.png", aType);
    sName = CCString::createWithFormat("load_ctyHighBg_%d.png", aType);//gai
    bgName = CCString::create("load_ctyIconBbg_1.png");//gai
    for (int ii = 0; ii < 5; ii++) {
        CCSprite *lightBg = CCSprite::createWithSpriteFrameName(bgName->getCString());//gai
        CCSprite *lightSp = CCSprite::createWithSpriteFrameName(sName->getCString());//gai
        float posX = size.width*.5f - wid*(ii-2);
        float posY = size.height*.5f + high*.75f;
        
        lightSp->setPosition(ccp(posX, posY+20));
        lightBg->setPosition(ccp(posX, posY+10));
        lightSp->setOpacity(0);
        lightBg->setOpacity(0);
        this->addChild(lightBg);
        this->addChild(lightSp);
        armyBArray->addObject(lightBg);
        armyBArray->addObject(lightSp);
    }
    
    float discX = IconWid*.48f, discY = IconHig*.32f;//.48,.32
    const char *fileName = "loadscene/load_ctyIcon_num.png";
    for (int ii = 0; ii < 10; ii++) {
        float posX = size.width*.5f-IconWid*2+IconWid*(ii%5) - discX;
        float posY = size.height*.5f - high*.75f + discY;
        float posXX = size.width*.5f-IconWid*(ii%5-2) - discX;
        float posYY = size.height*.5f + high*.75f + discY;
        
        unsigned wid = 343/10, hit = 51;
        if (ii == 9) { // 对10的特殊处理
            // 10中的1
            const char *num = CCString::createWithFormat("%d", 1)->getCString();
            CCLabelAtlas *label = CCLabelAtlas::create(num, fileName, wid, hit, '0');
            label->setPosition(ccp(posX, posY));
            selfNumArray->addObject(label);
            
            label = CCLabelAtlas::create(num, fileName, wid, hit, '0');
            label->setPosition(ccp(posXX, posYY));
            armyNumArray->addObject(label);
            
            // 10中的0
            num = CCString::createWithFormat("%d", 0)->getCString();
            label = CCLabelAtlas::create(num, fileName, wid, hit, '0');
            label->setPosition(ccpAdd(ccp(posX, posY), ccp(wid*.5f, 0)));
            selfNumArray->addObject(label);
            
            label = CCLabelAtlas::create(num, fileName, wid, hit, '0');
            label->setPosition(ccpAdd(ccp(posXX, posYY), ccp(wid*.5f, 0)));
            armyNumArray->addObject(label);
        }else {
            const char *num = CCString::createWithFormat("%d", ii+1)->getCString();
            CCLabelAtlas *label = CCLabelAtlas::create(num, fileName, wid, hit, '0');
            label->setPosition(ccp(posX, posY));
            selfNumArray->addObject(label);
            
            label = CCLabelAtlas::create(num, fileName, wid, hit, '0');
            label->setPosition(ccp(posXX, posYY));
            armyNumArray->addObject(label);
        }
    }
    for (int ii = 0; ii < 11; ii++) {
        CCLabelAtlas *label1 = (CCLabelAtlas *)armyNumArray->objectAtIndex(ii);
        CCLabelAtlas *label2 = (CCLabelAtlas *)selfNumArray->objectAtIndex(ii);
        label1->setOpacity(0);
        label2->setOpacity(0);
//        this->addChild(label1);
//        this->addChild(label2);
    }
    
    CCSpriteBatchNode *node = CCSpriteBatchNode::create("loadscene/load_dismiss.png");
    this->addChild(node);
    for (int ii = 0; ii < 5; ii++) { // 消失动画
        CCString *fileName = CCString::createWithFormat("load_ctyLight_%d.png", ii);
        CCSpriteFrame *sprite = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName->getCString());
        aniArray->addObject(sprite);
//		CCSpriteFrame* spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName->getCString());
//		aniArray->addSpriteFrame(spriteFrame);
    }
}

void SGBattlePreNLayer::showMainIcon()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float ctime = 0.15f;
    armyCtyBg->runAction(CCMoveTo::create(ctime, ccp(size.width/2 , armyCtyBg->getPositionY())));
    selfCtyBg->runAction(CCMoveTo::create(ctime, ccp(size.width/2 , selfCtyBg->getPositionY())));
    
    // call next method
    CCAction *action = CCSequence::create(CCDelayTime::create(ctime*1.2f),
                                          CCCallFunc::create(this, callfunc_selector(SGBattlePreNLayer::showIcons)),
                                          NULL);
    this->runAction(action);
}

void SGBattlePreNLayer::showIcons()//cgp_pro
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    float ctime = 1.0f;
    
    float posYY = size.height * .5f - IconHig * .51f;//.48f
    for (int ii = 0; ii < selfArray->count(); ii++)
    {
        CCSprite *sprite = (CCSprite *)selfArray->objectAtIndex(ii);
        CCMoveTo *moveTo = CCMoveTo::create(ctime, ccp(sprite->getPositionX(), posYY-10));
        CCAction *spawn = CCSpawn::create(CCEaseBackOut::create(moveTo),
                                          CCEaseExponentialOut::create(CCFadeIn::create(ctime*0.7f)),
                                          NULL);
        if (ii < 5)
        {
            sprite->runAction(spawn);
        }
        else
        {
            sprite->runAction(moveTo);
        }
        
    }
    posYY = size.height * .5f + IconHig * 1.52f;//1.47
    for (int ii = 0; ii < armyArray->count(); ii++)
    {
        CCSprite *sprite = (CCSprite *)armyArray->objectAtIndex(ii);
        CCMoveTo *moveTo = CCMoveTo::create(ctime, ccp(sprite->getPositionX(), posYY+10));
        CCAction *spawn = CCSpawn::create(CCEaseBackOut::create(moveTo),
                                          CCEaseExponentialOut::create(CCFadeIn::create(ctime*0.7f)),
                                          NULL);
        if (ii < 5)
        {
            sprite->runAction(spawn);
        }
        else
        {
            sprite->runAction(moveTo);
        }
    }
    
    /*haha it is here*///cgp_pro
    
    CCAction *action = CCSequence::create(CCDelayTime::create(ctime*1.15f),
                                          CCCallFunc::create(this, callfunc_selector(SGBattlePreNLayer::showHighLight)),
                                          NULL);
    this->runAction(action);
}

void SGBattlePreNLayer::showHighLight()  //cgp_pro
{
    float ctime = 0.25f;
    int scount = selfArray->count();
    scount > 5 && (scount = 5);
    for (int ii = 0; ii < scount; ii++)
    {
        CCAction *action = CCSequence::create(CCFadeTo::create(ctime, 55),
                                              CCFadeTo::create(ctime, 255), NULL);
        OfficerSprite *sprite = (OfficerSprite *)selfArray->objectAtIndex(ii);
        sprite->runAction(action);
        
        action = CCSequence::create(CCFadeTo::create(ctime, 255),
                                    CCFadeTo::create(ctime, 0), NULL);
        CCSprite *spriteC = (CCSprite *)selfLArray->objectAtIndex(ii);
        spriteC->runAction(action);
    }
    int acount = armyArray->count();
    acount > 5 && (acount = 5);
    for (int ii = 0; ii < acount; ii++)
    {
        CCAction *action = CCSequence::create(CCFadeTo::create(ctime, 55),
                                              CCFadeTo::create(ctime, 255), NULL);
        OfficerSprite *sprite = (OfficerSprite *)armyArray->objectAtIndex(ii);
        sprite->runAction(action);
        
        action = CCSequence::create(CCFadeTo::create(ctime, 255),
                                    CCFadeTo::create(ctime, 0), NULL);
        CCSprite *spriteC = (CCSprite *)armyLArray->objectAtIndex(ii);
        spriteC->runAction(action);
    }
    
    for (int ii = 0; ii < 5; ii++)
    {
        CCAction *action1 = CCSequence::create(CCDelayTime::create(ctime-0.05f),
                                              CCFadeTo::create(0.05f, 255),
                                              NULL);
        CCAction *action2 = CCSequence::create(CCDelayTime::create(ctime-0.05f),
                                               CCFadeTo::create(0.05f, 255),
                                               NULL);
        CCSprite *spUp = (CCSprite *)armyNumArray->objectAtIndex(ii);
        CCSprite *spDown = (CCSprite *)selfNumArray->objectAtIndex(ii);
        spUp->runAction(action1);
        spDown->runAction(action2);
    }
    if (scount <= 5)
    {
        for (int ii = scount; ii < 5; ii++)
        {
            CCSprite *sprite1 = (CCSprite *)selfBArray->objectAtIndex((5-(ii+1))*2);
            CCSprite *sprite2 = (CCSprite *)selfBArray->objectAtIndex((5-(ii+1))*2+1);
            sprite1->runAction(CCFadeIn::create(0.1f));
            sprite2->runAction(CCFadeIn::create(0.1f));
        }
    }
    if (acount <= 5)
    {
        for (int ii = acount; ii < 5; ii++) {
            CCSprite *sprite1 = (CCSprite *)armyBArray->objectAtIndex(ii*2);
            CCSprite *sprite2 = (CCSprite *)armyBArray->objectAtIndex(ii*2+1);
            sprite1->runAction(CCFadeIn::create(0.1f));
            sprite2->runAction(CCFadeIn::create(0.1f));
        }
    }
    
    //cgp_pro
    /**haha it is here**/
    CCAction *action = CCSequence::create(CCDelayTime::create(ctime * 4.5f),
                                          CCCallFunc::create(this, callfunc_selector(SGBattlePreNLayer::dismissIcons)),
                                          NULL);
    this->runAction(action);
}

void SGBattlePreNLayer::dismissIcons()
{
    int scount = selfArray->count();
    int acount = armyArray->count();
    
    if (scount > 5)
    {
        for (int ii = 0; ii < selfBArray->count(); ii++)
        {
            CCSprite *sprite = (CCSprite *)selfBArray->objectAtIndex(ii);
            sprite->runAction(CCFadeIn::create(0.1f));
        }
    }
    if (acount > 5)
    {
        for (int ii = 0; ii < armyBArray->count(); ii++)
        {
            CCSprite *sprite = (CCSprite *)armyBArray->objectAtIndex(ii);
            sprite->runAction(CCFadeIn::create(0.1f));
        }
    }
    
    if (scount > 5)
    {
        for (int ii = 0; ii < 5; ii++)
        {
            OfficerSprite *spriteO = (OfficerSprite *)selfArray->objectAtIndex(ii);
            spriteO->runAction(CCFadeOut::create(0.1f));
            
            CCSprite *sprite = (CCSprite *)CCSprite::createWithSpriteFrame((CCSpriteFrame*)aniArray->objectAtIndex(0));
            this->addChild(sprite);
            CCSprite *spriteC = (CCSprite *)selfLArray->objectAtIndex(ii);
            sprite->setPosition(spriteC->getPosition());
            CCAnimation *animation = CCAnimation::createWithSpriteFrames(aniArray, 0.125f);
            CCAction *action = CCSequence::create(CCFadeIn::create(0.15f),
                                                  CCRepeat::create(CCAnimate::create(animation),1) ,
                                                  CCCallFuncN::create(this, callfuncN_selector(SGBattlePreNLayer::removeLights)),
                                                  NULL);
            sprite->runAction(action);
        }
    }
    
    if (acount > 5)
    {
        for (int ii = 0; ii < 5; ii++)
        {
            OfficerSprite *spriteO = (OfficerSprite *)armyArray->objectAtIndex(ii);
            spriteO->runAction(CCFadeOut::create(0.1f));
            
            CCSprite *spriteC = (CCSprite *)armyLArray->objectAtIndex(ii);
            CCSprite *sprite = (CCSprite *)CCSprite::createWithSpriteFrame((CCSpriteFrame*)aniArray->objectAtIndex(0));
            this->addChild(sprite);
            sprite->setPosition(spriteC->getPosition());
            CCAnimation *animation = CCAnimation::createWithSpriteFrames(aniArray, 0.125f);
            CCAction *action = CCSequence::create(CCFadeIn::create(0.15f),
                                                  CCRepeat::create(CCAnimate::create(animation), 1),
                                                  CCCallFuncN::create(this, callfuncN_selector(SGBattlePreNLayer::removeLights)),
                                                  NULL);
            sprite->runAction(action);
        }
    }
    
    if (scount <= 5 && acount <= 5)
    {
        this->gotoFightLayer();
    }
    else
    {
        if (scount > 5 && acount > 5)
        {
            this->showNextNum(1);
            this->showNextNum(2);
        }
        if (scount<=5 && acount>5) {
            this->showNextNum(2);
        }
        if (scount>5 && acount<=5) {
            this->showNextNum(1);
        }
        
        /**haha it is here***///cgp_pro
        CCAction *action = CCSequence::create(CCDelayTime::create(0.125*5+0.15f),
                                              CCCallFunc::create(this, callfunc_selector(SGBattlePreNLayer::dismissIcons2)),
                                              NULL);
        this->runAction(action);
    }

}

void SGBattlePreNLayer::dodismiss1()  // 双方武将都大于5
{
    for (int ii = 0; ii < 5; ii++)
    { // 隐藏第一排数字
        CCAction *action1 = CCSequence::create(CCDelayTime::create(0.25f),
                                               CCFadeTo::create(0.05f, 0), NULL);
        CCAction *action2 = CCSequence::create(CCDelayTime::create(0.25f),
                                               CCFadeTo::create(0.05f, 0), NULL);
        
        CCSprite *sp1 = (CCSprite *)armyNumArray->objectAtIndex(ii);
        CCSprite *sp2 = (CCSprite *)selfNumArray->objectAtIndex(ii);
        sp1->runAction(action1);
        sp2->runAction(action2);
        
    }
    for (int ii = 5; ii < selfNumArray->count(); ii++)
    { // 显示第二排数字
        CCAction *action3 = CCSequence::create(CCDelayTime::create(0.25f),
                                               CCFadeTo::create(0.05f, 255), NULL);
        CCAction *action4 = CCSequence::create(CCDelayTime::create(0.25f),
                                               CCFadeTo::create(0.05f, 255), NULL);
        
        CCSprite *sp3 = (CCSprite *)armyNumArray->objectAtIndex(ii);
        CCSprite *sp4 = (CCSprite *)selfNumArray->objectAtIndex(ii);
        sp3->runAction(action3);
        sp4->runAction(action4);
    }
    
    //dismissIcons2 继续显示
    CCAction *action = CCSequence::create(CCDelayTime::create(0.125*5+0.15f),
                                          CCCallFunc::create(this, callfunc_selector(SGBattlePreNLayer::dismissIcons2)),
                                          NULL);
    
    this->runAction(action);
}
void SGBattlePreNLayer::dodismiss2()   // 双方武将都小于5
{
    this->gotoFightLayer();   //直接pve战斗
}
void SGBattlePreNLayer::dodismiss3()   // 自己武将小于5，敌方武将大于5
{
    for (int ii = 0; ii < 5; ii++)
    { // 隐藏第一排数字
        CCAction *action1 = CCSequence::create(CCDelayTime::create(0.25f),
                                               CCFadeTo::create(0.05f, 0), NULL);
        CCSprite *sp1 = (CCSprite *)armyNumArray->objectAtIndex(ii);
        sp1->runAction(action1);   
    }
    for (int ii = 5; ii < selfNumArray->count(); ii++)
    { // 显示第二排数字
        CCAction *action3 = CCSequence::create(CCDelayTime::create(0.25f),
                                               CCFadeTo::create(0.05f, 255), NULL);        
        CCSprite *sp3 = (CCSprite *)armyNumArray->objectAtIndex(ii);
        sp3->runAction(action3);
    }
    
    CCAction *action = CCSequence::create(CCDelayTime::create(0.125*5+0.15f),
                                          CCCallFunc::create(this, callfunc_selector(SGBattlePreNLayer::dismissIcons2)),
                                          NULL);
    this->runAction(action);
}
void SGBattlePreNLayer::dodismiss4()  // 敌方武将小于5，自己武将大于5
{
    for (int ii = 0; ii < 5; ii++)
    { // 隐藏第一排数字
        CCAction *action2 = CCSequence::create(CCDelayTime::create(0.25f),
                                               CCFadeTo::create(0.05f, 0), NULL);
        CCSprite *sp2 = (CCSprite *)selfNumArray->objectAtIndex(ii);
        sp2->runAction(action2);
        
    }
    for (int ii = 5; ii < selfNumArray->count(); ii++)
    { // 显示第二排数字
        CCAction *action4 = CCSequence::create(CCDelayTime::create(0.25f),
                                               CCFadeTo::create(0.05f, 255), NULL);
        CCSprite *sp4 = (CCSprite *)selfNumArray->objectAtIndex(ii);
        sp4->runAction(action4);
    }
    
    CCAction *action = CCSequence::create(CCDelayTime::create(0.125*5+0.15f),
                                          CCCallFunc::create(this, callfunc_selector(SGBattlePreNLayer::dismissIcons2)),
                                          NULL);
    this->runAction(action);
}

void SGBattlePreNLayer::showNextNum(int type)
{
    if (type == 1) {
        for (int ii = 0; ii < 5; ii++) { // 隐藏第一排数字
            CCAction *action2 = CCSequence::create(CCDelayTime::create(0.25f),
                                                   CCFadeTo::create(0.05f, 0), NULL);
            CCSprite *sp2 = (CCSprite *)selfNumArray->objectAtIndex(ii);
            sp2->runAction(action2);
            
        }
        for (int ii = 5; ii < selfNumArray->count(); ii++) { // 显示第二排数字
            CCAction *action4 = CCSequence::create(CCDelayTime::create(0.25f),
                                                   CCFadeTo::create(0.05f, 255), NULL);
            CCSprite *sp4 = (CCSprite *)selfNumArray->objectAtIndex(ii);
            sp4->runAction(action4);
        }
    }
    
    if (type == 2) {
        for (int ii = 0; ii < 5; ii++) { // 隐藏第一排数字
            CCAction *action1 = CCSequence::create(CCDelayTime::create(0.25f),
                                                   CCFadeTo::create(0.05f, 0), NULL);
            CCSprite *sp1 = (CCSprite *)armyNumArray->objectAtIndex(ii);
            sp1->runAction(action1);
        }
        for (int ii = 5; ii < selfNumArray->count(); ii++) { // 显示第二排数字
            CCAction *action3 = CCSequence::create(CCDelayTime::create(0.25f),
                                                   CCFadeTo::create(0.05f, 255), NULL);
            CCSprite *sp3 = (CCSprite *)armyNumArray->objectAtIndex(ii);
            sp3->runAction(action3);
        }
    }
}

void SGBattlePreNLayer::dismissIcons2()
{
    int aIndex = aniArray->count() - 1;
    int scount = selfArray->count();
    for (int ii = 5; ii < scount; ii++)
    {
        OfficerSprite *spriteO = (OfficerSprite *)selfArray->objectAtIndex(ii);
        CCAction *actionO = CCSequence::create(CCDelayTime::create(0.125f*4),
                                               CCFadeIn::create(0.05f),
                                               CCCallFuncN::create(this, callfuncN_selector(SGBattlePreNLayer::dismissSelfIconBgs)),
                                               NULL);
        spriteO->setTag(ii);
        spriteO->runAction(actionO);
        
        CCSprite *sprite = (CCSprite *)CCSprite::createWithSpriteFrame((CCSpriteFrame*)aniArray->objectAtIndex(aIndex));
        this->addChild(sprite);
        CCSprite *spriteC = (CCSprite *)selfLArray->objectAtIndex(ii-5);
        sprite->setPosition(spriteC->getPosition());
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(aniArray, 0.125f);
        
        CCAction *action = CCSequence::create(CCFadeIn::create(0.15f),
                                              CCRepeat::create(CCAnimate::create(animation)->reverse(), 1),
                                              CCCallFuncN::create(this, callfuncN_selector(SGBattlePreNLayer::removeLights)),
                                              NULL);
        sprite->runAction(action);
    }
    
    int acount = armyArray->count();
    for (int ii = 5; ii < acount; ii++)
    {
        OfficerSprite *spriteO = (OfficerSprite *)armyArray->objectAtIndex(ii);
        CCAction *actionO = CCSequence::create(CCDelayTime::create(0.125f*4),
                                               CCFadeIn::create(0.05f),
                                               CCCallFuncN::create(this, callfuncN_selector(SGBattlePreNLayer::dismissArmyIconBgs)),
                                               NULL);
        spriteO->setTag(ii);
        spriteO->runAction(actionO);
        
        CCSprite *sprite = (CCSprite *)CCSprite::createWithSpriteFrame((CCSpriteFrame*)aniArray->objectAtIndex(aIndex));
        this->addChild(sprite);
        CCSprite *spriteC = (CCSprite *)armyLArray->objectAtIndex(ii-5);
        sprite->setPosition(spriteC->getPosition());
        CCAnimation *animation = CCAnimation::createWithSpriteFrames(aniArray, 0.125f);
        
        CCAction *action = CCSequence::create(CCFadeIn::create(0.15f),
                                              CCRepeat::create(CCAnimate::create(animation)->reverse(), 1),
                                              CCCallFuncN::create(this, callfuncN_selector(SGBattlePreNLayer::removeLights)),
                                              NULL);
        sprite->runAction(action);
    }
    
    float dtime = 0.05f;
    if (acount >= 5 || scount >= 5)
    {
        dtime = 0.15 + 0.125*5;
    }
    CCAction *callAction = CCSequence::create(CCDelayTime::create(dtime),
                                               CCCallFunc::create(this, callfunc_selector(SGBattlePreNLayer::gotoFightLayer)),
                                               NULL);
    this->runAction(callAction);
}

void SGBattlePreNLayer::dismissArmyIconBgs(CCNode *node)
{
    int ii = node->getTag();
    CCSprite *spriteB1 = (CCSprite *)armyBArray->objectAtIndex((ii-5)*2);
    CCSprite *spriteB2 = (CCSprite *)armyBArray->objectAtIndex((ii-5)*2+1);
    spriteB1->runAction(CCFadeOut::create(0.05f));
    spriteB2->runAction(CCFadeOut::create(0.05f));
}
void SGBattlePreNLayer::dismissSelfIconBgs(CCNode *node)
{
    int ii = node->getTag();
    CCSprite *spriteB1 = (CCSprite *)selfBArray->objectAtIndex((10-(ii+1))*2);
    CCSprite *spriteB2 = (CCSprite *)selfBArray->objectAtIndex((10-(ii+1))*2+1);
    spriteB1->runAction(CCFadeOut::create(0.05f));
    spriteB2->runAction(CCFadeOut::create(0.05f));
}

void SGBattlePreNLayer::removeLights(CCNode *node)
{
    node->removeFromParentAndCleanup(true);
}

void SGBattlePreNLayer::gotoFightLayer()  //cgp_pro
{
    DDLog("xx=====================gotoFightLayer");
    bool isPVE = SGAIManager::shareManager()->isPVE;
    if (isPVE == true)
    {
        SGAIManager *aiMgr = SGAIManager::shareManager();
        //重新进战斗重置自动战斗所有变量
        aiMgr->setIsCompleteAttack(false);
        aiMgr->setIsCompleteSkill(true);
        aiMgr->setIsOpenAutoBattle(false);
        aiMgr->setIsDispSkillHead(false);
        aiMgr->setAnimationSpeed(1);
        SGBattleManager::sharedBattle()->pveStarts();   //cgp_pro
    }
    else
    {
        SGBattleManager::sharedBattle()->pvpStarts();
    }
}

#pragma mark -
#pragma mark - -----------OfficerSprite Define-----------

#if (PLATFORM == IOS)

CCSprite *CCHLSprite::graylightWithCCSprite(CCSprite *oldSprite, bool isLight)
{
    CCPoint point = oldSprite->getAnchorPoint();
    oldSprite->setAnchorPoint(ccp(0, 0));
    int wid = oldSprite->getContentSize().width;
    int hit = oldSprite->getContentSize().height;
    CCRenderTexture *outTexture = CCRenderTexture::create(wid, hit);
    outTexture->begin();
    oldSprite->visit();
    outTexture->end();
    oldSprite->setAnchorPoint(point);
    
    CCImage *finalImg = outTexture->newCCImage();
    unsigned char *pData = finalImg->getData();
    int iIndex = 0;
    
    if (isLight) {
        for (int i = 0; i < finalImg->getHeight(); i++) {
            for (int j = 0; j < finalImg->getWidth(); j++) {
                // hightlight
                int iHightlightPlus = 50;
                int iBPos = iIndex;
//                unsigned int iB = pData[iIndex++];
//                unsigned int iG = pData[iIndex++];
//                unsigned int iR = pData[iIndex++];
                unsigned int iB = pData[iIndex];
                iIndex++;
                unsigned int iG = pData[iIndex];
                iIndex++;
                unsigned int iR = pData[iIndex];
                iIndex++;
//                unsigned int o = pData[iIndex++];
                iIndex++;
                iB = (iB + iHightlightPlus > 255 ? 255 : iB + iHightlightPlus);
                iG = (iG + iHightlightPlus > 255 ? 255 : iG + iHightlightPlus);
                iR = (iR + iHightlightPlus > 255 ? 255 : iR + iHightlightPlus);
                pData[iBPos] = (unsigned char)iB;
                pData[iBPos+1] = (unsigned char)iG;
                pData[iBPos+2] = (unsigned char)iR;
            }
        }
    }
    
    CCTexture2D *texture = new CCTexture2D();
    texture->initWithImage(finalImg);
    CCSprite *newSprite = CCSprite::createWithTexture(texture);
    delete finalImg;
    texture->release();
    return newSprite;
}
#endif