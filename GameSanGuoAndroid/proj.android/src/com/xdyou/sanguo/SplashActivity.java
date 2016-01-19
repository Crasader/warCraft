package com.xdyou.sanguo;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnTouchListener;
import android.widget.RelativeLayout;
import android.widget.VideoView;

//import com.generalmobi.hero.tw.R;

//public class SplashActivity extends Activity
//{
//	@Override
//	public void onCreate(Bundle savedInstanceState)
//	{
//		requestWindowFeature(Window.FEATURE_NO_TITLE);
//		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
//				WindowManager.LayoutParams.FLAG_FULLSCREEN);
//		super.onCreate(savedInstanceState);
//		setContentView(R.layout.splashscreen);
//
//		//倒计时
//		new CountDownTimer(2000, 1000)
//		{
//			@Override
//			public void onTick(long millisUntilFinished)
//			{
//			}
//
//			@Override
//			public void onFinish()
//			{
//				Intent intent = new Intent();
//				//启动画面在先，之后是播放动画
//				intent.setClass(SplashActivity.this, BloodScreen.class);
//				startActivity(intent);
//
//				//运行时获取android sdk的版本
//				int VERSION = android.os.Build.VERSION.SDK_INT;
//				if (VERSION >= 5)
//				{
//					//淡入淡出效果
//					SplashActivity.this.overridePendingTransition(
//							R.anim.alpha_in, R.anim.alpha_out);
//				}
//				finish();
//			}
//		}.start();
//	}
//}

public class SplashActivity extends Activity
{
	
	private Activity myself;


	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		super.onCreate(savedInstanceState);
		setContentView(R.layout.splashscreen);
		myself = this;

		
		Log.i("BloodScreen", "touch pass!");
		myself.finish();

		Intent intent = new Intent();
		intent.setClass(SplashActivity.this, GameSanGuo.class);
		startActivity(intent);
		//运行时获取android sdk的版本
		int VERSION = android.os.Build.VERSION.SDK_INT;
		//if (VERSION >= 5)
		{
			//淡入淡出效果
			SplashActivity.this.overridePendingTransition(
					R.anim.alpha_in, R.anim.alpha_out);
		}

	}
}
