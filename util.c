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

void* Malloc(const size_t size)
{
    void *ptr = malloc(size);
    if(IS_NULL(ptr))
    {
        log_msg("Malloc(%u) failed", size);
    }
    return ptr;
}

void Free(void *ptr)
{
    free(ptr);
}
