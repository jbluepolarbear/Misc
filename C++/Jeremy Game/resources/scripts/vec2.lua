--- vec2
---
--- simple 2d vector class
--- Author - Jeremy Anderson



Vec2 = class()

function Vec2:init( x, y )
    if x == nil then
        self.x = 0.0
    else
        self.x = x
    end

    if y == nil then
        self.y = 0.0
    else
        self.y = y
    end
end

function Vec2:__add(rhs)
    return Vec2( self.x + rhs.x, self.y + rhs.y )
end

function Vec2:__sub(rhs)
    return Vec2( self.x - rhs.x, self.y - rhs.y )
end

--scale by
function Vec2:__mul(f)
    return Vec2( self.x * f, self.y * f )
end

--scale by
function Vec2:__div(f)
    return Vec2( self.x / f, self.y / f )
end

function Vec2:__eq(rhs)
    return self.x == rhs.x and self.y == rhs.y
end

function Vec2:__nq(rhs)
    return self.x ~= rhs.x and self.y ~= rhs.y
end

function Vec2:dot(rhs)
    return self.x * rhs.x + self.y * rhs.y
end

function Vec2:__unm()
    return Vec2( -self.x, -self.y )
end

--returns Vec2 as a table
function Vec2:get()
    return {self.x, self.y}
end

function Vec2:normalize()
    return self / self:length()
end

function Vec2:length()
    return math.sqrt( self:dot(self) )
end

function Vec2:lengthSquared()
    return self:dot(self)
end

function Vec2:printVars()
    print( self.x..' '..self.y )
end

function distance( lhs, rhs )
    temp = lhs - rhs
    return temp:length()
end

function distanceSquared( lhs, rhs )
    temp = lhs - rhs
    return temp:lengthSquared()
end

--a = -Vec2( 1.0, 2.0 )
--b = Vec2( 1.0, -1.0 )
--c = a + b
--d = a * 3.0
--e = Vec2( 1.0, 1.0 )

--a:printVars()
--b:printVars()
--c:printVars()
--d:printVars()
--print(distance(a,b))
