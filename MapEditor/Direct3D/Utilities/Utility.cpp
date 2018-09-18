#include "../stdafx.h"
#include "Utility.h"

void Utility::float2Bytes(float val, byte * bytes_array)
{
	bytes_array[0] = (byte)((int)val >> 24);
	bytes_array[1] = (byte)((int)val >> 16);
	bytes_array[2] = (byte)((int)val >> 8);
	bytes_array[3] = (byte)((int)val);
	//// Create union of shared memory space
	//union {
	//	float float_variable;
	//	byte temp_array[4];
	//} u;
	//// Overite bytes of union with float variable
	//u.float_variable = val;
	//// Assign bytes to input array
	//memcpy(bytes_array, u.temp_array, 4);
}
