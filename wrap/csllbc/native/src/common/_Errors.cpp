/**
 * @file    _Errors.cpp
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2016/01/25
 * @version 1.0
 *
 * @brief
 */

#include "csllbc/common/Export.h"

#include "csllbc/common/_Errors.h"

LLBC_BEGIN_C_DECL

const char *csllbc_FormatLastError(int *lastErrorLen)
{
    const char *lastError = LLBC_FormatLastError();
    *lastErrorLen = LLBC_StrLenA(lastError);

    return lastError;
}

const char *csllbc_StrError(unsigned int errNo, int *errLen)
{
    const char *error = LLBC_StrError((int)errNo);
    *errLen = LLBC_StrLenA(error);

    return error;
}

LLBC_END_C_DECL
