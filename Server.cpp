#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Car.h"
using namespace std;
using namespace glm;
#define SERVER_PORT 50053
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

//sprawdzenie wiadomosci od klienta
void checkMessage(char *msg)
{
    string text(msg);
    int position = text.find("turnLeft");
    if (position==string::npos) 
    {
        turnLeft = false;
    }
    else
    {
        turnLeft = true;
    }

     position = text.find("turnRight");
    if (position==string::npos) 
    {
        turnRight = false;
    }
    else
    {
        turnRight = true;
    }

     position = text.find("goPlayer");
    if (position==string::npos) 
    {
        goPlayer = false;
    }
    else
    {
        goPlayer = true;
    }

     position = text.find("backPlayer");
    if (position==string::npos) 
    {
        backPlayer = false;
    }
    else
    {
        backPlayer = true;
    }
    cout<<"turnLeft: "<<turnLeft<<endl;
    cout<<"turnRight: "<<turnRight<<endl;
    cout<<"goPlayer: "<<goPlayer<<endl;
    cout<<"backPlayer: "<<backPlayer<<endl;
}

//skrecanie
//wymaga isMoving, turnLeft, turnRight
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

//skrecanie kol
// //wymaga turnWheel, getWheelRotation
// void turningWheels(Car &player)
// {
//     if (turnLeft)
//         {
//             player.turnWheelLeft();
//         }
//         if (turnRight)
//         {
//             player.turnWheelRight();
//         }
//         if (!turnLeft && !turnRight)  //prostuj ko³a
//         {
//             if (player.getWheelRotation() > 0)
//             {
//                 player.turnWheelRight();
//             }
//             if (player.getWheelRotation() < 0)
//             {
//                 player.turnWheelLeft();
//             }
//         }
// }

//poruszanie przod, tyl
//wymaga move
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

//poruszanie graczem
void moving(Car &player)
{
    turning(player);
    //turningWheels(player);
    going(player);
}

//logika gry, zwraca komunikat do wyslania
//wymaga checkPointReached, getPosition, getWheelRotation, getRotation
char * logic(Car &player) 
{
    string msg;
    moving(player);
    bool checkpointReached = player.checkpointReached();
    vec3 position = player.getPosition();
    float wheelRotation = player.getWheelRotation();
    float rotation = player.getRotation(); 
    msg = to_string(checkpointReached)+" ";                 //checkPointedReached
    //msg+= to_string(wheelRotation)+" ";                     //wheelRotation
    msg+= to_string(rotation)+" ";                          //rotation
    msg+= to_string(position.x)+" ";                        //pos.x
    msg+= to_string(position.y)+" ";                        //pos.y
    msg+= to_string(position.z) +" ";                       //pos.z
    char * returningValue = &msg[0];
    cout<<"Wychodze z logic, zwracam: "<<returningValue<<endl;
    return returningValue;
}

//jak nazwa wskazuje
char * getMessage(int fd)
{
    char *msg = new char[AMOUNT_OF_CHAR_IN_MSG]; 
    char oneChar;
    int i = 0;
    int size = 0;
    size = read(fd, &oneChar, 1);
    if (size == 0)  //jesli klient sie rozlaczyl
    {
        strcpy(msg, "the end");
        return msg;
    }
    
    while (oneChar != '\n') {
        msg[i++] = oneChar;
        size = read(fd, &oneChar, 1);
        if (size == 0)  //jesli klient sie rozlaczyl
        {
            strcpy(msg, "the end");
            return msg;
        }
    }
    return msg;
    
}

//funkcja opisującą zachowanie wątku - musi przyjmować argument typu (void *) i zwracać (void *)
void *ThreadBehavior(void *t_data)
{
    cout<<"Wszedlem do watku!"<<endl;
    pthread_detach(pthread_self());
    struct thread_data_t *th_data = (struct thread_data_t*)t_data;
    int clients_sockets[AMOUNT_OF_PLAYERS]; //tablica na sockety klientow

    for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)  
    {
        clients_sockets[i]= th_data->clients_sockets[i]; //wczytanie danych z argumentow do ladniejszej postaci 
    }

    char *msg[AMOUNT_OF_PLAYERS];   //tablica wiadomosci do klientow
    
    for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)
    {
        msg[i] = new char[AMOUNT_OF_CHAR_IN_MSG];   //alokacja pamieci
    }

    //Car car[AMOUNT_OF_PLAYERS]; //tablica samochodow graczy
    Car car[AMOUNT_OF_PLAYERS];
    cout<<"Autka zostaly utworzone"<<endl;
    char *message = new char[AMOUNT_OF_CHAR_IN_MSG];
    char slashn = '\n';
    //------------------------------------------------------------------------------
    while(1)
    {
        for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)  //pobierz wszystkie wiadomosci od graczy
        {
            cout<<"Zaraz bede odbierac!"<<endl;
            msg[i] = getMessage(clients_sockets[i]);
            cout<<"Odebralem: "<<msg[i]<<endl;
        }
        

        for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)  //dla kazdej wiadomosci
        {
            checkMessage(msg[i]);   //uzupelnij zmienne booloweskie

            //logika gry
                //---------------------------------
                string msg2;
                moving(car[i]);
                bool checkpointReached = car[i].checkpointReached();
                vec3 position = car[i].getPosition();
                float wheelRotation = car[i].getWheelRotation();
                float rotation = car[i].getRotation(); 
                if (checkpointReached)
                {
                    msg2 = "1 ";
                } else
                {
                    msg2 = "0 ";
                }
                
                //msg2 = to_string(checkpointReached)+" ";
                //msg2+= "wheelRotation="+to_string(wheelRotation)+";";
                msg2 += to_string(rotation)+" ";
                msg2 += to_string(position.x) + " ";
                msg2 += to_string(position.y) + " ";
                msg2 += to_string(position.z);
                message = &msg2[0];
                //---------------------------------
                strncat(message, &slashn, 1);

            for(int j = 0; j< AMOUNT_OF_PLAYERS; j++)   //wyslij j-temu graczowi info o graczu i-tym
            {
                write(clients_sockets[j], message, strlen(message));    //wyslij dane
                cout<<"Wyslalem: "<<message<<endl;   //napisz  co wyslales
                
            }
        }
    }
    
    pthread_exit(NULL);
}

//sprawdzenie czy udalo sie utworzyc watek i reakcja na to
void check_thread(int &create_result)
{
    if (create_result){
       printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
       exit(-1);
    }
}

//funkcja obsługująca połączenie z nowym klientem
void handleConnection(int clients_sockets[]) {
    cout<<"Wszedlem do handleConnection"<<endl;
    //wynik funkcji tworzącej wątek
    int create_result = 0;

    //uchwyt na wątek
    pthread_t thread;

    //struktura ktorej dane zostaną przekazane do wątku (sockety klientow)
    struct thread_data_t *t_data;
    
    //wypelnienie powyzszych danych
    for(int i = 0; i< AMOUNT_OF_PLAYERS; i++)
    {
        t_data->clients_sockets[i] = clients_sockets[i];
    }
    create_result = pthread_create(&thread, NULL, ThreadBehavior, (void *)t_data);
    
    check_thread(create_result);    //sprawdzenie czy udalo sie utworzyc watek
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
    
    int clients_sockets[AMOUNT_OF_PLAYERS]; //tablica klientow w danej grze
    
    initializeServer(server_socket_descriptor, bind_result, listen_result, reuse_addr_val, server_address);  //jak nazwa wskazuje

    while(1) //Petla glowna programu
    {
        makeMinusOne(clients_sockets);  //ustaw tablicę klientów na -1
        
        for(int i = 0; i < AMOUNT_OF_PLAYERS; i++) //dla kazdego gracza ktory bedzie w jednym wyscigu
        {
            connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL); //Nowy klient przyszedl
            cout<<"Zaakceptowano nowego klienta"<<endl;
            check_client(connection_socket_descriptor);  //sprawdz utworzenie gniazda, jesli cos poszlo nie tak wylacz program, malo prawdp sytuacja

            clients_sockets[i] = connection_socket_descriptor;  //dodaj gniazdo do tablicy

            char *message = new char[AMOUNT_OF_CHAR_IN_MSG];
            sprintf(message, "%i", i);
            //strcpy(message, "client");
            char slashn = '\n';
            strncat(message, &slashn, 1);
            cout<<"Wysyłam: "<<message<<endl;
            write(clients_sockets[i], message, strlen(message)); //wyslij graczowi ktory jest

            cout<<"Klient "<<clients_sockets[i]<<" dolaczyl do wspaniej gry RacingGame! Witamy na pokladzie ;D"<<endl;
        
        }
        
        handleConnection(clients_sockets); //wyslij tablicę
       
    }    

    close(server_socket_descriptor);    //no widac co robi, co nie? :D 
    return(0);
}


