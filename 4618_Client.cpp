////////////////////////////////////////////////////////////////
// ELEX 4618 Client Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated April 2022
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>
#include <conio.h>

#include "Client.h"

std::string server_ip = "192.168.0.100";//"192.168.137.22";//127.0.0.1"; // old raspberrypi:	192.168.137.113 // pawel's  "192.168.137.36";
int server_port = 5004;// 4618;

float timeout_start;

void print_menu()
{
	std::cout << "\n***********************************";
	std::cout << "\n* ELEX4618 Client Project";
	std::cout << "\n***********************************";
	std::cout << "\n(1) Send image command";
	std::cout << "\n(2) Send other command";
	std::cout << "\n(0) Exit";
	std::cout << "\nCMD> ";
}

void send_command(CClient &client, std::string cmd)
{
	std::string str;

	client.tx_str(cmd);
	std::cout << "\nClient Tx: " << cmd;

	if (cmd == "im")
	{
		cv::Mat im;
		if (client.rx_im(im) == true)
		{
			timeout_start = cv::getTickCount();
			if (im.empty() == false)
			{
				std::cout << "\nClient Rx: Image received";
				cv::imshow("rx", im);
				cv::waitKey(10);
			}
		}
		else
		{
			if ((cv::getTickCount() - timeout_start) / cv::getTickFrequency() > 1000)
			{
				// No response, disconnect and reconnect
				timeout_start = cv::getTickCount();
				client.close_socket();
				client.connect_socket(server_ip, server_port);
			}
		}
	}
	else
	{
		if (client.rx_str(str) == true)
		{
			timeout_start = cv::getTickCount();
			std::cout << "\nClient Rx: " << str;
		}
		else
		{
			if ((cv::getTickCount() - timeout_start) / cv::getTickFrequency() > 1000)
			{
				// No response, disconnect and reconnect
				timeout_start = cv::getTickCount();
				client.close_socket();
				client.connect_socket(server_ip, server_port);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	CClient client;
	int cmd = -1;

	timeout_start = cv::getTickCount();
	client.connect_socket(server_ip, server_port);

	do
	{
		print_menu();
		std::cin >> cmd;

		switch (cmd)
		{
		case 1: send_command(client, "im"); break;
		case 2: send_command(client, "G 0"); break; //"cmd2"); break;
		case 3: {
			std::cout << "\nPress any key to exit\n";
			while (!_kbhit())
			{
				send_command(client, "im");
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
		} break;
		case 4: send_command(client, BIN1); break;
		case 5: send_command(client, BIN2); break;
		}
	} while (cmd != 0);
}
