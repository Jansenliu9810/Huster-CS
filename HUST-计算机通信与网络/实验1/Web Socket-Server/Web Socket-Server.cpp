#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <fstream>		//包含函数open，用来打开local文件
using namespace std;

#pragma comment(lib,"ws2_32.lib")

//从文件读入到string里
char* readFileIntoString(char * PictureDirect)
{
	char buffer[102400];
	int i;
	ifstream f(PictureDirect, ios::binary);  //以二进制读模式打开文件
	if (!f)
	{
		cout << "读取文件失败" << endl;
		return NULL;
	}
	char c;
	f.read(buffer, 102400);
	f.close();
	return buffer;
}


int main(int argc, char* argv[]) {


	//可配置监听地址（ip地址&端口号）
	int count;
	printf("命令行有 %d 个参数: \n", argc);
	for (count = 0; count < argc; ++count) {
		printf("%d: %s\n", count, argv[count]);
	}

	
	//初始化Socket网络库
	WSADATA wsadata;
	int wsa_start = WSAStartup(MAKEWORD(2, 2), &wsadata);			//定义网路库版本2.2
	if (wsadata.wVersion != MAKEWORD(2, 2)) {
		printf("网络库版本不正确\n");
		WSACleanup();
		return 0;
	}


	//检查socket是否有数据到来及是否可发送的文件描述符
	//用于非阻塞模式下socket等待网络事件通知
	fd_set read_fds, write_fds;


	//第一次连接
	bool first_connection = true;


	//创建Server端Socket服务器
	SOCKET server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock != INVALID_SOCKET) {
		printf("Socket服务端创建完成\n");
	}
	else {
		printf("Socket服务端创建出错\n");
	}


	//Server端Socket服务器赋值
	int port;
	port = atoi(argv[2]);
	char* ip_addr = argv[1];
	sockaddr_in server_inf;
	memset(&server_inf, 0, sizeof(server_inf));
	server_inf.sin_family = AF_INET;
	server_inf.sin_port = htons(port);
	server_inf.sin_addr.s_addr = inet_addr(ip_addr);


	//绑定Server端Socket服务器ip地址及端口
	if (bind(server_sock, (LPSOCKADDR)&server_inf, sizeof(server_inf)) != SOCKET_ERROR) {
		printf("Socket绑定完成\n");
	}
	else {
		printf("Socket绑定出错\n");
	}


	//设置监听并检查监听是否成功
	if (listen(server_sock, 5) != SOCKET_ERROR) {
		printf("Socket服务端设置监听成功\n");
	}
	else {
		printf("Socket服务端设置监听出错\n");
	}


	//将server端Socket设置为非阻塞模式
	unsigned long block_mode = 1;
	int Un_blockmode = ioctlsocket(server_sock, FIONBIO, &block_mode);
	if (Un_blockmode != SOCKET_ERROR) {
		printf("Socket服务端的非阻塞模式设置完成\n");
		printf("等待客户端的数据和请求.........\n\n");
	}
	else {
		printf("Socket服务端的非阻塞模式设置出错\n");
	}


	//创建Client端Socket服务器并赋值
	SOCKET client_sock;
	sockaddr_in client_inf;
	client_inf.sin_family = AF_INET;
	int client_addr_len = sizeof(client_inf);
	char recv_buffer[10240];


	//定义文件名及内容大小
	char file_name[102400];
	//定义数据报每次读取字符数
	char str_read[2];
	
	//字符串复制，数组为空
	strcpy(file_name, "");


	//循环接收数据
	back:while (true) {

		//清空read, write文件描述符，对read_fds和write_fds初始化
		//先用FD_ZERO清空，再用FD_SET重新添加描述符
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);


		//重新添加文件描述符，并设置等待Client连接
		FD_SET(server_sock, &read_fds);


		//判断第一次连接
		if (first_connection == false) {
			//设置等待的会话Socket可接收或发送数据
			FD_SET(client_sock, &read_fds);
			FD_SET(client_sock, &write_fds);
		}


		//开始等待连接
		int start_waiting = select(0, &read_fds, &write_fds, NULL, NULL);

		if (FD_ISSET(server_sock, &read_fds)) {
			start_waiting--;


			//接收Client端的请求并打开一个Socket会话
			client_sock = accept(server_sock, (LPSOCKADDR)&client_inf, &client_addr_len);
			if (client_sock != INVALID_SOCKET) {
				printf("接收到来自客户端的数据连接请求并生成Socket会话\n");
			}
			else {
				printf("接收客户端数据失败\n");
			}


			//将Socket会话设置成非阻塞模式
			int Un_blockmode = ioctlsocket(client_sock, FIONBIO, &block_mode);
			if (Un_blockmode != SOCKET_ERROR) {
				printf("Socket客户端的非阻塞模式设置完成\n");
				printf("等待来自Socket客户端的数据.........\n\n");
			}
			else {
				printf("Socket客户端的非阻塞模式设置出错\n");
				return 0;
			}


			//重新设置等待的会话Socket可接收或发送数据
			FD_SET(client_sock, &read_fds);
			FD_SET(client_sock, &write_fds);


			//再次判断首次连接
			first_connection = false;

		}
		
		strcpy(file_name, "");
		//检查Socket会话是否有数据传输到达
		if (true) {

			//如果有数据到达，则接收client端数据
			if (FD_ISSET(client_sock, &read_fds)) {

				//接收client端数据并处理
				memset(&recv_buffer, '\0', 10240);
				int Un_blockmode = recv(client_sock, recv_buffer, 10240, 0);
				if (Un_blockmode > 0) {
					printf("客户端IP地址: %s\n", inet_ntoa(client_inf.sin_addr));
					printf("客户端端口号：%d\n", ntohs(client_inf.sin_port));
					printf("客户端请求数据：%s\n", recv_buffer);


					//HTTP请求报文读取
					char* file = recv_buffer + 5;			//定义数据报从第五个字节开始读取
					while (*file != ' ') {					//读取到首个空格停止
						str_read[0] = *file;
						str_read[1] = '\0';
						strcat(file_name, str_read);		//字符串拼接
						file += 1;
					}


					//通过文件名查找文件并打开
					ifstream file_find(file_name, ios::binary);		//查找文件函数
					char cal_len[102400];
					strcpy(cal_len, "");
					char str_c[1];		//读取文件每个字符
					char str_s[2];		//字符串
					int str_len = 0;

					if (file_find.good()) {
						while (file_find.read(str_c, 1)) {
							str_len += 1;
							str_s[0] = str_c[0];
							str_s[1] = '\0';				//将每个被读取的str_c字符 + ‘\0’ 组合成字符串
							strcat(cal_len, str_s);		//将字符串还原到文件内容 = file_name内容
						};
							int file_len;				
							file_find.seekg(0, ios::end);
							file_len = file_find.tellg();
							file_find.close();
						//printf("%d\n", str_len);

						
						//构造响应报文并发回给客户端
						char response_mes[112000] = "";		//相应报文赋值大小
						char format_str[40];
						strcpy(response_mes, "HTTP/1.1 200 OK\r\n");
						strcat(response_mes, "Content-Length: "); //把两个字符串连接起来
						sprintf(format_str, "%d", str_len);  //格式化输出字符串
						strcat(response_mes, format_str);


						//多类型文件通信访问
						string s = file_name;
						if (strcmp(s.substr(s.length() -4, s.length() - 1).c_str(), "html") == 0) {
							strcat(response_mes, "\r\nContent-Type: text/html");
						}
						else if (strcmp(s.substr(s.length() - 3, s.length() - 1).c_str(), "jpg") == 0) {
							strcat(response_mes, "\r\nContent-Type: image/jpg");
						}
						else if (strcmp(s.substr(s.length() - 3, s.length() - 1).c_str(), "png") == 0) {
							strcat(response_mes, "\r\nContent-Type: image/png");
						}
						else if (strcmp(s.substr(s.length() - 3, s.length() - 1).c_str(), "gif") == 0) {
							strcat(response_mes, "\r\nContent-Type: image/gif");
						}


						strcat(response_mes, "\r\n");
						strcat(response_mes, "\r\n");
						int head_length = strlen(response_mes);
						printf("发送http报文头: %s\n", response_mes);
						char buffer[102400];
						int i;
						ifstream f(file_name, ios::binary);  //以二进制读模式打开文件
						if (!f)
						{
							cout << "读取文件失败" << endl;
							return NULL;
						}
						char c;
						f.read(buffer, 102400);
						f.close();
						char file_content[102400];
						memcpy(file_content, buffer, 39580);
						memcpy(response_mes + head_length, file_content, 39580);
						//printf("%s",file_name);
						//printf("%s", response_mes);
						int response = send(client_sock, response_mes, str_len+ head_length, 0);
					}
					else {
						printf("\n文件查找读取失败");
						char response_err[102400];
						char format_s[40];
						strcpy(file_name, "404.html");
						strcpy(response_err, "HTTP/1.1 200 OK\r\n") ;
						strcat(response_err, "Content-Length: 101"); //把两个字符串连接起来
						strcat(response_err, "\r\nContent-Type: text/html");
						strcat(response_err, "\r\n");
						strcat(response_err, "\r\n");
						strcat(response_err, readFileIntoString(file_name));
						send(client_sock, response_err, strlen(response_err), 0);
					}
				goto back;
				}
				else {
					printf("\nSocket客户端已离开.........\n");
					closesocket(client_sock);			//client端断开连接，便自动关闭会话窗口
					//break;
				}
			}
		}

	}
	//关闭Server端Socket服务器
	closesocket(server_sock);

	//重新初始化网络库
	WSACleanup();
	return 0;
}