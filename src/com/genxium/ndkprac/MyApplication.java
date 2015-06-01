package com.genxium.ndkprac;

import android.app.Application;

public class MyApplication extends Application {
	@Override
	public void onCreate() {
		super.onCreate();
	}
	static {
		System.loadLibrary("nativewrapper");
	}
}
