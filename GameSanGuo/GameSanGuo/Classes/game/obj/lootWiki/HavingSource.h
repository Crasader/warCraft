#include "cocos2d.h"
#include "CCPlatformMacros.h"
#ifndef __GameSanGuo__HavingSource__
#define __GameSanGuo__HavingSource__

using namespace cocos2d;
using namespace std;

class HavingSource : public CCObject
{
    CC_SYNTHESIZE(int , _lootType ,LootType );
    CC_SYNTHESIZE(int , _lootParam , LootParam );
    CC_SYNTHESIZE(std::string , _lootDesc , LootDesc);
    CC_SYNTHESIZE(int , _lootPriority, LootPriority);
    
public:
    HavingSource() :_lootType(0) , _lootParam(0) , _lootPriority(0)
    {
        
    }
    CREATE_FUNC(HavingSource);
    inline bool init(){    return true; }

};


#endif //------end.

