//
//  SGRankDetailLayer.cpp
//  GameSanGuo
//
//  Created by kanata on 14-2-12.
//
//

#include "SGRankDetailLayer.h"
#include "SGMainScene.h"
#include "SGMainManager.h"
#include "SGRankDetailCell.h"
#include "SGMenu.h"
#include "SGSoldierItem.h"
#include "SGStaticDataManager.h"
#include "SGStringConfig.h"
#include "SGMyFriend.h"

DPI::DPI()
:m_gmid(0),
m_playid(0),
m_name(NULL)
{
}
DPI::~DPI()
{
}


SGRankDetailLayer::SGRankDetailLayer()
:officedarray(NULL),
soldierarray(NULL),
_player(NULL),
entertype(0),////0为跨服，非0为本服
countrymm(0)
{
    
}

SGRankDetailLayer::~SGRankDetailLayer()
{
    

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_rankDetailLayer);
    CC_SAFE_RELEASE(_player);
}


SGRankDetailLayer *SGRankDetailLayer::create(SGGamePlayerInfo*player,int type)
{
    SGRankDetailLayer *layer = new SGRankDetailLayer();
    
    CCSize s=CCDirector::sharedDirector()->getWinSize();
    
    if (layer && layer->init(NULL, box_rank,CCSizeMake(620,650)))
    {
        layer->entertype=type;
        layer->initView(player,type);
        
        layer->autorelease();
        return layer;
    }
    CC_SAFE_RELEASE(layer);
    return NULL;
}


void SGRankDetailLayer::initView(SGGamePlayerInfo*player,int type)
{

    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer* mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGMyFriend* myLayer = (SGMyFriend *)mainScene->getChildByTag(sg_myFriend);
    if(myLayer)
    {
        
       
        myLayer->tableView->setIsTouchEnabled(false);
       
    }
    mainLayer->setIsCanTouch(false);
    mainLayer->setPortraitMenuCanTouch(false);
    SGBaseLayer *layer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (layer != NULL)
    {
        layer->setIsCanTouch(false);
    }
    
    ResourceManager::sharedInstance()->bindTexture("sgchat/sgchat.plist", RES_TYPE_LAYER_UI, sg_box_rankDetailLayer);
    ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_box_rankDetailLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist" , RES_TYPE_LAYER_UI, sg_box_rankDetailLayer);
    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-3303.plist" , RES_TYPE_LAYER_UI, sg_box_rankDetailLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/ofcNewUi.plist" , RES_TYPE_LAYER_UI, sg_box_rankDetailLayer);
    ResourceManager::sharedInstance()->bindTexture("sgfirstlayer/sgfirstlayer.plist", RES_TYPE_LAYER_UI, sg_box_rankDetailLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_box_rankDetailLayer);
  
    CCSize s=CCDirector::sharedDirector()->getWinSize();
    CCPoint center=  SGLayout::getPoint(kMiddleCenter);
    fontbg->setPreferredSize(CCSizeMake(592, 270));
    fontbg->setPosition(ccpAdd(center, ccp(0, -90)));
    
    

   
    //
    //_player=new SGGamePlayerInfo();
    _player=player;
    _player->retain();
    
    
    SGButton *ok = SGButton::create("box_btn_newbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGRankDetailLayer::boxClose),
                                    ccp(0, 0),
                                    false,
                                    true);
    
    this->addBtn(ok);
    ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -270)));////根据框的大小设置位置
    
    
    
 ///////////玩家国家头像
    SGButton*  portrait_bg = SGButton::create("player_portrait.png", CCString::createWithFormat( "countryIconbig_%d.png",_player->getCountry()>0?_player->getCountry():1)->getCString(),
                                   this, NULL,ccp(0.5, -6),false,true);
    
    //  portrait_bg->setAnchorPoint(ccp(0, 0));
    float wid = s.width - 115*4  ;
    portrait_bg->setPosition(ccp(wid/2, 245+center.y));
    this->addChild(portrait_bg);
    
    //人名
    SGCCLabelTTF* nickname =  SGCCLabelTTF::create(_player->getNickName()->getCString(),FONT_BOXINFO,FONTSIZE(16) );
    nickname->setAnchorPoint(ccp(0, 0.5));
    nickname->setPosition(ccpAdd(portrait_bg->getPosition(), ccp(portrait_bg->getContentSize().width*0.7, portrait_bg->getContentSize().height*0.25)));
    //modify by:zyc. merge into create.
    //nickname->setColor(ccRED);
    this->addChild(nickname);
 ///等级
    SGCCLabelTTF* levellabel =  SGCCLabelTTF::create(CCString::create("Lv")->getCString(),FONT_PANGWA,FONTSIZE(12) , ccc3(0xff, 0xc0, 0x00));
    //modify by:zyc. merge into create.
    //levellabel->setColor(ccc3(0xff, 0xc0, 0x00));
    levellabel->setAnchorPoint(ccp(0, 0.5));
    levellabel->setPosition(ccpAdd(nickname->getPosition(), ccp(0, -nickname->getContentSize().height*1.1)));
    this->addChild(levellabel);
 ///等级数值
    SGCCLabelTTF* levelnum =  SGCCLabelTTF::create(CCString::createWithFormat("%d", _player->getLevel())->getCString(),FONT_PANGWA,FONTSIZE(12));
    
    levelnum->setAnchorPoint(ccp(0, 0.5));
    levelnum->setPosition(ccpAdd(levellabel->getPosition(), ccp(levellabel->getContentSize().height*1.5, 0)));
    this->addChild(levelnum);
    
//军衔
    SGCCLabelTTF* junxian =  SGCCLabelTTF::create( _player->getpvpRankName()->getCString(),FONT_PANGWA,FONTSIZE(12));
    junxian->setAnchorPoint(ccp(0, 0.5));
    junxian->setPosition(ccpAdd(levellabel->getPosition(), ccp(150, -0)));
    this->addChild(junxian);
//战斗力
    SGCCLabelTTF* zhandouli =  SGCCLabelTTF::create(CCString::create(str_EmbattleLayer_str4)->getCString(),FONT_PANGWA,FONTSIZE(12) , ccc3(0xff, 0xc0, 0x00));
    zhandouli->setAnchorPoint(ccp(0, 0.5));
    zhandouli->setPosition(ccpAdd(levellabel->getPosition(), ccp(0, -levellabel->getContentSize().height*1.1)));
    //modify by:zyc. merge into create.
    //zhandouli->setColor(ccc3(0xff, 0xc0, 0x00));
    this->addChild(zhandouli);
    
  ///战斗力数值
    SGCCLabelTTF* num =  SGCCLabelTTF::create(CCString::createWithFormat("%d", _player->getCombatValue())->getCString(),FONT_PANGWA,FONTSIZE(12));
    
    num->setAnchorPoint(ccp(0, 0.5));
    num->setPosition(ccpAdd(zhandouli->getPosition(), ccp(zhandouli->getContentSize().width*1.2,0 )));
    this->addChild(num);
    
    const char aa[][100]={str_wujiang,str_soldiers,str_MainTaskLayer_str9,str_other};
    
    for (int j=0;j<4;++j)
    {
        SGButton *btn = SGButton::createFromLocal("store_exchangebtnbg.png", aa[j], this, menu_selector(SGRankDetailLayer::showinfo),CCPointZero,FONT_PANGWA,ccWHITE,25);
        btn->setPosition(ccp(0+btn->getContentSize().width*j, center.y+100));
        
        float tt=  s.width-( btn->getContentSize().width*4.8);
        
        btn->setPosition(ccp(btn->getContentSize().width*j*1.26+tt*0.5, center.y+110));
        
        btn->setAnchorPoint(ccp(0, 1));
        btn->setTag(j+100);//防止和下面tag重了
        
        addBtn(btn);
        if( type==0 )
        {
            if(j==0)
            {
                btn->setEnabled(false);
            }
            else
            {
                btn->setEnabled(false);
                btn->setImage("barrack_anniu_hui.png");
            }
            // btn->setFontColor(ccc3(70, 70, 70));
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////武将    tag为0到10系列
    
    officedarray=CCArray::create();
   
    officedarray= _player->getOfficerCardItemArray();
    officedarray->retain();
    soldierarray=CCArray::create();
    soldierarray= _player->getembattlesoldierArray();
    soldierarray->retain();
    int maxnum=-10;
    _player->getePVPOfficerArray()->count();
    
    if(type==0)
    {
        maxnum=_player->getePVPOfficerArray()->count()-1;
    }
    else
    {
        maxnum=officedarray->count()-1;
    }
    
    int lordid=_player->getLordid();
   
    
    
    SGPlayerDataModel *playerdata = SGStaticDataManager::shareStatic()->getPlayerByLevel(_player->getLevel());
    int embattlemax=playerdata->getPlayerOfficerMax();
    
    for(int i=0;i<10;++i)
    {
        SGButton*btn;
        if(i<=maxnum)
        {
            SGOfficerCard *card=NULL;
            SGOfficerDataModel* obj ;
            
            pvpInfo*info = NULL;
            if(type==0)  ///////跨服的
            {
                info = (pvpInfo*)_player->getePVPOfficerArray()->objectAtIndex(i);
                obj = SGStaticDataManager::shareStatic()->getOfficerById(info->getitmeId());
            }
            else   ///////本服的

            {
                card = (SGOfficerCard*)_player->getOfficerCardItemArray()->objectAtIndex(i);
                
                obj = SGStaticDataManager::shareStatic()->getOfficerById(card->getItemId());
            }
            
            int starlvl  =obj->getOfficerCurrStarLevel();
            if(starlvl==1||starlvl==2)
            {
                starlvl=1;
            }
            else
            {
                starlvl=starlvl-1;
            }
            /////////////////////////
        
            CCString*headicon=CCString::createWithFormat("head%d.png",obj->getIconId());
            CCString*kuang=CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);
        
            CCString*country= CCString::createWithFormat("countryName%d.png",obj->getOfficerRace());
            SGMainManager::shareMain()->addHeadIconPlistByNum(obj->getIconId(),sg_rank);
            btn=SGButton::create(headicon->getCString(),
                                          kuang->getCString(),
                                          this,
                                          menu_selector(SGRankDetailLayer::showgeneralinfo),
                                          ccp(0, 0),
                                          false,
                                          true);
            
            CCSprite*coun=CCSprite::createWithSpriteFrameName(country->getCString());
            coun->setPosition(ccp(12, btn->getContentSize().height*0.85));
            btn->addChild(coun);
            addBtn(btn);
            
            
            
            
            /////////////////武将等级
            
            
            if(type==0)  ///////跨服的
            {
                //MMDEBUG:: IMPROVE 貌似需要显示武将等级，但是card解析不出来武将，暂时用pvpinfo代替。
                SGCCLabelTTF* levellabel =  SGCCLabelTTF::create(CCString::createWithFormat("Lv%d", info->getlevel())->getCString(),FONT_PANGWA,FONTSIZE(12));
                levellabel->setPosition(ccp(btn->getContentSize().width*0.5, levellabel->getContentSize().height*0.5));
                btn->addChild(levellabel);
            }
            
            else   ///////本服的
                
            {
                SGCCLabelTTF* levellabel =  SGCCLabelTTF::create(CCString::createWithFormat("Lv%d", card->getCurrLevel())->getCString(),FONT_PANGWA,FONTSIZE(12));
                levellabel->setPosition(ccp(btn->getContentSize().width*0.5, levellabel->getContentSize().height*0.5));
                btn->addChild(levellabel);
                
                ///////////////转生次数
                if(card->getAdNum()>0)
                {
                    SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",card->getAdNum())->getCString(), FONT_PANGWA, 24);
                    advancenumber->setInsideColor(ccGREEN);
                    advancenumber->setAnchorPoint(ccp(1, 1));
                    advancenumber->setPosition(ccp(btn->getContentSize().width, btn->getContentSize().height));
                    btn->addChild(advancenumber);
                }
                
                 ////////////////////////////////////////////
                if(lordid==card->getSsid())/////主将图片组合体
                {
                    CCSprite *lord = CCSprite::createWithSpriteFrameName("font_zj_bg.png");
                    lord->setPosition(ccp(btn->getContentSize().width*0.5, lord->getContentSize().height*0.5));
                    btn->addChild(lord);
                    
                    CCSprite *font = CCSprite::createWithSpriteFrameName("font_zj.png");
                    font->setPosition(ccp(btn->getContentSize().width*0.5, font->getContentSize().height*0.5));
                    btn->addChild(font);
                    
                }
            }
        
        }
        
        else if(i<embattlemax)
        {
        
            btn=SGButton::create("embattle_huilan.png", "embattle_font_kong.png", this,
                                 NULL,
                                 ccp(0, 0),
                                 false,
                                 true);
            btn->setEnabled(false);
            addBtn(btn);
        
        }
 
    
    
        else
        {
            CCString *num = CCString::createWithFormat("embattle_num%d.png",i+1);
            btn=SGButton::create("embattle_lan.png", num->getCString(), this,
                                 NULL,
                                 ccp(0, 0),
                                 false,
                                 true);
            
            
            CCSprite *suo = CCSprite::createWithSpriteFrameName(CCString::create("embattle_suo.png")->getCString());
            suo->setAnchorPoint(ccp(1, 1));
            suo->setPosition(ccp(btn->getContentSize().width, btn->getContentSize().height));
            btn->addChild(suo);
            addBtn(btn);
        }
    
    
    
        btn->setPosition(ccpAdd(ccp(0, SGLayout::getPoint(kMiddleCenter).y-215),(ccp(wid/2 + (i%5) *115,-(i/5)*115 + 187))));
        btn->setTag(i);
        
    }
    
     /////////////////////////////////////////////////////////////////////////////////////////////////////士兵   tag为10系列

    int redId=_player->getRedSoldierId();
    int greenId=_player->getGreenSoldierId();
    int blueId=_player->getBlueSoldierId();
    int soldier[]={redId,greenId,blueId};
    int color[]={3,1,2};
    
    if(type==0)
    {
    
    }
    //else if (0)//此处只能看武将，不允许看小兵。
    else
    {
        for (int i = 0; i< 3; i++)
       {
           CCSprite*jingkuang=CCSprite::createWithSpriteFrameName("equipPinzhi_1_0.png");

           CCString*icon=CCString::createWithFormat("%d-%d.png",soldier[i],color[i]);
           CCString*kuang=CCString::create("equipPinzhi_1_0.png");
           SGButton*soldierbtn=SGButton::create(icon->getCString(), kuang->getCString(), this, menu_selector(SGRankDetailLayer::showSoldierinfo),CCPointZero,false,true);
           
           soldierbtn->setPosition(ccp(s.width/2 +15 +(i-1)*(jingkuang->getContentSize().width + 60) ,center.y-70));
           soldierbtn->setTag(10+i);
          // soldierbtn->setZOrder(soldier[i]);
           addBtn(soldierbtn);

           
           ////////////士兵等级
           SGSoldierCard*card=(SGSoldierCard*)soldierarray->objectAtIndex(i);
           SGCCLabelTTF* levellabel =  SGCCLabelTTF::create(CCString::createWithFormat("Lv%d", card->getCurrLevel())->getCString(),FONT_PANGWA,FONTSIZE(12));
           levellabel->setAnchorPoint(ccp(0.5, 0.5));
           levellabel->setPosition(ccp(soldierbtn->getContentSize().width*0.5, levellabel->getContentSize().height*0.35));
           soldierbtn->addChild(levellabel);
           
           SGSoldierDataModel*  soldierData = SGStaticDataManager::shareStatic()->getSoliderById(soldier[i]);
           if (soldierData)
           {
               SGCCLabelTTF * soldierName = SGCCLabelTTF::create(soldierData->getSoldierName()->getCString(), FONT_PANGWA,FONTSIZE(12));
               soldierName->setPosition(ccp(soldierbtn->getContentSize().width/2, -soldierbtn->getContentSize().height*0.3));
               soldierbtn->addChild(soldierName);
           }
       }
    }
   

    ///////////////////////////////////////////////////////////////////////////////////////////////////属性    tag为20系列
    
    ////统御力
   if(type==0)
   {
   }
    
   else
   {
       SGCCLabelTTF* tongyuli =  SGCCLabelTTF::create(CCString::createWithFormat(str_MainTaskLayer_str10, _player->getGoverning(),_player->getGoverningLimit() )->getCString(),FONT_BOXINFO,FONTSIZE(14));
       
       tongyuli->setAnchorPoint(ccp(0, 0.5));
       tongyuli->setPosition(ccpAdd(portrait_bg->getPosition(), ccp(-portrait_bg->getContentSize().width*0.0,-portrait_bg->getContentSize().height*1.8)));
       //  shengwang->setColor(ccBLACK);
       this->addChild(tongyuli);
       tongyuli->setTag(21);
       ///总血量
       
       SGCCLabelTTF* blood =  SGCCLabelTTF::create(CCString::createWithFormat(str_MainTaskLayer_str11,  _player->getHp())->getCString(),FONT_BOXINFO,FONTSIZE(14));
       
       blood->setAnchorPoint(ccp(0, 0.5));
       blood->setPosition(ccpAdd(tongyuli->getPosition(), ccp(0,-tongyuli->getContentSize().height*1.5 )));
       // zhanji->setColor(ccBLACK);
       this->addChild(blood);
       blood->setTag(22);
       
       
       ///总速度
       SGCCLabelTTF* speed =  SGCCLabelTTF::create(CCString::createWithFormat(str_MainTaskLayer_str12,  _player->getSpeed())->getCString(),FONT_BOXINFO,FONTSIZE(14));
       
       speed->setAnchorPoint(ccp(0, 0.5));
       speed->setPosition(ccpAdd(blood->getPosition(), ccp(0,-blood->getContentSize().height*1.5)));
       //  sheng->setColor(ccBLACK);
       this->addChild(speed);
       speed->setTag(23);
       
       ///国家
       countrymm=_player->getCountry();
       
       
       
       
       CCString*string;
       if (_player->getCountry() == 1) {
           
           string=CCString::create(str_MainTaskLayer_str13);
           
       }
       if (_player->getCountry() == 2) {
           string=CCString::create(str_MainTaskLayer_str14);
           
       }
       if (_player->getCountry() == 3) {
           string=CCString::create(str_MainTaskLayer_str15);
       }
       
       
       
       
       SGCCLabelTTF* country =  SGCCLabelTTF::create(string->getCString(),FONT_BOXINFO,FONTSIZE(14));
       
       country->setAnchorPoint(ccp(0, 0.5));
       country->setPosition(ccpAdd(speed->getPosition(), ccp(0,-speed->getContentSize().height*1.5 )));
       //  rank->setColor(ccBLACK);
       this->addChild(country);
       country->setTag(24);
       
       //2014-10-29 zyc modify. 禁用该功能。
//       SGButton *counbtn = SGButton::createFromLocal("ofc_moreinfo.png", NULL, this, menu_selector(SGRankDetailLayer::showcountryinfo),CCPointZero,FONT_PANGWA,ccWHITE,25);
//       counbtn->setVisible(false);
//       counbtn->setAnchorPoint(ccp(0, 0.5));
//       counbtn->setPosition(ccpAdd(country->getPosition(), ccp(country->getContentSize().width+20, 0)));
//       addBtn(counbtn);
//       counbtn->setTag(25);
       
   }
    
    
  ////////////////////////////////////////////////////////////////////////////////////////////其他   tag为30系列
    
   ////天梯声望
    if(type==0)
    {
    
    
    }
    
    
    else
    {
        SGCCLabelTTF* shengwang =  SGCCLabelTTF::create(CCString::createWithFormat(str_MainTaskLayer_str16,_player->getpresTige())->getCString(),FONT_BOXINFO,FONTSIZE(14));
        
        shengwang->setAnchorPoint(ccp(0, 0.5));
        shengwang->setPosition(ccpAdd(portrait_bg->getPosition(), ccp(-portrait_bg->getContentSize().width*0.0,-portrait_bg->getContentSize().height*1.8)));
        //  shengwang->setColor(ccBLACK);
        this->addChild(shengwang);
        shengwang->setTag(31);
        ///天梯战绩

        SGCCLabelTTF* zhanji =  SGCCLabelTTF::create(CCString::createWithFormat(str_MainTaskLayer_str17, _player->getpvpWinCount(),_player->getenterPVPCount()-_player->getpvpWinCount())->getCString(),FONT_BOXINFO,FONTSIZE(14));
        
        zhanji->setAnchorPoint(ccp(0, 0.5));
        zhanji->setPosition(ccpAdd(shengwang->getPosition(), ccp(0,-shengwang->getContentSize().height*1.5 )));
       // zhanji->setColor(ccBLACK);
        this->addChild(zhanji);
        zhanji->setTag(32);
        
        
       ///天梯胜率
        
        CCString*str;
       if(_player->getpvpWinCount()==0)
            str= CCString::create(str_MainTaskLayer_str20);
        else
            str= CCString::createWithFormat(str_MainTaskLayer_str21,100*_player->getpvpWinCount()/(float)_player->getenterPVPCount());
        
        
            
        SGCCLabelTTF* sheng =  SGCCLabelTTF::create(str->getCString(),FONT_BOXINFO,FONTSIZE(14));
        
        sheng->setAnchorPoint(ccp(0, 0.5));
        sheng->setPosition(ccpAdd(zhanji->getPosition(), ccp(0,-zhanji->getContentSize().height*1.5)));
        //  sheng->setColor(ccBLACK);
        this->addChild(sheng);
        sheng->setTag(33);
        
        ///天梯排名
        SGPlayerInfo* playInfo = SGPlayerInfo::sharePlayerInfo();
        
        
        
        CCString*fuck;
     //   CCLOG("%d",_player->getpvpTop());
        if(_player->getpvpTop()<=0)
        {
            
            fuck=CCString::create(str_MainTaskLayer_str18);
        }
        else
        {
        
            fuck=CCString::createWithFormat(str_MainTaskLayer_str19,  _player->getpvpTop());
        }
        
       SGCCLabelTTF* rank =  SGCCLabelTTF::create(fuck->getCString(),FONT_BOXINFO,FONTSIZE(14));
        
        rank->setAnchorPoint(ccp(0, 0.5));
        rank->setPosition(ccpAdd(sheng->getPosition(), ccp(0,-sheng->getContentSize().height*1.5 )));
      //  rank->setColor(ccBLACK);
        this->addChild(rank);
        rank->setTag(34);
    
    }

    
    
    if(type==0)
    {
    }
    else
    {
        setCanSee(0);
        setCanNotSee(1);
        setCanNotSee(2);
        setCanNotSee(3);
    }


    
}



void SGRankDetailLayer::boxClose(CCObject*obj)
{
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer* mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(false);
    SGBaseLayer *layer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (layer != NULL)
    {
        layer->setIsCanTouch(true);
    }
    //SGBaseBox::boxClose();
    this->setVisible(false);
}


void SGRankDetailLayer::showinfo(CCNode*node)
{
    int tag=node->getTag()-100;
    
    
    for(int i=0;i<4;i++)
    {
        
        
        SGButton*btn= (SGButton*) getBtnByTag(i+100);
        if(tag==i)
        {
            btn->setFontColor(ccYELLOW);
        }
        else
        {
        btn->setFontColor(ccWHITE);
        }
    
    }
    
    switch (tag)
    {
        case 0:
        {
            setCanSee(0);
            setCanNotSee(1);
            setCanNotSee(2);
            setCanNotSee(3);
            
        }
            
            break;
        case 1:
        {
            
            setCanNotSee(0);
            setCanSee(1);
            setCanNotSee(2);
            setCanNotSee(3);
            
  
        }
            
            break;
        case 2:
        {
            setCanNotSee(0);
            setCanNotSee(1);
            setCanSee(2);
            setCanNotSee(3);
          
        }
            break;
        case 3:
            
        {
            setCanNotSee(0);
            setCanNotSee(1);
            setCanNotSee(2);
            setCanSee(3);
     
        }
            break;
            
        default:
            break;
    }
    
    

}






void SGRankDetailLayer::showcountryinfo()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showCountriesLayer(2,false,countrymm);
        //SGBaseBox::boxClose();
    this->setVisible(false);
}




void SGRankDetailLayer::showgeneralinfo(CCNode*node)
{
    if(entertype==0)
    {
        return;
    }
    if(entertype == 3)//竞技场
    {
        int tag=node->getTag();
        SGOfficerCard *card = (SGOfficerCard*)officedarray->objectAtIndex(tag);
        SGMainManager::shareMain()->removeGeneralInfoLayerForNew();
        SGMainManager::shareMain()->showGeneralInfoLayer(card, 28,NULL,false);
        this->setVisible(false);

    }
    else if(entertype==2)//天梯排名
    {
        int tag=node->getTag();
        SGOfficerCard *card = (SGOfficerCard*)officedarray->objectAtIndex(tag);
        SGMainManager::shareMain()->removeGeneralInfoLayerForNew();
        SGMainManager::shareMain()->showGeneralInfoLayer(card, 27,NULL,false);
        this->setVisible(false);
    }
    else//聊天进来
    {
        int tag=node->getTag();
        SGOfficerCard *card = (SGOfficerCard*)officedarray->objectAtIndex(tag);
        SGMainManager::shareMain()->removeGeneralInfoLayerForNew();
        SGMainManager::shareMain()->showGeneralInfoLayer(card, 24,NULL,false);
        this->setVisible(false);
        
    }
    //SGBaseBox::boxClose();
    
}



void SGRankDetailLayer::showSoldierinfo(CCNode*node)
{
    if(entertype == 3)//竞技场进来
    {
        int id=node->getTag()-10;
        SGSoldierCard*card=(SGSoldierCard*) soldierarray->objectAtIndex(id);
        SGMainManager::shareMain()->showSoldierInfoLayer(card,25,false);
        //SGBaseBox::boxClose();
        this->setVisible(false);
    }
    else
    {
        int id=node->getTag()-10;
        SGSoldierCard*card=(SGSoldierCard*) soldierarray->objectAtIndex(id);
        SGMainManager::shareMain()->showSoldierInfoLayer(card,24,false);
        //SGBaseBox::boxClose();
        this->setVisible(false);
    }

}


void SGRankDetailLayer::setCanSee(int type)
{
    switch (type) {
        case 0:
        {
            for(int i=0;i<10;i++)
            {
                SGButton*btn= (SGButton*)  getBtnByTag(i);
                btn->setVisible(true);
                btn->setEnabled(true);
                
            }
            
        }
            
            break;
        case 1:
        {
            
            for(int i=0;i<3;i++)
            {
                SGButton*btn= (SGButton*)  getBtnByTag(i+10);
                btn->setVisible(true);
                btn->setEnabled(true);
            }
            
            
        }
            
            break;
        case 2:
        {
            getChildByTag(21)->setVisible(true);
            getChildByTag(22)->setVisible(true);
            getChildByTag(23)->setVisible(true);
            getChildByTag(24)->setVisible(true);
//            SGButton*btn= (SGButton*)  getBtnByTag(25);
//            btn->setVisible(true);
//            btn->setEnabled(true);
            
        }
            break;
        case 3:
            
        {
            getChildByTag(31)->setVisible(true);
            getChildByTag(32)->setVisible(true);
            getChildByTag(33)->setVisible(true);
            getChildByTag(34)->setVisible(true);
            
            
        }
            break;
            
        default:
            break;
    }

}

void SGRankDetailLayer::setCanNotSee(int type)
{
    
    
    switch (type) {
        case 0:
        {
            for(int i=0;i<10;i++)
            {
                SGButton*btn= (SGButton*)  getBtnByTag(i);
                btn->setVisible(false);
                btn->setEnabled(false);
                
            }
            
        }
            
            break;
        case 1:
        {
            for(int i=0;i<3;i++)
            {
            SGButton*btn= (SGButton*)  getBtnByTag(i+10);
            btn->setVisible(false);
            btn->setEnabled(false);
            }
            
        }
            
            break;
        case 2:
        {
            getChildByTag(21)->setVisible(false);
            getChildByTag(22)->setVisible(false);
            getChildByTag(23)->setVisible(false);
            getChildByTag(24)->setVisible(false);
//            SGButton*btn= (SGButton*)  getBtnByTag(25);
//            btn->setVisible(false);
//            btn->setEnabled(false);
            
        }
            break;
        case 3:
            
        {
            
            getChildByTag(31)->setVisible(false);
            getChildByTag(32)->setVisible(false);
            getChildByTag(33)->setVisible(false);
            getChildByTag(34)->setVisible(false);
            
            
        }
            break;
            
        default:
            break;
    }
    

    
}
