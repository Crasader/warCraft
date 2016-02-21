--[[
*成长计划活动
*by:zyc.
*19:20 2014/11/17
--]]

--
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
	
	ResourceManager:sharedInstance():bindTexture("sggeneralinfolayer/ofcNewUi.plist" , 1 , 191)
	ResourceManager:sharedInstance():bindTexture("sanguobigpic/activity_growplan.plist" , 1 ,191)
	--创建模板按钮 --- 约定 充值和购买的tag均小于1000
	local function createBtn(msg , bg ,text, fontSize ,tag)
		local ret = CCLayer:create()
        ret:setTag(tag)
        print ("button layer tag = "..ret:getTag())
        tag = tag + 1

		local bg = CCSprite:createWithSpriteFrameName(bg)
		bg:setAnchorPoint(ccp(0,0))
		bg:setPosition(0,0)			
		ret:addChild(bg)
		bg:setTag(tag)
        print ("tag = "..bg:getTag())
		tag = tag + 1

		--
		local size = bg:getContentSize()
		ret:setContentSize(size)
		
		--
		local label = SGCCLabelTTF:create(text ,FONT_PANGWA , fontSize , ccc3(255,255,255));
		label:setPosition(ccp(size.width * 0.5 , size.height * 0.5) )
		bg:addChild(label)
		label:setTag(tag)
		tag = tag + 1
		--
		local rect = CCRect:new_local(0,0 ,size.width , size.height)
		local isTouchMoved = false 
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
            if( false == ret:isVisible()) then
				return
            end
				local sender = SGNotificationCenter:sharedNotificationCenter()
				if 1000<= tag then
					sender:postNotificationWithObj(msg , ret)
				else
					sender:postNotification(msg)
				end
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
			
		--
		ret:registerScriptTouchHandler(onTouch)
		ret:setTouchEnabled(true)

		ret:ignoreAnchorPointForPosition(false)
		return ret
	end
	
	
	--创建模板框
	local function createTemplateLayer(tag)
		local ret=CCLayer:create()
		local size = CCSize:new_local(560 , 140)
		ret:setContentSize(size)
		
		--背景框
		local bg = CCScale9Sprite:createWithSpriteFrameName("barrack_kuang.png")
		bg:setPreferredSize(size)
		bg:setPosition(ccp(size.width*0.5 , size.height*0.5))
		ret:addChild(bg , -1, 10000+tag)

		--框背景
		local kuang_bg = CCScale9Sprite:createWithSpriteFrameName("barrack_kuangbg.png")
		kuang_bg:setPreferredSize(size)
		kuang_bg:setPosition(ccp(size.width*0.5 , size.height*0.5))
		ret:addChild(kuang_bg ,-2)

		
		--yuan bao.
		local spriteBg = CCSprite:createWithSpriteFrameName("card_bj.png")
		spriteBg:setPosition(ccp(70 , 74))
		bg:addChild(spriteBg)
		
		local border = CCSprite:createWithSpriteFrameName("equipPinzhi_4_0.png")
		border:setPosition(ccp(spriteBg:getContentSize().width*0.5 , spriteBg:getContentSize().height*0.5))
		spriteBg:addChild(border)
		
		local sprite = CCSprite:createWithSpriteFrameName("big_yb.png")
		sprite:setPosition(border:getPosition())
		spriteBg:addChild(sprite)
		
		--num
		local labelNum = SGCCLabelTTF:create("x650" , FONT_PANGWA , 20 , COLOR_GREEN)
		labelNum:setAnchorPoint(ccp(0.5 , 0))
		labelNum:setPosition(ccp(sprite:getPositionX() , -2))
		spriteBg:addChild(labelNum , 2)
		labelNum:setTag(tag)
		tag = tag + 1
		
		--des1 bg
		local des1Bg=CCScale9Sprite:createWithSpriteFrameName("ofc_100x100.png")
        des1Bg:setAnchorPoint(ccp(0 , 0.5))
		des1Bg:setPreferredSize(CCSize:new_local(240 , 40))
		des1Bg:setPosition(ccp(128 , 100))
		ret:addChild(des1Bg)
		
		--des1 
		local des1 = SGCCLabelTTF:create("50级成长基金" , FONT_PANGWA , 26  , COLOR_YELLOW)
		des1:setAnchorPoint(ccp(0 , 0.5))
		des1:setPosition(134 , des1Bg:getPositionY())
		ret:addChild(des1)
		des1:setTag(tag)
		tag = tag + 1
		
		--des2
		local des2 = SGCCLabelTTF:create("(达到50级即可领取650元宝)" , FONT_PANGWA , 22 , COLOR_GREEN)
		des2:setAnchorPoint(ccp( 0 , 0.5))
		des2:setPosition(ccp(130 ,40 ))
		ret:addChild(des2)
		des2:setTag(tag)
		tag = tag + 1
		
		--领取按钮
		local btn = createBtn("sendRewardReq" ,"store_exchangebtnbg.png" , "领取" , 26 ,tag)
		btn:setPosition(ccp(480 , 82))
		ret:addChild(btn)

		ret:ignoreAnchorPointForPosition(false)
		ret:setAnchorPoint(ccp(0.5 , 1))
		return ret
	end
	
	local function createRetLayer()
		local ret = CCLayer:create()
		local winSize = CCDirector:sharedDirector():getWinSize()
		ret:setContentSize(CCSize:new_local(winSize.width , 1800))
		
		local upBg = CCSprite:createWithSpriteFrameName("activity_growplan_upbg.png")
		upBg:setAnchorPoint(ccp(0.5 , 1))
		upBg:setPosition(ccp(winSize.width*0.5 , ret:getContentSize().height - 10))
		ret:addChild(upBg)
		
		--two button
		--btn1 
		local btn1 = createBtn("showStoreLayer" , "an.png" , "儲值" ,32 , 100)--tag no mean
		btn1:setAnchorPoint(ccp(1 , 1))
		btn1:setPosition(ccp(upBg:getContentSize().width*0.5 - 35 , -5))
		upBg:addChild(btn1)
		
		--btn2
		local btn2 = createBtn("buyGrowPlan" , "an.png" , "購買" ,32 , 200)--tag no mean
		btn2:setAnchorPoint(ccp(0 , 1))
		btn2:setPosition(ccp(upBg:getContentSize().width*0.5 - 5 , -5))
		upBg:addChild(btn2)
		
		--labelNum
		local labelNum = SGCCLabelTTF:create("20000" , FONT_PANGWA , 24 , COLOR_YELLOW)
		labelNum:setAnchorPoint(ccp(0,0))
		labelNum:setPosition(ccp(btn2:getPositionX() + btn2:getContentSize().width + 5 , btn2:getPositionY() - btn2:getContentSize().height))
		upBg:addChild(labelNum)
		labelNum:setTag(1)-----！！！！
		
		--yb pic
		local goldSprite = CCSprite:createWithSpriteFrameName("goldicon.png")
		goldSprite:setAnchorPoint(labelNum:getAnchorPoint())
		goldSprite:setPosition(ccp(btn2:getPositionX() + btn2:getContentSize().width + 20 + labelNum:getContentSize().width , btn2:getPositionY() - btn2:getContentSize().height + 18 ))
		upBg:addChild(goldSprite)
			
		--stick
        local stickPosY = ret:getContentSize().height - 360
        local stickM = CCSprite:createWithSpriteFrameName("activity_growplan_stick.png")
        stickM:setPosition(ccp(winSize.width*0.5 , stickPosY))
        ret:addChild(stickM , 10)
        --[[
        local stickL = CCSprite:createWithSpriteFrameName("title_bg_LR.png")
        stickL:setFlipY(true);
        stickL:setAnchorPoint(ccp(1 , 0.5))
        stickL:setPosition(ccp(stickM:getPositionX() - stickM:getContentSize().width*0.5*stickM:getScaleX() , stickPosY + 0.5))
		ret:addChild(stickL , 10)

        local stickR = CCSprite:createWithSpriteFrameName("title_bg_LR.png")
        stickR:setFlipY(true)
        stickR:setFlipX(true)
        stickR:setAnchorPoint(ccp(0 , 0.5))
        stickR:setPosition(ccp(stickM:getPositionX() + stickM:getContentSize().width*0.5*stickM:getScaleX() , stickPosY -0.5))
        ret:addChild(stickR , 10)
        --]]

		--容器
		local containerLayer = CCLayer:create()
		containerLayer:ignoreAnchorPointForPosition(false)
		containerLayer:setAnchorPoint(ccp(0.5 , 1))
		containerLayer:setContentSize(CCSize:new_local(584 , 629*3))
		containerLayer:setPosition(ccp(winSize.width*0.5, stickPosY + 15) )
		ret:addChild(containerLayer)

		--光圈 ， 4条线
        --横线
		for i=0 , 1 do
			local line = CCSprite:createWithSpriteFrameName("activity_growplan_line.png")
			line:setScaleY(containerLayer:getContentSize().width/line:getContentSize().height)
			line:setAnchorPoint(ccp(0.5 , 0.5))
            line:setRotation(90)
			line:setPosition(ccp(containerLayer:getContentSize().width*0.5 , i*(containerLayer:getContentSize().height + 2)-2 ))
			containerLayer:addChild(line)
		end

		--竖线
		for i =0 , 1 do
			local line = CCSprite:createWithSpriteFrameName("activity_growplan_line.png")
			line:setScaleY(containerLayer:getContentSize().height/line:getContentSize().height)
			line:setAnchorPoint(ccp(0.5 , 0.5))
			line:setPosition(ccp(containerLayer:getContentSize().width*i , containerLayer:getContentSize().height *0.5))
			containerLayer:addChild(line)
		end


		--3张背景图
		for i =0 , 2 do
			local bg = CCSprite:createWithSpriteFrameName("activity_growplan_downbg.png")
			bg:setAnchorPoint(ccp(0.5 , 1))
			bg:setFlipY(i==1)
			bg:setPosition(ccp(containerLayer:getContentSize().width*0.5 , containerLayer:getContentSize().height - i*bg:getContentSize().height))
			containerLayer:addChild(bg)
		end
		
		--nine template
		for i=0 , 8  do
			local layer = createTemplateLayer(1000*(i+1) + 1)
            layer:setPosition(containerLayer:getContentSize().width*0.5 , containerLayer:getContentSize().height -  i* 160 - 30)
			containerLayer:addChild(layer  , 10, i + 10)
		end
		
		--ret:ignoreAnchorPointForPosition(false)
		--ret:setAnchorPoint(ccp(0,1))
		--ret:setPosition(ccp(0 , winSize.height ))
		return ret
	end
	
	--
	local runningScene = CCDirector:sharedDirector():getRunningScene()
	runningScene:addChild(createRetLayer(1) , 1  ,654348)
end



xpcall ( main , __G__TRACKBACK__)