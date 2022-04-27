#include "stdafx.h"
#include "Global.h"
#include "GBN-Receiver.h"


GBN_Receiver::GBN_Receiver() :seq(1)
{

	lastAckPkt.acknum = 0;
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;

	for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++)
	{
		lastAckPkt.payload[i] = '.';
	}
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);

}


GBN_Receiver::~GBN_Receiver()
{
}


void GBN_Receiver::receive(const struct Packet& packet)
{
	int checkSum = pUtils->calculateCheckSum(packet);		//计算校验和

	if (checkSum == packet.checksum && this->seq == packet.seqnum)
	{

		pUtils->printPacket("接收方正确收到来自发送方的报文", packet);
		Message msg;
		memcpy(msg.data, packet.payload, sizeof(packet.payload));		//向上递交报文内容到应用层
		pns->deliverToAppLayer(RECEIVER, msg);				//向上递交

		lastAckPkt.acknum = packet.seqnum;					//准备向发送方回传ack确认
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);	//计算ack package的校验和

		pUtils->printPacket("接收方发送ack确认报文", lastAckPkt);
		
		this->seq += 1;
		
		
		pns->sendToNetworkLayer(SENDER, lastAckPkt);		//发送ack到发送方

	}
	else	//如果checksum错误，或者是seqnum乱序
	{

		if (packet.seqnum == this->seq)
		{
			pUtils->printPacket("ERROR: 接收方未收到正确报文：校验和错误", packet);
		}
		else
		{
			printf("this->seq=%d,packet.seqnum=%d\n", this->seq, packet.seqnum);
			pUtils->printPacket("ERROR: 接收方未收到正确报文：报文序号错误", packet);
		}

		pUtils->printPacket("接收方重新发送上一次的ack确认报文", lastAckPkt);	//发送上一次ack确认报文
		pns->sendToNetworkLayer(SENDER, lastAckPkt);							//发送上一次ack确认报文到发送方

	}
}