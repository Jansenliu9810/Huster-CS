#include "stdafx.h"
#include "Global.h"
#include "GBN-Sender.h"


GBN_Sender::GBN_Sender() :base(1), nextseqnum(1), waitingState(false), num_packet_win(0)
{
}

GBN_Sender::~GBN_Sender()
{
}

bool GBN_Sender::getWaitingState()
{
	return waitingState;
}


bool GBN_Sender::send(const struct Message& message)
{
	if (nextseqnum < base + len)			//����������һ������
	{

		this->waitingState = false;					//��ǵȴ�״̬�Ĵ���δ��
		this->win[num_packet_win].acknum = -1;		//����ackΪ-1
		this->win[num_packet_win].seqnum = this->nextseqnum;		//ָ����һ����ŵı���
		this->win[num_packet_win].checksum = 0;		//����У���Ϊ0

		memcpy(this->win[num_packet_win].payload, message.data, sizeof(message.data));		//���ݿ���

		this->win[num_packet_win].checksum = pUtils->calculateCheckSum(win[num_packet_win]);

		pUtils->printPacket("���ͷ����ͱ���", this->win[num_packet_win]);	//����У���
		if (base == nextseqnum)						//��Ϊ�����������򿪼�ʱ��
		{
			pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[num_packet_win].seqnum);
		}
		pns->sendToNetworkLayer(RECEIVER, this->win[num_packet_win]);		//���ͱ��ĵ������
		this->num_packet_win++;					//������packet����+1



		if (num_packet_win > len)					//�жϴ����Ƿ�������
		{
			this->waitingState = true;				//��������
		}
		else
		{
			this->nextseqnum++;					//����δ����������һ������
			return true;
		}

	}
	else
	{
		this->waitingState = true;
		return true;
	}
}


void GBN_Sender::receive(const struct Packet& ack)	//����ackȷ��
{
	if (this->num_packet_win > 0)					//������ڱ���������0
	{
		int checkSum = pUtils->calculateCheckSum(ack);			//����У���

		if (checkSum == ack.checksum && ack.acknum >= this->base)		//�ۼ�ȷ��ack
		{
			int num = ack.acknum - this->base + 1;				//��¼�յ���ack��ţ�e.g��base = 1����ack1��ʧ���յ���ack2��Ҳ��ack1�յ�

			base = ack.acknum + 1;					//��������base�������յ�ack���+1
			pUtils->printPacket("���ͷ��յ���ȷ��ackȷ��", ack);

			if (this->base == this->nextseqnum)
			{
				pns->stopTimer(SENDER, this->win[0].seqnum);	//������ʱ����ʱ
			}
			else									//����������ʱ����ʱ
			{
				pns->stopTimer(SENDER, this->win[0].seqnum);
				pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[num].seqnum);
			}


			for (int i = 0; i < num_packet_win - num; i += 1)
			{
				win[i] = win[i + num];				//�����ڵ����ݰ���ǰ�ƶ�numλ
				printf("��ȷ�Ĵ���%d�����%d\n", i, win[i].seqnum);
			}


			this->num_packet_win = this->num_packet_win - num;		//������ǰ�ƶ�numλ���������ڵ�packet�� - num
		}
	}
}


void GBN_Sender::timeoutHandler(int seqNum)						//��ʱ�ش�
{
	pUtils->printPacket("���ͷ���ʱ����ʱ�������ط���һ�α���", this->win[0]);
	pns->stopTimer(SENDER, this->win[0].seqnum);				//�������ö�ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[0].seqnum);			//��ʼ��ʱ
	for (int i = 0; i < num_packet_win; i += 1)
	{
		pns->sendToNetworkLayer(RECEIVER, this->win[i]);		//��packet�ݽ�������㣬���շ�
	}

}