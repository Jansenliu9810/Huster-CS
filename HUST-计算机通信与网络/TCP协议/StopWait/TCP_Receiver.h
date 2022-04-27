#include "RdtReceiver.h"
#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H



class TCP_Receiver :public RdtReceiver
{

private:
	int seq;					//期待收到的下一个报文序号
	Packet lastAckPkt;			//发送上一次的ACK确认报文


public:
	TCP_Receiver();
	virtual ~TCP_Receiver();


public:
	void receive(const Packet& packet);		//接收报文，被NetworkService调用

};

#endif