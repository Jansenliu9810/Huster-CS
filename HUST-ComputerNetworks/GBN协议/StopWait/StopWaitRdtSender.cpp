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
	if (nextseqnum < base + len)			//继续发送下一个报文
	{

		this->waitingState = false;					//标记等待状态的窗口未满
		this->win[num_packet_win].acknum = -1;		//设置ack为-1
		this->win[num_packet_win].seqnum = this->nextseqnum;		//指向下一个序号的报文
		this->win[num_packet_win].checksum = 0;		//设置校验和为0

		memcpy(this->win[num_packet_win].payload, message.data, sizeof(message.data));		//数据拷贝

		this->win[num_packet_win].checksum = pUtils->calculateCheckSum(win[num_packet_win]);

		pUtils->printPacket("发送方发送报文", this->win[num_packet_win]);	//计算校验和
		if (base == nextseqnum)						//若为窗口首序号则打开计时器
		{
			pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[num_packet_win].seqnum);
		}
		pns->sendToNetworkLayer(RECEIVER, this->win[num_packet_win]);		//发送报文到网络层
		this->num_packet_win++;					//窗口内packet数量+1



		if (num_packet_win > len)					//判断窗口是否满已满
		{
			this->waitingState = true;				//窗口已满
		}
		else
		{
			this->nextseqnum++;					//窗口未满，发送下一个分组
			return true;
		}

	}
	else
	{
		this->waitingState = true;
		return true;
	}
}


void GBN_Sender::receive(const struct Packet& ack)	//接收ack确认
{
	if (this->num_packet_win > 0)					//如果窗口报文数大于0
	{
		int checkSum = pUtils->calculateCheckSum(ack);			//计算校验和

		if (checkSum == ack.checksum && ack.acknum >= this->base)		//累计确认ack
		{
			int num = ack.acknum - this->base + 1;				//记录收到的ack序号（e.g：base = 1），ack1丢失但收到了ack2，也算ack1收到

			base = ack.acknum + 1;					//重新设置base，等于收到ack序号+1
			pUtils->printPacket("发送方收到正确的ack确认", ack);

			if (this->base == this->nextseqnum)
			{
				pns->stopTimer(SENDER, this->win[0].seqnum);	//结束定时器计时
			}
			else									//否则重启定时器计时
			{
				pns->stopTimer(SENDER, this->win[0].seqnum);
				pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[num].seqnum);
			}


			for (int i = 0; i < num_packet_win - num; i += 1)
			{
				win[i] = win[i + num];				//将窗口的数据包向前移动num位
				printf("正确的窗口%d序号是%d\n", i, win[i].seqnum);
			}


			this->num_packet_win = this->num_packet_win - num;		//窗口向前移动num位，即窗口内的packet数 - num
		}
	}
}


void GBN_Sender::timeoutHandler(int seqNum)						//超时重传
{
	pUtils->printPacket("发送方定时器计时结束，重发上一次报文", this->win[0]);
	pns->stopTimer(SENDER, this->win[0].seqnum);				//重新设置定时器
	pns->startTimer(SENDER, Configuration::TIME_OUT, this->win[0].seqnum);			//开始计时
	for (int i = 0; i < num_packet_win; i += 1)
	{
		pns->sendToNetworkLayer(RECEIVER, this->win[i]);		//将packet递交到网络层，接收方
	}

}