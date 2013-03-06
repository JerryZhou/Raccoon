#pragma once

#include "dwgui/dwurl.h"
#include "dwcore/dwdebug.h"
#include "dwcore/dwiodevice.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwAppLauncher
{
public:/// constructor
    DwAppLauncher();
    ~DwAppLauncher();

    /// set the executable path
    void setExecutable(const DwUrl& uri);
    /// set working directory
    void setWorkingDirectory(const DwUrl& uri);
    /// set command line arguments
    void setArguments(const DwString& args);
    /// do not open a console window for the new process
    void setNoConsoleWindow(bool b);
    /// set optional stdout capture stream
    void setStdoutCaptureStream(DwIODevice *device);
    /// get optional pointer to stdout capture stream
    const DwIODevice* stdoutCaptureStream() const;
    /// set optional stderr capture stream
    void setStderrCaptureStream(DwIODevice *device);
    /// get optional pointer to stderr capture stream
    const DwIODevice* stderrCaptureStream() const;

    /// launch application and wait for its termination
    bool launchWait() const;
    /// launch application (returns immediately)
    bool launch();
    /// Gets the state of application. Call this per frame.
    bool isRunning();
    /// Read data from the captured stdout and writes it to the stream.
    void updateStdoutStream();

private:
    /// launch without stdout capturing
    bool launchWithoutOutputCapturing(DWORD creationFlags, const DwString& cmdLine) const;
    /// launch with stdout capturing
    bool launchWithOutputCapturing(DWORD creationFlags, const DwString& cmdLine) const;
    /// create pipe for optional stdout capturing
    bool createStdoutCapturePipe(PHANDLE stdoutRead, PHANDLE stdoutWrite) const;
    /// capture data from output
    void captureOutput(HANDLE stdoutRead, HANDLE stderrRead, HANDLE childProcess) const;

    bool m_isRunning;
    bool m_noConsoleWindow;
    DwUrl m_exePath;
    DwUrl m_workingDir;
    DwString m_args;

    DwIODevice* m_stdoutCaptureStream;
    DwIODevice* m_stderrCaptureStream;

    PROCESS_INFORMATION m_lauchedProcessInfo;
    HANDLE m_stdoutAsyncRead;
    HANDLE m_stdoutAsyncWrite;
    HANDLE m_stderrAsyncRead;
    HANDLE m_stderrAsyncWrite;

    DWORD m_numAsyncBytesRead;
    DWORD m_numAsyncBytesAvailable;
    static const int asyncBufferSize = 4096;
    char m_asyncBuffer[asyncBufferSize];
};// end of

//------------------------------------------------------------------------------
/**
*/
inline void
DwAppLauncher::setExecutable(const DwUrl& uri)
{
    m_exePath = uri;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwAppLauncher::setWorkingDirectory(const DwUrl& uri)
{
    m_workingDir = uri;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwAppLauncher::setArguments(const DwString& a)
{
    m_args = a;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwAppLauncher::setNoConsoleWindow(bool b)
{
    m_noConsoleWindow = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwAppLauncher::setStdoutCaptureStream(DwIODevice *device)
{
    m_stdoutCaptureStream = device;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwIODevice *
DwAppLauncher::stdoutCaptureStream() const
{
    return m_stdoutCaptureStream;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwAppLauncher::setStderrCaptureStream(DwIODevice *device)
{
    m_stderrCaptureStream = device;
}

//------------------------------------------------------------------------------
/**
*/
inline const DwIODevice *
DwAppLauncher::stderrCaptureStream() const
{
    return m_stderrCaptureStream;
}