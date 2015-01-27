/* 
 * File:   socketClass.cpp
 * Author: root
 * 
 * Created on Nedeľa, 2015, január 25, 19:09
 */

#include "socketClass.h"

socketClass::socketClass() {
}

bool socketClass::getConnected() {
    return this->connected;
}

const char * socketClass::getHostName() {
    return this->hostName;
}

int socketClass::getPortNumber() {
    return this->portNumber;
}

int socketClass::connectToServer(int portNum, const char *hostname) {
    this->portNumber = portNum;
    std::strcpy(this->hostName, hostname);
    std::cout << "connecting to server IP: " << hostName << ":" << portNumber << "\n";
    
    int n;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cout << "ERROR opening socket\n";
        return -1;
    }
    server = gethostbyname(hostName);
    if (server == NULL) {
        std::cout << "ERROR, no such host\n";
        return -1;
    }
    bzero((char *) &serv_addr, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
            (char *) &serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(portNumber);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        std::cout << "ERROR connecting\n";
        return -1;
    }
    connected = true;
    std::cout << "connected to server IP: " << hostName << ":" << portNumber << "\n";
    return 0;
}

int socketClass::disconnect() {
    // todo odpojime zo socketu ak je pripojeny
    if (!connected) {
        return -1;
    }
    
    close(sockfd);
    connected = false;
    std::cout << "agent disconnected\n";
    return 0;
}

int socketClass::sendJson(const char *jsonData) {
    if (!connected) {
        return -1;
    }
    
    int n;
    
    n = write(sockfd, jsonData, strlen(jsonData));
    if (n < 0) {
        std::cout << "ERROR writing to socket\n";
        return -1;
    }
    return n; // vratime pocet poslanych
}

int socketClass::receiveJson(char *buffer, int bufSize) {
    if (!connected) {
        return -1;
    }
    
    int n;
    
    bzero(buffer, bufSize);
    n = read(sockfd, buffer, bufSize);
    if (n < 0) {
        std::cout << "ERROR reading from socket\n";
        return -1;
    }
    return 0;
}

int socketClass::test() {
    if (!connected) {
        return -1;
    }
    char buf[256];
    sendJson("nejaky string");
    receiveJson(buf, 255);
    std::cout << buf << "\n";
    return 0;
    
    /*int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    //portno = widget.ipPortEdit->text().toInt();
    portno = 17005;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) std::cout << "ERROR opening socket\n";
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
            (char *) &serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) std::cout << "ERROR connecting\n";
    //printf("Please enter the message: ");
    //bzero(buffer, 256);
    //fgets(buffer, 255, stdin);
    //n = write(sockfd, buffer, strlen(buffer));
    n = write(sockfd, "nejaky string", strlen("nejaky string"));
    if (n < 0) std::cout << "ERROR writing to socket";
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) std::cout << "ERROR reading from socket";
    printf("%s\n", buffer);
    close(sockfd);*/
}

socketClass::~socketClass() {
    std::cout << "destruktor socketClass\n";
    disconnect();
}

