#include "stable.h"
#include "simplewidgets.h"
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
#include "dwgui/dwgraphicsevent.h"
#include "dwgui/dweventsignaldispatch.h"
#include "dwgui/dwminidump.h"
#include "dwgui/dwgraphicssimpletextitem.h"
#include "dwgui/dwcursor.h"
#include "dwgui/dwprogressbar.h"
#include "dwgui/dwgui.h"

//------------------------------------------------------------------------------
/**
*/
DwString randImage()
{
    return DwString(L"home:testdata/graphics_image/") + DwString::number(dwRand()%20 + 10) + L".jpg";
}

//------------------------------------------------------------------------------
class AnimationTestSlots : public sigslot::has_slots
{
public:
    AnimationTestSlots();

    void onRestartCliked(DwRttiObject *item, DwEvent *evt);
    void onDumpCliked(DwRttiObject *item, DwEvent *evt);
    void onResizeCliked(DwRttiObject *item, DwEvent *evt);
    void onAnimation(DwRttiObject *item, DwEvent *evt);
    
    DwVector<DwString > m_exts;
    bool m_windowMoved;
    
    DwString m_path;
    DwVector<DwString> m_files;
    int m_curFileIndex;
private:
};

#include <algorithm>
AnimationTestSlots::AnimationTestSlots()
: m_windowMoved(false)
, m_curFileIndex(0)
{
    ;
}

AnimationTestSlots slots;
DwVector<DwImageWidget *> images;
DwProgressBar * bar = NULL;
//------------------------------------------------------------------------------
/**
*/
void animation_test(DwFrameWindow *window, const DwCommandLineArgs& )
{
	DwPushButton *restartButton = new DwPushButton(window);
    restartButton->setToolTips(L"Start A Opciaty Animation");
    restartButton->setTextOnly();
    restartButton->setText(RttiTr("Start"));
    restartButton->anchors()->setMargins(10);
	restartButton->anchors()->setRight(DwAnchorLine(window, Dw::Right));
    restartButton->anchors()->setBottom(DwAnchorLine(window, Dw::Bottom));
    restartButton->signalDispatch()->connect(DwMouseClickedEvent::EventId(), &slots, &AnimationTestSlots::onRestartCliked);

    DwPushButton *dumpButton = new DwPushButton(window);
    dumpButton->setToolTips(L"Dump All of Objects Information");
    dumpButton->setTextOnly();
    dumpButton->setText(RttiTr("Dump"));
    dumpButton->anchors()->setMargins(10);
    dumpButton->anchors()->setRight(DwAnchorLine(restartButton, Dw::Left));
    dumpButton->anchors()->setBottom(DwAnchorLine(window, Dw::Bottom));
    dumpButton->signalDispatch()->connect(DwMouseClickedEvent::EventId(), &slots, &AnimationTestSlots::onDumpCliked);

    DwPushButton *resizeButton = new DwPushButton(window);
    resizeButton->setToolTips(L"Width +1 And Height +1");
    resizeButton->setTextOnly();
    resizeButton->setText(RttiTr("suofang"));
    resizeButton->anchors()->setMargins(10);
    resizeButton->anchors()->setRight(DwAnchorLine(dumpButton, Dw::Left));
    resizeButton->anchors()->setBottom(DwAnchorLine(window, Dw::Bottom));
    resizeButton->signalDispatch()->connect(DwMouseClickedEvent::EventId(), &slots, &AnimationTestSlots::onResizeCliked);

    DwScrollArea *scrollArea = new DwScrollArea(window);
    scrollArea->anchors()->setMargins(10);
    scrollArea->anchors()->setTopMargin(80);
    scrollArea->anchors()->setBottomMargin(42);
    scrollArea->anchors()->setLeft(DwAnchorLine(window, Dw::Left));
    scrollArea->anchors()->setRight(DwAnchorLine(window, Dw::Right));
    scrollArea->anchors()->setBottom(DwAnchorLine(window, Dw::Bottom));
    scrollArea->anchors()->setTop(DwAnchorLine(window, Dw::Top));
    scrollArea->setAccessible(false);

    DwGraphicsItem *content = scrollArea;
	for (int i=0; i<9; ++i)
	{
		DwImageWidget *image = new DwImageWidget(content);
		image->setPos(DwPointF(40+dwRand()%460, 20 + dwRand()%280));
		image->setImage(randImage());
        image->setMovable(true);
		images.append(image);
	}
    {
        DwImageWidget *image = new DwImageWidget(content);
        image->setPos(DwPointF(40+dwRand()%460, 20 + dwRand()%280));
        image->setImage(randImage());
        image->setSize(DwSize(100, 100));
        image->setMovable(true);
        images.append(image);
    }

    bar = new DwProgressBar(window);
    bar->setHeight(26);
    bar->anchors()->setLeftMargin(10);
    bar->anchors()->setRightMargin(160);
    bar->anchors()->setLeft(DwAnchorLine(window, Dw::Left));
    bar->anchors()->setVerticalCenter(DwAnchorLine(restartButton, Dw::VCenter));
    bar->anchors()->setRight(DwAnchorLine(resizeButton, Dw::Left));
    images.at(0)->signalDispatch()->connect(DwAnimationProgressEvent::EventId(), &slots, &AnimationTestSlots::onAnimation);

    DwCheckBox *checkBox = new DwCheckBox(window);
    checkBox->anchors()->setLeft(bar, Dw::Right);
    checkBox->anchors()->setVerticalCenter(bar, Dw::VCenter);

    DwLineEdit *edit = new DwLineEdit(window);
    edit->anchors()->setTopMargin(40);
    edit->anchors()->setTop(window, Dw::Top);
    edit->anchors()->setHorizontalCenter(window, Dw::HCenter);
}

//------------------------------------------------------------------------------
/**
*/
void AnimationTestSlots::onRestartCliked(DwRttiObject *item, DwEvent *evt)
{
    DW_UNUSED(item);
    DW_UNUSED(evt);
    for (int i=0; i<images.size() && i < 10; ++i)
    {
        DwImageWidget *image = images.at(i);

        image->clearAnimations();
        DwPropertyAnimation* animation = new DwPropertyAnimation(NULL);
        animation->setTargetItem(image);
        animation->setProperty("pos");
        animation->setStartValue(image->pos());
        animation->setEndValue(DwPointF(40+dwRand()%460, 20 + dwRand()%240));
        animation->setDuration(2250);
        DwEasingCurve curve((Dw::Curve)(i%46));
        animation->setEasingCurve(curve);
        animation->start(Dw::DeleteWhenStopped);

        if (!i)
        {
            animation->setSendEvent(true);
        }
    }
}

void AnimationTestSlots::onDumpCliked(DwRttiObject *item, DwEvent *evt)
{
    DW_UNUSED(item);
    DW_UNUSED(evt);
    DwRttiObject::dumpObjects();
    DwMiniDump::writeMiniDump();
}

void AnimationTestSlots::onResizeCliked(DwRttiObject *item, DwEvent *evt)
{
    DW_UNUSED(item);
    DW_UNUSED(evt);
    DwPushButton *btn = dwsafe_cast<DwPushButton>(item);
    if (btn)
    {
        DwFrameWindow *window = dwsafe_cast<DwFrameWindow>(btn->parentItem());
        if (window)
        {
            window->setSize(window->size() + DwSize(1, 1));
        }
    }
}

void AnimationTestSlots::onAnimation(DwRttiObject *item, DwEvent *evt)
{
    DwImageWidget *w = dwsafe_cast<DwImageWidget>(item);
    DwAnimationProgressEvent *apEvt = dwsafe_cast<DwAnimationProgressEvent>(evt);
    if (w && apEvt)
    {
        bar->setCur(1.0f * apEvt->m_msecs/apEvt->animation()->duration());
    }
}