
#include "stable.h"
#include "dwhelloworldrequesthandler.h"
#include "dwapp/dwhtmlpagewriter.h"

DW_IMPLEMENT_CLASS(DwHelloWorldRequestHandler, 'HWRH', DwHttpRequestHandler);

//------------------------------------------------------------------------------
/**
*/
DwHelloWorldRequestHandler::DwHelloWorldRequestHandler()
{
    // we need to set a human-readable name, a description, and
    // the URI root location:
    this->setName("Hello World");
    this->setDesc("a sample HttpRequestHandler");
    this->setRootLocation("helloworld");
}

//------------------------------------------------------------------------------
/**
*/
void
DwHelloWorldRequestHandler::handleRequest(const DwPtr<DwHttpRequest>& request)
{
    // we could extract more information from the URI if we want, but
    // since this is the most simple HttpRequestHandler possible, we wont :)

    // in order to server a valid HTML page to the request's ResponseContentStream, 
    // we need to create a HtmlPageWriter and connect it to the 
    // ResponseContentStream, this is standard Nebula3 IO stuff...
    DwPtr<DwHtmlPageWriter> htmlWriter = DwHtmlPageWriter::create();
    htmlWriter->setStream(request->responseContentStream());
    htmlWriter->SetTitle("Hello World");
    if (htmlWriter->open())
    {
        // write some standard text to HTML page
        htmlWriter->Text("Hello World");
        htmlWriter->close();

        // finally set the HTTP status code
        request->setStatus(DwHttpStatus::OK);
    }
    else
    {
        // this shouldn't happen, but if something goes wrong, the 
        // web browser should know as well
        request->setStatus(DwHttpStatus::InternalServerError);
    }
}
