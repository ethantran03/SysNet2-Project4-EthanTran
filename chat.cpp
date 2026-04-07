#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
using namespace std;

void senderThreadBody(int connectionFileDescriptor) {
    char buffer[1024];
    while (true) {
        cin.getline(buffer, 1024);
        write(connectionFileDescriptor, buffer, strlen(buffer) + 1);
        memset(buffer, 0, 1024);
    }
}

void receiverThreadBody(int connectionFileDescriptor) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, 1024);
        if (read(connectionFileDescriptor, buffer, 1024) <= 0) break;
        cout << "Friend: " << buffer << endl;
    }
}

int main(int argc, char* argv[]) {
    int socketFileDescriptor, connectionFileDescriptor;
    sockaddr_in socketIPandPort;
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    socketIPandPort.sin_family = AF_INET;

    if (argc == 2) {
        socketIPandPort.sin_addr.s_addr = INADDR_ANY;
        socketIPandPort.sin_port = htons(stoi(argv[1]));
        bind(socketFileDescriptor, (sockaddr*)&socketIPandPort, sizeof(socketIPandPort));
        listen(socketFileDescriptor, 5);
        connectionFileDescriptor = accept(socketFileDescriptor, NULL, NULL);
    }
    else {
        socketIPandPort.sin_addr.s_addr = inet_addr(argv[1]);
        socketIPandPort.sin_port = htons(stoi(argv[2]));
        connect(socketFileDescriptor, (sockaddr*)&socketIPandPort, sizeof(socketIPandPort));
        connectionFileDescriptor = socketFileDescriptor;
    }

    thread senderThread(senderThreadBody, connectionFileDescriptor);
    thread receiverThread(receiverThreadBody, connectionFileDescriptor);
    senderThread.join();
    receiverThread.join();
    return 0;
}