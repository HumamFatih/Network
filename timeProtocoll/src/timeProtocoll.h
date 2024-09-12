/*
 * timeProtocoll.h
 *
 *  Created on: 17.06.2024
 *      Author: nwl
 */

#ifndef TIMEPROTOCOLL_H_
#define TIMEPROTOCOLL_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>

#define SOCKET_ERR -1
#define PORT_TIME 37

void getTime(char* buffer, char* format, char* hostIP);


#endif /* TIMEPROTOCOLL_H_ */
