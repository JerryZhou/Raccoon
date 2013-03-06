
#include "stable.h"
#include "dwsvgtestpagehandler.h"
#include "dwapp/dwsvgpagewriter.h"
#include "dwapp/dwsvglinechartwriter.h"
#include "dwapp/dwutil.h"

DW_IMPLEMENT_CLASS(DwSvgTestPageHandler, 'SVTP', DwHttpRequestHandler);

//------------------------------------------------------------------------------
/**
*/
DwSvgTestPageHandler::DwSvgTestPageHandler()
{
    this->setName("SVG Rendering Test");
    this->setDesc("test SVG rendering functionality");
    this->setRootLocation("svgtest");
}

//------------------------------------------------------------------------------
/**
*/
bool
DwSvgTestPageHandler::TestShapeRendering(const DwPtr<DwHttpRequest>& request)
{
    // setup a SVG page writer
    DwPtr<DwSvgPageWriter> svgWriter = DwSvgPageWriter::create();
    svgWriter->setStream(request->responseContentStream());
    svgWriter->SetCanvasDimensions(1000, 1000);
    svgWriter->SetUnitDimensions(1000, 1000);
    if (svgWriter->open())
    {
        // render an outline
        svgWriter->BeginPaintGroup("none", "black", 1);
            svgWriter->Rect(0, 0, 1000, 1000);
        svgWriter->EndGroup();

        // render rectangles
        svgWriter->BeginPaintGroup("red", "black", 1);
            svgWriter->Rect(100, 100, 150, 150);
            svgWriter->Rect(300, 300, 150, 150);
        svgWriter->EndGroup();
        svgWriter->BeginPaintGroup("black", "red", 1);
            svgWriter->Rect(300, 100, 150, 150);
            svgWriter->Rect(100, 300, 150, 150);
        svgWriter->EndGroup();

        // render circles
        svgWriter->BeginPaintGroup("red", "black", 1);
            svgWriter->Circle(700, 700, 50);
            svgWriter->Circle(800, 800, 50);
        svgWriter->EndGroup();
        svgWriter->BeginPaintGroup("black", "red", 1);
            svgWriter->Circle(700, 800, 50);
            svgWriter->Circle(800, 700, 50);
        svgWriter->EndGroup();

        // render a polyline
        svgWriter->BeginPaintGroup("none", "green", 1);
            DwVector<DwPointF> points;
            int x;
            for (x = 100; x < 400; x += 2)
            {
                points.append(DwPointF(float(x), 750.0f + DwMathUtil::random(-250, 250)));
            }
            svgWriter->PolyLine(points);
        svgWriter->EndGroup();

        // render some text
        svgWriter->BeginTextGroup(30, "black");
            svgWriter->Text(600, 250, "Hello World");
        svgWriter->EndGroup();

        // done...
        svgWriter->close();
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
DwSvgTestPageHandler::TestLineChartRendering(const DwPtr<DwHttpRequest>& request)
{
    // setup a SVG line chart writer
    DwPtr<DwSvgLineChartWriter> writer = DwSvgLineChartWriter::create();
    writer->setStream(request->responseContentStream());
    writer->SetCanvasDimensions(1024, 256);
    if (writer->open())
    {
        writer->SetupXAxis("frame", "frames", -512, 0);
        writer->SetupYAxis("fps", "1/sec", 0.0f, 60.0f);
        DwVector<float> values;
        values.reserve(512);
        int i;
        for (i = 0; i < 512; i++)
        {
            values.append(DwMathUtil::randomF(10.0f, 45.0f));
        }
        writer->AddTrack("fps", "green", values);
        writer->Draw();
        writer->close();
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgTestPageHandler::handleRequest(const DwPtr<DwHttpRequest>& request)
{
    if (this->TestLineChartRendering(request))
    {
        request->setStatus(DwHttpStatus::OK);
    }
    else
    {
        request->setStatus(DwHttpStatus::InternalServerError);
    }
}