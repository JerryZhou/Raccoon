#include "stable.h"
#include "dwninepatch.h"
#include "dwcore/dwvector.h"
#include "dwcore/dwmap.h"
#include "dwcore/dwglobalstatic.h"

//////////////////////////////////////////////////////////////////////////
const DwColor KSubsColor = DwColor(162, 0, 162, 255);
const DwColor KMarginsColor = DwColor(255, 0, 255, 255);
typedef DwVector<DwNinePatch::NinePatchInfo> NinePatchInfos;
typedef DwMap<int, NinePatchInfos > NinePatchInfosContainer;
struct NinePatchContainer 
{
    NinePatchInfosContainer container;

    /// if the img in setup
    bool has(const DwImage& img)const
    {
        return container.contains(img.uniqueKey());
    }

    /// if the img is a nine
    bool isNinePaths(const DwImage& img) 
    {
        NinePatchInfos infos = setup(img);
        return infos.size() > 1 || !(infos.at(0).margins.isZero());
    }

    /// cacl the img's nine paths info
    NinePatchInfos setup(const DwImage& img)
    {
        if (!img.isNull())
        {
            NinePatchInfosContainer::const_iterator ite = container.find(img.uniqueKey());
            if (ite != container.end())
            {
                return (ite->second);
            }

            NinePatchInfos infos;

            DwSize size = img.size();
            DwRect srcRect(0, 0, size.width(), size.height());
            DwMargins margins(0, 0, 0, 0);

            int cur = 0; int last = 0;
            for (cur=0; cur<size.width(); ++cur)
            {
                DwColor c = img.pixel(cur, 0);
                if (c == KSubsColor || cur == size.width()-1)
                {
                    // the last one need to be ++
                    if (cur == size.width()-1)
                    {
                        ++cur;
                    }

                    DwNinePatch::NinePatchInfo info;

                    if (margins.right())
                    {
                        margins.setRight(cur-margins.right()-1);
                    }

                    srcRect.setRect(last, 0, cur-last, size.height());
                    for (int j=0; j<size.height(); ++j)
                    {
                        DwColor c = img.pixel(last, j);
                        if (c==KMarginsColor)
                        {
                            if (!margins.top())
                            {
                                margins.setTop(j);
                            }
                            else
                            {
                                margins.setBottom(size.height() - j - 1);
                            }
                        }
                    }
                    info.srcRect = srcRect;
                    info.margins = margins;
                    last = cur+1;

                    margins = DwMargins(0, 0, 0, 0);
                    infos.append(info);
                }
                else if (c == KMarginsColor)
                {
                    if (!margins.left())
                    {
                        margins.setLeft(cur-last);
                    }
                    else
                    {
                        margins.setRight(cur);
                    }
                }
            }

            container.insert(img.uniqueKey(), infos);
            return infos;
        }
        return NinePatchInfos();
    }

    /// discard nine path info
    void discard(const DwImage& img)
    {
        discard(img.uniqueKey());
    }

    /// discard nine path info
    void discard(int uniqueKey)
    {
        container.remove(uniqueKey);
    }
};
DW_GLOBAL_STATIC(NinePatchContainer, _dw_NinePathContainer);

//------------------------------------------------------------------------------
/**
*/
static DwRect _rectOfSub(const DwImage &i, const NinePatchInfos &infos, int idx)
{
    int sidx = idx < infos.size() ? idx : infos.size() - 1;
    if (sidx >= 0)
    {
        return infos.at(sidx).srcRect;
    }
    return DwRect(DwPoint(0, 0), i.size());
}

//------------------------------------------------------------------------------
/**
*/
static DwMargins _marginsOfSub(const DwImage &i, const NinePatchInfos &infos, int idx)
{
    DW_UNUSED(i);
    int sidx = idx < infos.size() ? idx : infos.size() - 1;
    if (sidx >= 0)
    {
        return infos.at(sidx).margins;
    }
    return DwMargins(0, 0, 0, 0);
}

//------------------------------------------------------------------------------
/**
*/
DwNinePatch::NinePatchInfo DwNinePatch::ninePatchInfo(const DwImage &image, int idx)
{
    NinePatchInfos infos = _dw_NinePathContainer()->setup(image);
    int sidx = idx < infos.size() ? idx : infos.size() - 1;
    if (sidx >= 0)
    {
        return infos.at(sidx);
    }

    NinePatchInfo info;
    info.srcRect = DwRect(DwPoint(0, 0), image.size());
    info.margins = DwMargins(0, 0, 0, 0);
    return info;
}

//------------------------------------------------------------------------------
/**
*/
DwNinePatch::NinePatchInfos DwNinePatch::ninePatchInfos(const DwImage &image)
{
    return _dw_NinePathContainer()->setup(image);
}

//------------------------------------------------------------------------------
/**
*/
bool DwNinePatch::isValid(const DwImage &image, int idx)
{
    NinePatchInfos infos = _dw_NinePathContainer()->setup(image);
    return idx >= 0 && idx < infos.size() && !infos.at(idx).srcRect.isEmpty();
}

//------------------------------------------------------------------------------
/**
*/
bool DwNinePatch::isNinePatchs( const DwImage & i)
{
    return _dw_NinePathContainer()->isNinePaths(i);
}

//------------------------------------------------------------------------------
/**
*/
int DwNinePatch::numOfSubs( const DwImage & i)
{
    return _dw_NinePathContainer()->setup(i).size();
}

//------------------------------------------------------------------------------
/**
    if idx greater than the num of subs, will use the last valiable sub
*/
DwRect DwNinePatch::rectOfSub( const DwImage &i, int idx )
{
    NinePatchInfos infos = _dw_NinePathContainer()->setup(i);
    return _rectOfSub(i, infos, idx);
}

//------------------------------------------------------------------------------
/**
    if idx greater than the num of subs, will use the last valiable sub
*/
DwMargins DwNinePatch::marginsOfSub( const DwImage &i, int idx )
{
    NinePatchInfos infos = _dw_NinePathContainer()->setup(i);
    return _marginsOfSub(i, infos, idx);
}

//------------------------------------------------------------------------------
/**
*/
void DwNinePatch::releaseImage( const DwImage & i)
{
    releaseImage(i.uniqueKey());
}

//------------------------------------------------------------------------------
/**
*/
void DwNinePatch::releaseImage(int uniqueKey)
{
    _dw_NinePathContainer()->discard(uniqueKey);
}

//------------------------------------------------------------------------------
/**
*/
void DwNinePatch::setupImage( const DwImage & i)
{
    _dw_NinePathContainer()->setup(i);
}
