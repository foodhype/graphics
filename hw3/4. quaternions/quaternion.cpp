
#include "quaternion.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415927


// function New Quaternion
// This function returns a new quaternion structure given the four components
quaternion NewQuaternion(float qw, float qx, float qy, float qz) {
	quaternion new_quaternion;

	new_quaternion.w = qw;
	new_quaternion.x = qx;
	new_quaternion.y = qy;
	new_quaternion.z = qz;
	
	return new_quaternion;
}

// function Quaternion_To_Matrix
// This function takes a quaternion structure and a 4x4 matrix as arguments
// It fills in the matrix with the proper values to do the rotation in OpenGL
void Quaternion_To_Matrix(quaternion q, matrix m) {
	float w = q.w;
	float x = q.x;
	float y = q.y;
	float z = q.z;
	
	m[0][0] = 1 - 2*y*y - 2*z*z;
    m[1][0] = 2*x*y - 2*w*z;
    m[2][0] = 2*x*z + 2*w*y;
    m[3][0] = 0;

    m[0][1] = 2*x*y + 2*w*z;
    m[1][1] = 1 - 2*x*x - 2*z*z;
    m[2][1] = 2*y*z - 2*w*x;
    m[3][1] = 0;

    m[0][2] = 2*x*z - 2*w*y;
    m[1][2] = 2*y*z + 2*w*x;
    m[2][2] = 1 - 2*x*x - 2*y*y;
    m[3][2] = 0;

    m[0][3] = 0;
    m[1][3] = 0;
    m[2][3] = 0;
    m[3][3] = 1;
}

// function Matrix_To_Quaternion
// This function takes a 4x4 matrix of the form used in OpenGL MODELVIEW
// and returns the corresponding quaternion for orientation
// Right now this function only works for angles strictly less than 180 degrees
// because the w component of q is zero for rotations of 180 degrees, giving a 
// division by zero.
quaternion Matrix_To_Quaternion(matrix m) {
	quaternion q;

	q.w = 0.5 * sqrt (m[0][0] + m[1][1] + m[2][2] + m[3][3] );
	
	q.x = (m[1][2] - m[2][1])/(4*q.w);
	q.y = (m[2][0] - m[0][2])/(4*q.w);
	q.z = (m[0][1] - m[1][0])/(4*q.w);

	return q;
}

//function Axis_Angle_To_Quaternion
// This function takes a axis vector, expressed as an array of three floats,
// an angle of rotation about that axis and returns a quaternion structure
// The function is designed to take the angle in degrees.
quaternion Axis_Angle_To_Quaternion(axis a, float theta)
{
	//convert to radians
	theta = theta*(PI/180.0);
	float s = (float) sin(theta/2);
    float w = (float) cos(theta/2);
    float x = a[0] * s;
    float y = a[1] * s;
    float z = a[2] * s;
    float norm = (float) sqrt(x*x + y*y + z*z + w*w);
    
	
	w = w / norm;
    x = x / norm;
    y = y / norm;
    z = z / norm;
  
    return NewQuaternion(w,x,y,z);
}

// function Quaternion_To_Axis_Angle
//  This function takes a quaternion, an axis array, and a pointer to a (float)
//  angle.  The axis and angle are set to correspond to the quaternion orientation
//  The angle is converted to degrees so that it can be used directly by the 
//  OpenGL glRotatef() function.

void Quaternion_To_Axis_Angle(quaternion q, axis a, float *theta)
{
	float s = (float) sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
    if (s != 0.0)
	{
    
      *theta =(360.0f/PI) * (float) acos(q.w);  // angle converted to degrees
      a[0] = q.x / s;
      a[1] = q.y / s;
      a[2] = q.z / s;
    }
    else
	{
      *theta = 0.0;
      a[0] = 1.0;
      a[1] = 0.0;
      a[2] = 0.0;
    }
}

// function SLERP_Interpolate
// This function takes two quaternions and a (float) parameter t.  As
// t varies from 0.0 to 1.0, the function returns a quaternion interpolated
// according to the spherical linear interpolation (SLERP) formula

quaternion SLERP_Interpolate(quaternion q1, quaternion q2, float t)
{
	float EPS = 0.001;

    float o, co, so, scale0, scale1;
    float qi[4];
	quaternion m;

  // Do a linear interpolation between two quaternions (0 <= t <= 1).

  co = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;  // dot product

  if (co < 0)
  {
    co = -co;
    qi[0] = -q2.x;
    qi[1] = -q2.y;
    qi[2] = -q2.z;
    qi[3] = -q2.w;
  }
  else
  {
    qi[0] = q2.x;
    qi[1] = q2.y;
    qi[2] = q2.z;
    qi[3] = q2.w;
  }

  if ((1 - co) <= EPS)// If the quaternions are really close, do a simple linear interpolation.
  {
    scale0 = 1 - t;
    scale1 = t;
  }
  else
  {
    // Otherwise SLERP.
    o = (float) acos(co);
    so = (float) sin(o);
    scale0 = (float) sin((1 - t) * o) / so;
    scale1 = (float) sin(t * o) / so;
  }

  // Calculate interpolated quaternion:
  
  m.x = scale0 * q1.x + scale1 * qi[0];
  m.y = scale0 * q1.y + scale1 * qi[1];
  m.z = scale0 * q1.z + scale1 * qi[2];
  m.w = scale0 * q1.w + scale1 * qi[3];

  return m;
}
