//
//  pmserver.cpp
//  
//
//  Created by Jakob Svemar on 2015-03-24.
//
//

#include "pmserver.h"
#include "protocol.h"

PMServer::PMServer(int port){}

void PMServer::listNG(const std::shared_ptr<Connection>& conn) {
  unsigned char endByte = conn->read();
  if (endByte == Protocol::COM_END) {
    //database.listnewsgrops
  } else {
    //felmeddelande
  }
}

void PMServer::createNG(const std::shared_ptr<Connection>& conn) {
  string title = getStringP;
}

PMServer::getArt(std::shared_ptr<Connection>& conn){
    int group = getNumP(conn);
    int art = getNumP(conn);
    unsigned char end = conn.read();
    if(end == Protocol::COM_END){
        //database.getArt
    } else {
        //felmeddelande
        
    }
}

int PMServer::getNumP(const std::shared_ptr<Connection>& conn){
    unsigned char numPar = conn->read();
    int num;
    if (numPar == Protocol::PAR_NUM) {
        num = readNumber(conn);
    } else {
        //felmeddelande
    }
    return num;
}

string PMServer::getStringP(const std::shared_ptr<Connection>& conn) {
  unsigned char stringPar = conn->read();
  string title;
  if (stringPar == Protocol::PAR_STRING) {
    int nbrOfBytes = readNumber(conn);
    for (int i = 0; i < nbrOfBytes; ++i) {
      title += conn->read();
    }
  } else {
    //felmeddelande
  }
}

int readNumber(const shared_ptr<Connection>& conn) {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

int main(int argc, char* argv[]){
    if (argc != 2) {
        cerr << "Usage: myserver port-number" << endl;
        exit(1);
    }
    
    int port = -1;
    try {
        port = stoi(argv[1]);
    } catch (exception& e) {
        cerr << "Wrong port number. " << e.what() << endl;
        exit(1);
    }
    
    Server server(port);
    if (!server.isReady()) {
        cerr << "Server initialization error." << endl;
        exit(1);
    }
    
    while(true){
        auto conn = server.waitForActivity();
        if(conn != nullptr){
            try {
	      unsigned char command = conn->read();
	      switch (command) {
	      case Protocol::COM_LIST_NG : 
		pms.listNG(conn);
		break;
	      case Protocol::COM_CREATE_NG :
		pms.createNG(conn);
		break;
	      case Protocol::COM_DELETE_NG : 
		pms.deleteNG(conn);
		break;
	      case Protocol::COM_LIST_ART :
		pms.listArt(conn);
		break;
	      case Protocol::COM_CREATE_ART : 
		pms.createArt(conn);
		break;
	      case Protocol::COM_DELETE_ART :
		pms.deleteArt(conn);
		break;
	      case Protocol::COM_GET_ART : 
		pms.getArt(conn);
		break;
	      default :
		//todo felmeddelande till klienten
		break;
            } catch (ConnectionClosedException&) {
                server.deregisterConnection(conn);
                cout << "Client closed connection" << endl;
            }
        } else {
            conn = make_shared<connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
        }
    }
