#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "redis.h"

#include <string>
using namespace std;


char *filename;

/* This function loops continously looking
 * for new incoming connections or data*/
int start_server(void (*data_handler)(const char *buf, const int len, const int socket_fd), int port_number);

/*Expects contents in buf to be in redix protocol format*/
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
    int i;
    for(i = 0; (i < num_args) && (idx < len); i++)
    {
        idx++;
        int num = 0;
        while((buf[idx] != '\r') && (idx < len))
        {
            num = (num)*10 + (buf[idx] - '0');
            idx++;
        }
        idx+=2;
        s[i] = string(&buf[idx], num);
        idx += num+2;
    }

    if(i != num_args)
    {
        log_error("Input is not formed correctly. Ignoring this command");
        delete[] s;
        return NULL;
    }
    return s;
}

/* The callback from socket loop*/
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
        send_msg(socket_fd,"-Err Please send command using Redis protocol only\r\n");
    }
}

/*Function that gets called when ctrl-c is pressed*/
void my_handler(int s)
{
    Redis::GetInstance()->Save(filename);
    exit(0);
}

int main(int argc, char *argv[])
{
    int port_number = 0;
    if(argc < 2 || argc > 3)
    {
        log_error("usage %s <filename> <port_number:default 16000>", argv[0]);
        exit(1);
    }
    filename = argv[1];
    if(argc == 3)
        port_number = atoi(argv[2]);

    /*Enable debug msg's*/
    //enable_debug_msg();

    /* load the database*/
    Redis::GetInstance()->Load(filename);
    /*Register our signal handlers*/
    signal(SIGINT, my_handler);
    signal(SIGPIPE, SIG_IGN);

    /*start the socket server.
     * DataHandler() is the callback function for the data received*/
    start_server(DataHandler, port_number);

    return 0;
}

