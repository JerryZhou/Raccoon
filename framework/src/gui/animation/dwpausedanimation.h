#include "dwgui/dwabstractanimation.h"

//------------------------------------------------------------------------------
class DW_GUI_EXPORT DwPausedAnimation : public DwAbstractAnimation
{
    DW_DECLARE_CLASS(DwPausedAnimation);
public:
    DwPausedAnimation(DwObject *parent = NULL);
    DwPausedAnimation(int msecs, DwObject *parent = NULL);
    virtual ~DwPausedAnimation();

protected:
    virtual void updateCurrentTime(int);
private:
};// end of DwPausedAnimation
DW_REGISTER_CLASS(DwPausedAnimation);