#ifndef SERVER_H
#define SERVER_H

#undef UNICODE

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

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "20000"

//enum SEND_REC_FLAG {MOUSE_COORD};

class Server {
private:		
	std::string port;
	int buflen;	
	SOCKET *listen_sock;
	SOCKET *client_sock;	
public:
	Server();
	Server(std::string, int, SOCKET*, SOCKET*);
	SOCKET* get_listen_socket();
	SOCKET* get_client_socket();
	int open_socket();
	int listen_server();
	int shutdown_server();
	int receive();

	float* listen_mouse_coord();
	//bool listen_mouse_up_flag();
	int listen_left_mouse_status();
	int listen_ball_status();
	bool listen_ball_stop_flag();
	bool listen_ball_moving_flag();
	bool listen_ball_finished_moving();
	bool listen_ball_waiting_flag();
	float* listen_ball_vel();
	bool listen_ball_in_hole_flag();
	bool listen_ball_waiting();
	float* listen_ball_coord();
	int sync_ball_coord(float, float, float);

};

#endif