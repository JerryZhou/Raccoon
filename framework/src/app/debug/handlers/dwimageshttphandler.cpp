#include "stable.h"
#include "dwimageshttphandler.h"
#include "dwapp/dwhtmlpagewriter.h"
#include "dwgraphics/dwimage.h"
#include "dwgui/dwfactory.h"
#include "dwapp/dwutil.h"


DW_IMPLEMENT_CLASS(DwImagesHttpHandler, 'IMGR', DwHttpRequestHandler);

//------------------------------------------------------------------------------
/**
*/
DwImagesHttpHandler::DwImagesHttpHandler()
{
    this->setName("Images List");
    this->setDesc("see the detail of images");
    this->setRootLocation("images");
}

//------------------------------------------------------------------------------
/**
*/
void DwImagesHttpHandler::handleRequest( const DwPtr<DwHttpRequest>& request )
{
    DwPtr<DwHtmlPageWriter> htmlWriter = DwHtmlPageWriter::create();
    htmlWriter->setStream(request->responseContentStream());
    if (htmlWriter->open())
    {
        htmlWriter->Element(DwHtmlElement::Heading1, "DwImage Page");

        htmlWriter->Begin(DwHtmlElement::UnorderedList);
        DwLinkedList<DwImage> objects = DwImage::allImages();
        if (!objects.isEmpty())
        {
            DwString str;
            DwStringUtil::stringFormat(str, "There are %d images.", objects.size());
            htmlWriter->Element(DwHtmlElement::Heading2, str);

            DwLinkedList<DwImage>::const_iterator ite = objects.begin();
            while(ite != objects.end())
            {
                const DwImage& img = *ite;
                htmlWriter->Begin(DwHtmlElement::ListItem);

                DwStringUtil::stringFormat(str, L"-image path: %s -image id: %d -image ref: %d -size: (%d, %d)", 
                    img.path().constString(), 
                    img.uniqueKey(),
                    img.refCnt(),
                    img.size().width(),
                    img.size().height());

                htmlWriter->Text(str);
                htmlWriter->End(DwHtmlElement::ListItem);
                ++ite;
            }
        }
        htmlWriter->End(DwHtmlElement::UnorderedList);
        request->setStatus(DwHttpStatus::OK);
        htmlWriter->close();
    }
}

