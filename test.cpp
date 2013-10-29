#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#include "keyvalue.h"
#include "zlist.h"

#include "redis.h"

#include <iostream>
#include <string>
using namespace std;

int test_zlist(int argc, char *argv[])
{
    ZList zl;
    int count = argc >= 2 ? atoi(argv[1]) : 1000;

    for(int i = 0; i < count; i++)
    {
        int key = random() % 3214600 + 1;
        char buffer[64];
        snprintf(buffer, 64, "N%d", key);
        zl.ZADD(key, buffer);
        if(key % 7 == 0)
        {
            snprintf(buffer, 64, "N-%d", key);
            zl.ZADD(key, buffer);
        }
    }
    zl.ZADD(56394, "kiran");
    zl.ZADD(898784, "ind");
    zl.ZADD(898784, "pak");
    cout<<"ZCARD MYSET = "<<zl.ZCARD()<<endl;
    cout<<"ZCOUNT MYSET = "<<zl.ZCOUNT(0, 100000)<<endl;
    cout<<"ZRANK MYSET = "<<zl.ZRANK("N0940037")<<endl;
    zl.check();
    if(argc == 4)
    {
        int min = atoi(argv[2]), max = atoi(argv[3]);
        cout<<endl<<endl<<"ZRange("<<min<<", "<<max<<")"<<endl;
        if(zl.ZRANGEBYSCORE(min, max, false))
        {
            char str[1024];
            int key = 0, count = 0;
            while(zl.GetNext(&key, str, 1024))
            {
                cout<<++count<<"> "<<key<<" = "<<str<<endl;
            }
        }
    }
    return 0;
}

int test_avltree(int argc, char *argv[])
{
    int tmp = 0;
    int i = 0, count;
    Tree<int> t;

    count = argc > 1 ? atoi(argv[1]) : 100;

    time_t tm = time(NULL);
    cout<<"Seed "<<tm<<endl;
    srandom(tm);
    for(i = count; i >= 1; i--)
    {
        int key = (random() % 3214300 + 1);
        char buffer[1024];
        snprintf(buffer, 1024, "'%d", key+tmp++); 
        t.AddNode(key, string(buffer));
        snprintf(buffer, 1024, "'%d", key+tmp++); 
        t.AddNode(key, string(buffer));
    }
     
    t.AddNode(0, "Zero");
    t.AddNode(0, "Zero1");
    t.AddNode(0, "Zero2");
    t.AddNode(0, "Zero3");
    t.CheckList();

    int rt = 0;
    Node<int> *n = t.GetNthElement(18400, &rt);
    if(!IS_NULL(n))    
        cout<<"18400th element : "<<n->GetKey()<<"  t = "<<rt<<endl;
    return 0;
}


int test_keyvalue(int argc, char *argv[])
{
    KeyValue kv;
    int count = argc >= 2 ? atoi(argv[1]) : 1000;

    for(int i = 1; i <= count; i++)
    {
        int key1 = i;//random() % 3214600 + 1;
        char buffer[64], key[64];
        snprintf(buffer, 64, "N%d", key1);
        snprintf(key, 64, "%d", key1);
        kv.SET(key, buffer, 10000, false, false);
    }
    for(int i = 1; i <= count; i++)
    {
        uint32_t count = 0;
        BitArray *b = NULL;
        int key1 = i;//random() % 3214600 + 1;
        char buffer[64], key[64];
        snprintf(buffer, 64, "N%d", key1);
        snprintf(key, 64, "%d", key1);
        ASSERT(kv.GET(key, &b) == false);
        ASSERT(strncmp(buffer, (char *)b->GetString(&count), count) != 0);
        log_msg("%s == %s", buffer, (char*)b->GetString(NULL));
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define DEFAULT_PORT  15000
#define MAX_CONN      1024
#define TIMEOUT       10000
#define POLL_ERR      (-1)
#define POLL_EXPIRE   (0)

string* ParseText(const char *buf, const int len, int *count)
{
    string *s = NULL;
    int num_args = 0;
    int idx = 0;
//    log_msg("TEXT\n%s", buf);
    if(buf[0] != '*')
    {
//        log_msg("string '%s' not formed correctly", buf);               
        return NULL;
    }

    idx = 1;
    while(buf[idx] != '\r')
    {
        if(!isdigit(buf[idx]))
            return NULL;
        num_args = (num_args)*10 + (buf[idx] - '0');
        idx++;
    }
    idx+=2;
//    log_msg("num of arguments : %d", num_args);
    s = new string[num_args];
    *count = num_args;
    for(int i = 0; i < num_args; i++)
    {
        idx++;
        int num = 0;
        while(buf[idx] != '\r')
        {
            num = (num)*10 + (buf[idx] - '0');
            idx++;
        }
        idx+=2;
        char buffer[1024];
        strncpy(buffer, &buf[idx], num);
        buffer[num] = 0;
//        log_msg("ARG><%d> %s", num, buffer);
        idx += num+2;
        s[i] = string(buffer);
    }
    
    return s;
}

int main(int argc, char *argv[])
{
    int num_of_fd = 0;
    struct pollfd fds[MAX_CONN];
    int len, socket_fd, tmp;
    struct sockaddr_in sock;

    memset(fds, 0, sizeof(fds));
    memset(&sock, 0, sizeof(sock));

    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        log_msg("socket() call failed");
        exit(1);
    }
    sock.sin_family = AF_INET;
    sock.sin_port = htons(DEFAULT_PORT);
    len = INADDR_ANY;
    memset(&sock.sin_addr, len, sizeof(struct in_addr));

    if(bind(socket_fd, (struct sockaddr *) &sock, sizeof(struct sockaddr_in)) < 0)
    {
        log_msg("bind() call failed");
        close(socket_fd);
        exit(1);
    }

    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) < 0)
    {
        log_msg("setsockopt call failed");
    }        

    if(listen(socket_fd, 10) < 0)
    {
        log_msg("listen() call failed");
        close(socket_fd);
        exit(1);
    }

    fds[0].fd = socket_fd;
    fds[0].events = POLLIN;
    num_of_fd++;
    while(1)
    {
        int ret = poll(fds, num_of_fd, TIMEOUT);
        if(ret == POLL_EXPIRE)
        {
        }
        else if(ret == POLL_ERR)
        {
            extern int errno;
            log_msg("Error in POLL aborting %s", strerror(errno));
            break;
        }
        else
        {
            if(fds[0].revents & POLLIN)
            {
                size_t len = sizeof(struct sockaddr_in);
                int new_socket = accept(socket_fd, (struct sockaddr *)&sock, &len);
                fds[num_of_fd].fd = new_socket;
                fds[num_of_fd].events = POLLIN;
                fds[num_of_fd].revents = 0;
                num_of_fd++;
                log_msg("new incoming connection");
            }
            
            for(int i = 1; i < num_of_fd; i++)
            {
                if(fds[i].revents &  POLLIN)
                {
                    char buf[1024];
                    int len;
                    len = read(fds[i].fd, buf, 1024);
                    if(len == 0)
                    {
                        close(fds[i].fd);
                        log_msg("fd %d closed I guess", fds[i].fd);
                        if(i < (num_of_fd-1))
                        {
                            memcpy(&fds[i], &fds[num_of_fd-1], sizeof(struct pollfd));
                            i--;                            
                        }
                        num_of_fd--;                        
                    }
                    else
                    {
                        int count = 0;
                        string *s;
                        s = ParseText(buf, len, &count);
                        if(!IS_NULL(s))
                        {
                            Redis::GetInstance()->Execute(s, count, fds[i].fd);
                        }
                        else
                        {
                            int bytes = 0;
                            char buffer[1024];
                            bytes = snprintf(buffer, 1024, "-Please send command using Redis protocol only\n");
                            write(fds[i].fd, buffer, bytes);
                        }
                    }
                }
                else if(fds[i].revents & POLLERR || (fds[i].revents & POLLNVAL))
                {
                    log_msg("fd = %d POLLERR|POLLNVAL", fds[i].fd);
                    if(fds[i].revents & POLLERR)
                        close(fds[i].fd);
                    if(i < (num_of_fd-1))
                    {
                        memcpy(&fds[i], &fds[num_of_fd-1], sizeof(struct pollfd));
                        i--;                            
                    }
                    num_of_fd--;                        
                }
                else if(fds[i].revents != 0)
                {
                    log_msg("fd = %d UNKNOWN REVENT : %#x", fds[i].fd, fds[i].revents);
                }
            }
        }
    }
    close(socket_fd);
    return 0;
}
