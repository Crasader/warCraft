//
//
//  GameSanGuo
//
//  by ： zyc  14-7-9.
//
//

#include "SGMainLayer.h"
#include "SGBaseBox.h"
#include "SNSTableViewCellItem.h"

#ifndef __GameSanGuo__SGLootWiKiCell__
#define __GameSanGuo__SGLootWiKiCell__

class SGLootWiKiLayer;
class HaveSource;

class SGLootWiKiCell : public SNSTableViewCellItem
{
public:
    SGLootWiKiCell();
    ~SGLootWiKiCell();
    
    //static create
    static SGLootWiKiCell * create(HaveSource * ,SGLootWiKiLayer *, bool isOpened = false);
    //加载视图
    void initView(HaveSource *  , SGLootWiKiLayer* );
    
    //滑动时更新来源
    void updateItem(HaveSource * source, bool isOpened = false);
    
    //是否播放手型动画
    void showHandAnimate(bool );
private:
    //点击碎片的回调
    void onClicked();
    
    //创建闯关、、、背景和标题。
    CCSprite * createSprite(int lootType);
    
    //更新时 隐藏 显示 元素 (即 来源是否开启 来区分)
    void showHideEle(HaveSource * hs);
    
    //设置2个描述的文本内容
    void setDesc(std::string des1 , std::string des2 );
    
    SGLootWiKiLayer * m_lootWiKiLayer;
    //
    HaveSource * m_source;
    
    SGCCLabelTTF * m_sourceTitle;
    SGCCLabelTTF * m_sourceDes1;
    SGCCLabelTTF * m_sourceDes2;
    //精英闯关进度
    SGCCLabelTTF * m_rate;
    
    //有来源时 上部背景图片
    CCSprite * m_spriteOpen;
    
    //未开启时显示提示
    SGCCLabelTTF * m_notOpenLab;
    
    //是否开启的bool标志。
    bool _isOpened;
    
    //是否开启的文本。
    SGCCLabelTTF* _redLabel;
};




#endif /* end */
