#pragma once

#define MAX_VECTOR4D(A,B) \
{\
	(A).x > (B).x ? (A).x : (B).x,\
	(A).y > (B).y ? (A).y : (B).y,\
	(A).z > (B).z ? (A).z : (B).z,\
	(A).w > (B).w ? (A).w : (B).w\
}

#define MIN_VECTOR4D(A,B) \
{\
	(A).x < (B).x ? (A).x : (B).x,\
	(A).y < (B).y ? (A).y : (B).y,\
	(A).z < (B).z ? (A).z : (B).z,\
	(A).w < (B).w ? (A).w : (B).w\
}

#define SET_MAX_VEC4D {(float)INT_MAX,(float)INT_MAX,(float)INT_MAX,(float)INT_MAX}
#define SET_MIN_VEC4D {(float)INT_MIN,(float)INT_MIN,(float)INT_MIN,(float)INT_MIN}
#include<tuple>

struct VECTOR4D
{
	union
	{
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			float r, g, b, a;
		};
		float v[4];
	};
	bool operator<(const VECTOR4D& B) const
	{
		return std::tie(x, y, z) < std::tie(B.x, B.y, B.z);
	}

};

struct MATRIX4D
{
	union
	{
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
		float m[4][4];
		VECTOR4D vec[4];
		float v[16];
	};

};

MATRIX4D operator*(MATRIX4D& A, MATRIX4D &B);
VECTOR4D operator*(VECTOR4D& A, float B);
VECTOR4D operator*(float B, VECTOR4D& A);


VECTOR4D operator*(MATRIX4D& A, VECTOR4D& V);
VECTOR4D operator*(VECTOR4D& V, MATRIX4D& A);
VECTOR4D operator*(VECTOR4D& A, VECTOR4D& B);
VECTOR4D operator-(VECTOR4D& A, VECTOR4D &B);
VECTOR4D operator+(VECTOR4D& A, VECTOR4D &B);
VECTOR4D operator/(VECTOR4D& A, float B);
VECTOR4D Cross3(VECTOR4D&A, VECTOR4D &B);
float    Dot(VECTOR4D& A, VECTOR4D& B);
float    Magnity(VECTOR4D& A);
VECTOR4D Normalize(VECTOR4D& A);
MATRIX4D Zero();
MATRIX4D Identity();
MATRIX4D Transpose(MATRIX4D& M);
MATRIX4D FastInverse(MATRIX4D& M);
MATRIX4D RotationX(float theta);
MATRIX4D RotationY(float theta);
MATRIX4D RotationZ(float theta);
MATRIX4D RotationAxis(float theta, VECTOR4D& Axis);
MATRIX4D Translation(float dx, float dy, float dz);
MATRIX4D Scaling(float sx, float sy, float sz);
MATRIX4D Orthogonalize(MATRIX4D &M);

MATRIX4D View(VECTOR4D& EyePos,
	VECTOR4D& Target, VECTOR4D& Up);
MATRIX4D PerspectiveWidthHeightRH(
	float fWidth, float fHeight,
	float zNear, float zFar);
MATRIX4D PerspectiveWidthHeightLH(float fWidth, float fHeight, float zNear, float zFar);

VECTOR4D Lerp(VECTOR4D& A, VECTOR4D &B, float u);  // I=A+u*(B-A)
bool PtInTriangle(VECTOR4D &V0, VECTOR4D &V1, VECTOR4D &V2, VECTOR4D &P);
bool PtInTriangleBarycentric(
	VECTOR4D &V0, VECTOR4D &V1, VECTOR4D &V2, VECTOR4D &P,
	float& w0, float& w1, float& w2);
float Inverse(MATRIX4D& M, MATRIX4D& R);
void PlaneIntersect(
	VECTOR4D& RayOrigin, //Source
	VECTOR4D& RayDir,    //Direction
	VECTOR4D& Plane,     //Plane eq.
	float&    n,
	float&    d); // u=n/d, V'=V0+u*RayDir

bool RayCastOnTriangle(
	VECTOR4D& V0, VECTOR4D& V1, VECTOR4D& V2,
	VECTOR4D& RayOrigin, VECTOR4D RayDir,
	VECTOR4D& Intersection);
void BuildRayFromPerspective(MATRIX4D& PV,
	float x, float y,
	VECTOR4D& RayOrigin,
	VECTOR4D& RayDir);

MATRIX4D ReflectionMatrix(VECTOR4D& Plane);