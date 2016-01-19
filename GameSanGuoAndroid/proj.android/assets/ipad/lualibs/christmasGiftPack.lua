--[[
activity  圣诞礼包 送张飞
by:angel
2014-12-16
--]]

--引入了工程中预定义的字体、颜色值的lua版本。
require "lualibs/common.lua"


function __G__TRACKBACK__(msg)
    print("----------------------------------------")  
    print("LUA ERROR: " .. tostring(msg) .. "\n")  
    print(debug.traceback())  
    print("----------------------------------------")  
end

local function main()
	collectgarbage("setpause",100)
	collectgarbage("setstepmul",5000)
	
	--ToDo:在这里绑定资源
	ResourceManager:sharedInstance():bindTexture("sanguobigpic/activityChristmasGiftPack.plist" , 1 , 191)
	
	
	--通用。
	local visibleSize = CCDirector:sharedDirector():getVisibleSize()
    local scaleRate = visibleSize.height/960
    local scaleRate2 = visibleSize.width/640
	
	
	--strBg 背景图片   str 是前景字串  strSend 点击响应发送的消息
	--ret tag = baseTag , 背景 baseTag+1 前景图 baseTag + 2  前景label baseTag + 3
	local function createLayerBtn(baseTag, strBg ,str, strSend, isSendWithObj) --baseTag=101
		local ret = CCLayer:create()
		--背景 ，图片在public1中。
		local btnBg = CCSprite:createWithSpriteFrameName(strBg) -- "an.png"
		btnBg:setAnchorPoint(ccp(0.5,0.5))
		btnBg:setPosition(0,0)
		btnBg:setTag(baseTag + 1 )
		print("bn背景图片 = "..baseTag+1)
		
		
		local btnBgSize = btnBg:getContentSize()
		--设置该layer的大小
		ret:setContentSize(btnBgSize)
		ret:addChild(btnBg)
		
		
        --前景图片字 或者 前景图片  目前没有使用

        --[[
        local spriteLabel = CCSprite:createWithSpriteFrameName("firstChargeLabel.png");
		spriteLabel:setPosition(ccp(btnBgSize.width * 0.5 , btnBgSize.height * 0.5) )
		spriteLabel:setTag(baseTag + 2 )
		print("btn图片字Tag = "..baseTag + 2 )
		btnBg:addChild(spriteLabel)
        --]]
		

		--前景文字
		local fronLabel = SGCCLabelTTF:create(str , FONT_PANGWA , 30)
		fronLabel:setPosition(ccp(btnBgSize.width * 0.5 , btnBgSize.height * 0.5) )
		fronLabel:setTag(baseTag + 3)
		print("btn字Tag = "..baseTag + 3)
		btnBg:addChild(fronLabel)
				
		--关于new_local请参考：http://blog.csdn.net/wtyqm/article/details/8977975
		--c++ tolua时构造函数自动生成new_local
		local spriteBorderSize = btnBg:getContentSize()
		local rect =  CCRect:new_local( -spriteBorderSize.width * 0.5 ,  -spriteBorderSize.height * 0.5,spriteBorderSize.width , spriteBorderSize.height )
		local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
		local beganPosY = 0
		--touch callback
		local function onTouchBegan(x, y)
			if rect:containsPoint(ret:convertToNodeSpace(ccp(x,y ) ) ) and ret:getParent():isVisible()  then
				isTouchMoved = false
				beganPosY = y
				return true
			else
				return false	
			end
		end
		
		--触控结束时发送消息 
		local function onTouchEnded(x , y)
			if (false == ret:isVisible() ) then
				return
			end

			local sender = SGNotificationCenter:sharedNotificationCenter()
			if isSendWithObj then
                sender:postNotificationWithObj(strSend,ret)  -- "sendRewardReq"  set the get reward msg 
		    else
		    	sender:postNotification(strSend) --"showStoreLayer" go to recharge layer  ,
		    end
			
			--btnBg:setColor(ccc3(255,255,255))
		end
		
		--处理触控的回调
		local function onTouch(kind , x , y)
			print(kind)--观察下kind具体的内容。
			if "began"==kind then
				return onTouchBegan(x , y)
			elseif "moved" == kind then
				local deltaPoxY  = math.abs(y-beganPosY)
				    print ("*********** ** * * dela posY * ** * ** * **"..deltaPoxY)
				    if deltaPoxY >= 20 then 
				        isTouchMoved = true
				    end
			elseif "ended" == kind then
				if false == isTouchMoved then
					return onTouchEnded(x, y)
				end
			end
		end
		
		--注册回调
		ret:registerScriptTouchHandler(onTouch)
		ret:setTouchEnabled(true)
		
		ret:setTag(baseTag)
		print("按钮LayerTag = "..baseTag)
		
		return ret		
	end
	
	--************************************function createChargeLayerBtn(baseTag)  end.
	
	
	
	--************************************创建一个模板
	--对比效果图  创建一个可以放置n个iconbutton的layer，带背景。
	--左上方是一个label，可修改内容，右中有一个领的按钮
	--arguments:count是icon框的个数，baseTag是该layer中的起始tag。
	local function createTemplateLayer(count , baseTag, buttonTag)--此时baseTag = 201 , count=4
		local ret = CCLayer:create()
				
		--可获得物品的背景图片，=layer的背景图片
		local spriteCanGainBg = CCSprite:createWithSpriteFrameName("activityCanGainBg.png");
		spriteCanGainBg:setAnchorPoint(ccp(0 , 0))
		spriteCanGainBg:setPosition(0 , 0)
		spriteCanGainBg:setTag(baseTag) --baseTag=201
		print("可获得物品背景图片Tag = "..baseTag)
		baseTag = baseTag + 1
		ret:addChild(spriteCanGainBg)
		
		--设置该layer的大小 =背景的大小
		ret:setContentSize(spriteCanGainBg:getContentSize() )
		
		--昨上方描述文本
		local labelDes = CCLabelTTF:create("登陆一天奖励" , FONT_PANGWA , 24)
		labelDes:setAnchorPoint(ccp(0,0))
		labelDes:setPosition(30 ,170 )
		labelDes:setTag(baseTag) --baseTag=202
		print("可获得物品描述的labelTag = "..baseTag)
		baseTag = baseTag + 1
		ret:addChild(labelDes)
		
		--n个可放置icon的小框框
		for i = 1 , count do
			--用来响应触摸的layer容器
			local tempLayer = CCLayer:create()
			
			--该icon容器layer的背景图片
			local spriteBorder = CCSprite:createWithSpriteFrameName("card_bj.png")
			spriteBorder:setAnchorPoint(ccp(0.5,0.5))
			spriteBorder:setPosition(ccp(0,0))
			spriteBorder:setTag(baseTag)
			print("i = "..i.."icon容器layer的tag"..baseTag)
			baseTag = baseTag + 1
			tempLayer:addChild(spriteBorder)
			
			--该容器layer的size
			local spriteBorderSize = spriteBorder:getContentSize()
			tempLayer:setContentSize(spriteBorderSize)
			
			--触摸区域
			local rect = CCRect:new_local( -spriteBorderSize.width * 0.5 ,  -spriteBorderSize.height * 0.5,spriteBorderSize.width , spriteBorderSize.height )
			local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
			local beganPosY = 0
			
			--touch callback			
			local function onTouchBegan(x, y)
				if rect:containsPoint(tempLayer:convertToNodeSpace(ccp(x,y)))  and tempLayer:getParent():isVisible() then
					isTouchMoved = false
					beganPosY = y
					return true
				else
					return false
				end
			end

			local function onTouchEnded(x, y)
				if false == tempLayer:isVisible() then
					return 
				end
				local sender = SGNotificationCenter:sharedNotificationCenter()
				sender:postNotificationWithObj("showFirstChagreItem",tempLayer)
				--spriteBorder:setColor(ccc3(255,255,255))
			end

			local function onTouch(eventType, x, y)
                print(eventType)
				if eventType == "began" then   
					return onTouchBegan(x, y)
				elseif eventType == "moved" then
					local deltaPoxY  = math.abs(y-beganPosY)
				    print ("*********** ** * * dela posY * ** * ** * **"..deltaPoxY)
				    if deltaPoxY >= 20 then 
				        isTouchMoved = true
				    end
				elseif eventType == "ended" then
					if isTouchMoved == false then
						return onTouchEnded(x, y)
					end
				end
			end
			
			--注册触控回调
			tempLayer:registerScriptTouchHandler(onTouch)
			tempLayer:setTouchEnabled(true)
			
			tempLayer:setPosition(ccp(35+56 + (i-1) * 113  , 60+46 ))
			tempLayer:setTag(baseTag)
			print("i = "..i.." , iconlayerTag = "..baseTag)
			baseTag = baseTag + 1
			
			ret:addChild(tempLayer)
		end
		
		--领奖的按钮的layer btn
		local function createRewardLayer(baseTag) --baseTag=240
			local ret = CCLayer:create()
			local spriteReward = CCSprite:createWithSpriteFrameName("reward_lingq.png")
			--spriteReward:setPosition(528 , ret:getContentSize().height * 0.5)
			spriteReward:setAnchorPoint(ccp(0,0))
			spriteReward:setPosition(0,0)
			spriteReward:setTag(baseTag+1)
			print("领奖图片tag = "..baseTag)
			
			ret:addChild(spriteReward)
			
			--大小
			local size = spriteReward:getContentSize()
			ret:setContentSize(size)
			
			--触控区域
			local rect = CCRect:new_local(0,0 ,size.width , size.height)
			local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
			local beganPosY = 0

			--touch callback			
			local function onTouchBegan(x, y)
				if rect:containsPoint(ret:convertToNodeSpace(ccp(x,y)))  and ret:getParent():isVisible() then
					isTouchMoved = false
					beganPosY = y
					return true
				else
					return false
				end
			end

			local function onTouchEnded(x, y)
				if false == ret:isVisible() then
					return
				end
				local sender = SGNotificationCenter:sharedNotificationCenter()
				sender:postNotificationWithObj("sendRewardReq",ret)
			end

			local function onTouch(eventType, x, y)
                print(eventType)
				if eventType == "began" then   
					return onTouchBegan(x, y)
				elseif eventType == "moved" then
					local deltaPoxY  = math.abs(y-beganPosY)
				    print ("*********** ** * * dela posY * ** * ** * **"..deltaPoxY)
				    if deltaPoxY >= 20 then 
				        isTouchMoved = true
				    end
				elseif eventType == "ended" then
					if isTouchMoved == false then
						return onTouchEnded(x, y)
					end
				end
			end
			
			--注册触控回调
			ret:registerScriptTouchHandler(onTouch)
			ret:setTouchEnabled(true)
			
			ret:setTag(baseTag)
			print("领奖layerTag = "..baseTag)
			baseTag = baseTag + 4
			return ret
		end
		
		local rewardLayer = createRewardLayer(buttonTag)-- 此时baseTag=240
        rewardLayer:ignoreAnchorPointForPosition(false)
        rewardLayer:setAnchorPoint(ccp(0.5,0.5))
		rewardLayer:setPosition(525 , ret:getContentSize().height * 0.5)
		ret:addChild(rewardLayer)
		
		--描述文本右边  数量比例。--该活动没有这边的这个比例文本
		--[[local labelNum = CCLabelTTF:create("50000/50000" , FONT_PENGYOU , 24)
		--labelNum:setAnchorPoint(ccp(0,0))
		--labelNum:setPosition(ccp( labelDes:getContentSize().width + labelDes:getPositionX() + 20 , labelDes:getPositionY() ) )
        labelNum:setAnchorPoint(ccp(1,0))
        labelNum:setPosition(460 , labelDes:getPositionY() )
		labelNum:setTag(baseTag)
		print("580/500 tag = "..baseTag)
		baseTag = baseTag + 1
		ret:addChild(labelNum)
		
		--是否已领取文本
		local  hasGot = CCLabelTTF:create("" , FONT_PANGWA , 24)
		hasGot:setAnchorPoint(ccp(0,0))
		hasGot:setPosition(ccp(ret:getContentSize().width - 93 - 15 , labelNum:getPositionY() ) )
		hasGot:setTag(baseTag)
		print("(已领取) tag = " .. baseTag)
		baseTag = baseTag + 1
		ret:addChild(hasGot)
		
		
		ret:setTag(baseTag)
		print("可获得物品容器layer = "..baseTag)
		baseTag = baseTag + 1
		--]]
		return ret
	end
	
	--************************************function createTemplateLayer()   end.
	
	
	
	
	
	------本lua返回的layer
	local function createRetLayer(baseTag)
		
		--
		
        local retLayer = CCLayer:create()
	
		local visibleSize = CCDirector:sharedDirector():getVisibleSize()
		local orign = CCDirector:sharedDirector():getVisibleOrigin()
		local margin = 350 --上下的边距和 活动scroll view 距上下底边的和
		local spaceing  = 15 -- 大的item之间的距离

        --创建购买礼包区域的背景
        local buyBg = CCSprite:createWithSpriteFrameName("activityBuyBg.png")
        buyBg:ignoreAnchorPointForPosition(false)
        buyBg:setAnchorPoint(ccp(0.5, 0.5))
        retLayer:addChild(buyBg)

		--该活动的背景图片
		local spriteBg = CCSprite:createWithSpriteFrameName("activityChristmasGiftPackBg.png")
		spriteBg:setAnchorPoint(ccp(0.5,0.5))
		spriteBg:setTag(baseTag)
		print("活动背景图片的tag = "..baseTag)
		baseTag = baseTag + 1
		retLayer:addChild(spriteBg)
        --居中适配
        local isCenter = false 
        local nsub = visibleSize.height - margin - (buyBg:getContentSize().height + spriteBg:getContentSize().height + spaceing)
        if nsub >=  0 then
        	isCenter = true
        end
        local noffset = 0;
        if isCenter then 
        	--内容要居中 居中偏移量为 nsub的一半
           noffset = nsub * 0.5

        else
        	--内容要顶住上边 偏移为0 
           noffset = 0
        end

        buyBg:setPosition(visibleSize.width * 0.5, noffset + (buyBg:getContentSize().height * 0.5))

		spriteBg:setPosition( visibleSize.width * 0.5 , buyBg:getPositionY() + buyBg:getContentSize().height * 0.5 +spaceing + spriteBg:getContentSize().height *0.5  )
        
        --给item 添加内容
        local goldBorder = createLayerBtn(80,"card_bj.png","","showFirstChagreItem",true)-- 
        goldBorder:ignoreAnchorPointForPosition(false)
        goldBorder:setAnchorPoint(ccp(0.5, 0.5))
        goldBorder:setPosition(buyBg:getPositionX() - 200 + goldBorder:getContentSize().width*0.5 ,buyBg:getPositionY()+ goldBorder:getContentSize().height*0.5)
        retLayer:addChild(goldBorder)

        --[[ 
        local itemIcon = CCSprite:createWithSpriteFrameName("activityGiftIcon.png")
        itemIcon:ignoreAnchorPointForPosition(false)
        itemIcon:setAnchorPoint(ccp(0.5, 0.5))
        itemIcon:setPosition(goldBorder:getPositionX(), goldBorder:getPositionY())
        retLayer:addChild(itemIcon)
        --]]

        local buyOneBtn = createLayerBtn(100, "an.png", "購買1個", "sendRewardReq" ,true)
        buyOneBtn:ignoreAnchorPointForPosition(false)
        buyOneBtn:setAnchorPoint(ccp(0.5, 0.5))
        buyOneBtn:setPosition(buyBg:getPositionX() - 10 + buyOneBtn:getContentSize().width*0.5  ,buyBg:getPositionY() - 10 + buyOneBtn:getContentSize().height*0.5)
        retLayer:addChild(buyOneBtn)

        local buyTenBtn = createLayerBtn(200, "an.png", "購買10個", "sendRewardReq" ,true)
        buyTenBtn:ignoreAnchorPointForPosition(false)
        buyTenBtn:setAnchorPoint(ccp(0.5, 0.5))
        buyTenBtn:setPosition(buyBg:getPositionX() + 200 -10  + buyTenBtn:getContentSize().width*0.5 ,buyBg:getPositionY() -10 + buyTenBtn:getContentSize().height*0.5)
        retLayer:addChild(buyTenBtn)

        local ybIcon = CCSprite:createWithSpriteFrameName("goldicon.png")
        ybIcon:ignoreAnchorPointForPosition(false)
        ybIcon:setAnchorPoint(ccp(0.5, 0.5))
        ybIcon:setPosition(buyBg:getPositionX() - 43  ,buyBg:getPositionY() - 72)
        retLayer:addChild(ybIcon)


        local ybIcon2 = CCSprite:createWithSpriteFrameName("goldicon.png")
        ybIcon2:ignoreAnchorPointForPosition(false)
        ybIcon2:setAnchorPoint(ccp(0.5, 0.5))
        ybIcon2:setPosition(buyBg:getPositionX() + 200 -43  ,buyBg:getPositionY() -72)
        retLayer:addChild(ybIcon2)

        local label1 = SGCCLabelTTF:create("100" , FONT_PANGWA , 24,CCSizeMake(80,30),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter,ccc3(0,255,0),ccc3(0,0,0),2,true)
        label1:ignoreAnchorPointForPosition(false)
        label1:setAnchorPoint(ccp(0.5, 0.5))
        label1:setPosition(ybIcon:getPositionX() + 100  ,ybIcon:getPositionY() + 15)
        label1:setTag(50)
        retLayer:addChild(label1)


        local label2 = SGCCLabelTTF:create("900" , FONT_PANGWA , 24,CCSizeMake(80,30),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter,ccc3(0,255,0),ccc3(0,0,0),2,true)
        label2:ignoreAnchorPointForPosition(false)
        label2:setAnchorPoint(ccp(0.5, 0.5))
        label2:setPosition(ybIcon2:getPositionX() + 100 ,ybIcon2:getPositionY() +15)
        label2:setTag(51)
        retLayer:addChild(label2)



        local label3 = SGCCLabelTTF:create("當前擁有:        個" , FONT_PANGWA , 24,CCSizeMake(180,30),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter,ccc3(0,255,0),ccc3(0,0,0),2,true)
        label3:ignoreAnchorPointForPosition(false)
        label3:setAnchorPoint(ccp(0.5, 0.5))
        label3:setPosition(buyBg:getPositionX() + 180 ,buyBg:getPositionY() +75)
        label3:setTag(52)
        retLayer:addChild(label3)

        local label4 = SGCCLabelTTF:create("0" , FONT_PANGWA , 24)
        label4:ignoreAnchorPointForPosition(false)
        label4:setAnchorPoint(ccp(0.5, 0.5))
        label4:setPosition(buyBg:getPositionX() + 134 ,buyBg:getPositionY() +75)
        label4:setTag(53)
        retLayer:addChild(label4)


        local label5 = SGCCLabelTTF:create("" , FONT_PANGWA , 24)
        label5:ignoreAnchorPointForPosition(false)
        label5:setAnchorPoint(ccp(0.5, 0.5))
        label5:setPosition(buyBg:getPositionX()+ 20  ,buyBg:getPositionY() +105)
        label5:setTag(54)
        retLayer:addChild(label5)

        --添加内容 end 
		

		--2个label，第一个  活动时间
		local labelActivityTime = SGCCLabelTTF:create("活动时间 ： 2014/12/24 – 2015/1/3" , FONT_PANGWA , 20)
		labelActivityTime:setPosition(spriteBg:getContentSize().width * 0.5 , 58)
		labelActivityTime:setTag(baseTag)
		print("活动时间tag = "..baseTag)
		baseTag = baseTag + 1
		spriteBg:addChild(labelActivityTime)
		--第二个label  活动描述
		local labelActivityDes = SGCCLabelTTF:create("活动期间，购买大礼包,可以开出张飞整个或者碎片" , FONT_PANGWA , 22)
		labelActivityDes:setPosition(spriteBg:getContentSize().width * 0.5 , 20)
		labelActivityDes:setTag(baseTag)
		print("活动描述tag = "..baseTag)
		baseTag = baseTag + 1
		spriteBg:addChild(labelActivityDes)
        
        if isCenter then 
        	retLayer:setContentSize( CCSize:new_local( visibleSize.width , visibleSize.height - margin + 10 ) )
        else
        	retLayer:setContentSize( CCSize:new_local( visibleSize.width , spriteBg:getPositionY() + spriteBg:getContentSize().height * 0.5 + spaceing ) )
        end
        
        print("最顶层高**** "..retLayer:getContentSize().height)

        --红色的背景
		local spriteRedBg = CCSprite:createWithSpriteFrameName("activitybg.png")
		local scaleX = 1.0 * visibleSize.width / 245
        local scaleY = 1.0 * (retLayer:getContentSize().height ) / 233
		spriteRedBg:setScaleX(scaleX)
		spriteRedBg:setScaleY(scaleY)
		spriteRedBg:setAnchorPoint(ccp(0.5,0.5 ))
        spriteRedBg:setPosition(retLayer:getContentSize().width * 0.5 , retLayer:getContentSize().height*0.5 )


		retLayer:addChild(spriteRedBg , -100 , -100)
        retLayer:setTouchEnabled(true)
		retLayer:setTag(baseTag)
		print("最顶层layer的Tag = "..baseTag)
		baseTag = baseTag + 1
		return retLayer
	end
	
	--添加到当前执行的scene中。
	local runningScene = CCDirector:sharedDirector():getRunningScene()
	runningScene:addChild(createRetLayer(1) , 1  ,654355)
end

xpcall ( main , __G__TRACKBACK__)

--******************************file end.************************************