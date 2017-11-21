#include "Vector3.h"

const Vector3 Vector3::nan		= Vector3(NAN, NAN, NAN);
const Vector3 Vector3::max		= Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
const Vector3 Vector3::zero		= Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one		= Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::xAxis	= Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::yAxis	= Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::zAxis	= Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::axes[3]  = {xAxis, yAxis, zAxis};
const Vector3 Vector3::xNegAxis = Vector3(-1.0f,  0.0f,  0.0f);
const Vector3 Vector3::yNegAxis = Vector3( 0.0f, -1.0f,  0.0f);
const Vector3 Vector3::zNegAxis = Vector3( 0.0f,  0.0f, -1.0f);

