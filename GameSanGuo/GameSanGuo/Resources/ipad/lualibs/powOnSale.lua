--[[
*购买体力打折 活动  pow
*by:zyc
*time:15:29 2014/12/11
*weather:sunny
--]]

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
	
	--
	ResourceManager:sharedInstance():bindTexture("sanguobigpic/activity_pow.plist" , 1 , 191)
    --ResourceManager:sharedInstance():bindTexture("sgfirstactivitylayer/sgfirstactivitylayer.plist" , 1 , 191)

	
	
	local function createReturnLayer()
		local retLayer = CCLayer:create()
		
		local winSize = CCDirector:sharedDirector():getWinSize()
		retLayer:setContentSize(CCSizeMake(winSize.width , winSize.height-350));
		
		--红色背景
		local redBg = CCScale9Sprite:createWithSpriteFrameName("activitybg.png")
        redBg:setPreferredSize(retLayer:getContentSize())
		redBg:setPosition(ccp(winSize.width*0.5 , retLayer:getContentSize().height*0.5))
		retLayer:addChild(redBg , -1);
        print("redbg tag  "..redBg:getTag())
		--本次活动的背景
		local upbg = CCSprite:createWithSpriteFrameName("activity_pow_sale.png")
        upbg:setPosition(redBg:getPositionX() , retLayer:getContentSize().height - upbg:getContentSize().height*0.5 - 20)
		retLayer:addChild(upbg);
        print("bg tag  "..upbg:getTag())
		
		--label 1
		local activityDuration = SGCCLabelTTF:create("活動時間：開服后第8天到第14天",FONT_PANGWA , 20)
        activityDuration:setPosition(ccp(upbg:getContentSize().width*0.5 , 60))
		upbg:addChild(activityDuration);
		activityDuration:setTag(100);
        print("time tag  "..activityDuration:getTag())
		--label 2
		local activityDes = SGCCLabelTTF:create("活動期間內，每次購買體力享受7折優惠。千萬不要錯過哦！",FONT_PANGWA , 22)
        --activityDes:setAnchorPoint(ccp(0 , 0.5))
        activityDes:setPosition(ccp(upbg:getContentSize().width*0.5 , 30))
		upbg:addChild(activityDes)
        activityDes:setTag(200)
        print("des tag  "..activityDes:getTag())

        return retLayer
	
	end
	
	--
	local runningScene = CCDirector:sharedDirector():getRunningScene()
	runningScene:addChild(createReturnLayer() , 1  ,654351)
end

xpcall ( main , __G__TRACKBACK__)




----file end.