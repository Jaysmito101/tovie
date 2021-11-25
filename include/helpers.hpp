#pragma once

#include <string>

struct ProcAddr {
	int			  procId;
	unsigned long bAddr, eAddr;

	ProcAddr();

	ProcAddr(int procId, unsigned long bAddr, unsigned long eAddr);

	ProcAddr(const ProcAddr& other);
};

std::string to_data_type_name(int size);

std::string get_value(int *data, int size);