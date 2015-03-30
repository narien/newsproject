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
    cout << "exit: Quit the client" << endl;
}

void run(const Connection& conn) {
    help();
    
    string choice;
    bool exit = false;
    while(!exit){
        cin >> choice;
        switch (choice) {
            case "listng":
                listNG(conn);
                break;
            case "crtng":
                string title;
                cin >> title;
                createNG(conn, title);
                break;
            case "delng":
                int id;
                cin >> id;
                deleteNG(conn, id);
                break;
            case "listart":
                break;
            case "crtart":
                break;
            case "delart":
                break;
            case "getart":
                break;
            case "exit":
                exit = true;
            
            default:
                cout << "Incorrect input, please enter a command with the following syntax:" << endl;
                help();
                break;
        }
    }
}

void listNG(const Connection& conn) {
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
  cout << endl;
  unsigned char endByte = conn.read(Protocol::ANS_END);
}

void writeNumP(const Connection& conn, int num){
    conn->write(Protocol::PAR_NUM);
    writeNumber(conn, num);
}

void writeStringP(const Connection& conn, string s){
    conn->write(Protocol::PAR_STRING);
    writeNumber(conn, s.size());
    for (char c : s){
        conn->write(c);
    }
}

void createNG(const Connection& conn, string title) {
  conn.write(Protocol::COM_CREATE_NG);
  writeStringP(conn, title);
  conn.write(Protocol::COM_END);
  unsigned char ansCreateNG = conn.read();
  unsigned char answer = conn.read();
  if (answer == Protocol::ANS_ACK) {
    cout << title << " successfully created on the server." << endl;
  } else if (conn.read() == Protocol::ERR_NG_ALREADY_EXISTS && answer == ANS_NAK) {
    cout << "Error: " << title << " already exists." << endl;
  }
  unsigned char endByte = conn.read();
}

void deleteNG(const Connection& conn, int groupID) {
  conn.write(Protocol::COM_DELETE_NG);
  writeNumP(groupID);
  conn.write(Protocol::COM_END);
  unsigned char ansDeleteNG = conn.read();
  unsigned char answer = conn.read();
  if (answer == Protocol::ANS_ACK) {
    cout << title << " successfully deleted on the server." << endl;
  } else if (conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST && answer == ANS_NAK) {
    cout << "Error: " << groupID << " does not exist." << endl;
  }
  unsigned char endByte = conn.read();
}

void listArt(const Connection& conn, int groupID) {
  conn.write(Protocol::COM_LIST_ART);
  writeNumP(groupID);
  conn.write(Protocol::COM_END);
  unsigned char ansListArt = conn.read();
  unsigned char answer = conn.read();
  if (answer ==  Protocol::ANS_ACK) {
    cout << "ID\tTitle" << endl;
    int nbrOfArticles = getNumP();
    for (int i = 0; i < nbrOfArticles; ++i) {
      int id = getNumP(conn);
      string title = getStringP(conn);
      cout << id << "\t" << title << endl;
    }
    cout << endl;
  } else if (conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST && answer == Protocol::ANS_NAK) {
    cout << "Error: " << groupID << " does not exist." << endl;
  }
  unsigned char endByte = conn.read();
}

void createArt(const Connection& conn, int groupID, string title, string author, string text) {
  conn.write(Protocol::COM_CREATE_ART);
  writeNumP(groupID);
  writeStringP(title);
  writeStringP(author);
  writeStringP(text);
  conn.write(Protocol::COM_END);
  unsigned char ansCreateArt = conn.read();
  unsigned char answer = conn.read();
  if (answer == Protocol::ANS_ACK) {
    cout << title << " successfully created on the server." << endl;
  } else if (conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST && answer == ANS_NAK) {
    cout << "Error: the group with ID " << groupID << " does not exist." << endl;
  }
  unsigned char endByte = conn.read();
}

void deleteArt(const Connection& conn, int groupID, int artID) {
  
}

void getArt(const Connection& conn, int groupID, int artID) {
  
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
	
		try {
		  run(conn);
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
}

