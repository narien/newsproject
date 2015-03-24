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
    listNG();
    createNG();
    deleteNG();
    listArt();
    readArt();
    writeArt();
    deleteArt();
    
private:
};

#endif /* defined(____pmserver__) */
