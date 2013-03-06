#pragma once

#include "dwapp/dwrefcounted.h"
#include "dwapp/dwptr.h"
#include "dwapp/dwstream.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwStreamWriter : public DwRefCounted
{
    DW_DECLARE_CLASS(DwStreamWriter);
public:
    /// constructor
    DwStreamWriter();
    /// destructor
    virtual ~DwStreamWriter();
    /// set stream to write to
    void setStream(const DwPtr<DwStream>& s);
    /// get currently set stream 
    const DwPtr<DwStream>& stream() const;
    /// return true if a stream is set
    bool hasStream() const;
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
};// end of DwStreamWriter

//------------------------------------------------------------------------------
/**
*/
inline bool DwStreamWriter::isOpen() const
{
    return this->m_isOpen;
}