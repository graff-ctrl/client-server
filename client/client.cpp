

// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#define PORT 12008

//Funtion to connect to server.


int disconnectRPC()
{
    // input format="rpc=disconnect"
    // output format="status=<error status>; error=<error or blank>;
    return 0;
}

int foobarRPC()
{
    return 0;
}

int incrementRPC(int & sock, const char *buff)
{
    size_t valRead=0;
    char buffer[1024] = { 0 };
    send(sock, buff, strlen(buff), 0);
    printf("Hello message sent\n");
    if (strcmp(buff, "rpc=disconnect;") == 0) {
        return 0;
    }
    else {
        valRead = read(sock, buffer, 1024);
        printf("ValRead=%lu buffer=%s\n", valRead, buffer);
    }
    return 0;
}

int connectToServer(char *szHostName, char *szPort, int & sock)
{

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    uint16_t port = (uint16_t) atoi(szPort);

    serv_addr.sin_port = htons(port);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);


    if (inet_pton(AF_INET, szHostName, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    return 0;
}


int disconnectServer(int sock)
{
    int status;
    status = close(sock);
    return status;
}
int connectRPC(char *userName, char *password, char *buff)
{
    const char *un = "rpc=connect;username=";
    const char *pw = ";password=";
    strcpy(buff, un);
    strcat(buff, userName);
    strcat(buff, pw);
    strcat(buff, password);
    printf("%s", buff);
    return 0;
}
int disconnectRPC(int & sock)
{
    const char *disconnect = "rpc=disconnect;";
    incrementRPC(sock, disconnect);
    return 0;

}

int tipRPC(int & sock)
{
    const char *tip = "rpc=tip;";
    incrementRPC(sock, tip);
    return 0;
}

int adviceRPC(int & sock)
{
    const char *advice = "rpc=advice;";
    incrementRPC(sock, advice);
    return 0;
}

int quoteRPC(int & sock)
{
    const char *quote = "rpc=quote;";
    incrementRPC(sock, quote);
    return 0;
}

int main(int argc, char const *argv[])
{

// send port number from server in command line (i.e. ./client 127.0.0.1 12008)
    int sock = 0;
    int status;
    //char buff[1024];
//    const char *tip = "rpc=tip;";
//    const char *advice = "rpc=advice;";
//    const char *quote = "rpc=quote;";
    // We will find out how many times to send our hello

    status = connectToServer((char *) argv[1], (char *) argv[2], sock);
    // Asking user to input credentials to connect to server (user input always in main).
    //char username[20];
    //char password[20];

/*  std::cout << "\nEnter Username:";
    std::cin >> username;
    std::cout << "\nEnter Password:";
    std::cin >> password;
    connectRPC(username,password, buff);
    incrementRPC(sock, buff);*/

    for (int i = 0; i < 10; i++)
    {
        tipRPC(sock);
        sleep(1);
        adviceRPC(sock);
        sleep (1);
        //quoteRPC(sock);
        //sleep (1);
    }
    disconnectRPC(sock);
    //incrementRPC(sock, buff);

    status = disconnectServer(sock);

    return status;
}
