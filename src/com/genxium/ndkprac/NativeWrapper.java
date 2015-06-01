package com.genxium.ndkprac;

public class NativeWrapper {
	private static NativeWrapper s_instance = null;
	public static NativeWrapper getInstance() {
		if (s_instance != null) return s_instance;
		s_instance = new NativeWrapper();
		return s_instance;
	}
	public native int add(int a, int b);
	public native void start();
}
