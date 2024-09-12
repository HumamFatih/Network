/*
 ============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "timeProtocoll.h"

#define END_OF_FILE 0;

int main(void) {
	// Puffer für die empfangene Zeit vom Server
	char buffer[32];

	// IP-Adresse
	char ipAddress [] = "192.168.60.70";

	// Format zur Ausgabe der Zeit
	char format [] = "%d.%m.%Y %H:%M";

	// Die Funktion getTime aufrufen
	getTime(buffer, format, ipAddress);

	// Zeit im Konsole ausgeben
	printf("Time is %s \n", buffer);

	int sock; // Variable für den Socket
	struct sockaddr_in serverAdress; // Struktur der Serveradresse
	socklen_t adressLength = sizeof(serverAdress); // Lange der Serveradresse
	int dataPort = 44001; // Portnummer für die Verbindung
	int tcpConnect; // Variable für das Verbindungsstatus
	char tcpBuffer[4096]; // Puffer für empfangene Daten vom Server
	char welcomeMessage[4096] = ""; // Puffer die Nachtricht
	int linesNumber = 0; // Zahler für die Zeilen
	char* newPortStr; // Zeiger auf den Portnummer String
	uint16_t newPortInt; // Integerwert der Portnummer

	// Ein Stream Socket erstellen
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == SOCKET_ERR){
		fprintf(stderr, "socket is failed with error %d ", errno);
		perror("");
		return -1;
	}

	// Konfigurieren der Serveradresse
	serverAdress.sin_addr.s_addr = inet_addr(ipAddress);
	serverAdress.sin_family = AF_INET;
	serverAdress.sin_port = htons(dataPort);

	// TCP Verbindung zum Server erzeugen
	tcpConnect = connect(sock, (struct sockaddr *)& serverAdress, adressLength);
	if(tcpConnect == SOCKET_ERR){
		fprintf(stderr, "Connecton is failed with error %d ", errno);
		perror("");
		close(sock);
		return -1;
	}

	// Endlose Schliefe zum Emfang von Daten vom Server
	for(;;){
		int retval;

		// Daten vom Server empfangen und Prüfen, ob ein Fehler aufgetreten ist
		// oder das Ende der Datei erreicht wird
		retval = recv(sock, tcpBuffer, sizeof(tcpBuffer),0);
		if(retval == SOCKET_ERR || EOF == retval){
			break;
		}

		// Empfangene Daten null terminieren
		tcpBuffer[retval] = 0x0;

		// Empfangene Daten an die gesammte Nachricht einhängen
		strcat(welcomeMessage, tcpBuffer);

		// Zahlen im empfangenen Puffer zählen
		int length = strlen(tcpBuffer);
		for(int i = 0; i < length ; i++){
			if(tcpBuffer[i] == '\n'){
				linesNumber++;
			}
		}

		// Prüffen, ob 3 Zeilen schon empfangen werden
		if(linesNumber >=3){
			break;
		}
	}

	// Empfangene Nachricht ausgeben
	printf("\nMessage received :\n%s", welcomeMessage);

	// Die Portnummer aus der letzten Zeile extrahieren
	newPortStr = rindex(welcomeMessage, '\n') - 6;

	// Von String in Integer umwandeln
	newPortInt = atoi(newPortStr);

	// Die Portnummer als String und Integer ausgeben
	printf("Port number in string : %s", newPortStr);
	printf("Port number in integer : %d\n", newPortInt);

	// Socket schließen
	close(sock);

	// der erweiterte Zeitstring vorbereiten
	char stringToSend[256];
	snprintf(stringToSend, sizeof(stringToSend), "(%lu) %s\n",
			strlen(welcomeMessage), buffer);

	// Ein Stream Socket für neuen Port erstellen
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == SOCKET_ERR){
		fprintf(stderr, "socket is failed with error %d ", errno);
		perror("");
		return -1;
	}

	// Die Serveradresse mit neuem Port einstellen
	serverAdress.sin_port = htons(newPortInt);

	// die Lange der Serveradresse speichern
	adressLength = sizeof(serverAdress);

	// TCP Verbindung zum Server mit neuem Port erzeugen
	tcpConnect = connect(sock, (struct sockaddr*)&serverAdress, adressLength);
	if(tcpConnect == SOCKET_ERR){
		fprintf(stderr, "Connecton is failed with error %d ", errno);
		perror("");
		close(sock);
		return -1;
	}

	// der erweiterte Zeit-String an den Server schicken
	int retval = send(sock, stringToSend, strlen(stringToSend),0);
	if(retval == SOCKET_ERR){
		fprintf(stderr, "Sending is failed with error %d ", errno);
		perror("");
		close(sock);
		return -1;
	}

	// Socket schlißen
	close(sock);

	// gesendete Zeit-String ausgeben
	printf("\nMessage is sent!\n");
	printf("Message : %s", stringToSend);
}
