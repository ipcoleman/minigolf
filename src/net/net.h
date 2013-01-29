#ifndef NET_H
#define NET_H

#include <iostream>

#define DEFAULT_BUFLEN 512
#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT "20000"

// types of data send across the wire
enum SEND_REC_FLAG {MOUSE_COORD};

//void trim_str(char *);
//
//void trim_str(char *str)
//{
//	char *newline; // pointer to newline in str
//
//	// get pointer to newline
//	newline = strchr(str, '\n');
//	// set value to null char
//	if(newline)
//		*newline = '\0';
//}

#endif