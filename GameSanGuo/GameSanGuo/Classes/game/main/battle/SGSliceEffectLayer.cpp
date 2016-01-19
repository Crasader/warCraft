//
//  SGSliceEffectLayer.cpp
//  GameSanGuo
//
//  Created by Frank Xu on 12/15/15.
//
//

#include "SGSliceEffectLayer.h"
#include "ResourceManager.h"
#include "GlobalConfig.h"


SGSliceEffectLayer::SGSliceEffectLayer():
sourceNode(NULL) {
    sourceNode =  CCDirector::sharedDirector()->getRunningScene();
}

SGSliceEffectLayer::~SGSliceEffectLayer() {
    
    //release bind texture in layer
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_sliceEffectLayer);
}

void SGSliceEffectLayer::bindResources() {
    /* this is hacked for android
     * pkm texture that use two seperate image for color and alpha channels.
     * OpenES `glBlendFuc` func cann't handler like `GL_SRC1_ALPHA` mode.
     * So we used PNG file as mask texture.
     */
    
    // change to load png texture
    CCTextureCache::sharedTextureCache()->setLoadingMode(LIM_PNG_AS_PNG);
    
    /**
     * load resource
     */
    ResourceManager::sharedInstance()->bindTexture("qiepian/qiepian.plist", RES_TYPE_LAYER_UI, sg_sliceEffectLayer);
    
    /**
     * restore default texture loading mode by it's platform
     */
    if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
        CCTextureCache::sharedTextureCache()->setLoadingMode(LIM_PNG_AS_ETC);
    }
    else if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        CCTextureCache::sharedTextureCache()->setLoadingMode(LIM_PNG_AS_PVR);
    }
    else
    {
        CCTextureCache::sharedTextureCache()->setLoadingMode(LIM_PNG_AS_PNG);
    }
    
    
}

bool SGSliceEffectLayer::init() {
    
    //call super init
    if (!CCNode::init()) {
        return false;
    }
    
    bindResources();
    
    createSlices();
    
    return true;
}

CCRenderTexture* SGSliceEffectLayer::snapshot() {
    
    const CCSize& size =  sourceNode->getContentSize();
    
    CCRenderTexture* renderTexture = CCRenderTexture::create(size.width, size.height);
    
    renderTexture->begin();
    
    sourceNode->visit();
    
    renderTexture->end();
    
    return renderTexture;
}

void SGSliceEffectLayer::createSlices()  {
    
    CCRenderTexture* sourceSnapshotNode = snapshot();
    
    //slice texture start index
    int startIndex = 1;
    
    //number of slices
    int numsSlice = 2;
    
    //loop for create slice
    for (int i = startIndex; i <= numsSlice; i++) {
        
        CCString* sliceName =  CCString::createWithFormat("qiepian_%d.png", i);
        
        CCNode* slice = createSpriteWithMask(sourceSnapshotNode->getSprite(), sliceName->getCString());
        
        slice->setTag(i);
        
        this->addChild(slice);
    }
}

CCNode* SGSliceEffectLayer::createSpriteWithMask(cocos2d::CCSprite *src, const char *maskName) {
    
    CCSprite * mask = CCSprite::createWithSpriteFrameName(maskName);
    
    CCSize srcContent = src->getContentSize();
    
    CCSize maskContent = mask->getContentSize();
    
    
    float ratiow = srcContent.width / maskContent.width;
    float ratioh = srcContent.height / maskContent.height;
    
    mask->setScaleX(ratiow);
    mask->setScaleY(ratioh);
    
    float x = srcContent.width / 2;
    float y = srcContent.height / 2;
    
    const CCPoint& position = ccp(x,y);
    
    mask->setPosition(position);
    src->setPosition(position);

 
    CCRenderTexture * rt;
    if( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    {
        mask->setBlendFunc({GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
        mask->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        
        
        src->setBlendFunc({ GL_DST_ALPHA, GL_ZERO });
        src->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        
        
        rt = CCRenderTexture::create(srcContent.width, srcContent.height, kTexture2DPixelFormat_RGBA8888);
        
        
        rt->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        rt->getSprite()->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        
        rt->getSprite()->setBlendFunc({GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
        //rt->getSprite()->setBlendFunc({GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA});
    }
    else if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        mask->setBlendFunc({GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
        mask->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        
        
        src->setBlendFunc({ GL_DST_ALPHA, GL_ZERO });
        src->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        
        
        rt = CCRenderTexture::create(srcContent.width, srcContent.height, kTexture2DPixelFormat_RGBA8888);
        
        
        rt->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        rt->getSprite()->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        
        rt->getSprite()->setBlendFunc({GL_ONE, GL_ONE_MINUS_SRC_ALPHA});
    }
   
    rt->begin();
    mask->visit();
    src->visit();
    rt->end();
    
    const CCSize& visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    rt->setPosition(CCPoint(visibleSize.width/2,visibleSize.height/2));
    
    return rt;
}

void SGSliceEffectLayer::play() {
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCNode* slice = this->getChildByTag(1);
    
    CCDelayTime*   delay = CCDelayTime::create(0.5);
    
    CCFiniteTimeAction*  moveIn1 = CCEaseExponentialOut::create(CCMoveTo::create(1.5f, CCPoint(visibleSize.width/2, -visibleSize.height)));
    
    CCCallFunc*   callRemove = CCCallFunc::create(this, callfunc_selector(SGSliceEffectLayer::removeFromParent));
    

    slice->runAction(CCSequence::create(delay, moveIn1 ,callRemove, nullptr));
    
    //右边
    slice = this->getChildByTag(2);
    CCDelayTime*   delay2 = CCDelayTime::create(0.4);
    CCFiniteTimeAction*  moveIn2 = CCEaseExponentialOut::create(CCMoveTo::create(1.5f, CCPoint(-visibleSize.width / 2, -visibleSize.height)));
  
    slice->runAction(CCSequence::create(delay2, moveIn2, nullptr));
}
