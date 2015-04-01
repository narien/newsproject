#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <limits>
#include <cstdio>


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
    }
    return s;
}
void help(){
    cout << "listng: List all newsgroups" << endl;
    cout << "crtng <groupName>: Create a newsgroup with name <groupName>." << endl;
    cout << "delng <groupID>: Delete newsgroup <groupID>." << endl;
    cout << "listart <groupID>: List articles in newsgroup <groupID>." << endl;
    cout << "crtart <groupID>: Create an article in newsgroup <groupID> by following the subsequent instructions." <<endl;
    cout << "delart <groupID> <artID>: Delete article <artID> in newsgroup <groupID>." << endl;
    cout << "getart <groupID> <artID>: Display article <artID> in newsgroup <groupID>" << endl;
    cout << "exit: Quit the client" << endl;
}

void listNG(const Connection& conn) {
    conn.write(Protocol::COM_LIST_NG);
    conn.write(Protocol::COM_END);
    conn.read();
    int nbrOfGroups = getNumP(conn);
    
    cout << "Newsgroups:" << endl;
    cout << "ID\tTitle" << endl;
    
    for (int i = 0; i < nbrOfGroups; ++i) {
        int id = getNumP(conn);
        string title = getStringP(conn);
        cout << id << "\t" << title << endl;
    }
    cout << endl;
    conn.read();
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
    conn.read();
    unsigned char answer = conn.read();
    if (answer == Protocol::ANS_ACK) {
        cout << title << " successfully created on the server." << endl;
    } else if (conn.read() == Protocol::ERR_NG_ALREADY_EXISTS && answer == Protocol::ANS_NAK) {
        cout << "Error: " << title << " already exists." << endl;
    }
    conn.read();
}

void deleteNG(const Connection& conn, int groupID) {
    conn.write(Protocol::COM_DELETE_NG);
    writeNumP( conn, groupID);
    conn.write(Protocol::COM_END);
    conn.read();
    unsigned char answer = conn.read();
    if (answer == Protocol::ANS_ACK) {
        cout << groupID << " successfully deleted on the server." << endl;
    } else if (conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST && answer == Protocol::ANS_NAK) {
        cout << "Error: " << groupID << " does not exist." << endl;
    }
    conn.read();
}

void listArt(const Connection& conn, int groupID) {
    conn.write(Protocol::COM_LIST_ART);
    writeNumP(conn, groupID);
    conn.write(Protocol::COM_END);
    conn.read();
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
        cout << "Error: NewsgroupID " << groupID << " does not exist." << endl;
    }
    conn.read();
}

void createArt(const Connection& conn, int groupID, string title, string author, string text) {
    conn.write(Protocol::COM_CREATE_ART);
    writeNumP(conn, groupID);
    writeStringP(conn, title);
    writeStringP(conn, author);
    writeStringP(conn, text);
    conn.write(Protocol::COM_END);
    conn.read();
    unsigned char answer = conn.read();
    if (answer == Protocol::ANS_ACK) {
        cout << title << " successfully created on the server." << endl;
    } else if (conn.read() == Protocol::ERR_NG_DOES_NOT_EXIST && answer == Protocol::ANS_NAK) {
        cout << "Error: the group with ID " << groupID << " does not exist." << endl;
    }
    conn.read();
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
        conn.read();
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
            cout << "Article text: " << endl << text << endl;
        } else if (answer == Protocol::ANS_NAK) {
            unsigned char errorType = conn.read();
            if (errorType == Protocol::ERR_NG_DOES_NOT_EXIST) {
                cout << "Error: the newsgroup with ID " << groupID << " does not exist." << endl;
            } else if (errorType == Protocol::ERR_ART_DOES_NOT_EXIST) {
                cout << "Error: the article with ID " << artID << " does not exist." << endl;
            }
        }
        conn.read();
    }
}

void enterArt(const Connection& conn){
    int groupID;
    cin >> groupID;
    string title;
    string author;
    string text;
    string line;
    if (cin.good()) {
        cout << "Enter title of the article:" << endl;
        
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n' );
        getline(cin, title);
        cout << "Enter author of the article:" << endl;
        
        getline(cin, author);
        cout << "Enter the text of the article, separate sections by pressing enter and finish with end-of-file (usually ctrl + d on a new line):" << endl;
        
        while (getline(cin, line)) {
            text += line;
            text += "\n";
        }
        
        cin.clear();
        clearerr(stdin);
        createArt(conn, groupID, title, author, text);
    } else {
        cin.clear();
    }
    
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
            if (cin.good())
                createNG(conn, title);
            else
                cin.clear();
        } else if (choice == "delng"){
            int groupID;
            cin >> groupID;
            if (cin.good())
                deleteNG(conn, groupID);
            else
                cin.clear();
        } else if (choice == "listart"){
            int groupID;
            cin >> groupID;
            if (cin.good())
                listArt(conn, groupID);
            else
                cin.clear();
        } else if (choice == "crtart"){
            enterArt(conn);
        } else if (choice == "delart"){
            int groupID;
            int artID;
            cin >> groupID;
            cin >> artID;
            if (cin.good())
                deleteArt(conn, groupID, artID);
            else
                cin.clear();
        } else if (choice == "getart"){
            int groupID;
            int artID;
            cin >> groupID;
            cin >> artID;
            if (cin.good())
                getArt(conn, groupID, artID);
            else
                cin.clear();
        } else if (choice == "exit"){
            exit = true;
        } else {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n' );
            cout << endl << "Incorrect input, please enter a command with the following syntax:" << endl;
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

