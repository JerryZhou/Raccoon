#pragma once

#include "dwapp/dwsvgpagewriter.h"

//------------------------------------------------------------------------------
class DW_APP_EXPORT DwSvgLineChartWriter : public DwSvgPageWriter
{
    DW_DECLARE_CLASS(DwSvgLineChartWriter);
public:
    /// constructor
    DwSvgLineChartWriter();
    /// destructor
    virtual ~DwSvgLineChartWriter();
    
    /// begin writing to the stream
    virtual bool open();
    /// setup the x axis
    void SetupXAxis(const DwString& axisName, const DwString& unitName, int minVal, int maxVal);
    /// setup the y axis
    void SetupYAxis(const DwString& axisName, const DwString& unitName, float minVal, float maxVal);
    /// add a value track
    void AddTrack(const DwString& name, const DwString& color, const DwVector<float>& values);
    /// add a value track with timestamps
    void AddTrack(const DwString& name, const DwString& color, const DwVector<float>& values, const DwVector<DWORD>& timeStamps);
    /// draw diagram
    void Draw();

private:
    struct Track
    {
        DwString name;
        DwString color;
        DwVector<float> values;
        DwVector<DWORD> times;
    };

    DwString xAxisName;
    DwString xAxisUnitName;
    int xAxisMinVal;
    int xAxisMaxVal;
    DwString yAxisName;
    DwString yAxisUnitName;
    float yAxisMinVal;
    float yAxisMaxVal;
    DwVector<Track> tracks;
};// end of DwSvgLineChartWriter