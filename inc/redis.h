#ifndef _REDIS__H_
#define _REDIS__H_
#include "zlist.h"
#include "keyvalue.h"

#include <iostream>
#include <string>
using namespace std;

#define MAX_BUFFER_SIZE 2048

typedef enum
{
    REDIS_CMD_SET = 1,
    REDIS_CMD_GET,
    REDIS_CMD_SETBIT,
    REDIS_CMD_GETBIT,
    REDIS_CMD_ZADD,
    REDIS_CMD_ZRANGE,
    REDIS_CMD_ZCARD,
    REDIS_CMD_ZCOUNT,
    REDIS_CMD_SAVE,
}REDIS_CMD;

class Redis
{
    private:

        map<string, REDIS_CMD> cmd_to_id;

        map<string, ZList*>    zl;
        KeyValue               kv;
        static Redis           *redis;

        bool handleSetBitCmd(string *args, int count, int socket_fd);
        bool handleGetBitCmd(string *args, int count, int socket_fd);
        bool handleGetCmd(string *args, int count, int socket_fd);
        bool handleSetCmd(string *args, int count, int socket_fd);
        bool handleZAddCmd(string *args, int count, int socket_fd);
        bool handleZCardCmd(string *args, int count, int socket_fd);
        bool handleZCountCmd(string *args, int count, int socket_fd);
        bool handleZRangeCmd(string *args, int count, int socket_fd);
        bool handleSaveCmd(string *args, int count, int socket_fd);
    public:
        Redis()
        {
            cmd_to_id["get"] = REDIS_CMD_GET;
            cmd_to_id["set"] = REDIS_CMD_SET;
            cmd_to_id["getbit"] = REDIS_CMD_GETBIT;
            cmd_to_id["setbit"] = REDIS_CMD_SETBIT;

            cmd_to_id["zadd"] = REDIS_CMD_ZADD;
            cmd_to_id["zrange"] = REDIS_CMD_ZRANGE;
            cmd_to_id["zcard"] = REDIS_CMD_ZCARD;
            cmd_to_id["zcount"] = REDIS_CMD_ZCOUNT;
            cmd_to_id["save"] = REDIS_CMD_SAVE;

        }
        void Save(const char *filename);
        void Load(const char *filename);
        bool Execute(string *args, int count, int socket_fd);
        static Redis* GetInstance(void);
};

#endif
