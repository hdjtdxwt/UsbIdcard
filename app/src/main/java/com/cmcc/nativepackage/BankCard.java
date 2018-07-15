package com.cmcc.nativepackage;

public class BankCard
{
    static
    {
        System.loadLibrary("CMCC_UNITDEVICE_SUNNADA_M86");
    }
  public static native int closeBankCard();

  public static native int getBankCardInfo(byte[] paramArrayOfByte1, byte[] paramArrayOfByte2, byte[] paramArrayOfByte3);

  public static native int getBankCardVersion(byte[] paramArrayOfByte);

  public static native int initialBankCard();

  public static native int openBankCard(int paramInt, String paramString1, String paramString2);
}
