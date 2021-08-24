#include "HyperVCapability.hpp"

using namespace HyperV;

CapabilityException::CapabilityException(const std::string &str) noexcept : VirtualMachineException(str) {}

CapabilityException::CapabilityException(std::string &&str) noexcept : VirtualMachineException(std::move(str)) {}