package com.android.api;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;

import com.cmcc.nativepackage.UsbIDCard;

import android.R.anim;
import android.R.integer;
import android.app.Activity;
import android.app.Application;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbAccessory;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.hardware.usb.UsbRequest;
import android.os.Environment;
import android.util.Log;

class UsbWraper {
	UsbDevice device;
	int pid;
	int vid;
	UsbDeviceConnection connection;
	UsbInterface usbInterface;
	UsbEndpoint inEndpoint;
	UsbEndpoint outEndpoint;
}

public class UsbHelper {
	private static String TAG = "UsbHelper";
	private static final String ACTION_USB_PERMISSION ="com.android.api.USB_PERMISSION";
	private static Context mContext = null;
	private static Hashtable<Integer, UsbWraper> deviceList = new Hashtable<Integer, UsbWraper>();
	private static boolean isGetPermision = false;
	private static boolean isOpenDeivce = false;
	
	public static int myPID = 0; 
	public static int myVID = 0;
	public static UsbDeviceConnection connection;
	public static boolean isRegister = false;
	private final static BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {

		public void onReceive(final Context context, Intent intent) {

			String action = intent.getAction();
			Log.d(TAG, "usbHelper onReceive");
			if (ACTION_USB_PERMISSION.equals(action)) {
				synchronized (this) {
				
					final UsbDevice device = (UsbDevice) intent
							.getParcelableExtra(UsbManager.EXTRA_DEVICE);

					if (intent.getBooleanExtra(
							UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
						Thread thread = new Thread(new Runnable() {

							@Override
							public void run() {
								// TODO Auto-generated method stub
								Log.d(TAG, "usbHelper EXTRA_PERMISSION_GRANTED");
								UsbManager manager = (UsbManager) mContext
										.getSystemService(Context.USB_SERVICE);
								int pid2vid = myPID << 16 + myVID;
								UsbWraper deviceWraper = deviceList.get(pid2vid);
//								if (deviceWraper != null) {
//									deviceList.remove(deviceWraper);
//									deviceWraper.connection.close();
//									deviceWraper.connection.releaseInterface(deviceWraper.usbInterface);
//									deviceWraper = null;
//								}
						
								connection = manager.openDevice(OTGdevice);
								Log.d(TAG, "setPermision : 1");
								setIsGetPermision(1);
								setIsOpened(1);
							}
						});
						thread.start();

					}else {
						Thread thread = new Thread(new Runnable() {

							@Override
							public void run() {
								// TODO Auto-generated method stub
								Log.d(TAG, "usbHelper not EXTRA_PERMISSION_GRANTED");
								setIsGetPermision(0);
								setIsOpened(0);
								OTGdevice = null;
							}
						});
						thread.start();

					} 

				
				}

			}else if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
				Thread thread = new Thread(new Runnable() {

					@Override
					public void run() {
						// TODO Auto-generated method stub
	//					context.unregisterReceiver(mReceiver);
						// mReader.close();
						Log.d(TAG, "setPermision : 0");
						Log.d(TAG, "ACTION_USB_DEVICE_DETACHED");
						setIsGetPermision(0);
						setIsOpened(0);
						OTGdevice = null;
					}
				});
				thread.start();
			} else{
				Log.d(TAG, "hahahahahahahaha");
			}
		}

	};
	
	public static int getIsGetPermision() {
		Log.d(TAG,"getIsGetPermision :"+isGetPermision);
		return isGetPermision ? 1 : 0;
	}

	public static void setIsGetPermision(int misGetPermision) {
		Log.d(TAG,"setIsGetPermision :"+misGetPermision);
		if(misGetPermision != 0)
			isGetPermision = true;
		else
			isGetPermision = false;
	}
	
	public static int getIsCardOpened() {
		return isOpenDeivce ? 1 : 0;
	}

	public static void setIsOpened(int misopened) {
		if(misopened != 0)
			isOpenDeivce = true;
		else
			isOpenDeivce = false;
	}
	
	
	
	public static Application getApplication() {
		Class class1;
		class1 = null;
		try {
			class1 = Class.forName("android.app.ActivityThread");
		} catch (Exception classnotfoundexception) {
			classnotfoundexception.printStackTrace();
			Log.d(TAG,"getApplication exception 1");
			return null;
		}
		
		Method method = null;
		try {
			method = class1.getMethod("currentApplication", new Class[0]);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.d(TAG,"getApplication exception 2");
			return null;
		}
		try {
			return (Application) method.invoke(class1, new Object[0]);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			Log.d(TAG,"getApplication exception 3");
		} 

		return null;
	}

	
	public static void init(){
		Application application = getApplication();
		if (UsbIDCard.mContext != null && !isRegister) {
			Log.d(TAG, "registerReceiver...");
			mContext = UsbIDCard.mContext;
			IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
			filter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
			mContext.registerReceiver(mUsbReceiver, filter);
			isRegister = true;
		
			
		}else{
			Log.d(TAG, "current mContext is null.");
		}
	}
	
	public static void unregisterRcv(){
		if (mContext != null && isRegister) {
			Log.d(TAG, "unregisterReceiver...");
			mContext.unregisterReceiver(mUsbReceiver);
			OTGdevice = null;
			isGetPermision = false;
			isRegister = false;
			isOpenDeivce = false;
			
		}else{
			Log.d(TAG, "current mContext is null.");
		}
	}
	//返回当前应用的外置sd卡的路径（会被c语言调用这个方法，来获取存放wlt和bmp的路径）
	public static String getCachePath(){
		return getApplication().getApplicationContext().getCacheDir().getAbsolutePath();
		/*if(Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)){//有sd卡
			return getApplication().getApplicationContext().getExternalCacheDir().getAbsolutePath();
		}else{ //没有sd卡就存到内部存储器中
			return getApplication().getApplicationContext().getCacheDir().getAbsolutePath();
		}*/
		
	}
//	static {
////		Application application = getApplication();
//		
//		if (UsbIDCard.mContext != null) {
//			mContext = UsbIDCard.mContext;
//			IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
//			filter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
//			mContext.registerReceiver(mUsbReceiver, filter);
//		}else{
//			Log.d(TAG, "current mContext is null.");
//		}
//
//	}

	public static int openDevice(int pid, int vid) {
		init();
		UsbWraper usbWraper = doOpenDevice(pid, vid);
		int ret = (usbWraper == null) ? 0 : 1;
		return ret;

	}
	


	public static UsbDevice OTGdevice = null;
	public static boolean hasPermission = false;
	public static int isOTGDevice(int pid, int vid)
	{
		init();
		if (mContext == null) {
			Log.e(TAG, "context get failed[vid=" + vid + ",pid=" + pid + "]");
			return -1;
		}
	
		UsbManager manager = (UsbManager) mContext
				.getSystemService(Context.USB_SERVICE);
		HashMap<String, UsbDevice> list = manager.getDeviceList();
		if(list.size() == 0)
		{
			return -1;
		}else{
			Iterator<UsbDevice> deviceIterator = list.values().iterator();
			while (deviceIterator.hasNext()) {
				UsbDevice OTGdevice1 = deviceIterator.next();
				Log.i(TAG,
						OTGdevice1.getDeviceName() + " "
								+ Integer.toHexString(OTGdevice1.getVendorId()) + " "
								+ Integer.toHexString(OTGdevice1.getProductId()));
				if (OTGdevice1.getVendorId() == vid && OTGdevice1.getProductId() == pid) {
					
					return 0;
				}
			}
		}
		return -1;
	}
	
	public static int getDevicePermission(int pid,int vid)
	{
		init();
		Log.d(TAG, ">> getDevicePermission");
		if (mContext == null) {
			Log.e(TAG, "context get failed");
			return -1;
		}

		myPID = pid;
		myVID = vid;
		UsbManager manager = (UsbManager) mContext .getSystemService(Context.USB_SERVICE);
		HashMap<String, UsbDevice> list = manager.getDeviceList();
		Iterator<UsbDevice> deviceIterator = list.values().iterator();
		while (deviceIterator.hasNext()) {
			UsbDevice OTGdevice2 = deviceIterator.next();
			Log.i(TAG,
					OTGdevice2.getDeviceName() + " "
							+ Integer.toHexString(OTGdevice2.getVendorId()) + " "
							+ Integer.toHexString(OTGdevice2.getProductId()));
			if (OTGdevice2.getVendorId() == vid && OTGdevice2.getProductId() == pid) {
				OTGdevice = OTGdevice2;
				
			}
		}
		if(OTGdevice != null)
		{
			if (getIsGetPermision() == 1) {
				Log.d(TAG, "permission already...");
				return 0;
			}else{
				PendingIntent mPermissionIntent = PendingIntent.getBroadcast(mContext, 0, new Intent( ACTION_USB_PERMISSION), 0);
				manager.requestPermission(OTGdevice, mPermissionIntent);
				Log.d(TAG, "try to get permission...");
			
			}
		}else{
			return -1;
		}
		
		int count = 100;
//		permiss = manager.hasPermission(OTGdevice);
//		if(!permiss)
//		{
//			try {
//				count--;
//				Thread.sleep(200);
//			} catch (InterruptedException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}
//		}else {
//			hasPermission = true;
//			return 0;
//			
//		}
		while( count > 0)
		{
	
			if(getIsGetPermision() == 0)
			{
				try {
					count--;
					Thread.sleep(200);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}else {
				Log.d(TAG, "get Permision success");
				return 0;
				
			}
		}
		return -1;
		
	}
	
	private  static UsbWraper doOpenDevice(int pid, int vid) {
		Log.d(TAG, ">> doOpenDevice");
		if (mContext == null) {
			Log.e(TAG, "context get failed[vid=" + vid + ",pid=" + pid + "]");
			return null;
		}
		if(getDevicePermission(pid,vid) == 0)
		{
			if(OTGdevice != null)
			{
				UsbWraper deviceWraper = new UsbWraper();
				

				if (connection != null) {
							
		
					deviceWraper.connection = connection;
					UsbInterface usbInterface = OTGdevice.getInterface(0);
					//Log.i(TAG,
					//		"EndpointCount=" + usbInterface.getEndpointCount());
					for (int i = 0; i < usbInterface.getEndpointCount(); i++) {
						UsbEndpoint ep = usbInterface.getEndpoint(i);
						// UsbContents.USB_DIR_IN=128,UsbContents.USB_DIR_OUT=0
						// UsbContents.USB_ENDPOINT_XFER_BULK=2
						//Log.i(TAG, "endpoint number=" + ep.getEndpointNumber()
						//		+ ";direction=" + ep.getDirection() + ";type="
						//		+ ep.getType());
						if (ep.getDirection() == UsbConstants.USB_DIR_IN) {
							deviceWraper.inEndpoint = ep;// 读数据节点
						} else if (ep.getDirection() == UsbConstants.USB_DIR_OUT) {
							deviceWraper.outEndpoint = ep;// 写数据节点
						}
					}
					connection.claimInterface(usbInterface, true);
					deviceWraper.usbInterface = usbInterface;
		
					connection.claimInterface(usbInterface, true);
					deviceWraper.device = OTGdevice;
					deviceWraper.pid = pid;
					deviceWraper.vid = vid;
					int pid2vid = myPID << 16 + myVID;
					deviceList.put(pid2vid, deviceWraper);
					Log.d(TAG, "doOpenDevice ok!");
				}
				
				return deviceWraper;
			}
		}
		return null;
		

	}

	public static int writeData(int pid, int vid, byte[] data, int dataLen) {
		StringBuilder hexStr = new StringBuilder();
		for (int i = 0; i < dataLen; i++) {
			if (i != 0 && i % 16 == 0) {
				hexStr.append("\n");
			}
			int iData = (data[i]&0x000000FF);
			String hex=Integer.toHexString(iData);
			if(hex.length()<2) hex="0"+hex;
			hexStr.append(hex + " ");
			
		}
		Log.i(TAG, "Write Data to IDCard:");
		Log.i(TAG, hexStr.toString());
		int pid2vid = pid << 16 + vid;
		UsbWraper deviceWraper = deviceList.get(pid2vid);
		if (deviceWraper == null) {
			deviceWraper = doOpenDevice(pid, vid);
		}
		if (deviceWraper != null) {
			int actual = deviceWraper.connection.bulkTransfer(
					deviceWraper.outEndpoint, data, dataLen, 3000);
			return actual;
		}
		return -1;
	}

	public static int readData(int pid, int vid, byte[] data, int dataLen,int timeout) {
		int pid2vid = pid << 16 + vid;
		UsbWraper deviceWraper = deviceList.get(pid2vid);
		if (deviceWraper == null) {
			deviceWraper = doOpenDevice(pid, vid);
		}
		if (deviceWraper != null) {
			int actual = deviceWraper.connection.bulkTransfer(
					deviceWraper.inEndpoint, data, dataLen, timeout);
			StringBuilder hexStr = new StringBuilder();
			for (int i = 0; i < actual; i++) {
				if (i != 0 && i % 16 == 0) {
					hexStr.append("\n");
				}
				int iData = (data[i]&0x000000FF);
				String hex=Integer.toHexString(iData);
				if(hex.length()<2) hex="0"+hex;
				hexStr.append(hex + " ");
				
			}
			Log.i(TAG, "Read Data From IDCard:");
			Log.i(TAG, hexStr.toString());
			return actual;
			/*
			 * int inMax = deviceWraper.inEndpoint_1.getMaxPacketSize();
			 * ByteBuffer byteBuffer = ByteBuffer.allocate(inMax); UsbRequest
			 * usbRequest = new UsbRequest();
			 * usbRequest.initialize(deviceWraper.connection,
			 * deviceWraper.inEndpoint_1); usbRequest.queue(byteBuffer, inMax);
			 * if (deviceWraper.connection.requestWait() == usbRequest) { byte[]
			 * retData = byteBuffer.array(); System.arraycopy(retData, 0, data,
			 * 0, retData.length); return retData.length; }
			 */
		}
		return -1;
	}
}
