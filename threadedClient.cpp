#define WIN32_LEAN_AND_MEAN

#include <sys/types.h>
#include <winsock2.h>
// #include <netinet/in.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1132"

#define BUF_SIZE 1024
#define NUM_THREADS     5

const int AMOUNT_OF_CHAR_IN_MSG = 256;
const int AMOUNT_OF_PLAYERS = 2;


//struktura zawierająca dane, które zostaną przekazane do wątku
struct thread_data_t
{
int socket;
};

void sendMessage(int fd, char *msg)
{
    for (int i = 0; i < sizeof(msg)/sizeof(char); i++)
    {
        send(fd, &msg[i], sizeof(char), 0);
    }
}


//jak nazwa wskazuje
char * getMessage(int fd)
{
    char *msg = new char[AMOUNT_OF_CHAR_IN_MSG]; 
    char oneChar;
    int i = 0;
    recv(fd, &oneChar, 1, 0);
    msg[i++] = oneChar;
    while (oneChar != '\n') {
        recv(fd, &oneChar, 1 , 0);
        msg[i] = oneChar;
        i++;
    }
    return msg;
    
}


//funkcja obsługująca połączenie z serwerem
void handleConnection(int connection_socket_descriptor) {   //wysylanie
    //wynik funkcji tworzącej wątek
    //int create_result = 0;

    //uchwyt na wątek
    //pthread_t thread1;

    //dane, które zostaną przekazane do wątku
    //struct thread_data_t t_data;
    //t_data.socket = connection_socket_descriptor;
    //TODO
    //create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void *)&t_data);
    //if (create_result){
    //   printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
    //   exit(-1);
    //}
    char *msg = new char[AMOUNT_OF_CHAR_IN_MSG];
    char ch = '\n';
    while(1)
    {
      cin>>msg;
      strncat(msg, &ch, 1); 
      //sendMessage(connection_socket_descriptor, msg);
      send(connection_socket_descriptor, msg, strlen(msg), 0);

      char *msg = new char[AMOUNT_OF_CHAR_IN_MSG];
      for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)
      {
        strcpy(msg, getMessage(connection_socket_descriptor));
      
        cout<<msg<<endl;
      }

    }

    //TODO (przy zadaniu 1) odbieranie -> wyświetlanie albo klawiatura -> wysyłanie
}


int main (int argc, char *argv[])
{ 
   
   
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    const char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }    
   handleConnection(ConnectSocket);


   closesocket(ConnectSocket);


   return 0;
}
