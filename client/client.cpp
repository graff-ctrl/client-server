
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


/**
 * This function handles the requests to connect/disconnect threads to the server.
 * @param sock - the current client's socket ID
 * @param buff - the character buffer to send/return messagages between servers
 * @return - status ID
 */
int incrementRPC(int & sock, const char *buff)
{
    char buffer[1024] = { 0 };
    send(sock, buff, strlen(buff), 0);

    if (strcmp(buff, "rpc=disconnect;") == 0) {
        read(sock, buffer, 1024);
        printf("%s\n", buffer);
        return 0;
    }
    else {
        read(sock, buffer, 1024);
        printf("%s\n", buffer);
    }
    return 0;
}

/**
 * This function makes the initial connection to the server.
 * @param szHostName - the IP address of the server (cs1)
 * @param szPort - the server port number. Note: unique to each cs1 user.
 * @param sock - the socket ID
 * @return - function status
 */
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

/**
 * This function disconnects the current client thread from the server.
 * @param sock - socket ID
 * @return - error status
 */
int disconnectServer(int sock)
{
    int status;
    status = close(sock);
    return status;
}

/**
 * This function reads in console input username and password credentials,
 * reformats, and sends to the server.
 * @param userName - client's username
 * @param password - client's password
 * @param buff - message buffer to server
 * @return - error status
 */
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

/**
 * This function disconnects the client thread from the server.
 * @param sock - socket ID
 * @return - error status
 */
int disconnectRPC(int & sock)
{
    const char *disconnect = "rpc=disconnect;";
    incrementRPC(sock, disconnect);
    return 0;

}

/**
 * This function sends a formatted RPC call to the server requesting a
 * Tip.
 * @param sock - socket ID
 * @return - error status
 */
int tipRPC(int & sock)
{
    const char *tip = "rpc=tip;";
    incrementRPC(sock, tip);
    return 0;
}

/**
 * This function sends a formatted RPC call to the server requesting
 * Advice.
 * @param sock - socket ID
 * @return - error status
 */
int adviceRPC(int & sock)
{
    const char *advice = "rpc=advice;";
    incrementRPC(sock, advice);
    return 0;
}

/**
 * This function sends a formatted RPC call to the server requesting a
 * Quote.
 * @param sock - socket ID
 * @return - error status
 */
int quoteRPC(int & sock)
{
    const char *quote = "rpc=quote;";
    incrementRPC(sock, quote);
    return 0;
}

/**
 * This is the driver of the client thread. Note: for the purposes of
 * the demonstration, the typical commandline interactive client RPC
 * calls have been replaced with an automated testing process.
 * Interactive code has been commnented out for reference.
 * The loop was removed for demonstration clarity.
 *
 * argv - IP Address of Server (127.0.0.1), Port Number (12008)
 * return - error status
 */
int main(int argc, char const *argv[])
{
    // send port number from server in command line (i.e. ./client 127.0.0.1 12008)
    int sock = 0;
    int status;
    //char buff[1024];

    //Status returned from command line arguments passed to conenctToServer function.
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

    // Automated Test of RPC functionality
    printf("Testing Connect RPC with incorrect credentials.\n");
    const char *badCreds = "rpc=connect;user=falseadmin;password=falsepass;";
    incrementRPC(sock, badCreds);
    sleep(3);
    printf("\nTesting Connect RPC correct credentials.\n");
    const char *goodCreds = "rpc=connect;user=admin;password=pass;";
    incrementRPC(sock, goodCreds);
    sleep (3);
    printf("\nTesting Tip RPC\n");
    tipRPC(sock);
    sleep(3);
    printf("\nTesting Advice RPC\n");
    adviceRPC(sock);
    sleep (3);
    printf("\nTesting Quote RPC\n");
    quoteRPC(sock);
    sleep (1);
    printf("\nTesting Disconnect RPC\n");
    disconnectRPC(sock);
    status = disconnectServer(sock);

    return status;
}