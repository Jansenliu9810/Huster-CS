#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtSender.h"

int sendwin[6];

StopWaitRdtSender::StopWaitRdtSender():expectSequenceNumberSend(0),waitingState(false),num_send(0)
{
	sendwin[0] = 0;
	sendwin[1] = 0;
	sendwin[2] = 0;
	sendwin[3] = 0;
	sendwin[4] = 0;
	sendwin[5] = 0;
}


StopWaitRdtSender::~StopWaitRdtSender()
{
}



bool StopWaitRdtSender::getWaitingState() {
	return waitingState;
}


char data_save[6][Configuration::PAYLOAD_SIZE];			//payload大小


bool StopWaitRdtSender::send(const Message &message) 
{

	if (this->waitingState) { //发送方处于等待确认状态
		return false;
	}

	if ((this->num_send - this->expectSequenceNumberSend) != 6) 
	{

		this->packetWaitingAck.acknum = -1; //忽略该字段
		this->packetWaitingAck.seqnum = this->num_send;
		this->packetWaitingAck.checksum = 0;

		memcpy(this->packetWaitingAck.payload, message.data, sizeof(message.data));
		memcpy(data_save[this->num_send % 6], this->packetWaitingAck.payload, sizeof(message.data));

		pns->startTimer(SENDER, Configuration::TIME_OUT, this->num_send);			//启动发送方定时器
		printf("定时器%d启动\n", this->num_send);

		this->packetWaitingAck.checksum = pUtils->calculateCheckSum(this->packetWaitingAck);
		pUtils->printPacket("发送方发送报文", this->packetWaitingAck);

		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck);				//调用模拟网络环境的sendToNetworkLayer，通过网络层发送到对方
		this->num_send = this->num_send + 1;

		if ((this->num_send - this->expectSequenceNumberSend) == 6) {
			this->waitingState = true;
		}
		else if((this->num_send - this->expectSequenceNumberSend) < 6) {
			this->waitingState = false;
		}

		return true;

	}
}



void StopWaitRdtSender::receive(const Packet &ackPkt) 
{
	
	//检查校验和是否正确
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//如果校验和正确，并且确认序号>=发送方已发送并等待确认的数据包序号
	if (checkSum == ackPkt.checksum && ackPkt.acknum >= this->packetWaitingAck.seqnum) 
	{
		
		sendwin[(ackPkt.acknum - this->expectSequenceNumberSend + 6) % 6] = 1;
		pUtils->printPacket("发送方正确收到确认", ackPkt);
		
		sendwin[ackPkt.acknum - this->expectSequenceNumberSend] = 1;
		pns->stopTimer(SENDER, ackPkt.acknum);		//关闭定时器
		printf("定时器%d关闭\n", ackPkt.acknum);

		while (sendwin[0] == 1) {

			sendwin[0] = sendwin[1];
			sendwin[1] = sendwin[2];
			sendwin[2] = sendwin[3];
			sendwin[3] = sendwin[4];
			sendwin[4] = sendwin[5];
			sendwin[5] = 0;

			this->expectSequenceNumberSend = (this->expectSequenceNumberSend + 1);
			this->waitingState = false;

		}

		if ((this->num_send - this->expectSequenceNumberSend) < 6) 
		{
			this->waitingState = false;
		}

	}

	else 
	{
		pUtils->printPacket("发送方没有收到来自接收方的Ack确认", ackPkt);

		if ((this->num_send - this->expectSequenceNumberSend) < 6) \
		{
			this->waitingState = false;
		}
	}
}

void StopWaitRdtSender::timeoutHandler(int seqNum) 
{

	memcpy(this->packetWaitingAck.payload, data_save[seqNum % 6], Configuration::PAYLOAD_SIZE);
	pns->stopTimer(SENDER, seqNum);										//先关闭发送方定时器
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);			//重启发送方定时器

	if ((this->num_send - this->expectSequenceNumberSend) == 6) 
	{
		this->waitingState = true;
	}

	memcpy(this->packetWaitingAck.payload, data_save[seqNum % 6], Configuration::PAYLOAD_SIZE);
	this->packetWaitingAck.seqnum = seqNum;
	this->packetWaitingAck.checksum = pUtils->calculateCheckSum(this->packetWaitingAck);

	pUtils->printPacket("发送方定时器计时结束，重发上一次报文", this->packetWaitingAck);
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck);

	if ((this->num_send - this->expectSequenceNumberSend) < 6) 
	{
		this->waitingState = false;
	}

}
