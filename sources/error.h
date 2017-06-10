#ifndef ERROR_H
#define ERROR_H

#include <assert.h>
#include <winerror.h>

#define OK(command) (SUCCEEDED(command) ? 0 : assert(0), 0)

#endif
