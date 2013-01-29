#ifndef CLIENT_H
#define CLIENT_H

//#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "convert.h"
#include "net.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

//#define DEFAULT_HOST "localhost"
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "20000"

//enum SEND_REC_FLAG {MOUSE_COORD};

class Client
{
private:
	std::string ip_addr;	
	std::string port;
	int buflen;	
	SOCKET *sock;	
public:
	Client();
	Client(std::string, std::string, int, SOCKET *);
	std::string get_ip();
	std::string get_port();
	SOCKET* Client::get_socket();
	int connect_client();	
	int shutdown_client();
	int get_input();

	int send_mouse_coord(float, float, float);
	int send_left_mouse_status(int);
	int send_ball_status(int);
	int send_ball_stop_flag(bool);
	int send_ball_moving_flag(bool);
	int send_ball_finished_moving_flag(bool);
	int send_ball_waiting_flag(bool);
	int send_ball_vel(float, float, float);
	int send_ball_in_hole_flag(bool);
	int send_ball_coord(float, float, float);
	
};

#endif