#ifndef TCP_SENDER_H
#define TCP_SENDER_H
#include "RdtSender.h"
#define len 8


class TCP_Sender :public RdtSender
{

private:
	int base;			//发送窗口的base
	int nextseqnum;		//下一个发送窗口序号
	bool waitingState;	//是否处于等待Ack状态
	Packet win[len];	//发送窗口及其大小为8
	int num_packet_win;	//窗口中报文的个数
	int count=0;
	int current_recv_ack;
	int last_recv_ack;


public:
	bool getWaitingState();
	bool send(const Message& message);	//发送来自应用层的Message，被NetworkServiceSimulator调用
	void receive(const Packet& ackPkt);	//接收ACK，将被网络层调用
	void timeoutHandler(int seqNum);


public:
	TCP_Sender();
	virtual ~TCP_Sender();

};

#endif		//TCP_SENDER_H