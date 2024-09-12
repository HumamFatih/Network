/*
 * timeProtocoll.c
 *
 *  Created on: 17.06.2024
 *      Author: nwl
 */

#include "timeProtocoll.h"

void getTime(char* buffer, char* format, char* hostIP){
	int socket_; // Socket
	struct sockaddr_in serverAddress; // Severadresse struktur
	socklen_t serverLength = sizeof(serverAddress); // Größe der Serveradresse
	char receiveBuffer[32]; // Puffer
	char request = 0; // Anforderung an den Time-Server
	time_t serverTime; // Emfangene Zeit vom Server
	struct tm* actualTime; // aktuelle Time (lokal time)
	int returnValue; // Rückgabewert für sendto und recvfrom

	// Ein Socket erzeugen
	socket_ = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_ == SOCKET_ERR){
		perror("Fail to create socket");
		return;
	}

	// Die Server-Adresse konfigurieren
	serverAddress.sin_addr.s_addr = inet_addr(hostIP);
	serverAddress.sin_family = AF_INET; // IPv4
	serverAddress.sin_port = htons(PORT_TIME); // Port 37

	// Ein Request an den Time-Server sendet
	returnValue = sendto(socket_, &request, sizeof(request), 0,
			(struct sockaddr*)&serverAddress, serverLength);
	if(returnValue == SOCKET_ERR){
		perror("Fail to send");
		close(socket_);
		return;
	}

	// die Zeit vom Server empfangen
	returnValue = recvfrom(socket_, receiveBuffer, sizeof(receiveBuffer), 0,
			(struct sockaddr*) &serverAddress, &serverLength);
	if(returnValue == SOCKET_ERR){
		perror("Fail to receive");
		close(socket_);
		return;
	}

	// Socket schließen
	close(socket_);

	// erhaltene Zeit in host byte order konvertieren
	serverTime = ntohl(*(uint32_t*)receiveBuffer) - 2208988800U;

	// Die Zeit in der lokalen Zeit umwandeln und mit strftime formatieren
	actualTime = localtime(&serverTime);
	if(strftime(buffer, 32, format, actualTime) == 0){
		perror("Characters exceed maximum size");
		return;
	}
}



