#pragma once

#include "dwapp/dwrefcounted.h"
#include "dwapp/dwptr.h"
#include "dwapp/dwstream.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwStreamReader : public DwRefCounted
{
    DW_DECLARE_CLASS(DwStreamReader);
public:
    /// constructor
    DwStreamReader();
    /// destructor
    virtual ~DwStreamReader();
    /// set stream to read from
    void setStream(const DwPtr<DwStream>& s);
    /// get currently set stream 
    const DwPtr<DwStream>& stream() const;
    /// return true if a stream is set
    bool hasStream() const;
    /// return true if the stream has reached EOF
    bool eof() const;
    /// begin reading from the stream
    virtual bool open();
    /// end reading from the stream
    virtual void close();
    /// return true if currently open
    bool isOpen() const;
protected:
    DwPtr<DwStream> m_stream;
    bool m_isOpen;
    bool m_streamWasOpen;
};// end of DwStreamReader

//------------------------------------------------------------------------------
/**
*/
inline
bool
DwStreamReader::isOpen() const
{
    return this->m_isOpen;
}