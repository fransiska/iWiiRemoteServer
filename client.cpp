#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //read write close
#include <netdb.h> //hostent

using namespace std;

int main(int argc, char** argv) {
  int sock, portno;
  string serverName;  
  char buffer[256];
  sockaddr_in serverAddress;
  string message;
  
  if(argc <3) {
    portno = 12345;
    serverName = "127.0.0.1";
  }
  else {
    stringstream ss;
    ss << argv[1];
    ss >> serverName;
    portno = atoi(argv[2]);
  }
  cout << "server " << serverName << ":" << portno << endl;

  //create a listening socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) {
    cerr << "error opening socket" << endl;
    return 1;
  }

  //setup the sockaddr structure
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(portno);
  serverAddress.sin_addr.s_addr = inet_addr(serverName.c_str());

  if(connect(sock, (sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
    cerr << "error on connecting" << endl;
    return 1;
  }

  while(true) {
    cout << "Please type the message: ";
    getline(cin, message);

    if(message.length() == 0) {
      cout << "quitting" << endl;
      break;
    }      

    int n = write(sock, message.c_str(), message.length());
    if(n < 0) {
      cerr << "error writing to socket" << endl;
      return 1;
    }

    n = read(sock, buffer, 255);
    if(n < 0) {
      cerr << "error reading from socket" << endl;
      return 1;
    }
    cout << buffer << endl;    
  }
  
  close(sock);
  return 0;
}
