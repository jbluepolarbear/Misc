--- Object class
---
--- Author - Jeremy Anderson

Object = class()

function Object:init( texture, pos, radius )
    EM:Subscribe( self, 'OnPause' )
    self.texture = texture
    self.pos = pos
    self.radius = radius
    self.rotation = 0.0
    self.pause = false
    self.center = Vec2(0.5, 0.5)
    self.zlevel = -1.0
    self.textureCoord = {1.0, 0.0, 0.0, 1.0}
    self.type = 'Base'
    self.alpha = 1.0
    self.useCamera = true
    self.color = {1.0, 0.0, 1.0}
end

function Object:Update()
    return
end

function Object:GetType()
    return self.type
end

function Object:uninit()
    EM:Subscribe( self, 'OnPause' )
end

function Object:OnPause(pause)
    self.pause = pause
end

function Object:SetPause(pause)
    self.pause = pause
end

function Object:GetPause()
    return self.pause
end

function Object:SetPos(pos)
    self.pos = pos
end

function Object:GetPos()
    return self.pos
end

function Object:SetRotation(rotation)
    self.rotation = rotation
end

function Object:GetRotation()
    return self.rotation
end

function Object:SetRadius(radius)
    self.radius = radius
end

function Object:GetRadius()
    return self.radius
end

function Object:SetZ(zlevel)
    self.zlevel = zlevel
end

function Object:GetZ()
    return self.zlevel
end

function Object:SetCenter(center)
    self.center = center
end

function Object:GetCenter()
    return self.center
end

function Object:SetTexture(texture)
    self.texture = texture
end

function Object:GetTexture()
    return self.texture
end

function Object:SetColor(color)
    self.color = color
end

function Object:GetColor()
    return self.color
end


function Object:Draw()
    self.transform = sprite:BuildTransformation( self.pos:get(), self.center:get(), {self.radius, self.radius}, self.rotation )
    sprite:Draw( self.texture, self.transform, self.textureCoord, self.zlevel, self.alpha, self.useCamera, self.color )
end
