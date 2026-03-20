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
    struct sockaddr_in serverAdress, localAdress;
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

    memset((char *)&localAdress, 0, sizeof(localAdress));
    localAdress.sin_family = AF_INET;
    localAdress.sin_addr.s_addr = INADDR_ANY;
    localAdress.sin_port = htons(PORT);

    if (bind(udpSocket, (struct sockaddr *)&localAdress, sizeof(localAdress)) == SOCKET_ERROR)
    {
        cout << "Bind fehlgeschlagen! Fehler: " << WSAGetLastError() << endl;
    }

    memset((char *)&serverAdress, 0, sizeof(serverAdress));
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_port = htons(PORT);
    serverAdress.sin_addr.S_un.S_addr = inet_addr(SERVERADDRESS.c_str());


    while (true)
    {
        printf("\nDeine Nachricht: ");
        cin.getline(message, MESSAGELENGTH);

        // Nachricht an den Relay-Server senden
        if (sendto(udpSocket, message, strlen(message), 0, (struct sockaddr *) &serverAdress, slen) == SOCKET_ERROR)
        {
            cout << "Senden fehlgeschlagen: " << WSAGetLastError() << endl;
            break;
        }

        // Puffer leeren für den Empfang
        memset(message, '\0', MESSAGELENGTH);

        if ((recv_len = recvfrom(udpSocket, message, MESSAGELENGTH, 0, (struct sockaddr *) &serverAdress, &slen)) == SOCKET_ERROR)
        {
            cout << "Empfangen fehlgeschlagen: " << WSAGetLastError() << endl;
            break;
        }

        cout << "Eingang: " << message << endl;
    }

    closesocket(udpSocket);
    WSACleanup();
    return 0;
}