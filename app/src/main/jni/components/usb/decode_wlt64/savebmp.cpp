// for RAW2BMP ...
#include <string.h>
#include "savebmp.h"

typedef unsigned long  LONG;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned char  UCHR;
typedef char BYTE;
typedef struct	tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER;

typedef struct  tagBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagRGBQUAD {
    BYTE rgbBlue;// 蓝色的亮度（值范围为0-255)
    BYTE rgbGreen; // 绿色的亮度（值范围为0-255)
    BYTE rgbRed; // 红色的亮度（值范围为0-255)
    BYTE rgbReserved;// 保留，必须为0
} RGBQUAD;
#define PADWIDTHDW(x)	(( x + 31)  & (~31))>>3
#define PADWIDTHCH(x)	(( x + 7)  & (~7))>>3

typedef struct
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} rgb_color;

int raw2bmp(char* pSrc, char* pDst)
{
  size_t bitcount = 24, biheight = 126, biwidth = 102;
  unsigned char ** data = NULL;
//  unsigned char *dst = NULL;
  rgb_color *buf = NULL;
  unsigned int i(0);
  unsigned int j(0);

  data    = (unsigned char **)malloc(biheight * sizeof(unsigned char *));
  data[0] = (unsigned char *)(pSrc);
  for(i = 0; i < biheight; i++){
    data[i] = data[0] + i * biwidth * 3;
  }

  buf = (rgb_color *) malloc(biwidth * 3);
 // dst = (unsigned char*)malloc(biheight * biwidth * 3);

  long PosIndex(0);

 // for( i = 0 ; i < biheight; i++ ){
 //   memcpy(buf, data[biheight - 1 - i], biwidth * 3);
 //   for( j = 0 ; j < biwidth; j++ ){
 //     dst[PosIndex++] = buf[j].red;
 //     dst[PosIndex++] = buf[j].green;
 //     dst[PosIndex++] = buf[j].blue;
 //   }/
 // }

  int tmp;
  int PadWidth;
  size_t pos(0);

  PadWidth = PADWIDTHDW(biwidth * 3 * 8);
  buf = (rgb_color *) malloc(PadWidth);

  for( i = 0 ; i < biheight; i++ ){
    memcpy(buf, data[i], biwidth * 3);

    for( j = 0 ; j < biwidth; j++ ){
      tmp = buf[j].red ;
      buf[j].red = buf[j].blue;
      buf[j].blue = tmp;
    }

    memcpy(pDst + pos, buf, PadWidth);
    pos += PadWidth;


  }

  free(data);
  free(buf);
  return 0;
}
