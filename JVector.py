from JMatrix import *
import math
import numpy
import copy

class VectorError(Exception):
  #Matrix Exception class
  def __init__( self, description ):
    Exception.__init__( self )
    self.description = description

  def __str__( self ):
    return self.description

class Vector(object):
    def __init__( self, x = 0.0, y = 0.0, z = 0.0, w = 0.0 ):
        map(float,( x, y, z, w ))
        self.data = numpy.matrix( map(float,( x, y, z, w )) )

    @property
    def x(self):
        return self.data[0,0]

    @property
    def y(self):
        return self.data[0,1]

    @property
    def z(self):
        return self.data[0,2]

    @property
    def w(self):
        return self.data[0,3]

    @x.setter
    def x(self, value):
        self.data[0,0] = float(value)

    @y.setter
    def y(self, value):
        self.data[0,1] = float(value)

    @z.setter
    def z(self, value):
        self.data[0,2] = float(value)

    @w.setter
    def w(self, value):
        self.data[0,3] = float(value)

    def __eq__( self, rhs ):
        if self.x == rhs.x and self.y == rhs.y and self.z == rhs.z and self.w == rhs.w:
          return True
        else:
          return False

    def __ne__( self, rhs ):
        return not(self == rhs)

    def copy( self ):
        return copy.deepcopy( self )

    #def __add__( self, rhs ):
    #    return Vector( self.data[0] + rhs.data[0], self.data[1] + rhs.data[1], self.data[2] + rhs.data[2], self.data[3] + rhs.data[3] )

    def __add__( self, rhs ):
        t = Vector()
        t.data = self.data + rhs.data
        return t

    def __sub__( self, rhs ):
        return self + -rhs

    def __neg__( self ):
        t = Vector( -self.x, -self.y, -self.z, -self.w )
        return t

    def addvalue( self, value ):
        t = Vector()
        t.data = self.data + value
        return t

    def subvalue( self, value ):
        t = Vector()
        t.data = self.data - value
        return t

    def __div__( self, value ):
        t = Vector()
        t.data = self.data / value
        return t

    #dot product
    def __mul__( self, rhs ):
        return self.x * rhs.x + self.y * rhs.y + self.z * rhs.z + self.w * rhs.w

    def mulvalue( self, value ):
        t = Vector()
        t.data = self.data * value
        return t

    def normalize( self ):
        return self / math.sqrt( self * self )

    def __getitem__( self, index ):
        return self.data[0,index]

    def __setitem__( self, index, value ):
        self.data[0,index] = value

    def mulmat( self, lhs ):
##        _1 = lhs.row0
##        _2 = lhs.row1
##        _3 = lhs.row2
##        _4 = lhs.row3
##
##        return Vector(
##          self.data[0] * _1[0] + self.data[1] * _1[1] + self.data[2] * _1[2] + self.data[3] * _1[3],
##          self.data[0] * _2[0] + self.data[1] * _2[1] + self.data[2] * _2[2] + self.data[3] * _2[3],
##          self.data[0] * _3[0] + self.data[1] * _3[1] + self.data[2] * _3[2] + self.data[3] * _3[3],
##          self.data[0] * _4[0] + self.data[1] * _4[1] + self.data[2] * _4[2] + self.data[3] * _4[3]
##        )

        t = Vector()
        t.data = (lhs.data * self.data.T).T
        return t

    def mulmat2( self, lhs ):
        _1 = lhs.row0
        _2 = lhs.row1

        return (
          self.x * _1[0,0] + self.y * _1[0,1] + self.z * _1[0,2] + self.w * _1[0,3],
          self.x * _2[0,0] + self.y * _2[0,1] + self.z * _2[0,2] + self.w * _2[0,3]
        )

    def length( self ):
        return math.sqrt( self.lenghtSquared() )

    def lenghtSquared( self ):
        return self * self

    def crossproduct( self, rhs ):
        return Vector( self.y*rhs.z - self.z*rhs.y, self.z*rhs.x - self.x*rhs.z, self.x*rhs.y - self.y*rhs.x, 1.0 )

    def __repr__( self ):
        return repr( str( self.data ) )

    def Distance( lhs, rhs ):
        temp = lhs - rhs
        return temp.length()

    def DistanceSquared( lhs, rhs ):
        temp = lhs - rhs
        return temp.lengthSquared()

if __name__ == '__main__':
    m = Matrix.translate( 2.0, 4.0, 6.0 )
    v = Vector( 1.0, 2.0, 3.0, 4.0 )
    v1 = v.copy()
    v2 = Vector( 2.0, 3.0, 4.0, 5.0 )
    print v.x
    print v.y
    print v.z
    print v.w

    v.y = 5.0
    print v.y

    print v
    print v1
    print v2
    print
    print v2 - v
    print v2 + v
    print v2 * v
    print
    print v.mulmat(m)
    print v.mulmat2(m)