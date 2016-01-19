//
//  SGLoadingView.m
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-7.
//
//

#include "PlatformAdp.h"

#if (PLATFORM == IOS)
#import "SGLoadingView.h"
#import "EAGLView.h"

@interface SGLoadingView ()

@end

@implementation SGLoadingView

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSLog(@"屏蔽所有触摸");
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self.view setBackgroundColor:[UIColor clearColor]];
    CGRect rect = [[EAGLView sharedEGLView]frame];
    [self.view setFrame:rect];
//    animation = [[UIActivityIndicatorView alloc]init];
//    //设置显示方式
//    [animation setActivityIndicatorViewStyle:UIActivityIndicatorViewStyleWhiteLarge];
//    [animation setCenter:CGPointMake(rect.size.width/2, rect.size.height/2)];
//    [self.view setCenter:CGPointMake(rect.size.width/2, rect.size.height/2)];
//    [animation startAnimating];
//    [animation setColor:[UIColor whiteColor]];
//    [self.view addSubview:animation];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)viewDidUnload
{
//    [animation stopAnimating];
}

-(void)dealloc
{
//    [animation stopAnimating];
//    [animation release];
    [super dealloc];
}

@end

#else
//android code

#include "SGLoadingView.h"
#include "cocos2d.h"
USING_NS_CC;

bool SGLoadingView::init()
{
	if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 0)) )
	{
		return false;
	}
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCEGLView *egl = CCEGLView::sharedOpenGLView();
	egl->setFrameSize(winSize.width, winSize.height);
	
	return true;
}

//void SGLoadingView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
bool SGLoadingView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	std::cout << "∆¡±ŒÀ˘”–¥•√˛" << std::endl;
	return true;
}
#endif


