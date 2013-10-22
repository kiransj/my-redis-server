#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdarg.h>

#define IS_NULL(x) ((x) == NULL)
#define MAX(x, y)  ((x > y) ? (x) : (y))

void log_msg(const char *format, ...);
void* Malloc(const size_t size);
void Free(void *);
#endif
