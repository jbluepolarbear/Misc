--- Game Values
--- Author - Jeremy Anderson

--------------------
numberOfZlevels = 10

local function CreateZLevel(levels)
    local level = 1.0
    local zlevel = {}
    for i = 1, levels-1, 1 do
        table.insert(zlevel, i, level)
        level = level/2.0
    end
    return zlevel
end

zlevel = CreateZLevel(numberOfZlevels)
--------------------
shipImage = 'resources/textures/ship.png'
shipColor = {1.0, 0.0, 1.0}
shipSpeed = 200.0
shipSize = 30.0
shipZLevel = zlevel[2]
--------------------
bulletImage = 'resources/textures/bullet.png'
bulletSpeed = 200.0
bulletSize = 15.0
bulletZLevel = zlevel[2]
--------------------
tileImage = 'resources/textures/backtile.png'--'resources/textures/back.png'
horizontalTiles = 40
verticalTiles = 30
numberOfTiles = horizontalTiles * verticalTiles
tileSize = screenWidth / horizontalTiles
--------------------
--Shooting Speed in second interval
shootingSpeed = 0.15
--------------------
mousePointerImage = 'resources/textures/mouse.png'
--------------------
enemyImage = 'resources/textures/blue_circle.png'
enemyColor = {1.0, 0.0, 1.0}
enemyColorRed = {1.0, 0.0, 0.0}
enemyColorGreen = {0.0, 1.0, 0.0}
enemyColorBlue = {0.0, 0.0, 1.0}
enemySpeed = 100.0
enemySize = 20.0
enemyZLevel = zlevel[2]

enemyColors = {
    enemyColorRed,
    enemyColorGreen,
    enemyColorBlue
}
