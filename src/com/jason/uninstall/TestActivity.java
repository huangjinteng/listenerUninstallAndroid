package com.jason.uninstall;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.widget.RelativeLayout;

public class TestActivity extends Activity{
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		RelativeLayout root = new RelativeLayout(this);
		root.setLayoutParams(new RelativeLayout.LayoutParams(-1, -1));
		root.setBackgroundColor(Color.RED);
		setContentView(root);
		Log.e("HJT", "TestActivity oncreate");

	}
}
