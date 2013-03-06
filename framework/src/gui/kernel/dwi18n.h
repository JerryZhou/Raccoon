#pragma once
#include "dwgui/dwurl.h"
#include "dwcore/dwhash.h"

//------------------------------------------------------------------------------
class DwXmlDocumentContainer;

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwI18n
{
public:
    DwI18n();
    virtual ~DwI18n();

    virtual bool	isEmpty () const;

    bool    load ( const DwUrl & url);
    bool	load ( const char * data, int len );
    bool    save ( const DwUrl & url);

    // context is the class name, usually the class name for the tr() caller.
    // sourceText is the real solution, usually the argument to tr().
    // disambiguation is an optional string that helps disambiguate different uses of the same text in the same context 
    DwString    translate ( const char * context, const char * sourceText, const char * disambiguation = 0 );

    DW_GLOBAL_STATIC(DwI18n, instance);
protected:
    /* {{{ TODO: use the memory hash table
    typedef DwString Context;
    typedef DwString SourceText;
    typedef DwString Disambiguation;
    typedef DwHash<Disambiguation, DwString> Disambiguations;
    typedef DwHash<SourceText, Disambiguations> SourceTexts;
    typedef DwHash<Context, SourceTexts> Contexts;
    Contexts m_contents;
    }}} */

    DwXmlDocumentContainer *m_doc;
};// end of DwI18n

// will only used in class dwrttiobject and it's sub class
// in default namespace("DwI18n")
#define RttiTr(sourceText) DwI18n::instance()->translate(NULL, sourceText, NULL)
#define RttiTrWith(sourceText, disambiguation) DwI18n::instance()->translate(NULL, sourceText, disambiguation)
#define RttiTrWithIn(sourceText, disambiguation, inIt) DwI18n::instance()->translate(inIt, sourceText, disambiguation)