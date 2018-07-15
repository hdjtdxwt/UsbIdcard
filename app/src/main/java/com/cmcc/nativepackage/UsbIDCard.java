package com.cmcc.nativepackage;

import com.android.api.UsbHelper;

import android.R.integer;
import android.content.Context;

/**
 * @author 作者 E-mail: WangJC
 * @version 创建时间：2013-10-12 上午9:43:55 <br>
 *          类说明 二代证身份识别设备
 *          <p>so库文件名:CMCC_IDCARD_厂家英文名缩写_厂家设备型号缩写.so
 */
public class UsbIDCard {
	public static Context mContext;
	static {
		//System.loadLibrary("WltRS");
		System.loadLibrary("CMCC_USB_IDCARD");
		
		
	}

	/**
	 * <b><i>public static native int openIDCard(int idCardType, String
	 * deviceId, String password)</i></b>
	 * <p>
	 * 连接二代证身份识别设备<br>
	 * password是为了将来可能出现的wifi设备预留的参数。
	 * 
	 * @param idCardType
	 *            二代证身份识别设备类型：<i>idCardType
	 *            1：USB二代证身份识别设备(包括USB智能终端外设一体机中的二代证身份识别设备)
	 *            2：蓝牙二代证身份识别设备（包括蓝牙智能终端外设一体机中的二代证身份识别设备）
	 *            3：串口二代证身份识别设备（包括串口智能终端外设一体机中的二代证身份识别设备）
	 *            4、内置二代证身份识别设备（包括智能终端一体机中的二代证身份识别设备） </i>
	 * @param deviceId
	 *            设备ID：<i>当idCardType 为2时 deviceId为蓝牙二代证身份识别设备MAC地址
	 *            当idCardType为3时 deviceId为USB 卡(线)转换的串口，如/dev/ttyUSB0。 </i>
	 * @param password
	 *            密码：<i>预留字段</i>
	 * @return <li>0－>Success；状态码－> Fail
	 */
	public static native int openIDCard(int idCardType, String deviceId,
			String password);
	
//	public static int openIDCard(Context context,int idCardType, String deviceId,
//			String password){
//		mContext = context;
//		int ret = openIDCard(idCardType,deviceId,password);
//		return ret;
//	}

	/**
	 * <b><i>public static native int closeIDCard()</i></b>
	 * <p>
	 * 关闭与二代证身份识别设备的连接<br>
	 * 
	 * @return <li>0－>Success；状态码－> Fail
	 */
	public static native int closeIDCard();

	/**
	 * <b><i>public static native int getIDCardVersion(byte[] version)</i></b>
	 * <p>
	 * 获取各厂商二代证身份识别设备组件的版本信息。<br>
	 * 
	 * @param version
	 *            版本号：<i>各厂商二代证身份识别设备组件的版本号(三位整数)，数值越大代表版本越高。 </i>
	 * @return <li>0－>Success；状态码－> Fail
	 */
	public static native int getIDCardVersion(byte[] version);
	
	/**
	 * <b><i>public static native int initialIDCard ()</i></b>
	 * <p>
	 * 初始化二代证身份识别设备，清除二代证身份识别设备内缓存数据。<br>
	 * 
	 * @return <li>0－>Success；状态码－> Fail
	 */
	public static native int initialIDCard ();
	
	/**
	 * <b><i>public static native int getIdCardInfo(String[] idCardInfo,byte[] img)</i></b>
	 * <p>
	 * 获得二代证信息。<br>
	 * 
	 * @param idCardInfo
	 *            二代证基本信息：<i>数组长度为9，内容按顺序分别为：姓名、性别、民族、出生年月、地址、公民身份号码、签发机关、有效期限，照片保存地址。 </i>
	 * @param img
	 *            照片文件内容：<i>证件头像图片BASE64编码 </i>
	 *            
	 * @return <li>0－>Success；状态码－> Fail
	 */
	public static native int getIdCardInfo(String[] idCardInfo,byte[] img);
	
	public static native int isOTGDevice();
		
	public static native int getDevicePermission();
	
	public static void setContext(Context context){
		mContext = context;
	}
	
	public static void unregisterR(){
		UsbHelper.unregisterRcv();
	}

}
