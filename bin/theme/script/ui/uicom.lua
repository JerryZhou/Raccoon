-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script FrameWork toolkit.
-- Author: Jeffery Zhou
-- RCS ID: $Id: uicom.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------
function GET_GUI_PROXY(id)
    return ui._ui_widget_get_proxy(id)
end

function GET_GUI_ID(p)
    return ui._ui_widget_get_id(p)
end

function GUI_CLEAR_PROXY_OF(id)
    if SigSlotDisconnectAll then
        SigSlotDisconnectAll(GET_GUI_PROXY(id))
    end
    ui._ui_widget_unRegister_of_id(id)
end

function AddProperty(p, name, setter, getter)
    ui._ui_add_property(p, name, setter, getter)
end

function DestroyItem(p)
    ui._ui_widget_destroy_of_p(p)
end

function GraphicsItemId(p)
    return ui._ui_widget_get_id(p)
end

function GraphicsItem(...)
    return ui._ui_widget_create("GraphicsItem", ...)
end

function GraphicsRectItem(...)
    return ui._ui_widget_create("GraphicsRectItem", ...)
end

function GraphicsSimpleTextItem(...)
    return ui._ui_widget_create("GraphicsSimpleTextItem", ...)
end

function GraphicsPixmapItem(...)
    return ui._ui_widget_create("GraphicsPixmapItem", ...)
end

function GraphicsFrameItem(...)
    return ui._ui_widget_create("GraphicsFrameItem", ...)
end

function GridPositioner(...)
    return ui._ui_widget_create("GridPositioner", ...)
end

function RowPositioner(...)
    return ui._ui_widget_create("RowPositioner", ...)
end

function ColumnPositioner(...)
    return ui._ui_widget_create("ColumnPositioner", ...)
end

function WidgetItem(...)
    return ui._ui_widget_create("WidgetItem", ...)
end

function WidgetItemOf(t, ...)
    return ui._ui_widget_create(t, ...)
end

function Window(...)
    return ui._ui_widget_create("MainWindow", ...)
end

function FrameWindow(...)
    return ui._ui_widget_create("FrameWindow", ...)
end