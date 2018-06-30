#include "ContainerTerminal.hpp"

std::string TerminalContainer::read(int p_timeoutSec, int p_timeoutUsec)
{
	return readDataFromFdWithTimeout(0, p_timeoutSec, p_timeoutUsec);
}

ssize_t TerminalContainer::write(std::string p_data)
{
	return writeDataToFd(1, p_data);
}
