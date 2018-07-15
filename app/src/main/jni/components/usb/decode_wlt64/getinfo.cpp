//
// Created by 段碧伟 on 2017/7/14.
//

#include <malloc.h>
#include <string.h>
#include "getinfo.h"
#include "include_lib.h"
#include "savebmp.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <linux/tty.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <Android_Log.h>
#include <string.h>
const unsigned char BMPHEADER[]	= { 0x42, 0x4d, 0xce, 0x97, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                       0x36, 0x00, 0x0, 0x0, 0x28, 0x0,
                                       0x0, 0x0, 0x66, 0x0, 0x0, 0x0, 0x7e, 0x0,
                                       0x0, 0x0, 0x01, 0x0, 0x18, 0x0, 0x0
};
const size_t LENGTH_OF_WLT	= 1024;
const size_t LENGTH_OF_BMP	= 38862;
const size_t LENGTH_PATH	= 256;
const size_t LENGTH_OF_LIC	= 30;


int wlt2bmp(char* pBufWlt, char szPath[])
{
    int iResult = 0;

    size_t cntRead = 0;

    char* pBuffer = NULL;
    char* pBufBmp2 = NULL;
    pBuffer = (char*)malloc(LENGTH_OF_BMP);
    pBufBmp2 = (char*)malloc(LENGTH_OF_BMP);
    if (pBuffer)
    {
        memset(pBuffer, 0, LENGTH_OF_BMP);
    }
    else
    {
        return 7348;
    }

    if (pBufBmp2)
    {
        memset(pBufBmp2, 0, LENGTH_OF_BMP);
    }
    else
    {
        return 7348;
    }
    int iRet = 0, iErr = 0;

    cntRead = LENGTH_OF_WLT;

    if (cntRead == 0)
    {
        iErr = errno;
        return iErr;
    }

    LOGE("---1-------wlt2bmp");
    LOGE("----pBufWlt[0]=%xd   pBufWlt[1]=%xd   pBufWlt[2]=%xd  pBufWlt[6]=%xd ",pBufWlt[0],pBufWlt[1],pBufWlt[2],pBufWlt[6] );
    if (pBufWlt[0] != 0x57 || pBufWlt[1] != 0x4c || pBufWlt[2] != 0x66 || pBufWlt[6] != 0x32)
        return 7347;

    memcpy(pBufBmp2, BMPHEADER, 31);

    LOGE("---2-------wlt2bmp");
    if (iRet == 0)
    {
    	//这个方法的声明在include_lib.h中，真实的实现已经放到了libidcard.a中去
        iResult = unpack(pBufWlt, pBuffer, 0);

        if (iResult == 1)
        {
            int cntWrote = LENGTH_OF_BMP;
            //这个方法在savebmp.cpp里
            int res = raw2bmp(pBuffer, pBufBmp2 + 54); // convert to BMP format ...
        }
    }
    if (pBuffer)
        free(pBuffer);

    writeFile(szPath, pBufBmp2, LENGTH_OF_BMP, 1);
    if (pBufBmp2)
        free(pBufBmp2);

    return iResult;
}

int writeFile(const char szPath[], char* pBuf, size_t cnt, int bBinary)
{
    FILE *f;
    int iRet = 0, iErr = 0, cntWrote = 0;
    char szModeB[] = "wb";
    char szModeC[] = "w";
    char *pMode = NULL;

    if (bBinary)
        pMode = szModeB;
    else
        pMode = szModeC;

    if((f = fopen (szPath, pMode)) == NULL)
    {
        iErr = errno;
        return iErr;
    }

    if (pBuf == NULL || cnt == 0)
    {
        return -1;
    }

    cntWrote = fwrite(pBuf, sizeof(char), cnt, f);
    if (!cntWrote)
    {
        iErr = errno;
        LOGE("----------writeFile");
//        __android_log_print(ANDROID_LOG_INFO,"Gzidcard","  fwrite  iErr = %d\n",iErr);
        return iErr;
    }

    fclose(f);

    iRet = !(cntWrote == cnt);
    LOGE("----------writeFile   %s",iRet);
    return iRet;

}
