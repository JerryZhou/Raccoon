
#include "stable.h"
#include "dwsvglinechartwriter.h"
#include "dwapp/dwutil.h"

DW_IMPLEMENT_CLASS(DwSvgLineChartWriter, 'SLCW', DwSvgPageWriter);

//------------------------------------------------------------------------------
/**
*/
DwSvgLineChartWriter::DwSvgLineChartWriter() :
    xAxisMinVal(0),
    xAxisMaxVal(100),
    yAxisMinVal(0.0f),
    yAxisMaxVal(1.0f)
{
    // empty
}    

//------------------------------------------------------------------------------
/**
*/
DwSvgLineChartWriter::~DwSvgLineChartWriter()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
DwSvgLineChartWriter::open()
{
    // always set virtual dimensions to 1000x250
    this->SetUnitDimensions(1200, 250);
    return DwSvgPageWriter::open();
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgLineChartWriter::SetupXAxis(const DwString& axisName, const DwString& unitName, int minVal, int maxVal)
{
    DW_ASSERT(maxVal > minVal);
    this->xAxisName = axisName;
    this->xAxisUnitName = unitName;
    this->xAxisMinVal = minVal;
    this->xAxisMaxVal = maxVal;
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgLineChartWriter::SetupYAxis(const DwString& axisName, const DwString& unitName, float minVal, float maxVal)
{
    DW_ASSERT(maxVal > minVal);
    this->yAxisName = axisName;
    this->yAxisUnitName = unitName;
    this->yAxisMinVal = minVal;
    this->yAxisMaxVal = maxVal;
}

//------------------------------------------------------------------------------
/**
*/
void
DwSvgLineChartWriter::AddTrack(const DwString& name, const DwString& color, const DwVector<float>& values)
{
    DW_ASSERT(values.size() > 0);
    Track newTrack;
    this->tracks.append(newTrack);
    this->tracks.back().name = name;
    this->tracks.back().color = color;
    this->tracks.back().values = values;
}

//------------------------------------------------------------------------------
/**
*/
void 
DwSvgLineChartWriter::AddTrack(const DwString& name, const DwString& color, const DwVector<float>& values, const DwVector<DWORD>& timeStamps)
{    
    DW_ASSERT(values.size() > 0);
    DW_ASSERT(values.size() == timeStamps.size());
    Track newTrack;
    this->tracks.append(newTrack);
    this->tracks.back().name = name;
    this->tracks.back().color = color;
    this->tracks.back().values = values;
    this->tracks.back().times = timeStamps;

}
//------------------------------------------------------------------------------
/**
*/
void
DwSvgLineChartWriter::Draw()
{
    // establish a transform for the main diagram
    this->BeginTransformGroup(DwPointF(10.0f, 10.0f), 0.0f, DwPointF(1.0f, 1.0f));
    
    // draw light-weight helper lines
    this->BeginPaintGroup("grey", "grey", 1);
    this->Line(0, 0,   1000, 0);
    this->Line(0, 50,  1000, 50);
    this->Line(0, 100, 1000, 100);
    this->Line(0, 150, 1000, 150);
    this->EndGroup();

    // draw the main diagram axis
    this->BeginPaintGroup("black", "black", 2);
    this->Line(0, 0, 0, 200);           // left y-axis
    this->Line(1000, 0, 1000, 200);     // right y-axis
    this->Line(0, 200, 1000, 200);      // x-axis
    this->EndGroup();

    // for each track...
    int trackIndex;
    for (trackIndex = 0; trackIndex < this->tracks.size(); trackIndex++)
    {
        const Track& curTrack = this->tracks[trackIndex];
        float x = 0.0f;
        float dx = 1.0f / (this->xAxisMaxVal - this->xAxisMinVal);
        float minVal = 1000000.0f;
        float maxVal = -1000000.0f;
        float avgVal = 0.0f;
        int valIndex = 0;
        int batchSize = 100;
        DwVector<DwPointF> points;
        points.reserve(batchSize + 1);
        DwVector<DwString> timeText;
        timeText.reserve(batchSize + 1);
        this->BeginPaintGroup("none", curTrack.color, 2);
        while (valIndex < curTrack.values.size())
        {
            // need to render points in chunks of 100 due to a limitation
            // in TinyXML(?), XML attribute values are limited to about 4096 characters
            float curValue;
            float curY = 0.0f;
            int i;
            for (i = 0; (i < batchSize) && (valIndex < curTrack.values.size()); i++)
            {
                curValue = curTrack.values[valIndex++];
                curY = 1.0f - (curValue - this->yAxisMinVal) / (this->yAxisMaxVal - this->yAxisMinVal);
                points.append(DwPointF(1000.0f * x, 200.0f * curY));
                x += dx;
                minVal = dwMin(minVal, curValue);
                maxVal = dwMax(maxVal, curValue);
                avgVal += curValue;  
                if (curTrack.times.size() > 0)
                {           
                    timeText.append(DwString::number(curTrack.times[i]));
                }
            }
            this->PolyLine(points);
            
            if (curTrack.times.size() > 0)
            {       
                this->EndGroup();
                this->BeginTextGroup(8, "black"); 
                int txtIdx;
                for (txtIdx = 0; txtIdx < timeText.size(); ++txtIdx)
                {
                    this->Text((float)points[txtIdx].x(), 210, timeText[txtIdx]);      	
                }
                this->EndGroup();
                this->BeginPaintGroup("none", curTrack.color, 2);
                timeText.clear();
            }

            // prepare next batch
            points.clear();
            points.append(DwPointF(1000.f * (x - dx), 200.0f * curY));
        }
        this->EndGroup();
        // compute average value
        avgVal /= curTrack.values.size();

        // draw min/max/avg lines
        float minY = 1.0f - (minVal - this->yAxisMinVal) / (this->yAxisMaxVal - this->yAxisMinVal);
        float maxY = 1.0f - (maxVal - this->yAxisMinVal) / (this->yAxisMaxVal - this->yAxisMinVal);
        float avgY = 1.0f - (avgVal - this->yAxisMinVal) / (this->yAxisMaxVal - this->yAxisMinVal);
        this->BeginPaintGroup("none", curTrack.color, 1);
        this->Line(0.0f, 200.0f * minY, 1000.0f, 200.0f * minY);
        this->Line(0.0f, 200.0f * maxY, 1000.0f, 200.0f * maxY);
        this->Line(0.0f, 200.0f * avgY, 1000.0f, 200.0f * avgY);
        this->EndGroup();

        // draw min/max/avg text
        DwString minText;
        DwString maxText;
        DwString avgText;
        DwStringUtil::stringFormat(maxText, "max = %.3f", maxVal);
        if (avgVal != maxVal) DwStringUtil::stringFormat(avgText, "avg = %.3f", avgVal);
        if (maxVal != minVal) DwStringUtil::stringFormat(minText, "min = %.3f", minVal);                              

        this->BeginTextGroup(10, curTrack.color);                                                        
        this->Text(1010.0f, 200.0f * maxY, maxText);
        float curOffset = 0;
        const float minDist = 0.04f;
        if (avgY - maxY < minDist)
        {
            avgY += minDist;
            curOffset = minDist;
        }
        if (!avgText.isEmpty()) this->Text(1010.0f, 200.0f * avgY, avgText);
        if (minY + curOffset - avgY < minDist) minY += curOffset + minDist;
        if (!minText.isEmpty()) this->Text(1010.0f, 200.0f * minY, minText);        
        this->EndGroup();
    }
    this->EndGroup();
}
