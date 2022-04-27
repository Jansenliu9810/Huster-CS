#ifndef TCP_SENDER_H
#define TCP_SENDER_H
#include "RdtSender.h"
#define len 8


class TCP_Sender :public RdtSender
{

private:
	int base;			//���ʹ��ڵ�base
	int nextseqnum;		//��һ�����ʹ������
	bool waitingState;	//�Ƿ��ڵȴ�Ack״̬
	Packet win[len];	//���ʹ��ڼ����СΪ8
	int num_packet_win;	//�����б��ĵĸ���
	int count=0;
	int current_recv_ack;
	int last_recv_ack;


public:
	bool getWaitingState();
	bool send(const Message& message);	//��������Ӧ�ò��Message����NetworkServiceSimulator����
	void receive(const Packet& ackPkt);	//����ACK��������������
	void timeoutHandler(int seqNum);


public:
	TCP_Sender();
	virtual ~TCP_Sender();

};

#endif		//TCP_SENDER_H