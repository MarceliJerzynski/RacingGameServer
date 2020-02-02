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


#include "Vec3.h"
#include "Car.h"
using namespace std;
#define SERVER_PORT 50065
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

class CarBehaviour;
class MessageClass;
class ThreadThings;
class ServerThings;
class Game;

//funkcja opisującą zachowanie wątku - musi przyjmować argument typu (void *) i zwracać (void *)
void *ThreadBehavior(void *t_data) {
    pthread_detach(pthread_self());
    struct thread_data_t *th_data = (struct thread_data_t*)t_data;

    int clients_sockets[AMOUNT_OF_PLAYERS];

    for(int i = 0; i < AMOUNT_OF_PLAYERS; i++) {
        clients_sockets[i] = th_data->clients_sockets[i];
    }

    Game game(clients_sockets);

    game.game();
    
    pthread_exit(NULL);
}

class CarBehaviour {    //nie tworzyc instancji tej klasy, same statyczne tu są 

    public:

        //poruszanie graczem
        static void moving(Car &player) {
            turning(player);
            going(player);
        }

    private:
        CarBehaviour(); //prywatny konstuktor, nie tworzyć instancji tej klasy

        //skrecanie
        static void turning(Car &player) {

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

        //poruszanie przod, tyl
        static void going(Car &player) {
            if (goPlayer)       //jesli trzyma W
            {
                player.move(1);     //rusz gracza
            } else
            if (backPlayer)     //jesli trzyma S
            {
                player.move(2);     //cofaj gracza    
            } else              //nie trzyma W ani S
            {
                player.move(0);     //Zwalniaj gracza
            }
        }
};

class MessageClass {    //nie tworzyc instancji tej klasy, same statyczne tu są 

    public:
        //na podstawie wiadomosci od klienta zmienia odpowiednie zmienne
        static void changeBooleanValues(string text) {
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

        }

        //odbiera wiadomosc, zwraca w formie char *
        static string getMessage(int fd) {
            string msg;
            char oneChar;
            int i = 0;
            int size = 0;
            size = read(fd, &oneChar, 1);
            if (size == 0)  //jesli klient sie rozlaczyl
            {
                msg = "the end";
            }
            
            while (oneChar != '\n') {
                msg[i++] = oneChar;
                size = read(fd, &oneChar, 1);
                if (size == 0)  //jesli klient sie rozlaczyl
                {
                    msg = "the end";
                }
            }
            return  msg;
            
        }

        //jesli cos pojdzie nie tak to zwraca -1, jak jest wszystko git to 1 
        static int sendMessage(int fd, string message) {

            char oneChar;
            int i = 0;
            int result;
            for(char character: message) {
                 result = write(fd, &character, 1);    //wyslij dane, bajt po bajcie
                 if (result == -1) {
                     return -1;
                 }
            }
             return write(fd, "\n", 1); //wyslij slahn n

        }

    private:
        MessageClass();

};

class ThreadThings {    //nie tworzyc instancji tej klasy, same statyczne tu są 

    public:
        //sprawdzenie czy udalo sie utworzyc watek i reakcja na to
        static void check_thread(int &create_result)
        {
            if (create_result){
                printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
                exit(-1);
            }
        }

    private:
        ThreadThings();

};

class ServerThings {    //nie tworzyc instancji tej klasy, same statyczne tu są 

    public:

        //inicjalizacja serwera
        static void initializeServer(int &server_socket_descriptor, int &bind_result, int &listen_result, 
                                     char &reuse_addr_val,   struct sockaddr_in &server_address)
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
        static void check_client(int connection_socket_descriptor) 
        {
            if (connection_socket_descriptor < 0)
            {
                fprintf(stderr, ": Błąd przy próbie utworzenia gniazda dla połączenia.\n");
                exit(1);
            }
        }

        //funkcja obsługująca połączenie z nowym klientem, odpala wątek i przesyła do niego clients_sockets
        static void handleConnection(int clients_sockets[]) {

                //wynik funkcji tworzącej wątek
                int create_result = 0;

                //uchwyt na wątek
                pthread_t thread;

                //struktura ktorej dane zostaną przekazane do wątku (sockety klientow)
                struct thread_data_t *t_data = new thread_data_t();
                
                //wypelnienie powyzszych danych
                for(int i = 0; i< AMOUNT_OF_PLAYERS; i++)
                {
                    t_data->clients_sockets[i] = clients_sockets[i];
                }

                create_result = pthread_create(&thread, NULL, ThreadBehavior, (void *)t_data);
                
                ThreadThings::check_thread(create_result);    //sprawdzenie czy udalo sie utworzyc watek

                delete t_data;
        }

        //ustawia clients_sockets na -1
        static void makeMinusOne(int clients_sockets[])
        {
            
            for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)
            {
                clients_sockets[i] = -1;
            }
            
        }

    private:

        ServerThings();

};


class Game {

    public:
        
        Game(int (&arg_clients_sockets)[AMOUNT_OF_PLAYERS] )
         : clients_sockets(arg_clients_sockets) {
            alocateMemory();
        }

        void game() {
            
            while(1) {
                
                getMessageFromAllPlayers();
                for (int i = 0; i < AMOUNT_OF_PLAYERS; i++) {

                    MessageClass::changeBooleanValues(msg[i]);   //uzupelnij zmienne booloweskie
                    
                    CarBehaviour::moving(car[i]);               //porusz furką

                    string message = createMessage(i);          //stworz stringa w ktorym zawarta jest nowa pozycja kazdej furki

                    for(int j = 0; j< AMOUNT_OF_PLAYERS; j++)   //wyslij j-temu graczowi info o graczu i-tym
                    {
                        MessageClass::sendMessage(clients_sockets[j], message);   
                    }
                }
            }
        }

        ~Game() {
            delete message;

        }

    private:
        
        int (&clients_sockets)[AMOUNT_OF_PLAYERS];           //tablica socketow
        string msg[AMOUNT_OF_PLAYERS];                      //tablica wiadomosci do klientow
        Car car[AMOUNT_OF_PLAYERS];
        char *message;

        void alocateMemory() {

            message = new char[AMOUNT_OF_CHAR_IN_MSG];

        }

        void getMessageFromAllPlayers() {

            for(int i = 0; i < AMOUNT_OF_PLAYERS; i++)
            {    
                msg[i] = MessageClass::getMessage(clients_sockets[i]);
            }
        }

        string createMessage(int i) {
            
                string msg2;
                //CarBehaviour::moving(car[i]);
                bool checkpointReached = car[i].checkpointReached();
                Vec3 position = car[i].getPosition();
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

                return message;
                // //---------------------------------
                // strncat(message, &slashn, 1);

        }

};



int main() {


    //takie tam gowienka do dzialania serwera
    int server_socket_descriptor;
    int connection_socket_descriptor;
    int bind_result;
    int listen_result;
    char reuse_addr_val = 1;
    struct sockaddr_in server_address;
    
    int clients_sockets[AMOUNT_OF_PLAYERS]; //tablica klientow w danej grze
    
    ServerThings::initializeServer(server_socket_descriptor, bind_result, listen_result, reuse_addr_val, server_address);  //jak nazwa wskazuje

    while(1) //Petla glowna programu
    {
        ServerThings::makeMinusOne(clients_sockets);  //ustaw tablicę klientów na -1
        
        for(int i = 0; i < AMOUNT_OF_PLAYERS; i++) //dla kazdego gracza ktory bedzie w jednym wyscigu
        {
            connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL); //Nowy klient przyszedl
            ServerThings::check_client(connection_socket_descriptor);  //sprawdz utworzenie gniazda, jesli cos poszlo nie tak wylacz program, malo prawdp sytuacja

            clients_sockets[i] = connection_socket_descriptor;  //dodaj gniazdo do tablicy

            char *message = new char[AMOUNT_OF_CHAR_IN_MSG];
            sprintf(message, "%i", i);

            cout<<"Wysyłam: "<<message<<endl;
            
            MessageClass::sendMessage(clients_sockets[i], message);

            cout<<"Klient "<<clients_sockets[i]<<" dolaczyl do wspaniej gry RacingGame! Witamy na pokladzie ;D"<<endl;

        }
        
        ServerThings::handleConnection(clients_sockets); //wyslij tablicę
       
    }    

    close(server_socket_descriptor);    //no widac co robi, co nie? :D 
    return(0);
}


