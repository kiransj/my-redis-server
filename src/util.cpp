#include <stdlib.h>
#include <unistd.h>
#include "util.h"

static bool debug_flag = false;

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
        printf("%s\n", buffer);
    }
    return;
}

int send_msg(int socket_fd, const char *format, ...)
{
    int n, ret;
    va_list ap;
    char buffer[1024];
    va_start(ap, format);
    n = vsnprintf(buffer, 1024, format, ap);
    ret = write(socket_fd, buffer, n);
    va_end(ap);
    return ret;
}
