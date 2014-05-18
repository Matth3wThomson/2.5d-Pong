#include "Matrix3.h"


Matrix3::Matrix3(void)
{
}


Matrix3::~Matrix3(void)
{
}

Matrix3::Matrix3(const Matrix4& m){
	values[0] = m.values[0];
	values[1] = m.values[1];
	values[2] = m.values[2];
	values[3] = m.values[4];
	values[4] = m.values[5];
	values[5] = m.values[6];
	values[6] = m.values[8];
	values[7] = m.values[9];
	values[8] = m.values[10];
}
