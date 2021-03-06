/**
 * @file    LogFileAppender.h
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2013/06/11
 * @version 1.0
 *
 * @brief
 */
#ifndef __LLBC_CORE_LOG_LOG_FILE_APPENDER_H__
#define __LLBC_CORE_LOG_LOG_FILE_APPENDER_H__

#include "llbc/common/Common.h"

#include "llbc/core/log/BaseLogAppender.h"

__LLBC_NS_BEGIN

/**
 * Pre-declare some classes.
 */
class LLBC_File;

__LLBC_NS_END

__LLBC_NS_BEGIN

/**
 * \brief File log appender class encapsulation.
 */
class LLBC_LogFileAppender : public LLBC_BaseLogAppender
{
    typedef LLBC_BaseLogAppender _Base;

public:
    LLBC_LogFileAppender();
    virtual ~LLBC_LogFileAppender();

public:
    /**
     * Get log appender type, see LLBC_LogAppenderType.
     * @return int - log appender type.
     */
    virtual int GetType() const;

public:
    /**
     * Initialize the log appender.
     * @param[in] initInfo - log appender initialize info structure.
     * @return int - return 0 if success, otherwise return -1.
     */
    virtual int Initialize(const LLBC_LogAppenderInitInfo &initInfo);

    /**
     * Finalize the appender.
     */
    virtual void Finalize();

    /**
     * Output log data.
     * @param[in] data - log data.
     * @return int - return 0 if success, otherwise return -1.
     */
    virtual int Output(const LLBC_LogData &data);

protected:
    /**
     * Flush method.
     */
    virtual void Flush();

private:
    /**
     * Check and update log file.
     * @param[in] now 
     */
    void CheckAndUpdateLogFile(sint64 now);

    /**
     * Build log file name.
     * @now                - now time.
     * @return LLBC_String - the log file name.
     */
    LLBC_String BuildLogFileName(sint64 now) const;

    /**
     * Check is need reopen file or not.
     * @param[in] now     - now time.
     * @param[out] clear  - the clear flag, if true, means when reopen file, must clear file content.
     * @param[out] backup - the backup flag, if true, means must backup log files.
     *                      about backup, see BackupFiles() method.
     * @return bool - need reopen if true, otherwise return false.
     */
    bool IsNeedReOpenFile(sint64 now, const LLBC_String &newFileName, bool &clear, bool &backup) const;

    /**
     * ReOpen the log file.
      * @param[in] newFileName - the new log file name.
      * @param[in] clear       - clear flag.
     * @return int - return 0 if success, otherwise return -1.
     */
    int ReOpenFile(const LLBC_String &newFileName, bool clear);

    /**
     * Backup all log files.
     */
    void BackupFiles() const;

    /**
     * Update log file buffer info(included buffer mode and buffer size).
     */
    void UpdateFileBufferInfo();

    /**
     * Get backup files count, if failed, return -1.
     */
    int GetBackupFilesCount(const LLBC_String &logFileName) const;

private:
    LLBC_String _baseName;
    LLBC_String _basePath;

    int _fileBufferSize;
    bool _isDailyRolling;

    long _maxFileSize;
    int _maxBackupIndex;

private:
    LLBC_String _fileName;

    LLBC_File *_file;
    long _fileSize;

    sint64 _nonFlushLogCount;
    sint64 _logfileLastCheckTime;
};

__LLBC_NS_END

#endif // !__LLBC_CORE_LOG_LOG_FILE_APPENDER_H__
