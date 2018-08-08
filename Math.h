#include <cmath>
#define _USE_MATH_DEFINES
#include "SDK.h"

#define M_PI		3.14159265358979323846
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / (float)M_PI) )
#define DEG2RAD( x  )  ( (float)(x) * (float)((float)M_PI / 180.f) )

namespace Math {
	void VectorTransform(const Vector in1, matrix3x4_t in2, Vector &out);
	Vector CalculateAngle(const Vector& in, Vector out);
	void AngleVectors(const QAngle& angles, Vector& forward);
	void VectorAngles(const Vector& forward, QAngle& angles);
	void SinCos(float radians, float* sine, float* cosine);
	double deg2rad(double deg);
	double rad2deg(double rad);
	void VectorAngles2(const Vector& forward, Vector &angles);
	Vector NormalizeAngle(Vector angle);
	void ClampAngles(Vector& angle);
	Vector CalcAngle(Vector src, Vector dst);
};