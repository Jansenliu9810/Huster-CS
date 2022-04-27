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


char data_save[6][Configuration::PAYLOAD_SIZE];			//payload��С


bool StopWaitRdtSender::send(const Message &message) 
{

	if (this->waitingState) { //���ͷ����ڵȴ�ȷ��״̬
		return false;
	}

	if ((this->num_send - this->expectSequenceNumberSend) != 6) 
	{

		this->packetWaitingAck.acknum = -1; //���Ը��ֶ�
		this->packetWaitingAck.seqnum = this->num_send;
		this->packetWaitingAck.checksum = 0;

		memcpy(this->packetWaitingAck.payload, message.data, sizeof(message.data));
		memcpy(data_save[this->num_send % 6], this->packetWaitingAck.payload, sizeof(message.data));

		pns->startTimer(SENDER, Configuration::TIME_OUT, this->num_send);			//�������ͷ���ʱ��
		printf("��ʱ��%d����\n", this->num_send);

		this->packetWaitingAck.checksum = pUtils->calculateCheckSum(this->packetWaitingAck);
		pUtils->printPacket("���ͷ����ͱ���", this->packetWaitingAck);

		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck);				//����ģ�����绷����sendToNetworkLayer��ͨ������㷢�͵��Է�
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
	
	//���У����Ƿ���ȷ
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//���У�����ȷ������ȷ�����>=���ͷ��ѷ��Ͳ��ȴ�ȷ�ϵ����ݰ����
	if (checkSum == ackPkt.checksum && ackPkt.acknum >= this->packetWaitingAck.seqnum) 
	{
		
		sendwin[(ackPkt.acknum - this->expectSequenceNumberSend + 6) % 6] = 1;
		pUtils->printPacket("���ͷ���ȷ�յ�ȷ��", ackPkt);
		
		sendwin[ackPkt.acknum - this->expectSequenceNumberSend] = 1;
		pns->stopTimer(SENDER, ackPkt.acknum);		//�رն�ʱ��
		printf("��ʱ��%d�ر�\n", ackPkt.acknum);

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
		pUtils->printPacket("���ͷ�û���յ����Խ��շ���Ackȷ��", ackPkt);

		if ((this->num_send - this->expectSequenceNumberSend) < 6) \
		{
			this->waitingState = false;
		}
	}
}

void StopWaitRdtSender::timeoutHandler(int seqNum) 
{

	memcpy(this->packetWaitingAck.payload, data_save[seqNum % 6], Configuration::PAYLOAD_SIZE);
	pns->stopTimer(SENDER, seqNum);										//�ȹرշ��ͷ���ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);			//�������ͷ���ʱ��

	if ((this->num_send - this->expectSequenceNumberSend) == 6) 
	{
		this->waitingState = true;
	}

	memcpy(this->packetWaitingAck.payload, data_save[seqNum % 6], Configuration::PAYLOAD_SIZE);
	this->packetWaitingAck.seqnum = seqNum;
	this->packetWaitingAck.checksum = pUtils->calculateCheckSum(this->packetWaitingAck);

	pUtils->printPacket("���ͷ���ʱ����ʱ�������ط���һ�α���", this->packetWaitingAck);
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck);

	if ((this->num_send - this->expectSequenceNumberSend) < 6) 
	{
		this->waitingState = false;
	}

}
