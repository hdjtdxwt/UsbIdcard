/*
 * UsbChannel.h
 *
 *  Created on: 2015-3-11
 *      Author: liuzg
 */

#ifndef USBCHANNEL_H_
#define USBCHANNEL_H_

#include "BaseChannel.h"
#include "libusb.h"
class UsbChannel: public BaseChannel {
public:
	UsbChannel();
	virtual ~UsbChannel();
public:
	void Write(jbyteArray);
	void ReadResp();
	int CheckResponse();
protected:
	int OpenConnect(int, char*);
	void CloseConnect();
private:
	bool initOk;
	libusb_context *m_ctx;
	libusb_device_handle *m_dev_handle;
	libusb_device **m_devs;
	int m_cnt;
	int device_satus(libusb_device_handle *hd);
	void printdev(libusb_device*dev);
	int m_interface;
	void DebugTransferLog(int r);
};

#endif /* USBCHANNEL_H_ */
