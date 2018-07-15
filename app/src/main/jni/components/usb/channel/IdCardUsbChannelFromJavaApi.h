/*
 * IdCardUsbChannelFromJavaApi.h
 *
 *  Created on: 2015-3-16
 *      Author: liuzg
 */

#ifndef IDCARDUSBCHANNELFROMJAVAAPI_H_
#define IDCARDUSBCHANNELFROMJAVAAPI_H_

#include "BaseChannel.h"
class UsbChannel: public BaseChannel {
public:
	UsbChannel();
	virtual ~UsbChannel();
public:
	void Write(jbyteArray);
	void ReadResp();
	void ReadResp(int timeout);
	int CheckResponse(int timeout=0);
protected:
	int OpenConnect(int, char*);
	void CloseConnect();
private:
	jclass mUsbHelper;
};
#endif /* IDCARDUSBCHANNELFROMJAVAAPI_H_ */
