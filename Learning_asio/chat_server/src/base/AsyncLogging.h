#pragma once

#include <cstdio>
#include <string>
#include <list>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <cstring>

#define LOG_API

enum LOG_LEVEL
{
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,    //����ҵ�����
    LOG_LEVEL_SYSERROR, //���ڼ�����ܱ���Ĵ���
    LOG_LEVEL_FATAL,    //FATAL �������־�����ڳ��������־���˳�
    LOG_LEVEL_CRITICAL  //CRITICAL ��־������־������ƣ��������
};

#define LOG_TRACE(...)     CAsyncLog::output(LOG_LEVEL_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...)     CAsyncLog::output(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)      CAsyncLog::output(LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...)   CAsyncLog::output(LOG_LEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)     CAsyncLog::output(LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_SYSERROR(...)  CAsyncLog::output(LOG_LEVEL_SYSERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...)     CAsyncLog::output(LOG_LEVEL_FATAL, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_CRITICAL(...)  CAsyncLog::output(LOG_LEVEL_CRITICAL, __FILE__, __LINE__, __VA_ARGS__)

#define LOG_DEBUG_BIN(buf, buflength) CAsyncLog::outputBinary(buf, buflength)

class LOG_API CAsyncLog
{
public:
    static bool init(const char* pszLogFileName = nullptr, bool bTruncateLongLine = false, int64_t m_nFileRollSize = 10 * 1024 * 1024);
    static void uninit();

    static void setLevel(LOG_LEVEL nLevel);
    static bool isRunning();

    //������߳�ID�ͺ���ǩ�����к�
    static bool output(long nLevel, const char* pszFmt, ...);
    //����߳�ID�ͺ���ǩ�����к�
    static bool output(long nLevel, const char* pszFileName, int nLineNo, const char* pszFmt, ...);

    static bool outputBinary(unsigned char* buffer, size_t);
private:
    CAsyncLog() = delete;
    ~CAsyncLog() = delete;

    CAsyncLog(const CAsyncLog& rhs) = delete;
    CAsyncLog& operator=(const CAsyncLog& rhs) = delete;

    static void makeLinePrefix(long nLevel, std::string& strPrefix);
    static void getTime(char* pszTime, int nTimeStrLength);
    static bool createNewFile(const char* pzsLogFileName);
    static bool writeToFile(const std::string& data);

    static void crash();

    static const char* ullto4Str(int n);
    static char* formLog(int& index, char* szbuf, size_t size_buf, unsigned char* buffer, size_t size);

    static void writeThreadProc();
private:
    static bool                          m_bToFile;                   //��־д�����̨�����ļ�
    static FILE* m_hLogFile;
    static std::string                   m_strFileName;
    //static std::string                   m_strFileNamePID;            //�ļ����еĽ���ID
    static bool                          m_bTruncateLongLog;          //����־�Ƿ�ض�
    static LOG_LEVEL                     m_nCurrentLevel;
    static int64_t                       m_nFileRollSize;             //�����ļ�������ֽ���
    static int64_t                       m_nCurrentWrittenSize;       //�Ѿ�д����ֽ���Ŀ
    static std::list<std::string>        m_listLinesToWrite;          //��д����־�洢��
    static std::unique_ptr<std::thread>  m_spWriteThread;
    static std::mutex                    m_mutexWrite;
    static std::condition_variable       m_cvWrite;
    static bool                          m_bExit;
    static bool                          m_bRunning;
};