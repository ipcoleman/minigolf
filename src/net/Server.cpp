#include "Server.h"
//#include "net.h"

using namespace std;

Server::Server()
{
	port = DEFAULT_PORT;
	buflen = DEFAULT_BUFLEN;	
	*listen_sock = INVALID_SOCKET;
	*client_sock = INVALID_SOCKET;
}

Server::Server(std::string prt, int blen, SOCKET *listen, SOCKET *client)
{
	port = prt;
	buflen = blen;
	listen_sock = listen;
	client_sock = client;
}

SOCKET* Server::get_listen_socket()
{
	return listen_sock;
}

SOCKET* Server::get_client_socket()
{
	return client_sock;
}

int Server::open_socket()
{
	WSADATA wsaData;
    int iResult;

    /*SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;*/

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    *listen_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (*listen_sock == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( *listen_sock, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(*listen_sock);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);
}

int Server::listen_server()
{
	//WSADATA wsaData;
    int iResult;

    /*SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;*/

    //struct addrinfo *result = NULL;
    //struct addrinfo hints;

    int iSendResult;
    //char recvbuf[DEFAULT_BUFLEN];
    //int recvbuflen = DEFAULT_BUFLEN;
    
    //// Initialize Winsock
    //iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    //if (iResult != 0) {
    //    printf("WSAStartup failed with error: %d\n", iResult);
    //    return 1;
    //}

    //ZeroMemory(&hints, sizeof(hints));
    //hints.ai_family = AF_INET;
    //hints.ai_socktype = SOCK_STREAM;
    //hints.ai_protocol = IPPROTO_TCP;
    //hints.ai_flags = AI_PASSIVE;

    //// Resolve the server address and port
    //iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    //if ( iResult != 0 ) {
    //    printf("getaddrinfo failed with error: %d\n", iResult);
    //    WSACleanup();
    //    return 1;
    //}

    //// Create a SOCKET for connecting to server
    //*listen_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    //if (*listen_sock == INVALID_SOCKET) {
    //    printf("socket failed with error: %ld\n", WSAGetLastError());
    //    freeaddrinfo(result);
    //    WSACleanup();
    //    return 1;
    //}

    //// Setup the TCP listening socket
    //iResult = bind( *listen_sock, result->ai_addr, (int)result->ai_addrlen);
    //if (iResult == SOCKET_ERROR) {
    //    printf("bind failed with error: %d\n", WSAGetLastError());
    //    freeaddrinfo(result);
    //    closesocket(*listen_sock);
    //    WSACleanup();
    //    return 1;
    //}

    //freeaddrinfo(result);

    iResult = listen(*listen_sock, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(*listen_sock);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    *client_sock = accept(*listen_sock, NULL, NULL);
    if (*client_sock == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(*listen_sock);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(*listen_sock);
}

int Server::shutdown_server()
{
	int iResult;
	// shutdown the connection since we're done
    iResult = shutdown(*client_sock, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(*client_sock);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(*client_sock);
    WSACleanup();

    return 0;
}

int Server::receive()
{
	int iResult;
	int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
	float *mouse_coord_test;
	//int num_index = 0;
	//char nums[2][DEFAULT_BUFLEN];

    // Receive until the peer shuts down the connection

	listen_left_mouse_status();
	//listen_mouse_coord();
	mouse_coord_test = listen_mouse_coord();
	cout << "receive(): Mouse x: " << mouse_coord_test[0] << endl;
	cout << "receive(): Mouse y: " << mouse_coord_test[1] << endl;
	cout << "receive(): Mouse z: " << mouse_coord_test[2] << endl;

	//-----------------------
	// generic receive function
	//-----------------------
  //  do {
		//cout << "Receiving..." << endl;		
  //      iResult = recv(*client_sock, recvbuf, recvbuflen, 0);		
  //      if (iResult > 0) {						
  //          //printf("Bytes received: %d\n", iResult);
		//	trim_str(recvbuf);
		//	//strcpy_s(nums[num_index], recvbuf);
		//	printf("This is what the client sent you: %s\n", recvbuf);			


		//	// Echo the buffer back to the sender
  //         /* iSendResult = send( *client_sock, recvbuf, iResult, 0 );
  //          if (iSendResult == SOCKET_ERROR) {
  //              printf("send failed with error: %d\n", WSAGetLastError());
  //              closesocket(*client_sock);
  //              WSACleanup();
  //              return 1;
  //          }
  //          printf("Bytes sent: %d\n", iSendResult);*/
  //      }
  //      else if (iResult == 0)
  //          printf("Connection closing...\n");
  //      else  {
  //          printf("recv failed with error: %d\n", WSAGetLastError());
  //          closesocket(*client_sock);
  //          WSACleanup();
  //          return 1;
  //      }

  //  } while (iResult > 0);	

	return 0;
}

float* Server::listen_mouse_coord()
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	float *mouse_coord = new float[3];
	int iResult;
	int index = 0;

	//cout << "Listening for mouse coordinates" << endl;
	
	for(int i=0; i<3; i++)
	{
		iResult = recv(*client_sock, recvbuf, recvbuflen, 0);
		if(iResult > 0)
		{
			trim_str(recvbuf);
			mouse_coord[i] = atof(recvbuf);
			cout << "Mouse Coordinate " << i << ": " << mouse_coord[i] << endl;
		}
	}
		
	return mouse_coord;
}

int Server::listen_ball_status()
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	//float mouse_coord[3];
	int iResult;
	int index = 0;
	int iBallStatus;
	bool bMouse_up = false;

	//cout << "Listening for ball status" << endl;
	
	/*for(int i=0; i<3; i++)
	{*/
	iResult = recv(*client_sock, recvbuf, recvbuflen, 0);
	if(iResult > 0)
	{
		trim_str(recvbuf);		
		iBallStatus = atoi(recvbuf);
		//cout << "Ball Status: " << iBallStatus << endl;
		//bMouse_up = (iMouse_up == 1 ? true : false);
		//cout << "Mouse up?: " << (bMouse_up ? "true" : "false") << endl;
	}
	//}

	return iBallStatus;
}

int Server::listen_left_mouse_status()
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	//float mouse_coord[3];
	int iResult;
	int index = 0;
	int iLeftMouseStatus;
	//bool bMouse_up = false;

	//cout << "Listening for mouse up" << endl;
	
	/*for(int i=0; i<3; i++)
	{*/
	iResult = recv(*client_sock, recvbuf, recvbuflen, 0);
	if(iResult > 0)
	{
		trim_str(recvbuf);		
		iLeftMouseStatus = atoi(recvbuf);
		//bMouse_up = (iMouse_up == 1 ? true : false);
		//cout << "Mouse up?: " << (bMouse_up ? "true" : "false") << endl;
	}
	//}

	return iLeftMouseStatus;
}

//bool Server::listen_mouse_up_flag()
//{
//	char recvbuf[DEFAULT_BUFLEN];
//	int recvbuflen = DEFAULT_BUFLEN;
//	//float mouse_coord[3];
//	int iResult;
//	int index = 0;
//	int iMouse_up;
//	bool bMouse_up = false;
//
//	//cout << "Listening for mouse up" << endl;
//	
//	/*for(int i=0; i<3; i++)
//	{*/
//	iResult = recv(*client_sock, recvbuf, recvbuflen, 0);
//	if(iResult > 0)
//	{
//		trim_str(recvbuf);		
//		iMouse_up = atoi(recvbuf);
//		bMouse_up = (iMouse_up == 1 ? true : false);
//		//cout << "Mouse up?: " << (bMouse_up ? "true" : "false") << endl;
//	}
//	//}
//
//	return bMouse_up;
//}

bool Server::listen_ball_stop_flag()
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	//float mouse_coord[3];
	int iResult;
	int index = 0;
	int iStopped;
	bool bStopped = false;

	//cout << "Listening for ball stopped" << endl;
	
	/*for(int i=0; i<3; i++)
	{*/
	iResult = recv(*client_sock, recvbuf, recvbuflen, 0);
	if(iResult > 0)
	{
		trim_str(recvbuf);		
		iStopped = atoi(recvbuf);
		bStopped = (iStopped == 1 ? true : false);
		//cout << "Ball stopped?: " << (bStopped ? "true" : "false") << endl;
	}
	//}

	return bStopped;
}

bool Server::listen_ball_moving_flag()
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	//float mouse_coord[3];
	int iResult;
	int index = 0;
	int iMoving;
	bool bMoving = false;

	//cout << "Listening for ball moving flag" << endl;
	
	/*for(int i=0; i<3; i++)
	{*/
	iResult = recv(*client_sock, recvbuf, recvbuflen, 0);
	if(iResult > 0)
	{
		trim_str(recvbuf);		
		iMoving = atoi(recvbuf);
		bMoving = (iMoving == 1 ? true : false);
		//cout << "Mouse up?: " << (bMoving ? "true" : "false") << endl;
	}
	//}

	return bMoving;
}

bool Server::listen_ball_waiting_flag()
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	//float mouse_coord[3];
	int iResult;
	int index = 0;
	int iWaiting;
	bool bWaiting = false;

	//cout << "Listening for ball waiting to be hit" << endl;
	
	/*for(int i=0; i<3; i++)
	{*/
	iResult = recv(*client_sock, recvbuf, recvbuflen, 0);
	if(iResult > 0)
	{
		trim_str(recvbuf);		
		iWaiting = atoi(recvbuf);
		bWaiting = (iWaiting == 1 ? true : false);
		//cout << "Mouse up?: " << (bWaiting ? "true" : "false") << endl;
	}
	//}

	return bWaiting;
}

float* Server::listen_ball_vel()
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	float *ball_vel = new float[3];
	int iResult;
	int index = 0;

	//cout << "Listening for ball vel" << endl;
	
	for(int i=0; i<3; i++)
	{
		iResult = recv(*client_sock, recvbuf, recvbuflen, 0);
		if(iResult > 0)
		{
			trim_str(recvbuf);
			ball_vel[i] = atof(recvbuf);
			cout << "Ball Vel " << i << ": " << ball_vel[i] << endl;
		}
	}
		
	return ball_vel;
}

bool Server::listen_ball_in_hole_flag()
{
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	//float mouse_coord[3];
	int iResult;
	int index = 0;
	int iInHole;
	bool bInHole = false;

	//cout << "Listening for ball in hole flag" << endl;
	
	/*for(int i=0; i<3; i++)
	{*/
	iResult = recv(*client_sock, recvbuf, recvbuflen, 0);
	if(iResult > 0)
	{
		trim_str(recvbuf);		
		iInHole = atoi(recvbuf);
		bInHole = (iInHole == 1 ? true : false);
		//cout << "Mouse up?: " << (bInHole ? "true" : "false") << endl;
	}
	//}

	return bInHole;
}

