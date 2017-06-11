#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

class MessageReader {
  const int &sock;
  std::string &message;
  const char terminator;
  int bufferStart;
  int bufferEnd;
  char buffer[256];
public:
  MessageReader(const int &psock, string& pmessage, const char pterminator):
    sock(psock), message(pmessage), terminator(pterminator), bufferStart(0), bufferEnd(0) {
  }

  bool checkMessage() {
    for(;bufferStart<bufferEnd;bufferStart++) {
      if(buffer[bufferStart] == terminator && message.length()>0) {	
	bufferStart++;
	return true;
      }
      if(buffer[bufferStart] == terminator) continue;
      message += buffer[bufferStart];
    }
    return false;
  }

  bool readMessage() {
    while(true) {
      //terminator found, so exit
      if(checkMessage()) {
	return true;
      }
      //waits for message from client
      bufferStart = 0;
      bufferEnd = read(sock, buffer, sizeof(buffer));
      if(bufferEnd <= 0) {
	return false;
      }
    }
  }
  
};


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

  //wait and accept client connection
  clientLength = sizeof(clientAddress);
  newSock = accept(sock, (sockaddr*) &clientAddress, &clientLength);   
  if(newSock < 0)
    cerr << "error on accept" << endl;
  
  string message = "";
  MessageReader messageReader(newSock, message, ';');
  
  while(messageReader.readMessage()) {
    do {
      cout << "> " << message << endl;
      message.clear();
    }while(messageReader.checkMessage());
  }
  
  close(sock);  
  close(newSock);
  return 0;
}
