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
      Vector<pair<int, string>> groups = db.listNewsgrops();
      conn->write(Protocol::ANS_LIST_NG);
      writeNumP(groups.size());
      for(Pair p : groups) {
          writeNumP(conn, p.first);
          writeStringP(conn, p.second);
      }
      conn->write(Protocol::ANS_END);
  } else {
    //felmeddelande
  }
}

void PMServer::createNG(const std::shared_ptr<Connection>& conn) {
  string title = getStringP(conn);
  unsigned char endByte = conn->read();
  if (endByte == Protocol::COM_END) {
    //skapa nyhetsgruppen och lägg till i databasen
      conn->write(Protocol::ANS_CREATE_NG);
      if(db.insert_newsgroup(title)){
          conn->write(Protocol::ANS_ACK);
      } else {
          conn->write(Protocol::ANS_NAK);
          conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
      }
      conn->write(Protocol::ANS_END);
  } else {
    //felmeddelande
  }
}

void PMServer::deleteNG(const std::shared_ptr<Connection>& conn) {
  int groupID = getNumP(conn);
  unsigned char endByte = conn->read();
  if (endByte == Protocol::COM_END) {
    //ta bort nyhetsgruppen
  } else {
    //felmeddelande
  }
}

void PMServer::listArt(const std::shared_ptr<Connection>& conn) {
  int groupID = getNumP(conn);
  unsigned char endByte = conn->read();
  if (endByte == Protocol::COM_END) {
    //lista artiklarna i nyhetsgruppen med id groupID
  } else {
    //felmeddelande
  }
}

void PMServer::createArt(const std::shared_ptr<Connection>& conn) {
  int groupID = getNumP(conn);
  string title = getStringP(conn);
  string author = getStringP(conn);
  string text = getStringP(conn);
  if (endByte == Protocol::COM_END) {
    //skapa artikeln och lägg i databasen
  } else {
    //felmeddelande
  }
}

/**
 COM_DELETE_ART num_p num_p COM_END
 ANS_DELETE_ART [ANS_ACK |
 ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
 **/
void PMServer::deleteArt(){
    int group = getNumP(conn);
    int art = getNumP(conn);
    unsigned char endByte = conn->read();
    if(endByte == Protocoll::COM_END){
        //database.deleteArt
    } else {
        //felmeddelande
    }
}

void PMServer::getArt(std::shared_ptr<Connection>& conn){
    int group = getNumP(conn);
    int art = getNumP(conn);
    unsigned char endByte = conn.read();
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

void PMserver::writeNumP(const std::shared_ptr<Connection>& conn, int num){
    conn->write(Protocol::PAR_NUM);
    writeNumber(conn, num);
}

string PMServer::getStringP(const std::shared_ptr<Connection>& conn) {
  unsigned char stringPar = conn->read();
  string s;
  if (stringPar == Protocol::PAR_STRING) {
    int nbrOfBytes = readNumber(conn);
    for (int i = 0; i < nbrOfBytes; ++i) {
      s += conn->read();
    }
  } else {
    //felmeddelande
  }
  return s;
}

void PMServer::writeStringP(const std::shared_ptr<Connection>& conn, string s){
    conn->write(Protocol::PAR_STRING);
    writeNumber(s.size());
    for (char c : s){
        conn->write(c);
    }
}

int PMServer::readNumber(const shared_ptr<Connection>& conn) {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void PMServer::writeNumber(const shared_ptr<Connection>& conn, int value) {
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)	 & 0xFF);
    conn.write(value & 0xFF);
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
