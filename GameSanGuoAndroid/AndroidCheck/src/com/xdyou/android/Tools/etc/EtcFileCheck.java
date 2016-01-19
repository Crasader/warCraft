package com.xdyou.android.Tools.etc;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import com.xdyou.android.Tools.R;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.ETC1Util;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

/**
 * 
 * @author bugcode
 *
 */
public class EtcFileCheck 
{
	private static String TAG="ETC_FILE_CHECK";
	private static Activity context=null;
	private static AssetManager assetManager = null;
	private static boolean isRunning=false;
	
	private static  Handler scaningHandler;
	
	private static Handler itemListHandler;
	
	/**
	 * 获得所有的PKM文件列表
	 * @return
	 */
	public static void checkEtcFiles()
	{
		if(!isRunning)
		{
			isRunning=true;
			Thread thread=new MyThread();
			thread.start();
//			try
//			{
//				
//				String[] files=assetManager.list("");
//				for(String file:files)
//				{
//					readEtcFileName(file);
//				}
//			}
//			catch(Exception e)
//			{
//				e.printStackTrace();
//			}
		}
	}
	static class MyThread extends Thread
	{
		
		@Override
		public void run() {
			try
			{
				
				String[] files=assetManager.list("");
				for(String file:files)
				{
					readEtcFileName(file);
				}
			}
			catch(Exception e)
			{
				e.printStackTrace();
			}
		}
	}
	/**
	 * 核心的迭代函数，进行检测
	 * @param fileName
	 * @param fileList
	 * @return
	 */
	private static void readEtcFileName(String fileName)
	{
		ETC1Util.ETC1Texture texture = null;
		InputStream inputStream = null;
		try
		{
			if(isFolder(fileName))
			{
//				Log.d(TAG, "读取文件夹="+fileName);
				Message msg=new Message();
				msg.obj=fileName;
				scaningHandler.sendMessage(msg);
				String[] files=assetManager.list(fileName);
				for(String str:files)
				{
					String strFileName=fileName+"/"+str;
					readEtcFileName(strFileName);
				}
			}
			else
			{
				if(fileName.toLowerCase().endsWith(".pkm"))
				{
//					Log.d(TAG, "正在检测的文件="+fileName);
//					textView.setText(fileName);
					Message msg=new Message();
					msg.obj=fileName;
					scaningHandler.sendMessage(msg);
					try
					{
						inputStream = assetManager.open(fileName);
						texture = ETC1Util.createTexture(inputStream);
						inputStream.close();
						if(texture==null)
						{
							Log.e(TAG, "生成了空的纹理."+fileName);
							Message msg1=new Message();
							msg1.obj=fileName;
							itemListHandler.sendMessage(msg1);
						}
					}
					catch(Exception e)
					{
						e.printStackTrace();
						Log.e(TAG, "检测到无法加载的文件."+fileName);
						Message msg2=new Message();
						msg2.obj=fileName;
						itemListHandler.sendMessage(msg2);
					}
				}
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
	}
	
	/**
	 * 判断一个文件是否是文件夹
	 * @param fileFullPath
	 * @return
	 */
	private static boolean isFolder(String fileFullPath)
	{
		try
		{
			String[] files=assetManager.list(fileFullPath);
			if(files.length>0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
			return false;
		}
	}
	
	public static void setContext(Context c,Handler scan,Handler itemList)
	{
		context=(Activity)c;
		assetManager=context.getAssets();
		scaningHandler=scan;
		itemListHandler=itemList;
//		textView=(TextView)context.findViewById(R.id.textView3);
//		listView=(ListView)context.findViewById(R.id.listView1);
//		errorList.add("test1");
//		adapter=new ArrayAdapter<String>(context,android.R.layout.simple_list_item_1, errorList);
//		listView.setAdapter(adapter);
	
		
	}

}
