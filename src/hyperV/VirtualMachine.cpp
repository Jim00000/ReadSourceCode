#include "Common.hpp"
#include "VirtualMachine.hpp"

using namespace HyperV;

namespace
{
    template <typename EXCEPTIONBASE, size_t SERIAL>
    class VersatileException : public EXCEPTIONBASE
    {
    public:
        VersatileException<EXCEPTIONBASE, SERIAL>() noexcept = default;
        VersatileException<EXCEPTIONBASE, SERIAL>(const std::string &str) noexcept : EXCEPTIONBASE(str) {}
        VersatileException<EXCEPTIONBASE, SERIAL>(std::string &&str) noexcept : EXCEPTIONBASE(std::move(str)) {}
    };

    using CreatePartitionFailedException = typename VersatileException<VirtualMachineException, 1>;
}

VirtualMachine::VirtualMachine()
try : hPartition(nullptr)
{
    const HRESULT Status = WHvCreatePartition(&hPartition);
    if (Status != S_OK)
        throw CreatePartitionFailedException(GetWin32LastError());
}
catch (const CreatePartitionFailedException &e)
{
    spdlog::warn(e.what());
    throw VirtualMachineException("Creating Hyper-V Partition failed");
}
catch (...)
{
    spdlog::warn("Unidentified error got caught");
    throw VirtualMachineException("Unknown error");
}

VirtualMachine::~VirtualMachine()
{
}