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

class MainServer {
public:
    MainServer(Server& server);
    void listNG(const std::shared_ptr<Connection>& conn);
    void createNG(const std::shared_ptr<Connection>& conn);
    void deleteNG(const std::shared_ptr<Connection>& conn);
    void listArt(const std::shared_ptr<Connection>& conn);
    void getArt(const std::shared_ptr<Connection>& conn);
    void createArt(const std::shared_ptr<Connection>& conn);
    void deleteArt(const std::shared_ptr<Connection>& conn);
    
    string getString(const std::shared_ptr<Connection>& conn);
    int getNumP(const std::shared_ptr<Connection>& conn);
    void writeNumP(const std::shared_ptr<Connection>& conn);
    int readNumber(const std::shared_ptr<Connection>& conn);
    void writeNumber(const std::shared_ptr<Connection>& conn);
    void writeStringP(const std::shared_ptr<Connection>& conn, string s);

    
private:
    DatabaseInterface db;
    Server& server;
};

#endif /* defined(____mainserver__) */
