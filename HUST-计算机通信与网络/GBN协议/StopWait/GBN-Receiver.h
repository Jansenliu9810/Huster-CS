#include "RdtReceiver.h"
#ifndef STOPWAIT_GBNRECEIVER_H
#define STOPWAIT_GBNRECEIVER_H
class GBN_Receiver :public RdtReceiver
{

private:
	int seq;				//期待收到下一个报文序号
	Packet lastAckPkt;		//上次发送的Ack确认报文

public:
	GBN_Receiver();
	virtual ~GBN_Receiver();

public:
	void receive(const Packet& packet);		//接收报文，被NetWorkService调用

};

#endif	//STOPWAIT_GBNRECEIVER_H