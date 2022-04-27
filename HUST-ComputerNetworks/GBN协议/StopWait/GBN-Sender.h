#include "RdtSender.h"
#ifndef STOPWAIT_GBNSENDER_H
#define STOPWAIT_GBNSENDER_H
#define len 8

class GBN_Sender :public RdtSender 
{
private:
	int base;				//���ʹ��ڵ�base
	int nextseqnum;			//���ʹ��ڵ�nextseqnum
	bool waitingState;		//�ж��Ƿ��ڵȴ�Ack״̬
	Packet win[len];		//���ʹ��ڵĴ��ڴ�СΪ4
	int num_packet_win;		//��¼���ʹ����еı��ĸ���

public:
	bool getWaitingState();
	bool send(const Message& message);		//����Ӧ�ò�������Message���ȴ���NetWorkService����
	void receive(const Packet& ackPkt);		//����Ack���ȴ���NetWorkService����
	void timeoutHandler(int seqNum);

public:
	GBN_Sender();
	virtual ~GBN_Sender();
};

#endif	//STOPWAIT_GBNSENDER_H