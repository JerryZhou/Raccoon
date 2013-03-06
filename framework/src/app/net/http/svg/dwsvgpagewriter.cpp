
#include "stable.h"
#include "dwsvgpagewriter.h"
#include "dwapp/dwutil.h"

DW_IMPLEMENT_CLASS(DwSvgPageWriter, 'SVGW', DwStreamWriter);

//------------------------------------------------------------------------------
/**
*/
DwSvgPageWriter::DwSvgPageWriter() :
    canvasWidth(100),
    canvasHeight(100),
    unitWidth(100),
    unitHeight(100)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DwSvgPageWriter::~DwSvgPageWriter()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
DwSvgPageWriter::open()
{
    if (DwStreamWriter::open())
    {
        // create an XmlWriter and attach it to our stream
        this->xmlWriter = DwXmlWriter::create();
        this->xmlWriter->setStream(this->m_stream);
        this->xmlWriter->open();

        // write SVG root element
        this->xmlWriter->beginNode("svg");
        this->xmlWriter->setString("xmlns", "http://www.w3.org/2000/svg");
        this->xmlWriter->setInt("width", this->canvasWidth);
        this->xmlWriter->setInt("height", this->canvasHeight);
        DwString viewBox;
        DwStringUtil::stringFormat(viewBox, "0 0 %d %d", this->unitWidth, this->unitHeight);
        this->xmlWriter->setString("viewBox", viewBox);
        this->xmlWriter->setString("preserveAspectRatio", "none");

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::close()
{
    DW_ASSERT(this->isOpen());

    // close SVG page
    this->xmlWriter->endNode();

    // close XML writer
    this->xmlWriter->close();
    this->xmlWriter = 0;

    // set MIME type on our stream
    this->m_stream->setMediaType(DwMediaType("image/svg+xml"));

    // call parent class
    DwStreamWriter::close();
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::BeginNode(const DwString& nodeName)
{
    this->xmlWriter->beginNode(nodeName);
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::EndNode()
{
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::SetString(const DwString& name, const DwString& value)
{
    this->xmlWriter->setString(name, value);
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::BeginTransformGroup(const DwPointF& translate, float rotate, const DwPointF& scale)
{
    // format the attribute string
    DwString str;
    DwStringUtil::stringFormat(str, "translate(%.3f,%.3f) rotate(%.3f) scale(%.3f,%.3f)", 
        translate.x(), translate.y(),
        rotate,
        scale.x(), scale.y());

    // write XML element
    this->xmlWriter->beginNode("g");
    this->xmlWriter->setString("transform", str);
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::BeginPaintGroup(const DwString& fillColor, const DwString& strokeColor, int strokeWidth)
{
    DW_ASSERT(!fillColor.isEmpty());
    DW_ASSERT(!strokeColor.isEmpty());

    this->xmlWriter->beginNode("g");
    this->xmlWriter->setString("fill", fillColor);
    this->xmlWriter->setString("stroke", strokeColor);
    this->xmlWriter->setInt("stroke-width", strokeWidth);
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::BeginTextGroup(int fontSize, const DwString& textColor)
{
    this->xmlWriter->beginNode("g");
    this->xmlWriter->setInt("font-size", fontSize);
    this->xmlWriter->setString("fill", textColor);
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::EndGroup()
{
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::Rect(float x, float y, float w, float h)
{
    this->xmlWriter->beginNode("rect");
        this->xmlWriter->setFloat("x", x);
        this->xmlWriter->setFloat("y", y);
        this->xmlWriter->setFloat("width", w);
        this->xmlWriter->setFloat("height", h);
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::Circle(float x, float y, float r)
{
    this->xmlWriter->beginNode("circle");
        this->xmlWriter->setFloat("cx", x);
        this->xmlWriter->setFloat("cy", y);
        this->xmlWriter->setFloat("r", r);
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::Ellipse(float x, float y, float rx, float ry)
{
    this->xmlWriter->beginNode("ellipse");
        this->xmlWriter->setFloat("cx", x);
        this->xmlWriter->setFloat("cy", y);
        this->xmlWriter->setFloat("rx", rx);
        this->xmlWriter->setFloat("ry", ry);
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::Line(float x1, float y1, float x2, float y2)
{
    this->xmlWriter->beginNode("line");
        this->xmlWriter->setFloat("x1", x1);
        this->xmlWriter->setFloat("y1", y1);
        this->xmlWriter->setFloat("x2", x2);
        this->xmlWriter->setFloat("y2", y2);
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
    Note: due to a limitation for the length of XML attributes in TinyXML
    the array size for a single poly primitive should be about 200 max.
*/
void
DwSvgPageWriter::PolyLine(const DwVector<DwPointF>& points)
{
    // build coordinates string
    DwString str;
    int i;
    for (i = 0; i < points.size(); i++)
    {
        DwString s;
        DwStringUtil::stringFormat(s, "%.6f,%.6f ", points[i].x(), points[i].y());
        str.append(s);
    }

    // write XML element
    this->xmlWriter->beginNode("polyline");
        this->xmlWriter->setString("points", str);
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
    Note: due to a limitation for the length of XML attributes in TinyXML
    the array size for a single poly primitive should be about 200 max.
*/
void
DwSvgPageWriter::Polygon(const DwVector<DwPointF>& points)
{
    // build coordinates string
    DwString str;
    int i;
    for (i = 0; i < points.size(); i++)
    {
        DwString s;
        DwStringUtil::stringFormat(s, "%.6f,%.6f ", points[i].x(), points[i].y());

        str.append(s);
    }

    // write XML element
    this->xmlWriter->beginNode("polygon");
        this->xmlWriter->setString("points", str);
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::Text(float x, float y, const DwString& text)
{
    this->xmlWriter->beginNode("text");
        this->xmlWriter->setFloat("x", x);
        this->xmlWriter->setFloat("y", y);
        this->xmlWriter->writeContent(text);
    this->xmlWriter->endNode();
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgPageWriter::WriteContent(const DwString& text)
{
    this->xmlWriter->writeContent(text);
}