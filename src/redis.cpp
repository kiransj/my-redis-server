#include "redis.h"

#include <algorithm>
#include <string>
#include <map>
using namespace std;

Redis *Redis::redis = NULL;

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
}REDIS_CMD;


bool Redis::handleSetCmd(string *args, int count, int socket_fd)
{
    bool NX = false, XX = false;
    unsigned int exp_time = 0;
    if(count < 3)
    {
        send_msg(socket_fd, "-Err Wrong number of arguments\r\n");
        return false;
    }

    for(int i = 3; i < count; i++)
    {        
        std::transform(args[i].begin(), args[i].end(), args[i].begin(), ::tolower);
        if(args[i] == "ex" || args[i] == "px")
        {
            if(i+1 == count)
            {
                send_msg(socket_fd, "-ERR wrong number of args\r\n");
            }
            const char *ptr = args[++i].c_str();
            int idx = 0;
            while(ptr[idx] != 0)
            {
                if(!isdigit(ptr[idx]))
                {
                    send_msg(socket_fd, "-Err value is not a integer or out of range\r\n");
                }
                idx++;
            }
            exp_time = atoi(args[i].c_str());
            if(args[i-1] == "ex")
                exp_time *= 1000;
        }
        else if(args[i] == "nx")
            NX = true;
        else if(args[i] == "xx")
            XX = true;
        else
        {
            send_msg(socket_fd, "-Err ERR syntax error near '%s'\r\n", args[i].c_str());
            return false;
        }

    }
    if(kv.SET(args[1], args[2], exp_time, NX, XX) == true)
    {
        send_msg(socket_fd, "+OK\r\n");
    }
    else
    {
        send_msg(socket_fd, "$-1\r\n");
        return false;
    }
    return true;
}

bool Redis::handleGetCmd(string *args, int count, int socket_fd)
{
    BitArray *b;
    if(count != 2)
    {
        send_msg(socket_fd, "-Wrong number of arguments for 'GET' command\r\n");
        return false;
    }
    if(kv.GET(args[1], &b) == true)
    {
        uint32_t size = 0;
        const char *str;
        str = (char*)b->GetString(&size);
        send_msg(socket_fd, "$%d\r\n%s\r\n", size, str);
    }
    else
    {
        send_msg(socket_fd, "$-1\r\n");
    }

    return true;
}


bool Redis::handleZAddCmd(string *args, int count, int socket_fd)
{
    int ret = 0;
    if(count < 4)
    {
        send_msg(socket_fd, "-Wrong number of arguments for 'ZADD' command\r\n");
        return false;
    }

    ret = zl.ZADD(atoi(args[2].c_str()), args[3]);
    send_msg(socket_fd, ":%d\r\n", ret);

    return true;
}

bool Redis::handleZCardCmd(string *args, int count, int socket_fd)
{
    int ret = 0;
    if(count < 2)
    {
        send_msg(socket_fd, "-Err Wrong number of arguments for 'ZADD' command\r\n");
        return false;
    }

    ret = zl.ZCARD();
    send_msg(socket_fd, ":%d\r\n", ret);

    return true;

}
bool Redis::Execute(string *args, int count, int socket_fd)
{
    bool flag = false;
    map<string, REDIS_CMD> cmd_to_id;
    cmd_to_id["get"] = REDIS_CMD_GET;
    cmd_to_id["set"] = REDIS_CMD_SET;
    cmd_to_id["getbit"] = REDIS_CMD_GETBIT;
    cmd_to_id["setbit"] = REDIS_CMD_SETBIT;

    cmd_to_id["zadd"] = REDIS_CMD_ZADD;
    cmd_to_id["zrange"] = REDIS_CMD_ZRANGE;
    cmd_to_id["zcard"] = REDIS_CMD_ZCARD;
    cmd_to_id["zcount"] = REDIS_CMD_ZCOUNT;

    std::transform(args[0].begin(), args[0].end(), args[0].begin(), ::tolower);
    switch(cmd_to_id[args[0]])
    {
        case REDIS_CMD_SET:
            flag = handleSetCmd(args, count, socket_fd);
            break;
        case REDIS_CMD_GET:
            flag = handleGetCmd(args, count, socket_fd);
            break;
        case REDIS_CMD_SETBIT:
        case REDIS_CMD_GETBIT:
        case REDIS_CMD_ZADD:
            flag = handleZAddCmd(args, count, socket_fd);
            break;
        case REDIS_CMD_ZCARD:
            flag = handleZCardCmd(args, count, socket_fd);
            break;
        case REDIS_CMD_ZRANGE:
        case REDIS_CMD_ZCOUNT:
        default:
            log_msg("unknown cmd '%s'", args[0].c_str());
            send_msg(socket_fd, "-Err Unknown command '%s'\r\n", args[0].c_str());
    }
    return flag;
}

Redis* Redis::GetInstance(void)
{
    if(IS_NULL(redis))
    {
        redis = new Redis;
    }
    return Redis::redis;
}

