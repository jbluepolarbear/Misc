--- helper functions
---
--- general helper functions that pretain to gameplay
--- Author - Jeremy Anderson


LEFT_X = 0
LEFT_Y = 1
RIGHT_X = 3
RIGHT_Y = 2

function GetRightAxisMoved()
    if input:GetAxis(RIGHT_X) ~= 0.0 or input:GetAxis(RIGHT_Y) ~= 0.0 then
        return true
    end

    return false
end

function GetLeftAxisMoved()
    if input:GetAxis(LEFT_X) ~= 0.0 or input:GetAxis(LEFT_Y) ~= 0.0 then
        return true
    end

    return false
end

function CanShoot()
    if input:JoystickActive() then
        if GetRightAxisMoved() then
            return true
        end
    end

    if input:MouseButtonPressed(Mouse['Left']) then
        return true
    end

    return false
end

function GetShootingDirection(position)
    local x = 0
    local y = 0

    if input:JoystickActive() and GetRightAxisMoved() then
        x = input:GetAxis(RIGHT_X)
        y = input:GetAxis(RIGHT_Y)
    else
        x = input:GetCursorX() - position.x
        y = input:GetCursorY() - position.y
    end

    local angle = math.deg(math.atan2(y,x)) + 90.0

    return angle
end

function GetMovingDirection()
    --return Rad2Deg(math.atan2(position.y - input:GetCursorY(), position.x - input:GetCursorX())) - 90.0
    local x = 0
    local y = 0

    if input:JoystickActive() and GetLeftAxisMoved() then
        x = input:GetAxis(LEFT_X)
        y = input:GetAxis(LEFT_Y)
    else
        if input:KeyPressed(Key['W']) then
            y = y - 1.0
        end

        if input:KeyPressed(Key['S']) then
            y = y + 1.0
        end

        if input:KeyPressed(Key['A']) then
            x = x - 1.0
        end

        if input:KeyPressed(Key['D']) then
            x = x + 1.0
        end

        if x == 0 and y == 0 then
            return nil
        end
    end

    local angle = math.deg(math.atan2(y,x)) + 90.0

    return angle
end

--Simple Collision
function TestCircleToCircle( Ctr0, radius0, Ctr1, radius1 )
    Radius0 = radius0 * radius0
    Radius1 = radius1 * radius1
    if distanceSquared( Ctr0, Ctr1 ) <= ( Radius0 + Radius1 ) then
        return true
    end
    return false
end

function round(value)
    return math.floor(value+0.5)
end
