#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdarg.h>

#define IS_NULL(x) ((x) == NULL)
#define MAX(x, y)  ((x > y) ? (x) : (y))

typedef unsigned int uint32_t;
void log_msg(const char *format, ...);
#endif
