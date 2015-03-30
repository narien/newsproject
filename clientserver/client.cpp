//
//  client.cpp
//  
//
//  Created by Jakob Svemar on 2015-03-30.
//
//

#include "client.h"

#include <iostream>

using namespace std;

Client::Client() {}

void Client::help(){
    cout << "To send requests to the server type the command for the following actions:" << endl;
    cout << "1: List Newsgroups" << endl;
    cout << "2: Create NewsGroup" << endl;
    cout << "3: Delete NewsGroup" << endl;
    cout << "4: List Articles in a specific NewsGroup" << endl;
    cout << "5: Create Article" << endl;
    cout << "6: Delete Article" << endl;
    cout << "7: Get Article" << endl;
    cout << "help: Show this list again" << endl;
}

void Client::run(){
    help();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: myclient host-name port-number" << endl;
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
    Client c;
    
    try{
        c.run();
    }catch{
        cout << " no reply from server. Exiting." << endl;
        exit(1);
    }
}