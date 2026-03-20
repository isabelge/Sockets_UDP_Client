#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <string>

using namespace std;

#pragma comment(lib,"ws2_32.lib")

int main(void)
{
    const int PORT = 8888;
    const string SERVERADDRESS = "192.168.1.128";
    const int MESSAGELENGTH = 512;

    SOCKET udpSocket;
    struct sockaddr_in serverAdress;
    int slen, recv_len;
    char message[MESSAGELENGTH];
    WSADATA wsaData;
    slen = sizeof(serverAdress);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup fehlgeschlagen: " << WSAGetLastError() << endl;
        return 1;
    }

    if ((udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
    {
        cout << "Socket erstellen fehlgeschlagen: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    memset((char *)&serverAdress, 0, sizeof(serverAdress));
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_port = htons(PORT);
    serverAdress.sin_addr.S_un.S_addr = inet_addr(SERVERADDRESS.c_str());

    cout << "Client bereit. Tippe deine Nachrichten (Endlosschleife):" << endl;

    while (true)
    {
        printf("\nDeine Nachricht: ");
        cin.getline(message, MESSAGELENGTH);

        // 1. Nachricht an den Server senden
        if (sendto(udpSocket, message, strlen(message), 0, (struct sockaddr *) &serverAdress, slen) == SOCKET_ERROR)
        {
            cout << "Senden fehlgeschlagen: " << WSAGetLastError() << endl;
            break;
        }

        // Puffer leeren für den Empfang
        memset(message, '\0', MESSAGELENGTH);

        // 2. Auf Antwort vom Server warten (Echo)
        if ((recv_len = recvfrom(udpSocket, message, MESSAGELENGTH, 0, (struct sockaddr *) &serverAdress, &slen)) == SOCKET_ERROR)
        {
            cout << "Empfangen fehlgeschlagen: " << WSAGetLastError() << endl;
            break;
        }

        cout << "Antwort vom Server: " << message << endl;
    }

    closesocket(udpSocket);
    WSACleanup();

    return 0;
}