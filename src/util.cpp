#include <stdlib.h>
#include <unistd.h>
#include "util.h"

static bool debug_flag = false;
static int line_number = 0;

void enable_debug_msg(void) { debug_flag = true; }
void disable_debug_msg(void) { debug_flag = false; }
void log_msg(const char *format, ...)
{
    if(debug_flag)
    {
        char buffer[1024];
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, 1024, format, ap);
        printf("%d> %s\n", ++line_number, buffer);
    }
    return;
}

void log_error(const char *format, ...)
{
    char buffer[1024];
    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, 1024, format, ap);
    fprintf(stderr, "%d> %s\n", ++line_number, buffer);
    return;
}

int send_binary(int socket_fd, char *buf, int length)
{
    return write(socket_fd, buf, length);
}
int send_msg(int socket_fd, const char *format, ...)
{
    int n, ret;
    va_list ap;
    char buffer[4096];
    va_start(ap, format);
    n = vsnprintf(buffer, 4906, format, ap);
    if(n > 4096)
    {
        char *buf = (char*)malloc(n+1);
        n = vsnprintf(buf, n+1, format, ap);
        ret = write(socket_fd, buf, n);
        log_msg(buf);
        free(buf);
    }
    else
    {
        ret = write(socket_fd, buffer, n);
        log_msg(buffer);
    }
    va_end(ap);
    return ret;
}
