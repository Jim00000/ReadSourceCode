#include "VirtualMachine.hpp"

using namespace HyperV;

VirtualMachine::VirtualMachine()
try : hPartition(nullptr)
{
}
catch (...)
{
}

VirtualMachine::~VirtualMachine()
{
}