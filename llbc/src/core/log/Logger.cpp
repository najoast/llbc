/**
 * @file    Logger.cpp
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2013/06/11
 * @version 1.0
 *
 * @brief
 */

#include "llbc/common/Export.h"
#include "llbc/common/BeforeIncl.h"

#include "llbc/core/utils/Util_Text.h"

#include "llbc/core/os/OS_Time.h"

#include "llbc/core/thread/Guard.h"
#include "llbc/core/thread/MessageBlock.h"

#include "llbc/core/log/LogLevel.h"
#include "llbc/core/log/LogData.h"
#include "llbc/core/log/LoggerConfigInfo.h"
#include "llbc/core/log/ILogAppender.h"
#include "llbc/core/log/LogAppenderBuilder.h"
#include "llbc/core/log/LogRunnable.h"
#include "llbc/core/log/Logger.h"

#if LLBC_TARGET_PLATFORM_WIN32
#pragma warning(disable:4996)
#endif

__LLBC_INTERNAL_NS_BEGIN

static const LLBC_NS LLBC_String __g_invalidLoggerName;

__LLBC_INTERNAL_NS_END

__LLBC_NS_BEGIN

LLBC_Logger::LLBC_Logger()
: _mutex()
, _name()
, _logLevel(LLBC_LogLevel::Debug)
, _config(NULL)
, _logRunnable(NULL)
{
}

LLBC_Logger::~LLBC_Logger()
{
    Finalize();
}

int LLBC_Logger::Initialize(const LLBC_String &name, const LLBC_LoggerConfigInfo *config)
{
    if (name.empty() || !config)
    {
        LLBC_SetLastError(LLBC_ERROR_ARG);
        return LLBC_FAILED;
    }
    else if (IsInit())
    {
        LLBC_SetLastError(LLBC_ERROR_REENTRY);
        return LLBC_FAILED;
    }

    LLBC_Guard guard(_mutex);
    _name.append(name);

    _config = config;

    _logLevel = MIN(_config->GetConsoleLogLevel(), _config->GetFileLogLevel());

    _logRunnable = new LLBC_LogRunnable;
    _logRunnable->SetFlushInterval(_config->GetFlushInterval());

    if (_config->IsLogToConsole())
    {
        LLBC_LogAppenderInitInfo appenderInitInfo;
        appenderInitInfo.level = _config->GetConsoleLogLevel();
        appenderInitInfo.pattern = _config->GetConsolePattern();
        appenderInitInfo.colourfulOutput = _config->IsColourfulOutput();

        LLBC_ILogAppender *appender = 
            LLBC_LogAppenderBuilderSingleton->BuildAppender(LLBC_LogAppenderType::Console);
        if (appender->Initialize(appenderInitInfo) != LLBC_OK)
        {
            LLBC_XDelete(appender);
            return LLBC_FAILED;
        }

        _logRunnable->AddAppender(appender);
    }

    if (_config->IsLogToFile())
    {
        LLBC_LogAppenderInitInfo appenderInitInfo;
        appenderInitInfo.level = _config->GetFileLogLevel();
        appenderInitInfo.pattern = _config->GetFilePattern();
        appenderInitInfo.file = _config->GetLogFile();
        appenderInitInfo.forceAppLogPath = _config->GetForceAppLogPath();
        appenderInitInfo.dailyRolling = _config->IsDailyRollingMode();
        appenderInitInfo.maxFileSize = _config->GetMaxFileSize();
        appenderInitInfo.maxBackupIndex = _config->GetMaxBackupIndex();

        if (!_config->IsAsyncMode())
            appenderInitInfo.fileBufferSize = 0;
        else
            appenderInitInfo.fileBufferSize = _config->GetFileBufferSize();

        LLBC_ILogAppender *appender =
            LLBC_LogAppenderBuilderSingleton->BuildAppender(LLBC_LogAppenderType::File);
        if (appender->Initialize(appenderInitInfo) != LLBC_OK)
        {
            LLBC_XDelete(appender);
            return LLBC_FAILED;
        }

        _logRunnable->AddAppender(appender);
    }

    if (_config->IsAsyncMode())
        _logRunnable->Activate(1);

    return LLBC_OK;
}

bool LLBC_Logger::IsInit() const
{
    LLBC_Logger *nonConstThis = const_cast<LLBC_Logger *>(this);
    LLBC_Guard guard(nonConstThis->_mutex);

    return (_logRunnable ? true : false);
}

void LLBC_Logger::Finalize()
{
    LLBC_Guard guard(_mutex);
    if (!_logRunnable)
        return;

    if (_config->IsAsyncMode())
    {
        _logRunnable->Stop();
        _logRunnable->Wait();
    }
    else
        _logRunnable->Cleanup();

    LLBC_XDelete(_logRunnable);

    _name.clear();
    _config = NULL;
}

const LLBC_String &LLBC_Logger::GetLoggerName() const
{
    LLBC_Logger *nonConstThis = const_cast<LLBC_Logger *>(this);
    LLBC_Guard guard(nonConstThis->_mutex);
    if (!_logRunnable)
    {
        LLBC_SetLastError(LLBC_ERROR_NOT_INIT);
        return LLBC_INTERNAL_NS __g_invalidLoggerName;
    }

    LLBC_SetLastError(LLBC_ERROR_SUCCESS);
    return _name;
}

int LLBC_Logger::GetLogLevel() const
{
    LLBC_Logger *nonConstThis = const_cast<LLBC_Logger *>(this);
    LLBC_Guard guard(nonConstThis->_mutex);
    if (!_logRunnable)
    {
        LLBC_SetLastError(LLBC_ERROR_NOT_INIT);
        return LLBC_LogLevel::End;
    }

    return _logLevel;
}

void LLBC_Logger::SetLogLevel(int level)
{
    LLBC_Guard guard(_mutex);

    _logLevel = MIN(MAX(LLBC_LogLevel::Begin, level), LLBC_LogLevel::End - 1);
}

bool LLBC_Logger::IsTakeOver() const
{
    return _config->IsTakeOver();
}

int LLBC_Logger::Debug(const char *tag, const char *file, int line, const char *message, ...)
{
    if (LLBC_LogLevel::Debug < _logLevel)
        return LLBC_OK;

    char *fmttedMsg; int msgLen;
    LLBC_FormatArg(message, fmttedMsg, msgLen);

    return DirectOutput(LLBC_LogLevel::Debug, tag, file, line, fmttedMsg, msgLen);
}

int LLBC_Logger::Info(const char *tag, const char *file, int line, const char *message, ...)
{
    if (LLBC_LogLevel::Info < _logLevel)
        return LLBC_OK;

    char *fmttedMsg; int msgLen;
    LLBC_FormatArg(message, fmttedMsg, msgLen);

    return DirectOutput(LLBC_LogLevel::Info, tag, file, line, fmttedMsg, msgLen);
}

int LLBC_Logger::Warn(const char *tag, const char *file, int line, const char *message, ...)
{
    if (LLBC_LogLevel::Warn < _logLevel)
        return LLBC_OK;

    char *fmttedMsg; int msgLen;
    LLBC_FormatArg(message, fmttedMsg, msgLen);

    return DirectOutput(LLBC_LogLevel::Warn, tag, file, line, fmttedMsg, msgLen);
}

int LLBC_Logger::Error(const char *tag, const char *file, int line, const char *message, ...)
{
    if (LLBC_LogLevel::Error < _logLevel)
        return LLBC_OK;

    char *fmttedMsg; int msgLen;
    LLBC_FormatArg(message, fmttedMsg, msgLen);

    return DirectOutput(LLBC_LogLevel::Error, tag, file, line, fmttedMsg, msgLen);
}

int LLBC_Logger::Fatal(const char *tag, const char *file, int line, const char *message, ...)
{
    if (LLBC_LogLevel::Fatal < _logLevel)
        return LLBC_OK;

    char *fmttedMsg; int msgLen;
    LLBC_FormatArg(message, fmttedMsg, msgLen);

    return DirectOutput(LLBC_LogLevel::Fatal, tag, file, line, fmttedMsg, msgLen);
}

int LLBC_Logger::Output(int level, const char *tag, const char *file, int line, const char *message, ...) 
{
    if (level < _logLevel)
        return LLBC_OK;

    char *fmttedMsg; int msgLen;
    LLBC_FormatArg(message, fmttedMsg, msgLen);

    return DirectOutput(level, tag, file, line, fmttedMsg, msgLen);
}

int LLBC_Logger::OutputNonFormat(int level, const char *tag, const char *file, int line, const char *message, size_t messageLen)
{
    if (level < _logLevel)
        return LLBC_OK;

    if (UNLIKELY(message == NULL))
        return DirectOutput(level, tag, file, line, NULL, 0);

    if (messageLen < 0)
        messageLen = LLBC_StrLenA(message);

    char *copyMessage = LLBC_Malloc(char, messageLen + 1);
    LLBC_MemCpy(copyMessage, message, messageLen);
    copyMessage[messageLen] = '\0';

    return DirectOutput(level, tag, file, line, copyMessage, static_cast<int>(messageLen));
}

int LLBC_Logger::DirectOutput(int level, const char *tag, const char *file, int line, char *message, int len) 
{
    LLBC_LogData *data = BuildLogData(level, tag, file, line, message, len);
    if (!_config->IsAsyncMode())
    {
        const int ret = _logRunnable->Output(data);
        LLBC_LogRunnable::FreeLogData(data);

        return ret;
    }

    LLBC_MessageBlock *block = new LLBC_MessageBlock;
    block->Write(&data, sizeof(LLBC_LogData));

    _logRunnable->Push(block);

    return LLBC_OK;
}

LLBC_LogData *LLBC_Logger::BuildLogData(int level,
                                        const char *tag,
                                        const char *file,
                                        int line,
                                        char *message,
                                        int len)
{
    LLBC_LogData *data = new LLBC_LogData;
    
    data->level = level;
    data->loggerName = _name.c_str();

    data->tagLen = tag ? LLBC_StrLenA(tag) : 0;
    data->fileLen = file ? LLBC_StrLenA(file) : 0;

    data->tagBeg = 0;
    data->fileBeg = data->tagLen;

    const uint32 othersSize = data->tagLen + data->fileLen;
    if (othersSize == 0)
    {
        data->others = NULL;
    }
    else
    {
        data->others = LLBC_Malloc(char, othersSize);
        if (tag)
            memcpy(data->others + data->tagBeg, tag, data->tagLen);
        if (file)
            memcpy(data->others + data->fileBeg, file, data->fileLen);
    }

    data->logTime = LLBC_GetMilliSeconds();

    data->line = line;

    data->msg = message;
    data->msgLen = len;

    __LLBC_LibTls *tls = __LLBC_GetLibTls();
    data->threadHandle = tls->coreTls.nativeThreadHandle;

    return data;
}

__LLBC_NS_END

#if LLBC_TARGET_PLATFORM_WIN32
#pragma warning(default:4996)
#endif

#include "llbc/common/AfterIncl.h"
