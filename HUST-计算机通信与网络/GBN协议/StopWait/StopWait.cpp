#include "stdafx.h"
#include "Global.h"
#include "RdtSender.h"
#include "RdtReceiver.h"
#include "GBN-Sender.h"
#include "GBN-Receiver.h"
#include <iostream>


int main(int argc, char* argv[]) {

	RdtSender* ps = new GBN_Sender();
	RdtReceiver* pr = new GBN_Receiver();


	pns->setRunMode(1);
	pns->init();
	pns->setRdtSender(ps);
	pns->setRdtReceiver(pr);
	pns->setInputFile("W:\\Programming\\COSC264-计算机通信与网络实验项目（HUST）\\实验二（可靠传输协议）\\GBN协议\\input.txt");
	pns->setOutputFile("W:\\Programming\\COSC264-计算机通信与网络实验项目（HUST）\\实验二（可靠传输协议）\\GBN协议\\output.txt");
	pns->start();


	delete ps;
	delete pr;
	delete pUtils;
	delete pns;


	return 0;

}
