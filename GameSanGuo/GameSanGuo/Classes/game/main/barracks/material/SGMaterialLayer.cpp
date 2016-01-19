//
//  SGMaterialLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-10-22.
//
//

#include "SGMaterialLayer.h"
#include "SGMainManager.h"
#include "SGShowString.h"
#include "SGMaterialCell.h"


//每页显示的item的数量
#define EACH_PAGE_NUM 10


SGMaterialLayer::SGMaterialLayer():
pageLabel(NULL)
{
    
}

SGMaterialLayer::~SGMaterialLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_materialLayer);
    CC_SAFE_RELEASE(datas);
    CC_SAFE_RELEASE(_allData);
}


SGMaterialLayer *SGMaterialLayer::create()
{
    SGMaterialLayer *material = new SGMaterialLayer();
    if (material && material->init(NULL, sg_materialLayer))
    {
        material->initView();
        material->autorelease();
        return material;
    }
    CC_SAFE_RELEASE(material);
    return NULL;
}

//排序回调
static int SGSortAccorStarLvl(CCObject *p1, CCObject *p2)
{
    SGMaterialCard *card1 = static_cast<SGMaterialCard *>(p1);
    SGMaterialCard *card2 = static_cast<SGMaterialCard *>(p2);
    if (card1 == NULL || card2 == NULL) {
        return false;
    }
    return card1->getMaterialBorderColor() > card2->getMaterialBorderColor();
}


void SGMaterialLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_materialLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_materialLayer);
	ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_materialLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgbarrackslayer/sgbarrackslayer.plist", RES_TYPE_LAYER_UI, sg_materialLayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_materialLayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_materialLayer);
    
	
    float h = SGMainManager::shareMain()->getHeadHeight();
    float b = SGMainManager::shareMain()->getBottomHeight();
    
    CCSize s= CCDirector::sharedDirector()->getWinSize();
    
    
    datas = CCArray::create();
    datas->retain();
    
    _allData = CCArray::create();
    _allData->retain();
    
    //排序卡牌
    CCArray *tempMaterial = SGPlayerInfo::sharePlayerInfo()->getMaterialCards();
    std::sort(tempMaterial->data->arr, tempMaterial->data->arr + tempMaterial->data->num, SGSortAccorStarLvl);
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    
    //逐个放入数组中
    for (int i = 0; i < playerInfo->getMaterialCards()->count(); i++)
    {
        SGMaterialCard *card = (SGMaterialCard *)playerInfo->getMaterialCards()->objectAtIndex(i);
        
        if (card)
        {
            _allData->addObject(card);
            
            if(i<EACH_PAGE_NUM)
                datas->addObject(card);
        }
    }
    _totalPage = ceilf(1.0*_allData->count() / EACH_PAGE_NUM);
    _nowPage = 1;
    
    CCSprite *titleBg = CCSprite::createWithSpriteFrameName("titleredbg.png");
    this->addChild(titleBg);
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - h)));
	
	CCLabelTTF *title = CCLabelTTF::create(str_Material_str1, FONT_BOXINFO, 32);
	title->setColor(ccc3(0xe9, 0xd3, 0x7a));
	this->addChild(title, 1);
	title->setPosition(ccpAdd(titleBg->getPosition(), ccp(0, -titleBg->getContentSize().height / 2)));
	
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this,
                                                  menu_selector(SGMaterialLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    backBtn->setScale(1.01);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2));
    
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_materialLayer);
    
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, 30,bg->getContentSize().width,s.height - h - b - titleBg->getContentSize().height + 25);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
	
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(0, b));
    
	
    tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - h  - titleBg->getContentSize().height));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b)));
    tableView->setDown(-25);
    //设置左右箭头
    float posY = tableView->getContentSize().height*.5f + tableView->getPositionY();
    CCSprite *spriteL = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    float widLR = spriteL->getContentSize().width*.5f;
    float hitLR = spriteL->getContentSize().height*.5f;
    spriteL->setPosition(ccp(widLR, posY-hitLR));
    spriteL->setVisible(false);
    spriteL->setTag(111);
    this->addChild(spriteL);
    CCSprite *spriteR = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    spriteR->setFlipX(true);
    spriteR->setTag(112);
    spriteR->setVisible(false);
    spriteR->setPosition(ccp(s.width-widLR, posY-hitLR));
    this->addChild(spriteR);
    
    if (_totalPage > 1)
    {
        spriteL->setVisible(true);
        spriteR->setVisible(true);
        tableView->setPagegate(this);
        tableView->setPageNEnable(true);
        tableView->setShowSlider(false);
    }
	
    
    CCSprite *pagecountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
    pagecountbg->setAnchorPoint(ccp(0, 0));
    this->addChild(pagecountbg,40,110);
    pagecountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
	
    CCString *strpg = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    pageLabel = SGCCLabelTTF::create(strpg->getCString(), FONT_BOXINFO, 28 , COLOR_YELLOW);
    this->addChild(pageLabel,42);
    pageLabel->setPosition(ccpAdd(pagecountbg->getPosition(), ccp(pagecountbg->getContentSize().width*.5f,
                                                                  pagecountbg->getContentSize().height*.5f)));
	
}


SNSTableViewCellItem *SGMaterialLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int index = tableView->getItemPointerWithIndexPath(indexPath);
    //内部的材料卡牌
	SGMaterialCard *temp = static_cast<SGMaterialCard *>(datas->objectAtIndex(index));
	SGMaterialCell *materialCell = (SGMaterialCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);

	if (!materialCell)
	{
		materialCell = SGMaterialCell::create(temp);
	}
	else if (index < datas->count())// 更新各种显示
	{
        materialCell->updateCell(temp);
	}
	
	return materialCell;
}

unsigned int SGMaterialLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    if (datas)
    {
        return datas->count();
    }
    return 0;
}
void SGMaterialLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	EFFECT_PLAY(MUSIC_BTN);
    selectIndex = tableView->getItemPointerWithIndexPath(indexPath);//选择哪一item
    if (selectIndex >= 0 && selectIndex < datas->count())
    {
        SGMaterialCard* pc = dynamic_cast<SGMaterialCard*>(datas->objectAtIndex(selectIndex));
        if (pc)
        {
            SGMainManager::shareMain()->showCommonInfoBoxInProto(pc, this);
        }
    }

}


void SGMaterialLayer::refreshPage(int type)
{
    datas->removeAllObjects();
    int pageCount =  _allData->count() > EACH_PAGE_NUM ? EACH_PAGE_NUM : _allData->count(); // 每页显示的个数
    _totalPage =ceilf( 1.0*_allData->count()/pageCount);
    if (type == 1) { // to next page
        if (_nowPage < _totalPage) {
            _nowPage++;
        }else { // _nowPage >= _totalPage
            _nowPage = 1;
        }
    }else if (type == 2) { // to pre page
        if (_nowPage > 1) {
            _nowPage--;
        }else {
            _nowPage = _totalPage;
        }
    }
    
    int last = pageCount;
    if (_nowPage == _totalPage) {
        last = _allData->count()-(_nowPage-1)*pageCount;
    }
    for (int ii = 0; ii < last; ii++) {
        CCObject *obj = _allData->objectAtIndex((_nowPage-1)*pageCount+ii);
        datas->addObject(obj);
    }
    
    CCString *str = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    if(pageLabel) pageLabel->setString(str->getCString());
    tableView->reloadData();
}


void SGMaterialLayer::backHandler(CCObject *obj)
{
    SGMainManager::shareMain()->showBarrackslayer();
}




