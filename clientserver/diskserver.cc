#include "server.h"
#include "databasedisk.h"
#include "mainserver.h"

#include <stdio.h>
#include <iostream>

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
    DatabaseDisk db;
    MainServer MS(server, db);
    MS.run();
}
