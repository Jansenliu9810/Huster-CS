#include "RdtReceiver.h"
#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H



class TCP_Receiver :public RdtReceiver
{

private:
	int seq;					//�ڴ��յ�����һ���������
	Packet lastAckPkt;			//������һ�ε�ACKȷ�ϱ���


public:
	TCP_Receiver();
	virtual ~TCP_Receiver();


public:
	void receive(const Packet& packet);		//���ձ��ģ���NetworkService����

};

#endif