/*
 * DecodePhoto.cpp
 *
 *  Created on: 2015-3-12
 *      Author: liuzg
 */
#include "DecodePhoto.h"

#include "Android_Log.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
#include <jni.h>
#include "BitmapFile.h"
HMODULE hModuleWltRS = NULL;
Unpack unpackWltRS = 0;
extern JavaVM *mVM;
int unpack(char *src,char *dst,int bmpSave);
/*
int Loaddl() {
	char soPath[] = "./libWltRS.so";
	hModuleWltRS = (int*) dlopen(soPath, RTLD_NOW);
	if (hModuleWltRS != NULL) {
		unpackWltRS = (Unpack) dlsym(hModuleWltRS, "unpack");
		LOGW("DecodePhoto Loaddl() unpack=%x", unpackWltRS);
		return (unpackWltRS != NULL) ? 1 : 0;
	} else {
		LOGW("DecodePhoto Loaddl() faild ,hModule=NULL");
	}
	return 0;

}
int CallWlt2Bmp(char* wltPath, char* bmpPath) {
	int ret = -1;
	if (unpackWltRS != NULL) {
		ret = unpackWltRS(wltPath, bmpPath, 1);
	} else {
		if (Loaddl() > 0) {
			ret = unpackWltRS(wltPath, bmpPath, 1);
		}
	}
	LOGW("DecodePhoto CallWlt2Bmp ret:%d", ret);
	return ret;
}
*/
int CallWlt2BmpBylib(char* wltFilePath, char* bmpFilePath) {
	LOGD(">> call CallWlt2BmpBylib(),wltFilePath:%s,bmpFilePath", wltFilePath,bmpFilePath);
	JNIEnv *m_Env=NULL;
	mVM->GetEnv((void **) &m_Env, JNI_VERSION_1_2);
	mVM->AttachCurrentThread(&m_Env, NULL);
	jstring wltPath = m_Env->NewStringUTF(wltFilePath);
	jstring bmpPath = m_Env->NewStringUTF(bmpFilePath);

	const char* pszWltPath = NULL;
	const char* pszBmpPath = NULL;
	jboolean isCopy = JNI_FALSE;
	jint result;
	const int xsize = 102;
	const int ysize = 126;
	int i, j;
	//static int testindex = 1;
	char * testptr = NULL;

	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD rgbQuad;

	if (NULL == wltPath || NULL == bmpPath)
		return -11;
	do {
		pszWltPath = m_Env->GetStringUTFChars(wltPath, &isCopy);
		if (NULL == pszWltPath) {
			result = -14;
			break;
		}
		pszBmpPath = m_Env->GetStringUTFChars(bmpPath, &isCopy);
		if (NULL == pszBmpPath) {
			result = -14;
			break;
		}
		LOGD("CallWlt2BmpBylib(),begin read wlt file.");

		// ????wlt

		char * wlt = (char *) malloc(1024);
		char * bmp = (char *) malloc(xsize * ysize * 3);
		FILE * fp = fopen(pszWltPath, "r+b");
		if (fp != NULL) {
			fread(wlt, sizeof(char), 1024, fp);
			fclose(fp);
		} else {
			result = -15;
			free(wlt);
			free(bmp);
			LOGE("CallWlt2BmpBylib(),wlt file open failed.");
			break;
		}
		LOGD("CallWlt2BmpBylib(),read wlt file end.");
		result = unpack(wlt, bmp, 0);
		LOGD("CallWlt2BmpBylib(),unpack complete!unpack result = %d", result);

		//result = 1;
		//	 testindex++;
		//	 if (testindex>=3)
		//	 {
		//	 testptr = (char*)0xfefefefc;
		//	 LOGE("Fatal test begin!testindex=%d",testindex);
		//	 memset(testptr, 0x00, 10);
		//	 }
		//	 result = 1;
		if (result == 1) {
			bmfHeader.bfType = 0x4D42;
			bmfHeader.bfSizeL = (102 * 3 + 2) * 126 + 54;
			bmfHeader.bfReserved1 = 0x0000;
			bmfHeader.bfReserved2 = 0x0000;
			bmfHeader.bfOffBitsL = 54;

			bmiHeader.biSize = 40;
			bmiHeader.biWidth = 102;
			bmiHeader.biHeight = 126;
			bmiHeader.biPlanes = 1;
			bmiHeader.biBitCount = 24;
			bmiHeader.biCompression = 0;
			bmiHeader.biSizeImage = 0;
			bmiHeader.biXPelsPerMeter = 0;
			bmiHeader.biYPelsPerMeter = 0;
			bmiHeader.biClrUsed = 0;
			bmiHeader.biClrImportant = 0;

			rgbQuad.rgbBlue = 0x00; //0xCE;
			rgbQuad.rgbGreen = 0x00;
			rgbQuad.rgbRed = 0x00;

			//fp = fopen("/mnt/sdcard/photo.bmp", "w+b");
			fp = fopen(pszBmpPath, "w+b");
			if (fp != NULL) {
				fwrite(&bmfHeader, 14, 1, fp);
				fwrite(&bmiHeader, 40, 1, fp);
				for (j = 0; j < ysize; j++) {
					for (i = 0; i < xsize; i++) {
						fwrite(bmp + j * xsize * 3 + i * 3 + 2, 1, 1, fp);
						fwrite(bmp + j * xsize * 3 + i * 3 + 1, 1, 1, fp);
						fwrite(bmp + j * xsize * 3 + i * 3 + 0, 1, 1, fp);
					}
					fwrite(&rgbQuad, 1, 2, fp);
				}
				fclose(fp);
				LOGD("CallWlt2BmpBylib(),write bmp file %s", pszBmpPath);
			}
		}
		free(wlt);
		free(bmp);
	} while (/*FALSE*/0);
	if (NULL != pszWltPath)
		m_Env->ReleaseStringUTFChars(wltPath, pszWltPath);
	if (NULL != pszBmpPath)
		m_Env->ReleaseStringUTFChars(bmpPath, pszBmpPath);
	LOGD("CallWlt2BmpBylib() end,result= %d", result);
	return result;

}
