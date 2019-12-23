#include "networking.h"
#include "err.h"

/*=========================
  server_setup
  args:

  creates, binds a server side socket
  and sets it to the listening state

  returns the socket descriptor
  =========================*/
int server_setup(char *port) {
  int sd, i;

  //create the socket
  sd = socket( AF_INET, SOCK_STREAM, 0 );
  exit_err( "server socket", sd );
  printf("[server] socket created\n");

  //setup structs for getaddrinfo
  struct addrinfo * hints, * results;
  hints = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;  //IPv4 address
  hints->ai_socktype = SOCK_STREAM;  //TCP socket
  hints->ai_flags = AI_PASSIVE;  //Use all valid addresses
  getaddrinfo(NULL, port, hints, &results); //NULL means use local address

  //bind the socket to address and port
  i = bind( sd, results->ai_addr, results->ai_addrlen );
  exit_err( "server bind",i );
  printf("[server] socket bound\n");

  //set socket to listen state
  i = listen(sd, 10);
  exit_err( "server listen",i );
  printf("[server] socket in listen state\n");

  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);
  return sd;
}


/*=========================
  server_connect
  args: int sd

  sd should refer to a socket in the listening state
  run the accept call

  returns the socket descriptor for the new socket connected
  to the client.
  =========================*/
int server_connect(int sd) {
  int client_socket;
  socklen_t sock_size;
  struct sockaddr_storage client_address;

  sock_size = sizeof(client_address);
  client_socket = accept(sd, (struct sockaddr *)&client_address, &sock_size);
  exit_err("server accept",client_socket);

  return client_socket;
}

/*=========================
  client_setup
  args: int * to_server

  to_server is a string representing the server address

  create and connect a socket to a server socket that is
  in the listening state

  returns the file descriptor for the socket
  =========================*/
int client_setup(char * server,char *port) {
  int sd, i;

  //create the socket
  sd = socket( AF_INET, SOCK_STREAM, 0 );
  exit_err( "client socket", sd );

  //run getaddrinfo
  /* hints->ai_flags not needed because the client
     specifies the desired address. */
  struct addrinfo * hints, * results;
  hints = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;  //IPv4
  hints->ai_socktype = SOCK_STREAM;  //TCP socket
  getaddrinfo(server, port, hints, &results);

  //connect to the server
  //connect will bind the socket for us
  i = connect( sd, results->ai_addr, results->ai_addrlen );
  exit_err( "client connect", i );

  free(hints);
  freeaddrinfo(results);

  return sd;
}
