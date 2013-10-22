#include <stdlib.h>
#include "util.h"

void log_msg(const char *format, ...)
{
    char buffer[1024];
    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, 1024, format, ap);
    printf("%s\n", buffer);
    return;
}
