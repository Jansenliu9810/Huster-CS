#include "stdafx.h"
#include "Global.h"
#include "RdtSender.h"
#include "RdtReceiver.h"
#include "TCP_Sender.h"
#include "TCP_Receiver.h"
#include <iostream>


int main(int argc, char* argv[])
{
	RdtSender * ps = new TCP_Sender();
	RdtReceiver * pr = new TCP_Receiver();

	
	pns->setRunMode(1);  //安静模式
	pns->init();
	pns->setRtdSender(ps);
	pns->setRtdReceiver(pr);
	pns->setInputFile("W:\\Programming\\COSC264-计算机通信与网络实验项目（HUST）\\实验二（可靠传输协议）\\TCP协议\\input.txt");
	pns->setOutputFile("W:\\Programming\\COSC264-计算机通信与网络实验项目（HUST）\\实验二（可靠传输协议）\\TCP协议\\output.txt");

	pns->start();

	delete ps;
	delete pr;
	delete pUtils;									//指向唯一的工具类实例，只在main函数结束前delete
	delete pns;										//指向唯一的模拟网络环境类实例，只在main函数结束前delete
	
	return 0;
}

