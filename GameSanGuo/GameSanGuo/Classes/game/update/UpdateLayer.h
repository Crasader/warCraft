//
//  UpdateLayer.h
//  WarCraftCardGame
//
//  Created by canglang on 13-2-18.
//
//

#ifndef __WarCraftCardGame__UpdateLayer__
#define __WarCraftCardGame__UpdateLayer__

#include "SGBaseLayer.h"
#include "cocos2d.h"

USING_NS_CC;

class PointLayer;
class SGTouchLayer;

//幻灯片最大数量
#define SLIDING_IMAGES_NUM 8

/**
 * 资源更新UI。
 */
class UpdateLayer : public SGBaseLayer
{
public:
	UpdateLayer();
	~UpdateLayer();
    static UpdateLayer* create(bool b);
    
	virtual bool initWithData();
    void viewDidLoad();
    
private:
    //更新整体框架。
    CCLayerColor* _updFrame;
    
    //更新星空背景。
    CCSprite* _updBkg;
    
    //更新中心点。
    CCPoint _updBasePos;
    
    //控制滑动的控件。
    SGTouchLayer* _updTouchLayer;
    
    //幻灯片海报的载体。
    CCLayerColor* _updPostersPanel;
    
    //幻灯片海报。
    CCSprite* _updPosters[SLIDING_IMAGES_NUM];
    
    //海报载体的位置。
    CCPoint _updPostersPanelPos[SLIDING_IMAGES_NUM];
    
    //点图阵列，单独的控件。
    PointLayer* _updSpotsPanel;
    
    //灰色蒙版。
    CCLayerColor* _updMaskLayer;
    
    //当前显示到第几张。
    int _updCurPosterIndex;
    
    //是否自动播放，只要有点击发生，则关闭自动播放。
    bool _updIsAutoPlaying;
    
public:
    //开启幻灯片更新。
    void switchUpdFrame(bool onoff);
    
    //动态调整幻灯片。
    void playUpdPosters();
    
    //重置滑动参数。
    void resetHelloParam();
    
    //移动layer
    void movePoster();
    
    //修改提示玩家的文本，从“检查更新”到“系统更新”。
    void setLabelInfo(std::string str);
    
    //began
    bool helloBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    //moved
    void helloMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    //ended
    void helloEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    //cancelled
    void helloCancelled(CCTouch *pTouch, CCEvent *pEvent);

    
public:
	//更新检测函数，每0.2秒触发一次。
    void checkingUpdate();
    
    //显示此UI，并关闭其他UI触控。
    void setUpdateLayerShow();
    
    //设置动态文本。
    void setTheLabelString();
    
    //设置进度百分比。
    void setUpdateProgress(float percent);
    
    //控制"..."显隐的变量。
    int m_showDotCount;
    
    //大标题。（资源更新）
    CCLabelTTF *labelTitle;
    
    //更新进度文本。 （81%）
    CCLabelTTF *labelSchedule;
    
    //点1。
    SGCCLabelTTF *m_plabeDot1;
    
    //点2。
    SGCCLabelTTF *m_plabeDot2;
    
    //点3。
    SGCCLabelTTF *m_plabeDot3;
    
    //指示文本。（玩儿命加载资源中）
    SGCCLabelTTF *labelResourceBag;
	
    //进度条背景。
    CCSprite* scalebg;
    
    //进度条。
	CCProgressTimer *updateProgress;

};
#endif /* defined(__WarCraftCardGame__UpdateLayer__) */
