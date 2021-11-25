struct ProcAddr {
	int			  procId;
	unsigned long bAddr, eAddr;

	ProcAddr();

	ProcAddr(int procId, unsigned long bAddr, unsigned long eAddr);

	ProcAddr(const ProcAddr& other);
};

