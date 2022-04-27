#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtReceiver.h"

char receiver_win[6][Configuration::PAYLOAD_SIZE];
int recv[6];

StopWaitRdtReceiver::StopWaitRdtReceiver()
{
	lastAckPkt.acknum = -1; //��ʼ״̬�£��ϴη��͵�ȷ�ϰ���ȷ�����Ϊ-1��ʹ�õ���һ�����ܵ����ݰ�����ʱ��ȷ�ϱ��ĵ�ȷ�Ϻ�Ϊ-1
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;	//���Ը��ֶ�

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
	//���У����Ƿ���ȷ
	int checkSum = pUtils->calculateCheckSum(packet);

	//���У�����ȷ��ͬʱ�յ����ĵ���ŵ��ڽ��շ��ڴ��յ��ı������һ��
	if (checkSum == packet.checksum && ((this->expectSequenceNumberRcvd == packet.seqnum) || (this->expectSequenceNumberRcvd + 1 == packet.seqnum)
		|| (this->expectSequenceNumberRcvd + 2 == packet.seqnum) || (this->expectSequenceNumberRcvd + 3 == packet.seqnum) || (this->expectSequenceNumberRcvd + 4 == packet.seqnum)
		|| (this->expectSequenceNumberRcvd + 5 == packet.seqnum))){
		pUtils->printPacket("���շ���ȷ�յ����ͷ��ı���", packet);

		
		//ȡ��Message�����ϵݽ���Ӧ�ò�
		Message msg;
		if (packet.seqnum - this->expectSequenceNumberRcvd >= 0) 
		{
			memcpy(receiver_win[packet.seqnum - this->expectSequenceNumberRcvd], packet.payload, sizeof(packet.payload));	  //���汨��

			lastAckPkt.acknum = packet.seqnum;																//ȷ����ŵ����յ��ı������
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);			//����ģ�����绷����sendToNetworkLayer��ͨ������㷢��ȷ�ϱ��ĵ��Է�
		
			recv[packet.seqnum - this->expectSequenceNumberRcvd] = 1;

		}
		
		while (recv[0]) 
		{

			memcpy(msg.data, receiver_win[0], sizeof(msg.data));
			pns->delivertoAppLayer(RECEIVER, msg);				//�ϴ�
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
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���,����У�����", packet);
		}
		else {

			lastAckPkt.acknum = packet.seqnum;								//ȷ���յ��ı������
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);	//ȷ�ϲ������յ����ĵ�У���
			pUtils->printPacket("���ͷ����յ�ȷ�ϱ���", lastAckPkt);

		}
		pns->sendToNetworkLayer(SENDER, lastAckPkt);	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢���ϴε�ȷ�ϱ���

	}
}