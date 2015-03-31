#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>


using namespace std;

int readNumber(const Connection& conn) {
    unsigned char byte1 = conn.read();
    unsigned char byte2 = conn.read();
    unsigned char byte3 = conn.read();
    unsigned char byte4 = conn.read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void writeNumber(const Connection& conn, int value) {
    conn.write((value >> 24) & 0xFF);
    conn.write((value >> 16) & 0xFF);
    conn.write((value >> 8)	 & 0xFF);
    conn.write(value & 0xFF);
}

int getNumP(const Connection& conn){
    unsigned char numPar = conn.read();
    int num = -1;
    if (numPar == Protocol::PAR_NUM) {
        num = readNumber(conn);
    } else {
        // server->deregisterConnection(conn);
    }
    return num;
}

string getStringP(const Connection& conn) {
    unsigned char stringPar = conn.read();
    string s;
    if (stringPar == Protocol::PAR_STRING) {
        int nbrOfBytes = readNumber(conn);
        for (int i = 0; i < nbrOfBytes; ++i) {
            s += conn.read();
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

void listNG(const Connection& conn) {
    conn.write(Protocol::COM_LIST_NG);
    conn.write(Protocol::COM_END);
    unsigned char ansList = conn.read();
    int nbrOfGroups = getNumP(conn);
    
    cout << "Newsgroups:" << endl;
    cout << "ID\tTitle" << endl;
    
    for (int i = 0; i < nbrOfGroups; ++i) {
        int id = getNumP(conn);
        string title = getStringP(conn);
        cout << id << "\t" << title << endl;
    }
    cout << endl;
    unsigned char endByte = conn.read();
}

void writeNumP(const Connection& conn, int num){
    conn.write(Protocol::PAR_NUM);
    writeNumber(conn, num);
}

void writeStringP(const Connection& conn, string s){
    conn.write(Protocol::PAR_STRING);
    writeNumber(conn, s.size());
    for (char c : s){
        conn.write(c);
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
    } else if (conn.read() == Protocol::ERR_NG_ALREADY_EXISTS && answer == Protocol::ANS_NAK) {
        cout << "Error: " << title << " already exists." << endl;
    }
    unsigned char endByte = conn.read();
}

void deleteNG(const Connection& conn, int groupID) {
    conn.write(Protocol::COM_DELETE_NG);
    writeNumP( conn, groupID);
    conn.write(Protocol::COM_END);
    unsigned char ansDeleteNG = conn.read();
    unsigned char answer = conn.read();
    if (answer == Protocol::ANS_ACK) {
        cout << groupID << " successfully deleted on the server." << endl;
    } else if (conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST && answer == Protocol::ANS_NAK) {
        cout << "Error: " << groupID << " does not exist." << endl;
    }
    unsigned char endByte = conn.read();
}

void listArt(const Connection& conn, int groupID) {
    conn.write(Protocol::COM_LIST_ART);
    writeNumP(conn, groupID);
    conn.write(Protocol::COM_END);
    unsigned char ansListArt = conn.read();
    unsigned char answer = conn.read();
    if (answer ==  Protocol::ANS_ACK) {
        cout << "ID\tTitle" << endl;
        int nbrOfArticles = getNumP(conn);
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
    writeNumP(conn, groupID);
    writeStringP(conn, title);
    writeStringP(conn, author);
    writeStringP(conn, text);
    conn.write(Protocol::COM_END);
    unsigned char ansCreateArt = conn.read();
    unsigned char answer = conn.read();
    if (answer == Protocol::ANS_ACK) {
        cout << title << " successfully created on the server." << endl;
    } else if (conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST && answer == Protocol::ANS_NAK) {
        cout << "Error: the group with ID " << groupID << " does not exist." << endl;
    }
    unsigned char endByte = conn.read();
}

void deleteArt(const Connection& conn, int groupID, int artID) {
    conn.write(Protocol::COM_DELETE_ART);
    writeNumP(conn, groupID);
    writeNumP(conn, artID);
    conn.write(Protocol::COM_END);
    unsigned char answer = conn.read();
    if (answer == Protocol::ANS_DELETE_ART) {
        answer = conn.read();
        if (answer == Protocol::ANS_ACK) {
            cout << artID << " successfully deleted." << endl;
        } else if (answer == Protocol::ANS_NAK) {
            unsigned char errorType = conn.read();
            if (errorType == Protocol::ERR_NG_DOES_NOT_EXIST) {
                cout << "Error: the group with ID " << groupID << " does not exist." << endl;
            } else if (errorType == Protocol::ERR_ART_DOES_NOT_EXIST) {
                cout << "Error: the article with ID " << artID << " does not exist." << endl;
            }
        }
        unsigned char endByte = conn.read();
    }
}

void getArt(const Connection& conn, int groupID, int artID) {
    conn.write(Protocol::COM_GET_ART);
    writeNumP(conn, groupID);
    writeNumP(conn, artID);
    conn.write(Protocol::COM_END);
    unsigned char answer = conn.read();
    if(answer == Protocol::ANS_GET_ART) {
        answer = conn.read();
        if (answer == Protocol::ANS_ACK) {
            string title = getStringP(conn);
            string author = getStringP(conn);
            string text = getStringP(conn);
            cout << "Title: " <<  title << endl;
            cout << "Author: " << author << endl;
            cout << "Article text: " << text << endl;
        } else if (answer == Protocol::ANS_NAK) {
            cout << "server accepterar inte" << endl;
            
            unsigned char errorType = conn.read();
            if (errorType == Protocol::ERR_NG_DOES_NOT_EXIST) {
                cout << "Error: the newsgroup with ID " << groupID << " does not exist." << endl;
            } else if (errorType == Protocol::ERR_ART_DOES_NOT_EXIST) {
                cout << "Error: the article with ID " << artID << " does not exist." << endl;
            }
        }
        unsigned char endByte = conn.read();
    }
}

void enterArt(const Connection& conn){
  int groupID;
    cin >> groupID;
    cout << "Enter title of the article:" << endl;
    string title;
    cin.ignore();
    cin.clear();
    getline(cin, title);
    cout << "Enter author of the article:" << endl;
    string author;
    cin.ignore(-1);
    cin.clear();
    getline(cin, author);
    cout << "Enter the text of the article:" << endl;
    string text;
    cin.ignore(-1);
    cin.clear();
    getline(cin, text);
    cin.sync();
    createArt(conn, groupID, title, author, text);
}

void run(const Connection& conn) {
    help();
    
    string choice;
    bool exit = false;
    while(!exit){
        cin >> choice;
        if(choice == "listng"){
            listNG(conn);
        }else if (choice == "crtng"){
            string title;
            cin >> title;
            createNG(conn, title);
        } else if (choice == "delng"){
            int groupID;
            cin >> groupID;
            deleteNG(conn, groupID);
        } else if (choice == "listart"){
            int groupID;
            cin >> groupID;
            listArt(conn, groupID);
        } else if (choice == "crtart"){
            enterArt(conn);
        } else if (choice == "delart"){
            int groupID;
            int artID;
            cin >> groupID;
            cin >> artID;
            deleteArt(conn, groupID, artID);
        } else if (choice == "getart"){
            int groupID;
            int artID;
            cin >> groupID;
            cin >> artID;
            getArt(conn, groupID, artID);
        } else if (choice == "exit"){
            exit = true;
            
        } else {
            cout << "Incorrect input, please enter a command with the following syntax:" << endl;
            help();
        }
    }
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

