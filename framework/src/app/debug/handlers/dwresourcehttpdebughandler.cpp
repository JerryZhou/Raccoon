#include "stable.h"
#include "dwresourcehttpdebughandler.h"
#include "dwapp/dwhtmlpagewriter.h"
#include "dwgui/dwresourceinterface.h"
#include "dwgui/dwresourcemapping.h"
#include "dwgui/dwfactory.h"
#include "dwapp/dwutil.h"

DW_IMPLEMENT_CLASS(DwResourceHttpDebugHandler, 'RHDH', DwHttpRequestHandler);

//------------------------------------------------------------------------------
/**
*/
DwResourceHttpDebugHandler::DwResourceHttpDebugHandler()
{
    setName("Resources List");
    setDesc("see all the application resources");
    setRootLocation("resource");
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceHttpDebugHandler::handleRequest( const DwPtr<DwHttpRequest>& request )
{
    DwPtr<DwHtmlPageWriter> htmlWriter = DwHtmlPageWriter::create();
    htmlWriter->setStream(request->responseContentStream());
    if (htmlWriter->open())
    {
        const DwUrl& url = request->URI();
        if (url.localPath() == rootLocation())
        {
            handleMainPage(htmlWriter);
            request->setStatus(DwHttpStatus::OK);
        }
        else
        {
            DwVector<DwString> tokens = DwStringUtil::tokenize(url.localPath(), "/");
            if (tokens.size() == 2)
            {
                DwString scheme = tokens[1];

                DwResourceMapping * mapping = dwResourceLib()->mappingOf(scheme);
                if (mapping)
                {
                    handleDetailOfResourceMapping(mapping, htmlWriter);
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
void DwResourceHttpDebugHandler::handleMainPage( const DwPtr<DwHtmlPageWriter>& htmlWriter )
{
    htmlWriter->Element(DwHtmlElement::Heading1, "Resources Page");

    htmlWriter->Element(DwHtmlElement::Heading3, "Available Pages");        

    htmlWriter->Begin(DwHtmlElement::UnorderedList);
    DwVector<DwResourceMapping*> maps = dwResourceLib()->mappings();
    if (maps.size() > 0)
    {
        DwString str;
        DwStringUtil::stringFormat(str, "There are %d scheme.", maps.size());
        htmlWriter->Element(DwHtmlElement::Heading2, str);

        for (int i=0; i<maps.size(); ++i)
        {
            DwString str;
            const DwResourceMapping *obj = maps.at(i);
            htmlWriter->Begin(DwHtmlElement::ListItem);

            DwStringUtil::stringFormat(str, L"resource/%s", obj->scheme().constString());
            htmlWriter->AddAttr("href", str);
            htmlWriter->Element(DwHtmlElement::Anchor, obj->scheme());

            DwStringUtil::stringFormat(str, L"-class name: %s", obj->rtti()->name().constString());
            htmlWriter->Text(str);
            htmlWriter->End(DwHtmlElement::ListItem);
        }
    }
    htmlWriter->End(DwHtmlElement::UnorderedList);
}

//------------------------------------------------------------------------------
/**
*/
void DwResourceHttpDebugHandler::handleDetailOfResourceMapping( DwResourceMapping* mapping, const DwPtr<DwHtmlPageWriter>& htmlWriter )
{
    DwString str;
    DwStringUtil::stringFormat(str, L"Scheme: %s, Class Name: %s", mapping->scheme().constString(), mapping->rtti()->name().constString());
    htmlWriter->Element(DwHtmlElement::Heading2, str);

    const DwMap<DwString, DwByteArray>& mapDatas = mapping->datas();
    DwStringUtil::stringFormat(str, "There are %d resources in this mapping.", mapDatas.count());
    htmlWriter->Element(DwHtmlElement::Heading2, str);

    if (mapDatas.count() > 0)
    {
        int totalSize = 0;
        DwMap<DwString, DwByteArray>::const_iterator ite = mapDatas.begin();
        while(ite != mapDatas.end())
        {
            const DwString & location = ite->first;
            const DwByteArray & data = ite->second;

            htmlWriter->Begin(DwHtmlElement::ListItem);

            DwStringUtil::stringFormat(str, L"location: %s -size: %d(byte)", location.constString(), data.size());
            htmlWriter->Text(str);
            htmlWriter->End(DwHtmlElement::ListItem);

            totalSize += data.size();
            ++ite;
        }

        DwStringUtil::stringFormat(str, L"Total size of scheme %s is %.3f(Kb)", mapping->scheme().constString(), 0.001 * totalSize);
        htmlWriter->Element(DwHtmlElement::Heading2, str);
    }
}
