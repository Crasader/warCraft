package com.xdyou.sanguo;

import java.util.UUID;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.R.bool;
import android.app.ActivityManager;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Toast;

import com.gmobi.trade.Actions;
import com.gmobi.trade.ICallback;
import com.gmobi.trade.TradeService;


// android的SDK适配类, 通过这个类与c++进行通信
public class SDKCaller
{	
	//sdk连接信息
	static String connectInfo = "";
	//sdk注册信息
	static String regInfo = "";
	//sdk登陆信息
	static String loginInfo = "";
	//sdk检测注册帐号名是否存在
	static boolean isExist = true;
	//sdk使用FB登陆所记录的用户id。
	static String fbUserId = "";
	
	static final String tag = "go2play";
	
	//註冊和登陸的錯誤提示，SDK方未提供錯誤代碼，統一為此。
	static final String regFailedNotice = "用戶名已存在，請重新輸入。";
	static final String loginFailedNotice = "登入失敗，請檢查後再嘗試。";
	static final String fbLogoutFailedNotice = "facebook登出失敗，請檢查後再嘗試。";
	static final String loginGameFailedNotice = "登入遊戲失敗，請檢查後再嘗試。";
	static final String fastLoginFailedNotice = "快速登入失敗，請檢查後再嘗試。";
	static final String fbLoginFailedNotice = "facebook登入失敗，請檢查後再嘗試。";
	
	//特殊参数key，用于对接用户系统。
	static final String GO2PLAY_CP_UUID_KEY = "cp_uuid";
	static final String GO2PLAY_CP_MAC_ADDRESS = GameSanGuo.urlEncode(GameSanGuo.getMacID());
	
	//检测SDK是不问已经启动
	public static boolean sdkIsStarted()
	{
		return  TradeService.isStarted();
	}

	//获取余额
	public static int getCurrentBalance(boolean isSync)
	{
		//sdk未启动, 直接返回
		if (!TradeService.isStarted())
		{
			return 0;
		}
		return TradeService.getBalance(isSync);
	}
	
	
	/*
	 * 获取渠道的类型 
	 */
	
	public static native boolean isGoolePlayChannel();
	
	//执行不同的action, (注册, 登陆, 购买, 检测帐号是否可用, 检测连接状态)
	//购买
	public static void charge(String chargeId, int payment, int serverId, String accountId, int roleId)
	{
		Log.e(tag, "Charge Starting...");
		//sdk未启动, 直接返回
		if (!TradeService.isStarted())
		{
			return ;
		}
		//chargeid不合法
		if (chargeId.length() == 0)
		{
			Log.e(tag, "invalid charge id");
			return ;
		}
		
		//订单跟踪id，由于暂时弄不清他是否转发玩家信息，在此拼接上serverId/accountId/roleId，最终形如：“17,haoren123,98765,9ce1874d-0359-466e-961f-db85a1d7be9e”。
		final String trackId = serverId + "," + accountId + "," + roleId + "," + chargeId + "," + UUID.randomUUID().toString();
		//充值的金额，RMB/NT$等。
		final int billNum = payment;
		Log.d(tag, "chargeParam || bId=" + trackId + ", bNum=" + billNum);
		//额外的参数
		Bundle bundle = new Bundle();
		
		//设置trackId
		String ptiStr = Actions.PARAM_TRACK_ID;
		bundle.putString(ptiStr, trackId);
		//付款金额
		String rpStr = Actions.RESULT_PAYMENT;
		bundle.putString(rpStr, payment + "");
		
		
		
		//由本地c++获取渠道id，是不是googleplay渠道
		boolean isGoolePlay = isGoolePlayChannel();
		
		//MM: 这里开启后可使用google wallet进行测试。
		
		//是googleplay渠道的包才开启google wallet
		if (isGoolePlay)
		{
			bundle.putBoolean(Actions.PARAM_FORCE_GOOGLE_WALLET, true);			
		}
		
		//执行action
		String chStr = Actions.CHARGE + chargeId;
		TradeService.execute(chStr, new ICallback()
		{
			@Override
			public void onResult(boolean isSuccess, Bundle resData, Throwable error)
			{
				if (resData == null)
				{
					Log.e(tag, "chargeResult || error! rd is null!");
					return;
				}
				
//				String bId = resData.getString(Actions.PARAM_TRACK_ID, "hehe"); //订单追踪ID
//				int bNum = resData.getInt(Actions.RESULT_PAYMENT, 1); //订单金额NUM 
//				Log.d(tag, "chargeResult || isOk=" + isSuccess + ", bId=" + bId + ", bNum=" + bNum);
				
				//如果成功
				if (isSuccess)
				{
					Log.d(tag, "chargeResult || Charge Succeeded!");
					//充值成功的页面，向talkingdata发送成功充值的消息
//					dealChargeResult(true, bId, bNum);
				}
				else
				{
					Log.e(tag, "chargeResult || Charge Failed!");
//					dealChargeResult(false, bId, bNum);
				}
			}
			
		}, bundle);		
		
	}

	//即fastLoginGame，SSO_CONNECT 如果当前设备已经注册帐号,就会直接返回userId与userKey,如果当前设备没有注册,就会注册一个新userId并在数据中返回这个userId
	public static String connect()
	{
		//sdk未启动, 直接返回
		if (!TradeService.isStarted())
		{
			return null;
		}
		//每次调用connect都重新初始化connectInfo 
		connectInfo = "";
		
		Bundle param = new  Bundle();
		param.putString(GO2PLAY_CP_UUID_KEY, GO2PLAY_CP_MAC_ADDRESS);
		Log.i(tag, "connect | my madr=" + GO2PLAY_CP_MAC_ADDRESS);
		
		TradeService.execute(Actions.SSO_CONNECT, new ICallback()
		{
			@Override
			public void onResult(boolean isSuccess, Bundle param, Throwable error)
			{
				String userId = "";
				String userName = "";
				boolean isSuc = false;
				
				if (isSuccess)
				{
					if (param != null)
					{
						userId = param.getString(Actions.RESULT_USER_ID);
						userName = param.getString(Actions.RESULT_USER_NAME);
	
						if (userId != null)
						{
							isSuc = true;
							Log.e(tag, "connect userId=" + userId + ", userName=" + userName);
							loginInfo = userId;
						}
						else
						{
							showToast(fastLoginFailedNotice);
							Log.e(tag, "connect Error!");
						}
					}
				}
				else
				{
					showToast(fastLoginFailedNotice);
					Log.e(tag, "connect Failed!");
				}
				
				//将玩家账户信息设置到本地，加上成功与否标志。
				final boolean cIsSuc = isSuc;
				final String cUid = userId;
				final String cUname = userName;
				Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable()
				{
					@Override
					public void run()
					{
						fastLoginGameNative(cIsSuc, cUid, cUname);
					}
				});
			}
			
		}, param);
		return connectInfo;
	}
	
	//注册的Action, 注册帐号,如果成功,就直接返回这个数据
	public static String register(String userName, String password)
	{
		//sdk未启动, 直接返回
		if (!TradeService.isStarted())
		{
			return null;
		}
		
		regInfo = "";
		if (userName.length() == 0 || password.length() == 0 || password.length() < 6)
		{
			Log.e(tag, "invalid userName or password");
			return null;
		}
		
		Bundle param = new  Bundle();
		param.putString(Actions.PARAM_USERNAME, userName);
		param.putString(Actions.PARAM_PASSWORD, password);
		param.putString(GO2PLAY_CP_UUID_KEY, GO2PLAY_CP_MAC_ADDRESS);
		Log.i(tag, "register | my madr=" + GO2PLAY_CP_MAC_ADDRESS);
		
		TradeService.execute(Actions.SSO_REGISTER, new ICallback()
		{
			@Override
			public void onResult(boolean isSuccess, Bundle regData, Throwable error)
			{
				String userId = "";
				String userName = "";
				boolean isSuc = false;
				
				if (isSuccess)
				{
					if (regData != null)
					{
						userId = regData.getString(Actions.RESULT_USER_ID);
						userName = regData.getString(Actions.RESULT_USER_NAME);
						
						if (userId != null)
						{
							isSuc = true;
							Log.e(tag, "register userId=" + userId + ", userName=" + userName);
							regInfo = userId;
						}
						else
						{
							showToast(regFailedNotice);
							Log.e(tag, "register Error!");
						}
					}
				}
				else
				{
					showToast(regFailedNotice);
					Log.e(tag, "register Failed!");
				}
				
				//将玩家账户信息设置到本地。
				boolean cIsSuc = isSuc;
				String cUid = userId;
				String cUname = userName;
				setUserData(cIsSuc, cUid, cUname, false);
			}
			
		}, param);
		return regInfo;
	}
	
	//Login Action, 如果登陆成功就直接返回这个数据,里面包含了对应的userid与username
	public static String login(String userName, String password)
	{
		//sdk未启动, 直接返回
		if (!TradeService.isStarted())
		{
			return null;
		}
		//每次调用重新初始化登陆信息
		loginInfo = "";
		//信息为空或者密码长度不够
		if (userName.length() == 0 || password.length() == 0 || password.length() < 6)
		{
			Log.e(tag, "invalid userName or password");
			return null;
		}
		
		Bundle param = new Bundle();
		param.putString(Actions.PARAM_USERNAME, userName);
		param.putString(Actions.PARAM_PASSWORD, password);
		
		TradeService.execute(Actions.SSO_LOGIN, new ICallback()
		{
			@Override
			public void onResult(boolean isSuccess, Bundle param, Throwable error)
			{
				String userId = "";
				String userName = "";
				boolean isSuc = false;
				
				if (isSuccess)
				{
					if (param != null)
					{
						userId = param.getString(Actions.RESULT_USER_ID);
						userName = param.getString(Actions.RESULT_USER_NAME);
	
						if (userId != null)
						{
							isSuc = true;
							Log.e(tag, "login userId=" + userId + ", userName=" + userName);
							loginInfo = userId;
						}
						else
						{
							showToast(loginFailedNotice);
							Log.e(tag, "login Error!");
						}
					}
				}
				else
				{
					showToast(loginFailedNotice);
					Log.e(tag, "login Failed!");
				}
				
				//将玩家账户信息设置到本地，加上成功与否标志。
				boolean cIsSuc = isSuc;
				String cUid = userId;
				String cUname = userName;
				setUserData(cIsSuc, cUid, cUname, false);
			}
			
		}, param);
		
		return loginInfo;
	}
	
	//Login Game Action, 与login流程基本一致，只是本函数回调会直接登陆游戏。
	public static void loginGame(String userName, String password)
	{
		//sdk未启动, 直接返回
		if (!TradeService.isStarted())
		{
			return;
		}
		//每次调用重新初始化登陆信息
		loginInfo = "";
		//信息为空或者密码长度不够
		if (userName.length() == 0 || password.length() == 0 || password.length() < 6)
		{
			Log.e(tag, "invalid userName or password");
			return;
		}
		
		Bundle param = new Bundle();
		param.putString(Actions.PARAM_USERNAME, userName);
		param.putString(Actions.PARAM_PASSWORD, password);
		
		TradeService.execute(Actions.SSO_LOGIN, new ICallback()
		{
			@Override
			public void onResult(boolean isSuccess, Bundle param, Throwable error)
			{
				String userId = "";
				String userName = "";
				boolean isSuc = false;
				
				if (isSuccess)
				{
					if (param != null)
					{
						userId = param.getString(Actions.RESULT_USER_ID);
						userName = param.getString(Actions.RESULT_USER_NAME);
	
						if (userId != null)
						{
							isSuc = true;
							Log.e(tag, "login userId=" + userId + ", userName=" + userName);
							loginInfo = userId;
						}
						else
						{
							showToast(loginGameFailedNotice);
							Log.e(tag, "login Error!");
						}
					}
				}
				else
				{
					showToast(loginGameFailedNotice);
					Log.e(tag, "login Failed!");
				}
				
				//将玩家账户信息设置到本地，加上成功与否标志。
				final boolean cIsSuc = isSuc;
				final String cUid = userId;
				final String cUname = userName;
				Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable()
				{
					@Override
					public void run()
					{
						loginGameNative(cIsSuc, cUid, cUname);
					}
				});
			}
			
		}, param);
	}
	
	//FB的登录。
	public static void fbLogin()
	{
		//sdk未启动, 直接返回
		if (!TradeService.isStarted())
		{
			return;
		}
		//每次调用重新初始化登陆信息
		fbUserId = "";
		
		Bundle param = new Bundle();
		param.putString(Actions.PARAM_SSO_LOGIN_TYPE, Actions.FACEBOOK_LOGIN);
		param.putString(GO2PLAY_CP_UUID_KEY, GO2PLAY_CP_MAC_ADDRESS);
		
		Log.i(tag, "fbLogin | my madr=" + GO2PLAY_CP_MAC_ADDRESS);
		
		TradeService.execute(Actions.SSO_LOGIN, new ICallback()
		{
			@Override
			public void onResult(boolean isSuccess, Bundle param, Throwable error)
			{
				String userId = "";
				String userName = "";
				boolean isSuc = false;
				
				if (isSuccess)
				{
					if (param != null)
					{
						userId = param.getString(Actions.RESULT_USER_ID);
						userName = param.getString(Actions.RESULT_USER_NAME);
	
						if (userId != null)
						{
							isSuc = true;
							Log.e(tag, "fbLogin userId=" + userId + ", userName=" + userName);
							fbUserId = userId;
						}
						else
						{
							showToast(fbLoginFailedNotice);
							Log.e(tag, "fbLogin Error!");
						}
					}
				}
				else
				{
					showToast(fbLoginFailedNotice);
					Log.e(tag, "fbLogin Failed!");
				}
				
				//将玩家账户信息设置到本地，加上成功与否标志。
				boolean cIsSuc = isSuc;
				String cUid = userId;
				String cUname = userName;
				setUserData(cIsSuc, cUid, cUname, true);
			}
			
		}, param);
		
		return;
	}
	
	//FB的登出。
	public static void fbLogout()
	{
		//sdk未启动, 直接返回
		if (!TradeService.isStarted())
		{
			return;
		}
		
		Bundle param = new Bundle();
		param.putString(Actions.PARAM_SSO_LOGIN_TYPE, Actions.FACEBOOK_LOGIN);
		param.putString(Actions.PARAM_USERNAME, fbUserId);
		
		TradeService.execute(Actions.SSO_THIRDPARTY_LOGOUT, new ICallback()
		{
			@Override
			public void onResult(boolean isSuccess, Bundle param, Throwable error)
			{
				if (isSuccess)
				{
					Log.e(tag, "fbLogout Success!");
				}
				else
				{
					showToast(fbLogoutFailedNotice);
					Log.e(tag, "fbLogout Failed!");
				}
				
				final boolean cIsSuc = isSuccess;
				Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable()
				{
					@Override
					public void run()
					{
						fbLogoutNative(cIsSuc);
					}
				});
			}
			
		}, param);
		
		return;
	}
	
	//check Action,检测帐号是不是已经注册过了
	public static boolean checkUser(String userName)
	{
		//sdk未启动, 直接返回
		if (!TradeService.isStarted())
		{
			return false;
		}
		//重新初始化帐号是否存在
		isExist = true;
		if (userName.length() == 0)
		{
			Log.e(tag, "invalid userName!");
			return false;
		}
		Bundle param = new Bundle();
		param.putString(Actions.PARAM_USERNAME, userName);
		
		TradeService.execute(Actions.SSO_CHECK, new ICallback()
		{
			@Override
			public void onResult(boolean isSuccess, Bundle params, Throwable error)
			{
				if (isSuccess)
				{
					boolean isExists = params.getBoolean(Actions.RESULT_EXISTS);
					if (isExists)
					{
						Log.e(tag, "userName is exist!");
						return ;
					}
					else
					{
						Log.e(tag, "userName aviable!");
					}
					isExist = isExists;
				}
			}
			
		}, param);
		return isExist;
	}
	
	//Toast提示快捷调用。
	private static void showToast(String notice)
	{
		GameSanGuo.tryShowToast(notice);
	}
	
	//设置用户数据快捷调用。
	private static void setUserData(boolean isSuc, String uid, String uname, boolean isFb)
	{
		SgUserData ud = new SgUserData();
		ud.isSuccess = isSuc;
		ud.userId = uid;
		ud.userName = uname;
		ud.isFacebook = isFb;
		GameSanGuo.trySetUserData(ud);
	}
	
	//c++：在本处写入userId，userName，方便统一登陆处理。註冊/登入賬戶/FB登入賬戶這三項，通過傳入不同參數，統一在此处理。
	public static native void setUserDataNative(boolean isSuc, String uid, String uname, boolean isFb);
	
	//c++: FB登出。
	public static native void fbLogoutNative(boolean isSuc);
	
	//c++: 登陆游戏。
	public static native void loginGameNative(boolean isSuc, String uid, String uname);
	
	//c++: 快速登陆游戏。
	public static native void fastLoginGameNative(boolean isSuc, String uid, String uname);
	
	//c++: 显示一段提示文本。
	public static native void showToastNative(String text);
	
	//c++: 充值成功，通知服务器，求给予元宝。
    public static native void dealChargeResult(boolean isOk, String bId, int bNum);
}

