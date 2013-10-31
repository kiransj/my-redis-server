#include "redis.h"

#include <algorithm>
#include <string>
#include <map>
using namespace std;

Redis *Redis::redis = NULL;

void strtolower(string &str)
{
    int len = str.length(), i = 0;
    while(i < len)
    {
        str[i] = tolower(str[i]);
        i++;
    }
}

bool StringToInt(const char *s, int *num)
{
    int sign = 1;
    int idx = 0;
    int number = 0;

    if(s[idx] == '-')
    {
        sign = -1;
        idx++;
    }
    while(s[idx] != 0)
    {
        if(isdigit(s[idx]))
        {
            number = number*10 + (s[idx] - '0');
        }
        else
        {
            return false;
        }
        idx++;
    }    
    *num = number * sign;
    return true;
}
bool Redis::handleSetCmd(string *args, int count, int socket_fd)
{
    bool NX = false, XX = false;
    int exp_time = 0;
    if(count < 3)
    {
        send_msg(socket_fd, "-Err Wrong number of arguments for 'SET' command\r\n");
        return false;
    }

    for(int i = 3; i < count; i++)
    {      
        bool is_ex = false;
        strtolower(args[i]);
        if((is_ex = (args[i] == "ex")) || args[i] == "px")
        {
            if(i+1 == count)
            {
                send_msg(socket_fd, "-ERR wrong number of args\r\n");
                return false;
            }
            else if(StringToInt(args[i+1].c_str(), &exp_time) == false)
            {
                send_msg(socket_fd, "-Err value is not a integer or out of range\r\n");
                return false;
            }
            if(is_ex)
                exp_time *= 1000;
            i++;
        }
        else if(args[i] == "nx")
        {
            NX = true;
        }
        else if(args[i] == "xx")            
        {
            XX = true;
        }
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
    ZList *z;
    int ret = 0;
    if(count < 4)
    {
        send_msg(socket_fd, "-Wrong number of arguments for 'ZADD' command\r\n");
        return false;
    }

    z = zl[args[1]];
    if(IS_NULL(z))
    {
        z = new ZList;
        if(IS_NULL(z))
        {
            send_msg(socket_fd, ":0\r\n");
            log_error("memory allocation failed in %s:%d", __FILE__, __LINE__);
            return false;
        }
        zl[args[1]] = z;
    }
    int score = 0;
    if(StringToInt(args[2].c_str(), &score) == false)
    {
        send_msg(socket_fd, "-Err score is not a integer or out of range\r\n");
        return false;
    }
    ret = z->ZADD(score, args[3]);
    send_msg(socket_fd, ":%d\r\n", ret);

    return true;
}

bool Redis::handleZCardCmd(string *args, int count, int socket_fd)
{
    ZList *z;
    int ret = 0;
    if(count < 2)
    {
        send_msg(socket_fd, "-Err Wrong number of arguments for 'ZCARD' command\r\n");
        return false;
    }
    z = zl[args[1]];
    if(IS_NULL(z))
    {
        send_msg(socket_fd, ":0\r\n");
    }
    else
    {
        ret = z->ZCARD();
        send_msg(socket_fd, ":%d\r\n", ret);
    }

    return true;
}

bool Redis::handleZCountCmd(string *args, int count, int socket_fd)
{
    ZList *z;
    int min,max;
    int ret = 0;
    if(count != 4)
    {
        send_msg(socket_fd, "-Err Wrong number of arguments for 'ZCOUNT' command\r\n");
        return false;
    }

    z = zl[args[1]];
    if(IS_NULL(z))
    {
        send_msg(socket_fd, ":0\r\n");
        return false;
    }
    
    if(StringToInt(args[2].c_str(), &min) == false)
    {
        send_msg(socket_fd, "-Err max value is not a integer or out of range\r\n");
        return false;
    }
    if(StringToInt(args[3].c_str(), &max) == false)
    {
        send_msg(socket_fd, "-Err max value is not a integer or out of range\r\n");
        return false;
    }
    
    log_msg("Zcount min=%d max=%d", min, max);
    ret = z->ZCOUNT(min, max);
    send_msg(socket_fd, ":%d\r\n", ret);

    return true;

}

bool Redis::handleZRangeCmd(string *args, int count, int socket_fd)
{
    ZList *z;
    int min,max;
    int ret = 0;
    if(count < 4)
    {
        send_msg(socket_fd, "-Err Wrong number of arguments for 'ZCOUNT' command\r\n");
        return false;
    }

    z = zl[args[1]];
    if(IS_NULL(z))
    {
        send_msg(socket_fd, "*0\r\n");
        return false;
    }
    if(StringToInt(args[2].c_str(), &min) == false)
    {
        send_msg(socket_fd, "-Err min value is not a integer or out of range\r\n");
        return false;
    }
    if(StringToInt(args[3].c_str(), &max) == false)
    {
        send_msg(socket_fd, "-Err max value is not a integer or out of range\r\n");
        return false;
    }

    ret = z->ZRANGE(min, max, false)+1;
    send_msg(socket_fd, "*%d\r\n", ret);

    int score, length = 0;
    char *buffer;
    while(z->GetNext(&score, &buffer, &length) && (ret > 0))
    {
        int rets = 0;
        /* Varify the return value of send_msg as if the connection is broken then
         * we have to stop the loop*/
        rets = send_msg(socket_fd, "$%d\r\n", length);
        rets += send_binary(socket_fd, buffer, length);
        rets += send_msg(socket_fd, "\r\n");
        if(rets <= 0)
        {
            log_error("send_msg() failed, could be broken pipe");
            return false;
        }
        --ret;
    }
    return true;
}

bool Redis::Execute(string *args, int count, int socket_fd)
{
    bool flag = false;

    strtolower(args[0]);
    switch(cmd_to_id[args[0]])
    {
        case REDIS_CMD_SET:
            flag = handleSetCmd(args, count, socket_fd);
            break;
        case REDIS_CMD_GET:
            flag = handleGetCmd(args, count, socket_fd);
            break;
        case REDIS_CMD_ZADD:
            flag = handleZAddCmd(args, count, socket_fd);
            break;
        case REDIS_CMD_ZCARD:
            flag = handleZCardCmd(args, count, socket_fd);
            Save(NULL);
            break;
        case REDIS_CMD_ZRANGE:
            flag = handleZRangeCmd(args, count, socket_fd);
            break;
        case REDIS_CMD_ZCOUNT:
            flag = handleZCountCmd(args, count, socket_fd);
            break;
        case REDIS_CMD_SETBIT:
        case REDIS_CMD_GETBIT:
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

#include <netinet/in.h>
#define SET_MAGIC 0xFF00FF43
#define ZADD_MAGIC 0xFF00FF44
void Redis::Load(const char *filename)
{
    int count = 0;
    FILE *fp = fopen(filename, "rb");
    if(!IS_NULL(fp))
    while(!feof(fp))
    {
        uint32_t type = 0;
        if(fread(&type, 1, 4, fp) != 4)
            break;
        switch(ntohl(type))
        {
            case SET_MAGIC:
                {
                    log_msg("loading");
                    char buffer[4096], buffer2[4096];
                    int key_length, data_length;

                    fread(&key_length, 1, 4, fp);
                    key_length = ntohl(key_length);                    
                    fread(buffer, 1, key_length, fp);

                    fread(&data_length, 1, 4, fp);
                    data_length = ntohl(data_length);
                    fread(buffer2, 1, data_length, fp);

                    kv.SET(string(buffer, key_length), string(buffer2, data_length), 0, false, false);
                    count++;
                    break;
                }
            case ZADD_MAGIC:
                {
                    log_msg("loading");
                    char buffer[4096] = { 0 }, buffer2[4096] = { 0 };
                    int key_length, data_length, score;

                    fread(&key_length, 1, 4, fp);
                    key_length = ntohl(key_length);                    
                    fread(buffer, 1, key_length, fp);

                    fread(&score, 1, 4, fp);
                    score = ntohl(score);
                    fread(&data_length, 1, 4, fp);
                    data_length = ntohl(data_length);
                    fread(buffer2, 1, data_length, fp);

                    ZList *z = zl[buffer];
                    if(IS_NULL(z))
                    {
                        z = new ZList;
                        zl[buffer] = z;
                    }                    
                    z->ZADD(score, string(buffer2, data_length));
                    count++;
                    break;
                }
            default:
                log_error("invalid type");
                abort();
        }
    }
    log_error("total number of elements loaded : %d", count);
    return;
}
void Redis::Save(const char *filename)
{
    uint8_t *value = NULL;
    unsigned int length;
    FILE *fp;

    string str;

    fp = fopen(filename, "wb");
    if(IS_NULL(fp))
    {
        log_error("Unable to open file '%s'", filename);
        return;
    }
    kv.BeginIterator();
    
    int count = 0;
    while(kv.GetNext(&str, &value, &length))
    {
        uint32_t type = htonl(SET_MAGIC);
        int key_length = htonl(str.size());
        int data_length = htonl(length);
        fwrite(&type, 1, 4, fp);
        fwrite(&key_length, 1, 4, fp);
        fwrite(str.c_str(), 1, str.size(), fp);
        fwrite(&data_length, 1, 4, fp);
        fwrite(value, 1, length, fp);       
        count++;
    }

    map<string, ZList*>::iterator ii = zl.begin();
    while(ii != zl.end())
    {
        const char *str = ii->first.c_str();
        int str_size = ii->first.size();
        int tmp_str_size = htonl(str_size);
        ZList *z = ii->second;
        if(IS_NULL(z))
        {
            ii++;
            continue;
        }
        int ret = z->ZRANGE(0, -1, false)+1;
        int score, length = 0;
        char *buffer;
        while(z->GetNext(&score, &buffer, &length) && (ret > 0))
        {
            uint32_t type = htonl(ZADD_MAGIC);
            int tmp_buf_len = htonl(length);
            fwrite(&type, 1, 4, fp);
            fwrite(&tmp_str_size, 1, 4, fp);
            fwrite(str, 1, str_size, fp);
            score = htonl(score);
            fwrite(&score, 1, 4, fp);
            fwrite(&tmp_buf_len, 1, 4, fp);
            fwrite(buffer, 1, length, fp);
            ret--;
            count++;
        }
        ii++;
    }
    log_error("%d elements saved", count);
    fclose(fp);
}
