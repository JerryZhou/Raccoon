-----------------------------------------------------------------------------
-- UI/1.1 Direct UI Of Raccoon
-- UI Script FrameWork toolkit.
-- Author: Jeffery Zhou
-- RCS ID: $Id: anchorcom.lua,v 1.70 2012/05/24 10:08:40 duowan.Inc $
-----------------------------------------------------------------------------

function AnchorLine(p, l)
    return Anchor.AnchorLine(p, l)
end

function GET_ANCHOR_PROXY(item)
    return Anchor._anchor_proxy(item)
end

--[[ item, left, leftMargin, top, topMargin]]
function anc_setLeftTop(target, l, lm, t, tm)
	local anc = target:d_anchors()
	anc:d_setLeftMargin(lm)
	anc:d_setLeft(l, Dw.Left)
	anc:d_setTopMargin(tm)
	anc:d_setTop(t, Dw.Top)
end

--[[ item, left, leftMargin, top, topMargin]]
function anc_setRightTop(target, r, rm, t, tm)
	local anc = target:d_anchors()
	anc:d_setRightMargin(rm)
	anc:d_setRight(r, Dw.Right)
	anc:d_setTopMargin(tm)
	anc:d_setTop(t, Dw.Top)
end

--[[ item, left, leftMargin, right, rightMargin]]
function anc_setLeftRight(target, l, lm, r, rm)
	local anc = target:d_anchors()
	anc:d_setLeftMargin(lm)
	anc:d_setLeft(l, Dw.Left)
	anc:d_setRightMargin(rm)
	anc:d_setRight(r, Dw.Right)
end

--[[ item, left, leftMargin, right, rightMargin]]
function anc_setTopBottom(target, t, tm, b, bm)
	local anc = target:d_anchors()
	anc:d_setTopMargin(tm)
	anc:d_setTop(t, Dw.Top)
	anc:d_setBottomMargin(bm)
	anc:d_setBottom(b, Dw.Bottom)
end

--[[ item, leftMargin, topMargin, rightMargin, bottomMargin]]
function anc_setMargins(target, lm, tm, rm, bm)
	local anc = target:d_anchors()
	anc:d_setLeftMargin(lm)
	anc:d_setTopMargin(tm)
	anc:d_setRightMargin(rm)
	anc:d_setBottomMargin(rm)
end