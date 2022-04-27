#include "stdafx.h"
#include "Global.h"
#include "TCP_Sender.h"



TCP_Sender::TCP_Sender() :base(1), nextseqnum(1), waitingState(false), num_packet_win(0) {}

TCP_Sender::~TCP_Sender() {}


bool TCP_Sender::getWaitingState()
{
    return waitingState;
}


bool TCP_Sender::send(const struct Message& message)
{
    if (nextseqnum < base + len)//����δ��ʱ
    {
        this->waitingState = false;//���ڲ���
        this->win[num_packet_win].acknum = -1;
        this->win[num_packet_win].seqnum = this->nextseqnum;//��ʼֵΪ1
        this->win[num_packet_win].checksum = 0;
        memcpy(this->win[num_packet_win].payload, message.data, sizeof(message.data));//��ȡӦ�ò�����
        this->win[num_packet_win].checksum = pUtils->calculateCheckSum(this->win[num_packet_win]);//�������

        pUtils->printPacket("���ͷ����ͱ���", this->win[num_packet_win]);
        if (base == nextseqnum)//�����base�ı��ģ���������ʱ��
            pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[num_packet_win].seqnum);
        pns->sendToNetworkLayer(RECEIVER, this->win[num_packet_win]);//���ͱ����������


        this->num_packet_win += 1;//������packet��Ŀ+1

        if (num_packet_win > len)//������ˣ������״̬
            this->waitingState = true;

        this->nextseqnum++;//������һ������nextsequm+1��
        return true;
    }

    else//�����Ѿ���ʱ
    {
        this->waitingState = true;
        return false;
    }
}


void TCP_Sender::receive(const struct Packet& ackPkt)
{
    if (this->num_packet_win > 0)//����������д�ȷ�ϵı���
    {
        int checkSum = pUtils->calculateCheckSum(ackPkt);//�������

        printf("Receive ACK number��%d\n", ackPkt.acknum);
        printf("Base number�� %d\n", this->base);

        if (checkSum == ackPkt.checksum && ackPkt.acknum >= this->base)//���ack_num��base����
        {

            if (ackPkt.acknum == this->win[0].seqnum)//�ж����ڵ���ź���һ�ε�����Ƿ���ͬ
            {
                this->count += 1;//����+1
                if (count == 4)//���ack�ĴΣ����ش���ǰ���ڵĵ�һ������
                {
                    pns->stopTimer(SENDER, this->win[0].seqnum);
                    pns->sendToNetworkLayer(RECEIVER, this->win[0]);//����һ�����ķ��͸����շ�
                    pUtils->printPacket("\n����ACK * 4�������ش���ǰ���ڵ�һ������", win[0]);
                    pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[0].seqnum);
                    printf("\n����ACK %d * 4 \n", ackPkt.acknum);
                    this->count = 0;
                    return;
                }//����ack�����ش�
            }

            else {
                this->count = 1;
            }

            if (this->count != 1)
                return;

            else
            {
                int num = ackPkt.acknum - this->base;
                base = ackPkt.acknum;//���ͷ���ackΪ�ڴ��յ���base���ĵ����
                pUtils->printPacket("���յ�ACK����", ackPkt);

                if (this->base == this->nextseqnum)//����յ���ȷ���Ƿ��ʹ����е����һ�����ĵ�ȷ��
                    pns->stopTimer(SENDER, this->win[0].seqnum);
                else
                {
                    pns->stopTimer(SENDER, this->win[0].seqnum);
                    pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[num].seqnum);//�����µļ�ʱ��
                }
                for (int i = 0; i < num_packet_win - num; i += 1)
                {
                    win[i] = win[i + num];
                    printf("The current windows's %d number is %d\n", i, win[i].seqnum);
                }
                this->num_packet_win = this->num_packet_win - num;//ƽ�ƴ���
            }
        }
    }
}


void TCP_Sender::timeoutHandler(int seqNum)
{
    pUtils->printPacket("���ͷ���ʱ����ʱ�����·��ͱ��Ķ�", this->win[0]);
    pns->stopTimer(SENDER, this->win[0].seqnum);
    pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[0].seqnum);
    pns->sendToNetworkLayer(RECEIVER, this->win[0]);

}