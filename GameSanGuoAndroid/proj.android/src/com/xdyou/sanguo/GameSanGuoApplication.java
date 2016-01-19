package com.xdyou.sanguo;

import android.app.Application;
import android.os.Bundle;

import com.gmobi.trade.TradeService;
//import com.generalmobi.hero.tw.R;

public class GameSanGuoApplication extends Application 
{
	public void onCreate()
	{
		super.onCreate();
		
		//此处开始初始化Go2Play的SDK
		// appId与appKey同SDK方提供
		//正式
		TradeService.DEBUG = false;
		final String appId = "acb1e5702e0d464a93aa271e6ec9a20f";
		final String appKey = "032f72d81cda4db5b372422fcbd1c2ae";
		//测试
//		TradeService.DEBUG = true;
//		String appId = "4fcc86d0a5d911e3a2c1782bcb337be1";
//		String appKey = "570c303aa5d911e3a2c1782bcb337be1";
		
		//传给sdk的参数, 做为启动sdk的参数
		Bundle params = new Bundle();
		//置入参数值, 载入的title与信息
		params.putString(TradeService.PARAM_LOADING_TITLE, getResources().getString(R.string.app_name));
		params.putString(TradeService.PARAM_LOADING_MESSAGE, "loading...");
		TradeService.start(this, appId, appKey, params);
		
		System.out.println("go2play -- TradeService starting....");
		
	}
	
	static
	{
		// System.loadLibrary("sanguocppcommon");
		System.loadLibrary("sanguocpp");
	}
}
