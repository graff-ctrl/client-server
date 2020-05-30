#include <cstdio>


// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
//#include <bits/stdc++.h>
#include <assert.h>
#include <pthread.h>

using namespace std;
//Chang port to your unique port
#define PORT 12008

// Class to set up server

class ServerSetup
{
    int connectionAmount, rpcAmount, nSocket; // Number of Connections, RPC Counter, Socket

    pthread_mutex_t lock;
    pthread_cond_t fill;
    int nMax;

public:
    ServerSetup()
    {
        connectionAmount = 0;
        rpcAmount = 0;
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&fill, NULL);
        nSocket = 0;
        nMax = 0;
    }
    // Function returns the amount of connections
    int getConnectionAmount()
    {
        int amount;
        pthread_mutex_lock(&lock);
        amount = connectionAmount;
        pthread_mutex_unlock(&lock);
        return amount;
    }
    // Function increments the number of total connections.
    void incConnectionAmount()
    {
        pthread_mutex_lock(&lock);
        connectionAmount++;
        pthread_mutex_unlock(&lock);
    }
    //Function returns the total number of RPC calls.
    int getRpcTotals()
    {
        int total;
        pthread_mutex_lock(&lock);
        total = rpcAmount;
        pthread_mutex_unlock(&lock);
        return total;
    }
    // Function increments the total number of RPC calls.
    void incRpcCalls()
    {
        pthread_mutex_lock(&lock);
        rpcAmount++;
        pthread_mutex_unlock(&lock);
    }
    // Function sets socket.
    void setSocket(int nSocket)
    {
        this->nSocket = nSocket;
    }
    //Function returns the socket.
    int getSocket()
    {
        return nSocket;
    }

};
class ConnectionContextData
{
    // You will put in your own "Global Data" that you will share among all the various client connections you have. Change your "getters" to reflect that

public:
    ConnectionContextData()
    {
        rpcAmount = 0;
    }

    void addRpcAmount()
    {
        rpcAmount++;
    }

    void addSumAmount(int incAmt)
    {
        sumAmount += incAmt;
    }

    int getRpcAmount()
    {
        return rpcAmount;
    }

    int getSumAmount()
    {
        return sumAmount;
    }
private:
    int rpcAmount = 0;
    int sumAmount = 0;

};

class Server
{
private:
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int port;

public:
    // Constructor
    Server(int port)
    {
        this->port = port;
    }
    //Deconstructor
    ~Server(){};
    // Funtion to start the server up;
    int serverStartup()
    {
        int opt = 1;
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("Socket Failed");
            exit(EXIT_FAILURE);
        }

        // Forcefully attaching socket to the port 8080
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &opt, sizeof(opt)))
        {
            perror("SetSockOpt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;

        address.sin_port = (uint16_t) htons((uint16_t) port);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *)&address,
                 sizeof(address))<0)
        {
            perror("Bind Failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0)
        {
            perror("Listen");
            exit(EXIT_FAILURE);
        }

        return 0;

    }

    int newConnection()
    {
        int new_socket;

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t*)&addrlen)) < 0)
        {
            perror("Accept");
            return (-1);
        }
        return new_socket;
    }

    int chatter(int new_socket)
    {
        int valread;
        char buffer[1024] = {0};
        const char *welcome = "Welcome to the Server";
        valread = (int) read(new_socket, (void*) buffer, (size_t) 1024);
        printf("%s Bytes in message = %d\n", buffer, valread);
        // What RPC is it
        // Parse out arguments
        // Call the correct RPC Function
        send(new_socket, welcome, strlen(welcome), 0);
        printf("Hello message sent.");
        return 0;

    }

    int closeServer()
    {
        return 0;
    }

};

void *rpcThread(void *arg)
{
    int nSocket = *(int*)socket;
    int valread;
    char buffer[1024] = {0};
    void *status = NULL;

    ServerSetup *pntrServerStartup = (ServerSetup *) arg;
    nSocket = pntrServerStartup->getSocket();
    ConnectionContextData *connectionObj = new ConnectionContextData();
    for (;;){
        const char *statusOK = "STATUS=OK";
        valread = (int)read(nSocket, (void*) buffer, (size_t)1024);
        if (strcmp(buffer, "QUIT") == 0)
        {
            connectionObj->addRpcAmount();
            pntrServerStartup->incRpcCalls();
            printf("Client with socket %d has disconnected.\nTotal RPC count: %d\n", nSocket
                    ,pntrServerStartup->getRpcTotals());
            pthread_exit(status);
        }
        if (strcmp(buffer, "QUOTE") == 0){
            printf("Quote for socket %d: Make the most of today.", nSocket);
        }
        int incAmt = atoi((char const*) buffer);
        connectionObj->addSumAmount(incAmt);
        connectionObj->addRpcAmount();
        pntrServerStartup->incRpcCalls();
        printf("%s Bytes read = %d  from socket %d\n", buffer, valread, nSocket);
        printf("sumAmt=%d  ThreadRPCAmt=%d\n", connectionObj->getSumAmount(), connectionObj->getRpcAmount());
        send(nSocket, statusOK, strlen(statusOK), 0);

        printf("Response message sent.\n");

    }
    fflush(stdout);

    return NULL;

}
int main(int argc, char const *arg[])
{
    pthread_t pthread;
    int status;
    //int argPort = atoi((char const *) argv[1]);
    Server *server = new Server(PORT);
    ServerSetup *serverDataObj = new ServerSetup();
    int rpcAmount = serverDataObj->getRpcTotals();
    printf("Total Server PRC Count: %d\n", rpcAmount);
    server->serverStartup();

    do {
        int newSocket = server->newConnection();
        if (newSocket <= 0)
        {
            printf("Error");
            status = -1;
        }
        server->chatter(newSocket);
        serverDataObj->setSocket(newSocket);
        pthread_create(&pthread, NULL, rpcThread, (void *) serverDataObj);
        printf("Server started thread.");
        } while (status == 0);
}
