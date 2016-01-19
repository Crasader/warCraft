//
//  SGLoadingView.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-7.
//
//

#ifndef __GameSanGuo__SGLoadingView__
#define __GameSanGuo__SGLoadingView__

#include "PlatformAdp.h"

#if (PLATFORM == IOS)

#include "SGCantAdvanceBox.h"

#import <UIKit/UIKit.h>
#include <Foundation/Foundation.h>

@interface SGLoadingView : UIViewController
{
    //UIActivityIndicatorView *animation;
}
@end

#else
//android code
#include "game/main/barracks/advance/SGCantAdvanceBox.h"
#include "cocos2d.h"

USING_NS_CC;

class SGLoadingView : public cocos2d::CCLayerColor
{
public:
	virtual bool init();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	
	CREATE_FUNC(SGLoadingView);
};

#endif

#endif