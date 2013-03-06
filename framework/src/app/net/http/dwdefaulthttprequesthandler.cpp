
#include "stable.h"
#include "dwdefaulthttprequesthandler.h"
#include "dwapp/dwptr.h"
#include "dwapp/dwhttpserver.h"
#include "dwapp/dwutil.h"
#include "dwapp/dwhtmlpagewriter.h"

DW_IMPLEMENT_CLASS(DwDefaultHttpRequestHandler, 'DHRH', DwHttpRequestHandler);

//------------------------------------------------------------------------------
/**
*/
void DwDefaultHttpRequestHandler::handleRequest(const DwPtr<DwHttpRequest>& request)
{
    // don't accept anything then Get
    if (DwHttpMethod::Get != request->method())
    {
        request->setStatus(DwHttpStatus::NotImplemented);
        return;
    }

    // always show the home page
    DwPtr<DwHtmlPageWriter> htmlWriter = DwHtmlPageWriter::create();
    htmlWriter->setStream(request->responseContentStream());
    htmlWriter->SetTitle("Raccoon Application");
    if (htmlWriter->open())
    {
        htmlWriter->Element(DwHtmlElement::Heading1, "Main Page");
        htmlWriter->Element(DwHtmlElement::Heading3, "Application Info");
        htmlWriter->Begin(DwHtmlElement::Table);
        htmlWriter->Begin(DwHtmlElement::TableRow); 
        htmlWriter->Element(DwHtmlElement::TableData, "Company Name:");
        htmlWriter->Element(DwHtmlElement::TableData, "Duowan");
        htmlWriter->End(DwHtmlElement::TableRow);
        htmlWriter->Begin(DwHtmlElement::TableRow); 
        htmlWriter->Element(DwHtmlElement::TableData, "Application Name:");
        htmlWriter->Element(DwHtmlElement::TableData, "Raccoon");
        htmlWriter->End(DwHtmlElement::TableRow);
        htmlWriter->Begin(DwHtmlElement::TableRow); 
        htmlWriter->Element(DwHtmlElement::TableData, "Root Directory:");
        htmlWriter->Element(DwHtmlElement::TableData, DwUrl("bin:").localPath());        
        htmlWriter->End(DwHtmlElement::TableRow);
        htmlWriter->End(DwHtmlElement::Table);

        // list registered HttpRequestHandlers
        htmlWriter->Element(DwHtmlElement::Heading3, "Available Pages");        
        DwVector<DwPtr<DwHttpRequestHandler> > handlers = DwHttpServer::Instance()->requestHandlers();
        if (handlers.size() > 0)
        {
            htmlWriter->Begin(DwHtmlElement::UnorderedList);
            int i;
            for (i = 0; i < handlers.size(); i++)
            {
                const DwPtr<DwHttpRequestHandler>& handler = handlers[i];
                htmlWriter->Begin(DwHtmlElement::ListItem);
                htmlWriter->AddAttr("href", handler->rootLocation());
                htmlWriter->Element(DwHtmlElement::Anchor, handler->name());
                htmlWriter->Text(DwString(" - ") + handler->desc());
                htmlWriter->End(DwHtmlElement::ListItem);
            }
            htmlWriter->End(DwHtmlElement::UnorderedList);
        }
        htmlWriter->close();
        request->setStatus(DwHttpStatus::OK);
    }
    else
    {
        request->setStatus(DwHttpStatus::InternalServerError);
    }
    
   request->setStatus(DwHttpStatus::OK);
}