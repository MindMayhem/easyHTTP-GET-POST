#include <iostream>
#include <string>

#pragma comment (lib, "wsock32.lib")

#include <stdlib.h>
#include <winsock.h>
#include <sstream>


using namespace std;

#define BUFFERSIZE 1024
void die_with_error(char* errorMessage);
void die_with_wserror(char* errorMessage);

string HTTP_POST(string host, string path, string data) {
    string request;
    string response;
    int resp_leng;

    char buffer[BUFFERSIZE];
    struct sockaddr_in serveraddr;
    int sock;

    WSADATA wsaData;

    struct hostent* ipadd = gethostbyname(host.c_str());
    int port = 80;

    std::stringstream ss;
    ss << data.length();

    std::stringstream request2;

    request2 << "POST " << path << " HTTP/1.1" << endl;
    request2 << "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)" << endl;
    //request2 << "" << endl;
    request2 << "Host: " << host << endl;
    request2 << "Content-Length: " << data.length() << endl;

    request2 << "Content-Type: application/x-www-form-urlencoded" << endl;
    request2 << "Accept-Language: en-au" << endl;
    request2 << endl;
    request2 << data;
    request = request2.str();
    //init winsock
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
        die_with_wserror("WSAStartup() failed");
    //open socket
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        die_with_wserror("socket() failed");

    //connect
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = *((unsigned long*)ipadd->h_addr);
    serveraddr.sin_port = htons((unsigned short)port);
    if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
        die_with_wserror("connect() failed");

    //send request
    if (send(sock, request.c_str(), request.length(), 0) != request.length())
        die_with_wserror("send() sent a different number of bytes than expected");

    //get response
    response = "";
    resp_leng = BUFFERSIZE;
    while (resp_leng == BUFFERSIZE)
    {
        resp_leng = recv(sock, (char*)&buffer, BUFFERSIZE, 0);
        if (resp_leng > 0)
            response += string(buffer).substr(0, resp_leng);
        //note: download lag is not handled in this code
    }

    //disconnect
    closesocket(sock);

    //cleanup
    WSACleanup();

    return  response;
}


string HTTP_GET(string host, string path) {
    string request;
    string response;
    int resp_leng;

    char buffer[BUFFERSIZE];
    struct sockaddr_in serveraddr;
    int sock;

    WSADATA wsaData;
    struct hostent *ipadd = gethostbyname(host.c_str());
    int port = 80;

    std::stringstream ss;

    std::stringstream request2;

    request2 << "GET " << path << " HTTP/1.1" << endl;
    request2 << "User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)" << endl;
    //request2 << "" << endl;
    request2 << "Host: " << host << endl;

    request2 << "Connection: close" << endl;
    request2 << endl;
    request = request2.str();
    //init winsock
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
        die_with_wserror("WSAStartup() failed");
    //open socket
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        die_with_wserror("socket() failed");

    //connect
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = *((unsigned long*)ipadd->h_addr);
    serveraddr.sin_port = htons((unsigned short)port);
    if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
        die_with_wserror("connect() failed");

    //send request
    if (send(sock, request.c_str(), request.length(), 0) != request.length())
        die_with_wserror("send() sent a different number of bytes than expected");

    //get response
    response = "";
    resp_leng = BUFFERSIZE;
    while (resp_leng == BUFFERSIZE)
    {
        resp_leng = recv(sock, (char*)&buffer, BUFFERSIZE, 0);
        if (resp_leng > 0)
            response += string(buffer).substr(0, resp_leng);
        //note: download lag is not handled in this code
    }

    //disconnect
    closesocket(sock);

    //cleanup
    WSACleanup();

    return response.substr(response.find("\r\n\r\n"));
}


void die_with_error(char* errorMessage)
{
    cerr << errorMessage << endl;
    cin.get();
    exit(1);
}

void die_with_wserror(char* errorMessage)
{
    cerr << errorMessage << ": " << WSAGetLastError() << endl;
    cin.get();
    exit(1);
}