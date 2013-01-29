#include "Client.h"
//#include "net.h"

//*****************************************
// Much of the Winsock client/server
// setup code comes from a tutorial on a 
// MSDN Winsock site
// http://msdn.microsoft.com/en-us/library/ms738545%28v=vs.85%29.aspx
//*****************************************

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")

using namespace std;



Client::Client()
{
	ip_addr = DEFAULT_HOST;
	std::string port = DEFAULT_PORT;
	int buflen = DEFAULT_BUFLEN;
	*sock = INVALID_SOCKET;
}

Client::Client(std::string ip, std::string prt, int blen, SOCKET *socket)
{
	ip_addr = ip;
	port = prt;
	buflen = blen;
	sock = socket;
}

std::string Client::get_ip()
{
	return ip_addr;
}

std::string Client::get_port()
{
	return port;
}

SOCKET* Client::get_socket()
{
	return sock;
}

int Client::connect_client()
{
	WSADATA wsaData;	
    struct addrinfo *result = NULL, 
					*ptr = NULL,
                    hints;
    int iResult;
    
	//cout << "Start of connect_client()" << endl;
    // Validate the parameters
	if (ip_addr.length() == 0) {
		cout << "Error: no IP address" << endl;
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
	iResult = getaddrinfo(ip_addr.c_str(), port.c_str(), &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        *sock = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (*sock == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( *sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
			cout << "connect() error" << endl;
            closesocket(*sock);
            *sock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (*sock == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

}

int Client::shutdown_client()
{
    // cleanup
    closesocket(*sock);
    WSACleanup();

	return 0;
}

int Client::get_input()
{
	// get input
	char nums[2][DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

	while(strcmp(sendbuf, "quit\n") != 0)
	{		
		//cout << "Please enter a number: " << endl;	
		//fgets(sendbuf, DEFAULT_BUFLEN, stdin);
		//strcpy_s(nums[0], sendbuf);

		cout << "Please enter a number: " << endl;	
		fgets(sendbuf, DEFAULT_BUFLEN, stdin);
		//strcpy_s(nums[1], sendbuf);

		//printf("Sum: %d\n", atoi(nums[0]) + atoi(nums[1]));

		// Send an initial buffer
		/*cout << "Please enter a string to send across wire: " << endl;
		fgets(sendbuf, DEFAULT_BUFLEN, stdin);*/

		if(strcmp(sendbuf, "quit\n") == 0)
			break;

		// send buffer to server
		iResult = send( *sock, sendbuf, (int)strlen(sendbuf), 0 );

		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(*sock);
			WSACleanup();
			return 1;
		}

		printf("Bytes Sent: %ld\n", iResult);
	}

    // shutdown the connection since no more data will be sent
    iResult = shutdown(*sock, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(*sock);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(*sock, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
		{
            printf("Bytes received: %d\n", iResult);
			printf("This is what you sent the server: %s\n", recvbuf);
		}
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );
}

int Client::send_mouse_coord(float x, float y, float z)
{	
	char sendbuf[3][DEFAULT_BUFLEN];
	char sendbuf_ch[DEFAULT_BUFLEN];
    char recvbuf[DEFAULT_BUFLEN];
	int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
	float mouse_coord[3];

	mouse_coord[0] = x;
	mouse_coord[1] = y;
	mouse_coord[2] = z;

	//cout << "Sending mouse coordinates" << endl;
	for(int i=0; i<3; i++)
	{
		// copy mouse coords into buffers
		strcpy_s(sendbuf_ch, float_to_str(mouse_coord[i]).c_str());
		// send mouse coordinates
		cout << "Sending mouse coord: " << sendbuf_ch << endl;
		iResult = send( *sock, sendbuf_ch, sizeof(sendbuf_ch), 0 );		

		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(*sock);
			WSACleanup();
			return 1;
		}
	}
	

	//printf("Bytes Sent: %ld\n", iResult);
	//}
}

int Client::send_left_mouse_status(int iLeftMouseStatus)
{
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
	//int iMouse_up;

	//cout << "Sending mouse up flag" << endl;
	//iMouse_up = (bMouse_up ? 1 : 0); //1 = true; 0 = false
	// copy mouse up flag into buffer
	strcpy_s(sendbuf, int_to_str(iLeftMouseStatus).c_str());
	// send mouse coordinates
	//cout << "Mouse up?: " << sendbuf << endl;
	iResult = send( *sock, sendbuf, sizeof(sendbuf), 0 );

	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(*sock);
		WSACleanup();
		return 1;
	}
}

int Client::send_ball_status(int iStatus)
{
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
	//int iWaiting;
	
	//iWaiting = (bWaiting ? 1 : 0); //1 = true; 0 = false
	// copy mouse up flag into buffer
	strcpy_s(sendbuf, int_to_str(iStatus).c_str());
	// send mouse coordinates
	//cout << "Ball Status: " << sendbuf << endl;
	iResult = send( *sock, sendbuf, sizeof(sendbuf), 0 );

	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(*sock);
		WSACleanup();
		return 1;
	}
}

int Client::send_ball_waiting_flag(bool bWaiting)
{
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
	int iWaiting;
	
	iWaiting = (bWaiting ? 1 : 0); //1 = true; 0 = false
	// copy mouse up flag into buffer
	strcpy_s(sendbuf, int_to_str(iWaiting).c_str());
	// send mouse coordinates
	//cout << "Ball waiting to be hit?: " << sendbuf << endl;
	iResult = send( *sock, sendbuf, sizeof(sendbuf), 0 );

	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(*sock);
		WSACleanup();
		return 1;
	}
}

int Client::send_ball_moving_flag(bool bMoving)
{
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
	int iMoving;
	
	iMoving = (bMoving ? 1 : 0); //1 = true; 0 = false
	// copy mouse up flag into buffer
	strcpy_s(sendbuf, int_to_str(iMoving).c_str());
	// send mouse coordinates
	//cout << "Ball waiting to be hit?: " << sendbuf << endl;
	iResult = send( *sock, sendbuf, sizeof(sendbuf), 0 );

	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(*sock);
		WSACleanup();
		return 1;
	}
}

int Client::send_ball_vel(float x, float y, float z)
{
	//char sendbuf[3][DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
    char recvbuf[DEFAULT_BUFLEN];
	int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
	float vel[3];

	vel[0] = x;
	vel[1] = y;
	vel[2] = z;

	//cout << "Sending velocity" << endl;
	for(int i=0; i<3; i++)
	{
		// copy mouse coords into buffers
		strcpy_s(sendbuf, float_to_str(vel[i]).c_str());
		// send mouse coordinates
		cout << "Sending velocity: " << i << ": " << sendbuf << endl;
		iResult = send( *sock, sendbuf, sizeof(sendbuf), 0 );		

		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(*sock);
			WSACleanup();
			return 1;
		}
	}
	

	//printf("Bytes Sent: %ld\n", iResult);
	//}
}

int Client::send_ball_stop_flag(bool bStopped)
{
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
	int iStopped;
	
	iStopped = (bStopped ? 1 : 0); //1 = true; 0 = false
	// copy mouse up flag into buffer
	strcpy_s(sendbuf, int_to_str(iStopped).c_str());
	// send mouse coordinates
	//cout << "Ball stopped?: " << sendbuf << endl;
	iResult = send( *sock, sendbuf, sizeof(sendbuf), 0 );

	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(*sock);
		WSACleanup();
		return 1;
	}
}

int Client::send_ball_coord(float x, float y, float z)
{
	//char sendbuf[3][DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
    char recvbuf[DEFAULT_BUFLEN];
	int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
	float ball_coord[3];

	ball_coord[0] = x;
	ball_coord[1] = y;
	ball_coord[2] = z;

	//cout << "Sending ball coord" << endl;
	for(int i=0; i<3; i++)
	{
		// copy mouse coords into buffers
		strcpy_s(sendbuf, float_to_str(ball_coord[i]).c_str());
		// send mouse coordinates
		cout << "Sending ball coord: " << i << ": " << sendbuf << endl;
		iResult = send( *sock, sendbuf, sizeof(sendbuf), 0 );		

		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(*sock);
			WSACleanup();
			return 1;
		}
	}
	

	//printf("Bytes Sent: %ld\n", iResult);
	//}
}

int Client::send_ball_finished_moving_flag(bool bFinishedMoving)
{
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
	int iFinishedMoving;
	
	iFinishedMoving = (bFinishedMoving ? 1 : 0); //1 = true; 0 = false
	// copy mouse up flag into buffer
	strcpy_s(sendbuf, int_to_str(iFinishedMoving).c_str());
	// send mouse coordinates
	//cout << "Ball stopped?: " << sendbuf << endl;
	iResult = send( *sock, sendbuf, sizeof(sendbuf), 0 );

	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(*sock);
		WSACleanup();
		return 1;
	}
}

