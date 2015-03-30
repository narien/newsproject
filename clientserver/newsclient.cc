#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>


using namespace std;

int readNumber(const Connection& conn) {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

int getNumP(const Connection& conn){
    unsigned char numPar = conn->read();
    int num = -1;
    if (numPar == Protocol::PAR_NUM) {
        num = readNumber(conn);
    } else {
      // server->deregisterConnection(conn);
    }
    return num;
}

string getStringP(const Connection& conn) {
  unsigned char stringPar = conn->read();
  string s;
  if (stringPar == Protocol::PAR_STRING) {
    int nbrOfBytes = readNumber(conn);
    for (int i = 0; i < nbrOfBytes; ++i) {
      s += conn->read();
    }
  } else {
    //server->deregisterConnection(conn);
  }
  return s;
}
void help(){
    cout << "Options:" << endl;
    cout << "listng: List all newsgroups" << endl;
    cout << "crtng <groupName>: Create a newsgroup with name <groupName>." << endl;
    cout << "delng <groupID>: Delete newsgroup <groupID>." << endl;
    cout << "listart <groupID>: List articles in newsgroup <groupID>." << endl;
    cout << "crtart <groupID>: Creates an article in newsgroup <groupID> and follow the instructions." <<endl;
    cout << "delart <groupID> <artID>: Deletes article <artID> in newsgroup <groupID>." << endl;
    cout << "getart <groupID> <artID>: Displays article <artID> in newsgroup <groupID>" << endl;
}

void rootMenu(const Connection& conn) {
  conn.write(Protocol::COM_LIST_NG);
  conn.write(Protocol::COM_END);
  unsigned char ansList = conn.read();
  int nbrOfGroups = getNumP(conn);
  cout "Newsgroups:" << endl;
  cout << "ID\tTitle" << endl;
  for (int i = 0; i < nbrOfGroups; ++i) {
    int id = getNumP(conn);
    string title = getStringP(conn);
    cout << id << "\t" << title << endl;
  }
  unsignes char endByte = conn.read();
  help();
  int choice;
  cin >> choice;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: newsclient host-name port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}
	
	while (true) {
		try {
		  rootMenu(conn);
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

