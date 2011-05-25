--- ship
---
--- the player (aka ship) class
--- Author - Jeremy Anderson


require('resources/scripts/Object')

Ship = class(Object)

function Ship:init( texture, pos, radius, speed )
    Object.init(self, texture, pos, radius)
    self.speed = speed
    self.velocity = Vec2(0.0,0.0)
    self.zlevel = shipZLevel
    self.moving = false
    self.type = 'Ship'
    self.color = shipColor
end

function Ship:GetVelocity()
    return self.velocity
end

function Ship:GetMoving()
    return self.moving
end

function Ship:Update()
    self.velocity = Vec2()

    self.moving = false

    local angle = GetMovingDirection()

    if angle then
        self.rotation = angle

        self.moving = true
        self.velocity = Vec2(self.speed, self.speed)
    end

    --self.pos[1] = self.pos[1] + self.velocity[1] * dt--math.cos(Deg2Rad(self.direction))
    --self.pos[2] = self.pos[2] + self.velocity[2] * dt--math.sin(Deg2Rad(self.direction))

    --self.pos = self.pos + self.velocity * dt
    self.pos = self.pos + Vec2( self.velocity.x * math.sin(math.rad(self.rotation)), self.velocity.y * -math.cos(math.rad(self.rotation)) ) * dt

    local halfRadius = self.radius/2.0
    --x coord
    if self.pos.x + halfRadius > screenWidth then
        self.pos.x = screenWidth - halfRadius
    end

    if self.pos.x < halfRadius then
        self.pos.x = halfRadius
    end

    --y coord
    if self.pos.y + halfRadius > screenHeight then
        self.pos.y = screenHeight - halfRadius
    end

    if self.pos.y < halfRadius then
        self.pos.y = halfRadius
    end
end

function Ship:Draw()
    self.transform = sprite:BuildTransformation( self.pos:get(), self.center:get(), {self.radius, self.radius}, self.rotation )
    sprite:Draw( self.texture, self.transform, self.textureCoord, self.zlevel, self.alpha, self.useCamera, self.color )
end

