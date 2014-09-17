

//=============================================

//      Type Definitions

//definition of the quaternion type

typedef struct {
	float w;
	float x;
	float y;
	float z;
} quaternion;

//definition of matrix type

typedef float matrix[4][4];

//definition of axis vector

typedef float axis[3];


//       Function Prototypes


quaternion NewQuaternion(float qw, float qx, float qy, float qz);

void Quaternion_To_Matrix(quaternion q, matrix m);

quaternion Axis_Angle_To_Quaternion(axis a, float theta);

void Quaternion_To_Axis_Angle(quaternion q, axis a, float *theta);

quaternion SLERP_Interpolate(quaternion q1, quaternion q2, float t);

quaternion Matrix_To_Quaternion (matrix);