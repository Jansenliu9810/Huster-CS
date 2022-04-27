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

	
	pns->setRunMode(1);  //����ģʽ
	pns->init();
	pns->setRtdSender(ps);
	pns->setRtdReceiver(pr);
	pns->setInputFile("W:\\Programming\\COSC264-�����ͨ��������ʵ����Ŀ��HUST��\\ʵ������ɿ�����Э�飩\\TCPЭ��\\input.txt");
	pns->setOutputFile("W:\\Programming\\COSC264-�����ͨ��������ʵ����Ŀ��HUST��\\ʵ������ɿ�����Э�飩\\TCPЭ��\\output.txt");

	pns->start();

	delete ps;
	delete pr;
	delete pUtils;									//ָ��Ψһ�Ĺ�����ʵ����ֻ��main��������ǰdelete
	delete pns;										//ָ��Ψһ��ģ�����绷����ʵ����ֻ��main��������ǰdelete
	
	return 0;
}

