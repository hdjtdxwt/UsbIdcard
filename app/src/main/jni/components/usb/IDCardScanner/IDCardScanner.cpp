/*
 * IDCardScanner.cpp
 *
 *  Created on: 2015-3-11
 *      Author: liuzg
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "DecodePhoto.h"
#include "IDCardScanner_usb.h"
#include "Android_Log.h"
//#include "UsbChannel.h"
#include "IdCardUsbChannelFromJavaApi.h"
#include "common.h"
#include "base64.h"
UsbChannel *channel = NULL;
char* wltPath = NULL;
char* bmpPath = NULL;
//char *wltPath = "/sdcard/photo.wlt";
//char *bmpPath = "/sdcard/photo.bmp";
bool hasMCU = false;//usb二代证不需要经过mcu转发
#ifdef HASMCU
hasMCU=true;
#endif

#include "../decode_wlt64/getinfo.cpp"
#include "../decode_wlt64/include_lib.h"

char COMMANDDATA_RESET_SAM_V[10] = { -86, -86, -86, -106, 105, 0, 3, 16, 255,		-20 };
//char COMMANDDATA_RESET_SAM_V[10] = { -86, -86, -86, -106, 105, 0, 3, 16, 255,		236 };
char COMMANDDATA_FIND_CARD[10] = { -86, -86, -86, -106, 105, 0, 3, 32, 1, 34 };
char COMMANDDATA_SELECT_CARD[10] = { -86, -86, -86, -106, 105, 0, 3, 32, 2, 33 };
char COMMANDDATA_READ_CARD[10] = { -86, -86, -86, -106, 105, 0, 3, 48, 1, 50 };
char nations[][16] = { "解码错",		// 00
		"汉",			// 01
		"蒙古",			// 02
		"回",			// 03
		"藏",			// 04
		"维吾尔",		// 05
		"苗",			// 06
		"彝",			// 07
		"壮",			// 08
		"布依",			// 09
		"朝鲜",			// 10
		"满",			// 11
		"侗",			// 12
		"瑶",			// 13
		"白",			// 14
		"土家",			// 15
		"哈尼",			// 16
		"哈萨克",		// 17
		"傣",			// 18
		"黎",			// 19
		"傈僳",			// 20
		"佤",			// 21
		"畲",			// 22
		"高山",			// 23
		"拉祜",			// 24
		"水",			// 25
		"东乡",			// 26
		"纳西",			// 27
		"景颇",			// 28
		"柯尔克孜",		// 29
		"土",			// 30
		"达斡尔",		// 31
		"仫佬",			// 32
		"羌",			// 33
		"布朗",			// 34
		"撒拉",			// 35
		"毛南",			// 36
		"仡佬",			// 37
		"锡伯",			// 38
		"阿昌",			// 39
		"普米",			// 40
		"塔吉克",		// 41
		"怒",			// 42
		"乌孜别克",		// 43
		"俄罗斯",		// 44
		"鄂温克",		// 45
		"德昴",			// 46
		"保安",			// 47
		"裕固",			// 48
		"京",			// 49
		"塔塔尔",		// 50
		"独龙",			// 51
		"鄂伦春",		// 52
		"赫哲",			// 53
		"门巴",			// 54
		"珞巴",			// 55
		"基诺",			// 56
		"编码错",		// 57
		"其他",			// 97
		"外国血统"		// 98
		};
char OPState[][32] = { "SUCCESS_OPERATOR",  // 0
		"ERROR_FIND_CARD",   // 1
		"ERROR_SELECT_CARD", // 2
		"ERROR_READ_CARD",   // 3
		"DECODE_PHOTO_ERROR",   // 4
		"UNKNOWN_ERROR"      // 5
		};

int getPermission(int ye, int mon, int da) {
	int ret = 0;
	time_t now;
	struct tm *timenow;
	time(&now);
	timenow = localtime(&now);
	if ((timenow->tm_year + 1900) > (ye + 2000)) {
		return -1;
	} else if ((timenow->tm_year + 1900) == (ye + 2000)) {
		if ((timenow->tm_mon + 1) > mon) {
			return -1;
		} else if ((timenow->tm_mon + 1) == mon) {
			if ((timenow->tm_mday) >= da) {
				return -1;
			}
		}

	}
	LOGD("permi ret = %d", ret);
	return ret;
}
/**
 * 连接二代证扫描设备
 * 连接成功返回0
 * 连接失败返回-1
*/
int openIDCard_usb(int idCardType, char* deviceId, char* password) {
	LOGD(">> call openIDCard,idCardType=%d ",idCardType);

	int ret = -1;
//	if (getPermission(50, 8, 3) != 0) {
//		LOGD("GetPermission Fail!");
//		return -1;
//	} else {
//		LOGD("GetPermission Suc!");
//	}
	/*if (idCardType != type_usb) { // 非串口设备不能用此驱动
		LOGD("idCardType must be %d,input=%d", type_usb,idCardType);
		return -1;
	} */
	if (channel == NULL) {
		channel = new UsbChannel();
	}
	if (channel->OpenChannel(channel->Channel_ID_Scanner, deviceId, hasMCU)
			== 1) {
		ret = 0;
//		usleep(100000);
		if (hasMCU) {
			channel->Buzzer(1, 100);
		}
	} else {
		delete channel;
		channel = NULL;
		ret = -1;
	}
	LOGD("openIDCard ret:%d", ret);
	return ret;
}
/** 关闭与二代证设备的连接 */
int closeIDCard_usb() {
	LOGD(">> call closeIDCard()");
	int ret = 0;
	if (channel != NULL) {
		delete channel;
		channel = NULL;
	}
	LOGD("closeIDCard ret:%d", ret);
	return ret;
}
/** 获取各厂商银行卡读卡器组件的版本信息 */
int getIDCardVersion_usb(char* version) {
	LOGD(">> call getIDCardVersion()");
	char ver[5] = "001";
	if (strcpy(version, ver) != NULL) {
		return 0;
	}
	LOGD("getIDCardVersion 001");
	return -1;
}
/**
 * 初始化二代证扫描设备,清除缓存数据
 * 初始化成功，返回0
 * 初始化失败，返回-1
 */
int initialIDCard_usb() {
	LOGD(">> call initialIDCard()");
	int ret = -1;
	if (channel != NULL && channel->isConnected) {
		JNIEnv *env = channel->GetJNIEnv(true);
		jbyteArray cmd = env->NewByteArray(10);
		env->SetByteArrayRegion(cmd, 0, 10, (jbyte*) COMMANDDATA_RESET_SAM_V);
		channel->Write(cmd);
		if (channel->CheckResponse(1000) > 0) {
			channel->ReadResp();
			//if (receive[7] == 0 && receive[8] == 0 && receive[9] == 0x90) {
			if (channel->buffer[7] == 0 && channel->buffer[8] == 0 && channel->buffer[9] == 0x90) {
				ret = 0;
				channel->Buzzer(1, 50);
			}
		}else{
			LOGD("initialIDCard failed,CheckResponse()<0");
			ret=0;
		}
		env->DeleteLocalRef(cmd);
	}else{
		LOGD("initialIDCard failed,channel=NULL or channel->isConnected=false");
	}
	LOGD("initialIDCard ret:%d", ret);
	return ret;
}
/** 获得二代证扫描设备数据。 */
int getIdCardInfo_usb(jobjectArray cardInfo, char* img) {
	char *wlt = "/mnt/sdcard/photo.wlt";
	char *bmp = "/mnt/sdcard/photo.bmp";

	wltPath = (char*)malloc(200);
	bmpPath = (char*)malloc(200);
	memset(bmpPath,0,200);
	memset(wltPath,0,200);
	if (channel == NULL || !channel->isConnected) {
		LOGD("getIdCardInfo failed,channel=NULL or channel->isConnected=false");
		return -1;
	}
	int ret = -1;
	if ((ret = FindIDCard()) != 0) {
		/*free(wltPath);
		free(bmpPath);
		return ret;*/
	}
	if ((ret = SelectIDCard()) != 0) {
		/*free(wltPath);
		free(bmpPath);
		return ret;*/
	}
	if ((ret = ReadIDCard()) != 0) {
		if ((ret = ReadIDCard()) != 0) {
			return ret;
		}

	}
	LOGD("getIdCardInfo len: %d", channel->totalLen);
	if (channel->totalLen != 1295) {
		free(bmpPath);
		free(wltPath);
		return -1;
	}

	decodeIDCardData(cardInfo, NULL, &channel->buffer[0] + 7);
	LOGD("getIDCardInfo  decodeIDCardData complete");
	char* imageByte = (char*) malloc(1024);
	memset(imageByte, 0, 1024);
	int textLen = channel->buffer[10] * 256 + channel->buffer[11];
	LOGD("textLen=%d",textLen);
	int textLen2 = channel->buffer[0] +textLen+14;
	LOGD("textLen2=%d",textLen2);
	memcpy(imageByte, &channel->buffer[0] + textLen + 14, 1024);

	int saveWlt = -1;

	FILE *fp = fopen(wltPath, "wb+");
	if (fp) {
		if ((saveWlt = fwrite(imageByte, sizeof(char), 1024, fp)) > 0) {
			fclose(fp);
			LOGD("getIDCardInfo save wlt success!");
		} else {
			LOGD("getIDCardInfo save wlt failed!");
			fclose(fp);
			free(imageByte);

			free(bmpPath);
			free(wltPath);
			return -1;
		}
	} else {
		//下面的操作是针对某些手机没有那个路径，只能用当前应用的路径了  在新版的手机6.0上，某一些手机没有/mnt/sdcard/这么一个目录的，下面的操作就是针对那些手机的
		LOGE("--------来到else了，没有/mnt/sdcard那个目录");
		JNIEnv *env = channel->GetJNIEnv(true);
		jclass UsbHelper = env->FindClass("com/android/api/UsbHelper");
		if (UsbHelper != NULL) {
			jmethodID getCachePath = env->GetStaticMethodID(UsbHelper, "getCachePath", "()Ljava/lang/String;");
			jstring dirpath = (jstring)( env->CallStaticObjectMethod(UsbHelper,getCachePath));
			char* dir = jstringToCharWithCharset(env,dirpath,"UTF-8");
			int len = strlen(dir);
			LOGD("dir=%s   len=%d",dir,len);
			const char* wltName = "/photo.wlt";
			const char* bmpName = "/photo.bmp";
			int bmpNameLen = strlen(bmpName);
			//生成bmpPath
			char* finalBmpName = (char*) malloc(200);
			memset(finalBmpName,0,200);


			memcpy(finalBmpName, dir, len);
			memcpy(finalBmpName+len,bmpName, bmpNameLen);
			LOGD("finalBmpName=%s   ", finalBmpName);
			memcpy(bmpPath,finalBmpName,strlen(finalBmpName));


			//生成wltPath
			char* finalWltName = (char*) malloc(200);
			memset(finalWltName,0,200);
			memcpy(finalWltName, dir, len);
			memcpy(finalWltName+len,wltName,strlen(wltName));
			LOGD("finalWltName=%s   ", finalWltName);
			memcpy(wltPath,finalWltName,strlen(finalWltName));

			free(finalWltName);
			free(finalBmpName);
			LOGE("------1------bmpPathPath=%s",bmpPath);
			fp = fopen(wltPath, "wb+");
			if ((saveWlt = fwrite(imageByte, sizeof(char), 1024, fp)) > 0) {
				fclose(fp);
				LOGD("getIDCardInfo save wlt success!");
			} else {
				LOGD("getIDCardInfo save wlt failed!");
				fclose(fp);
				free(imageByte);

				free(bmpPath);
				free(wltPath);
				return -1;
			}

			//free(finalWltName);
			//free();
		}else{
			LOGD("getIDCardInfo open wlt path failed,wlt file not exist!");
			free(imageByte);
			return -1;
		}
	}
	if (saveWlt > 0) {
		//int ret = CallWlt2BmpBylib((char*)wltPath, (char*)bmpPath);//旧的写法，不支持64位的手机，
		LOGD("---wlt2bmp前---%xd    %s",imageByte[1],  bmpPath);
		int ret= wlt2bmp(imageByte, (char*)bmpPath);//新的写法，支持64位手机，修改于2017-8-5  by wangtao

		if (ret>0) {  // 图片解析成功,设置图片路径
			JNIEnv *env = channel->GetJNIEnv(true);
			jstring image = stringTojstring(env, bmpPath);
			env->SetObjectArrayElement(cardInfo, 8, (jobject) image);
			//			读取bmp
			env->DeleteLocalRef(image);
			size_t bytes_in;
			int fileSize = 0;
			int totallen = 0;

			LOGE("------bmpPath=%s",bmpPath);
			FILE *bmpFp = fopen(bmpPath, "r+");
			if (NULL == bmpFp) {
				LOGD("fail to open photo.bmp");
				return -1;
			} else {
				LOGI("open photo.bmp success");
			}
			fseek(bmpFp, 0L, SEEK_END);
			fileSize = ftell(bmpFp);
			//LOGD("图片文件大小：%d", fileSize);
			//			fclose(bmpFp);//如果不使用SEEK_SET,拷贝文件会失败
			//			bmpFp=fopen(bmpPath, "r+");
			fseek(bmpFp, 0L, SEEK_SET);
			char *src = (char*) malloc(fileSize);

			while ((bytes_in = fread(src + totallen, 1, fileSize, bmpFp)) > 0) {
				//				+ -数文件变小
				totallen = totallen + bytes_in;
				//LOGD("拷贝：%d", totallen);
			}
			//			strlen遇到第一个0就返回
			//LOGD("实际拷贝图片大小：%d", totallen);
			char *tem = base64_encode(src, totallen);
			//LOGD("编码：bmp_buf:%s", tem);
			memcpy(img, tem, strlen(tem));

			free(src);
			free(tem);
			fclose(bmpFp);
			free(imageByte);

			free(bmpPath);
			free(wltPath);
		} else {
			LOGD("getIDCardInfo convert wlt to bmp failed!");
			free(imageByte);

			//由于可能走了else，bmpPath和wltPath可能是malloc生成的，所以还是要free下
			free(bmpPath);
			free(wltPath);
			return -1;
		}

	}
	return 0;
}
/**
 * 查找证卡,返回0 表示成功,其余失败
 */
int FindIDCard() {
	LOGD("FindIDCard");
	usleep(500000);
	int ret = -1;
	if (channel == NULL || !channel->isConnected)
		return -1;
	JNIEnv *env = channel->GetJNIEnv(true);
	jbyteArray cmd = env->NewByteArray(10);
	env->SetByteArrayRegion(cmd, 0, 10, (jbyte*) COMMANDDATA_FIND_CARD);
	channel->Write(cmd);
	if (channel->CheckResponse() > 0) {

		channel->ReadResp();
		if (channel->buffer[7] == 0 && channel->buffer[8] == 0
				&& channel->buffer[9] == 0x9F) {
			ret = 0;
		} else if (channel->buffer[7] == 0 && channel->buffer[8] == 0
				&& channel->buffer[9] == 0x80) {
			ret = 1;
		} else {
			ret = 5;
		}
	}

	env->DeleteLocalRef(cmd);
	LOGD("FindIDCard ret:%d, %s", ret, OPState[ret]);
	return ret;
}
int SelectIDCard() {
	int ret = -1;
	if (channel == NULL || !channel->isConnected)
		return -1;
	JNIEnv *env = channel->GetJNIEnv(true);
	jbyteArray cmd = env->NewByteArray(10);
	env->SetByteArrayRegion(cmd, 0, 10, (jbyte*) COMMANDDATA_SELECT_CARD);
	channel->Write(cmd);
	if (channel->CheckResponse() > 0) {
		channel->ReadResp();
		if (channel->buffer[7] == 0 && channel->buffer[8] == 0
				&& channel->buffer[9] == 0x90) {
			ret = 0;
		} else if (channel->buffer[7] == 0 && channel->buffer[8] == 0
				&& channel->buffer[9] == 0x81) {
			ret = 2;
		} else {
			ret = 5;
		}
	}
	env->DeleteLocalRef(cmd);
	LOGD("SelectIDCard ret:%d, %s", ret, OPState[ret]);
	return ret;
}
int ReadIDCard() {
	int ret = -1;
	if (channel == NULL || !channel->isConnected)
		return -1;
	JNIEnv *env = channel->GetJNIEnv(true);
	jbyteArray cmd = env->NewByteArray(10);
	env->SetByteArrayRegion(cmd, 0, 10, (jbyte*) COMMANDDATA_READ_CARD);
	channel->Write(cmd);
	if (channel->CheckResponse() > 0) {
		channel->ReadResp();
		if (channel->buffer[7] == 0 && channel->buffer[8] == 0
				&& channel->buffer[9] == 0x90) {
			ret = 0;
		} else if (channel->buffer[7] == 0 && channel->buffer[8] == 0
				&& channel->buffer[9] == 0x41) {
			ret = 3;
		} else {
			ret = 5;
		}
	}
	LOGD("ReadIDCard  ret:%d, %s", ret, OPState[ret]);
	return ret;
}
/*
 * data 除去帧头(5 byte)、长度(2 byte) 字段的数据
 */
int decodeIDCardData(jobjectArray cardInfo, char* img, char * data) {

	int tempLen = 4 + 256 + 1024;
	int textLen = 0, wltLen = 0;
	char temp[tempLen]; // 除去帧头(5 byte)、长度(2 byte)和状态字段（3 byte）的数据
	memcpy(temp, data + 3, tempLen);

	textLen = temp[0] * 256 + temp[1];
	wltLen = temp[2] * 256 + temp[3];
	char *tmp = (char*) malloc(30);
	memset(tmp, 0, 30);
	memcpy(tmp, temp + 4, 30); // copy name
	JNIEnv *env = channel->GetJNIEnv(true);
	jstring nameString = byte2jstring(env, tmp, 30);
	nameString = trim(env, nameString);
	env->SetObjectArrayElement(cardInfo, 0, (jobject) nameString);
	free(tmp);
	tmp = NULL;

	tmp = (char*) malloc(2);
	memset(tmp, 0, 2);
	memcpy(tmp, temp + 34, 2); // copy sex
	jstring genderStr = NULL;
	LOGD("SEX: %d, %d", tmp[0], tmp[1]);
	if (tmp[0] == 49) {
		genderStr = stringTojstring(env, "男");
	} else {
		genderStr = stringTojstring(env, "女");
	}
	env->SetObjectArrayElement(cardInfo, 1, (jobject) genderStr);
	free(tmp);
	tmp = NULL;

	tmp = (char*) malloc(4);
	memset(tmp, 0, 4);
	memcpy(tmp, temp + 36, 4);  // copy nation
	jstring nationStr = byte2jstring(env, tmp,4);
	jclass Integer = env->FindClass("java/lang/Integer");
	jmethodID parseInt = env->GetStaticMethodID(Integer, "parseInt",
			"(Ljava/lang/String;)I");
	jint nationcode = env->CallStaticIntMethod(Integer, parseInt, nationStr);
	if (nationcode >= 1 && nationcode <= 56) {
		nationStr = stringTojstring(env, nations[nationcode]);
	} else if (nationcode == 97 || nationcode == 98) {
		nationStr = stringTojstring(env, nations[nationcode]);

	} else {
		nationStr = stringTojstring(env, "编码错");
	}
	env->SetObjectArrayElement(cardInfo, 2, (jobject) nationStr);
	free(tmp);
	tmp = NULL;
	tmp = (char*) malloc(16);
	memset(tmp, 0, 16);
	memcpy(tmp, temp + 40, 16);  // copy birthday
	jstring birthdayString = byte2jstring(env, tmp, 16);
	env->SetObjectArrayElement(cardInfo, 3, (jobject) birthdayString);
	free(tmp);
	tmp = NULL;

	tmp = (char*) malloc(70);
	memset(tmp, 0, 70);
	memcpy(tmp, temp + 56, 70);  // copy address
	jstring addressString = byte2jstring(env, tmp, 70);
//	addressString = trim(addressString);
	env->SetObjectArrayElement(cardInfo, 4, (jobject) addressString);
	LOGD("SEX:1");
	free(tmp);
	tmp = NULL;

	tmp = (char*) malloc(36);
	memset(tmp, 0, 36);
	memcpy(tmp, temp + 126, 36);  // copy idcardno

	jstring idCardNoString = byte2jstring(env, tmp, 36);
	env->SetObjectArrayElement(cardInfo, 5, (jobject) idCardNoString);
	free(tmp);
	tmp = NULL;

	tmp = (char*) malloc(30);
	memset(tmp, 0, 30);
	memcpy(tmp, temp + 162, 30);  // copy address
	jstring policeStationString = byte2jstring(env, tmp, 30);
	env->SetObjectArrayElement(cardInfo, 6, (jobject) policeStationString);
	free(tmp);
	tmp = NULL;

	char sign[6] = { 32, 0, 45, 0, 32, 0 };
	char *validData = (char*) malloc(38);  // IDCard valid data
	memset(validData, 0, 38);
	memcpy(validData, temp + 192, 16);
	memcpy(validData + 16, sign, 6);
	memcpy(validData + 22, temp + 208, 16);
	LOGD("valid date : %s",validData);

	jstring valiDataString = byte2jstring(env, validData, 38);
	env->SetObjectArrayElement(cardInfo, 7, (jobject) valiDataString);
	free(validData);
	validData = NULL;
	return 0;
}

