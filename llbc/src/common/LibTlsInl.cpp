/**
 * @file    LibTlsInl.cpp
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2013/08/19
 * @version 1.0
 *
 * @brief
 */

#include "llbc/common/Export.h"
#include "llbc/common/BeforeIncl.h"

#include "llbc/common/OSHeader.h"
#include "llbc/common/Macro.h"
#include "llbc/common/BasicDataType.h"
#include "llbc/common/Errno.h"

#include "llbc/common/LibTlsInl.h"

__LLBC_INTERNAL_NS_BEGIN
static LLBC_NS LLBC_TlsHandle __LLBC_libTlsHandle = LLBC_INVALID_TLS_HANDLE;
__LLBC_INTERNAL_NS_END

__LLBC_NS_BEGIN

__LLBC_LibTls::__LLBC_LibTls()
{
    commonTls.errNo = LLBC_ERROR_SUCCESS;
    commonTls.subErrNo = LLBC_ERROR_SUCCESS;
    ::memset(&commonTls.errDesc, 0, __LLBC_ERROR_DESC_SIZE);

    coreTls.llbcThread = false;
    coreTls.entryThread = false;
    coreTls.threadHandle = LLBC_INVALID_HANDLE;
    coreTls.nativeThreadHandle =LLBC_INVALID_NATIVE_THREAD_HANDLE;
    coreTls.task = NULL;
    coreTls.timerScheduler = NULL;

    objbaseTls.poolStack = NULL;

    ::memset(commTls.services, 0, sizeof(commTls.services));
}

void __LLBC_CreateLibTls()
{
    LLBC_TlsHandle &tlsHandle = LLBC_INTERNAL_NS __LLBC_libTlsHandle;

    bool tlsCreated = false;
#if LLBC_TARGET_PLATFORM_NON_WIN32
    tlsCreated = (pthread_key_create(&tlsHandle, NULL) == 0) ? true : false;
#else
    tlsCreated = ((tlsHandle = ::TlsAlloc()) != TLS_OUT_OF_INDEXES) ? true : false;
#endif

    if (!tlsCreated)
    {
        ASSERT(!tlsCreated && "Create llbc library TLS handle failed!");
        exit(-1);
    }
}

void __LLBC_DestroyLibTls()
{
    LLBC_TlsHandle &tlsHandle = LLBC_INTERNAL_NS __LLBC_libTlsHandle;

#if LLBC_TARGET_PLATFORM_NON_WIN32
    (void)pthread_key_delete(tlsHandle);
#else
    (void)::TlsFree(tlsHandle);
#endif

    tlsHandle = LLBC_INVALID_TLS_HANDLE;
}

__LLBC_LibTls *__LLBC_GetLibTls()
{
    LLBC_TlsHandle &tlsHandle = LLBC_INTERNAL_NS __LLBC_libTlsHandle;
    if (UNLIKELY(tlsHandle == LLBC_INVALID_TLS_HANDLE))
        return NULL;

    __LLBC_LibTls *libTls = NULL;
#if LLBC_TARGET_PLATFORM_NON_WIN32
    libTls = reinterpret_cast<__LLBC_LibTls *>(pthread_getspecific(tlsHandle));
#else
    libTls = reinterpret_cast<__LLBC_LibTls *>(::TlsGetValue(tlsHandle));
#endif

    if (UNLIKELY(!libTls))
    {
        libTls = new __LLBC_LibTls;

#if LLBC_TARGET_PLATFORM_NON_WIN32
        (void)pthread_setspecific(tlsHandle, libTls);
#else
        (void)::TlsSetValue(tlsHandle, libTls);
#endif
    }

    return libTls;
}

void __LLBC_ResetLibTls()
{
    LLBC_TlsHandle &tlsHandle = LLBC_INTERNAL_NS __LLBC_libTlsHandle;

    __LLBC_LibTls *libTls = NULL;
#if LLBC_TARGET_PLATFORM_NON_WIN32
    if ((libTls = reinterpret_cast<__LLBC_LibTls *>(
        pthread_getspecific(tlsHandle))))
    {
        delete libTls;
        pthread_setspecific(tlsHandle, NULL);
    }
#else
    if ((libTls = reinterpret_cast<__LLBC_LibTls *>(
        ::TlsGetValue(tlsHandle))))
    {
        delete libTls;
        ::TlsSetValue(tlsHandle, NULL);
    }
#endif
}

__LLBC_NS_END

#include "llbc/common/AfterIncl.h"
