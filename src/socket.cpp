#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>

#include "util.h"

#define DEFAULT_PORT  15000
#define MAX_CONN      4096
#define TIMEOUT       10000
#define POLL_ERR      (-1)
#define POLL_EXPIRE   (0)

int start_server(void (*data_handler)(const char *buf, const int len, const int socket_fd), int port_number)
{
    int num_of_fd = 0;
    struct pollfd fds[MAX_CONN];
    int len, socket_fd, tmp = 0;
    struct sockaddr_in sock;

    memset(fds, 0, sizeof(fds));
    memset(&sock, 0, sizeof(sock));

    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        log_msg("socket() call failed");
        exit(1);
    }

    port_number = (0 == port_number) ? DEFAULT_PORT : port_number;
    sock.sin_family = AF_INET;
    sock.sin_port = htons(port_number);
    len = INADDR_ANY;
    memset(&sock.sin_addr, len, sizeof(struct in_addr));

    if(bind(socket_fd, (struct sockaddr *) &sock, sizeof(struct sockaddr_in)) < 0)
    {
        printf("bind() call failed\n");
        close(socket_fd);
        exit(1);
    }
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) < 0)
    {
        printf("setsockopt call failed\n");
    }
    if(listen(socket_fd, 10) < 0)
    {
        printf("listen() call failed\n");
        close(socket_fd);
        exit(1);
    }
    log_error("listening on port %d", port_number);
    fds[0].fd = socket_fd;
    fds[0].events = POLLIN;
    num_of_fd++;
    while(1)
    {
        int num_processed = 0;
        int ret = poll(fds, num_of_fd, TIMEOUT);
        if(ret == POLL_EXPIRE)
        {
        }
        else if(ret == POLL_ERR)
        {
            printf("Error in POLL aborting %s\n", strerror(errno));
            break;
        }
        else
        {
            if(fds[0].revents & POLLIN)
            {
                socklen_t len = sizeof(struct sockaddr_in);
                int new_socket = accept(socket_fd, (struct sockaddr *)&sock, &len);
                if(num_of_fd < MAX_CONN)
                {
                    fds[num_of_fd].fd = new_socket;
                    fds[num_of_fd].events = POLLIN;
                    fds[num_of_fd].revents = 0;
                    num_of_fd++;
                    log_msg("new incoming connection");
                }
                else
                {
                    log_msg("ignoring new connections as only 1024 simulatenous connections are supported");
                }
                num_processed++;
            }

            for(int i = 1; (i < num_of_fd) && (num_processed < ret); i++)
            {
                num_processed++;
                if(fds[i].revents &  POLLIN)
                {
                    char buf[1024];
                    int len;
                    len = read(fds[i].fd, buf, 1024);
                    if(len == 0)
                    {
                        close(fds[i].fd);
                        log_msg("fd %d closed I guess %d", fds[i].fd, num_of_fd);
                        if(i != (num_of_fd-1))
                        {
                            memcpy(&fds[i], &fds[num_of_fd-1], sizeof(struct pollfd));
                            i--;
                        }
                        num_of_fd--;
                    }
                    else
                    {
                        data_handler(buf, len, fds[i].fd);
                    }
                }
                else if(fds[i].revents & POLLERR || (fds[i].revents & POLLNVAL))
                {
                    log_msg("fd = %d POLLERR|POLLNVAL", fds[i].fd);
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
                else
                    num_processed--;
            }
        }
    }
    close(socket_fd);
    return 0;
}
