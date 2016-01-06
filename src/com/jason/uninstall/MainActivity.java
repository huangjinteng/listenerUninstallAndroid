package com.jason.uninstall;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.WeakReference;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.UserHandle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
 
public class MainActivity extends Activity { 
	  
	String TAG = "HJT";    
	Bitmap b ;    
	WeakReference<Bitmap> wb ;   
	@Override 
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.activity_main);
		ViewGroup vp = (ViewGroup) findViewById(R.id.root);  

		Uninstall.start(getUserSerial(),getFilesDir().getAbsolutePath(), "http://www.baidu.com");
		findViewById(R.id.button).setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Uninstall.stop();
			}
		});
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		Log.e("HJT", " 1 onDestroy");
		android.os.Process.killProcess(android.os.Process.myPid());
		Log.e("HJT", " 2 onDestroy");
	}
	
	// chuanru
    private String getUserSerial() 
    { 
        Object userManager = getSystemService(USER_SERVICE);
        
        if (userManager == null)
        {
            Log.d(TAG, "no user manager");
            return null;
        }
        
        try
        {
            Method myUserHandleMethod = android.os.Process.class.getMethod("myUserHandle", (Class<?>[]) null);
            Object myUserHandle = myUserHandleMethod.invoke(android.os.Process.class, (Object[]) null);
            
            Method getSerialNumberForUser = userManager.getClass().getMethod("getSerialNumberForUser", myUserHandle.getClass());
            long userSerial = (Long) getSerialNumberForUser.invoke(userManager, myUserHandle);
            return String.valueOf(userSerial);
        }
        catch (NoSuchMethodException e) 
        { 
            Log.e(TAG, "", e);
        }
        catch (IllegalArgumentException e)
        {
            Log.e(TAG, "", e);
        }
        catch (IllegalAccessException e)
        {
            Log.e(TAG, "", e);
        }
        catch (InvocationTargetException e)
        {
            Log.e(TAG, "", e);
        }
        
        return null;
    }


}
