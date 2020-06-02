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
#include <fstream> // allow access to files



using namespace std;
//Chang port to your unique port
#define PORT 12008


class KeyValue
{
private:
    char m_szKey[128];
    char m_szValue[2048];

public:

    KeyValue() {};
    void setKeyValue(char *pszBuff)
    {
        char *pch1;

        // find out where the "=" sign is, and take everything to the left of the equal for the key
        // go one beyond the = sign, and take everything else
        pch1 = strchr(pszBuff, '=');
        assert(pch1);
        int keyLen = (int)(pch1 - pszBuff);
        strncpy(m_szKey, pszBuff, keyLen);
        m_szKey[keyLen] = 0;
        strcpy(m_szValue, pszBuff + keyLen + 1);
    }

    char *getKey()
    {
        return m_szKey;
    }

    char *getValue()
    {

        return m_szValue;
    }

};


// This class will take a string that is passed to it in this format:

// input to constructor:
// <variable1>=<value1>;<variable2>=<value2>;
//You will then call the method  getNextKeyValue until getNextKeyValue returns NULL.
// getNextKeyValue will return a KeyValue object. Inside of that KeyValue object will contain the variable and the value
// You will then call getKey or getValue to get the contents of those fields.
// The example in main() will show how to call this function.
// By extracting the contents you then can determine the rpc you need to switch to, along with variables you will need
// You can also use this class in your client program, since you will need to determine the contents that you receive from server

class RawKeyValueString
{
private:

    char m_szRawString[32768];
    int m_currentPosition;
    KeyValue *m_pKeyValue;
    char *m_pch;
public:

    RawKeyValueString(char *szUnformattedString)
    {
        assert(strlen(szUnformattedString));
        strcpy(m_szRawString, szUnformattedString);

        m_pch = m_szRawString;

    }
    ~RawKeyValueString()
    {
        if (m_pKeyValue)
            delete (m_pKeyValue);
    }

    void getNextKeyValue(KeyValue & keyVal)
    {
        // It will attempt to parse out part of the string all the way up to the ";", it will then create a new KeyValue object  with that partial string
        // If it can;t it will return null;
        char *pch1;
        char szTemp[32768];

        pch1 = strchr(m_pch, ';');
        assert(pch1 != NULL);
        int subStringSize = (int)(pch1 - m_pch);
        strncpy(szTemp, m_pch, subStringSize);
        szTemp[subStringSize] = 0;
        m_pch = pch1 + 1;
        if (m_pKeyValue)
            delete (m_pKeyValue);
        keyVal.setKeyValue(szTemp);

    }

};

int connectRPC(char *username, char *password)
{
    if (strcmp(username, "admin") == 0 && strcmp(password, "pass") == 0)
    {
        return 0;
    }
    return -1;
}

void disconnectRPC(char *buffer)
{
    const char *disconnect = "You are disconnected.";
    strcpy(buffer, disconnect);
}

/*void Quote(char *buffer)
{
    //Create temp char array
    //Open file
    //Read file until % into char array
    //Copy temp to buffer
    //Close file.
  const int SIZE = 1024;
  //char temp[SIZE][SIZE]; //2D Array
  char temp[1024];
  int colIndex = 0;
  int rowIndex = 0;
  int randomNumber = rand() % 10 + 1;

  fstream file; // reference file
  char input[SIZE] = ""; // read from file
  file.open("startrek.txt"); // direct to file address

  while(file>>input){
      if(input == "%"){
        colIndex++;
        rowIndex = 0;
      }
    else{

        for(int i = 0; i < SIZE; i++){
            temp[rowIndex][colIndex] = input[i];
            rowIndex++;
        }
    }
    colIndex = randomNumber;

    // overwrite buffer
    strcpy(buffer, temp[colIndex]);;
    file.close();
  }
}*/

void Advice(char *buffer)
{
    const char *advice = "Advice: This is your advice.\n";
    strcpy(buffer, advice);
}
void Tip(char *buffer)
{
    // declare vars
    // char TipList[][1024];
    int listInd;

    // create random number generator as class function?
        // get random number w/in Tip list/database
        // need to know size of Tip list/database to bound int generation
        listInd = 1;

    // access Tip list/database
        // if list, read in .txt file at startup, save as class var
        // would that require putting RPC calls in separate class?
        // hardcoded for now
        char TipList[][1024] = {"Get 8 hours of sleep",
                                "Exercise for 30 minutes each day",
                                "Take a break to stretch after sitting for long periods"};

    // pull random tip from TipList
    const char *curTip = TipList[listInd];

    // append new line escape character
    char *newstr = (char*)malloc(strlen(curTip) + 2);
    strcpy(newstr, curTip);
    strcat(newstr, "\n");

    // overwrite buffer
    strcpy(buffer, newstr);
}

// Function to parse buffer from client for RPC call.
int parseBuffer(char *buff, char *response)
{
    // Create a couple of buffers, and see if works
    //const char *szTest1 = "rpc=connect;user=mike;password=123;";
    RawKeyValueString *pRawKey = new RawKeyValueString((char *)buff);
    KeyValue rpcKeyValue;
    char *pszRpcKey;
    char *pszRpcValue;

    // Figure out which rpc it is

    pRawKey->getNextKeyValue(rpcKeyValue);
    pszRpcKey = rpcKeyValue.getKey();
    pszRpcValue = rpcKeyValue.getValue();

    if (strcmp(pszRpcKey, "rpc") == 0)
    {
        if (strcmp(pszRpcValue, "connect") == 0)
        {
            // Get the next two arguments (user and password);
            KeyValue userKeyValue;
            KeyValue passKeyValue;

            [[maybe_unused]] char *pszUserKey;
            [[maybe_unused]] char *pszUserValue;
            [[maybe_unused]] char *pszPassKey;
            [[maybe_unused]] char *pszPassValue;
            //int status;
            [[maybe_unused]] char username[15];
            [[maybe_unused]] char password[15];


            pRawKey->getNextKeyValue(userKeyValue);
            pszUserKey = userKeyValue.getKey();
            pszUserValue = userKeyValue.getValue();

            pRawKey->getNextKeyValue(passKeyValue);
            pszPassKey = passKeyValue.getKey();
            pszPassValue = passKeyValue.getValue();

            //strcpy(username, pszUserValue);
            //strcpy(password, pszPassValue);

            if (connectRPC(pszUserValue,pszPassValue) == 0)
            {
                const char *success = "You are connected.";
                strcpy(response, success);
            }
            else{
                const char *fail = "User=Invalid";
                strcpy(response, fail);
            }
        }
        if (strcmp(pszRpcValue, "disconnect") == 0)
        {

            printf("Client wants to disconnect\n");
            disconnectRPC(response);
        }
        if (strcmp(pszRpcValue, "quote") == 0)
        {
            printf("Client wants a quote\n");
            //Send to Quote();
        }
        if (strcmp(pszRpcValue, "tip") == 0)
        {
            printf("Client wants a tip\n");
            Tip(response);
        }
        if (strcmp(pszRpcValue, "advice") == 0)
        {
            printf("Client wants advice\n");
            Advice(response);
        }
    }
    return 0;
}

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
    void *status = NULL;
    ServerSetup *pntrServerStartup = (ServerSetup *) arg;
    nSocket = pntrServerStartup->getSocket();
    ConnectionContextData *connectionObj = new ConnectionContextData();
    for (;;){
        char buffer[1024] = {0};
        char response[1024];
        valread = (int)read(nSocket, (void*) buffer, (size_t)1024);
        parseBuffer(buffer, response);
        if (strcmp(buffer, "rpc=disconnect;") == 0 || strcmp(response, "rpc=disconnect;") == 0)
        {
            connectionObj->addRpcAmount();
            pntrServerStartup->incRpcCalls();
            printf("Client with socket %d has disconnected.\nTotal RPC count: %d\n", nSocket
                    ,pntrServerStartup->getRpcTotals());
            send(nSocket, response, strlen(response), 0);
            pthread_exit(status);
        }
        int incAmt = atoi((char const*) buffer);
        connectionObj->addSumAmount(incAmt);
        connectionObj->addRpcAmount();
        pntrServerStartup->incRpcCalls();
        printf("%s Bytes read = %d  from socket %d\n", buffer, valread, nSocket);
        printf("sumAmt=%d  ThreadRPCAmt=%d\n", connectionObj->getSumAmount(), connectionObj->getRpcAmount());
        send(nSocket, response, strlen(response), 0);

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
    printf("Total Server RPC Count: %d\n", rpcAmount);
    server->serverStartup();

    do {
        int newSocket = server->newConnection();
        if (newSocket <= 0)
        {
            printf("Error");
            status = -1;
        }
        //server->chatter(newSocket);
        serverDataObj->setSocket(newSocket);
        pthread_create(&pthread, NULL, rpcThread, (void *) serverDataObj);
        printf("\nServer started thread.\n\n");
        } while (status == 0);
}
