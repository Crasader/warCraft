/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.xdyou.sanguo;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.WindowManager;

import com.appsflyer.AppsFlyerLib;
import com.chartboost.sdk.Chartboost;
import com.facebook.FacebookException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.HttpMethod;
import com.facebook.Request;
import com.facebook.Request.Callback;
import com.facebook.Request.GraphUserCallback;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.SessionState;
import com.facebook.model.GraphPlace;
import com.facebook.model.GraphUser;
import com.facebook.widget.WebDialog;
import com.facebook.widget.WebDialog.OnCompleteListener;
import com.gameanalytics.android.GameAnalytics;
import com.gmobi.trade.ICallback;
import com.gmobi.trade.TradeService;
import com.gocpa.android.sdk.GocpaTracker;
import com.gocpa.android.sdk.GocpaUtil;
import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.ads.identifier.AdvertisingIdClient.Info;
import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;
import com.mobileapptracker.MobileAppTracker;
import com.mobvista.Tracker;
import com.sponsorpay.SponsorPay;
import com.tendcloud.tenddata.TalkingDataGA;

public class GameSanGuo extends Cocos2dxActivity
{

	// hasoffers 全局
	public MobileAppTracker mobileAppTracker = null;

	// facebook 后台提供的APPID，与此处相对应values里的string.xml也有对应的appid需要设置，与此相一致
//	private final String APP_ID = "566941626743891";
	private final String APP_ID = "1543699082536762";

	private final String APP_LOGO_LINK = "http://privacypolicy.tw.xdyou.com/image/logo.png";
	private static Handler handler = null;
	
	//gocpa sdk appid
	private static final String GOCPA_APPID = "9d2d5abb56f74e6c984aa48c9626e34e";
	private static final String GOCPA_ADVID = "442";
	
	//GA SDK key&id
	private static final String GA_GAME_KEY = "8565cc41fc3d519b16be1254acf0ee04";
	private static final String GA_SCR_KEY = "7100775487b5ad597d11b376f5e8a76ea2bce9ff";
	
	
	// handler操作参数都写在这里。
	// facebook的操作。
	private static final int FB_LOGIN = 1;
	private static final int FB_LOGOUT = 2;
	private static final int FB_SHARE = 3;
	private static final int FB_CREATE_SESSION = 4;
	// 打开指定url。
	private static final int HANDLER_OPEN_URL = 10;	
	// 创建对话框。
	private static final int BUID_DIALOG = 7;
	
	private static final int GOTO_GOOGLE_PLAY = 5;
	
	//AppFlyers 登陆消息
	private static final int APP_FLYERS_LOGIN = 6;
	
	//MM:新增 调用游戏的showMessage。
	private static final int MSG_SHOW_TOAST = 20;
	
	//MM:新增 调用游戏的setUserDataNative
	private static final int MSG_SET_USER_DATA = 21;
	
	private static boolean IS_PRE_LOGIN_FACEBOOK = false;
	
	//是否因为分享而登陆FB。如果为true能退出时候清空FBsession，正常的起手登陆FB流程此永远为false。
	private static boolean IS_LOGIN_FB_VIA_SHARING_CONTENT = false;
	
	
	// 图像名字，与要发送的消息
	private String imageName = "";
	private String message = "";
	//facebook 相册中图片的地址
    String imageUrl = "";
    //上传facebook相册之后获取的id
	String imageId = "";

	// 游戏登陆向服务发送的loginKey
	private static String loginKey = "";
	
	private final static String MARKET_LINK = "market://details?id=";
	
	
	// 要向facebook登陆用户请求的权限
	private static final String PERMISSION = "publish_actions";
	// 分享成功与失败的消息
	private static final String SHARE_FAILED_INFO = "分享失败! 请重试!";
	private static final String SHARE_SUCCESS_INFO = "分享成功!";

	// facebookSession状态改变的回调
	private Session.StatusCallback statusCallback = new Session.StatusCallback()
	{
		@Override
		public void call(final Session session, SessionState state,
				Exception exception)
		{
			onSessionStateChange(session, state, exception);
		}
	};
	
	private int tempTick = 0;

	// facebook session 状态改变之后根据需要获取相应内容
	private void onSessionStateChange(Session session, SessionState state,
			Exception exception)
	{
		session = Session.getActiveSession();
		Log.e("FB", "onSessionStateChange call=" + tempTick + " begin!");

		// 如果session存在，并且处理打开状态
		if (session != null && session.isOpened())
		{
			Log.e("FB", "onSessionStateChange || session state changed!" + session.toString());

			// 请求publish_actions成功之后发布消息与图片 的状态
			if (session.getState().equals(SessionState.OPENED_TOKEN_UPDATED))
			{
				postImageAndMessage();
				return;
			}

			// 获取登陆用户的基本信息， public_profile的内容
			loginKey = session.getAccessToken();
			Request.newMeRequest(session, new GraphUserCallback()
			{
				@Override
				public void onCompleted(GraphUser user, Response response)
				{
					Log.e("FB", "onSessionStateChange onCompleted callback!");
					
					String fbUserName = "";
					String fbUserId = "";
					if (user != null)
					{
						// 在游戏登陆内充当loginkey
						Log.e("FB", "facebook token: " + loginKey);
						Log.e("FB", "facebook id : " + user.getId());
						Log.e("FB", "facebook username : " + user.getName());
						
						fbUserName = user.getName();
						fbUserId = user.getId();
						

						// 加入新的状态判定，最后一个参数
						if (IS_PRE_LOGIN_FACEBOOK == true)
						{
							Log.e("FB", "onSessionStateChange || POST IMAGE=" + imageName);
							IS_PRE_LOGIN_FACEBOOK = false;
							IS_LOGIN_FB_VIA_SHARING_CONTENT = true;
							
							// 查看是否拥有发布状态的权限
							if (!hasPublishPermission())
							{
								Log.d("sanguo", "go2play fbShareContent || request permission");
								requestNewPublishPermission();
								return;
							}
							else
							{
								// 如果有发布的权限直接发布
								Log.d("sanguo", "go2play fbShareContent || publish content");
								postImageAndMessage();
							}
						}
						else
						{
							Log.e("FB", "onSessionStateChange || LOGIN DONE=" + imageName);
							runFbUserIdAndUserName(fbUserName, fbUserId, loginKey, true);
						}


						
					}
					if (response.getError() != null)
					{
						// 处理获取失败的错误
						Log.e("FB", "error : " + response.getError());
					}
				}
			}).executeAsync();
		}
		
		Log.e("FB", "onSessionStateChange call=" + tempTick + " end!");
		tempTick++;
	}

	// 本地方法，实现在C++
	// 获取用户id与用户名称，都是facebook的
	public static native boolean fbUserIdAndUserName(String userName,
			String userId, String loginKey, boolean setFBState);

	// 设置session状态
	public static native void setSessionState(boolean state);

	// facebook分享信息提示（成功或者失败）
	public static native void fbShareTips(String tips);

	// facebook登出
	public static native void fbDoLogout();

	// 如果发布状态成功，也就是分享成功，向服务器以送消息，向用户发送奖励
	public static native void sendPostSuccess();
	
	// 本地方法，获取游戏客户端更新URL。
	public static native String getClientUpdateUrl();

	// jni从java调用c++涉及到渲染的，需要在cocos2d-x
	// thread去完成，而不是cocos2dxActivity中（下面函数名带run的都是这个目的）
	public static void runFbUserIdAndUserName(final String userName,
			final String userId, final String loginKey, final boolean setFbState)
	{
		Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable()
		{
			@Override
			public void run()
			{
				fbUserIdAndUserName(userName, userId, loginKey, setFbState);
			}
		});
	}

	public static void runSetSessionState(final boolean state)
	{
		Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable()
		{
			@Override
			public void run()
			{
				runSetSessionState(state);
			}
		});
	}

	public static void runFbShareTips(final String tips)
	{
		Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable()
		{
			@Override
			public void run()
			{
				fbShareTips(tips);
			}
		});
	}

	public static void runfbDoLogout()
	{
		Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable()
		{
			@Override
			public void run()
			{
				fbDoLogout();
			}
		});
	}

	public static void runSendPostSuccess()
	{
		Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable()
		{
			@Override
			public void run()
			{
				sendPostSuccess();
			}
		});
	}

	//由getTalkingDataAppId本地方法获取，这里留底
	// for talkingdata AADCA30D169116CF80FCAEA8EB601177
	//private static final String talkingDataAppId = "191F6CA0284EFD11DCF6E8AAB81851D7";
	
	private static final String talkingDataAppId = getTalkingDataAppId();
	private static final String talkingDataChannelId = getTalkingDataAppChannel();
	
	//统一由c++处获取 talkingdata AppId
	public static native String getTalkingDataAppId();
	//统一由c++处获取 td AppChannelTag
	public static native String getTalkingDataAppChannel();
	

	private Chartboost cb; // for chartboost ad sdk
	private static Cocos2dxActivity mActivity = null;

	

	
	protected void onCreate(Bundle savedInstanceState)
	{
		Log.d("SGAPP", "onCreate");
		mActivity = this;
		super.onCreate(savedInstanceState);
		
		//AppsFlyerSDK initial 2015.1.12
		AppsFlyerLib.setAppsFlyerKey("6HRzTLQfzQHGBQ87KzZttH");	
		//开启跟踪 AppsFlyer
		AppsFlyerLib.sendTracking(getApplicationContext());
		
		//GA sdk 启动
		GameAnalytics.initialise(this, GA_SCR_KEY, GA_GAME_KEY);
		GameAnalytics.startSession(this);
		
		
		
		//MM: AdvertiserSDK 2014.11.26
		Tracker.conversionTrack(this, "zywx_sgyxlm_hk_mo_tw");
		
		//设置屏幕不待机
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		// ========================启动TalkingData 实例
		Log.d("SGAPP", "talkingdataAppid || " + talkingDataAppId);
		TalkingDataGA.init(this, talkingDataAppId, talkingDataChannelId);
		// ========================启动TalkingData 实例

		
		
		// ===================Facebook 启动相关设置
		// 在启用之前测试并创建一个session
		this.createFBSession();
		// Settings.addLoggingBehavior(LoggingBehavior.INCLUDE_ACCESS_TOKENS);
		// ===================Facebook 启动相关设置
		
		
		//GoCPA
		GocpaUtil.setAppId(GOCPA_APPID);
		GocpaUtil.setAdvertiserId(GOCPA_ADVID);
		GocpaUtil.setReferral(false);
		
		GocpaTracker.getInstance(this).reportDevice();		
		
		// ==================Go2Play 充值全局listener_Begin_=======================================
		TradeService.setOnBalanceChangedListener(new ICallback()
		{
			@Override
			public void onResult(boolean isSuccess, Bundle bundle, Throwable ths)
			{
				// 这里做余额变动的逻辑
				if (isSuccess)
				{
					Log.e("go2play", "Charged Success!");
					Log.e("go2play",
							"Balance: " + TradeService.getBalance(false));
				}
			}

		});
		// ==================Go2Play 充值全局listener_End_=========================================

		
		// ==================ChartBoost 广告SDK _Begin_============================================

		// 初始化对应的chartboost实例
		this.cb = Chartboost.sharedChartboost();

		// chartboost的appId与appKey, 都是由sdk提供方提供
		final String appIdCB = "54af952a43150f627b0fbb4a";
		final String appSignatureCB = "bea16683ef974c0393cf55c41719ef694ae8a0d5";

		this.cb.onCreate(this, appIdCB, appSignatureCB, null);

		// ==================ChartBoost 广告SDK_End_==============================================

		// ==================hasoffers 广告统计_Begin_============================================
		MobileAppTracker.init(getApplicationContext(), "24078", "3fba6dcf91bc7251866e59278574f16a");
		mobileAppTracker = MobileAppTracker.getInstance();

		// Collect Google Play Advertising ID
		new Thread(new Runnable()
		{
			@Override
			public void run()
			{
				try
				{
					Info adInfo = AdvertisingIdClient.getAdvertisingIdInfo(getApplicationContext());
					
					mobileAppTracker.setGoogleAdvertisingId(adInfo.getId(), adInfo.isLimitAdTrackingEnabled());
				}
				catch (IOException e)
				{

				}
				catch (GooglePlayServicesNotAvailableException e)
				{
					mobileAppTracker.setAndroidId(Secure.getString(getContentResolver(), Secure.ANDROID_ID));
				} 
				catch (GooglePlayServicesRepairableException e)
				{
					// Encountered a recoverable error connecting to Google Play
					// services.
				}
			}
		}).start();

		// For collecting Android ID, device ID, and MAC address
		// Before August 1st 2014, remove these calls - only Google AID should
		// be collected.
		mobileAppTracker.setAndroidId(Secure.getString(getContentResolver(),
				Secure.ANDROID_ID));
		String deviceId = ((TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE))
				.getDeviceId();
		mobileAppTracker.setDeviceId(deviceId);
		try
		{
			WifiManager wm = (WifiManager) getSystemService(Context.WIFI_SERVICE);
			mobileAppTracker.setMacAddress(wm.getConnectionInfo()
					.getMacAddress());
		} catch (NullPointerException e)
		{
			System.out.println("NullPointerExp");
		}

		// ==================hasoffers 广告统计 _End_===========================

		// 处理消息请求， 这里做Handler的消息分发处理，根据不同的数值确定不同的facebook操作
		handler = new Handler()
		{
			public void handleMessage(Message msg)
			{
				switch (msg.what)
				{
				case FB_LOGIN:
					fbLogin();
					break;
				case FB_LOGOUT:
					fbLogout();
					break;
				case FB_SHARE:
				{
					// 获取之后再切分，是图片名称与要发送的消息
					String imageName = msg.obj.toString();
					fbShareContent(imageName);
				}
					break;
				case FB_CREATE_SESSION:
				{
					createFBSession();
				}
					break;
				case GOTO_GOOGLE_PLAY:
				{
					enterGooglePlay();
				}
					break;
				case BUID_DIALOG:
				{
					buildDialog();
				}
					break;
				case HANDLER_OPEN_URL:
				{
					execOpenUrl((String) msg.obj); 
				}
					break;
				case APP_FLYERS_LOGIN:
				{
					appFlyersTrackerLogin((String) msg.obj); 
				}
					break;
				case MSG_SHOW_TOAST:
				{
					handleShowToast((String) msg.obj);
				}
					break;
				case MSG_SET_USER_DATA:
				{
					handleSetUserData((SgUserData) msg.obj);
				}
					break;
				default:
					Log.e("facebook", "error come out!");
					System.out.println("oh shit!");
					break;
				}
			}
		};

	}

	public Cocos2dxGLSurfaceView onCreateView()
	{
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		// TestCpp should create stencil buffer
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

		return glSurfaceView;
	}

	// 外部c++调用，facebook创建session
	public static void callFBCreateSession()
	{
		Message createSession = new Message();
		createSession.what = FB_CREATE_SESSION;
		handler.sendMessage(createSession);

	}

	// 由外部c++调用 facebook 登陆
	public static void callFBLogin()
	{
		Message loginMessage = new Message();
		loginMessage.what = FB_LOGIN;
		handler.sendMessage(loginMessage);
	}

	// c++ 调用 facebook 登出
	public static void callFBLogout()
	{
		Message logoutMessage = new Message();
		logoutMessage.what = FB_LOGOUT;
		handler.sendMessage(logoutMessage);
	}

	// c++调用 facebook 分享
	public static void callFBShare(String imageName, String message)
	{
		Message shareMessage = new Message();
		shareMessage.what = FB_SHARE;
		// 将二个信息字符串拼接
		shareMessage.obj = imageName;
		handler.sendMessage(shareMessage);
	}
	//c++ 调用 进入google play
	public static void callGotoGoolePlay()
	{
		Message gotoGP = new Message();
		gotoGP.what = GOTO_GOOGLE_PLAY;
		handler.sendMessage(gotoGP);
	}
	//appsflyer 检测app登陆的方法，由c++调用检测
	public static void appsFlyerLoginTracker(String userId)
	{
		Message appFlyer = new Message();
		appFlyer.what = APP_FLYERS_LOGIN;
		appFlyer.obj = userId;
		handler.sendMessage(appFlyer);
	}
	
	public void appFlyersTrackerLogin(String userId)
	{
		//跟踪登陆， userId为预留信息字段这里暂定为唯一用户标识
		AppsFlyerLib.sendTrackingWithEvent(getApplicationContext(), "Login", userId);
	}
	
	
	//跳望gp/3rd等指定url 
	public void enterGooglePlay()
	{
		String url = this.getClientUpdateUrl();
		Intent intent = new Intent(Intent.ACTION_VIEW);
		Log.d("url", url);
		intent.setData(Uri.parse(url));
		startActivity(intent);
	}
	
	//c++ 调用 打开url。
	public static void callOpenUrl(String url)
	{
		Message ou = new Message();
		ou.what = HANDLER_OPEN_URL;
		ou.obj = url;
		handler.sendMessage(ou);
	}
	
	//执行打开url。
	public void execOpenUrl(String url)
	{
		Intent intent = new Intent(Intent.ACTION_VIEW);
		Log.d("sanguo", "openUrl" + url);
		intent.setData(Uri.parse(url));
		startActivity(intent);
	}

	// facebook login
	public void fbLogin()
	{
		Log.e("go2play", "facebook login!");
		Session session = Session.getActiveSession();
		if (!session.isOpened() && !session.isClosed())
		{
			Log.e("go2play", "set permission");
			Session.OpenRequest op = new Session.OpenRequest(this);
			op.setCallback(statusCallback);
			session.openForRead(op);
		} else
		{
			Session.openActiveSession(this, true, statusCallback);
		}

	}

	// facebook logout
	public void fbLogout()
	{
		Log.e("go2play", "facebook logout!");
		Session session = Session.getActiveSession();
		if (!session.isClosed())
		{
			runfbDoLogout();
			session.closeAndClearTokenInformation();
		}
	}

	// 如果session不存在, 创建session
	public void createSession()
	{
		Session activeSession = Session.getActiveSession();
		if (activeSession == null || activeSession.getState().isClosed())
		{
			activeSession = new Session.Builder(this).setApplicationId(APP_ID).build();
			Session.setActiveSession(activeSession);
		}
	}

	// 创建facebook的session
	public void createFBSession()
	{

		Log.e("go2play", "enter createFBCreate function!");
		Session session = Session.getActiveSession();
		if (session == null)
		{
			Log.e("go2play", "session is null");
			if (session == null)
			{
				session = new Session(this);
			}
			Session.setActiveSession(session);
			if (session.getState().equals(SessionState.CREATED_TOKEN_LOADED))
			{
				session.openForRead(new Session.OpenRequest(this)
						.setCallback(statusCallback));
			}
		}
		// 如果session存在，并且处理打开状态
		if (session != null && !session.isOpened())
		{
			if (session.getState().equals(SessionState.CREATED_TOKEN_LOADED))
			{
				session.openForRead(new Session.OpenRequest(this)
						.setCallback(statusCallback));
			}
		}
	}

	// 请求新的权限
	private void requestNewPublishPermission()
	{
		Session session = Session.getActiveSession();
		System.out.println("session in requestNewPublishPermission "
				+ session.toString());

		// 向facebook 请求publish_actions权限
		if (session.isOpened())
		{
			session.requestNewPublishPermissions(new Session.NewPermissionsRequest(
					this, PERMISSION));
			return;
		}
	}

	
	//上传图片到facebook
	private void uploadImage(Bitmap mUploadBitmap) {
        // 上传图片
        if (mUploadBitmap != null && !mUploadBitmap.isRecycled()) {
            // // 上传图片的请求, 可以用
        	Log.d("FB", "upload image");
            Request request = Request.newUploadPhotoRequest(Session.getActiveSession(), mUploadBitmap, mUploadCallback);
            request.executeAsync();
        }
    }

    private Callback mUploadCallback = new Callback()
    {
        @Override
        public void onCompleted(Response response) 
        {
        	Log.d("FB", "upload image completed");
            if (response.getError() != null) {
                Log.d("FB", "upload image error "+ response.getError().toString());
            } 
            else 
            {
                // 图片上传成功,获取response 的 json对象
                JSONObject jsonObject = response.getGraphObject().getInnerJSONObject();
                try 
                {
                	Log.d("FB", "get Json Data Success");
                    // get image id
                    imageId = jsonObject.getString("id");
                    
                    Log.d("FB", imageId);
                    
                    new Thread(new Runnable(){  
                        @Override  
                        public void run() {  
                        	doImageUrlRequest(imageId);
                        }  
                    }).start();  
                    
                } catch (JSONException e) 
                {
                    e.printStackTrace();
                }
            }
        } // end of onCompleted
    };
    
    
    String streamToJson(InputStream is) throws IOException
    { 
    	   BufferedReader in = new BufferedReader(new InputStreamReader(is)); 
    	   StringBuffer buffer = new StringBuffer(); 
    	   String line = ""; 
    	   while ((line = in.readLine()) != null){ 
    	     buffer.append(line); 
    	   } 
    	   return buffer.toString(); 
    } 
    private String doImageUrlRequest(String imageID) 
    {
    	Log.d("FB", "start get image url");
        String FACEBOOK_URL = "https://graph.facebook.com/?";
        // 构造http请求来获取图片地址
        HttpGet httpget = new HttpGet(FACEBOOK_URL + "id=" + imageID
                + "&access_token=" + Session.getActiveSession().getAccessToken());
        
        Log.d("FB", FACEBOOK_URL + "id=" + imageID
                + "&access_token=" + Session.getActiveSession().getAccessToken());
        
        HttpParams httpparams = new BasicHttpParams();
        HttpConnectionParams.setConnectionTimeout(httpparams, 15000);
        HttpConnectionParams.setSoTimeout(httpparams, 30000);

        // http client
        HttpClient httpclient = new DefaultHttpClient(httpparams);
        try {
            HttpResponse response = httpclient.execute(httpget);

            if (response.getStatusLine().getStatusCode() == 200) {
            	Log.d("FB", "http requst status 200");
                // 获取json data
                String json = streamToJson(response.getEntity().getContent());
                Log.d("FB", json);
           
                // get the image url
                if (json.contains("picture")) {
                	
                	Log.e("FB", json);
                    imageUrl = getImageUrl(json);
                    Log.d("FB","image Url ===> " + imageUrl);
                    sendBuildMessage();
                    
                } else {
                    Log.e("FB", "### 获取分享到facebook的图片url失败");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return imageUrl;
    }
	public void sendBuildMessage()
	{
		Message build = new Message();
		build.what = BUID_DIALOG;
		handler.sendMessage(build);
	}
    private void buildDialog()
    {
        //构造弹出框

     		Bundle params = new Bundle();

     	    params.putString("app_id", APP_ID);
     	    params.putString("name", "三國英雄聯盟");
     	    params.putString("caption", "我正在玩《三國英雄聯盟》，一起來試試吧！");
     	    params.putString("description", "顛覆傳統三國手遊，猛將美姬一應俱全。實時對戰酣暢淋漓，號令天下指日可待！");
     	    String url = this.getClientUpdateUrl();
     	    Log.d("buildDialog", "updateUrl=" + url);
     	    params.putString("link", url);
     	    params.putString("picture", APP_LOGO_LINK);

     	    WebDialog feedDialog = (new WebDialog.FeedDialogBuilder(getActivity(), Session.getActiveSession(),params))
             .setOnCompleteListener(new OnCompleteListener() 
             {
                 @Override
                 public void onComplete(Bundle values,
                     FacebookException error) 
                 {
              	   Log.d("FB", "WebDialog build ");
                     if (error == null)
                     {
                         // When the story is posted, echo the success
                         // and the post Id.
                         final String postId = values.getString("post_id");
                         //发布成功
                         if (postId != null) 
                         {
                         		runSendPostSuccess();
                         } 
                         else
                         {
                             	// User clicked the Cancel button
                         		runFbShareTips(SHARE_FAILED_INFO);
                         }
                     }
                     else if (error instanceof FacebookOperationCanceledException) 
                     {
                         	// User clicked the "x" button
                     		runFbShareTips(SHARE_FAILED_INFO);
                     } else 
                     {
                         	// Generic, ex: network error
                     		runFbShareTips(SHARE_FAILED_INFO);
                     }
                 }

             })
             .build();
     	    feedDialog.show();
    }
	
	
	private String getImageUrl(String json) throws JSONException
	{
	     JSONObject jsonObj=new JSONObject(json); 

	     if(jsonObj.has("picture")) 
	     {
	    	 Log.d("FB","image Url [getImageUrl Json]" + jsonObj.getString("picture"));
	    	 return jsonObj.getString("picture");
	     }
	     else
	     {
	    	 return null;
	     }
	}
	
	

	// 请求发送状态
	private void postImageAndMessage()
	{
		Bitmap image = null;
		if (imageName != null)
		{
			image = BitmapFactory.decodeFile(imageName);
			System.out.println("go2play -- image " + image);
		}
		//uploadImage(image);
		sendBuildMessage();
	
		
//		Request request = newFaceBookRequest(Session.getActiveSession(),
//				imageName, message, null, new Callback()
//				{
//					@Override
//					public void onCompleted(Response response)
//					{
//						// need publish_actions permission
//						if (response.getError() != null)
//						{
//							Log.d("sanguo", "go2play fbShareContent || failed! error=" + response.getError().getErrorMessage());
//							runFbShareTips(SHARE_FAILED_INFO);
//						}
//						else
//						{
//							Log.d("sanguo", "go2play fbShareContent || succeeded!");
//							// 分享成功提示
//							// runFbShareTips(SHARE_SUCCESS_INFO);
//							// 分享成功后，直接向服务器发送消息
//							runSendPostSuccess();
//						}
//					}
//				});
//		RequestAsyncTask task = new RequestAsyncTask(request);
//		task.execute();
	}

	private Context getActivity()
	{
		return this.getContext();
	}

	// 检测是否拥有pulish_actions权限
	private boolean hasPublishPermission()
	{
		Session session = Session.getActiveSession();
		return session != null
				&& session.getPermissions().contains("publish_actions");
	}

	// 包装一层newFaceBookRequest
	public void fbShareContent(String imageName)
	{
		this.imageName = imageName;
		Session session = Session.getActiveSession();
		
		Log.e("FB", session.toString() + "state ||" + session.isOpened());
		if (!session.isOpened() && !session.isClosed())
		{
			Log.d("FB", "create session");

			Session.OpenRequest op = new Session.OpenRequest(this);
			op.setCallback(statusCallback);
			session.openForRead(op);
			IS_PRE_LOGIN_FACEBOOK = true;
			return;
		}
		
		Log.d("sanguo", "go2play fbShareContent || imageName:" + imageName + "，message:"
				+ message + ",tk:" + session.getAccessToken());

		// 查看是否拥有发布状态的权限
		if (!hasPublishPermission())
		{
			Log.d("sanguo", "go2play fbShareContent || request permission");
			requestNewPublishPermission();
			return;
		}
		else
		{
			// 如果有发布的权限直接发布
			Log.d("sanguo", "go2play fbShareContent || publish content");
			postImageAndMessage();
		}

	}

	// 同时发布文字与图片
	public Request newFaceBookRequest(Session session, String imageName,
			String message, GraphPlace graphPlace, Callback callback)
	{
		// 如果含有图片
		if (session != null && session.isOpened())
		{
			System.out.println("go2play -- session = " + session.toString()
					+ "session.isOpened : " + session.isOpened());
			Bitmap image = null;
			if (imageName != null)
			{
				image = BitmapFactory.decodeFile(imageName);
				System.out.println("go2play -- image " + image);
			}

			Bundle parameters = new Bundle();
			// 位置信息
			if (graphPlace != null)
			{
				parameters.putString("place", graphPlace.getId());
			}
			// 文字信息
			if (message != null)
			{
				parameters.putString("message", message);
			}
			// 图片信息
			if (image != null)
			{
				parameters.putParcelable("picture", image);
				// 如果有图片直接返回这个带图片和文字信息的内容
				return new Request(session, "me/photos", parameters,
						HttpMethod.POST, callback);
			}
			// 否则只发布文字
			return new Request(session, "me/feed", parameters, HttpMethod.POST,
					callback);
		} else
		{
			System.out.println("session is null or session is closed!");
			return null;
		}
	}
	
	public static String urlEncode(String str)
	{
		return str;
//		try {
//			return URLEncoder.encode(str, "UTF-8");
//		} catch (UnsupportedEncodingException e) {
//			e.printStackTrace();
//			return "UFO_MAC_ADDRESS";
//		}
	}

	// 获取android mac地址
	public static String getMacID()
	{
		String str = "mac_address";

		WifiManager wifi = (WifiManager) mActivity
				.getSystemService(Context.WIFI_SERVICE);
		WifiInfo info = wifi.getConnectionInfo();
		str = info.getMacAddress();

		// 如果获取失败，获取imei码
		if (str == null)
		{
			TelephonyManager tm = (TelephonyManager) mActivity
					.getSystemService(TELEPHONY_SERVICE);
			str = tm.getDeviceId();
		}
		Log.e("mac_address", str);
		return str;
	}

	// 大对象分配前的内存回收
	public static void beforeAllocationGc()
	{
		System.gc();
	}

	// 响应返回按钮 =====BEGIN======
	/*
	@Override
	public boolean dispatchKeyEvent(KeyEvent event)
	{
		if (event.getKeyCode() == KeyEvent.KEYCODE_BACK
				&& event.getAction() == KeyEvent.ACTION_DOWN
				&& event.getRepeatCount() == 0)
		{
			AlertDialog.Builder build = new AlertDialog.Builder(this);
			build.setTitle("主公且慢")
					.setMessage("要放棄天下歸隱山林了嗎?")
					.setPositiveButton("確定",
							new DialogInterface.OnClickListener()
							{
								@Override
								public void onClick(DialogInterface dialog,
										int which)
								{
									finish();
								}
							})
					.setNegativeButton("取消",
							new DialogInterface.OnClickListener()
							{

								@Override
								public void onClick(DialogInterface dialog,
										int which)
								{
								}
							}).show();
		}
		return super.dispatchKeyEvent(event);
	}
	*/
	// 响应返回按钮 =====END======

	// for chartboost and facebook sdk override
	@Override
	protected void onStart()
	{
		Log.d("SGAPP", "onStart");
		super.onStart();
		this.cb.onStart(this);
		// facebook
		Session.getActiveSession().addCallback(statusCallback);
	}

	@Override
	protected void onStop()
	{
		Log.d("SGAPP", "onStop");
		super.onStop();
		this.cb.onStop(this);
		// facebook
		Session.getActiveSession().removeCallback(statusCallback);
	}

	@Override
	protected void onDestroy()
	{
		Log.d("SGAPP", "onDestroy");
		//clear fb session
		if (IS_LOGIN_FB_VIA_SHARING_CONTENT)
		{
			Log.e("FB", "IS_LOGIN_FB_VIA_SHARING_CONTENT! so we clear facebook session!");
			Session session = Session.getActiveSession();
			if (session != null)
			{
				session.closeAndClearTokenInformation();
			}
		}
		this.cb.onDestroy(this);
		// setSessionState(false);
		super.onDestroy();
		System.exit(0);
	}

	@Override
	public void onBackPressed()
	{
		if (this.cb.onBackPressed())
			return;
		else
			super.onBackPressed();
	}

	// facebook sdk override
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		Log.d("sgresult", "req=" + requestCode + "result," + resultCode);
		super.onActivityResult(requestCode, resultCode, data);
		Session.getActiveSession().onActivityResult(this, requestCode,
				resultCode, data);
	}

	@Override
	protected void onSaveInstanceState(Bundle outState)
	{
		super.onSaveInstanceState(outState);
		// facebook的session状态相关
		Session session = Session.getActiveSession();
		Session.saveSession(session, outState);
	}

	@Override
	protected void onResume()
	{
		Log.d("SGAPP", "onResume");
		super.onResume();
		//MM: FB统计
		com.facebook.AppEventsLogger.activateApp(this, APP_ID);
		
		//start sponsor
		try 
		{
			SponsorPay.start("637a0987b77b1277eaa099e92f31787e", null, null, this); 
		} 
		catch (RuntimeException e)
		{
			Log.d("SGAPP", e.getLocalizedMessage());
		}
		
		TalkingDataGA.onResume(this);
		//MM: AdvertiserSDK 2014.11.26
		Tracker.conversionTrack(this, "zywx_sgyxlm_hk_mo_tw");
		
		//GA
		GameAnalytics.startSession(this);

		// hasoffers
		mobileAppTracker.setReferralSources(this);
		mobileAppTracker.measureSession();
	}

	@Override
	protected void onPause()
	{
		Log.d("SGAPP", "onPause");
		super.onPause();
		TalkingDataGA.onPause(this);
		GameAnalytics.stopSession();
	}
	
	//由sdkcaller调用。
	public static void tryShowToast(String notice)
	{
		Message msg = new Message();
		msg.what = MSG_SHOW_TOAST;
		msg.obj = notice;
		handler.sendMessage(msg);
	}
	
	//由sdkcaller调用。
	public static void trySetUserData(SgUserData ud)
	{
		Message msg = new Message();
		msg.what = MSG_SET_USER_DATA;
		msg.obj = ud;
		handler.sendMessage(msg);
	}
	
	//匹配楼上try的处理。
	public static void handleShowToast(String notice)
	{
		final String text = notice;
		Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable()
		{
			@Override
			public void run()
			{
				SDKCaller.showToastNative(text);
			}
		});
	}
	
	//匹配楼上try的处理。
	public static void handleSetUserData(SgUserData ud)
	{
		final boolean cIsSuc = ud.isSuccess;
		final String cUid = ud.userId;
		final String cUname = ud.userName;
		final boolean cIsFb = ud.isFacebook;
		
		Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable()
		{
			@Override
			public void run()
			{
				SDKCaller.setUserDataNative(cIsSuc, cUid, cUname, cIsFb);
			}
		});
	}
	
}
