//
//  SGshoujingzhouEffectLayerCell.cpp
//  GameSanGuo
//
//  Created by kanata on 13-12-19.
//
//

#include "SGshoujingzhouEffectLayerCell.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"

SGshoujingzhouEffectLayerCell::SGshoujingzhouEffectLayerCell()
{
    
}
SGshoujingzhouEffectLayerCell::~SGshoujingzhouEffectLayerCell()
{

}



SGshoujingzhouEffectLayerCell * SGshoujingzhouEffectLayerCell::create(CCArray*array,int number)
{

   SGshoujingzhouEffectLayerCell *item = new SGshoujingzhouEffectLayerCell();

   if (item && item->init())
     {
//         item->vec=vector;
         item->num=number;
         item->datas=array;
         item->initView();
         item->autorelease();
         return item;
     }
    CC_SAFE_RELEASE(item);
     return NULL;
}

void SGshoujingzhouEffectLayerCell::initView()
{

    //头像
    SGMainManager::shareMain() -> addHeadIconPlistByNum(1001,sg_rankLayer);
    item = CCSprite::createWithSpriteFrameName("head1001.png");
    this->addChild(item);


    // 框
    kuang = CCSprite::createWithSpriteFrameName("officerPinzhi_2_0.png");
       
    kuang->setPosition(item->getPosition());
   //装备名字
    label = SGCCLabelTTF::create("fuck", FONT_BOXINFO, 25);
    label->setPosition(ccpAdd(kuang->getPosition(), ccp(kuang->getContentSize().width*0.0, -kuang->getContentSize().height*0.6)));
    this->addChild(kuang);
    this->addChild(label);
       
    //武将装备所属国家
    country = CCSprite::createWithSpriteFrameName("countryName2.png");
    country->setAnchorPoint(ccp(0,1 ));
    country->setPosition(ccpAdd(item->getPosition(), ccp(-item->getContentSize().width*0.5, item->getContentSize().height*0.5)));
    this->addChild(country);

          

     




}



void  SGshoujingzhouEffectLayerCell::setData(SeRewardTypeId*obj)
{
    

    CCString *str; //装备武将等等本体图片
    CCString *str2;//框
    char*str3="";//武将专用标识国家字段
    CCString*str4=NULL;//武将装备名字
    int xx=0;//读plist专用
    CCString *advancenumber=NULL;
    
    SeRewardTypeId*mm=(SeRewardTypeId* )datas->objectAtIndex(num);
    int itemid =mm->getidid();
    int type=  mm->gettypetype();
    
    if (IT_OFFICER == type)
        
    {
        
        SGOfficerDataModel* obj = SGStaticDataManager::shareStatic()->getOfficerById(itemid);;
        
        int starlvl  =obj->getOfficerCurrStarLevel();
        
        SGOfficerCard *card = new SGOfficerCard;
        card->setHeadSuffixNum(obj->getIconId());
        str = CCString::createWithFormat("head%d.png",card->getHeadSuffixNum());
        str3 = (char*)CCString::createWithFormat("countryName%d.png",obj->getOfficerRace())->getCString();
        str2=  CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
        str4=CCString::create(obj->getOfficerName()->getCString());
        xx=card->getHeadSuffixNum();
        SGMainManager::shareMain()->addHeadIconPlistByNum(xx,sg_soulExchangeLayer);
        if (obj->getAdvNum()>0) {
            advancenumber = CCString::createWithFormat("+%d",obj->getAdvNum());
        }
        
    }
    else if (IT_EQUIPMENT == type)
    {
        
        SGEquipmentDataModel* obj = SGStaticDataManager::shareStatic()->getEquipById(itemid);
        
        SGEquipCard *equipCard = new SGEquipCard;
        equipCard->setHeadSuffixNum(obj->getIconId());
        str = CCString::createWithFormat("head%d.png",equipCard->getHeadSuffixNum());
        xx=equipCard->getHeadSuffixNum();
        str2 = CCString::createWithFormat("equipPinzhi_%d_0.png", obj->getEquipStarPinzhi());
        
        str4=CCString::create(obj->getEquipName()->getCString());
        SGMainManager::shareMain()->addHeadIconPlistByNum(xx,sg_soulExchangeLayer);
        if (obj->getAdvNum()>0) {
            advancenumber = CCString::createWithFormat("+%d",obj->getAdvNum());
        }
        
    }
    
    else if (IT_PROPS == type)
    {
        SGPropsDataModel* obj = SGStaticDataManager::shareStatic()->getPropsById(itemid);;
        xx=obj->getIconId();
        str=CCString::createWithFormat("head%d.png",obj->getIconId());
        std:: string ch=("jinkuang.png");
        str2 = CCString::createWithFormat("officerPinzhi_%d_0.png",obj->getPropsCurrStarLevel() );
        str4=CCString::create(obj->getPropsName()->getCString());
        SGMainManager::shareMain()->addHeadIconPlistByNum(xx,sg_soulExchangeLayer);
    }
    else if (IT_PIECES == type)  /////不知道会不会掉碎片
    {
        SGPiecesDataModel* temp = SGStaticDataManager::shareStatic()->getPiecesDictById(itemid);
        if (temp->getCallingCardId() == -1)// 通用碎片//common_pieces_border
        {
            ResourceManager::sharedInstance()->bindTexture("sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_shoujingzhou);
            str = CCString::createWithFormat("common_pieces_border.png");
        }
        else if (temp->getPiecesType() == 0)//武将碎片
        {
            
            SGOfficerDataModel *of = SGStaticDataManager::shareStatic()->getOfficerById(temp->getCallingCardId());
            SGMainManager::shareMain() -> addHeadIconPlistByNum(of->getIconId(),sg_storyLayer);
            str = CCString::createWithFormat("head%d.png", of->getIconId());
            int tag=of->getOfficerVisitType();
            if (tag>1) {
                tag  =tag-1;
            }
            str2 = CCString::createWithFormat("officerPinzhi_%d_0.png",tag );
            
            
        }
        else if (temp->getPiecesType() == 1)//装备碎片
        {
            
            SGEquipmentDataModel *eq = SGStaticDataManager::shareStatic()->getEquipById(temp->getCallingCardId());
            SGMainManager::shareMain() -> addHeadIconPlistByNum(eq->getIconId(),sg_storyLayer);
            str = CCString::createWithFormat("head%d.png", eq->getIconId());
            
            int tag=eq->getEquipStarPinzhi();
            if (tag>1) {
                tag  =tag-1;
            }
            
            str2 = CCString::createWithFormat("equipPinzhi_%d_0.png",tag);
        }
        str4=CCString::create(temp->getPiecesName()->getCString());
        //str4=CCString
        
    }
    
     item->setDisplayFrame(CCSprite::createWithSpriteFrameName(str->getCString())->displayFrame());
     kuang->setDisplayFrame(CCSprite::createWithSpriteFrameName(str2->getCString())->displayFrame());
     label->setString(str4->getCString());
    
    if(*str3)
    {
        item->setDisplayFrame(CCSprite::createWithSpriteFrameName(str3)->displayFrame());
        
    }
    else
    {
        country->setVisible(false);
    }
    if (advancenumber) {
        kuang->removeChildByTag(250);
        SGCCLabelTTF *ad = SGCCLabelTTF::create(advancenumber->getCString(), FONT_PANGWA, 24);
        ad->setInsideColor(ccGREEN);
        ad->setAnchorPoint(ccp(1,1));
        ad->ignoreAnchorPointForPosition(false);
        ad->setPosition(ccp(kuang->getContentSize().width*0.8,kuang->getContentSize().height - ad->getContentSize().height));
        kuang->addChild(ad,250,250);
    }


}