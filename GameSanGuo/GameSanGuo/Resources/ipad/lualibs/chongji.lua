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
    ResourceManager:sharedInstance():bindTexture("sanguobigpic/serviceopenbuttom.plist", 1, 191)
    ResourceManager:sharedInstance():bindTexture("sanguobigpic/chongji.plist", 1, 191)
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
        local  beganPosY = 0
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
        local  beganPosY = 0
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

      ------------------大按钮逻辑
    local function createLayerButton4()
        local LayerButton = CCLayer:create()

        -- add background
        local buttonBg = CCSprite:createWithSpriteFrameName("card_bj.png")
        buttonBg:setPosition(0 ,0)
        buttonBg:setScaleX(320/64)
        buttonBg:setScaleY(130/64)
   local boxSize = CCSizeMake(320/64*buttonBg:getContentSize().width, 130/64*buttonBg:getContentSize().height)
   buttonBg:setContentSize(boxSize)
        LayerButton:setContentSize(boxSize)
        LayerButton:addChild(buttonBg)
        LayerButton:setVisible(false)
       local rect = CCRect:new_local( -buttonBg:getContentSize().width/2 ,-buttonBg:getContentSize().height/2,buttonBg:getContentSize().width ,buttonBg:getContentSize().height)
	   local isTouchMoved = false --如果本次包含滑动事件，则不处理touchend消息
       local beganPosY =  0
   local pox=0
   local poy=0

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

-------顶图
    local sprite2 = CCSprite:createWithSpriteFrameName("chongji.png")
    sprite2:setPosition(ccpAdd(ccp(spriteqp:getPosition()),ccp(-6,sprite2:getContentSize().height/2+spriteqp:getContentSize().height/2)))
    layerFarm:addChild(sprite2)
    
-------底框字
    local labelinfo = CCLabelTTF:create("全民獎勵:活動截止時,高于或等于           級的主公可獲得",FONT_BOXINFO,20)
    --labelinfo:setAnchorPoint(CCPoint(0,0.5))
    labelinfo:setPosition(ccpAdd(ccp(spriteqp:getPosition()),ccp(0,110)))
    layerFarm:addChild(labelinfo)

-----level
    local labellevel = CCLabelTTF:create("1000",FONT_BOXINFO,20)
    labellevel:setPosition(ccpAdd(ccp(labelinfo:getPosition()),ccp(75,0)))
    layerFarm:addChild(labellevel)
    labellevel:setTag(403)
    
-----三个国家
    local labelwei = CCLabelTTF:create("魏國:",FONT_BOXINFO,25)
    labelwei:setAnchorPoint(CCPoint(0,0.5))
    labelwei:setPosition(ccpAdd(ccp(sprite2:getPosition()),ccp(-60,-210)))
    layerFarm:addChild(labelwei)
    labelwei:setColor(ccc3(67,195,255))
    
    local labelshu = CCLabelTTF:create("蜀國:",FONT_BOXINFO,25)
    labelshu:setAnchorPoint(CCPoint(0,0.5))
    labelshu:setPosition(ccpAdd(ccp(labelwei:getPosition()),ccp(0,-35)))
    layerFarm:addChild(labelshu)
    labelshu:setColor(ccc3(0,255,0))
    
    local labelwu = CCLabelTTF:create("吴國:",FONT_BOXINFO,25)
    labelwu:setAnchorPoint(CCPoint(0,0.5))
    labelwu:setPosition(ccpAdd(ccp(labelshu:getPosition()),ccp(0,-35)))
    layerFarm:addChild(labelwu)
    labelwu:setColor(ccc3(255,0,0))
    

-----------三个国家玩家名字

    local playerwei = CCLabelTTF:create(" 暫無",FONT_BOXINFO,25)
    playerwei:setAnchorPoint(CCPoint(0,0.5))
    playerwei:setPosition(ccpAdd(ccp(labelwei:getPosition()),ccp(labelwei:getContentSize().width,0)))
    layerFarm:addChild(playerwei)
    playerwei:setColor(ccc3(67,195,255))
    playerwei:setTag(400)
    
    local playershu = CCLabelTTF:create(" 暫無",FONT_BOXINFO,25)
    playershu:setAnchorPoint(CCPoint(0,0.5))
    playershu:setPosition(ccpAdd(ccp(labelshu:getPosition()),ccp(labelshu:getContentSize().width,0)))
    layerFarm:addChild(playershu)
    playershu:setColor(ccc3(0,255,0))
    playershu:setTag(401)

   local playerwu = CCLabelTTF:create(" 暫無",FONT_BOXINFO,25)
    playerwu:setAnchorPoint(CCPoint(0,0.5))
    playerwu:setPosition(ccpAdd(ccp(labelwu:getPosition()),ccp(labelwu:getContentSize().width,0)))
    layerFarm:addChild(playerwu)
    playerwu:setColor(ccc3(255,0,0))
    playerwu:setTag(402)

-------------天\时\分\秒
    local day = CCLabelTTF:create("0",FONT_BOXINFO,30)
    day:setPosition(ccpAdd(ccp(sprite2:getPosition()),ccp(-48,124)))
    layerFarm:addChild(day)
    day:setColor(ccc3(0,255,0))
    day:setTag(103)
    
    local hour = CCLabelTTF:create("0",FONT_BOXINFO,30)
    hour:setPosition(ccpAdd(ccp(day:getPosition()),ccp(90,0)))
    layerFarm:addChild(hour)
    hour:setColor(ccc3(0,255,0))
    hour:setTag(102)
    
    local min = CCLabelTTF:create("0",FONT_BOXINFO,30)
    min:setPosition(ccpAdd(ccp(hour:getPosition()),ccp(110,1)))
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
    btn2:setTag(302)
    layerFarm:addChild(btn2)

    local btn1 =  createLayerButton1()
    btn1:setPosition(ccpAdd(ccp(btn2:getPosition()),ccp(-btn1:getContentSize().width * 1.2,0)))
    btn1:setTag(301)
    layerFarm:addChild(btn1)
    

    local btn3 =  createLayerButton3()
    btn3:setPosition(ccpAdd(ccp(btn2:getPosition()),ccp(btn3:getContentSize().width * 1.2,0)))
    btn3:setTag(303)
    layerFarm:addChild(btn3)

------------大按钮

    local btn4 =  createLayerButton4()
    btn4:setPosition(ccpAdd(ccp(sprite2:getPosition()),ccp(6,-75)))
    btn4:setTag(300)
    layerFarm:addChild(btn4)







    layerFarm:setContentSize(CCSizeMake(visibleSize.width, sprite2:getPositionY()+sprite2:getContentSize().height/2))
    return layerFarm
    end

    -- run
    local sceneGame = CCDirector:sharedDirector():getRunningScene()
    sceneGame:addChild(createLayerFarm(),1,654330)

end

xpcall(main, __G__TRACKBACK__)
