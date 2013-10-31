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
//    zl.check();
    if(argc == 4)
    {
        int min = atoi(argv[2]), max = atoi(argv[3]), ret = 0;
        cout<<endl<<endl<<"ZRange("<<min<<", "<<max<<")"<<endl;
        ret = zl.ZRANGE(min, max, false)+1;
        if(ret > 0)
        {
            char *str;
            int key = 0, count = 0;
            while(zl.GetNext(&key, &str, &count) && (ret > 0))
            {
                str[count] = 0;
                cout<<++count<<"> "<<key<<" = "<<str<<endl;
                --ret;
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


string* ParseRedixProtocol(const char *buf, const int len, int *count)
{
    string *s = NULL;
    int num_args = 0;
    int idx = 0;
    log_msg("TEXT\n%s", buf);
    if(buf[0] != '*')
    {
        log_msg("string '%s' not formed correctly", buf);               
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
    log_msg("num of arguments : %d", num_args);
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
        s[i] = string(&buf[idx], num);
        idx += num+2;        
    }
    
    return s;
}

void DataHandler(const char *buf, const int len, const int socket_fd)
{
    string *s;
    int count = 0;    
    s = ParseRedixProtocol(buf, len, &count);
    if(!IS_NULL(s))
    {
        Redis::GetInstance()->Execute(s, count, socket_fd);
        delete[] s;
    }
    else
    {
        send_msg(socket_fd,"-Err Please send command using Redis protocol only\n");
    }
}

#include <signal.h>


static char *filename;
void my_handler(int s)
{
    Redis::GetInstance()->Save(filename);
    exit(1);
}
int start_server(void (*data_handler)(const char *buf, const int len, const int socket_fd));
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        log_error("usage %s <filename>", argv[0]);
    }
    filename = argv[1];
    Redis::GetInstance()->Load(filename);
    signal(SIGINT, my_handler);
    signal(SIGPIPE, SIG_IGN);
    start_server(DataHandler);
}

