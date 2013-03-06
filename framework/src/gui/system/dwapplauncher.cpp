#include "stable.h"
#include "dwappLauncher.h"

//------------------------------------------------------------------------------
/**
*/
DwAppLauncher::DwAppLauncher() :
    m_noConsoleWindow(false),
    m_isRunning(false),
    m_lauchedProcessInfo(PROCESS_INFORMATION()),
    m_stdoutAsyncRead(0),
    m_stdoutAsyncWrite(0),
    m_stdoutCaptureStream(NULL),
    m_stderrCaptureStream(NULL)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwAppLauncher::~DwAppLauncher()
{
    if (m_stdoutCaptureStream && m_stdoutCaptureStream->parent() == NULL)
    {
        delete m_stdoutCaptureStream;
    }

    if (m_stderrCaptureStream && m_stderrCaptureStream->parent() == NULL)
    {
        delete m_stderrCaptureStream;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
DwAppLauncher::launchWait() const
{
    DW_ASSERT(m_exePath.isValid());

    DWORD creationFlags = 0;
    if (m_noConsoleWindow)
    {
        creationFlags |= CREATE_NO_WINDOW;
    }

    // build a command line
    DwString cmdLine = m_exePath.localPath();
    cmdLine.append(" ");
    cmdLine.append(m_args);

    if(m_stdoutCaptureStream || m_stderrCaptureStream )
    {
        return launchWithOutputCapturing(creationFlags, cmdLine);
    }
    else
    {
        return launchWithoutOutputCapturing(creationFlags, cmdLine);
    }
}

//------------------------------------------------------------------------------
/**
    Launch the application process and returns immediately. The state of the launched
    process can be checked by calling IsRunning().
*/
bool
DwAppLauncher::launch()
{
    DW_ASSERT(m_exePath.isValid());

    DWORD creationFlags = 0;
    if (m_noConsoleWindow)
    {
        creationFlags |= CREATE_NO_WINDOW;
    }

    // build a command line
    DwString cmdLine = m_exePath.localPath();
    cmdLine.append(" ");
    cmdLine.append(m_args);


    STARTUPINFO startupInfo = { sizeof(STARTUPINFO), 0 };

    // setup some additional startup info for stdout / stderr streaming
    if(m_stdoutCaptureStream || m_stderrCaptureStream )
    {
        startupInfo.dwFlags |= STARTF_USESTDHANDLES;
        startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        if(m_stdoutCaptureStream)
        {
            if(!createStdoutCapturePipe((PHANDLE)&m_stdoutAsyncRead, (PHANDLE)&m_stdoutAsyncWrite))
            {
                return false;
            }
            startupInfo.hStdOutput = m_stdoutAsyncWrite;
        }
        else
        {
            startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        }
        if(m_stderrCaptureStream)
        {
            if(!createStdoutCapturePipe((PHANDLE)&m_stderrAsyncRead, (PHANDLE)&m_stderrAsyncWrite))
            {
                return false;
            }
            startupInfo.hStdError = m_stderrAsyncWrite;
        }
        else
        {
            startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
        }
    }

    // create the process, return false if this fails
    if (!CreateProcess(NULL,                        // lpApplicationName
                       cmdLine.constString(),                // lpCommandLine
                       NULL,                                       // lpProcessAttributes
                       NULL,                                       // lpThreadAttributes
                       TRUE,                                       // bInheritsHandle
                       creationFlags,                              // dwCreationFlags
                       NULL,                                       // lpEnvironment
                       m_workingDir.localPath().constString(),   // lpCurrentDirectory
                       &startupInfo,                               // lpStartupInfo
                       (LPPROCESS_INFORMATION)&(m_lauchedProcessInfo))) // lpProcessInformation
    {
        if(m_stdoutCaptureStream)
        {
            CloseHandle(m_stdoutAsyncRead);
            CloseHandle(m_stdoutAsyncWrite);
        }
        if(m_stderrCaptureStream)
        {
            CloseHandle(m_stderrAsyncRead);
            CloseHandle(m_stderrAsyncWrite);
        }
        return false;
    }

    if (m_stdoutCaptureStream)
    {
        bool openDevice = m_stdoutCaptureStream->open(DwIODevice::ReadWrite);
        if (!openDevice)
        {
            CloseHandle(m_stdoutAsyncRead);
            CloseHandle(m_stdoutAsyncWrite);
        }
    }

    if (m_stderrCaptureStream)
    {
        bool openDevice = m_stderrCaptureStream->open(DwIODevice::ReadWrite);
        if (!openDevice)
        {
            CloseHandle(m_stderrAsyncRead);
            CloseHandle(m_stderrAsyncWrite);
        }
    }

    m_isRunning = true;
    return true;
}

//------------------------------------------------------------------------------
/**
    Gets the state of the application.
*/
bool
DwAppLauncher::isRunning()
{
    if(m_isRunning)
    {
        DWORD exitCode = 0;
        GetExitCodeProcess(m_lauchedProcessInfo.hProcess, &exitCode);
        if (exitCode != STILL_ACTIVE)
        {
            updateStdoutStream();
            if(m_stdoutCaptureStream)
            {
                CloseHandle(m_stdoutAsyncRead);
                CloseHandle(m_stdoutAsyncWrite);
            }
            if(m_stderrCaptureStream)
            {
                CloseHandle(m_stderrAsyncRead);
                CloseHandle(m_stderrAsyncWrite);
            }
            CloseHandle(m_lauchedProcessInfo.hProcess);
            CloseHandle(m_lauchedProcessInfo.hThread);

            m_isRunning = false;
        }
        updateStdoutStream();
    }

    return m_isRunning;
}
//------------------------------------------------------------------------------
/**
    Reads all arrived data from stdout since the last call of this method and puts it to
    the stream.
*/
void
DwAppLauncher::updateStdoutStream()
{
    if(m_stdoutCaptureStream)
    {
        DW_ASSERT(0 != m_stdoutAsyncRead);
        DW_ASSERT(0 != m_lauchedProcessInfo.hProcess);

        m_numAsyncBytesRead = 0;
        m_numAsyncBytesAvailable = 0;
        memset(m_asyncBuffer, 0, asyncBufferSize);

        // peek to see if there is any data to read
        PeekNamedPipe(m_stdoutAsyncRead, m_asyncBuffer, asyncBufferSize, &m_numAsyncBytesRead, &m_numAsyncBytesAvailable, NULL);

        if (m_numAsyncBytesRead != 0)
        {
            if (m_numAsyncBytesAvailable > asyncBufferSize)
            {
                while (m_numAsyncBytesRead >= asyncBufferSize)
                {
                    ReadFile(m_stdoutAsyncRead, m_asyncBuffer, asyncBufferSize, &m_numAsyncBytesRead, NULL);
                    m_stdoutCaptureStream->write(m_asyncBuffer, m_numAsyncBytesRead);
                }
            }
            else
            {
                ReadFile(m_stdoutAsyncRead, m_asyncBuffer, asyncBufferSize, &m_numAsyncBytesRead, NULL);
                m_stdoutCaptureStream->write(m_asyncBuffer, m_numAsyncBytesRead);
            }
        }
    }
    if(m_stderrCaptureStream)
    {
        DW_ASSERT(0 != m_stderrAsyncRead);
        DW_ASSERT(0 != m_lauchedProcessInfo.hProcess);

        m_numAsyncBytesRead = 0;
        m_numAsyncBytesAvailable = 0;
        memset(m_asyncBuffer, 0, asyncBufferSize);

        // peek to see if there is any data to read
        PeekNamedPipe(m_stderrAsyncRead, m_asyncBuffer, asyncBufferSize, &m_numAsyncBytesRead, &m_numAsyncBytesAvailable, NULL);

        if (m_numAsyncBytesRead != 0)
        {
            if (m_numAsyncBytesAvailable > asyncBufferSize)
            {
                while (m_numAsyncBytesRead >= asyncBufferSize)
                {
                    ReadFile(m_stderrAsyncRead, m_asyncBuffer, asyncBufferSize, &m_numAsyncBytesRead, NULL);
                    m_stderrCaptureStream->write(m_asyncBuffer, m_numAsyncBytesRead);
                }
            }
            else
            {
                ReadFile(m_stderrAsyncRead, m_asyncBuffer, asyncBufferSize, &m_numAsyncBytesRead, NULL);
                m_stderrCaptureStream->write(m_asyncBuffer, m_numAsyncBytesRead);
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
DwAppLauncher::launchWithoutOutputCapturing(DWORD creationFlags, const DwString& cmdLine) const
{
    STARTUPINFO startupInfo = { sizeof(STARTUPINFO), 0 };
    PROCESS_INFORMATION processInfo = { 0 };

    if (!CreateProcess(NULL,                            // lpApplicationName
                       cmdLine.constString(),     // lpCommandLine
                       NULL,                            // lpProcessAttributes
                       NULL,                            // lpThreadAttributes
                       FALSE,                           // bInheritsHandle
                       creationFlags,                   // dwCreationFlags
                       NULL,                            // lpEnvironment
                       m_workingDir.localPath().constString(),    // lpCurrentDirectory
                       &startupInfo,                    // lpStartupInfo
                       &processInfo))                   // lpProcessInformation
    {
        return false;
    }

    // wait until process exits
    WaitForSingleObject(processInfo.hProcess, INFINITE);

    // cleanup
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
DwAppLauncher::launchWithOutputCapturing(DWORD creationFlags, const DwString& cmdLine) const
{
    HANDLE stdoutRead = 0;
    HANDLE stdoutWrite = 0;
    HANDLE stderrRead = 0;
    HANDLE stderrWrite = 0;

    // build startup info
    STARTUPINFO startupInfo = { sizeof(STARTUPINFO), 0 };
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;
    startupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    // create stdout output
    if(m_stdoutCaptureStream)
    {
        if(!createStdoutCapturePipe(&stdoutRead, &stdoutWrite))
        {
            return false;
        }
        startupInfo.hStdOutput = stdoutWrite;
    }
    else
    {
        startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    // create stderr output
    if(m_stderrCaptureStream)
    {
        if(!createStdoutCapturePipe(&stderrRead, &stderrWrite))
        {
            return false;
        }
        startupInfo.hStdError = stderrWrite;
    }
    else
    {
        startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    }

    // create the process
    PROCESS_INFORMATION processInfo = { 0 };

    if (!CreateProcess(NULL,                            // lpApplicationName
                       cmdLine.constString(),     // lpCommandLine
                       NULL,                            // lpProcessAttributes
                       NULL,                            // lpThreadAttributes
                       TRUE,                            // bInheritsHandle
                       creationFlags,                   // dwCreationFlags
                       NULL,                            // lpEnvironment
                       m_workingDir.localPath().constString(),    // lpCurrentDirectory
                       &startupInfo,                    // lpStartupInfo
                       &processInfo))                   // lpProcessInformation
    {
        // cleanup if failed
        if(m_stdoutCaptureStream)
        {
            CloseHandle(stdoutRead);
            CloseHandle(stdoutWrite);
        }
        if(m_stderrCaptureStream)
        {
            CloseHandle(stderrRead);
            CloseHandle(stderrWrite);
        }
        return false;
    }

    // try to open stdout stream
    if (m_stdoutCaptureStream)
    {
        bool openDevice = m_stdoutCaptureStream->open(DwIODevice::ReadWrite);
        if (!openDevice)
        {
            CloseHandle(m_stdoutAsyncRead);
            CloseHandle(m_stdoutAsyncWrite);
            return false;
        }
    }

    // try to open stderr stream
    if (m_stderrCaptureStream)
    {
        bool openDevice = m_stderrCaptureStream->open(DwIODevice::ReadWrite);
        if (!openDevice)
        {
            CloseHandle(m_stderrAsyncRead);
            CloseHandle(m_stderrAsyncWrite);
            return false;
        }
    }

    // capture output
    captureOutput(stdoutRead, stderrRead, processInfo.hProcess);

    // cleanup
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    if(m_stdoutCaptureStream)
    {
        m_stdoutCaptureStream->close();
        CloseHandle(stdoutRead);
        CloseHandle(stdoutWrite);
    }
    if(m_stderrCaptureStream)
    {
        m_stderrCaptureStream->close();
        CloseHandle(stderrRead);
        CloseHandle(stderrWrite);
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
DwAppLauncher::createStdoutCapturePipe(PHANDLE stdoutRead, PHANDLE stdoutWrite) const
{
    // configure pipe security attributes
    SECURITY_ATTRIBUTES security;
    security.bInheritHandle = TRUE;
    security.lpSecurityDescriptor = 0;
    security.nLength = sizeof(SECURITY_ATTRIBUTES);

    // create a pipe for the child process's stdout
    if (!CreatePipe(stdoutRead, stdoutWrite, &security, 4096))
    {
        return false;
    }

    // ensure the read handle to the pipe for stdout is not inherited.
    SetHandleInformation((*stdoutRead), HANDLE_FLAG_INHERIT, 0);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
DwAppLauncher::captureOutput(HANDLE stdoutRead, HANDLE stderrRead, HANDLE childProcess) const
{
    DW_ASSERT(0 != childProcess);

    DWORD numBytesRead = 0;
    DWORD numBytesAvailable = 0;
    DWORD exitCode = 0;
    const int bufferSize = 4096;
    char buffer[bufferSize];
    memset(buffer, 0, bufferSize);

    // read everything from read end of pipe and write to stream ...
    while(true)
    {
        if(m_stdoutCaptureStream)
        {
            DW_ASSERT(0 != stdoutRead);
            // peek to see if there is any data to read
            PeekNamedPipe(stdoutRead, buffer, bufferSize, &numBytesRead, &numBytesAvailable, NULL);

            if (numBytesRead != 0)
            {
                if (numBytesAvailable > bufferSize)
                {
                    while (numBytesRead >= bufferSize)
                    {
                        ReadFile(stdoutRead, buffer, bufferSize, &numBytesRead, NULL);
                        m_stdoutCaptureStream->write(buffer, numBytesRead);
                    }
                }
                else
                {
                    ReadFile(stdoutRead, buffer, bufferSize, &numBytesRead, NULL);
                    m_stdoutCaptureStream->write(buffer, numBytesRead);
                }
            }
        }
        if(m_stderrCaptureStream)
        {
            DW_ASSERT(0 != stderrRead);
            // peek to see if there is any data to read
            PeekNamedPipe(stderrRead, buffer, bufferSize, &numBytesRead, &numBytesAvailable, NULL);

            if (numBytesRead != 0)
            {
                if (numBytesAvailable > bufferSize)
                {
                    while (numBytesRead >= bufferSize)
                    {
                        ReadFile(stderrRead, buffer, bufferSize, &numBytesRead, NULL);
                        m_stderrCaptureStream->write(buffer, numBytesRead);
                    }
                }
                else
                {
                    ReadFile(stderrRead, buffer, bufferSize, &numBytesRead, NULL);
                    m_stderrCaptureStream->write(buffer, numBytesRead);
                }
            }
        }

        // ... while the child process is running
        GetExitCodeProcess(childProcess, &exitCode);
        if (exitCode != STILL_ACTIVE)
        {
            break;
        }
    }
}