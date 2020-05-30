

// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
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
//Funtion to connect to server.
int connectRPC(char *userName, char *password)
{
    // Input Arguments are:
    // username
    // password
    // input format="rpc=connect;username=<Your user>;password=<Your password>;"
    // Output arguments are:
    // status     (This will be set to 1 if success and -1 if error)
    // error      ( This will be to blank or an error message)
    // output format="status=<error status>;error=<error or blank>
    return 0;
}

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

int incrementRPC(int & sock, char *buff)
{
    size_t valRead=0;
    char hello[24];
    strcpy(hello,"Hello from client");

    char buffer[1024] = { 0 };
    send(sock, buff, strlen(buff), 0);
    printf("Hello message sent\n");
    if (strcmp(buff, "QUIT") == 0) {
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
int Connect(char *pszUserName, char *pszPass)
{
    printf("Username = %s  Password = %s", pszUserName, pszPass);
    return 0;
}
int processRPC(char *szTest1){
    // Create a couple of buffers, and see if works
    //const char *szTest1 = "rpc=connect;user=mike;password=123;";
    RawKeyValueString *pRawKey = new RawKeyValueString((char *)szTest1);
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

            char *pszUserKey;
            char *pszUserValue;
            char *pszPassKey;
            char *pszPassValue;
            int status;

            pRawKey->getNextKeyValue(userKeyValue);
            pszUserKey = userKeyValue.getKey();
            pszUserValue = userKeyValue.getValue();

            pRawKey->getNextKeyValue(passKeyValue);
            pszPassKey = passKeyValue.getKey();
            pszPassValue = passKeyValue.getValue();

            status = Connect(pszUserValue, pszPassValue);
        }
    }
}
int main(int argc, char const *argv[])
{
    // Testing for string parsing below
//    const char *szTest1 = "rpc=connect;user=mike;password=123;";
//    int status;
//    status = processRPC((char *)szTest1);

    int sock = 0;
    int status;
    char buff[128];
    // We will find out how many times to send our hello

    status = connectToServer((char *) argv[1], (char *) argv[2], sock);
    for (int i = 0; i < 20; i++) {
        strcpy(buff, "1");
        incrementRPC(sock, buff);
        sleep(5);
        strcpy(buff, "15");
        incrementRPC(sock, buff);
    }
    strcpy(buff, "QUIT");
    incrementRPC(sock, buff);

    status = disconnectServer(sock);

    return status;
}
