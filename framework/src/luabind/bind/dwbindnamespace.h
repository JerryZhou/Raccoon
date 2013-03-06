#pragma once

namespace Dw
{
enum CallExportWhen
{
    CallWhenItemCreate=0x0001,
    CallWhenItemDestroy=0x0002,
    CallWhenEngineCreate=0x0004,
    CallWhenEngineDestroy=0x0008,
};
}