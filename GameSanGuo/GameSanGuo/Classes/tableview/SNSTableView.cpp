//
//  SNSTableView.cpp
//  tableView
//	version 2.0
//  Created by yang jie on 28/11/2011.
//  Copyright 2011 ourpalm.com. All rights reserved.
//

#include "SNSTableView.h"
#include "GameMusicmanage.h"


SNSTableView::SNSTableView()
:m_datasource(NULL),
m_tableType(TableViewTypeINVALID),
m_lazyLoad(false), m_rowCount(0),
m_itemCount(0), m_columnCount(0),
m_rowMaxInstance(0),
m_topPointer(0),
m_bottomPointer(0),
m_minRowWidthOrHeight(0),
m_padding(0),
m_margin(0),
m_lastDragIndex(NULL),
m_orientation(ReuseCellOrientationINVALID),
m_selectType(TableViewSelectTypeINVALID),
m_selectedItem(NULL),
m_itemSize(CCSizeZero),
m_down(0),
m_isMoveEnd(true),
iscreateall(0)

{
}

SNSTableView::~SNSTableView()
{
	CC_SAFE_DELETE_ARRAY(m_selectedItem);
}

bool SNSTableView::initTableViewFrame(CCRect frame, TableViewType types)
{
    if ( !SNSScrollView::initScrollViewFrame(frame) ) {
        return false;
    }
    
    m_padding = 1;
    m_margin = 1; //wgx change 1 to 0.
    
    setTableType(types);
    if (!m_tableType) {
        setTableType(TableViewTypeVertical);
    }
    
    setIsTouchEnabled(true);
    setLazyLoad(false);
    
    return true;
}

SNSTableView* SNSTableView::initWithFrame(CCRect frame, TableViewType types)
{
    return SNSTableView::create(frame, types);
}

SNSTableView* SNSTableView::create(CCRect frame, TableViewType types)
{
	SNSTableView* instance = new SNSTableView();
    
    if (instance && instance->initTableViewFrame(frame, types) )
    {
        instance->autorelease();
        return instance;
    }
    CC_SAFE_DELETE(instance);
    return instance;
}

TableViewType SNSTableView::getTableType()
{
    return m_tableType;
}

void SNSTableView::setTableType(TableViewType var)
{
    m_tableType = var;
    switch ( m_tableType )
    {
        case TableViewTypeHorizontal:
            setHorizontal(true);
            setVertical(false);
            break;
        case TableViewTypeVertical:
            setHorizontal(false);
            setVertical(true);
            break;
        default:
            break;
    }
}

TableViewSelectType SNSTableView::getSelectType()
{
	return m_selectType;
}

void SNSTableView::setSelectType(TableViewSelectType var)
{
	m_selectType = var;
	// 根据cell的数量初始化选中数组
	CC_SAFE_DELETE_ARRAY(m_selectedItem);
	m_selectedItem = new bool[m_itemCount];
	// 默认全部置false
	for (int i = 0; i < m_itemCount; i++) {
		m_selectedItem[i] = false;
	}
}

void SNSTableView::onEnterTransitionDidFinish()
{
    SNSScrollView::onEnterTransitionDidFinish();
    
    if ( !m_body->getChildren() || m_body->getChildren()->count() <= 0 )
    {
        loadData();
    }
}

void SNSTableView::reloadData(bool fullReload)
{
	// 停止滚动，防止因为修改指针而崩溃
	stopScroll();
	if (fullReload)
    {
		// 清理body上的所有child
		m_body->removeAllChildrenWithCleanup(true);
		m_itemCount = 0;
		m_rowCount = 0;
		m_columnCount = 0;
		
		//清空移动指针
		m_topPointer = 0;
		m_bottomPointer = 0;
		
		//重设body的position
		setBodySize(CCSizeMake(0, 0));
		loadData();
		
	}
    else
    {

        int lastRowCount = m_rowCount;
        int lastpy = m_body->getPosition().y;
        int lasth = m_bodySize.height;
        //CCLOG("y=%d,h=%d",lastpy,lasth);
		// 重算item数
		if ( m_datasource )
        {
			m_itemCount = m_datasource->tableViewAllItemsNumber(this, 1);
		} else
        {
			m_itemCount = 0;
		}
		// 根据cell的数量初始化选中数组
		CC_SAFE_DELETE_ARRAY(m_selectedItem);
		m_selectedItem = new bool[m_itemCount];
		// 默认全部置false
		for (int i = 0; i < m_itemCount; i++)
        {
			m_selectedItem[i] = false;
		}
		
		//获取当前tableView的列数
		if ( m_datasource )
        {
			m_columnCount = m_datasource->tableViewColumnsOfItemsNumber(this, 1);
		}
        else
        {
			m_columnCount = 1;
		}
		
		//然后计算当前的行数
		m_rowCount = (int)((float)m_itemCount / (float)m_columnCount + 0.9999f);
		     
		// 如果两次的行数不一样，证明数据量发生了变化，那么重新刷新数据
		if (lastRowCount != m_rowCount)
        {
//			return reloadData();
            //计算总高度
            setScrollWidthOrHeight();
            int y = -m_bodySize.height+lasth+lastpy;
            m_body->setPositionY(y);

        }
                //遍历指针之间的区域，刷新item
        for (int i = m_topPointer; i < m_bottomPointer; i++)
        {
            SNSTableViewCell *cell = (SNSTableViewCell *)m_body->getChildByTag(i);
            if (cell)
            {
                // 如果cell存在的话reset cell的item数据
                moveCellToIndexPath(cell, cell->getIndexPath());
            }
        }
            //最后停止自己未结束的滑动
        stopScroll();
        fixPosition();
	
	}
}

void SNSTableView::loadData()
{
    
//    EFFECT_PLAY(MUSIC_ITEM);
    float nowLength = 0;
    //获取当前所有tableView的item数量
	if ( m_datasource )
    {
        m_itemCount = m_datasource->tableViewAllItemsNumber(this, 1);
	}
    else
    {
		m_itemCount = 0;
	}
	
	// 根据cell的数量初始化选中数组
	CC_SAFE_DELETE_ARRAY(m_selectedItem);
	m_selectedItem = new bool[m_itemCount];
	// 默认全部置false
	for (int i = 0; i < m_itemCount; i++)
    {
		m_selectedItem[i] = false;
	}
    
	//获取当前tableView的列数
	if ( m_datasource )
    {
        m_columnCount = m_datasource->tableViewColumnsOfItemsNumber(this, 1);
	}
    else
    {
		m_columnCount = 1;
	}
    
	//然后计算当前的行数
	m_rowCount = (int)((float)m_itemCount / (float)m_columnCount + 0.9999f);
    
    //计算总高度
	setScrollWidthOrHeight();
    //设置底部指针位置
	if (m_rowCount == m_rowMaxInstance + 1 || m_rowCount <= m_rowMaxInstance) m_bottomPointer = m_rowCount;
    //设置顶部指针
	for (int i = 0; i < m_rowCount; i++)
    {
		if (i == 0)
        {
			m_topPointer = 0;
            if (iscreateall)
            {
                m_bottomPointer = m_rowCount-1;
            }
		}
        SNSIndexPath* indexPath = SNSIndexPath::create(i, 1);
		SNSTableViewCell *cell = getAndReuseCellWithIndexPath(indexPath);
        m_body->addChild(cell, -i, i); // 设置zOrder以做到上压下
        moveCellToIndexPath(cell, indexPath);
		//如果当前宽高超过屏幕高度的话,设置底部指针(这里后边-1是为了防止对不上导致bottomPointer对不上)
		if (m_bottomPointer == 0)
        {
			if (m_tableType == TableViewTypeVertical)
            {
                
                if (!iscreateall)
                {
                    if (nowLength >= m_frame.size.height + getRowWidthOrHeight(indexPath))
                    {
                        m_bottomPointer = i;
                        break;
                    }
                    
					
				}
			}
            else if (m_tableType == TableViewTypeHorizontal)
            {

				if (nowLength >= m_frame.size.width + getRowWidthOrHeight(indexPath))
                {
					m_bottomPointer = i;
					break;
				}
			}
			nowLength += getRowWidthOrHeight(indexPath);
		}
	}
    
	//最后停止自己未结束的滑动
	stopScroll();
	fixPosition();
    
    // 显示初始动作
//    CCSize size = CCDirector::sharedDirector()->getWinSize();
    if (m_pageEnable==true)
    {
        
        cellmove();
    }
}

void SNSTableView::endMove()
{
   this-> m_isMoveEnd = true;
}


//根据当前行数算出body应该多宽或多高
void SNSTableView::setScrollWidthOrHeight()
{
	//归零最小格子高或宽
	m_minRowWidthOrHeight = 0;
	float length = 0;
	float widthOrHeight = 0;
    //获得最小格子的高度或宽度
	for (int i = 0; i < m_rowCount; i++)
    {
        
        SNSIndexPath* indexPath = SNSIndexPath::create(i, 1);
        widthOrHeight = getRowWidthOrHeight(indexPath);
		length += widthOrHeight;
        
		//初始化最短格子宽度
		if (i == 0)
        {
			m_minRowWidthOrHeight = widthOrHeight;
		}
		//纪录最短格子的宽或高
		if (widthOrHeight < m_minRowWidthOrHeight)
        {
			m_minRowWidthOrHeight = widthOrHeight;
		}
        
	}
    
	if (m_tableType == TableViewTypeVertical)
    {
		
		//重新设置body的高度，并且加上margin的高度
		setBodySize(CCSizeMake(m_bodySize.width, -m_down + length + ((m_rowCount + m_padding) * m_margin)));
		//计算应该最多实例化多少个格子对象
        m_rowMaxInstance = (int)(m_frame.size.height / m_minRowWidthOrHeight + 0.9999f);
        
	}
    else if (m_tableType == TableViewTypeHorizontal)
    {
        
		//重新设置body的宽度，并且加上margin的宽度
        setBodySize(CCSizeMake(length + ((m_rowCount + m_padding) * m_margin), m_bodySize.height));
		//计算应该最多实例化多少个格子对象
		m_rowMaxInstance = (int)(m_frame.size.width / m_minRowWidthOrHeight + 0.9999f);
        
	}
}

SNSTableViewCell* SNSTableView::getAndReuseCellWithIndexPath(SNSIndexPath* indexPath)
{
	SNSTableViewCell *cell = NULL;
	if (NULL != m_datasource)
    {
		cell = m_datasource->tableViewCellForRow(this, indexPath);
	}
	if (NULL == cell)
    {
		cell = dequeueReusableCellWithIdentifier("SNSTableViewCell");
		if (NULL == cell)
        {
			cell = SNSTableViewCell::create("SNSTableViewCell");
		}
	}
	//CCLOG("cell:%s", cell->description());
	return cell;
}

// 重新设置选中数组指定某个item应该选中(主要用于当tableView里的元素需要根据某些条件设置为true的时候)
void SNSTableView::setSelectedArrayByIndex(int num, bool isSelected)
{
	m_selectedItem[num] = isSelected;
}

SNSTableViewCell* SNSTableView::getCellByIndexPath(SNSIndexPath* indexPath)
{
	// 独立给其他继承类用的getCellByIndexPath
	int row = indexPath->getRow();
	SNSTableViewCell *cell = dynamic_cast<SNSTableViewCell*>(m_body->getChildByTag(row));
	return cell;
}

SNSTableViewCellItem* SNSTableView::getItemByIndexPath(SNSIndexPath *indexPath)
{
	int row = indexPath->getRow();
	int column = indexPath->getColumn();
	SNSTableViewCellItem *item = NULL;
	SNSTableViewCell *cell = (SNSTableViewCell *)m_body->getChildByTag(row);
	if (cell)
    {
		item = (SNSTableViewCellItem *)cell->getChildByTag(column);
		if (item)
        {
			return item;
		}
	}
	return NULL;
}

bool SNSTableView::indexPathInArea(SNSIndexPath *indexPath)
{
	int row = indexPath->getRow();
	if (row >= m_topPointer && row <= m_bottomPointer)
    {
		return true;
	}
	return false;
}

bool SNSTableView::selectItemByIndexPath(SNSIndexPath *indexPath)
{
	if (indexPathInArea(indexPath))
    {
		SNSTableViewCellItem *item = getItemByIndexPath(indexPath);
		if (item)
        {
			switch (m_selectType)
            {
				case TableViewSelectTypeSingle:
				case TableViewSelectTypeSingleOpposite:
				{
					bool wasSel = (bool)item->getSelected();
//					CCLOG("Was selected:%d", wasSel);
					// 默认取消选中body中的所有子item
					SNSTableViewCellItem *tempItem = NULL;
                    
                    for (int ii = 0; ii < m_rowCount; ii++)
                    {
                        SNSTableViewCell *cell = (SNSTableViewCell *)m_body->getChildByTag(ii);
  						if (cell) {
							for (int j = 0; j < m_columnCount; j++)
                            {
								tempItem = (SNSTableViewCellItem*)cell->getChildByTag(j);
								if (tempItem)
                                {
									tempItem->setSelected(false);
								}
							}
						}
                    }
					// 修改选中状态布尔数组也为false
					for (int i = 0; i < m_itemCount; i++)
                    {
						m_selectedItem[i] = false;
					}
					if (m_selectType == TableViewSelectTypeSingleOpposite) {
						//可反选的话根据之前状态反选
						item->setSelected(!wasSel);
					} else {
						//不可反选的话直接选中
						item->setSelected(true);
					}
				}
					break;
				case TableViewSelectTypeMulti:
				{
					// 多选的时候才可以取消
					item->setSelected(!item->getSelected());
				}
					break;
				default:
					break;
			}
			m_selectedItem[getItemPointerWithIndexPath(indexPath)] = item->getSelected();
			return true;
		}
	}
	return false;
}

//复用cell
SNSTableViewCell* SNSTableView::dequeueReusableCellWithIdentifier(const char* cellIdentifier)
{
	SNSTableViewCell *cell = NULL;
	SNSIndexPath *indexPath = NULL;
	switch (m_orientation) {
		case ReuseCellOrientationBefore:
		{
			// 纵向向上，横向向左复用(最右移最左)
			if (m_bottomPointer < m_rowCount + 1 && m_bottomPointer > 0) {
                
                if (m_topPointer > 0) {
                    indexPath = SNSIndexPath::create(m_topPointer - 1, 1);
                    cell = (SNSTableViewCell *)m_body->getChildByTag(m_bottomPointer - 1);
                    if (m_topPointer + 1 < m_rowCount) {
                        SNSTableViewCellItem *item = NULL;
                        SNSIndexPath *newIndexPath = NULL;
                        for (int j = 0; j < m_columnCount; j++) {
                            if (NULL == m_datasource) break;
                            newIndexPath = SNSIndexPath::create(m_topPointer, j, 1);
                            item = m_datasource->tableViewItemForCell(this, newIndexPath);
                        }
                    }
                }
                else if(m_topPointer == 0)
                {
                    SNSTableViewCellItem *item = NULL;
                    SNSIndexPath *newIndexPath = NULL;
                    for (int j = 0; j < m_columnCount; j++) {
                        if (NULL == m_datasource) break;
                        newIndexPath = SNSIndexPath::create(m_topPointer, j, 1);
                        item = m_datasource->tableViewItemForCell(this, newIndexPath);
                    }
                }

			}
		}
			break;
		case ReuseCellOrientationAfter:
		{
			// 纵向向下，横向向右复用(最左移最右)
			if (m_topPointer > 0) {
				indexPath = SNSIndexPath::create(m_bottomPointer - 1, 1);
				cell = (SNSTableViewCell *)m_body->getChildByTag(m_topPointer - 1);
			}
            else
            {
               	//设置预留的最下、最右cell
                SNSTableViewCellItem *item = NULL;
                SNSIndexPath *newIndexPath = NULL;
                if (m_bottomPointer >0) {
                    for (int j = 0; j < m_columnCount; j++) {
                        if (NULL == m_datasource) break;
                        newIndexPath = SNSIndexPath::create(m_bottomPointer-1, j, 1);
                        item = m_datasource->tableViewItemForCell(this, newIndexPath);
                    }
                }
            }
		}
			break;
		default:
			break;
	}
	if (cell && indexPath) {
		int tag = indexPath->getRow();
		CCAssert(tag >= 0, "tag must be more than equal to 0");
		cell->setTag(tag);
		// 重设cell的indexPath
		cell->setIndexPath(indexPath);
		// 重新设置cell的zindex以做到上压下
		//cell->getParent()->reorderChild(cell, -tag);//错误设置会引起渲染问题，先注掉
		//CCLOG("cell.tag:%d", cell->getTag);
		// 先改tag再移动，否则会出现找不到这个cell的问题
		moveCellToIndexPath(cell, indexPath);
	}
	return cell;
}

//根据头尾指针判断应该往哪个方向复用
void SNSTableView::reuseCellForOrientation(ReuseCellOrientation orientation)
{
	m_orientation = orientation;
	SNSIndexPath *indexPath = NULL;
	switch (m_orientation) {
		case ReuseCellOrientationBefore: // 纵向向上，横向向左复用(最右移最左)
		{
			if (m_topPointer > 0) {
				// 这里要传位移之后的cell的indexPath
				indexPath = SNSIndexPath::create(m_topPointer - 1, 1);
				// 调用重新设置cell的方法来更新cell
				getAndReuseCellWithIndexPath(indexPath);
			}
            else if(m_topPointer == 0){
                indexPath = SNSIndexPath::create(m_topPointer , 1);
                // 调用重新设置cell的方法来更新cell
				getAndReuseCellWithIndexPath(indexPath);
            }
		}
			break;
		case ReuseCellOrientationAfter: // 纵向向下，横向向右复用(最左移最右)
		{
			// 这里要传位移之后的cell的indexPath
			indexPath = SNSIndexPath::create(m_bottomPointer - 1, 1);
			// 调用重新设置cell的方法来更新cell
			getAndReuseCellWithIndexPath(indexPath);
		}
			break;
		default:
			break;
	}
}

void SNSTableView::moveCellToIndexPath(SNSTableViewCell *cell, SNSIndexPath *indexPath)
{
	float scrollWidth = 0;
    float scrollHeight = 0;
	unsigned int i = indexPath->getRow();
	//	cell.opacity = 0.3f;
	float widthOrHeight = getRowWidthOrHeight(indexPath);
	if (m_tableType == TableViewTypeVertical) {
		scrollWidth = m_frame.size.width;
		scrollHeight = m_bodySize.height;
		cell->setContentSize(CCSizeMake(scrollWidth - (m_padding << 1), widthOrHeight));
		cell->setPosition(ccp(m_padding, scrollHeight - (i + 1) * (widthOrHeight + m_margin)+m_down));
	} else if (m_tableType == TableViewTypeHorizontal) {
		//scrollWidth = self.bodySize.width;
		scrollHeight = m_frame.size.height;
		cell->setContentSize(CCSizeMake(widthOrHeight, scrollHeight - (m_padding << 1)));
		cell->setPosition(ccp((i + 1) * (widthOrHeight + m_margin) - (widthOrHeight + m_margin), m_padding));
	}
	//CCLOG("move %d to:%d", cell->getTag(), i);
	// 如果没有实例化过tableViewCellItem那么实例化它(复用情况下只能在这里实例化item)
	SNSTableViewCellItem *item = NULL;
	SNSIndexPath *newIndexPath = NULL;
	// 根据横纵设置item的contentSize
	if (m_tableType == TableViewTypeVertical) {
		m_itemSize = CCSizeMake(cell->getContentSize().width / m_columnCount, widthOrHeight);
	} else if (m_tableType == TableViewTypeHorizontal) {
		m_itemSize = CCSizeMake(widthOrHeight, cell->getContentSize().height / m_columnCount);
	}
	// 没辙，必须在这里设置cell的Item
	int nowCount;
	for (int j = 0; j < m_columnCount; j++) {
		if (NULL == m_datasource) break;
		newIndexPath = SNSIndexPath::create(indexPath->getRow(), j, indexPath->getSection());
		nowCount = getItemPointerWithIndexPath(newIndexPath);
		// 无论如何都要去执行一下取item的方法，否则初始化会有问题
		item = m_datasource->tableViewItemForCell(this, newIndexPath);
        //BYMM:解决商城多列的问题。如果不存在元素，则不添加，为什么要ASSERT保证ITEM不为NULL呢？
        if (!item)
            break;
		CCAssert(item, "If you overwrited delegate method:\"ItemForCellAtIndexPath\", so this function should not return a NULL value!");
		item->setContentSize(m_itemSize);
		item->setIndexPath(newIndexPath);
		//根据记录判断是否应该让他选中,需要在这里选中，如果在dequeue那里调用的话如果之前把item删掉重新创建，那么就无法选中了
		bool isSelected = m_selectedItem[getItemPointerWithIndexPath(newIndexPath)];
//		CCLOG("isSelected:%d", isSelected);
		item->setSelected(isSelected);
		
		if (nowCount >= m_itemCount) {
			// 如果当前读取的数据已经大于总数据量了，那么隐藏当前item
			item->setVisible(false);
		} else {
			// 否则显示他，并且做重置操作
			item->setVisible(true);
		}
		if (NULL == cell->getChildByTag(j)) {
			cell->addChild(item, 0, j);
			item->setPosition(getItemPositionWithIndexPath(newIndexPath, widthOrHeight));
			//CCLOG("item.width:%f, item.height:%f", item->getContentSize().width, item->getContentSize().height);
		}
//		else {
//			// 否则移除这个item
//			item->removeFromParentAndCleanup(true);
//		}
	}
	// 为了保证indexPath一直正确，所以最后再设置cell的indexPath（因为有可能这个indexPath是来自cell的indexPath，具体看代码）
	cell->setIndexPath(indexPath);
}

//复用item
SNSTableViewCellItem* SNSTableView::dequeueReusableCellItemForIndexPath(SNSIndexPath *indexPath)
{
	int row = indexPath->getRow();
	SNSTableViewCell *cell = (SNSTableViewCell *)m_body->getChildByTag(row);
    if (cell == NULL) {
        return NULL;
    }
	SNSTableViewCellItem *item = (SNSTableViewCellItem *)cell->getChildByTag(indexPath->getColumn());
	if (item) {
//		CCLOG("item::::::::::::%d, row:%d", indexPath->getColumn(), row);
		// 判断item是否是cellItem子类
		if (typeid(*item) == typeid(SNSTableViewCellItem)) {
			//只复用格子，内容物重新获取
			item->removeAllChildrenWithCleanup(true);
		}
		return item;
	}
	return NULL;
}

float SNSTableView::getRowWidthOrHeight(SNSIndexPath* indexPath)
{
	float rowWidthOrHeight = 64.0f;
    if ( m_delegate ) {
        rowWidthOrHeight = ((SNSTableViewDelegate *)m_delegate)->tableViewHeightForRow(this, indexPath);
    }
    return rowWidthOrHeight;
}

float SNSTableView::getCellPositionWithIndexPath(SNSIndexPath *indexPath)
{
	unsigned int row = indexPath->getRow();
	float returnValue = 0;
	for (int j = 0; j < row; j++) {
		returnValue += (getRowWidthOrHeight(SNSIndexPath::create(j, 0, 1)) + m_margin);
	}
	return returnValue;
}

CCPoint SNSTableView::getItemPositionWithIndexPath(SNSIndexPath* indexPath, float widthOrHeight)
{
	//计算每个item的size
	int xyValue = 0;
	int column = indexPath->getColumn();
    
	CCPoint point = ccp(0, 0);
	if (m_tableType == TableViewTypeVertical) {
        
		xyValue = (int)(m_bodySize.width / m_columnCount + 0.5f);
		xyValue = xyValue * column + xyValue * 0.5f;
		point = ccp(xyValue, widthOrHeight * 0.5f);
//        point = ccp(xyValue, widthOrHeight * 0.5f+m_down);
	} else if (m_tableType == TableViewTypeHorizontal) {
		// 为了让横屏的时候计算从上向下走
        column = m_columnCount - 1 - column;
		xyValue = (int)(m_bodySize.height / m_columnCount + 0.5f);
		xyValue = xyValue * column + xyValue * 0.5f;
		point = ccp(widthOrHeight * 0.5f, xyValue);
        
	}
    
	return point;
}

unsigned int SNSTableView::getItemPointerWithIndexPath(SNSIndexPath* indexPath)
{
	int pointer = -1;
	if (indexPath) {
		if ( 0 == indexPath->getRow() ) {
			pointer = indexPath->getColumn();
		} else {
			pointer = (indexPath->getRow() * m_columnCount) + indexPath->getColumn();
		}
	}
	return pointer;
}

CCSize SNSTableView::getItemSize()
{
	return m_itemSize;
}

void SNSTableView::moveToIndexPath(SNSIndexPath *indexPath)
{
//	SNSTableViewCell *cell = (SNSTableViewCell *)m_body->getChildByTag(indexPath->getRow());
//	CCPoint pos = ccp(0, -m_bodySize.height + m_frame.size.height);
//	if (cell) {
//		if (m_tableType == TableViewTypeVertical) {
//            pos = ccp(m_body->getPosition().x, -cell->getPosition().y + cell->getContentSize().height * 0.5f + m_margin);
//			if (pos.y > 0) {
//				pos = CCPointZero;
//			}
//		} else if (m_tableType == TableViewTypeHorizontal) {
//            pos = ccp(-cell->getPosition().x, m_body->getPosition().y);
//			if (pos.x > 0) {
//				pos = CCPointZero;
//			}
//		}
//	}
//	
//	moveBodyToPosition(pos);
    CCPoint originPos = ccp(m_frame.size.width, -m_bodySize.height + m_frame.size.height);
	CCPoint pos = originPos;
	float height = 0;
	for (int i = 0; i <= indexPath->getRow(); i++) {
		height += getRowWidthOrHeight(SNSIndexPath::create(i, 1)) + m_margin;//+47
	}
	if (m_tableType == TableViewTypeVertical) {
        //		CCLOG("pos:%f", -m_bodySize.height + m_frame.size.height + height);
		pos = ccp(m_body->getPositionX(), -m_bodySize.height + height + (m_frame.size.height - getRowWidthOrHeight(indexPath)));
		if (pos.y > 0) {
			pos = CCPointZero;
		}
		if (pos.y > originPos.y) {
			for (int i = 30; i > 0; i--) {
				m_body->setPosition(ccp(pos.x, pos.y + i));
				scrollViewDidScroll();
			}
		}
	} else if (m_tableType == TableViewTypeHorizontal) {
		pos = ccp(-height, m_body->getPositionY());
		if (pos.x > 0) {
			pos = CCPointZero;
		}
		if (pos.x < originPos.x) {
			for (int i = 30; i > 0; i--) {
				m_body->setPosition(ccp(pos.x - i, pos.y));
				scrollViewDidScroll();
			}
		}
	}
    
}

void SNSTableView::moveBodyToPosition1(CCPoint pos)
{
    moveBodyToPosition(pos);
}

#pragma mark - overwrite parent method

void SNSTableView::scrollViewDidScroll() {
 
//    if(!m_isMoveEnd)
//        return;
    
    SNSScrollView::scrollViewDidScroll();
    
    //tableview拖动的时候让所有的button都是不黑的
    for (int i=m_topPointer; i<m_bottomPointer;i++) {
        SNSTableViewCell *cell = (SNSTableViewCell*)m_body->getChildByTag(i);
        for (int j=0;j<m_columnCount; j++) {
            SNSTableViewCellItem *item = (SNSTableViewCellItem*)cell->getChildByTag(j);
            CCObject *obj = NULL;
            if (item) {
                CCARRAY_FOREACH(item->getAllBtn(), obj)
                {
                    CCNode *node = (CCNode *)obj;
                    ((CCMenuItem*)node)->unselected();
                }
            }
        }
        
    }
    
    
    CCLOG("scrollViewDidScroll");
    //如果有数据且底部指针不为0
    if (m_rowCount > 0 && m_bottomPointer > 0) {
//        CCLOG("顶部指针：%d", m_topPointer);
        //更新位置开始
		SNSTableViewCell *topCell = (SNSTableViewCell*)m_body->getChildByTag(m_topPointer);
		float scrollPos = 0;
		float topCellPos = 0;
		float topCellWidthOrHeight = 0;
		
		if (m_tableType == TableViewTypeVertical) {
			//如果是纵向滚动的话
			scrollPos = m_body->getPosition().y + m_bodySize.height - m_frame.size.height;
            CCLOG("m_body->getPosition().y= %f,m_bodySize.height =%f,m_frame.size.height=%f,topCell->getPosition().y=%f",m_body->getPosition().y,m_bodySize.height ,m_frame.size.height,topCell->getPosition().y);
			topCellPos = m_bodySize.height - topCell->getPosition().y;
			topCellWidthOrHeight = topCell->getContentSize().height;
		} else if (m_tableType == TableViewTypeHorizontal) {
			//如果是横向滚动的话
			scrollPos = -m_body->getPosition().x;
			topCellWidthOrHeight = topCell->getContentSize().width;
			// 这里加上一个cell的宽，否则如果当第一个cell有一半出屏幕的时候，他就不响应点击事件了
			topCellPos = topCell->getPosition().x + topCellWidthOrHeight;
		}
		
		// 如果是body向上或向左滑动的话
		if (scrollPos > topCellPos) {
			if (m_bottomPointer < m_rowCount) {
				++m_bottomPointer;
				reuseCellForOrientation(ReuseCellOrientationAfter);
				++m_topPointer;
				CCLOG("top:%d bottom:%d isAfter", m_topPointer, m_bottomPointer);
			}
		}
		// 如果是body向下或向右滑动的话
		if (scrollPos < (topCellPos - topCellWidthOrHeight)) {
			if (m_topPointer > 0) {
				--m_topPointer;
				reuseCellForOrientation(ReuseCellOrientationBefore);
				--m_bottomPointer;
				CCLOG("top:%d bottom:%d isBefore", m_topPointer, m_bottomPointer);
			}
		}
    }
}

void SNSTableView::returnNowPage(int nowPage, int pageCount)
{
	if (m_delegate) {
		((SNSTableViewDelegate *)m_delegate)->tableViewAtPage(this, nowPage, pageCount);
	}
}

void SNSTableView::scrollViewDidClick(CCPoint position, CCTouch* touch)
{
	//NSLog("点击：%f ** %f", position.x, position.y);
	CCRect frame;
    //position = ccpAdd(position, ccp(0, -m_down)); //区域点击可能有问题
	//如果delegate实现了点击函数那么进行处理，否则什么都不干！
	if (m_rowCount > 0 && m_delegate && ((SNSTableViewDelegate *)m_delegate)->isAchieve_tableViewSelectRow ) {
        
		//遍历指针之间的区域，获得点击位置
		for (int i = m_topPointer; i < m_bottomPointer; i++) {
            
			SNSTableViewCell *cell = (SNSTableViewCell *)m_body->getChildByTag(i);
            
			frame = CCRectMake(cell->getPosition().x, cell->getPosition().y, cell->getContentSize().width, cell->getContentSize().height);
//			CCLOG("frame:%f ++ %f ++ %f ++ %f -- %d", frame.origin.x, frame.origin.y, frame.size.width, frame.size.height, i);
            
			bool flagClick = false;//是否执行下面的
			
			if( frame.containsPoint(position) ) {
                
				//NSLog(@"点击了第%d个cell", i);
//				SNSIndexPath *indexPath = SNSIndexPath::create(i, 0);
				
				CCRect itemFrame = CCRectMake(0, 0, 0, 0);
				float itemWidthOrHeight = 0;
				CCSize itemSize = CCSizeMake(0, 0);
                
				for (int j = 0; j < m_columnCount; j++)
                {
                    
					if (m_tableType == TableViewTypeVertical) {
						itemWidthOrHeight = (int)(m_frame.size.width / m_columnCount + 0.5f);
						itemFrame = CCRectMake(cell->getPosition().x + (itemWidthOrHeight * j), cell->getPosition().y, itemWidthOrHeight, cell->getContentSize().height);
						itemSize = CCSizeMake(0, cell->getContentSize().height);
					} else if (m_tableType == TableViewTypeHorizontal) {
						itemWidthOrHeight = (int)(m_frame.size.height / m_columnCount + 0.5f);
						itemFrame = CCRectMake(cell->getPosition().x, cell->getPosition().y + (itemWidthOrHeight * (m_columnCount - j - 1)), cell->getContentSize().width, itemWidthOrHeight);
						itemSize = CCSizeMake(cell->getContentSize().width, 0);
					}
                    
					if (itemFrame.containsPoint(position) )
                    {
                        
						SNSIndexPath *itemIndexPath = SNSIndexPath::create(i, j, 0);
                        
						int now = getItemPointerWithIndexPath(itemIndexPath);
						if (now < m_itemCount) {
                            
//                            SNSTableViewCellItem *item = (SNSTableViewCellItem*)cell
                            int tag = 0;
							CCLog("点击了第%d个item", now);
							if ( m_delegate && ((SNSTableViewDelegate *)m_delegate)->isAchieve_tableViewSelectItem )
                            {
                                ((SNSTableViewDelegate *)m_delegate)->tableViewSelectItem(this, itemIndexPath);
                                SNSTableViewCellItem *item = (SNSTableViewCellItem*)cell->getChildByTag(j);
                                CCObject *obj = NULL;
                                if (item) {
                                    CCARRAY_FOREACH(item->getAllBtn(), obj)
                                    {
                                        CCPoint point = ccpSub(position, ccp(itemFrame.origin.x, itemFrame.origin.y));
                                        point = ccpSub(point, ccp(itemFrame.size.width*.5,itemFrame.size.height*.5));
                                        CCNode *node = (CCNode *)obj;
                                        
                                        CCRect btnRect = CCRectMake(node->getPosition().x-node->getContentSize().width*.5, node->getPosition().y-node->getContentSize().height*.5, node->getContentSize().width, node->getContentSize().height);
                                        if (btnRect.containsPoint(point))
                                        {
//                                            CCLOG("我去，点击了");
                                            tag = node->getTag();
                                            ((CCMenuItem*)node)->unselected();
                                            ((CCMenuItem*)node)->activate();
                                            break;
                                        }
                                    }
                                }
							}
                            
//                            CCLOG("dege seq = %d, tag = %d", i, tag);
							if ( m_delegate && ((SNSTableViewDelegate *)m_delegate)->isAchieve_tableViewSelectItemPoint )
                            {
								if (tag != -1)
								{
//									CCLOG("keep 1");
									((SNSTableViewDelegate *)m_delegate)->tableViewSelectItem(this, itemIndexPath, tag);
									flagClick = false;//true的时候会有重复点击问题
								}
								else
								{
//									CCLOG("keep 2");
									flagClick = true;
								}
							}
						}
						break;
					}
				}
			}
		}
	}
}

void SNSTableView::setBunSelected(CCPoint position, CCTouch* touch, bool isEnd)
{
	CCRect frame;
	//如果delegate实现了点击函数那么进行处理，否则什么都不干！
	if (m_rowCount > 0 && m_delegate && ((SNSTableViewDelegate *)m_delegate)->isAchieve_tableViewSelectRow ) {
        
		//遍历指针之间的区域，获得点击位置
		for (int i = m_topPointer; i < m_bottomPointer; i++) {
            
			SNSTableViewCell *cell = (SNSTableViewCell *)m_body->getChildByTag(i);
            
			frame = CCRectMake(cell->getPosition().x, cell->getPosition().y, cell->getContentSize().width, cell->getContentSize().height);

            
			if( frame.containsPoint(position) ) {

				
				CCRect itemFrame = CCRectMake(0, 0, 0, 0);
				float itemWidthOrHeight = 0;
				CCSize itemSize = CCSizeMake(0, 0);
                
				for (int j = 0; j < m_columnCount; j++) {
                    
					if (m_tableType == TableViewTypeVertical) {
						itemWidthOrHeight = (int)(m_frame.size.width / m_columnCount + 0.5f);
						itemFrame = CCRectMake(cell->getPosition().x + (itemWidthOrHeight * j), cell->getPosition().y, itemWidthOrHeight, cell->getContentSize().height);
						itemSize = CCSizeMake(0, cell->getContentSize().height);
					} else if (m_tableType == TableViewTypeHorizontal) {
						itemWidthOrHeight = (int)(m_frame.size.height / m_columnCount + 0.5f);
						itemFrame = CCRectMake(cell->getPosition().x, cell->getPosition().y + (itemWidthOrHeight * (m_columnCount - j - 1)), cell->getContentSize().width, itemWidthOrHeight);
						itemSize = CCSizeMake(cell->getContentSize().width, 0);
					}
                    
					if (itemFrame.containsPoint(position) ) {
                        
						SNSIndexPath *itemIndexPath = SNSIndexPath::create(i, j, 0);
                        
						int now = getItemPointerWithIndexPath(itemIndexPath);
						if (now < m_itemCount) {
                            
                            // SNSTableViewCellItem *item = (SNSTableViewCellItem*)cell
							CCLog("点击了第%d个item", now);
							if ( m_delegate && ((SNSTableViewDelegate *)m_delegate)->isAchieve_tableViewSelectItem )
                            {
                                SNSTableViewCellItem *item = (SNSTableViewCellItem*)cell->getChildByTag(j);

                                    CCObject *obj = NULL;
                                if (item) {
                                    CCARRAY_FOREACH(item->getAllBtn(), obj)
                                    {
                                        CCPoint point = ccpSub(position, ccp(itemFrame.origin.x, itemFrame.origin.y));
                                        point = ccpSub(point, ccp(itemFrame.size.width*.5,itemFrame.size.height*.5));
                                        CCNode *node = (CCNode *)obj;
                                        
                                        CCRect btnRect = CCRectMake(node->getPosition().x-node->getContentSize().width*.5, node->getPosition().y-node->getContentSize().height*.5, node->getContentSize().width, node->getContentSize().height);
                                        if (btnRect.containsPoint(point))
                                        {
                                            CCLOG("我去，点击了");
                                            //tag = node->getTag();
                                            if (((CCMenuItem*)node)->isSelected()) {
                                                ((CCMenuItem*)node)->unselected();
                                            }else{
                                                ((CCMenuItem*)node)->selected();
                                            }
                                            
                                            
                                            break;
                                        }
                                        else
                                        {
                                            ((CCMenuItem*)node)->unselected();
                                        }
                                        // }
                                    }
                                }

							}
                            
						}
					}
				}
                
			}
		}
	}
}


void SNSTableView::scrollViewCellDeleteAction(SNSScrollView* scrollView, CCPoint position)
{
    CCRect frame;
    
	//如果delegate实现了点击函数那么进行处理，否则什么都不干！
	if (m_rowCount > 0 && m_delegate && ((SNSTableViewDelegate *)m_delegate)->isAchieve_tableViewDeleteActionItem ) {
        
		//遍历指针之间的区域，获得点击位置
		for (int i = m_topPointer; i < m_bottomPointer; i++) {
            
			SNSTableViewCell *cell = (SNSTableViewCell *)m_body->getChildByTag(i);
            
			frame = CCRectMake(cell->getPosition().x, cell->getPosition().y, cell->getContentSize().width, cell->getContentSize().height);
            
			if( frame.containsPoint(position) ) {
                
				//NSLog(@"点击了第%d个cell", i);
				SNSIndexPath *indexPath = SNSIndexPath::create(i, 0);
				
				CCRect itemFrame = CCRectMake(0, 0, 0, 0);
				float itemWidthOrHeight = 0;
				CCSize itemSize = CCSizeMake(0, 0);
                
				for (int j = 0; j < m_columnCount; j++) {
                    
					if (m_tableType == TableViewTypeVertical) {
						itemWidthOrHeight = (int)(m_frame.size.width / m_columnCount + 0.5f);
						itemFrame = CCRectMake(cell->getPosition().x + (itemWidthOrHeight * j), cell->getPosition().y, itemWidthOrHeight, cell->getContentSize().height);
						itemSize = CCSizeMake(0, cell->getContentSize().height);
					} else if (m_tableType == TableViewTypeHorizontal) {
						itemWidthOrHeight = (int)(m_frame.size.height / m_columnCount + 0.5f);
						itemFrame = CCRectMake(cell->getPosition().x, cell->getPosition().y + (itemWidthOrHeight * (m_columnCount - j - 1)), cell->getContentSize().width, itemWidthOrHeight);
						itemSize = CCSizeMake(cell->getContentSize().width, 0);
					}
                    
					if ( itemFrame.containsPoint(position) ) {
                        
						SNSIndexPath *itemIndexPath = SNSIndexPath::create(i, j, 0);
                        
						int now = getItemPointerWithIndexPath(itemIndexPath);
						if (now < m_itemCount) {
							//CCLOG("横扫了第%d个item", now);
                            if ( m_delegate && ((SNSTableViewDelegate *)m_delegate)->isAchieve_tableViewDeleteActionItem ) {
                                SNSTableViewCellItem *item = (SNSTableViewCellItem*)cell->getChildByTag(j);
								//cell
								CCPoint point = ccpSub(position, ccp(itemFrame.origin.x, itemFrame.origin.y));
								//item
								point = ccpSub(point, ccp(item->getPosition().x - (itemSize.width * j), item->getPosition().y - (itemSize.height * j)));
                                
                                ((SNSTableViewDelegate *)m_delegate)->tableViewDeleteActionItem(this, indexPath, point);
                            }
						}
						break;
					}
				}
			}
		}
	}
}

//scrollView内容物拖动开始的事件
void SNSTableView::scrollViewDidStartDrag(CCPoint position)
{
    //CCLOG("拖动开始");
    CCRect frame;
    
	//如果delegate实现了点击函数那么进行处理，否则什么都不干！
	if (m_rowCount > 0 && m_delegate) {
        
		//遍历指针之间的区域，获得点击位置
		for (int i = m_topPointer; i < m_bottomPointer; i++) {
            
			SNSTableViewCell *cell = (SNSTableViewCell *)m_body->getChildByTag(i);
            
			frame = CCRectMake(cell->getPosition().x, cell->getPosition().y, cell->getContentSize().width, cell->getContentSize().height);
            
			if( frame.containsPoint(position) ) {
                
				//NSLog(@"点击了第%d个cell", i);
				SNSIndexPath *indexPath = SNSIndexPath::create(i, 0);
				
				CCRect itemFrame = CCRectMake(0, 0, 0, 0);
				float itemWidthOrHeight = 0;
				CCSize itemSize = CCSizeMake(0, 0);
                
				for (int j = 0; j < m_columnCount; j++) {
                    
					if (m_tableType == TableViewTypeVertical) {
						itemWidthOrHeight = (int)(m_frame.size.width / m_columnCount + 0.5f);
						itemFrame = CCRectMake(cell->getPosition().x + (itemWidthOrHeight * j), cell->getPosition().y, itemWidthOrHeight, cell->getContentSize().height);
						itemSize = CCSizeMake(0, cell->getContentSize().height);
					} else if (m_tableType == TableViewTypeHorizontal) {
						itemWidthOrHeight = (int)(m_frame.size.height / m_columnCount + 0.5f);
						itemFrame = CCRectMake(cell->getPosition().x, cell->getPosition().y + (itemWidthOrHeight * (m_columnCount - j - 1)), cell->getContentSize().width, itemWidthOrHeight);
						itemSize = CCSizeMake(cell->getContentSize().width, 0);
					}
                    
					if ( itemFrame.containsPoint(position) ) {
                        
						SNSIndexPath *itemIndexPath = SNSIndexPath::create(i, j, 0);
                        
						int now = getItemPointerWithIndexPath(itemIndexPath);
						if (now < m_itemCount) {
							//CCLOG("拖动第%d个item", now);
                            if ( m_delegate && ((SNSTableViewDelegate *)m_delegate)->isAchieve_tableViewDragStartItem) {
								m_isCanScroll = false;
                                SNSTableViewCellItem *item = (SNSTableViewCellItem*)cell->getChildByTag(j);
								//cell
								CCPoint point = ccpSub(position, ccp(itemFrame.origin.x, itemFrame.origin.y));
								//item
								point = ccpSub(point, ccp(item->getPosition().x - (itemSize.width * j), item->getPosition().y - (itemSize.height * j)));
                                
                                ((SNSTableViewDelegate *)m_delegate)->tableViewDidDragStartItem(this, indexPath, point);
                                m_lastDragIndex = indexPath;
                            }
						}
						break;
					}
				}
			}
		}
	}
}

//scrollView内容物拖动结束的事件
void SNSTableView::scrollViewDidEndDrag(CCPoint position)
{
    //CCLOG("拖动结束");
    if ( m_delegate && ((SNSTableViewDelegate *)m_delegate)->isAchieve_tableViewDragEndItem) {
        ((SNSTableViewDelegate *)m_delegate)->tableViewDidDragEndItem(this, m_lastDragIndex, position);
		m_isCanScroll = true;
    }
}

void SNSTableView::showScrollItemScale(SNSTableView* tableView)
{
    EFFECT_PLAY(MUSIC_ITEM);
    
    if (tableView == NULL) {
        return;
    }
    int rowCount = tableView->getRowCount();
    CCAction *action1 = NULL;
    CCAction *action2 = NULL;
    
    for (int ii = 0; ii < rowCount; ii++) {
        SNSTableViewCell *cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
        if (cell != NULL) {
            cell->stopAllActions();
            cell->setScale(0.01);
            action1 = CCSequence::create(CCDelayTime::create(ITEMACTIONTIME*(ii/10.0f+1)), CCScaleTo::create(ITEMACTIONTIME, 1),NULL);
            action2 = CCSequence::create(CCDelayTime::create(ITEMACTIONTIME*(ii/10.0f+1)), CCFadeIn::create(ITEMACTIONTIME),NULL);
            cell->runAction(action1);
            cell->runAction(action2);
        }
    }
}

void SNSTableView::showScrollItemMultiMove(SNSTableView* tableView)
{
    EFFECT_PLAY(MUSIC_ITEM);
    
    if (tableView == NULL) {
        return;
    }
    bool isSingleRow = true;
    CCAction *action = NULL;
    SNSTableViewCell *cell = NULL;
    int rowCount = tableView->getRowCount();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = CCPointZero;
    

    for (int ii = 0; ii < rowCount; ii++) {
        cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
        if (cell != NULL)
        {
            pos = cell->getPosition();
            cell->stopAllActions();
            if (isSingleRow)
            {
                isSingleRow = false;
                cell->setPosition(ccpAdd(ccp(0,pos.y), ccp(-size.width, 0)));
                action =  CCMoveBy::create(ITEMACTIONTIME, ccp(size.width, 0));

            }
            else{
                isSingleRow = true;
                cell->setPosition(ccpAdd(ccp(0,pos.y), ccp(size.width, 0)));
                action = //CCSequence::create(CCDelayTime::create(ITEMACTIONTIME*(ii/10.0f+1)),
                CCMoveBy::create(ITEMACTIONTIME, ccp(-size.width, 0));
                                            //NULL);
            }
            cell->runAction(action);
        }
    }
}


void SNSTableView::showScrollItemMultiMoveSlide(SNSTableView* tableView)
{
    EFFECT_PLAY(MUSIC_ITEM);
    
    if (tableView == NULL) {
        return;
    }
    CCAction *action = NULL;
    SNSTableViewCell *cell = NULL;
    int rowCount = tableView->getRowCount();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = CCPointZero;
    
	
    for (int ii = 0; ii < rowCount; ii++) {
        cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
        if (cell != NULL)
        {
            pos = cell->getPosition();
            cell->stopAllActions();
			cell->setPosition(ccpAdd(ccp(0,pos.y), ccp(size.width, 0)));
			action = CCSequence::create(CCDelayTime::create(0.15f*(ii/10.0f+1)),
												  CCMoveBy::create(0.15f, ccp(-size.width, 0)),
												  NULL);
			cell->runAction(action);
        }
    }
}

void SNSTableView::showScrollItemMultiMoveSlideHide(SNSTableView* tableView)
{
    EFFECT_PLAY(MUSIC_ITEM);
    
    if (tableView == NULL)
    {
        return;
    }
    CCAction *action = NULL;
    SNSTableViewCell *cell = NULL;
    int rowCount = tableView->getRowCount();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = CCPointZero;
    
	
    for (int ii = 0; ii < rowCount; ii++)
    {
        cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
        if (cell != NULL)
        {
            pos = cell->getPosition();
            cell->stopAllActions();
			//cell->setPosition(ccpAdd(ccp(0, pos.y), ccp(size.width, 0)));
			
			action = CCSequence::create(CCDelayTime::create(0.15f*(ii/10.0f+1)),
										CCMoveBy::create(0.15f, ccp(size.width, 0)),
										NULL);
			cell->runAction(action);
        }
    }
}




void SNSTableView::moveCellByDistance(int startCellIndex, float moveDistance,bool isMoveDown,float preDownDis)
{
    EFFECT_PLAY(MUSIC_ITEM);
    if (isMoveDown)
    {
        int rowCount = this->getRowCount();
        if (startCellIndex+1 >= rowCount)
        {
            return;
        }
        SNSTableViewCell *cell = NULL;
        for (int ii = startCellIndex+1; ii < rowCount; ii++)
        {
            cell = (SNSTableViewCell *)m_body->getChildByTag(ii);
            if (cell != NULL) {
                cell->stopAllActions();
//                cell->runAction(CCMoveBy::create(ITEMACTIONTIME/rowCount, ccp(0,moveDistance)));
                cell->setPosition(ccpAdd(cell->getPosition(), ccp(0,moveDistance)));
            }
        }
    }
    else
    {
        int rowCount = this->getRowCount();
        if (startCellIndex >= rowCount)
        {
            return;
        }
        SNSTableViewCell *cell = NULL;
        
        for (int ii = rowCount-1; ii >= 0; ii--)
        {
            cell = (SNSTableViewCell *)m_body->getChildByTag(ii);
            if (cell != NULL)
            {
                cell->stopAllActions();
                if (ii<=startCellIndex)
                {
                    cell->runAction(CCSequence::create(CCDelayTime::create(ITEMACTIONTIME/2),CCMoveBy::create(ITEMACTIONTIME/2, ccp(0,-moveDistance)),NULL));
                }
                else
                {
                    cell->runAction(CCSequence::create(CCMoveBy::create(ITEMACTIONTIME/2, ccp(0,-preDownDis)),CCMoveBy::create(ITEMACTIONTIME/2, ccp(0,-moveDistance)),NULL));
                }
                //cell->setPosition(ccpAdd(cell->getPosition(), ccp(0,-moveDistance)));
            }
        }
        
    }

}

void SNSTableView::setCellByDistance(int startCellIndex, float moveDistance,bool isMoveDown,float preDownDis)
{
    EFFECT_PLAY(MUSIC_ITEM);
    if (isMoveDown)
    {
        int rowCount = this->getRowCount();
        if (startCellIndex+1 >= rowCount)
        {
            return;
        }
        SNSTableViewCell *cell = NULL;
        for (int ii = startCellIndex+1; ii < rowCount; ii++)
        {
            cell = (SNSTableViewCell *)m_body->getChildByTag(ii);
            if (cell != NULL) {
                cell->stopAllActions();
                cell->setPosition(ccpAdd(cell->getPosition(), ccp(0,moveDistance)));
            }
        }
    }
    else
    {
        int rowCount = this->getRowCount();
        if (startCellIndex >= rowCount)
        {
            return;
        }
        SNSTableViewCell *cell = NULL;
        
        for (int ii = rowCount-1; ii >= 0; ii--)
        {
            cell = (SNSTableViewCell *)m_body->getChildByTag(ii);
            if (cell != NULL)
            {
                cell->stopAllActions();
                if (ii<=startCellIndex)
                {
                    cell->setPosition(ccpAdd(cell->getPosition(), ccp(0,-moveDistance)));
                }
                else
                {
                     cell->setPosition(ccpAdd(cell->getPosition(), ccp(0,-preDownDis-moveDistance)));
                }

            }
        }
        
    }
    
}

CCRect SNSTableView::getScissorFrame()
{
    return m_frame;
}






///////////kanata
void SNSTableView::showScrollItemMultiMoveFromRight(SNSTableView* tableView)
{
    EFFECT_PLAY(MUSIC_ITEM);
    
    if (tableView == NULL)
    {
        return;
    }
    tableView->cellmove();
//    int rowCount = tableView->getRowCount();
//    tableView-> m_isMoveEnd = false;
//    int cellnum = 0;
//    CCPoint pos = CCPointZero;
//    CCSize size = CCDirector::sharedDirector()->getWinSize();
//    for (int ii = 0; ii < rowCount; ii++) {
//        SNSTableViewCell *cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
//        if (cell != NULL) {
//            CCPoint pos = cell->getPosition();
//            cell->setPosition(ccpAdd(pos, ccp(size.width, 0)));
//            cellnum+=1;
//        }
//    }
//    for (int ii = 0; ii < cellnum; ii++) {
//        SNSTableViewCell *cell = (SNSTableViewCell *)tableView->m_body->getChildByTag(ii);
//        
//        if (cell != NULL) {
//            //////////效果优化注释了
//            //                CCAction *action = CCSequence::create(CCDelayTime::create(0.15f*(ii/10.0f+1)),
//            //                                                      CCMoveBy::create(0.15f, ccp(-size.width, 0)),
//            //                                                      NULL);
//            //////优化效果
//            CCMoveBy*moveby1=CCMoveBy::create(0.15f, ccp(-size.width*1.05, 0));
//            CCMoveBy*moveby2=CCMoveBy::create(0.15f, ccp(size.width*0.1, 0));
//            CCMoveBy*moveby3=CCMoveBy::create(0.15f, ccp(-size.width*0.05, 0));
//            CCCallFunc *call = CCCallFunc::create(tableView, (ii == cellnum -1) ? callfunc_selector(SNSTableView::endMove) : NULL);
//            CCAction *action = CCSequence::create(CCDelayTime::create(0.15f*(ii/10.0f+1)),moveby1,moveby2,moveby3, call,NULL);
//            
//            cell->runAction(action);
//        }
//    }

}

void SNSTableView::cellmove()
{

    int rowCount = m_rowCount;
    m_isMoveEnd = false;
    int cellnum = 0;
    CCPoint pos = CCPointZero;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    for (int ii = 0; ii < rowCount; ii++)
    {
        SNSTableViewCell *cell = (SNSTableViewCell *)m_body->getChildByTag(ii);
        if (cell != NULL)
        {
            CCPoint pos = cell->getPosition();
            cell->setPosition(ccpAdd(pos, ccp(size.width, 0)));
            cellnum+=1;
        }
    }
    for (int ii = 0; ii < cellnum; ii++)
    {
        SNSTableViewCell *cell = (SNSTableViewCell *)m_body->getChildByTag(ii);
        
        if (cell != NULL)
        {

            //////优化效果
            CCMoveBy*moveby1=CCMoveBy::create(0.15f, ccp(-size.width*1.05, 0));
            CCMoveBy*moveby2=CCMoveBy::create(0.15f, ccp(size.width*0.1, 0));
            CCMoveBy*moveby3=CCMoveBy::create(0.15f, ccp(-size.width*0.05, 0));
            CCCallFunc *call = CCCallFunc::create(this, (ii == cellnum -1) ? callfunc_selector(SNSTableView::endMove) : NULL);
            CCAction *action = CCSequence::create(CCDelayTime::create(0.15f*(ii/10.0f+1)),moveby1,moveby2,moveby3, call,NULL);
            
            cell->runAction(action);
        }
    }


}

bool SNSTableView::isMoveEnd()
{
    return m_isMoveEnd;
}
