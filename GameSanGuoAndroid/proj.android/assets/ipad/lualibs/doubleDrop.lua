--[[
双倍掉落
by:angel
2014-12-08
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
	ResourceManager:sharedInstance():bindTexture("sanguobigpic/activityDoubleDrop.plist" , 1 , 191)
	
	
	--通用。
	local visibleSize = CCDirector:sharedDirector():getVisibleSize()
    local scaleRate = visibleSize.height/960
    local scaleRate2 = visibleSize.width/640
	
	--前往充值的按钮，单建一个layer
	--每次执行完setTag后  需对baseTag自增1
	local function createChargeLayerBtn(baseTag) --baseTag=101
		local ret = CCLayer:create()
		--前往储值的背景 ，图片在public1中。
		local btnBg = CCSprite:createWithSpriteFrameName("an.png")
		btnBg:setAnchorPoint(ccp(0,0))
		btnBg:setPosition(0,0)
		btnBg:setTag(baseTag)
		print("前往储值的背景图片 = "..baseTag)
		baseTag = baseTag + 1
		
		local btnBgSize = btnBg:getContentSize()
		--设置该layer的大小
		ret:setContentSize(btnBgSize)
		ret:addChild(btnBg)
		
		
		--前往储值的图片字
		local spriteLabel = CCSprite:createWithSpriteFrameName("firstChargeLabel.png");
		spriteLabel:setPosition(ccp(btnBgSize.width * 0.5 , btnBgSize.height * 0.5) )
		spriteLabel:setTag(102)--baseTag)
		print("前往储值的图片字Tag = "..102)
		--baseTag = baseTag + 1
		btnBg:addChild(spriteLabel)
				
		--关于new_local请参考：http://blog.csdn.net/wtyqm/article/details/8977975
		--c++ tolua时构造函数自动生成new_local
		local rect = CCRect:new_local(0,0 , btnBgSize.width , btnBgSize.height)
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
		
		--触控结束时发送消息  前往储值处
		local function onTouchEnded(x , y)
			if (false == ret:isVisible() ) then
				return
			end

			local sender = SGNotificationCenter:sharedNotificationCenter()
			sender:postNotification("showStoreLayer")
			btnBg:setColor(ccc3(255,255,255))
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
		
		ret:setTag(102)--baseTag)
		print("前往储值的layerTag = "..102)
		--baseTag = baseTag + 1
		return ret		
	end
	
	--************************************function createChargeLayerBtn(baseTag)  end.
	
	
	
	
	
	
	
	
	------本lua返回的layer
	local function createRetLayer(baseTag)
		
		--
		
        local retLayer = CCLayer:create()
	
		local visibleSize = CCDirector:sharedDirector():getVisibleSize()
		local orign = CCDirector:sharedDirector():getVisibleOrigin()
	
       
       --描述框的背景
       local spriteDecribeBg = CCSprite:createWithSpriteFrameName("barrack_kuangbg.png")
       local sY = 144/(spriteDecribeBg:getContentSize().height)
       local sX = 586/spriteDecribeBg:getContentSize().width
       spriteDecribeBg:setScaleX(sX)
       spriteDecribeBg:setScaleY(sY)       
       retLayer:addChild(spriteDecribeBg)

       -- 最下边的描述框
       local spriteDescribe = CCSprite:createWithSpriteFrameName("doubleDropBox.png")
       spriteDescribe:setAnchorPoint(ccp(0.5,0.5))
       spriteDescribe:setPosition(visibleSize.width/2,spriteDescribe:getContentSize().height/2+10)
       retLayer:addChild(spriteDescribe)
       spriteDecribeBg:setPosition(spriteDescribe:getPositionX(),spriteDescribe:getPositionY())


       local labeldes = SGCCLabelTTF:create("关物品掉落翻倍男不男女不女那你你你你  按阿娜达" , FONT_PANGWA , 24,CCSizeMake(500,100),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop,ccc3(255,255,255),ccc3(0,0,0),2,true)
       labeldes:setPosition(labeldes:getContentSize().width*0.6-10,labeldes:getContentSize().height/2+20)
       spriteDescribe:addChild(labeldes)
       labeldes:setTag(4)

       --中间的对话框
       local spriteDialog = CCSprite:createWithSpriteFrameName("activityDoubleDropDialog.png")
       spriteDialog:setAnchorPoint(ccp(0.5,0.5))
       spriteDialog:setPosition(visibleSize.width/2,spriteDescribe:getPositionY() + spriteDescribe:getContentSize().height/2 + spriteDialog:getContentSize().height/2 +15)
       retLayer:addChild(spriteDialog)
       --上边的对话气泡
        local labelDialogR = SGCCLabelTTF:create("活动时间物品掉落翻物品掉落翻倍男倍男不男活动掉落翻倍男倍男不男女女" , FONT_PANGWA , 24,CCSizeMake(260,100),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop,ccc3(255,255,255),ccc3(0,0,0),2,true)
       labelDialogR:setPosition(labelDialogR:getContentSize().width*1.7+5,labelDialogR:getContentSize().height*2+35)
       spriteDialog:addChild(labelDialogR)
       labelDialogR:setTag(2)
       --下边的对话气泡
        local labelDialogL = SGCCLabelTTF:create("活动时间物品掉落翻物2掉落翻倍男倍男不男活动掉落翻倍男倍男不男女女" , FONT_PANGWA , 24,CCSizeMake(260,100),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop,ccc3(255,255,255),ccc3(0,0,0),2,true)
       labelDialogL:setPosition(labelDialogL:getContentSize().width*0.6,labelDialogL:getContentSize().height/2+10)
       spriteDialog:addChild(labelDialogL)
       labelDialogL:setTag(3)


		--该活动的背景图片
		local spriteBg = CCSprite:createWithSpriteFrameName("activityDoubleDrop.png")
		spriteBg:setAnchorPoint(ccp(0.5,0.5))
        spriteBg:setPosition( visibleSize.width * 0.5 ,spriteDialog:getPositionY() + spriteDialog:getContentSize().height * 0.5 + spriteBg:getContentSize().height * 0.5 +15)
		retLayer:addChild(spriteBg)

		--活动时间
		local labelActivityTime = SGCCLabelTTF:create("活动时间内所有闯" , FONT_PANGWA ,20)
		labelActivityTime:setPosition(spriteBg:getContentSize().width * 0.5 , 45)
		labelActivityTime:setTag(1)
		
		spriteBg:addChild(labelActivityTime)
		
        --设置返回layer的尺寸
        retLayer:setContentSize(CCSizeMake(visibleSize.width,spriteDescribe:getContentSize().height + spriteDialog:getContentSize().height + spriteBg:getContentSize().height + 50))

        --红色的背景
		local spriteRedBg = CCSprite:createWithSpriteFrameName("activitybg.png")
		local scaleX = 1.0 * visibleSize.width / 245
        local scaleY = 1.0 * (retLayer:getContentSize().height ) / 233
		spriteRedBg:setScaleX(scaleX)
		spriteRedBg:setScaleY(scaleY)
		spriteRedBg:setAnchorPoint(ccp(0.5,0.5 ))
        spriteRedBg:setPosition(retLayer:getContentSize().width * 0.5 , retLayer:getContentSize().height*0.5 )
        --spriteRedBg:setPosition(0,0)
		retLayer:addChild(spriteRedBg , -100 , -100)

        retLayer:setTouchEnabled(true)
		retLayer:setTag(baseTag)
		
		return retLayer
	end
	
	--添加到当前执行的scene中。
	local runningScene = CCDirector:sharedDirector():getRunningScene()
	runningScene:addChild(createRetLayer(1) , 1  ,654349)
end

xpcall ( main , __G__TRACKBACK__)

--******************************file end.************************************