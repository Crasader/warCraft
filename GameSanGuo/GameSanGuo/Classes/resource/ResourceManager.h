//
//  ResourceManager.h
//  GameSanGuo
//
//  Created by Loong on 13-10-31.
//
//


#ifndef GameSanGuo_ResourceManager_h
#define GameSanGuo_ResourceManager_h

#include "cocos2d.h"
#include "../game/base/SGBaseTags.h"
USING_NS_CC;

//资源管理类型。
//@notice: LAYER_UI和BATTLE_IMAGE视同一样处理，由于保有量较大，不进行统一改写。
//@notice: 游戏当前只有两种资源，常驻资源(COMMON_IMAGE)和即时资源(LAYER_UI/BATTLE_IMAGE)。
typedef enum
{
    RES_TYPE_COMMON_IMAGE,   //公共资源，这类资源常驻内存无需释放，一般为publicX型图片。
    RES_TYPE_LAYER_UI,       //专属资源，打开Layer时创建，关闭Layer时释放，包含所有的动画/UI。
    RES_TYPE_BATTLE_IMAGE,   //战斗资源，离开战斗则全部释放。包含所有的动画/UI。
    RES_TYPE_NUM,
} RES_TYPE;


typedef std::map<std::string, int> STR_INT_MAP;

//广义的文件类型，以后缀名区分。
enum FILE_TYPE
{
    kFT_PLIST,
    kFT_PVR,
    kFT_SCML,
    kFT_TOTAL_NUM,
};

//#define PNG_BEGIN LIM_PNG_AS_PVR

//资源的类型标识、TAG标识、引用计数
struct ResTypeTagCount
{
public:
    RES_TYPE type;
    LayerTag tag;
    uint count;
    
    ResTypeTagCount() : type(RES_TYPE_LAYER_UI), tag(sg_LayerNull), count(0)
    {}
    
    ResTypeTagCount(RES_TYPE p, LayerTag t, uint c)
    {
        type = p;
        tag = t;
        count = c;
    }
};

///////////////////////////////////////////////////////----ResourceManager-------/////////////////////////////////////////////
class ResourceManager : public cocos2d::CCObject
{
private:
    //layterTag的纹理list的map，允许list里的元素重复。
    std::map<LayerTag, std::list<std::string>* >* _layerResListMap;
    
    //plist引用计数map。
    std::map<std::string, ResTypeTagCount>* _resRefMap;
    
    //广义资源文件统计map。
    std::map< FILE_TYPE, STR_INT_MAP* >* _fileStatMap;
    
    //待移除的plist的list。
    std::list<std::string>* _toRemoveResList;
   
private:
    ResourceManager();
    ~ResourceManager();
    
public:
    //单例
    static ResourceManager* sharedInstance(void);

    //@重要：绑定laTag的纹理，并交由laTag所在的layer进行管理。

    void bindTexture(const char *pszName,RES_TYPE resmType,LayerTag laTag,
        LIM_OPTION limOpt = LIM_PNG_AS_PNG);

    
    
    //@重要：释放laTag的纹理，即laTag对应的Layer管理下的全部资源。
    void unBindLayerTexture(LayerTag laTag);
    
    //@重要：释放laTag的名为pszName的纹理。
    void unBindTexture(const char *pszName,RES_TYPE resmType,LayerTag laTag);
    
    //绑定初始资源，这是从APK里直接读取，这里的资源永远不要更新。
    void bindInitialImageTexture();

    //绑定公共UI纹理，这个资源不会释放。
    void bindCommonImageTexture();
    
    //增加plist计数。返回true表示首次增加。
    bool addResRef(const std::string& key, RES_TYPE tp, LayerTag tag);
    
    //减少plist计数。返回true表示可以删除。
    bool reduceResRef(const std::string& key, RES_TYPE tp, LayerTag tag);
    
    //通过layerTag获得资源所属的Layer的名称，方便鉴别。
    static const char* getResLayerNameByTag(LayerTag tag);
    
    //为统计MINI版所需文件，添加此文件统计。
    void addFileStat(const char* name, FILE_TYPE tp);
    
    //显示被管理的图形资源概况。
    void showInfo();
    
    //显示引用计数器的概况。
    void showResRefInfo();
    
    //显示文件统计。
    void showFileStatInfo();
    
    //真·奥义·移除纹理。
    void bangkai();
    
private:
    //移除纹理。
    void _removeTexture(std::string& texName);
    
    //整理纹理。主要是将需要加载的纹理从待删除的纹理中移除。
    void _tidyTexture(std::string& texName);
};

#endif
