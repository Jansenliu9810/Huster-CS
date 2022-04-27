#include "RdtSender.h"
#ifndef STOPWAIT_GBNSENDER_H
#define STOPWAIT_GBNSENDER_H
#define len 8

class GBN_Sender :public RdtSender 
{
private:
	int base;				//发送窗口的base
	int nextseqnum;			//发送窗口的nextseqnum
	bool waitingState;		//判断是否处于等待Ack状态
	Packet win[len];		//发送窗口的窗口大小为4
	int num_packet_win;		//记录发送窗口中的报文个数

public:
	bool getWaitingState();
	bool send(const Message& message);		//发送应用层下来的Message，等待被NetWorkService调用
	void receive(const Packet& ackPkt);		//接收Ack，等待被NetWorkService调用
	void timeoutHandler(int seqNum);

public:
	GBN_Sender();
	virtual ~GBN_Sender();
};

#endif	//STOPWAIT_GBNSENDER_H