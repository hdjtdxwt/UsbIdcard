package com.cmcc.nativepackage;

public class Printer
{
    static
    {
        System.loadLibrary("CMCC_UNITDEVICE_SUNNADA_M86");
    }
  public static native int closePrinter();
  public static native int getPrinterVersion(byte[] version);
  public static native int initialPrinter();
  public static native int openPrinter(int printerType,String deviceId,String password);
  public static native int print(String content);
  public static native int printHTML(String content);
  public static native int setAlignType(int alignType);
  public static native int setBold(int paramInt);
  public static native int setInverse(int paramInt);
  public static native int setLeftMargin(int paramInt);
  public static native int setLineSpacingByDotPitch(int paramInt);
  public static native int setPrintOrientation(int paramInt);
  public static native int setRightMargin(int paramInt);
  public static native int setUnderLine(int paramInt);
  public static native int setWordSpacingByDotPitch(int paramInt);
  public static native int setZoonIn(int paramInt1, int paramInt2);
}
