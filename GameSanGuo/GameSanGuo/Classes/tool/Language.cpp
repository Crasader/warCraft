//
//  Language.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-31.
//
//

#include "Language.h"
const char * Language::getStringByKey(const char *key)
{
    CCDictionary *pDic = NULL;
    ccLanguageType ntype = CCApplication::sharedApplication()->getCurrentLanguage();
    switch (ntype) {
        case kLanguageChinese:
        {
            pDic = CCDictionary::createWithContentsOfFile("zh-Hans.plist");  
        }
            break;
            
        default:
            break;
    }
    std::string keyStr = key;
    if (pDic != NULL) {
        return (pDic -> valueForKey(keyStr))->getCString();
    }
    else
    {
        return "Language dic did not exist";
    }
    
}