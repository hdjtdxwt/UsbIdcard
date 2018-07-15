package com.cmcc.nativepackage;


public class SimCard
{
    static
    {
        System.loadLibrary("CMCC_UNITDEVICE_SUNNADA_M86");
    }
    public static native int GetOPSVersion(char[] Version);
    public static native int GetCardSN(char[] CardSN);
    public static native int WriteCard(char[] IssueData, char[] Result);
    public static native int ConfigReader(int ReaderType, char[] DeviceID, char[] Password);
    public static native int GetOPSErrorMsg(int ErrorCode, char[] ErrorMsg);
    public static native int GetCardInfo(char[] CardInfo);

}
