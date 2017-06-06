#ifndef ERROR_H
#define ERROR_H

#include <assert.h>
#include <winerror.h>

#define OK(command) assert(SUCCEEDED(command))

#endif
