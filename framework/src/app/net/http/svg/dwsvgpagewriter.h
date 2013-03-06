#pragma once
#include "dwapp/dwstreamwriter.h"
#include "dwapp/dwxmlwriter.h"
#include "dwcore/dwpoint.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwSvgPageWriter : public DwStreamWriter
{
    DW_DECLARE_CLASS(DwSvgPageWriter);
public:
    /// constructor
    DwSvgPageWriter();
    /// destructor
    virtual ~DwSvgPageWriter();
    /// set width and height of canvas in pixels
    void SetCanvasDimensions(size_t w, size_t h);
    /// set width and height in draw units
    void SetUnitDimensions(size_t w, size_t h);
    /// begin writing to the stream
    virtual bool open();
    /// end writing to the stream
    virtual void close();

    /// write content text
    void WriteContent(const DwString& text);
    /// begin a new node under the current node
    void BeginNode(const DwString& nodeName);
    /// end current node, set current node to parent
    void EndNode();
    /// set string attribute on current node
    void SetString(const DwString& name, const DwString& value);

    /// begin a transformation group
    void BeginTransformGroup(const DwPointF& translate, float rotate, const DwPointF& scale);
    /// begin a paint group
    void BeginPaintGroup(const DwString& fillColor, const DwString& strokeColor, int strokeWidth);
    /// begin a text group
    void BeginTextGroup(int fontSize, const DwString& textColor);
    /// end the most recent group
    void EndGroup();

    /// draw a rectangle, all units are in pixels
    void Rect(float x, float y, float w, float h);
    /// draw a circle, all units are in pixels
    void Circle(float x, float y, float r);
    /// draw an ellipse, all units are in pixels
    void Ellipse(float x, float y, float rx, float ry);
    /// draw a line, all units are in pixels
    void Line(float x1, float y1, float x2, float y2);
    /// draw a poly-line, all units are in pixels
    void PolyLine(const DwVector<DwPointF>& points);
    /// draw a polygon, all units are in pixels
    void Polygon(const DwVector<DwPointF>& points);
    /// draw text, all units are in pixels
    void Text(float x, float y, const DwString& text);

protected:
    DwPtr<DwXmlWriter> xmlWriter;
    size_t canvasWidth;
    size_t canvasHeight;
    size_t unitWidth;
    size_t unitHeight;
};// end of DwSvgPageWriter

//------------------------------------------------------------------------------
/**
*/
inline void
DwSvgPageWriter::SetCanvasDimensions(size_t w, size_t h)
{
    this->canvasWidth = w;
    this->canvasHeight = h;
}

//------------------------------------------------------------------------------
/**
*/
inline void
DwSvgPageWriter::SetUnitDimensions(size_t w, size_t h)
{
    this->unitWidth = w;
    this->unitHeight = h;
}
    