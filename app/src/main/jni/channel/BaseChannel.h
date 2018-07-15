#ifndef BASECHANNEL_H_
#define BASECHANNEL_H_
#include <jni.h>
#include "EnvQueue.h"

enum Channel_Type {
	type_usb = 1, type_bluetooth, type_serial, type_inner
};
class BaseChannel {
public:
	BaseChannel();
	virtual ~BaseChannel();
	const static int Channel_Write_Card = 3;
	const static int Channel_Unipay_Scanner = 3;
	const static int Channel_Printer = 2;
	const static int Channel_PinPad = 1;
	const static int Channel_ID_Scanner = 0;
	const static int Channel_Exit = -1;
	const static int Channel_ID_Scanner_CTS = 10;//华视二代证
protected:
	Channel_Type mChannelType;
	JNIEnv * mJNIEnv;
	bool isEnterDev;
	bool hasMCU;
public:
	int totalLen;
	bool isConnected;
	char buffer[2048];
	void SetJNIEnv(JNIEnv * env);
	JNIEnv *GetJNIEnv(bool IsBindThread = true);
public:
	int OpenChannel(int, char *, bool hasMCU = true);
	void CloseChannel(bool exitMcu = true);
	void Buzzer(int time, int period);
protected:
	void Init();
	int EnterDev(int);
protected:
	//sub class must implemnet
	virtual int OpenConnect(int, char*)=0;
	virtual void CloseConnect()=0;
public:
	virtual void Write(jbyteArray)=0;
	virtual void ReadResp()=0;
	virtual int CheckResponse(int timeout=0)=0;
};
#endif /* BASECHANNEL_H_ */
