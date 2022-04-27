#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtReceiver.h"

char receiver_win[6][Configuration::PAYLOAD_SIZE];
int recv[6];

StopWaitRdtReceiver::StopWaitRdtReceiver()
{
	lastAckPkt.acknum = -1; //初始状态下，上次发送的确认包的确认序号为-1，使得当第一个接受的数据包出错时该确认报文的确认号为-1
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;	//忽略该字段

	recv[0] = 0;
	recv[1] = 0;
	recv[2] = 0;
	recv[3] = 0;
	recv[4] = 0;
	recv[5] = 0;

	this->expectSequenceNumberRcvd = 0;

	for(int i = 0; i < Configuration::PAYLOAD_SIZE;i += 1){
		lastAckPkt.payload[i] = '.';
	}
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
}


StopWaitRdtReceiver::~StopWaitRdtReceiver()
{
}

void StopWaitRdtReceiver::receive(const Packet &packet) {
	//检查校验和是否正确
	int checkSum = pUtils->calculateCheckSum(packet);

	//如果校验和正确，同时收到报文的序号等于接收方期待收到的报文序号一致
	if (checkSum == packet.checksum && ((this->expectSequenceNumberRcvd == packet.seqnum) || (this->expectSequenceNumberRcvd + 1 == packet.seqnum)
		|| (this->expectSequenceNumberRcvd + 2 == packet.seqnum) || (this->expectSequenceNumberRcvd + 3 == packet.seqnum) || (this->expectSequenceNumberRcvd + 4 == packet.seqnum)
		|| (this->expectSequenceNumberRcvd + 5 == packet.seqnum))){
		pUtils->printPacket("接收方正确收到发送方的报文", packet);

		
		//取出Message，向上递交给应用层
		Message msg;
		if (packet.seqnum - this->expectSequenceNumberRcvd >= 0) 
		{
			memcpy(receiver_win[packet.seqnum - this->expectSequenceNumberRcvd], packet.payload, sizeof(packet.payload));	  //缓存报文

			lastAckPkt.acknum = packet.seqnum;																//确认序号等于收到的报文序号
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("接收方发送确认报文", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);			//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
		
			recv[packet.seqnum - this->expectSequenceNumberRcvd] = 1;

		}
		
		while (recv[0]) 
		{

			memcpy(msg.data, receiver_win[0], sizeof(msg.data));
			pns->delivertoAppLayer(RECEIVER, msg);				//上传
			recv[0] = recv[1];
			recv[1] = recv[2];
			recv[2] = recv[3];
			recv[3] = recv[4];
			recv[4] = recv[5];
			recv[5] = 0;

			memcpy(receiver_win[0], receiver_win[1], sizeof(msg.data));
			memcpy(receiver_win[1], receiver_win[2], sizeof(msg.data));
			memcpy(receiver_win[2], receiver_win[3], sizeof(msg.data));
			memcpy(receiver_win[3], receiver_win[4], sizeof(msg.data));
			memcpy(receiver_win[4], receiver_win[5], sizeof(msg.data));

			this->expectSequenceNumberRcvd += 1;

		}

	}
	else {
		if (checkSum != packet.checksum) {
			pUtils->printPacket("接收方没有正确收到发送方的报文,数据校验错误", packet);
		}
		else {

			lastAckPkt.acknum = packet.seqnum;								//确认收到的报文序号
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);	//确认并计算收到报文的校验和
			pUtils->printPacket("发送方已收到确认报文", lastAckPkt);

		}
		pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送上次的确认报文

	}
}