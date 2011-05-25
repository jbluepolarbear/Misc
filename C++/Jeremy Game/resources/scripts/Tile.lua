--- Tile
---
--- general tile
--- Author - Jeremy Anderson

require('resources/scripts/Object')

Tile = class( Object )

function Tile:init( texture, pos, radius, color )
    Object.init(self, texture, pos, radius)
    self.working = false
    self.type = 'Tile'
    if color then
        self.color = color
    end
end

function Tile:SetWorking(working)
    self.working = working
end

function Tile:GetWorking()
    return self.working
end

