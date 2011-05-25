--- main
---
--- main logic of demo
--- Author - Jeremy Anderson

require('resources/scripts/Game Values')
require('resources/scripts/Object')
require('resources/scripts/Tile')
require('resources/scripts/grid')
require('resources/scripts/Ship')
require('resources/scripts/Bullet')
require('resources/scripts/Enemy')
require('resources/scripts/helper_functions')
--require('lfs')

Test = class(Object)

function Test:init( texture, pos, scale )
    Object.init(self, texture, pos, 1.0)
    self.scale = scale
    self.textureCoord = {0.66, 0.2, 0.064, 0.976}
end

function Test:Draw()
    self.transform = sprite:BuildTransformation( self.pos:get(), self.center:get(), self.scale:get(), self.rotation )
    sprite:Draw( self.texture, self.transform, self.textureCoord, self.zlevel, self.alpha, self.useCamera, self.color )
end

objects = {}
grid = {}--BuildGrid()

local lastPause = false

local function rotateVec2( vec, offset, angle )
    return vec + Vec2(offset.x * math.sin(math.rad(angle)), offset.y * -math.cos(math.rad(angle)))
end


local function EnemySpawning(target)
    return function()
        local enemy = images:Create(enemyImage)
        local interval = 1.0
        local numberOfEnemies = 4
        local iterations = 8
        local target = target
        for i = 0, iterations-1, 1 do
            Wait(interval)
            for j = 0, numberOfEnemies-1, 1 do
                table.insert(objects, Enemy(enemy, Vec2(math.random(screenWidth),math.random(screenHeight)), enemySize, enemySpeed, target))
            end
            interval = interval + 0.3
            numberOfEnemies = numberOfEnemies * 1.25
            coroutine.yield()
        end
    end
end

local function Explosion(grid, texture, x, y, color)
    return function()
        local texture = texture
        local x = x
        local y = y
        local grid = grid
        local color = color
        --Wait(0.1)
        CreateTile(grid, texture, x, y, color)
        Wait(0.1)
        CreateTile(grid, texture, x, y+1, color)
        CreateTile(grid, texture, x, y-1, color)
        CreateTile(grid, texture, x+1, y, color)
        CreateTile(grid, texture, x-1, y, color)
        Wait(0.1)
        CreateTile(grid, texture, x, y-2, color)
        CreateTile(grid, texture, x+1, y-1, color)
        CreateTile(grid, texture, x+2, y, color)
        CreateTile(grid, texture, x+1, y+1, color)
        CreateTile(grid, texture, x, y+2, color)
        CreateTile(grid, texture, x-1, y+1, color)
        CreateTile(grid, texture, x-2, y, color)
        CreateTile(grid, texture, x-1, y-1, color)
        Wait(0.1)
        CreateTile(grid, texture, x-2, y-1, color)
        CreateTile(grid, texture, x+2, y+1, color)
        CreateTile(grid, texture, x+2, y-1, color)
        CreateTile(grid, texture, x-2, y+1, color)
        CreateTile(grid, texture, x-1, y-2, color)
        CreateTile(grid, texture, x+1, y-2, color)
        CreateTile(grid, texture, x, y-3, color)
        CreateTile(grid, texture, x-1, y+2, color)
        CreateTile(grid, texture, x+1, y+2, color)
        CreateTile(grid, texture, x, y+3, color)
        CreateTile(grid, texture, x+3, y, color)
        CreateTile(grid, texture, x-3, y, color)
        Wait(0.1)
    end
end


local function MainLoop()

    local ship = images:Create(shipImage)
    local bullet = images:Create(bulletImage)
    local tile = images:Create(tileImage)
    local player = Ship(ship, Vec2(screenWidth/2.0,screenHeight/2.0), shipSize, shipSpeed)
    table.insert(objects, player)
    local count = 0.0
    CreateCoroutine(EnemySpawning(player))
    while true do
        EM:Update()

        if input:KeyTriggered(Key['P']) then
            lastPause = not lastPause
            EM:FireEvent('OnPause', lastPause)
        end

        --if input:MouseButtonPressed(Mouse['Left']) and idx and not lastPause then
        --    if not grid[idx]:GetWorking() then
        --        CreateCoroutine(twirl(grid[idx],1.0))
        --        grid[idx]:SetWorking( true )
        --    end
        --end

        count = count + dt

        --if input:MouseButtonPressed(Mouse['Left']) and not lastPause and count > 0.15 then
        if CanShoot() and not lastPause and count > shootingSpeed then
            local speed = bulletSpeed
            if player:GetMoving() then
                speed = speed + shipSpeed
            end

            local angle = GetShootingDirection(player:GetPos())

            table.insert(objects, Bullet(bullet, rotateVec2(player:GetPos(), Vec2(player:GetRadius(), player:GetRadius()), angle), bulletSize, speed, angle))
            count = 0.0
        end

        --[[if player:GetMoving() and not lastPause then
            local pos = player:GetPos()
            local radius = player:GetRadius()
            x = math.floor((pos.x)/tileSize)+1
            y = math.floor((pos.y)/tileSize)+1
            idx = (y-1)*horizontalTiles+x

            if idx < 0 or idx > numberOfTiles then
                idx = nil
            end

            if idx and not lastPause then
                if not grid[idx]:GetWorking() then
                    CreateCoroutine(twirl(grid[idx],1.0))
                    grid[idx]:SetWorking( true )
                end
            end
        end]]--

        --SetCamera( player:GetPos().x, player:GetPos().y )

        DrawGrid(grid)

        for i, v in pairs(objects) do
            v:Update()
            if v:GetType() == 'Bullet' then
                --local nx = distance(v:GetPos(),Vec2(screenWidth/2.0,screenHeight/2.0))
                for j, u in pairs(objects) do
                    if v ~= u and u:GetType() == 'Enemy' then
                        if TestCircleToCircle( v:GetPos(), v:GetRadius(), u:GetPos(), u:GetRadius() ) then
                            v:SetKill(true)
                            objects[j] = nil

                            local x = round((u:GetPos().x)/tileSize)
                            local y = round((u:GetPos().y)/tileSize)

                            --CreateTile(grid, tile, x, y)
                            CreateCoroutine(Explosion(grid, tile, x, y, u:GetColor()))
                        end
                    end
                end

                if v:Kill() then
                    objects[i] = nil
                end
            end
            v:Draw()
        end

        coroutine.yield()
    end
end

local function MouseControl()
    local pointer = images:Create(mousePointerImage)
    local mouse = Object( pointer, Vec2(input:GetCursorX(), input:GetCursorY()), 20.0 )
    mouse:SetCenter(Vec2(0.25, 0.12))
    mouse:SetZ(1.0)
    mouse.useCamera = false
    while true do
        mouse:SetPos(Vec2(input:GetCursorX(), input:GetCursorY()))
        mouse:Draw()
        coroutine.yield()
    end
end

CreateCoroutine( MainLoop )
CreateCoroutine( MouseControl )

