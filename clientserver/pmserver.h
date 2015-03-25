//
//  pmserver.h
//  
//
//  Created by Jakob Svemar on 2015-03-24.
//
//

#ifndef ____pmserver__
#define ____pmserver__

#include <stdio.h>

class PMServer {
public:
    PMServer(int port);
    void listNG(const std::shared_ptr<Connection>& conn);
    void createNG(const std::shared_ptr<Connection>& conn);
    void deleteNG(const std::shared_ptr<Connection>& conn);
    void listArt(const std::shared_ptr<Connection>& conn);
    void getArt(const std::shared_ptr<Connection>& conn);
    void createArt(const std::shared_ptr<Connection>& conn);
    void deleteArt(const std::shared_ptr<Connection>& conn);
    
    string getString(const std::shared_ptr<Connection>& conn);
    
    
private:
};

#endif /* defined(____pmserver__) */
