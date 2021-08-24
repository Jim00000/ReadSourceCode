#include <string>

#include "Common.hpp"

using namespace HyperV;

VirtualMachineException::VirtualMachineException() noexcept : std::exception() {}

VirtualMachineException::VirtualMachineException(const std::string &str) noexcept : std::exception(str.c_str()) {}

VirtualMachineException::VirtualMachineException(std::string &&str) noexcept : std::exception(str.c_str()) {}