//
//  pmserver.cpp
//  
//
//  Created by Jakob Svemar on 2015-03-24.
//
//

#include "pmserver.h"

PMServer::PMServer(int port){}





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