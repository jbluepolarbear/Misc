--- Enemy
---
---
--- Author - Jeremy Anderson


require('resources/scripts/Object')

Enemy = class(Object)

function Enemy:init( texture, pos, radius, speed, target )
    Object.init(self, texture, pos, radius)
    self.speed = speed
    self.velocity = Vec2(speed, speed)
    self.zlevel = enemyZLevel
    self.moving = false
    self.type = 'Enemy'
    self.color = enemyColors[math.random(3)]
    self.target = target
end

function Enemy:Update()
    self.rotation = math.deg(math.atan2(self.target:GetPos().y - self.pos.y, self.target:GetPos().x - self.pos.x))+90.0
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
