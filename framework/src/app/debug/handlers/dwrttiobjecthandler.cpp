#include "stable.h"
#include "dwrttiobjecthandler.h"
#include "dwapp/dwhtmlpagewriter.h"
#include "dwgui/dwfactory.h"
#include "dwapp/dwutil.h"

DW_IMPLEMENT_CLASS(DwRttiObjectHandler, 'ROHR', DwHttpRequestHandler);

//------------------------------------------------------------------------------
/**
*/
DwRttiObjectHandler::DwRttiObjectHandler()
{
    this->setName("Rtti Object List");
    this->setDesc("see the detail of run time information");
    this->setRootLocation("rttiobject");
}

//------------------------------------------------------------------------------
/**
*/
void DwRttiObjectHandler::handleRequest(const DwPtr<DwHttpRequest>& request)
{
    DwPtr<DwHtmlPageWriter> htmlWriter = DwHtmlPageWriter::create();
    htmlWriter->setStream(request->responseContentStream());
    if (htmlWriter->open())
    {
        const DwUrl& url = request->URI();

        if (url.localPath() == "rttiobject")
        {
            handleMainPage(htmlWriter);
            request->setStatus(DwHttpStatus::OK);
        }
        else
        {
            DwVector<DwString> tokens = DwStringUtil::tokenize(url.localPath(), "/");
            if (tokens.size() == 2)
            {
                DwString className = tokens[1];
                const DwRtti* info = dwFactory()->classRttiOf(className);
                if (info)
                {
                    handleClassPage(info, htmlWriter);
                    request->setStatus(DwHttpStatus::OK);
                }
            }
            else if (tokens.size() == 3)
            {
                DwString className = tokens[1];
                DwString kind = tokens[2];
                const DwRtti* info = dwFactory()->classRttiOf(className);
                if (info && kind == "Instances")
                {
                    handleObjectPage(info, htmlWriter);
                    request->setStatus(DwHttpStatus::OK);
                }
            }
        }

        htmlWriter->close();
    }
}

struct DFourCC
{
    DFourCC(int c)
    {
        fourcc = c;
    }

    union
    {
        int fourcc;
        struct
        {
            char revision;
            char build;
            char minor;
            char major;
        };
    };
};

//------------------------------------------------------------------------------
/**
*/
void DwRttiObjectHandler::handleMainPage(const DwPtr<DwHtmlPageWriter>& htmlWriter)
{
    htmlWriter->Element(DwHtmlElement::Heading1, "DwRttiObject Page");

    htmlWriter->Element(DwHtmlElement::Heading3, "Available Pages");        

    htmlWriter->Begin(DwHtmlElement::UnorderedList);
    DwVector<DwRtti*> objects = dwFactory()->rttis();
    if (objects.size() > 0)
    {
        DwString str;
        DwStringUtil::stringFormat(str, "There are %d class.", objects.size());
        htmlWriter->Element(DwHtmlElement::Heading2, str);

        for (int i=0; i<objects.size(); ++i)
        {
            DwString str;
            DwRtti *obj = objects.at(i);
            htmlWriter->Begin(DwHtmlElement::ListItem);

            DwStringUtil::stringFormat(str, L"rttiobject/%s", obj->name().constString());
            htmlWriter->AddAttr("href", str);
            htmlWriter->Element(DwHtmlElement::Anchor, obj->name());
            
            DFourCC fourcc(obj->fourCC());
            if (fourcc.major != 0)
            {
                DwStringUtil::stringFormat(str, "-instance size: %d -fourcc: '%c%c%c%c'", obj->instanceSize(), 
                    fourcc.major, fourcc.minor, fourcc.build, fourcc.revision);
            }
            else
            {
                DwStringUtil::stringFormat(str, "-instance size: %d -fourcc: %d", obj->instanceSize(), obj->fourCC());
            }
            
            htmlWriter->Text(str);
            htmlWriter->End(DwHtmlElement::ListItem);
        }
    }
    htmlWriter->End(DwHtmlElement::UnorderedList);
}

//------------------------------------------------------------------------------
/**
*/
void DwRttiObjectHandler::handleClassPage(const DwRtti* obj, const DwPtr<DwHtmlPageWriter>& htmlWriter)
{
    htmlWriter->Element(DwHtmlElement::Heading2, obj->name());

    htmlWriter->Begin(DwHtmlElement::ListItem);

    const DwRtti *parentInfo = obj->parent();
    DFourCC fourcc(obj->fourCC());

    DwString str;
    if (parentInfo)
    {
        htmlWriter->Text("parent: ");
        htmlWriter->AddAttr("href", parentInfo->name());
        htmlWriter->Element(DwHtmlElement::Anchor, parentInfo->name());
    }

    if (fourcc.major != 0)
    {
        DwStringUtil::stringFormat(str, " -instance size: %d -fourcc: '%c%c%c%c'",
            obj->instanceSize(), 
            fourcc.major, fourcc.minor, fourcc.build, fourcc.revision);
    }
    else
    {
        DwStringUtil::stringFormat(str, " -instance size: %d -fourcc: %d",
            obj->instanceSize(), obj->fourCC());
    }
    htmlWriter->Text(str);

    DwStringUtil::stringFormat(str, L"%s/%s", obj->name().constString(), "Instances");
    htmlWriter->AddAttr("href", str);
    htmlWriter->Element(DwHtmlElement::Anchor, " Instances");

    htmlWriter->End(DwHtmlElement::ListItem);
}

//------------------------------------------------------------------------------
/**
*/
void DwRttiObjectHandler::handleObjectPage(const DwRtti* obj, const DwPtr<DwHtmlPageWriter>& htmlWriter)
{
    DwVector<DwRttiObject *> objects = DwRttiObject::dumpObjectsOf((DwRtti*)obj);
    
    DwString str;
    DwStringUtil::stringFormat(str, L"There are %d instance of %s.", objects.size(), obj->name().constString());
    htmlWriter->Element(DwHtmlElement::Heading2, str);

    if (objects.size() > 0)
    {
        htmlWriter->Begin(DwHtmlElement::UnorderedList);
        for (int i=0; i<objects.size(); ++i)
        {
            htmlWriter->Begin(DwHtmlElement::ListItem);
            DwDebug info;
            objects.at(i)->debugString(info);
            info>>str;
            htmlWriter->Text(str);
            htmlWriter->End(DwHtmlElement::ListItem);
        }
        htmlWriter->End(DwHtmlElement::UnorderedList);
    }
}