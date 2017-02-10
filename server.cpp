#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

static int MAX_QUEUE = 6;
static int PORT = 3132;

using namespace std;

void process (int conn) ;

int main( int argc, char *argv[] ) {

//g++ -std=c++1y server.cpp -lpthread -o server
	int sock, conn, pid;
	socklen_t client_len;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
    client_len = sizeof(cli_addr);

    if((PORT > 65535) || (PORT < 2000)){
        cerr << "Please enter a port number between 2000 - 65535" << endl;
        exit(1);
    }

	//Create socket
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1){
		cerr << "Could not create socket" << endl;
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
   	serv_addr.sin_addr.s_addr = INADDR_ANY;
   	serv_addr.sin_port = htons(PORT);
   
   	//Now bind the host address using bind() call.
	while (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		cerr << "ERROR on binding, trying again in 3 seconds" << endl;
        sleep(3);
	}

    bzero((char*) &serv_addr, sizeof(serv_addr));

    //Now start listening for the clients, here process will go in sleep mode and will wait for the incoming connection
    listen(sock, MAX_QUEUE);
    cout << "Listening" << endl;

    //this is where client connects. svr will hang in this mode until client conn
    while ((conn = accept(sock, (struct sockaddr *)&cli_addr, &client_len)) != -1){

        if (conn < 0){
            cerr << "Cannot accept connection" << endl;
            return 0;
        }else{
            cout << "Connection successful "  << endl;
        }

        if ((pid = fork()) < 0) {
            perror("ERROR on fork");
            exit(1);
        }
        
        if (pid == 0) {
            // This is the client process 
            close(sock);
            process(conn);
            exit(0);
        }else {
            close(conn);
        }
    }
    
    return 0;
}


void process (int conn) {

   int n;
   char buffer[256];
   bzero(buffer,256);
   while((n = read(conn, buffer,255))){
   
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		
		printf("Here is the message: %s\n",buffer);

		n = write(conn, "I got your message\n",19);
		bzero(buffer,256);
			
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
   }
	
}