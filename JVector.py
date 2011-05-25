from Matrix import *
import math

class Vector(object):
    def __init__( self, x = 0.0, y = 0.0, z = 0.0, w = 0.0 ):
        data = map(float,( x, y, z, w ))
        self.data = data[:]

    @property
    def x(self):
        return self.data[0]

    @property
    def y(self):
        return self.data[1]

    @property
    def z(self):
        return self.data[2]

    @property
    def w(self):
        return self.data[3]

    @x.setter
    def x(self, value):
        self.data[0] = float(value)

    @y.setter
    def y(self, value):
        self.data[1] = float(value)

    @z.setter
    def z(self, value):
        self.data[2] = float(value)

    @w.setter
    def w(self, value):
        self.data[3] = float(value)

    def __eq__( self, rhs ):
        if self.data[0] == rhs.data[0] and self.data[1] == rhs.data[1] and self.data[2] == rhs.data[2] and self.data[3] == rhs.data[3]:
          return True
        else:
          return False

    def __ne__( self, rhs ):
        if self.data[0] == rhs.data[0] and self.data[1] == rhs.data[1] and self.data[2] == rhs.data[2] and self.data[3] == rhs.data[3]:
          return True
        else:
          return False

    def __add__( self, rhs ):
        return Vector( self.data[0] + rhs.data[0], self.data[1] + rhs.data[1], self.data[2] + rhs.data[2], self.data[3] + rhs.data[3] )

    def __sub__( self, rhs ):
        return self + -rhs

    def __sub__( self, val ):
        t = Vector( -val, -val, -val, -val )
        return self + t

    def addvalue( self, value ):
        return Vector( self.data[0] + value, self.data[1] + value, self.data[2] + value, self.data[3] + value )

    def subvalue( self, value ):
        return Vector( self.data[0] - value, self.data[1] - value, self.data[2] - value, self.data[3] - value )

    def __neg__( self ):
        return Vector( -self.data[0], -self.data[1], -self.data[2], -self.data[3] )

    def __div__( self, value ):
        return Vector( self.data[0] / value, self.data[1] / value, self.data[2] / value, self.data[3] / value )

    def __mul__( self, rhs ):
        return self.data[0] * rhs.data[0] + self.data[1] * rhs.data[1] + self.data[2] * rhs.data[2] + self.data[3] * rhs.data[3]

    def mulvalue( self, value ):
        return Vector( self.data[0] * value, self.data[1] * value, self.data[2] * value, self.data[3] * value )

    def normalize( self ):
        return self / math.sqrt( self * self )

    def __getitem__( self, index ):
        return self.data[index]

    def __setitem__( self, index, value ):
        self.data[index] = value

    def mulmat( self, lhs ):
        _1 = lhs.row0
        _2 = lhs.row1
        _3 = lhs.row2
        _4 = lhs.row3

        return Vector(
          self.data[0] * _1[0] + self.data[1] * _1[1] + self.data[2] * _1[2] + self.data[3] * _1[3],
          self.data[0] * _2[0] + self.data[1] * _2[1] + self.data[2] * _2[2] + self.data[3] * _2[3],
          self.data[0] * _3[0] + self.data[1] * _3[1] + self.data[2] * _3[2] + self.data[3] * _3[3],
          self.data[0] * _4[0] + self.data[1] * _4[1] + self.data[2] * _4[2] + self.data[3] * _4[3]
        )

    def mulmat2( self, lhs ):
        _1 = lhs.row0
        _2 = lhs.row1

        return (
          self.data[0] * _1[0] + self.data[1] * _1[1] + self.data[2] * _1[2] + self.data[3] * _1[3],
          self.data[0] * _2[0] + self.data[1] * _2[1] + self.data[2] * _2[2] + self.data[3] * _2[3]
        )

    def length( self ):
        return math.sqrt( self.lenghtSquared() )

    def length( self ):
        return self * self

    def crossproduct( self, rhs ):
        return Vector( self.data[1]*rhs.data[3] - self.data[3]*rhs.data[1], self.data[3]*rhs.data[0] - self.data[0]*rhs.data[3], self.data[0]*rhs.data[1] - self.data[1]*rhs.data[0], 1.0 )

    def __repr__( self ):
        return repr( self.data )

    def Distance( lhs, rhs ):
        temp = lhs - rhs
        return temp.length()

    def DistanceSquared( lhs, rhs ):
        temp = lhs - rhs
        return temp.lengthSquared()

if __name__ == '__main__':
    v = Vector( 1.0, 2.0, 3.0, 4.0 )
    print v.x
    print v.y
    print v.z
    print v.w

    v.y = 5.0
    print v.y