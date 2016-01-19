//
//  SGShareBox.h
//  GameSanGuo
//
//  Created by dbq on 14-6-4.
//
//

#ifndef __GameSanGuo__SGShareBox__
#define __GameSanGuo__SGShareBox__

#include <iostream>
#include "SGBaseBox.h"
#define SHOW_SHAREBOX_ID 1

enum ShareType{
    SGShareTypeGeneral = 0,
    SGShareTypeVisit,
    SGShareTypeBossBattel,
    SGShareTypeShoujingzhou,
    SGShareTypeGeneralFight,
};
class SGShareBox : public SGBaseBox
{
public:
    ~SGShareBox();
    static SGShareBox *create(SGBoxDelegate *bdg,ShareType type);
    bool init(SGBoxDelegate *bdg,ShareType shareType);
    void boxCloseWithOutCallBack();
    void  onClick1();
    void  onClick2();
    void  onClick3();
    void  onClick4();
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
private:
    ShareType m_shareType;
    std::string m_shareInfo;
    std::string m_sharePictureName;
    std::string creatSharePicture(ShareType shareType);
    bool createShareData();
};

#endif /* defined(__GameSanGuo__SGShareBox__) */
