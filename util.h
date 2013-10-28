#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdarg.h>

#define IS_NULL(x) ((x) == NULL)
#define MAX(x, y)  ((x > y) ? (x) : (y))

#define ASSERT(x)   do { if(x) { log_msg("%s:%d abort\n", __FILE__, __LINE__);  abort(); }}while(0)

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
void log_msg(const char *format, ...);
#endif
