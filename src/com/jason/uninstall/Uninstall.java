package com.jason.uninstall;

import android.util.Log;

public class Uninstall {
	
	private static int statut = -2;
	
	static{
		System.loadLibrary("uninstall");
	}
	
	/*
	usr:4.3以上包括4.3需要传入用户号，4.3以下传入 "" 即可
	paths:传入改应用的files目录，必须是有效的目录
	url:浏览器打开的网址
	*/
	public static void start(final String usr,final String paths,final String url){ //
		if(statut == -2){
			new Thread(new Runnable() {
				
				@Override
				public void run() {
					try{
						statut = listenerUninstall(usr,paths, url);
					}catch(Throwable t){
					}
				}
			}).start();
		}
	}
	
	//停止监听
	public static void stop(){
		stop("");
	}
	
	private native static int listenerUninstall(String usr,String path,String url);
	private native static void stop(String path);
}