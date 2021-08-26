#include "Common.hpp"
#include "VirtualMachine.hpp"

using namespace HyperV;

namespace
{
    using CreatePartitionFailedException = typename VersatileException<VirtualMachineException, 1>;
    using DeletePartitionFailedException = typename VersatileException<VirtualMachineException, 2>;
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

VirtualMachine::~VirtualMachine() noexcept
{
    try
    {
        if (this->hPartition != nullptr)
        {
            const HRESULT Status = WHvDeletePartition(&this->hPartition);
            if (Status != S_OK)
            {
                throw DeletePartitionFailedException(GetWin32LastError());
            }
        }
    }
    catch (const DeletePartitionFailedException &e)
    {
        spdlog::warn(e.what());
    }
    catch (...)
    {
        spdlog::warn("Unidentified error got caught");
    }
}