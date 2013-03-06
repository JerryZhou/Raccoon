#include "stable.h"
#include "dwringloghttphandler.h"
#include "dwapp/dwhtmlpagewriter.h"
#include "dwgui/dwfactory.h"
#include "dwapp/dwutil.h"
#include "dwgui/dwringloglib.h"

DW_IMPLEMENT_CLASS(DwRingLogHttpHandler, 'RLHH', DwHttpRequestHandler);

//------------------------------------------------------------------------------
/**
*/
DwRingLogHttpHandler::DwRingLogHttpHandler()
{
    this->setName("Ring Log List");
    this->setDesc("see the detail of thread log");
    this->setRootLocation("ringlog");
}

//------------------------------------------------------------------------------
/**
*/
void DwRingLogHttpHandler::handleRequest( const DwPtr<DwHttpRequest>& request )
{
    DwPtr<DwHtmlPageWriter> htmlWriter = DwHtmlPageWriter::create();
    htmlWriter->setStream(request->responseContentStream());
    if (htmlWriter->open())
    {
        const DwUrl& url = request->URI();

        if (url.localPath() == "ringlog")
        {
            handleMainPage(htmlWriter);
            request->setStatus(DwHttpStatus::OK);
        }
        else
        {
            DwVector<DwString> tokens = DwStringUtil::tokenize(url.localPath(), "/");
            if (tokens.size() == 2)
            {
                DWORD id = tokens[1].toUInt32();
                if (id)
                {
                    handleThreadPage(id, htmlWriter);
                    request->setStatus(DwHttpStatus::OK);
                }
            }
        }

        htmlWriter->close();
    }
}

//------------------------------------------------------------------------------
/**
*/
void DwRingLogHttpHandler::handleMainPage( const DwPtr<DwHtmlPageWriter>& htmlWriter )
{
    htmlWriter->Element(DwHtmlElement::Heading1, "DwRingLog Page");

    htmlWriter->Element(DwHtmlElement::Heading3, "Available Pages");        

    htmlWriter->Begin(DwHtmlElement::UnorderedList);
    DwMap<DWORD, DwString>  objects = DwRingLogLib::Instance()->logContents();
    if (!objects.isEmpty())
    {
        DwString str;
        DwStringUtil::stringFormat(str, "There are %d thread log.", objects.count());
        htmlWriter->Element(DwHtmlElement::Heading2, str);

        DwMap<DWORD, DwString>::const_iterator ite = objects.begin();
        while(ite != objects.end())
        {
            DwString str;
            htmlWriter->Begin(DwHtmlElement::ListItem);
            DwStringUtil::stringFormat(str, "ringlog/%d", ite->first);
            htmlWriter->AddAttr("href", str);
            DwStringUtil::stringFormat(str, "thread: %d", ite->first);
            htmlWriter->Element(DwHtmlElement::Anchor, str);
            htmlWriter->Begin(DwHtmlElement::Paragraph);
            DwVector<DwString> lines = DwStringUtil::tokenize(ite->second, "\n");
            for (int i=0; i<lines.size(); ++i)
            {
                htmlWriter->Text(lines.at(i));
                if (i != lines.size()-1)
                {
                    htmlWriter->LineBreak();
                }
            }
            htmlWriter->End(DwHtmlElement::Paragraph);
            htmlWriter->End(DwHtmlElement::ListItem);
            ++ite;
        }
    }
    htmlWriter->End(DwHtmlElement::UnorderedList);
}

//------------------------------------------------------------------------------
/**
*/
void DwRingLogHttpHandler::handleThreadPage( DWORD id, const DwPtr<DwHtmlPageWriter>& htmlWriter )
{
    DwString str;
    DwStringUtil::stringFormat(str, "Thread id: %d", id);
    htmlWriter->Element(DwHtmlElement::Heading2, str);
    htmlWriter->LineBreak();
    htmlWriter->Element(DwHtmlElement::Heading2, "Log:");
    htmlWriter->LineBreak();

    str = DwRingLogLib::Instance()->logContent(id);
    htmlWriter->Begin(DwHtmlElement::Paragraph);
    DwVector<DwString> lines = DwStringUtil::tokenize(str, "\n");
    for (int i=0; i<lines.size(); ++i)
    {
        htmlWriter->Text(lines.at(i));
        if (i != lines.size()-1)
        {
            htmlWriter->LineBreak();
        }
    }
    
    htmlWriter->End(DwHtmlElement::Paragraph);
}
