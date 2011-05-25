--- Bullet
---
---
--- Author - Jeremy Anderson


require('resources/scripts/Object')

Bullet = class(Object)

function Bullet:init( texture, pos, radius, speed, rotation )
    Object.init(self, texture, pos, radius)
    self.speed = speed
    self.rotation = rotation
    self.velocity = Vec2(speed,speed)
    self.zlevel = bulletZLevel
    self.moving = false
    self.type = 'Bullet'
    self.kill = false
end

function Bullet:Kill()
    return self.kill
end

function Bullet:SetKill(kill)
    self.kill = kill
end

function Bullet:GetMoving()
    return self.moving
end

function Bullet:GetVelocity()
    return self.velocity
end

function Bullet:Update()
    self.pos = self.pos + Vec2( self.velocity.x * math.sin(math.rad(self.rotation)), self.velocity.y * -math.cos(math.rad(self.rotation)) ) * dt

    local halfRadius = self.radius/2.0
    --x coord
    if self.pos.x - halfRadius > screenWidth then
        self.kill = true
    end

    if self.pos.x < -halfRadius then
        self.kill = true
    end

    --y coord
    if self.pos.y - halfRadius > screenHeight then
        self.kill = true
    end

    if self.pos.y < -halfRadius then
        self.kill = true
    end
end
