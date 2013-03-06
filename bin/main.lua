-- must be no bom utf-8
require("script:ui/controls/button")

local _Template_Role_Info = 
{
	visible=true,
	childrens=function(self, evt)
		local head = GraphicsPixmapItem()
		local hAnchors = head:d_anchors()
		hAnchors:d_setLeftMargin(5)
		hAnchors:d_setLeft(self, Dw.Left)
		hAnchors:d_setVCenter(self, Dw.VCenter)
		
		local detail = GraphicsSimpleTextItem()
		local dAnchors = detail:d_anchors()
		dAnchors:d_setLeftMargin(5)
		dAnchors:d_setLeft(head, Dw.Right)
		dAnchors:d_setVCenter(self, Dw.VCenter)
		
		self.m_head = head
		self.m_detail = detail
	end,
}

local function _give_a_iocn()
	return "home:testdata/graphics_image/" .. math.random(10, 30) .. ".jpg"
end

local function _give_me_a_detail()
	local choose = math.random(1, 3)
	if choose == 1 then
		return "saw hello to \n everybody!!"
	elseif choose == 2 then
		return "wa haha,\n hooly shit, \n what a craple \ngigle baster!"
	else
		return "how could you"
	end
end

local function _add_role(icon, txt, p)
	local role = RowPositioner(_Template_Role_Info)
	role.m_head.pixmap = icon
	role.m_detail.text = txt
	if p then role.parent = p end
	return role
end

local _Template_Sub_TextItem = 
{
	eventHandler = "DefaultItemEventHandler",
	d_setSubText = function(self, txt)
		self.text = txt
	end,
	d_subText = function(self)
		return self.text
	end,

	onItemCreate=function(self, evt)
		AddProperty(self, "subText", self.d_setSubText, self.d_subText)
	end,
}

local mainWindow = FrameWindow
{
	width=290,
	childrens=function(self)
		self:d_setAttribute(Dw.WA_ResizeAble, false)
		
		local win_content = WidgetItemOf("ScrollArea", {
			rect = {0, 0, 200, 200}, dragPolicy = Dw.DragWindow,
			
			onItemCreate=function(self, evt)
				local anchors = self:d_anchors()
				anchors:d_setHCenter(self.parent, Dw.HCenter)
				anchors:d_setTop(self.parent, Dw.Top)
				anchors:d_setBottom(self.parent, Dw.Bottom)
				
				self.content = ColumnPositioner
				{
					spacing=6,
					childrens=function(self, evt)
						for i=0, 2 do
							_add_role(_give_a_iocn(), _give_me_a_detail())
						end
					end,
				}
			end,
		})
		
		self.m_content = win_content.content	
		ClosedButton()
	end,
	
	onMouseClicked=function(self, evt)
		_add_role(_give_a_iocn(), _give_me_a_detail(), self.m_content)
	end,
	
	onKeyDown=function(self, evt)
		if self.m_content.childCount > 0 then
			self.m_content:d_childAt(0):d_deleteLater()
		end
	end,
}
--mainWindow:d_show()

function showMainWindow()
    mainWindow:d_show()
end

local urls = 
{
	"http://news.longhoo.net/attachement/jpg/site2/20110105/002481cc2d930e8dcd7629.jpg",
	"http://pic1a.nipic.com/2008-10-27/20081027213034357_2.jpg",
	"http://pic1a.nipic.com/2009-02-10/200921075919155_2.jpg",
	"http://www.dongping.com.cn/up_files/image/Article/2011/03/11/61244051.jpg",
	"http://img.ivsky.com/img/bizhi/img/201102/23/guozi_mm-015.jpg",
	"http://2.zol-img.com.cn/product/62_450x337/699/ceRxKg6PvNX7A.jpg",
	"http://pic6.nipic.com/20100326/3670524_085325048325_2.jpg",
	"http://titanimg.titan24.com/cartoon/2010/04/29/1e279baf67_1272520362.jpg",
	"http://i2.3conline.com/images/piclib/201008/11/batch/1/66731/1281495450529931tbg73fc.jpg",
}

local idx = 1
local getURL = function(uidx)
	if (not uidx) then
		local idxNum = #urls
		idx = idx + 1
		if (idx > idxNum) then
			idx = 1
		end
		return urls[idx]
	end
	return urls[uidx]
end

local win2 = FrameWindow
{
	width = 400,
    backgroundImage = getURL(1),

	onItemCreate=function(self, evt)      
		local subTextItem = GraphicsSimpleTextItem(_Template_Sub_TextItem, 
        {
			y = 100,
            subText="hello World!!"
        })
		self.m_tick = GraphicsSimpleTextItem(_Template_Sub_TextItem, 
        { 
            subText = subTextItem.subText .. "Here we go", 
            y = 300
        })

		Button 
		{
			textOnly = true,
			text = "Change",
			x = 2,
			y = 100,
			
			onMouseClicked=function(self, evt)
				--self.parent.m_targetPixmap.url = getURL()
                self.parent.m_progress.cur = 0.0
				self.parent.backgroundImage = getURL()
                self.parent:d_updateWinSize()
			end,
		}

		local h = Button 
		{
			text = "Quit",
			icon = "home:testdata/mu/aio_toobar_game.png",
			x = 2,
			y = 200,
			height = 40,
            onMouseClicked=function(self, evt)
                app.quit()
            end,
		}
		h:d_adjustSize()

		ClosedButton()
        
        --local dat = self:d_acquireData("http://channel.mobile.yy.com/commend_channel/recommendcell.php?cid=1", true)
        --local dat = self:d_acquireData("http://yydl.duowan.com/4/setup/YYSetup-5.4.0.2-zh-CN.exe", true)
        self.m_progress = WidgetItemOf("ProgressBar", {
            width = 200, y = 20,
			onItemCreate=function(self, evt)
				local anc = self:d_anchors()
				anc:d_setHCenter(self.parent, Anchor.HCenter)
			end,
        })
		
		self.m_inputUrl = WidgetItemOf("LineEdit", {
			width = 200, y = 50,
			placeHolderText = "Input Window Img URL!!",
			onItemCreate=function(self, evt)
				local anc = self:d_anchors()
				anc:d_setHCenter(self.parent, Anchor.HCenter)
			end,
		})
		Button
		{
			text = "Get",
			onItemCreate=function(self, evt)
				local anc = self:d_anchors()
				anc:d_setVCenter(self.parent.m_inputUrl, Anchor.VCenter)
				anc:d_setLeftMargin(10)
				anc:d_setLeft(self.parent.m_inputUrl, Anchor.Right)
			end,
			onMouseClicked=function(self, evt)
                local uri = self.parent.m_inputUrl.text
				self.parent.backgroundImage = uri
            end,
		}
		
		local Extent = Button
		{
			text = "Extent",
			onItemCreate=function(self, evt)
				local anc = self:d_anchors()
				anc:d_setBottom(self.parent, Anchor.Bottom)
				anc:d_setRight(self.parent, Anchor.Right)
			end,
			onMouseClicked=function(self, evt)
				if mainWindow:d_isValid() then
					local ans = Animation.addAnimation(mainWindow, "width", 0, 290, 200)
					Animation.startAni(ans)
				end
			end,
		}
		Button
		{
			text = "UnExtent",
			onItemCreate=function(self, evt)
				local anc = self:d_anchors()
				anc:d_setBottom(Extent, Anchor.Top)
				anc:d_setRight(self.parent, Anchor.Right)
			end,
			onMouseClicked=function(self, evt)
				if mainWindow:d_isValid() then
					local ans = Animation.addAnimation(mainWindow, "width", 290, 0, 200)
					Animation.startAni(ans)
				end
			end,
		}
	end,

	d_doResizeBuddy=function(self)
		if mainWindow:d_isValid() then
			mainWindow.x = self.x + self.width
			mainWindow.y = self.y
		end
	end,

	onMove=function(self, evt)
		self:d_doResizeBuddy()
	end,

	onResize=function(self, evt)
		self:d_doResizeBuddy()
	end,
    
    onSceneImageLoadFinised=function(self, evt)
        self:d_updateWinSize()
    end,
    
    onSceneDataLoadFinised=function(self, evt)
        --local dat = self:d_acquireData("http://channel.mobile.yy.com/commend_channel/recommendcell.php?cid=1", true)
        --local j = dat
    end,
    
    onSceneResourceLoadProgress=function(self, evt)
        local progress = Event.invoke(evt, "vars", "percent")
		local uri = Event.invoke(evt, "vars", "url")
        self.m_tick.text = "download progress: " .. tostring(progress)
		--		.. "\nURL: " .. uri
        
        self.m_progress.cur = progress
    end,
    
    d_updateWinSize=function(self)
        local img = self.background.pixmap
        local imgSize = img:d_size()
        if imgSize.width > 0 and imgSize.height > 0 then
            self.size = { imgSize.width + 12, imgSize.height + 12 }
        end
    end,
}
--win2:d_show()

function showWindow2()
    win2:d_show()
end