#ifndef ____mainserver__
#define ____mainserver__

#include <stdio.h>
#include "server.h"
#include "databaseinterface.h"

class MainServer {
public:
    MainServer(Server& server);
    ~MainServer();
    void listNG(const std::shared_ptr<Connection>& conn);
    void createNG(const std::shared_ptr<Connection>& conn);
    void deleteNG(const std::shared_ptr<Connection>& conn);
    void listArt(const std::shared_ptr<Connection>& conn);
    void getArt(const std::shared_ptr<Connection>& conn);
    void createArt(const std::shared_ptr<Connection>& conn);
    void deleteArt(const std::shared_ptr<Connection>& conn);
    
    string getStringP(const std::shared_ptr<Connection>& conn);
    int getNumP(const std::shared_ptr<Connection>& conn);
    void writeNumP(const std::shared_ptr<Connection>& conn, int i);
    int readNumber(const std::shared_ptr<Connection>& conn);
    void writeNumber(const std::shared_ptr<Connection>& conn, int i);
    void writeStringP(const std::shared_ptr<Connection>& conn, string s);

private:
    Server* server;
    DatabaseInterface* db;
};
#endif /* defined(____mainserver__) */
