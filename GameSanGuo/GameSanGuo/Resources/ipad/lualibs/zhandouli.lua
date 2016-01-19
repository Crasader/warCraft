require "lualibs/common.lua"

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
    ResourceManager:sharedInstance():bindTexture("sanguobigpic/zhandouli.plist", 1, 191)
    ResourceManager:sharedInstance():bindTexture("sanguobigpic/serviceopenbuttom.plist", 1, 191)

    local visibleSize = CCDirector:sharedDirector():getVisibleSize()
    local origin = CCDirector:sharedDirector():getVisibleOrigin()


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

      ------------------框7按钮逻辑
    local function createLayerButton7()
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

      ------------------框8按钮逻辑
    local function createLayerButton8()
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
				buttonBg:setColor(ccc3(255,255,255) )
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

      ------------------框9按钮逻辑
    local function createLayerButton9()
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

      ------------------框10按钮逻辑
    local function createLayerButton10()
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

      ------------------框11按钮逻辑
    local function createLayerButton11()
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

      ------------------框12按钮逻辑
    local function createLayerButton12()
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

      ------------------大按钮逻辑
    local function createLayerButton6()
        local LayerButton = CCLayer:create()

        -- add background
        local buttonBg = CCSprite:createWithSpriteFrameName("card_bj.png")
        buttonBg:setPosition(0 ,0)
        buttonBg:setScaleX(290/64)
        buttonBg:setScaleY(125/64)
        local boxSize = CCSizeMake(290/64*buttonBg:getContentSize().width, 125/64*buttonBg:getContentSize().height)
		buttonBg:setContentSize(boxSize)
        LayerButton:setContentSize(boxSize)

        LayerButton:addChild(buttonBg)
        LayerButton:setVisible(false)
		local rect = CCRect:new_local( -buttonBg:getContentSize().width/2 ,-buttonBg:getContentSize().height/2,buttonBg:getContentSize().width ,buttonBg:getContentSize().height)
		local pox=0
		local poy=0
		local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
        local beganPosY = 0
        local function onTouchBegan(x, y)
            -- CCTOUCHBEGAN event must return true
			if rect:containsPoint(LayerButton:convertToNodeSpace(ccp(x,y)))  and LayerButton:getParent():isVisible() then
				pox=x
				poy=y
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
			local dis=ccpDistance(ccp(pox,poy),ccp(x,y))
			if dis<10 and dis > -10 then
			local sender = SGNotificationCenter:sharedNotificationCenter()
			sender:postNotificationWithObj("showFirstChagreItem",LayerButton)

            buttonBg:setColor(ccc3(255,255,255))
			end
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
  

    local viewSize = CCDirector:sharedDirector():getVisibleSize()

------底部图
    local spriteqp = CCSprite:createWithSpriteFrameName("serviceopenbuttom.png")
    spriteqp:setPosition(ccp(viewSize.width/2,10+spriteqp:getContentSize().height/2))
    layerFarm:addChild(spriteqp)
-------底框字
    local labelinfo = CCLabelTTF:create("全民獎勵:活動截止時,綜合戰鬥力高于             的主公可獲得",FONT_BOXINFO,20)
    --labelinfo:setAnchorPoint(CCPoint(0,0.5))
    labelinfo:setPosition(ccpAdd(ccp(spriteqp:getPosition()),ccp(0,110)))
    layerFarm:addChild(labelinfo)

-----level
    local labellevel = CCLabelTTF:create("10000",FONT_BOXINFO,20)
    labellevel:setPosition(ccpAdd(ccp(labelinfo:getPosition()),ccp(104,0)))
    layerFarm:addChild(labellevel)
    labellevel:setTag(405)
    
-------中间图1
    local sprite3 = CCSprite:createWithSpriteFrameName("serviceopenline.png")
    sprite3:setPosition(ccpAdd(ccp(spriteqp:getPosition()),ccp(0,sprite3:getContentSize().height/2+spriteqp:getContentSize().height/2)))
    layerFarm:addChild(sprite3)
    
-------中间1字
    local labelinfo1 = CCLabelTTF:create("全民獎勵:活動截止時,綜合戰鬥力高于             的主公可獲得",FONT_BOXINFO,20)
    --labelinfo:setAnchorPoint(CCPoint(0,0.5))
    labelinfo1:setPosition(ccpAdd(ccp(sprite3:getPosition()),ccp(0,70)))
    layerFarm:addChild(labelinfo1)

-----level1
    local labellevel1 = CCLabelTTF:create("10000",FONT_BOXINFO,20)
    labellevel1:setPosition(ccpAdd(ccp(labelinfo1:getPosition()),ccp(104,0)))
    layerFarm:addChild(labellevel1)
    labellevel1:setTag(404)

    
-------中间图2 资源在firstactivityLayer中。
    local sprite4 = CCSprite:createWithSpriteFrameName("serviceopenline.png")
    sprite4:setPosition(ccpAdd(ccp(sprite3:getPosition()),ccp(0,sprite3:getContentSize().height/2+sprite4:getContentSize().height/2)))
    layerFarm:addChild(sprite4)
-------中间2字
    local labelinfo2 = CCLabelTTF:create("全民獎勵:活動截止時,綜合戰鬥力高于             的主公可獲得",FONT_BOXINFO,20)
    --labelinfo:setAnchorPoint(CCPoint(0,0.5))
    labelinfo2:setPosition(ccpAdd(ccp(sprite4:getPosition()),ccp(0,70)))
    layerFarm:addChild(labelinfo2)

-----level2
    local labellevel2 = CCLabelTTF:create("10000",FONT_BOXINFO,20)
    labellevel2:setPosition(ccpAdd(ccp(labelinfo2:getPosition()),ccp(104,0)))
    layerFarm:addChild(labellevel2)
    labellevel2:setTag(403)



-------顶图
    local sprite2 = CCSprite:createWithSpriteFrameName("zhandouli.png")
    sprite2:setPosition(ccpAdd(ccp(sprite4:getPosition()),ccp(4,sprite2:getContentSize().height/2+sprite4:getContentSize().height/2)))
    layerFarm:addChild(sprite2)
    

    
-----三个国家+天梯数
    local labelwei = CCLabelTTF:create("第1名:",FONT_BOXINFO,22)
    labelwei:setAnchorPoint(CCPoint(0,0.5))
    labelwei:setPosition(ccpAdd(ccp(sprite2:getPosition()),ccp(-70,-280)))
    layerFarm:addChild(labelwei)
    labelwei:setColor(ccc3(67,195,255))
    
    local labelshu = CCLabelTTF:create("第2名:",FONT_BOXINFO,22)
    labelshu:setAnchorPoint(CCPoint(0,0.5))
    labelshu:setPosition(ccpAdd(ccp(labelwei:getPosition()),ccp(0,-45)))
    layerFarm:addChild(labelshu)
    labelshu:setColor(ccc3(0,255,0))
    
    local labelwu = CCLabelTTF:create("第3名:",FONT_BOXINFO,22)
    labelwu:setAnchorPoint(CCPoint(0,0.5))
    labelwu:setPosition(ccpAdd(ccp(labelshu:getPosition()),ccp(0,-45)))
    layerFarm:addChild(labelwu)
    labelwu:setColor(ccc3(255,0,0))
    

    

-----------三个国家玩家名字+天梯数

    local playerwei = CCLabelTTF:create(" 暫無",FONT_BOXINFO,22)
    playerwei:setAnchorPoint(CCPoint(0,0.5))
    playerwei:setPosition(ccpAdd(ccp(labelwei:getPosition()),ccp(labelwei:getContentSize().width,0)))
    layerFarm:addChild(playerwei)
    playerwei:setColor(ccc3(67,195,255))
    playerwei:setTag(400)
    
    local playershu = CCLabelTTF:create(" 暫無",FONT_BOXINFO,22)
    playershu:setAnchorPoint(CCPoint(0,0.5))
    playershu:setPosition(ccpAdd(ccp(labelshu:getPosition()),ccp(labelshu:getContentSize().width,0)))
    layerFarm:addChild(playershu)
    playershu:setColor(ccc3(0,255,0))
    playershu:setTag(401)

   local playerwu = CCLabelTTF:create(" 暫無",FONT_BOXINFO,22)
    playerwu:setAnchorPoint(CCPoint(0,0.5))
    playerwu:setPosition(ccpAdd(ccp(labelwu:getPosition()),ccp(labelwu:getContentSize().width,0)))
    layerFarm:addChild(playerwu)
    playerwu:setColor(ccc3(255,0,0))
    playerwu:setTag(402)
    

-------------天\时\分\秒
    local day = CCLabelTTF:create("0",FONT_BOXINFO,30)
    day:setPosition(ccpAdd(ccp(sprite2:getPosition()),ccp(-55.5,233)))
    layerFarm:addChild(day)
    day:setColor(ccc3(0,255,0))
    day:setTag(103)
    
    local hour = CCLabelTTF:create("0",FONT_BOXINFO,30)
    hour:setPosition(ccpAdd(ccp(day:getPosition()),ccp(92,0)))
    layerFarm:addChild(hour)
    hour:setColor(ccc3(0,255,0))
    hour:setTag(102)
    
    local min = CCLabelTTF:create("0",FONT_BOXINFO,30)
    min:setPosition(ccpAdd(ccp(hour:getPosition()),ccp(111,1)))
    layerFarm:addChild(min)
    min:setColor(ccc3(0,255,0))
    min:setTag(101)
    
    local sec = CCLabelTTF:create("0",FONT_BOXINFO,30)
    sec:setPosition(ccpAdd(ccp(min:getPosition()),ccp(90,0.5)))
    layerFarm:addChild(sec)
    sec:setColor(ccc3(0,255,0))
    sec:setTag(100)



-------框里的三个按钮
    local btn2 =  createLayerButton2()
    btn2:setPosition(ccpAdd(ccp(spriteqp:getPosition()),ccp(0,40)))
    btn2:setTag(310)
    layerFarm:addChild(btn2)

    local btn1 =  createLayerButton1()
    btn1:setPosition(ccpAdd(ccp(btn2:getPosition()),ccp(-btn1:getContentSize().width * 1.2,0)))
    btn1:setTag(309)
    layerFarm:addChild(btn1)
    

    local btn3 =  createLayerButton3()
    btn3:setPosition(ccpAdd(ccp(btn2:getPosition()),ccp(btn3:getContentSize().width * 1.2,0)))
    btn3:setTag(311)
    layerFarm:addChild(btn3)
    
-------中间1框里的三个按钮
    local btn8 =  createLayerButton8()
    btn8:setPosition(ccpAdd(ccp(sprite3:getPosition()),ccp(0,0)))
    btn8:setTag(307)
    layerFarm:addChild(btn8)

    local btn7 =  createLayerButton7()
    btn7:setPosition(ccpAdd(ccp(btn8:getPosition()),ccp(-btn7:getContentSize().width * 1.2,0)))
    btn7:setTag(306)
    layerFarm:addChild(btn7)
    

    local btn9 =  createLayerButton9()
    btn9:setPosition(ccpAdd(ccp(btn8:getPosition()),ccp(btn9:getContentSize().width * 1.2,0)))
    btn9:setTag(308)
    layerFarm:addChild(btn9)
    
-------中间2框里的三个按钮
    local btn11 =  createLayerButton11()
    btn11:setPosition(ccpAdd(ccp(sprite4:getPosition()),ccp(0,0)))
    btn11:setTag(304)
    layerFarm:addChild(btn11)

    local btn10 =  createLayerButton10()
    btn10:setPosition(ccpAdd(ccp(btn11:getPosition()),ccp(-btn10:getContentSize().width * 1.2,0)))
    btn10:setTag(303)
    layerFarm:addChild(btn10)
    

    local btn12 =  createLayerButton12()
    btn12:setPosition(ccpAdd(ccp(btn11:getPosition()),ccp(btn12:getContentSize().width * 1.2,0)))
    btn12:setTag(305)
    layerFarm:addChild(btn12)


------------大按钮

    local btn6 =  createLayerButton6()
    btn6:setPosition(ccpAdd(ccp(sprite2:getPosition()),ccp(-6,-17)))
    btn6:setTag(300)
    layerFarm:addChild(btn6)

-----------第二三名
    local btn4 =  createLayerButton4()
    btn4:setPosition(ccpAdd(ccp(btn6:getPosition()),ccp(-185,-190)))
    btn4:setTag(301)
    layerFarm:addChild(btn4)
    
    local btn5 =  createLayerButton5()
    btn5:setPosition(ccpAdd(ccp(btn6:getPosition()),ccp(200,-190)))
    btn5:setTag(302)
    layerFarm:addChild(btn5)




    layerFarm:setContentSize(CCSizeMake(visibleSize.width, sprite2:getPositionY()+sprite2:getContentSize().height/2))
    return layerFarm
    end

    -- run
    local sceneGame = CCDirector:sharedDirector():getRunningScene()
    sceneGame:addChild(createLayerFarm(),1,654333)

end

xpcall(main, __G__TRACKBACK__)
