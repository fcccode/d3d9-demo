#ifndef ERROR_H
#define ERROR_H

#include <d3d9.h>
#include <dinput.h>

void fatal(const char *file, int line, const char *message);

#define ASSERT_EQUAL(command, expected)                 \
    do {                                                \
        HRESULT result = (command);                     \
        if (result != expected) {                       \
            fatal(__FILE__, __LINE__, #command);        \
        }                                               \
    } while (false)

#define OK_DI(command) ASSERT_EQUAL(command, DI_OK)
#define OK_3D(command) ASSERT_EQUAL(command, D3D_OK)
#define OK_FX(command) ASSERT_EQUAL(command, S_OK)

#endif
