--- grid
---
--- manages the tiled background
--- Author - Jeremy Anderson

require('resources/scripts/Tile')

function index(x, y)
    return x + (y - 1) * horizontalTiles
end

function CreateTile(grid, texture, x, y, color )
    if x > horizontalTiles or x < 0 or y > verticalTiles or y < 0 then
        return
    end

    local idx = index(x, y)

    if grid[idx] then
        grid[idx]:SetColor(color)
        return
    end

    grid[idx] = Tile( texture, Vec2(tileSize * x + tileSize/2.0, tileSize * y + tileSize/2.0), tileSize, color )
end

function BuildGrid()
    local grid = {}
    local tile = images:Create(tileImage)
    for i = 0, verticalTiles-1, 1 do
        for j = 0, horizontalTiles-1, 1 do
            table.insert( grid, Tile( tile, Vec2(tileSize * j + tileSize/2.0, tileSize * i + tileSize/2.0), tileSize ) )
        end
    end

    return grid
end

function DrawGrid(grid)
    for i, v in pairs(grid) do
        v:Draw()
    end
end

function twirl(obj, mod)
    return function()
        local m = mod
        while obj:GetRotation() > -360.0 do
            if not obj:GetPause() then
                obj:SetRotation( obj:GetRotation() - ( 360.0 / m ) * dt )
            end
            coroutine.yield()
        end
        obj:SetRotation( 0.0 )
        obj:SetWorking( false )
    end
end

function bounce(obj, mod)
    return function()
        local m = mod
        local rr = obj:GetRadius()
        while obj:GetWorking() do
            if not obj:GetPause() then
                obj:SetRadius(obj:GetRadius() + (rr*mod*2.0*dt))
                if obj:GetRadius() < 0.0 then
                    obj:SetRadius(0.0)
                    mod = -mod
                end
                if obj:GetRadius() > rr then
                    obj:SetRadius(rr)
                    mod = -mod
                    obj:SetWorking(false)
                end
            end
            coroutine.yield()
        end
    end
end
