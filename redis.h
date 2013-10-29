#ifndef _REDIS__H_
#define _REDIS__H_
#include "zlist.h"
#include "keyvalue.h"

#include <iostream>
#include <string>
using namespace std;

#define MAX_BUFFER_SIZE 2048

class Redis
{
    private:
        char buffer[2048];

        KeyValue kv;
        ZList    zl;
        static Redis *redis;

        bool handleGetCmd(string *args, int count, int socket_fd);
        bool handleSetCmd(string *args, int count, int socket_fd);
    public:
        bool Execute(string *args, int count, int socket_fd);
        static Redis* GetInstance(void);        
};

#endif
