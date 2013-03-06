require("script:ui/controls/button")

local mainWindow = FrameWindow
{
	size = {380, 280},
	
	onItemCreate = function(self, evt)
		self:d_setAttribute(Dw.WA_ResizeAble, false)
	end,
	
	childrens = function(p)
		local content = RowPositioner
		{
			parent = p.background,
			
			childrens = function(p)
				p.m_span0 = GraphicsPixmapItem{ pixmap = "home:testdata/mu/4.jpg",}
				p.m_span1 = GraphicsPixmapItem{ pixmap = "home:testdata/mu/5.jpg",}
				p.m_span2 = GraphicsPixmapItem{ pixmap = "home:testdata/mu/6.jpg",}
				p.m_span3 = GraphicsPixmapItem{ pixmap = "home:testdata/mu/7.jpg",}
			end,
		}
		
		RowPositioner
		{
			spacing = 5,
			
			childrens = function(p)
				p.m_tab0 = Button 
				{
					m_idx = 0,
					
					childrens = function(p)
						p:d_setText("tab0")
					end,
				}
				
				p.m_tab1 = Button 
				{
					m_idx = 1,
					
					childrens = function(p)
						p:d_setText("tab1")
					end,
				}
				
				p.m_tab2 = Button 
				{
					m_idx = 2,
					
					childrens = function(p)
						p:d_setText("tab2")
					end,
				}
				
				p.m_tab3 = Button 
				{
					m_idx = 3,
					
					childrens = function(p)
						p:d_setText("tab3")
					end,
				}
				
				local tabClickedFunction = function(item, evt)
					content:d_clearAnimations()
					local anim = Animation.addAnimation(content, "x", content.x, -(380*item.m_idx), 1000, Dw.InOutQuad)
					Animation.startAni(anim)
				end
				
				p.m_tab0.onMouseClicked = tabClickedFunction
				p.m_tab1.onMouseClicked = tabClickedFunction
				p.m_tab2.onMouseClicked = tabClickedFunction
				p.m_tab3.onMouseClicked = tabClickedFunction
				
				local anchors = p:d_anchors()
				anchors:d_setBottomMargin(15)
				anchors:d_setBottom(p.parent, Dw.Bottom)
				anchors:d_setHCenter(p.parent, Dw.HCenter)
			end,
		}
		
		p.m_closedBtn = ClosedButton()
	end,
}

mainWindow:d_show()