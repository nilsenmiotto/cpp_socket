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

using namespace std;
void doprocessing (int sock) ;

int main( int argc, char *argv[] ) {

//g++ -std=c++1y server.cpp -lpthread -o server
	int sock, conn, port, pid;
	socklen_t client_len;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
    int max_thread_size;

    sock = -1;
    port = 3132;
    max_thread_size = 6;
    client_len = sizeof(cli_addr);

    if((port > 65535) || (port < 2000)){
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
   	serv_addr.sin_port = htons(port);
   
   	//Now bind the host address using bind() call.
	while (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		cerr << "ERROR on binding" << endl;
        sleep(100);
		//exit(1);
	}

    bzero((char*) &serv_addr, sizeof(serv_addr));

    //Now start listening for the clients, here process will go in sleep mode and will wait for the incoming connection
    listen(sock, max_thread_size);
    //this is where client connects. svr will hang in this mode until client conn
    while ((conn = accept(sock, (struct sockaddr *)&cli_addr, &client_len)) != -1){

        cout << "Listening" << endl;
        if (conn < 0){
            cerr << "Cannot accept connection" << endl;
            return 0;
        }else{
            cout << "Connection successful " << conn << endl;
        }

        if ((pid = fork()) < 0) {
            perror("ERROR on fork");
            exit(1);
        }
        
        if (pid == 0) {
            // This is the client process 
            close(sock);
            doprocessing(conn);
            exit(0);
        }else {
            close(conn);
        }
    }
    












  //tcp_client socket;
    //connect to host
   // socket.conn(PBX_HOST, PBX_PORT);
    //send some data

   // socket.send_data("Action: Login\nUsername: " + PBX_LOGIN + "\nSecret: " + PBX_PASSWD + "\n\n");

    return 0;
}

void *task1 (void *void_conn)
{
	int conn = (int &) void_conn;
    cout << "Thread No: " << pthread_self() << endl;
    char test[300];
    bzero(test, 301);
    bool loop = false;
    while(read(conn, test, 300) > 0)
    {    
    

     
        string tester (test);
        cout << tester << endl;
		      if(tester == "exit")
            break;
		    bzero(test, 301);
        cout<< "aqui" << endl;

        
        
  
    }
    cout << "\nClosing thread and conn" << endl;
    //close(conn);
}

void doprocessing (int sock) {
   int n;
   char buffer[256];
   bzero(buffer,256);
   while((n = read(sock,buffer,255))){
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   
   printf("Here is the message: %s\n",buffer);
   n = write(sock,"I got your message\n",19);
      bzero(buffer,256);
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
   }
	
}