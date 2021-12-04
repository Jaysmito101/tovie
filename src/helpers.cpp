#include "helpers.hpp"
#include <cstring>
#include <algorithm>

namespace tovie
{

ProcAddr::ProcAddr() {
	procId = -1;
	bAddr = eAddr = 0;
}
ProcAddr::ProcAddr(int procIdi, unsigned long bAddri, unsigned long eAddri) {
	procId = procIdi;
	bAddr	 = bAddri;
	eAddr	 = eAddri;
}
// copy constructor
ProcAddr::ProcAddr(const ProcAddr& other) {
	procId = other.procId;
	bAddr	 = other.bAddr;
	eAddr	 = other.eAddr;
}

std::string get_value(int *data, int size)
{
	switch(size)
	{
		case 1:
			return std::to_string(*data);
		case 2:
		{
			short val;
			memcpy(&val, data, sizeof(short));
			return std::to_string(val);
		}
		case 4:
		{
			int val;
			memcpy(&val, data, sizeof(int));
			if(val == 0)
			{
				float d;
				memcpy(&d, data, sizeof(float));
				return std::to_string(d);
			}
			return std::to_string(val);
		}
		case 8:
		{
			long long val;
			memcpy(&val, data, sizeof(long long));
			if(val == 0)
			{
				double d;
				memcpy(&d, data, sizeof(double));
				return std::to_string(d);
			}
			return std::to_string(val);
		}
		default:
		{
			char* d = new char[size];
			for(int i = 0; i < size; i++)
				d[i] = (char)data[i];
			std::string s(d);
			delete[] d;
			return s;
		}
	}
}

std::string to_data_type_name(int size)
{
	switch (size)
	{
	case 1:
		return "char";
	case 2:
		return "short";
	case 4:
		return "int or float";
	case 8:
		return "long long or double";
	default:
		return "string";
	}
}

}