#include "helpers.hpp"



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
