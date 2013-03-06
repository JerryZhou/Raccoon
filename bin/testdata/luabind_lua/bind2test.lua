require("script:ui/controls/button")
require("script:ui/controls/checkbox")

local Template_Rect_Actor = 
{
    boundingRect = {-15, -15, 30, 30},
    color = {255, 0, 0},
    
    m_speed = 100,
    m_direction = {0, 0, 0},
    m_target = {100, 100},
    
    d_seek = function(self, target)
        local selfLoc = self.pos        
        local targetLoc = target.pos
        local direction = Point.sub(targetLoc , selfLoc)
        direction = Point.normalize(direction)
        local mixDirection = Point.add(direction, self.m_direction)
        self.m_direction = Point.normalize(mixDirection)
        self.m_target = target.pos
    end,
    
    d_flee = function(self, target)
        local selfLoc = self.pos        
        local targetLoc = target.pos
        local direction = Point.sub(selfLoc , targetLoc)
        direction = Point.normalize(direction)
        local mixDirection = Point.add(direction, self.m_direction)
        self.m_direction = Point.normalize(mixDirection)
        self.m_target = target.pos
    end,
    
    d_wander = function(self)
        local direction = {math.random(), math.random()}
        direction = Point.normalize(direction)
        local mixDirection = Point.add(direction, self.m_direction)
        self.m_direction = Point.normalize(self.m_direction)
    end,

    d_advance = function(self, delta)
        self.m_speed = self.m_speed + 500 * delta/1000
        if self.m_speed > 100 then
            self.m_speed = 100
        end
        
        if self.m_speed > 0.001 then
            local distanceMax = Point.length(Point.sub(self.pos, self.m_target))
            local distance = self.m_speed*delta/1000
            if distance > distanceMax then
                distance = distanceMax
                self.m_speed = 0
            end
            local moveDistance = Point.scale(self.m_direction, distance)
            local newPos = Point.add(self.pos , moveDistance)
            self.pos = newPos
        end
    end,
    
    d_bound = function(self)
        if self.x < 85 then
            self.x = 85
        end
        if self.x > (640 - 85) then
            self.x = (640 - 85)
        end
        if self.y < 85 then
            self.y = 85
        end
        if self.y > (480 - 85) then
            self.y = (480 - 85)
        end
    end, 
}

local loadLib = function(url, entry)
    local path = Assign.localPath(url)
    local libEntry = package.loadlib(path, entry)
    if libEntry then
        libEntry()
    end
end

local Template_Icon_Button = 
{
    size = {40, 40},
    
    onItemCreate=function(self, evt)
        self:d_setImage(Dw.SNormal, "")
        self:d_setImage(Dw.SHover, "home:testdata/mu/icon_background_hover.png")
        self:d_setImage(Dw.SPressDown, "home:testdata/mu/icon_background_pressdown.png")
    end,
}

local Template_Icon_Dude_Button = 
{
    onItemCreate = function(self, evt)
        self:d_setImage(Dw.SHover, "home:testdata/mu/icon_background_pressdown.png")
    end,
}

local Template_Buddy_Button =
{
    onItemCreate=function(p, evt) 
        RowPositioner
        {
            eventHandler = "DefaultItemEventHandler",
            sendChangedEvent = true,
            
            onItemCreate = function(pp, evt)
                local left = DudeIconButton()
                left:d_setIcon("home:testdata/mu/aio_toobar_add.png")
                
                
                local right = DudeIconButton()
                right:d_setIcon("home:testdata/mu/aio_toobar_add.png")
            end,
            
            onItemChange = function(self, evt)
                if evt.change == Dw.ItemGemoryChange then
                    local newOne = evt.newOne
                    self.parent.width = newOne.width
                    self.parent.height = newOne.height
                end
            end,
        }
    end,
}

function IconButton(...)
    return Button(Template_Icon_Button, ...)
end

function DudeIconButton(...)
    return IconButton(Template_Icon_Dude_Button, ...)
end

function BuddyIconButton(...)
    return IconButton(Template_Buddy_Button, ...)
end

function main()
    local window = FrameWindow
    {
        childrens=function(p)
            ClosedButton()
            MinimizeButton()
            
			
			p.m_mouseTag = GraphicsRectItem(Template_Rect_Actor, {
                         x = math.random(0, 400),
                         y = math.random(0, 400),
                         color = {0, 155, 0}, 
                         boundingRect = {-25, -25, 50, 50},} )
			p.m_seeker = GraphicsRectItem(Template_Rect_Actor, { 
                        x = math.random(0, 400), 
                        y = math.random(0, 400), } )
            p.m_fleer = GraphicsRectItem(Template_Rect_Actor, { 
                    x = math.random(0, 400), 
                    y = math.random(0, 400),
                    color = {0, 0, 150}, } )
                    
            local container = RowPositioner
            {
                spacing = 6,
                eventHandler = "DefaultItemEventHandler",
                
                onItemCreate=function(p, evt)
                    BuddyIconButton()
                    BuddyIconButton()
                    BuddyIconButton()
                    BuddyIconButton()
                    BuddyIconButton()
                    BuddyIconButton()
                end,
            }
            container:d_anchors():d_setCenterIn(p)
        end,
        
        d_onUpdateTimer = function(timer, interval)
            local owner = timer.owner
            local flee = owner.m_fleer
            local seek = owner.m_seeker
            local tag = owner.m_mouseTag
            if not seek or not tag or not flee then return end
            
            seek:d_seek(tag)
            seek:d_advance(50)
            seek:d_bound()
            
            flee:d_flee(tag)
            flee:d_advance(50)
            flee:d_bound()
        end,
        
        onItemCreate = function(self, evt)
            self.m_updateTimer = Timer.new(50)
            self.m_updateTimer:d_connect(self.d_onUpdateTimer)
            self.m_updateTimer.owner = self
        end,
        
        onItemDestroy = function(self, evt)
            self.m_updateTimer:d_destroy()
        end,
        
        onMouseClicked = function(self, evt)
            self.m_mouseTag.pos = evt.itemPoint
        end,
        
        d_startSimulate = function(self)
            self.m_updateTimer:d_start()
        end,
        
        d_stopSimulate = function(self)
            self.m_updateTimer:d_stop()
        end,
    }
   
    window:d_show()  
    window:d_startSimulate()
end

main()
