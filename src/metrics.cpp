#include "metrics.h"

std::ostream& operator <<(std::ostream& os, const Metrics& p) {
	os << "l1Count = " << p.l1Count << ", ";
	std::cout << std::endl;
	os << "l2Count = " << p.l2Count << ", ";
	std::cout << std::endl;
	os << "l1WithoutQ = " << p.l1WithoutQ << ", ";
	std::cout << std::endl;
	os << "l2WithoutQ = " << p.l2WithoutQ << ", ";
	std::cout << std::endl;
	os << "l1FromFIFO = " << p.l1FromFIFO << ", ";
	std::cout << std::endl;
	os << "l2FromFIFO = " << p.l2FromFIFO << ", ";
	std::cout << std::endl;
	os << "serverBusyTime = " << p.serverBusyTime << ", ";
	std::cout << std::endl;
	os << "lastEventTime = " << p.lastEventTime << ", ";
//	outputStream << "FIFO size = " << p.l;
	return os;
}