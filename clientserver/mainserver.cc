#include "mainserver.h"
#include "protocol.h"
#include "databasememory.h"
#include "databasedisk.h"

#include <iostream>
#include <utility>
#include <string>
#include <vector>

using namespace std;

MainServer::MainServer(Server& server) {
    //if(smth)
    db = new DatabaseMemory();
    //else
    //wait with this till later...
    //db = new DatabaseDisk();

    this->server = &server;
}

MainServer::~MainServer() {
      delete server;
    //delete db; //ger warning, måste fixxas
}

/**
 COM_LIST_NG COM_END
 ANS_LIST_NG num_p [num_p string_p]* ANS_END
 **/
void MainServer::listNG(const std::shared_ptr<Connection>& conn) {
  unsigned char endByte = conn->read();
  if (endByte == Protocol::COM_END) {
      vector<pair<int, string>> groups = db->listNewsgroups();
      conn->write(Protocol::ANS_LIST_NG);
      writeNumP(conn, groups.size());
      for(auto p : groups) {
          writeNumP(conn, p.first);
          writeStringP(conn, p.second);
      }
      conn->write(Protocol::ANS_END);
  } else {
    server->deregisterConnection(conn);
  }
}

/**
 COM_CREATE_NG string_p COM_END
 ANS_CREATE_NG [ANS_ACK | ANS_NAK ERR_NG_ALREADY_EXISTS] ANS_END
 **/
void MainServer::createNG(const std::shared_ptr<Connection>& conn) {
  string title = getStringP(conn);
  unsigned char endByte = conn->read();
  if (endByte == Protocol::COM_END) {
      conn->write(Protocol::ANS_CREATE_NG);
      if(db->insertNewsgroup(title)){
          conn->write(Protocol::ANS_ACK);
      } else {
          conn->write(Protocol::ANS_NAK);
          conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
      }
      conn->write(Protocol::ANS_END);
  } else {
    server->deregisterConnection(conn);
  }
}


/**
 COM_DELETE_NG num_p COM_END
 ANS_DELETE_NG [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
 **/
void MainServer::deleteNG(const std::shared_ptr<Connection>& conn) {
  int groupID = getNumP(conn);
  unsigned char endByte = conn->read();
  if (endByte == Protocol::COM_END) {
      conn->write(Protocol::ANS_DELETE_NG);
      if(db->removeNewsgroup(groupID)){
          conn->write(Protocol::ANS_ACK);
      } else {
          conn->write(Protocol::ANS_NAK);
          conn->write(Protocol:: ERR_NG_DOES_NOT_EXIST);
      }
      conn->write(Protocol::ANS_END);
  } else {
    server->deregisterConnection(conn);
  }
}

/**
 COM_LIST_ART num_p COM_END
 ANS_LIST_ART [ANS_ACK num_p [num_p string_p]* |
 ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
 **/
void MainServer::listArt(const std::shared_ptr<Connection>& conn) {
  int groupID = getNumP(conn);
  unsigned char endByte = conn->read();
  if (endByte == Protocol::COM_END) {
      vector<pair<int, string>> articles;
      conn->write(Protocol::ANS_LIST_ART);
      if(db->listArticles(groupID, articles)){
          conn->write(Protocol::ANS_ACK);
          writeNumP(conn, articles.size());
          for(auto p : articles){
              writeNumP(conn, p.first);
              writeStringP(conn, p.second);
          }
      } else {
          conn->write(Protocol::ANS_NAK);
          conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
      }
      conn->write(Protocol::ANS_END);
  } else {
    server->deregisterConnection(conn);
  }
}

/**
 COM_CREATE_ART num_p string_p string_p string_p COM_END
 ANS_CREATE_ART [ANS_ACK | ANS_NAK ERR_NG_DOES_NOT_EXIST] ANS_END
 **/
void MainServer::createArt(const std::shared_ptr<Connection>& conn) {
  int groupID = getNumP(conn);
  string title = getStringP(conn);
  string author = getStringP(conn);
  string text = getStringP(conn);
  unsigned char endByte = conn->read();
  if (endByte == Protocol::COM_END) {
      if(db->insertArticle(groupID, title, author, text)){
          conn->write(Protocol::ANS_CREATE_ART);
          conn->write(Protocol::ANS_ACK);
      } else {
          conn->write(Protocol::ANS_NAK);
          conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
      }
      conn->write(Protocol::ANS_END);
  } else {
    server->deregisterConnection(conn);
  }
}

/**
 COM_DELETE_ART num_p num_p COM_END
 ANS_DELETE_ART [ANS_ACK |
 ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
 **/
void MainServer::deleteArt(const std::shared_ptr<Connection>& conn){
    int group = getNumP(conn);
    int art = getNumP(conn);
    unsigned char endByte = conn->read();
    if(endByte == Protocol::COM_END){
        int result = db->removeArticle(group, art);
        conn->write(Protocol::ANS_DELETE_ART);
        if (result == 1) {
            conn->write(Protocol::ANS_ACK);
        } else {
            conn->write(Protocol::ANS_NAK);
            if (result == 0) {
                conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
            } else {
                conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
            }
        }
        conn->write(Protocol::ANS_END);
    } else {
        server->deregisterConnection(conn);
    }
}

/**
 COM_GET_ART num_p num_p COM_END
 ANS_GET_ART [ANS_ACK string_p string_p string_p |
 ANS_NAK [ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
 **/
void MainServer::getArt(const std::shared_ptr<Connection>& conn){
    int group = getNumP(conn);
    int art = getNumP(conn);
    unsigned char endByte = conn->read();
    if(endByte == Protocol::COM_END){
        //database.getArt
        string title;
        string author;
        string text;
        int result = db->getArticle(group, art, title, author, text);
        conn->write(Protocol::ANS_GET_ART);
        if (result == 1){
            conn->write(Protocol::ANS_ACK);
            writeStringP(conn, title);
            writeStringP(conn, author);
            writeStringP(conn, text);
        } else {
            conn->write(Protocol::ANS_NAK);
            if (result == 0){
                conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
            } else {
                conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
            }
        }
        conn->write(Protocol::ANS_END);
    } else {
      server->deregisterConnection(conn);
    }
}

int MainServer::getNumP(const std::shared_ptr<Connection>& conn){
    unsigned char numPar = conn->read();
    int num = -1;
    if (numPar == Protocol::PAR_NUM) {
        num = readNumber(conn);
    } else {
        server->deregisterConnection(conn);
    }
    return num;
}

void MainServer::writeNumP(const std::shared_ptr<Connection>& conn, int num){
    conn->write(Protocol::PAR_NUM);
    writeNumber(conn, num);
}

string MainServer::getStringP(const std::shared_ptr<Connection>& conn) {
  unsigned char stringPar = conn->read();
  string s;
  if (stringPar == Protocol::PAR_STRING) {
    int nbrOfBytes = readNumber(conn);
    for (int i = 0; i < nbrOfBytes; ++i) {
      s += conn->read();
    }
  } else {
      server->deregisterConnection(conn);
  }
  return s;
}

void MainServer::writeStringP(const std::shared_ptr<Connection>& conn, string s){
    conn->write(Protocol::PAR_STRING);
    writeNumber(conn, s.size());
    for (char c : s){
        conn->write(c);
    }
}

int MainServer::readNumber(const std::shared_ptr<Connection>& conn) {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void MainServer::writeNumber(const std::shared_ptr<Connection>& conn, int value) {
    conn->write((value >> 24) & 0xFF);
    conn->write((value >> 16) & 0xFF);
    conn->write((value >> 8)	 & 0xFF);
    conn->write(value & 0xFF);
}

int main(int argc, char* argv[]){
    if (argc != 2) {
        cerr << "Usage: myserver port-number" << std::endl;
        exit(1);
    }
    
    int port = -1;
    try {
        port = std::stoi(argv[1]);
    } catch (std::exception& e) {
        cerr << "Wrong port number. " << e.what() << std::endl;
        exit(1);
    }
    
    Server server(port);
    if (!server.isReady()) {
        cerr << "Server initialization error." << std::endl;
        exit(1);
    }
    cout <<"1" << endl;
    MainServer MS(server);
    cout <<"2" << endl;

    while(true){
        cout <<"3" << endl;

        auto conn = server.waitForActivity();
        cout <<"4" << endl;

        if(conn != nullptr){
            cout <<"5" << endl;

            try {
	               unsigned int command = conn->read();
                cout <<"6" << endl;


              	 switch (command) {
              	      case Protocol::COM_LIST_NG :
                         cout << "LIST NG" << endl;
              		        MS.listNG(conn);
              		        break;
              	      case Protocol::COM_CREATE_NG :
                         cout << "CREATE NG" << endl;

              		        MS.createNG(conn);
              		        break;
              	      case Protocol::COM_DELETE_NG :
                         cout << "DELETE NG" << endl;

              		        MS.deleteNG(conn);
              		        break;
              	      case Protocol::COM_LIST_ART :
                         cout << "LIST ART" << endl;

              		        MS.listArt(conn);
              		        break;
              	      case Protocol::COM_CREATE_ART :
                         cout << "CREATE ART" << endl;

              		        MS.createArt(conn);
              		        break;
              	      case Protocol::COM_DELETE_ART :
                         cout << "DELETE ART" << endl;

              		        MS.deleteArt(conn);
              		        break;
              	      case Protocol::COM_GET_ART :
                         cout << "GET ART" << endl;

              		        MS.getArt(conn);
              		        break;
              	      default :
              		//todo felmeddelande till klienten
              		break;
                  }
            } //catch (ConnectionClosedException& e) { //temp removed
              catch (exception e) {
                server.deregisterConnection(conn);
                cout << "Client closed connection" << std::endl;
            }
        } else {
            conn = make_shared<Connection>(); //temp removed
            server.registerConnection(conn);
            cout << "New client connects" << std::endl;
        }
    }
}