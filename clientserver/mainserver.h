//
//  pmserver.h
//  
//
//  Created by Jakob Svemar on 2015-03-24.
//
//

#ifndef ____mainserver__
#define ____mainserver__

#include <stdio.h>
#include "server.h"

tpedef std::shared_ptr<Connection>& Conn;

class MainServer {
public:
    MainServer(Server& server);
    void listNG(const Conn conn);
    void createNG(const Conn conn);
    void deleteNG(const Conn conn);
    void listArt(const Conn conn);
    void getArt(const Conn conn);
    void createArt(const Conn conn);
    void deleteArt(const Conn conn);
    
    string getString(const Conn conn);
    int getNumP(const Conn conn);
    void writeNumP(const Conn conn);
    int readNumber(const Conn conn);
    void writeNumber(const Conn conn);
    void writeStringP(const Conn conn, string s);

    
private:
    DatabaseInterface db;
};

#endif /* defined(____mainserver__) */
