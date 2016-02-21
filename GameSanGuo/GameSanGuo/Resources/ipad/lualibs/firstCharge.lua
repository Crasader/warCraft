
	function __G__TRACKBACK__(msg)
		print("----------------------------------------")  
		print("LUA ERROR: " .. tostring(msg) .. "\n")  
		print(debug.traceback())  
		print("----------------------------------------")  
	end  

	local function main()
		-- avoid memory leak
		collectgarbage("setpause", 100)
		collectgarbage("setstepmul", 5000)
		
		---------------
		--SAMPLE: lua binds texture, '1' means layerUI, '66' means sg_activitylayer.
		ResourceManager:sharedInstance():bindTexture("sanguobigpic/firstCharge.plist", 1, 191)

		local visibleSize = CCDirector:sharedDirector():getVisibleSize()
		local origin = CCDirector:sharedDirector():getVisibleOrigin()

		   ------------------储值按钮逻辑
		-- create LayerButton
		local function createLayerButton()
			local LayerButton = CCLayer:create()

			-- add background
			local buttonBg = CCSprite:createWithSpriteFrameName("an.png")
			buttonBg:setPosition(buttonBg:getContentSize().width/2 , buttonBg:getContentSize().height/2)
			LayerButton:setContentSize(buttonBg:getContentSize())
			LayerButton:addChild(buttonBg)
            --LayerButton:setEnable(false)
            --LayerButton:setVisible(false)
			local rect = CCRect:new_local( 0 ,0,buttonBg:getContentSize().width ,buttonBg:getContentSize().height)
			local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
			local beganPosY = 0
			local function onTouchBegan(x, y)
				if rect:containsPoint(LayerButton:convertToNodeSpace(ccp(x,y))) and LayerButton:getParent():isVisible() then
					buttonBg:setColor(ccc3(160,160,160))
					isTouchMoved = false
					beganPosY = y
					return true
				else
					return false
				end
			end

			local function onTouchMoved(x, y)
				local deltaPoxY  = math.abs(y-beganPosY)
				    print ("*********** ** * * dela posY * ** * ** * **"..deltaPoxY)
				    if deltaPoxY >= 20 then 
				        isTouchMoved = true
				    end
			end

			local function onTouchEnded(x, y)
				local sender = SGNotificationCenter:sharedNotificationCenter()
				sender:postNotification("showStoreLayer")
				--local lua = SGLuaSocketSender:shareLuaSender()
				--lua:sendLuaBoxMsg(654321,1)
				buttonBg:setColor(ccc3(255,255,255))
			end

			local function onTouch(eventType, x, y)
				if eventType == "began" then   
					return onTouchBegan(x, y)
				elseif eventType == "moved" then
					return onTouchMoved(x, y)
				elseif "ended" == eventType then
                    buttonBg:setColor(ccc3(255,255,255))
					if not isTouchMoved then
						return onTouchEnded(x, y)
					end
				end
			end

			LayerButton:registerScriptTouchHandler(onTouch)
			LayerButton:setTouchEnabled(true)

			return LayerButton
		end

	-----------------

		  ------------------框1按钮逻辑
		local function createLayerButton1()
			local LayerButton = CCLayer:create()

			-- add background
			local buttonBg = CCSprite:createWithSpriteFrameName("card_bj.png")
			--buttonBg:setPosition(0 ,0)
			LayerButton:setContentSize(buttonBg:getContentSize())
			LayerButton:addChild(buttonBg)
			local rect = CCRect:new_local( -buttonBg:getContentSize().width/2 ,-buttonBg:getContentSize().height/2,buttonBg:getContentSize().width ,buttonBg:getContentSize().height)
			local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
			local beganPosY = 0
			local function onTouchBegan(x, y)
				-- CCTOUCHBEGAN event must return true
				if rect:containsPoint(LayerButton:convertToNodeSpace(ccp(x,y)))  and LayerButton:getParent():isVisible() then
					buttonBg:setColor(ccc3(160,160,160))
					isTouchMoved = false
					beganPosY = y
					return true
				else
					return false
				end
			end

			local function onTouchMoved(x, y)
				--cclog("onTouchMoved: %0.2f, %0.2f", x, y)
				local deltaPoxY  = math.abs(y-beganPosY)
				    print ("*********** ** * * dela posY * ** * ** * **"..deltaPoxY)
				    if deltaPoxY >= 20 then 
				        isTouchMoved = true
				    end
			end

			local function onTouchEnded(x, y)
				--cclog("onTouchEnded: %0.2f, %0.2f", x, y)
				local sender = SGNotificationCenter:sharedNotificationCenter()

				--print ("sender : " .. sender .. "\n")
				sender:postNotificationWithObj("showFirstChagreItem",LayerButton)
				buttonBg:setColor(ccc3(255,255,255))
			end

			local function onTouch(eventType, x, y)
				if eventType == "began" then   
					return onTouchBegan(x, y)
				elseif eventType == "moved" then
					return onTouchMoved(x, y)
				elseif "ended" == eventType then
                    buttonBg:setColor(ccc3(255,255,255))
					if not isTouchMoved then
						return onTouchEnded(x, y)
					end
				end
			end

			LayerButton:registerScriptTouchHandler(onTouch)
			LayerButton:setTouchEnabled(true)

			return LayerButton
		end

	-----------------
	-----------------

		  ------------------框2按钮逻辑
		local function createLayerButton2()
			local LayerButton = CCLayer:create()

			-- add background
			local buttonBg = CCSprite:createWithSpriteFrameName("card_bj.png")
			buttonBg:setPosition(0 ,0)
			LayerButton:setContentSize(buttonBg:getContentSize())
			LayerButton:addChild(buttonBg)
			local rect = CCRect:new_local( -buttonBg:getContentSize().width/2 ,-buttonBg:getContentSize().height/2,buttonBg:getContentSize().width ,buttonBg:getContentSize().height)
			local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
			local beganPosY  = 0
			local function onTouchBegan(x, y)
				-- CCTOUCHBEGAN event must return true
				if rect:containsPoint(LayerButton:convertToNodeSpace(ccp(x,y)))  and LayerButton:getParent():isVisible() then
					buttonBg:setColor(ccc3(160,160,160))
					isTouchMoved = false
					beganPosY  = y
					return true
				else
					return false
				end
			end

			local function onTouchMoved(x, y)
				--cclog("onTouchMoved: %0.2f, %0.2f", x, y)
				local deltaPoxY  = math.abs(y-beganPosY)
				    print ("*********** ** * * dela posY * ** * ** * **"..deltaPoxY)
				    if deltaPoxY >= 20 then 
				        isTouchMoved = true
				    end
			end

			local function onTouchEnded(x, y)
				--cclog("onTouchEnded: %0.2f, %0.2f", x, y)
				local sender = SGNotificationCenter:sharedNotificationCenter()
				sender:postNotificationWithObj("showFirstChagreItem",LayerButton)
				buttonBg:setColor(ccc3(255,255,255))
			end

			local function onTouch(eventType, x, y)
				if eventType == "began" then   
					return onTouchBegan(x, y)
				elseif eventType == "moved" then
					return onTouchMoved(x, y)
				elseif "ended" == eventType then
                    buttonBg:setColor(ccc3(255,255,255))
					if not isTouchMoved then
						return onTouchEnded(x, y)
					end
				end
			end

			LayerButton:registerScriptTouchHandler(onTouch)
			LayerButton:setTouchEnabled(true)

			return LayerButton
		end

	-----------------

	-----------------

		  ------------------框3按钮逻辑
		local function createLayerButton3()
			local LayerButton = CCLayer:create()

			-- add background
			local buttonBg = CCSprite:createWithSpriteFrameName("card_bj.png")
			buttonBg:setPosition(0 ,0)
			LayerButton:setContentSize(buttonBg:getContentSize())
			LayerButton:addChild(buttonBg)
			local rect = CCRect:new_local( -buttonBg:getContentSize().width/2 ,-buttonBg:getContentSize().height/2,buttonBg:getContentSize().width ,buttonBg:getContentSize().height)

			local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
			local beganPosY  = 0 
			local function onTouchBegan(x, y)
				-- CCTOUCHBEGAN event must return true
				if rect:containsPoint(LayerButton:convertToNodeSpace(ccp(x,y)))  and LayerButton:getParent():isVisible() then
					buttonBg:setColor(ccc3(160,160,160))
					isTouchMoved = false
					beganPosY = y
					return true
				else
					return false
				end
			end

			local function onTouchMoved(x, y)
				--cclog("onTouchMoved: %0.2f, %0.2f", x, y)
				local deltaPoxY  = math.abs(y-beganPosY)
				    print ("*********** ** * * dela posY * ** * ** * **"..deltaPoxY)
				    if deltaPoxY >= 20 then 
				        isTouchMoved = true
				    end
			end

			local function onTouchEnded(x, y)
				--cclog("onTouchEnded: %0.2f, %0.2f", x, y)
				local sender = SGNotificationCenter:sharedNotificationCenter()
				sender:postNotificationWithObj("showFirstChagreItem",LayerButton)
				buttonBg:setColor(ccc3(255,255,255))
			end

			local function onTouch(eventType, x, y)
				if eventType == "began" then   
					return onTouchBegan(x, y)
				elseif eventType == "moved" then
					return onTouchMoved(x, y)
				elseif "ended" == eventType then
                    buttonBg:setColor(ccc3(255,255,255))
					if not isTouchMoved then
						return onTouchEnded(x, y)
					end
				end
			end

			LayerButton:registerScriptTouchHandler(onTouch)
			LayerButton:setTouchEnabled(true)

			return LayerButton
		end

	-----------------

	-----------------

		  ------------------框4按钮逻辑
		local function createLayerButton4()
			local LayerButton = CCLayer:create()

			-- add background
			local buttonBg = CCSprite:createWithSpriteFrameName("card_bj.png")
			buttonBg:setPosition(0 ,0)
			LayerButton:setContentSize(buttonBg:getContentSize())
			LayerButton:addChild(buttonBg)
			local rect = CCRect:new_local( -buttonBg:getContentSize().width/2 ,-buttonBg:getContentSize().height/2,buttonBg:getContentSize().width ,buttonBg:getContentSize().height)

			local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
			local beganPosY = 0
			local function onTouchBegan(x, y)
				-- CCTOUCHBEGAN event must return true
				if rect:containsPoint(LayerButton:convertToNodeSpace(ccp(x,y)))  and LayerButton:getParent():isVisible() then
					buttonBg:setColor(ccc3(160,160,160))
					isTouchMoved = false
					beganPosY = y
					return true
				else
					return false
				end
			end

			local function onTouchMoved(x, y)
				--cclog("onTouchMoved: %0.2f, %0.2f", x, y)
				local deltaPoxY  = math.abs(y-beganPosY)
				    print ("*********** ** * * dela posY * ** * ** * **"..deltaPoxY)
				    if deltaPoxY >= 20 then 
				        isTouchMoved = true
				    end
			end

			local function onTouchEnded(x, y)
				--cclog("onTouchEnded: %0.2f, %0.2f", x, y)
				local sender = SGNotificationCenter:sharedNotificationCenter()
				sender:postNotificationWithObj("showFirstChagreItem",LayerButton)
				buttonBg:setColor(ccc3(255,255,255))
			end

			local function onTouch(eventType, x, y)
				if eventType == "began" then   
					return onTouchBegan(x, y)
				elseif eventType == "moved" then
					return onTouchMoved(x, y)
				elseif "ended" == eventType then
                    buttonBg:setColor(ccc3(255,255,255))
					if not isTouchMoved then
						return onTouchEnded(x, y)
					end
				end
			end

			LayerButton:registerScriptTouchHandler(onTouch)
			LayerButton:setTouchEnabled(true)

			return LayerButton
		end

	-----------------

	-----------------

		  ------------------框5按钮逻辑
		local function createLayerButton5()
			local LayerButton = CCLayer:create()

			-- add background
			local buttonBg = CCSprite:createWithSpriteFrameName("card_bj.png")
			buttonBg:setPosition(0 ,0)
			LayerButton:setContentSize(buttonBg:getContentSize())
			LayerButton:addChild(buttonBg)
			local rect = CCRect:new_local( -buttonBg:getContentSize().width/2 ,-buttonBg:getContentSize().height/2,buttonBg:getContentSize().width ,buttonBg:getContentSize().height)

			local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
			local beganPosY = 0
			local function onTouchBegan(x, y)
				-- CCTOUCHBEGAN event must return true
			   if rect:containsPoint(LayerButton:convertToNodeSpace(ccp(x,y)))  and LayerButton:getParent():isVisible() then
					buttonBg:setColor(ccc3(160,160,160))
					isTouchMoved = false
					beganPosY = y
					return true
			   else
					return false
			   end
			end

			local function onTouchMoved(x, y)
				local deltaPoxY  = math.abs(y-beganPosY)
				    print ("*********** ** * * dela posY * ** * ** * **"..deltaPoxY)
				    if deltaPoxY >= 20 then 
				        isTouchMoved = true
				    end
				--cclog("onTouchMoved: %0.2f, %0.2f", x, y)
			end

			local function onTouchEnded(x, y)
				--cclog("onTouchEnded: %0.2f, %0.2f", x, y)
				local sender = SGNotificationCenter:sharedNotificationCenter()
				sender:postNotificationWithObj("showFirstChagreItem",LayerButton)
				buttonBg:setColor(ccc3(255,255,255))
			end

			local function onTouch(eventType, x, y)
				if eventType == "began" then   
					return onTouchBegan(x, y)
				elseif eventType == "moved" then
					return onTouchMoved(x, y)
				elseif "ended" == eventType then
                    buttonBg:setColor(ccc3(255,255,255))
					if not isTouchMoved then
						return onTouchEnded(x, y)
					end
				end
			end

			LayerButton:registerScriptTouchHandler(onTouch)
			LayerButton:setTouchEnabled(true)

			return LayerButton
		end

	-----------------

		-- create farm
		local function createLayerFarm()
		local layerFarm = CCLayer:create()
	  
	--气泡
		local viewSize = CCDirector:sharedDirector():getVisibleSize()
		local scaleRate = viewSize.height/960
		local scaleRate2 = viewSize.width/640
		local spriteqp = CCSprite:createWithSpriteFrameName("firstChargeBg.png")
		spriteqp:setAnchorPoint(CCPoint(0,0))
		spriteqp:setPosition(ccp(0,0))
		spriteqp:setScaleX(scaleRate2)
		layerFarm:addChild(spriteqp)

		local visibleSize = spriteqp:getContentSize()


		

		------------储值按钮
		local buttonchong = createLayerButton()
		buttonchong:setPosition(ccp(visibleSize.width*0.64* scaleRate2,384))
		layerFarm:addChild(buttonchong)

		local labelname = CCSprite:createWithSpriteFrameName("firstChargeLabel.png")
		labelname:setAnchorPoint(CCPoint(0.5,0.5))
		labelname:setPosition(ccpAdd(ccp(buttonchong:getPosition()),ccp(buttonchong:getContentSize().width/2,buttonchong:getContentSize().height/2)))
		layerFarm:addChild(labelname)

	--------光效
		   local animation = CCAnimation:create()
		   local resSprite
		   local number, name
		   for i = 1, 14 do
				if i < 10 then
					number = "0"..i
				else
					number = i
				end
				name = "short_000"..number..".png"
				resSprite = CCSprite:createWithSpriteFrameName(name)
				animation:addSpriteFrame(resSprite:displayFrame())
			end
			-- should last 2.8 seconds. And there are 14 frames.
			animation:setDelayPerUnit(2.8 / 15.0)
			animation:setRestoreOriginalFrame(true)
	--------

		local btn1 =  createLayerButton1()
		btn1:setPosition(ccp(visibleSize.width*0.15* scaleRate2,200))
		btn1:setTag(300)
		btn1:setTouchEnabled(true)
		layerFarm:addChild(btn1)

		--local action = CCAnimate:create(animation)
		--resSprite:setPosition(ccp(btn1:getPosition()))
		--resSprite:runAction(CCRepeatForever:create(action))
		--layerFarm:addChild(resSprite)

		local btn2 =  createLayerButton2()
		btn2:setPosition(ccpAdd(ccp(btn1:getPosition()),ccp(btn2:getContentSize().width * 1.2* scaleRate2,0)))
		btn2:setTag(301)
		btn2:setTouchEnabled(true)
		layerFarm:addChild(btn2)
		
		--action = CCAnimate:create(animation)
		--resSprite = CCSprite:createWithSpriteFrameName(name)
		--resSprite:runAction(CCRepeatForever:create(action))
		--resSprite:setPosition(ccp(btn2:getPosition()))
		--layerFarm:addChild(resSprite)

		local btn3 =  createLayerButton3()
		btn3:setPosition(ccpAdd(ccp(btn2:getPosition()),ccp(btn3:getContentSize().width * 1.2* scaleRate2,0)))
		btn3:setTag(302)
		btn3:setTouchEnabled(true)
		layerFarm:addChild(btn3)

		local action = CCAnimate:create(animation)
		resSprite = CCSprite:createWithSpriteFrameName(name)
		resSprite:runAction(CCRepeatForever:create(action))
		resSprite:setPosition(ccp(btn3:getPosition()))
		layerFarm:addChild(resSprite)

		local btn4 =  createLayerButton4()
		btn4:setPosition(ccpAdd(ccp(btn3:getPosition()),ccp(btn4:getContentSize().width * 1.2* scaleRate2,0)))
		btn4:setTag(303)
		btn4:setTouchEnabled(true)
		layerFarm:addChild(btn4)

		--action = CCAnimate:create(animation)
		--resSprite = CCSprite:createWithSpriteFrameName(name)
		--resSprite:runAction(CCRepeatForever:create(action))
		--resSprite:setPosition(ccp(btn4:getPosition()))
		--layerFarm:addChild(resSprite)


		local btn5 =  createLayerButton5()
		btn5:setPosition(ccpAdd(ccp(btn4:getPosition()),ccp(btn5:getContentSize().width * 1.2* scaleRate2,0)))
		btn5:setTag(304)
		btn5:setTouchEnabled(true)
		layerFarm:addChild(btn5)

		--action = CCAnimate:create(animation)
		--resSprite = CCSprite:createWithSpriteFrameName(name)
		--resSprite:runAction(CCRepeatForever:create(action))
		--resSprite:setPosition(ccp(btn5:getPosition()))
		--layerFarm:addChild(resSprite)


		layerFarm:setContentSize(CCSizeMake(viewSize.width,spriteqp:getContentSize().height))
		return layerFarm
		end

		-- run
		local sceneGame = CCDirector:sharedDirector():getRunningScene()
		sceneGame:addChild(createLayerFarm(),1,654329)

		print (" excute ");
	end



	xpcall(main, __G__TRACKBACK__)
