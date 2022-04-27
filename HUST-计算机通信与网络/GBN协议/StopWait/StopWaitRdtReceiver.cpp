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
	int checkSum = pUtils->calculateCheckSum(packet);		//����У���

	if (checkSum == packet.checksum && this->seq == packet.seqnum)
	{

		pUtils->printPacket("���շ���ȷ�յ����Է��ͷ��ı���", packet);
		Message msg;
		memcpy(msg.data, packet.payload, sizeof(packet.payload));		//���ϵݽ��������ݵ�Ӧ�ò�
		pns->deliverToAppLayer(RECEIVER, msg);				//���ϵݽ�

		lastAckPkt.acknum = packet.seqnum;					//׼�����ͷ��ش�ackȷ��
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);	//����ack package��У���

		pUtils->printPacket("���շ�����ackȷ�ϱ���", lastAckPkt);
		
		this->seq += 1;
		
		
		pns->sendToNetworkLayer(SENDER, lastAckPkt);		//����ack�����ͷ�

	}
	else	//���checksum���󣬻�����seqnum����
	{

		if (packet.seqnum == this->seq)
		{
			pUtils->printPacket("ERROR: ���շ�δ�յ���ȷ���ģ�У��ʹ���", packet);
		}
		else
		{
			printf("this->seq=%d,packet.seqnum=%d\n", this->seq, packet.seqnum);
			pUtils->printPacket("ERROR: ���շ�δ�յ���ȷ���ģ�������Ŵ���", packet);
		}

		pUtils->printPacket("���շ����·�����һ�ε�ackȷ�ϱ���", lastAckPkt);	//������һ��ackȷ�ϱ���
		pns->sendToNetworkLayer(SENDER, lastAckPkt);							//������һ��ackȷ�ϱ��ĵ����ͷ�

	}
}