#ifndef ERROR_H
#define ERROR_H

#include <winerror.h>

void fatal(const char *file, int line, const char *message);
void debug(const char *file, int line, const char *message);

#define FATAL(message) fatal(__FILE__, __LINE__, message)
#define DEBUG(message) debug(__FILE__, __LINE__, message)

#define OK(command)                                     \
    do {                                                \
        HRESULT result = (command);                     \
        if (FAILED(result)) {                           \
            fatal(__FILE__, __LINE__, #command);        \
        }                                               \
    } while (false)

#endif
