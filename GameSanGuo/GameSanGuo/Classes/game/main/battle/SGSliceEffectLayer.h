//
//  SGSliceEffectLayer.hpp
//  The slice Effect for screen
//
//  Created by Frank Xu on 12/15/15.
//
//

#ifndef SGSliceEffectLayer_hpp
#define SGSliceEffectLayer_hpp

#include "cocos2d.h"

USING_NS_CC;

class SGSliceEffectLayer: public CCNode {
    
    
public:
    
    /**
     * Allocates the initializes a SGSliceEffectLayer
     * @return A initialized EffectLayer which is marked as 'autorelease'
     */
    CREATE_FUNC(SGSliceEffectLayer);
    
    /**
     * Initializes the instance of SGSliceEffectLayer
     * @return whether the initiazlied is successful.
     */
    bool init();
    
    /**
     * Default Constructor
     */
    SGSliceEffectLayer();
    
    /**
     * Destructor
     */
    ~SGSliceEffectLayer();
    
    /**
     * Run the slice effect actions.
     */
    void play();
    
private:
    
    /**
     * Create sprite with mask provided.
     * @param src The sprite that be masked.
     * @param maskName The texture name used as mask.
     * @return A masked node.
     */
    CCNode* createSpriteWithMask(CCSprite* src, const char* maskName);
    
    /**
     * Bind resource use `ResourceManage` for load texture we needed.
     */
    void bindResources();
    
    /**
     * create snapshot of node.
     * @param node The node used to snapshot
     * @return The RenderTexure with node snapshot
     */
    CCRenderTexture* snapshot();
    
    /**
     * Create slices from texture files
     */
    void createSlices();
    
private:
    
    /**
     * Source node reference that be snapshot
     */
    CCNode* sourceNode;
};

#endif /* SGSliceEffectLayer_hpp */
