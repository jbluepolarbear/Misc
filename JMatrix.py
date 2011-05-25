#-------------------------------------------------------------------------------
# Name:        JMatrix.py
# Purpose:     A simple Matrix class
#
# Author:      Jeremy Anderson
#
# Created:     25/02/2011
# Copyright:   (c) Jeremy Anderson 2011
# Licence:     <your licence>
#-------------------------------------------------------------------------------

import numpy
from JVector import *
import copy

from math import *
def Deg2Rad( x ):
  return 0.017453 * x
def Rad2Deg( x ):
  return x * 57.295779


class MatrixError(Exception):
  #Matrix Exception class
  def __init__( self, description ):
    Exception.__init__( self )
    self.description = description

  def __str__( self ):
    return self.description

def MatrixHelper( _11 = 0.0, _12 = 0.0, _13 = 0.0, _14 = 0.0,
                  _21 = 0.0, _22 = 0.0, _23 = 0.0, _24 = 0.0,
                  _31 = 0.0, _32 = 0.0, _33 = 0.0, _34 = 0.0,
                  _41 = 0.0, _42 = 0.0, _43 = 0.0, _44 = 0.0 ):
    return [(_11,_12,_13,_14),(_21,_22,_23,_24),(_31,_32,_33,_34),(_41,_42,_43,_44)]

class Matrix(object):
    def __init__( self, *args ):
        if not args:
            self.data = numpy.matrix( [(1.0,0.0,0.0,0.0), (0.0,1.0,0.0,0.0), (0.0,0.0,1.0,0.0), (0.0,0.0,0.0,1.0)] )

        elif len(args) == 4:
            self.data = numpy.matrix( [(1.0,0.0,0.0,0.0), (0.0,1.0,0.0,0.0), (0.0,0.0,1.0,0.0), (0.0,0.0,0.0,1.0)] )

            self.row0 = args[0]
            self.row1 = args[1]
            self.row2 = args[2]
            self.row3 = args[3]

        else:
            raise TypeError( 'Matrix.__init__() takes 0, or 4 arguments (%i given)' % len(args) )

    @property
    def row0(self):
        return self.data[0]

    @property
    def row1(self):
        return self.data[1]

    @property
    def row2(self):
        return self.data[2]

    @property
    def row3(self):
        return self.data[3]

    @row0.setter
    def row0(self, value):
        values = tuple(value)[:4]
        self.data[0] = map(float, values)

    @row1.setter
    def row1(self, value):
        values = tuple(value)[:4]
        self.data[1] = map(float, values)

    @row2.setter
    def row2(self, value):
        values = tuple(value)[:4]
        self.data[2] = map(float, values)

    @row3.setter
    def row3(self, value):
        values = tuple(value)[:4]
        self.data[3] = map(float, values)

    def getrow( self, row ):
        if row == 0:
            return row0
        if row == 1:
            return row1
        if row == 2:
            return row2
        if row == 3:
            return row3

    def __getitem__( self, index ):
        return self.data[index]

    def __setitem__( self, index, value ):
        self.data[index] = float(value)

    def copy( self ):
        return copy.deepcopy( self )

    @staticmethod
    def identity():
        return Matrix()

    def __mul__( self, rhs ):
        mat = Matrix()
        mat.data = self.data * rhs.data
        return mat

    def __add__( self, rhs ):
        mat = Matrix()
        mat.data = self.data + rhs.data
        return mat

    def __sub__( self, rhs ):
        mat = Matrix()
        mat.data = self.data - rhs.data

    def transpose( self ):
        mat = Matrix()
        mat.data = self.data.T
        return mat

    def tolist( self ):
        return self.data.tolist()

    @staticmethod
    def translate( x = 0.0, y = 0.0, z = 0.0 ):
        return Matrix(
            (1.0,0.0,0.0,x),
            (0.0,1.0,0.0,y),
            (0.0,0.0,1.0,z),
            (0.0,0.0,0.0,1.0)
        )

    @staticmethod
    def scale( x = 1.0, y = 1.0, z = 1.0 ):
        return Matrix(
            (x,0.0,0.0,0.0),
            (0.0,y,0.0,0.0),
            (0.0,0.0,z,0.0),
            (0.0,0.0,0.0,1.0)
        )

    @staticmethod
    def rotatex( angle ):
        return Matrix(
            (1.0,0.0,0.0,0.0),
            (0.0,cos(Deg2Rad(angle)),-sin(Deg2Rad(angle)),0.0),
            (0.0,sin(Deg2Rad(angle)),cos(Deg2Rad(angle)),0.0),
            (0.0,0.0,0.0,1.0)
        )

    @staticmethod
    def rotatey( angle ):
        return Matrix(
            (cos(Deg2Rad(angle)),0.0,sin(Deg2Rad(angle)),0.0),
            (0.0,1.0,0.0,0.0),
            (-sin(Deg2Rad(angle)),0.0,cos(Deg2Rad(angle)),0.0),
            (0.0,0.0,0.0,1.0)
        )

    @staticmethod
    def rotatez( angle ):
        return Matrix(
            (cos(Deg2Rad(angle)),-sin(Deg2Rad(angle)),0.0,0.0),
            (sin(Deg2Rad(angle)),cos(Deg2Rad(angle)),0.0,0.0),
            (0.0,0.0,1.0,0.0),
            (0.0,0.0,0.0,1.0)
        )

    def __repr__( self ):
        return str( self.row0 ) + '\n' + str( self.row1 ) + '\n' + str( self.row2 ) + '\n' + str( self.row3 )

if __name__ == '__main__':
    m = Matrix()
    print m
    print
    print m.row0
    print m.row1

    print
    temp = m[0,0]
    print temp
    m[0,0] = 5.0
    print
    print m
    print
    m1 = Matrix.translate( 2.0, 3.0, 4.0 )
    print m1

    print
    print m1 * Matrix()