//
//  SGRewardSubLayer.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-29.
//
//

#include "SGRewardSubLayer.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#include "SGPropRewardBox.h"
#include "SGGeneralInfoLayer.h"
#include "SGEquipInfoLayer.h"
#include "SGMenu.h"
#include "SGStringConfig.h"
#include "SGCommonInfoBox.h"
#define WIDTH_VIEW 270
#define  totalNum  15

#define ITEMSGAP 100
SGRewardSubLayer ::SGRewardSubLayer():dayNum(0),
officerID(1116),
allObject(NULL),
showType(0),
_viewSize(CCSizeZero),
index(0),
currentDay(0),//当前是第几天
isFinish(0),//是否完成
leftday(0),//剩余天数
day1_label(NULL),
day2_label(NULL),
day3_label(NULL),
day4_label(NULL),
menuNew(NULL)
{
    allObject=CCArray::create();
    allObject->retain();
}
SGRewardSubLayer ::~SGRewardSubLayer(){

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_rewardSubLayer);
}

SGRewardSubLayer * SGRewardSubLayer::create(int leftDay,int currentDay,int isFinish,CCArray *allObj,CCPoint position,CCSize viewSize){

    SGRewardSubLayer *subLayer=new SGRewardSubLayer();
    subLayer->_viewSize=viewSize;
    
    subLayer->index=position.x;
    if (subLayer && subLayer->init(NULL, sg_rewardSubLayer)) {
        subLayer->allObject=allObj;
        subLayer->currentDay=currentDay;
        subLayer->isFinish=isFinish;
        subLayer->leftday=leftDay;
        subLayer->constructView();
        subLayer->autorelease();
        return subLayer;
    }
    CC_SAFE_DELETE(subLayer);
         return NULL;
    
    
    
}
void SGRewardSubLayer::constructView(){
  
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_rewardSubLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer2.plist", RES_TYPE_LAYER_UI, sg_rewardSubLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_rewardSubLayer);
    ResourceManager::sharedInstance()->bindTexture("sgpieceslayer/sgpieceslayer.plist", RES_TYPE_LAYER_UI, sg_rewardSubLayer);

    
    //一个大button，多个button 
    
    if (allObject->count()==1) {
        showType  =2;
    }else
        showType=2;
    
    ccColor3B color;
    color.r=29;
    color.g=255;
    color.b=34;

    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(_viewSize.width, _viewSize.height));
    this->addChild(frame2);
    frame2->setPosition(ccp(WIDTH_VIEW, 0));
    frame2->setAnchorPoint(ccp(0.5, 0));
    
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg, -1);
    frame2bg->setScaleX(_viewSize.width/frame2bg->getContentSize().width);
    frame2bg->setScaleY(_viewSize.height/frame2bg->getContentSize().height);
    frame2bg->setPosition(frame2->getPosition());
    frame2bg->setAnchorPoint(ccp(0.5, 0));

    
    CCSize size=CCDirector::sharedDirector()->getWinSize();
      menuNew=SGMenu::create(CCRectMake(0, 200, size.width, size.height));
    
    
    if (showType==1) {
//         bgbutton->setEnabled(true);
         LoginRewardObj *obj=(LoginRewardObj *)allObject->objectAtIndex(0);
        if (obj->getItemType()==6) {
            
           SGOfficerDataModel  *_card = SGStaticDataManager::shareStatic()->getOfficerById(obj->getItemId());
            SGMainManager::shareMain()->addOfficerPng(_card->getIconId(),sg_rewardSubLayer);
            CCString *wjStr=CCString::createWithFormat("officer_%d.png",_card->getIconId());
            CCSprite *wj_sprite=CCSprite::createWithSpriteFrameName(wjStr->getCString());
            this->addChild(wj_sprite,2);
            wj_sprite->setPosition(ccp(WIDTH_VIEW-20, _viewSize.height/2-120));
            wj_sprite->setAnchorPoint(ccp(0.5, 0));
            
            
            
            int num=_card->getOfficerCurrStarLevel();
            
            for(int i=0; i < num;i++)
            {
                CCSprite *sprite = CCSprite::createWithSpriteFrameName("xx.png");
                sprite->setAnchorPoint(ccp(0.5, 1));
                sprite->setPosition(ccpAdd(ccp(WIDTH_VIEW+250,290), ccp(-sprite->getContentSize().width*i - 17,4)));
                this->addChild(sprite,3);
            }
        }
        else if (obj->getItemType()==7)
        {
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(obj->getItemId());
            SGMainManager::shareMain()->addEquipPng(temp->getIconId(),sg_rewardSubLayer);
            CCString *wjStr=CCString::createWithFormat("equip%d.png",temp->getIconId());
            CCSprite *wj_sprite=CCSprite::createWithSpriteFrameName(wjStr->getCString());
            this->addChild(wj_sprite,3);
            wj_sprite->setPosition(ccp(WIDTH_VIEW-20, _viewSize.height/2-120));
            wj_sprite->setAnchorPoint(ccp(0.5, 0));

        }
        else if (obj->getItemType()==9)
        {
            SGPropsDataModel *temp=SGStaticDataManager::shareStatic()->getPropsById(obj->getItemId());
            SGMainManager::shareMain()->addPropPng(temp->getIconId(),sg_rewardSubLayer);
            CCString *wjStr=CCString::createWithFormat("head%d.png",temp->getIconId());
            CCSprite *wj_sprite=CCSprite::createWithSpriteFrameName(wjStr->getCString());
            this->addChild(wj_sprite,3);
            wj_sprite->setPosition(ccp(WIDTH_VIEW-20, _viewSize.height/2-120));
            wj_sprite->setAnchorPoint(ccp(0.5, 0));
        }else if (obj->getItemType()==10)
        {
            
            
            SGPiecesDataModel *_card=SGStaticDataManager::shareStatic()->getPiecesDictById(obj->getItemId());
            int iconId;
            if (_card->getPiecesType()==0) {
                SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getCallingCardId());
                iconId = temp->getIconId();

            }else if (_card->getPiecesType()==1)
            {
                SGEquipmentDataModel *temp =SGStaticDataManager::shareStatic()->getEquipById(_card->getCallingCardId());
                iconId = temp->getIconId();
            }
            SGMainManager::shareMain()->addHeadIconPlistByNum(iconId, sg_rewardSubLayer);
            
            CCString *wjStr=CCString::createWithFormat("head%d.png",iconId);
            CCSprite *wj_sprite=CCSprite::createWithSpriteFrameName(wjStr->getCString());
            this->addChild(wj_sprite,3);
            wj_sprite->setPosition(ccp(WIDTH_VIEW-20, _viewSize.height/2-120));
            wj_sprite->setAnchorPoint(ccp(0.5, 0));
        }
        else if (obj->getItemType()==0)
        {
            CCString *wjStr=CCString::createWithFormat("big_yb.png");
            CCSprite *wj_sprite=CCSprite::createWithSpriteFrameName(wjStr->getCString());
            this->addChild(wj_sprite,3);
            wj_sprite->setPosition(ccp(WIDTH_VIEW-20, _viewSize.height/2-120));
            wj_sprite->setAnchorPoint(ccp(0.5, 0));
//             bgbutton->setEnabled(false);
        }else if (obj->getItemType()==1)
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9999,sg_totalLoginReward);
            
            CCString *wjStr=CCString::createWithFormat("head9999.png");
            CCSprite *wj_sprite=CCSprite::createWithSpriteFrameName(wjStr->getCString());
            this->addChild(wj_sprite,3);
            wj_sprite->setPosition(ccp(WIDTH_VIEW-20, _viewSize.height/2-120));
            wj_sprite->setAnchorPoint(ccp(0.5, 0));
//             bgbutton->setEnabled(false);
        }else if (obj->getItemType()==4)
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9998,sg_totalLoginReward);
            
            
            CCString *wjStr=CCString::createWithFormat("head9998.png");
            CCSprite *wj_sprite=CCSprite::createWithSpriteFrameName(wjStr->getCString());
            this->addChild(wj_sprite,3);
            wj_sprite->setPosition(ccp(WIDTH_VIEW-20, _viewSize.height/2-120));
            wj_sprite->setAnchorPoint(ccp(0.5, 0));
//             bgbutton->setEnabled(false);
        }else if (obj->getItemType()==5)
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9996,sg_totalLoginReward);
            
            
            CCString *wjStr=CCString::createWithFormat("head9996.png");
            CCSprite *wj_sprite=CCSprite::createWithSpriteFrameName(wjStr->getCString());
            this->addChild(wj_sprite,3);
            wj_sprite->setPosition(ccp(WIDTH_VIEW-20, _viewSize.height/2-120));
            wj_sprite->setAnchorPoint(ccp(0.5, 0));
//             bgbutton->setEnabled(false);
        }
        else if (obj->getItemType()==15)
        {
            SGConsumableDataModel *consume = SGStaticDataManager::shareStatic()->getConsumableById(obj->getItemId());
            SGMainManager::shareMain()->addHeadIconPlistByNum(consume->getIconId(), sg_totalLoginReward);
            int starLvl = consume->getConsumeStarLvl();
            if (starLvl > 1)
                starLvl -= 1;
            
            CCString *wjStr=CCString::createWithFormat("head%d.png", consume->getIconId());
            CCSprite *wj_sprite=CCSprite::createWithSpriteFrameName(wjStr->getCString());
            this->addChild(wj_sprite,3);
            wj_sprite->setPosition(ccp(WIDTH_VIEW-20, _viewSize.height/2-120));
            wj_sprite->setAnchorPoint(ccp(0.5, 0));
        }else if (obj->getItemType()==16)
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(9996,sg_totalLoginReward);
            
            CCString *wjStr=CCString::createWithFormat("head9996.png");
            CCSprite *wj_sprite=CCSprite::createWithSpriteFrameName(wjStr->getCString());
            this->addChild(wj_sprite,3);
            wj_sprite->setPosition(ccp(WIDTH_VIEW-20, _viewSize.height/2-120));
            wj_sprite->setAnchorPoint(ccp(0.5, 0));
//             bgbutton->setEnabled(false);
        }
         ;
        SGCCLabelTTF *reward_countSprite=SGCCLabelTTF::create(CCString::createWithFormat("X %d",obj->getRewardCount())->getCString(), FONT_PANGWA, 25 , ccGREEN);
        //modify by:zyc. merge into create.
        //reward_countSprite->setColor(ccGREEN);
        this->addChild(reward_countSprite,10);
        
        reward_countSprite->setPosition(ccp(WIDTH_VIEW-20, _viewSize.height/2-130 ));

    }else
        
    {
//        bgbutton->setEnabled(false);
        for (int i=0; i<allObject->count(); i++) {
            
            LoginRewardObj *obj=(LoginRewardObj *)allObject->objectAtIndex(i);
            
            CCString *kuang_string=NULL;
            CCString *reward_string=NULL;
            CCString *reward_count=NULL;
            CCString *reward_country=NULL;
            SGMainManager *sgMainManager=SGMainManager::shareMain();
            SGBaseItemType bit = (SGBaseItemType) obj->getItemType();
            switch (bit)
            {
                case BIT_OFFICER:
                {
                    SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(obj->getItemId());
                    sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_totalLoginReward);
                    
                    int starlvl = temp->getOfficerCurrStarLevel();
                    if (starlvl>1) {
                        starlvl-=1;
                    }
  
                    kuang_string=CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
                    reward_string=CCString::createWithFormat("head%d.png",temp->getIconId());
//                    reward_count=CCString::createWithFormat("%sx%d",temp->getOfficerName()->getCString(),obj->getRewardCount());
//                     reward_count=CCString::createWithFormat("X %d ",obj->getRewardCount());
                    reward_count=CCString::createWithFormat("%s",temp->getOfficerName()->getCString());
                    reward_country=CCString::createWithFormat("countryName%d.png",temp->getOfficerRace());
                  
                    
                }
                    break;
                case BIT_EQUIP:
                {
                    SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(obj->getItemId());
                    sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_totalLoginReward);
                    
                    int starlvl = temp->getEquipStarPinzhi();
                    kuang_string=CCString::createWithFormat("equipPinzhi_%d_0.png",starlvl);
                    reward_string=CCString::createWithFormat("head%d.png",temp->getIconId());
//                    reward_count=CCString::createWithFormat("%sx%d",temp->getEquipName()->getCString(),obj->getRewardCount());
                    reward_count=CCString::createWithFormat("X %d",obj->getRewardCount());
                    reward_count=CCString::createWithFormat("%s",temp->getEquipName()->getCString());
                    
                }
                    break;
                case BIT_PROP:
                {
                    SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(obj->getItemId());
                    sgMainManager->addHeadIconPlistByNum(temp->getIconId(),sg_totalLoginReward);
                    
                    kuang_string=CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getPropsCurrStarLevel());
                    reward_string=CCString::createWithFormat("head%d.png",temp->getIconId());
//                    reward_count=CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),obj->getRewardCount());
//                    reward_count=CCString::createWithFormat("X %d",obj->getRewardCount());
                    reward_count=CCString::createWithFormat("%s",temp->getPropsName()->getCString());
                }
                    break;
                case BIT_PIECE:
                {
                    SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(obj->getItemId());
                    
                    int starLv = pieces->getStarLevel();
                    if (starLv > 1) //策划表的蓝色为4，但是客户端的图形里标识为3，需要减1。
                    {
                        starLv -= 1;
                    }
                    
                    int piecesType=pieces->getPiecesType();
                    if (piecesType==0)
                    {
                        SGOfficerDataModel *officerData = SGStaticDataManager::shareStatic()->getOfficerById(pieces->getCallingCardId());
                        if (officerData)
                        {
                            
                            SGMainManager::shareMain()->addHeadIconPlistByNum(officerData->getIconId(), sg_totalLoginReward);
//                            int starlvl = officerData->getOfficerCurrStarLevel();
                            kuang_string=CCString::createWithFormat("officerPinzhi_%d_%d.png",starLv, 0);
                            reward_string=CCString::createWithFormat("head%d.png",officerData->getIconId());
                            reward_count = CCString::createWithFormat("%s", pieces->getPiecesName()->getCString());
                            reward_country=CCString::createWithFormat("countryName%d.png",officerData->getOfficerRace());

                        }
                        
                    }
                    else if (piecesType==1)
                    {
                        SGEquipmentDataModel *equip = SGStaticDataManager::shareStatic()->getEquipById(pieces->getCallingCardId());
                        SGMainManager::shareMain()->addHeadIconPlistByNum(equip->getIconId(), sg_totalLoginReward);
//                        int starlvl = equip->getEquipStarPinzhi();
                        kuang_string=CCString::createWithFormat("equipPinzhi_%d_0.png",starLv);
                        reward_string=CCString::createWithFormat("head%d.png",equip->getIconId());
                        reward_count=CCString::createWithFormat("%s", pieces->getPiecesName()->getCString());

                    }

                }
                    break;
                case BIT_CONSUMABLE:
                {
                    SGConsumableDataModel *consume = SGStaticDataManager::shareStatic()->getConsumableById(obj->getItemId());
                    SGMainManager::shareMain()->addHeadIconPlistByNum(consume->getIconId(), sg_totalLoginReward);
                    int starLvl = consume->getConsumeStarLvl();
                    if (starLvl > 1)
                        starLvl -= 1;
                    kuang_string=CCString::createWithFormat("equipPinzhi_%d_0.png", starLvl);
                    reward_string=CCString::createWithFormat("head%d.png", consume->getIconId());
                    //                    reward_count=CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),obj->getRewardCount());
                    reward_count=CCString::createWithFormat("%s",consume->getConsumeName()->getCString());
                }
                    break;
                case BIT_MEDAL:
                {
                    
                    ResourceManager::sharedInstance()->bindTexture("sgtongquelayer/sgtongquelayer.plist", RES_TYPE_LAYER_UI, sg_rewardSubLayer);
                    kuang_string=CCString::create("equipPinzhi_3_0.png");
                    reward_string=CCString::createWithFormat("tongqueXunBig.png");
                    //                    reward_count=CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),obj->getRewardCount());
                    reward_count=CCString::createWithFormat("X %d",obj->getRewardCount());                }
                    break;
                case BIT_INGOT:
                {
                    
                    kuang_string=CCString::create("equipPinzhi_4_0.png");
                    reward_string=CCString::createWithFormat("big_yb.png");
                    //                    reward_count=CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),obj->getRewardCount());
                    reward_count=CCString::createWithFormat("X %d",obj->getRewardCount());
                }
                    break;
                case BIT_GOLD:
                {
                    sgMainManager->addHeadIconPlistByNum(9999,sg_totalLoginReward);
                   
                    kuang_string=CCString::create("equipPinzhi_3_0.png");
                    reward_string=CCString::createWithFormat("head9999.png");
                    //                    reward_count=CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),obj->getRewardCount());
                    reward_count=CCString::createWithFormat("X %d",obj->getRewardCount());
                }
                    break;
                case BIT_HORN:
                {
                    sgMainManager->addHeadIconPlistByNum(9998,sg_totalLoginReward);
                    
                    kuang_string=CCString::create("equipPinzhi_3_0.png");
                    reward_string=CCString::createWithFormat("head9998.png");
                    //                    reward_count=CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),obj->getRewardCount());
                    reward_count=CCString::createWithFormat("X %d",obj->getRewardCount());
                }
                    break;
                case BIT_RECIPE:
                {
                    sgMainManager->addHeadIconPlistByNum(9996,sg_totalLoginReward);
                    
                    kuang_string=CCString::create("equipPinzhi_3_0.png");
                    reward_string=CCString::createWithFormat("head9996.png");
                    //                    reward_count=CCString::createWithFormat("%sx%d",temp->getPropsName()->getCString(),obj->getRewardCount());
                    reward_count=CCString::createWithFormat("X %d",obj->getRewardCount());
                }
                    break;
                //新增材料支持。
                case BIT_MATERIAL:
                {
                    //获取材料的原型
                    SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(obj->getItemId());
                    
                    if (materialModel)
                    {
                        sgMainManager->addHeadIconPlistByNum(materialModel->getMaterialIconId(), sg_totalLoginReward);
                        
                        reward_string = CCString::createWithFormat("head%d.png", materialModel->getMaterialIconId());
                        //边框颜色
                        int borderColor = materialModel->getMaterialBorderColor();
                        if (borderColor > 1)
                        {
                            borderColor -= 1;
                        }
                        kuang_string = CCString::createWithFormat("equipPinzhi_%d_0.png", borderColor);
                        reward_count = CCString::createWithFormat("%s",materialModel->getMaterialName()->getCString());
                    }
                    
                    break;
                }
                default:
                    break;
            }
 
            SGButton *buttonItem=SGButton::create(reward_string->getCString(),kuang_string->getCString() , this, menu_selector(SGRewardSubLayer::showRewardInfo));
            this->addBtn(buttonItem);
            buttonItem->setTag(i);
            buttonItem->setEnabled(true);
            buttonItem->setPosition(ccp( ITEMSGAP/2+30 +(ITEMSGAP/2*3-15)*i+10 ,_viewSize.height/2+10));
            //如果奖励是碎片，加上蒙版
            if (obj->getItemType() == 10)
            {
                CCSprite *mask = CCSprite::createWithSpriteFrameName("pieces_mask.png");
                this->addChild(mask, 11);
                mask->setPosition(ccpAdd(buttonItem->getPosition(), ccp(0, 0)));
                
                SGCCLabelTTF *rewardCount = SGCCLabelTTF::create(CCString::createWithFormat("X %d", obj->getRewardCount())->getCString(),
                                                                 FONT_PANGWA, 21);
                rewardCount->setInsideColor(ccGREEN);
                this->addChild(rewardCount, 12);
                rewardCount->setPosition(ccpAdd(mask->getPosition(), ccp(mask->getContentSize().width / 2 - rewardCount->getContentSize().width / 2,
                                                                         -mask->getContentSize().height / 2 + rewardCount->getContentSize().height / 2)));
                
            }
            
            
            
            CCSprite *count_bg1=CCSprite::createWithSpriteFrameName("reward_namebgl.png");
            this->addChild(count_bg1,10);
            
            CCSprite *count_bg2=CCSprite::createWithSpriteFrameName("reward_namebgc.png");
            this->addChild(count_bg2,10);
            
            
            CCSprite *count_bg3=CCSprite::createWithSpriteFrameName("reward_namebgl.png");
            this->addChild(count_bg3,10);
            count_bg3->setFlipX(true);
            
//            count_bg2->setSkewX(14.5);
            
            
            unsigned int flength = reward_count->length();
            unsigned int flen=flength;
            if (obj->getItemType()== 6 ||obj->getItemType()== 7 ||obj->getItemType()== 9 ||obj->getItemType()== 10 ||obj->getItemType()== 16 ||obj-> getItemType()== 15) {
                 flen = flength / 3 + flength % 3;
            }else
            {
                flen=flength / 2 + flength % 2;
            }
           
           
            float scaleX=1.0;
            if (flen <= 2)
            {
                count_bg2->setScaleX(1.0);
                
                scaleX=1.0;
 
            }
            else if (flen <= 3)
            {
                count_bg2->setScaleX(1.2);
                scaleX=1.2;
                
 
            }else
            {
                count_bg2->setScaleX(1.6);
                scaleX=1.6;
 
            }
            
            
            count_bg1->setPosition(ccp( ITEMSGAP/2+30-count_bg2->getContentSize().width*(scaleX/2+0.07) +(ITEMSGAP/2*3-15)*i+10 ,_viewSize.height/2-60));
            count_bg2->setPosition(ccp( ITEMSGAP/2+30 +(ITEMSGAP/2*3-15)*i+10 ,_viewSize.height/2-60));
            count_bg3->setPosition(ccp( ITEMSGAP/2+30+count_bg2->getContentSize().width*(scaleX/2+0.07) +(ITEMSGAP/2*3-15)*i+10 ,_viewSize.height/2-60));
            
            count_bg1->setColor(ccBLACK);
            count_bg2->setColor(ccBLACK);
            count_bg3->setColor(ccBLACK);
            
            
            
          
            
            CCSprite *kuang_bg=CCSprite::createWithSpriteFrameName("card_bj.png");
            buttonItem->addChild(kuang_bg,-100);
            kuang_bg->setAnchorPoint(ccp(0, 0));
            kuang_bg->setOpacity(166);
            
            if (obj->getItemType()== 6 ||obj->getItemType()== 7 ||obj->getItemType()== 9 ||obj->getItemType()== 10 ||obj->getItemType()== 16 ||obj-> getItemType()== 15){
           
                SGCCLabelTTF *reward_countSprite=SGCCLabelTTF::create(reward_count->getCString(), FONT_PANGWA, 20 , ccWHITE , ccBLACK);
//modify by:zyc . merge into create.
//                reward_countSprite->setInsideColor(ccWHITE);
//                reward_countSprite->setOutSideColor(ccBLACK);
                this->addChild(reward_countSprite,10);
                reward_countSprite->setPosition(ccp( ITEMSGAP/2+30 +(ITEMSGAP/2*3-15)*i+10 ,_viewSize.height/2-60));
               
            }else
                {
                    SGCCLabelTTF *reward_countSprite=SGCCLabelTTF::create(reward_count->getCString(), FONT_PANGWA, 24 , color , ccBLACK);
//modify by:zyc . merge into create.
//                    reward_countSprite->setInsideColor(color);
//                    reward_countSprite->setOutSideColor(ccBLACK);
                    this->addChild(reward_countSprite,10);
                    reward_countSprite->setPosition(ccp( ITEMSGAP/2+30 +(ITEMSGAP/2*3-15)*i+10 ,_viewSize.height/2-60));
                
              
                }
            
           
            if (reward_country!=NULL) {
                
                CCSprite *country_sprite=CCSprite::createWithSpriteFrameName(reward_country->getCString());
                this->addChild(country_sprite,10);
                country_sprite->setPosition(ccp((ITEMSGAP/2*3-15)*i+52 ,_viewSize.height/2+buttonItem->getContentSize().height/2-5));
            }
        }
    }
    
    
    CCSprite *day_rewardBg=CCSprite::createWithSpriteFrameName("reward_daybg.png");
    this->addChild(day_rewardBg);
    day_rewardBg->setPosition(ccp(WIDTH_VIEW+150, 150));
    
    CCSprite *day_rewardBg2=CCSprite::createWithSpriteFrameName("reward_day.png");
    this->addChild(day_rewardBg2);
    day_rewardBg2->setPosition(ccp(WIDTH_VIEW+220, 150));
    
    CCString *dayStr=CCString::createWithFormat("%d",currentDay);
    SGCCLabelTTF *day_reward_label=SGCCLabelTTF::create(dayStr->getCString(), FONT_PANGWA, 45 , ccWHITE , ccBLACK);
    this->addChild(day_reward_label,3);
//modify by:zyc . merge into create.
//    day_reward_label->setOutSideColor(ccBLACK);
//    day_reward_label->setInsideColor(ccWHITE);
    day_reward_label->setPosition(ccp(WIDTH_VIEW+140, 130));
    day_reward_label->setAnchorPoint(ccp(0.5, 0));

    
//    SGCCLabelTTF *dj_label=SGCCLabelTTF::create("点击查看", FONT_PANGWA, 30);
//    this->addChild(dj_label,3);
//    dj_label->setColor(ccWHITE);
//    dj_label->setPosition(ccp(WIDTH_VIEW-200, 10));
//    dj_label->setAnchorPoint(ccp(0.5, 0));
    
//    menu=CCMenu::create();
    
    this->addChild(menuNew,1);
    menuNew->setPosition(CCPointZero);
    if (isFinish==-1)
    {
            
            CCSprite *liqu_bottom=CCSprite::createWithSpriteFrameName("reward_login_yilingqu.png");
            liqu_bottom->setPosition(ccp(WIDTH_VIEW+80, 10));
            liqu_bottom->setAnchorPoint(ccp(0, 0));
            this->addChild(liqu_bottom,3);
    }else if(isFinish==0)
    {
        menu->setZOrder(5);
        lq_btn=SGButton::create("store_exchangebtnbg.png", "reward_login_lingqu.png", this, menu_selector(SGRewardSubLayer::getRewardHandle));
        this->addBtn(lq_btn);
        lq_btn->setZOrder(1);
        lq_btn->setPosition(ccp(WIDTH_VIEW+150, 50));
    }else
    {
        day1_label=SGCCLabelTTF::create(str_RewardRegLayer_str7, FONT_PANGWA, 24 , ccWHITE);
        this->addChild(day1_label,3);
        //modify by:zyc . merge into create.
        //day1_label->setColor(ccWHITE);
        day1_label->setPosition( ccp(WIDTH_VIEW+120, 40));
        day1_label->setAnchorPoint(ccp(0.5, 0));
        
        
        CCString *numStr=CCString::createWithFormat("%d",leftday);
        day2_label=SGCCLabelTTF::create(numStr->getCString(), FONT_PANGWA, 33 , color , ccBLACK);
        this->addChild(day2_label,3);
        //modify by:zyc . merge into create.
        //day2_label->setOutSideColor(ccBLACK);
        //day2_label->setInsideColor(color);
        day2_label->setPosition(ccp(WIDTH_VIEW+183, 40));
        day2_label->setAnchorPoint(ccp(0.5, 0));
        
        day3_label=SGCCLabelTTF::create(str_RewardRegLayer_str8, FONT_PANGWA, 24 , ccWHITE);
        this->addChild(day3_label,3);
        //modify by:zyc . merge into create.
        //day3_label->setColor(ccWHITE);
        day3_label->setPosition(ccp(WIDTH_VIEW+235, 40));
        day3_label->setAnchorPoint(ccp(0.5, 0));
        
        day4_label=SGCCLabelTTF::create(str_RewardRegLayer_str9, FONT_PANGWA, 24 , ccWHITE);
        this->addChild(day4_label,3);
        //modify by:zyc . merge into create.
        //day4_label->setColor(ccWHITE);
        day4_label->setPosition(ccp(WIDTH_VIEW+175, 10));
        day4_label->setAnchorPoint(ccp(0.5, 0));
        
    }
    
    
}


void SGRewardSubLayer::selectSubview(){
    
    CCLog("select item");
}
void SGRewardSubLayer::getRewardHandle(){
  
    SUBLAYRTAG=this->getTag();
    main::RewardLoginAwardRequest *request = new main::RewardLoginAwardRequest;
    request->set_day(currentDay);
    SGSocketClient::sharedSocketClient()->send(MSG_GETREWARD_LOGIN, request);
    
}



void SGRewardSubLayer::showRewardInfo(CCObject *sender){
    SGButton *button=(SGButton *)sender;
    int tag=button->getTag();
    if (showType==2 && button->getTag()==3001) {
        return;
    }else if(showType==2)// 多个的情况
    {
        LoginRewardObj *obj=(LoginRewardObj *)allObject->objectAtIndex(tag);
        
        showGenerOrEquipInfo(obj);
       
    }
    else if(showType==1)
    {
        LoginRewardObj *obj=(LoginRewardObj *)allObject->objectAtIndex(0);
        showGenerOrEquipInfo(obj);
    }
    
}

void SGRewardSubLayer::showGenerOrEquipInfo(LoginRewardObj *obj)
{
    SGBaseMilitaryCard* bmc = NULL;
    if (obj->getItemType()==6) //武将
    {
        SGOfficerCard *officerCard =SGStaticDataManager::shareStatic()->officerDataModeToOfficer(obj->getItemId());
        bmc = officerCard;
//        SGMainManager::shareMain()->showGeneralInfoLayer(officerCard, 25,NULL,false);
    }
    else if(obj->getItemType()==7) //装备
    {
        SGEquipCard *equipCard=SGStaticDataManager::shareStatic()->equipDataModeToequip(obj->getItemId());
        bmc = equipCard;
//        SGMainManager::shareMain()->showEquipInfoLayer(equipCard, 25,false);
    }
    else if(obj->getItemType()==9) //道具
    {
        SGPropsCard *propsCard=SGStaticDataManager::shareStatic()->propDataModelToProp(obj->getItemId());
        bmc = propsCard;
//        SGMainManager::shareMain()->showPropInfoLayer(propsCard, 25,false);
    }
    else if(obj->getItemType()==10) //碎片
    {
        SGPiecesCard *piecesCard=SGStaticDataManager::shareStatic()->piecesDataModelToPiece(obj->getItemId());
        bmc = piecesCard;
//        SGMainManager::shareMain()->showPiecesInfoLayer(piecesCard, 25, false);
    }
    else if(obj->getItemType() ==15) //消耗品
    {
        SGConsumableCard *consumCard=SGStaticDataManager::shareStatic()->consumableDataModelToConsumable(obj->getItemId());
        bmc = consumCard;
//        SGMainManager::shareMain()->showConsumableInfoLayer(consumCard,false,25);
    }
    
    if (bmc)
    {
        //这是sublayer，需要从scene中获取SGTotalLoginRewardLayer
        SGTotalLoginRewardLayer* tlrl = dynamic_cast<SGTotalLoginRewardLayer*>(SGMainManager::shareMain()->getNowShowLayer());
        if (tlrl)
        {
            SGMainManager::shareMain()->showCommonInfoBoxInProto(bmc, tlrl, 25, false, CIBET_TOTAL_LOGIN_REWARD);
        }
    }

}


//void SGRewardSubLayer::onEnter(){
//    SGBaseLayer::onEnter();
//    //    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-1, true);
//}
//
//void SGRewardSubLayer::onExit(){
//    SGBaseLayer::onExit();
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
//}
//
//bool SGRewardSubLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    
//    return true;
//}
//void SGRewardSubLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//    
//}


