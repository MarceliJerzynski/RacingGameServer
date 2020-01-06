#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <cstring>
#include "glm/glm.hpp"
#include "Car.h"
using namespace std;
using namespace glm;
#define SERVER_PORT 1132
#define QUEUE_SIZE 5
const int AMOUNT_OF_CHAR_IN_MSG = 256;
const int AMOUNT_OF_PLAYERS = 2;
//struktura zawierająca dane, które zostaną przekazane do wątku
struct thread_data_t
{
int clients_sockets[AMOUNT_OF_PLAYERS];
};

bool turnLeft = false;
bool turnRight = false;
bool goPlayer = false;
bool backPlayer = false;


void checkMessage(char *msg)
{
    string text(msg);
    int position = text.find("turnLeft");
    if (position == strlen(msg)) 
    {
        turnLeft = false;
    }
    else
    {
        turnLeft = true;
    }

     position = text.find("turnRight");
    if (position == strlen(msg)) 
    {
        turnRight = false;
    }
    else
    {
        turnRight = true;
    }

     position = text.find("goPlayer");
    if (position == strlen(msg)) 
    {
        goPlayer = false;
    }
    else
    {
        goPlayer = true;
    }

     position = text.find("backPlayer");
    if (position == strlen(msg)) 
    {
        backPlayer = false;
    }
    else
    {
        backPlayer = true;
    }
}


void turning(Car &player)
{

    if (player.isMoving() == 1)    //jesli jedzie do przodu
    {
        if (turnLeft)   //i jednoczesnie A
        {
            player.turnLeft();    //skrec gracza
        } else
        if (turnRight)
        {
            player.turnRight();
        }
    }
    if (player.isMoving() == -1)
    {
            if (turnLeft)
            {
                player.turnRight();
            } else
            if (turnRight)
            {
                player.turnLeft();
            }
            
    }
}

void turningWheels(Car &player)
{
    if (turnLeft)
        {
            player.turnWheelLeft();
        }
        if (turnRight)
        {
            player.turnWheelRight();
        }
        if (!turnLeft && !turnRight)  //prostuj ko³a
        {
            if (player.getWheelRotation() > 0)
            {
                player.turnWheelRight();
            }
            if (player.getWheelRotation() < 0)
            {
                player.turnWheelLeft();
            }
        }
}

void going(Car &player)
{
    if (goPlayer)       //jesli trzyma W
    {
        player.move(1);  //rusz gracza
    } else
    if (backPlayer)     //jesli trzyma S
    {
        player.move(2);
    } else
    {
        player.move(0);
    }
}

void moving(Car &player)
{
    turning(player);
    turningWheels(player);
    going(player);
}

const char * logic(Car &player) 
{
    string msg;
    moving(player);
    bool checkpointReached = player.checkpointReached();
    vec3 position = player.getPosition();
    float wheelRotation = player.getWheelRotation();
    float rotation = player.getRotation(); 
    msg = "checkPointedReached"+to_string(checkpointReached)+";";
    msg+= "wheelRotation="+to_string(wheelRotation)+";";
    msg+= "rotation="+to_string(rotation)+";";
    msg+= "position="+to_string(position.x);
    msg+= ","+to_string(position.y);
    msg+= ","+to_string(position.z);
    const char * returningValue = new char[AMOUNT_OF_CHAR_IN_MSG];
    returningValue = msg.c_str();
    return returningValue;
}




//jak nazwa wskazuje
char * getMessage(int fd)
{
    char *msg = new char[AMOUNT_OF_CHAR_IN_MSG]; 
    char oneChar;
    int i = 0;
    read(fd, &oneChar, 1);
    msg[i++] = oneChar;
    while (oneChar != '\n') {
        read(fd, &oneChar, 1);
        msg[i] = oneChar;
        i++;
    }
    return msg;
    
}



//funkcja opisującą zachowanie wątku - musi przyjmować argument typu (void *) i zwracać (void *)
void *ThreadBehavior(void *t_data)
{
    pthread_detach(pthread_self());
    struct thread_data_t *th_data = (struct thread_data_t*)t_data;
    int clients_sockets[AMOUNT_OF_PLAYERS];
    for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)
    {
        clients_sockets[i]= th_data->clients_sockets[i]; 
    }
    char *msg[AMOUNT_OF_PLAYERS];
    
    for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)
    {
        
        msg[i] = new char[AMOUNT_OF_CHAR_IN_MSG];
    }

    Car car[2];
    //------------------------------------------------------------------------------
    while(1)
    {
        for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)  //pobierz wszystkie wiadomosci od graczy
        {
            msg[i] = getMessage(clients_sockets[i]);
            cout<<"Odczytalem wiadomosc: "<<msg[i]<<endl;
        }
        

        for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)  //dla kazdej wiadomosci
        {
            checkMessage(msg[i]);   //uzupelnij zmienne booloweskie

            for(int j = 0; j< AMOUNT_OF_PLAYERS; j++)   //dla kazdeego gracza
            {
                const char * message = logic(car[i]);   //wykonaj poruszanie sie gracza
                write(clients_sockets[j], message, strlen(message));
                cout<<"Wyslalem: "<<msg[j]<<endl;
            }
        }
    }
    
    pthread_exit(NULL);
}


//funkcja obsługująca połączenie z nowym klientem
void handleConnection(int clients_sockets[]) {
    //wynik funkcji tworzącej wątek
    int create_result = 0;

    //uchwyt na wątek
    pthread_t thread1;

    //dane, które zostaną przekazane do wątku
    struct thread_data_t *t_data;
    for(int i = 0; i< AMOUNT_OF_PLAYERS; i++)
    {
        t_data->clients_sockets[i] = clients_sockets[i];
    }
    create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void *)t_data);
    if (create_result){
       printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
       exit(-1);
    }
    //TODO (przy zadaniu 1) odbieranie -> wyświetlanie albo klawiatura -> wysyłanie
}


//inicjalizacja serwera
void initializeServer(int &server_socket_descriptor, int &bind_result, int &listen_result, char &reuse_addr_val, struct sockaddr_in &server_address)
{
   memset(&server_address, 0, sizeof(struct sockaddr));
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port = htons(SERVER_PORT);

   server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket_descriptor < 0)
   {
       fprintf(stderr, ": Błąd przy próbie utworzenia gniazda..\n");
       exit(1);
   }
   setsockopt(server_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse_addr_val, sizeof(reuse_addr_val));

   bind_result = bind(server_socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
   if (bind_result < 0)
   {
       fprintf(stderr, ": Błąd przy próbie dowiązania adresu IP i numeru portu do gniazda.\n");
       exit(1);
   }

   listen_result = listen(server_socket_descriptor, QUEUE_SIZE);
   if (listen_result < 0) {
       fprintf(stderr, ": Błąd przy próbie ustawienia wielkości kolejki.\n");
       exit(1);
   }
    
}

//sprawdzenie czy poprawnie zostalo utworzone gniazdo
void check_client(int connection_socket_descriptor) 
{
    if (connection_socket_descriptor < 0)
    {
        fprintf(stderr, ": Błąd przy próbie utworzenia gniazda dla połączenia.\n");
        exit(1);
    }
    
}

//ustawia clients_sockets na -1
void makeMinusOne(int clients_sockets[])
{
    
    for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)
    {
        clients_sockets[i] = -1;
    }
    
}

int main()
{
    //takie tam gowienka do dzialania serwera
    int server_socket_descriptor;
    int connection_socket_descriptor;
    int bind_result;
    int listen_result;
    char reuse_addr_val = 1;
    struct sockaddr_in server_address;
    
    int clients_sockets[AMOUNT_OF_PLAYERS];
    
    initializeServer(server_socket_descriptor, bind_result, listen_result, reuse_addr_val, server_address);  //jak nazwa wskazuje
   
    while(1) //Petla glowna programu
    {
        makeMinusOne(clients_sockets);
        
        for(int i = 0; i < AMOUNT_OF_PLAYERS; i++) //dla kazdego gracza ktory bedzie w jednym wyscigu
        {
            connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL); //Nowy klient przyszedl
            
            check_client(connection_socket_descriptor);  //sprawdz utworzenie gniazda, jesli cos poszlo nie tak wylacz program, malo prawdp sytuacja

            clients_sockets[i] = connection_socket_descriptor;  //dodaj gniazdo do tablicy
            cout<<"Klient "<<clients_sockets[i]<<" dolaczyl do wspaniej gry RacingGame! Witamy na pokladzie ;D :*"<<endl;
        }
        
        handleConnection(clients_sockets); //wyslij tablicę
       
    }    

    close(server_socket_descriptor);    //no widac co robi, co nie? :D 
    return(0);
}


