#include <stdio.h>
#include <iostream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib")


int main(void)
{
	const int PORT = 8888; //Server Port
	const string SERVERADDRESS = "127.0.0.1"; //server adresse
	const int MESSAGELENGTH = 512; //max Länger der Nachricht

	SOCKET udpSocket;	//Socket Objekt erstellen, benötigt für socket()
	struct sockaddr_in serverAdress, clientAddress; //Structs in denen wir die Daten zum Server und Client Speichern
	int slen, recv_len; //Variablen gebraucht in sendto() und recvfrom()
	char message[MESSAGELENGTH];	//in message speichern wir die erhaltenen/versendeten Daten
	WSADATA wsaData;	//WSADATA Objekt, benötigt für WSAStartup()
	slen = sizeof(clientAddress); //Länge der Client Adresse, gebruacht in sendto()

	// Initialise Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "WSAStartup fehlgeschlagen: " << WSAGetLastError() << endl;
		system("pause");
		return 1;
	}

	// Create UDP socket
	if ((udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		cout << "Socket erstellen fehlgeschlagen: " << WSAGetLastError() << endl;
		system("pause");
		return 1;
	}


	memset((char *)&serverAdress, 0, sizeof(serverAdress)); //Reserviert Platz im Memory für die Daten
	//Block 1
	serverAdress.sin_family = AF_INET;
	serverAdress.sin_port = htons(PORT);
	serverAdress.sin_addr.S_un.S_addr = inet_addr(SERVERADDRESS.c_str());

	// Block 2
	printf("Enter message: ");
	cin.getline(message, MESSAGELENGTH);
	if (sendto(udpSocket, message, strlen(message), 0, (struct sockaddr *) &serverAdress, slen) == SOCKET_ERROR)
	{
		cout << "Daten senden fehlgeschlagen: " << WSAGetLastError() << endl;
		system("pause");
		return 1;
	}

	// Block 3
	closesocket(udpSocket);
	WSACleanup();

	system("pause");
	return 0;
}