#include "stable.h"
#include "picViewer.h"
#include "dwcore/dwpoint.h"
#include "dwcore/dwstring.h"
#include "dwgui/dwimagewidget.h"
#include "dwgui/dwpropertyanimation.h"
#include "dwgui/dwframewindow.h"
#include "dwgui/dwpushbutton.h"
#include "dwgui/dwscrollarea.h"
#include "dwgui/dwscrollbar.h"
#include "dwcore/sigslot.h"
#include "dwgui/dwanchors.h"
#include "dwgui/dwgridpositioner.h"
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwgui/dwminidump.h"
#include "dwgui/dwtitlebar.h"
#include "dwgui/dwlabel.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwcursor.h"
#include "dwgui/dwdesktop.h"
#include "dwgui/dwfswrapper.h"
#include "dwgui/dwgui.h"
#include "dwcore/dwdir.h"

//------------------------------------------------------------------------------
class AnimationTestSlots : public sigslot::has_slots
{
public:
    AnimationTestSlots();

    void onDropFiles(DwRttiObject *item, DwEvent *evt);
    void onMouseWheel(DwRttiObject *item, DwEvent *evt);
    void onMouseClicked(DwRttiObject *item, DwEvent *evt);
    void onDragStart(DwRttiObject *item, DwEvent *evt);
    void onDragEnd(DwRttiObject *item, DwEvent *evt);
    void onWindowMove(DwRttiObject *item, DwEvent *evt);
    void onKeyEvent(DwRttiObject *item, DwEvent *evt);

    bool scaleWindow(DwFrameWindow *window, int up);
    bool updateLabel(DwFrameWindow *window);
    bool isSupportExt(const DwString &ext);
    
    DwVector<DwString > m_exts;
    bool m_windowMoved;
    
    DwString m_path;
    DwVector<DwString> m_files;
    int m_curFileIndex;

    void dragPicToV(const DwUrl &url)
    {
        m_curFileIndex = 0;
        m_path = url.localPath();
        DwDir dir(m_path);
        dir.cdUp();
        m_path = dir.path();
        m_files = DwFsWrapper::listFiles(m_path, L"*.*");
        DwString curFileName = url.localPath().right(url.localPath().length() - m_path.length() - 1);
        for (int i=0; i<m_files.size(); ++i)
        {
            if (m_files[i] == curFileName)
            {
                m_curFileIndex = i;
                break;
            }
        }

        DW_UI_LOGER() << "current path: " << m_path << "\n";
        DW_UI_LOGER() << "current path files: " << m_files << "\n";
    }

    DwString nextValidFile()
    {
        ++m_curFileIndex;
        while (m_files.count() > 1)
        {
            m_curFileIndex = m_curFileIndex % m_files.count();
            DwString fileName = m_files.at(m_curFileIndex);
            int extIndex = fileName.lastIndexOf(L'.');
            if (extIndex != -1)
            {
                DwString ext = fileName.right(fileName.length() - extIndex - 1).toLower();
                if (isSupportExt(ext))
                {
                    return m_path + L"/" + fileName;
                }
                else
                {
                    m_files.remove(m_curFileIndex);
                }
            }
            else
            {
                m_files.remove(m_curFileIndex);
            }
        }

        if (m_files.count() > 0)
        {
            return m_path + L"/" + m_files[0];
        }
        
        return DwString();
    }

    DwString preValidFile()
    {
        --m_curFileIndex;
        if (m_curFileIndex < 0)
        {
            m_curFileIndex += m_files.count();
        }
        while (m_files.count() > 1)
        {
            m_curFileIndex = m_curFileIndex % m_files.count();
            DwString fileName = m_files.at(m_curFileIndex);
            int extIndex = fileName.lastIndexOf(L'.');
            if (extIndex != -1)
            {
                DwString ext = fileName.right(fileName.length() - extIndex - 1).toLower();
                if (isSupportExt(ext))
                {
                    return m_path + L"/" + fileName;
                }
                else
                {
                    m_files.remove(m_curFileIndex);
                }
            }
            else
            {
                m_files.remove(m_curFileIndex);
            }

            --m_curFileIndex;
            if (m_curFileIndex < 0)
            {
                m_curFileIndex += m_files.count();
            }
        }

        if (m_files.count() > 0)
        {
            return m_path + L"/" + m_files[0];
        }

        return DwString();
    }
private:
};

#include <algorithm>
AnimationTestSlots::AnimationTestSlots()
: m_windowMoved(false)
, m_curFileIndex(0)
{
    m_exts.append(L"jpg");
    m_exts.append(L"png");
    m_exts.append(L"bmp");
    m_exts.append(L"gif");
    m_exts.append(L"jpeg");
    m_exts.append(L"exif");
    m_exts.append(L"tiff");
    m_exts.append(L"icon");

    std::sort(m_exts.begin(), m_exts.end());
    DW_UI_LOGER() << "support ext: " << m_exts << "\n";
}

bool AnimationTestSlots::isSupportExt(const DwString &ext)
{
    return std::binary_search(m_exts.begin(), m_exts.end(), ext);
}

DwLabel* dwScaleLabel = NULL;
bool AnimationTestSlots::scaleWindow(DwFrameWindow *window, int up)
{
    const DwImage &image = window->background()->pixmap();
    if (image.isNull())
    {
        return false;
    }
    dwScaleLabel->setVisible(true);
    DwSizeF size = window->size() ;
    if (up > 0)
    {
        size += image.size() * 0.1;
    }
    else if(up < 0)
    {
        size -= image.size() * 0.1;
    }
    else 
    {
        size = image.size();
    }

    DwSizeF newSize = size;
    DwSize desktopSize = DwDesktop::desktopWorkArea().size();
    if (newSize.width() > desktopSize.width() || newSize.height() > desktopSize.height())
    {
        newSize.scale(desktopSize, Dw::KeepAspectRatioByExpanding);
    }
    size = newSize;

    window->setSize(size);
    window->setFocus();
    window->update();
    return true;
}

bool AnimationTestSlots::updateLabel(DwFrameWindow *window)
{
    const DwImage &image = window->background()->pixmap();
    if (image.isNull())
    {
        return false;
    }

    DwSizeF size = window->size() ;
    DwReal ratio = (DwReal)(size.width()  + 1) / image.size().width();
    int percent = (int)(ratio * 100);
    DwString text = DwString::number(percent);
    text.append(L"%");
    DwRectF bound = dwScaleLabel->text()->font().MeasureString(text);
    dwScaleLabel->setText(text);
    dwScaleLabel->setBoundingRect(bound);

    DwPropertyAnimation* animation1 = new DwPropertyAnimation(NULL);
    animation1->setTargetItem(dwScaleLabel);
    animation1->setProperty("opacity");
    animation1->setStartValue(1.0);
    animation1->setEndValue(0.0);
    animation1->setDuration(1000);
    animation1->setEasingCurve(DwEasingCurve(Dw::InExpo));
    animation1->start(Dw::DeleteWhenStopped);
    return true;
}

AnimationTestSlots slots;
//------------------------------------------------------------------------------
/**
*/
static void terminateAtIndex(DwString &str, int pos)
{
    str.resize(pos);
}

//------------------------------------------------------------------------------
/**
*/
static int lastSlashIndex(DwString &str, int from = -1)
{
    int index = str.lastIndexOf(L'/', from);
    if(index == -1) index = str.lastIndexOf(L'\\', from);
    if(index == -1) index = str.lastIndexOf(L':', from);
    return index;
}

// Show Image
//////////////////////////////////////////////////////////////////////////
void AnimationTestSlots::onDropFiles(DwRttiObject *item, DwEvent *evt)
{
    DW_ASSERT(item->isA(DwFrameWindow::RTTI()));
    DW_ASSERT(evt->isA(DwDragDropEvent::EventId()));
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(item);
    DwDragDropEvent *dropEvt = dwsafe_cast<DwDragDropEvent>(evt);
    if (window && dropEvt)
    {
        if( dropEvt->m_mimeData.hasFormat("urls", "files"))
        {
            const DwMimeData::MimeUrls *urls = dropEvt->m_mimeData.urls();
            for (int i=0; i<urls->size(); ++i)
            {
                const DwUrl& url = urls->at(i);
                DwString localPath = url.localPath();
                int index = localPath.lastIndexOf(L'/');
                if (index != -1)
                {
                    DwString fileName = localPath.right(localPath.length() - index - 1);
                    int extIndex = fileName.lastIndexOf(L'.');
                    if (extIndex != -1)
                    {
                        DwString ext = fileName.right(fileName.length() - extIndex - 1).toLower();
                        if (slots.isSupportExt(ext))
                        {
                            showImageInViewer(window, url);
                            dragPicToV(url);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void AnimationTestSlots::onMouseWheel(DwRttiObject *item, DwEvent *evt)
{
    DW_ASSERT(item->isA(DwFrameWindow::RTTI()));
    DW_ASSERT(evt->isA(DwMouseWheelEvent::EventId()));
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(item);
    DwMouseWheelEvent *wheelEvt = dwsafe_cast<DwMouseWheelEvent>(evt);
    if (window && wheelEvt)
    {
        slots.scaleWindow(window, wheelEvt->isRollUp() ? 1 : -1);
        slots.updateLabel(window);
    }
}

void AnimationTestSlots::onMouseClicked(DwRttiObject *item, DwEvent *evt)
{
    DW_ASSERT(item->isA(DwFrameWindow::RTTI()));
    DW_ASSERT(evt->isA(DwMouseClickedEvent::EventId()));
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(item);
    DwMouseClickedEvent *clickedEvt = dwsafe_cast<DwMouseClickedEvent>(evt);
    if (window && clickedEvt 
        && clickedEvt->m_button == Dw::LeftButton 
        && (!clickedEvt->m_isNC || clickedEvt->m_nc == HTCAPTION)
        && !m_windowMoved)
    {
        window->close();
    }
}

void AnimationTestSlots::onDragStart(DwRttiObject *, DwEvent *)
{
    DwCursor::save();
    DwCursor::setCursor(Dw::PointingHandCursor);

    m_windowMoved = false;
}

void AnimationTestSlots::onDragEnd(DwRttiObject *, DwEvent *)
{
    DwCursor::restore();

    m_windowMoved = false;
}

void AnimationTestSlots::onWindowMove(DwRttiObject *, DwEvent *)
{
    m_windowMoved = true;
}

void AnimationTestSlots::onKeyEvent(DwRttiObject *item, DwEvent *evt)
{
    DwKeyUpEvent *keyEvt = dwsafe_cast<DwKeyUpEvent>(evt);
    DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(item);
    if (keyEvt && window)
    {
        if (keyEvt->m_virtualKey == VK_ESCAPE)
        {
            window->close();
        }
        else if (keyEvt->m_virtualKey == VK_RIGHT )
        {
            slots.scaleWindow(window, 1);
            slots.updateLabel(window);
        }
        else if (keyEvt->m_virtualKey == VK_LEFT)
        {
            slots.scaleWindow(window, -1);
            slots.updateLabel(window);
        }
        else if (keyEvt->m_virtualKey == VK_UP )
        {
            DwString file = preValidFile();
            if (!file.isEmpty())
            {
                showImageInViewer(window, file);
            }
        }
        else if (keyEvt->m_virtualKey == VK_DOWN )
        {
            DwString file = nextValidFile();
            if (!file.isEmpty())
            {
                showImageInViewer(window, file);
            }
        }
    }
    evt->accept();
}

bool showImage(DwFrameWindow *window, const DwCommandLineArgs &args)
{
    window->setFocus();
    window->setFocusPolicy(Dw::StrongFocus);
    window->setAcceptedMouseButton(Dw::RightButton, true);
    window->titleBar()->setVisible(false);
    window->setBackgroundColor(DwColor(102, 102, 102, 102));
    window->setBackgroundImage(L"");
    window->setAcceptDrops(true);
    window->setHoverable(false);
    window->background()->setHover(false);
    window->signalDispatch()->connect(DwDragDropEvent::EventId(), &slots, &AnimationTestSlots::onDropFiles);
    window->signalDispatch()->connect(DwMouseWheelEvent::EventId(), &slots, &AnimationTestSlots::onMouseWheel);
    window->signalDispatch()->connect(DwMouseClickedEvent::EventId(), &slots, &AnimationTestSlots::onMouseClicked);
    window->signalDispatch()->connect(DwDragStartEvent::EventId(), &slots, &AnimationTestSlots::onDragStart);
    window->signalDispatch()->connect(DwDragEndEvent::EventId(), &slots, &AnimationTestSlots::onDragEnd);
    window->signalDispatch()->connect(DwMoveEvent::EventId(), &slots, &AnimationTestSlots::onWindowMove);
    window->signalDispatch()->connect(DwKeyUpEvent::EventId(), &slots, &AnimationTestSlots::onKeyEvent);
    
    dwScaleLabel = new DwLabel(window);
    dwScaleLabel->setVisible(false);
    dwScaleLabel->setFocusProxy(window);
    dwScaleLabel->anchors()->setCenterIn(window);
    dwScaleLabel->setDragPolicy(DwWidget::DragWindow);
    dwScaleLabel->setEventHandler(NULL);
    dwScaleLabel->setFlag(Dw::ItemStopsMousePropagation, false);
    dwScaleLabel->background()->setURL(L"dwgui:icon_background_pressdown.png");
    
    DwFont font = dwScaleLabel->text()->font();
    font.setSize(24);
    font.setStyle(Dw::FontStyleBold);
    font.setColor(DwColor(254, 254, 254));
    dwScaleLabel->text()->setFont(font);

    if (args.hasArg(L"-img"))
    {
        showImageInViewer(window, args.stringOf("-img"));
    }
    
    window->show();
    return true;
}

bool showImageInViewer(DwFrameWindow *window, const DwUrl& url)
{
    DwImage image = DwResourceInterface::acquireImage(url);
    if (!image.isNull())
    {
        window->background()->clearAnimations();
        window->setBackgroundImage(url);
        window->setFocus();
        slots.scaleWindow(window, 0);
        slots.updateLabel(window);

        DwPropertyAnimation* animation1 = new DwPropertyAnimation(NULL);
        animation1->setTargetItem(window->background());
        animation1->setProperty("opacity");
        animation1->setStartValue(0.0);
        animation1->setEndValue(1.0);
        animation1->setDuration(1000);
        animation1->setEasingCurve(DwEasingCurve(Dw::Linear));
        animation1->start(Dw::DeleteWhenStopped);

        DwImage::shrinkCaches(1);
    }
    
    return true;
}