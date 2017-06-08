#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {
  int sock, newSock, portno;
  char buffer[256];
  socklen_t clientLength;
  sockaddr_in serverAddress, clientAddress;

  if(argc <2) {
    portno = 12345;
  }
  else {
    stringstream ss;
    ss << argv[1];
    ss >> portno;
  }
  cout << "server " << portno << endl;

  //create a listening socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) {
    cerr << "error opening socket" << endl;
  }

  //setup the sockaddr structure
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(portno);

  //bind the listening socket
  if(bind(sock, (sockaddr*) &serverAddress, sizeof(serverAddress)) < 0)
    cerr << "error on binding" << endl;

  listen(sock, 5);
  clientLength = sizeof(clientAddress);
  newSock = accept(sock, (sockaddr*) &clientAddress, &clientLength);

  if(newSock < 0)
    cerr << "error on accept" << endl;

  int n = read(newSock, buffer, 255);
  if(n < 0)
    cerr << "error reading from socket" << endl;
  cout << "message is " << buffer << endl;

  n = write(newSock, "I got your message", 18);
  if(n < 0)
    cerr << "error writing to socket" << endl;

  close(newSock);
  close(sock);
  return 0;
}
