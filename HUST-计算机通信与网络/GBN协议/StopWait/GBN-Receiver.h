#include "RdtReceiver.h"
#ifndef STOPWAIT_GBNRECEIVER_H
#define STOPWAIT_GBNRECEIVER_H
class GBN_Receiver :public RdtReceiver
{

private:
	int seq;				//�ڴ��յ���һ���������
	Packet lastAckPkt;		//�ϴη��͵�Ackȷ�ϱ���

public:
	GBN_Receiver();
	virtual ~GBN_Receiver();

public:
	void receive(const Packet& packet);		//���ձ��ģ���NetWorkService����

};

#endif	//STOPWAIT_GBNRECEIVER_H