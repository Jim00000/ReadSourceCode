#include "Common.hpp"
#include "VirtualMachine.hpp"

using namespace HyperV;

namespace
{
    using CreatePartitionFailedException = typename VersatileException<VirtualMachineException, 1>;
    using DeletePartitionFailedException = typename VersatileException<VirtualMachineException, 2>;
}

VirtualMachine::VirtualMachine() noexcept : hPartition{nullptr}, Cap{}
{
    Cap.CheckAvailability();
}

VirtualMachine::VirtualMachine(Capability& capability) noexcept : hPartition{nullptr}, Cap{capability}
{
}

VirtualMachine::~VirtualMachine() noexcept
{
    try
    {
        if (this->hPartition != nullptr)
        {
            const HRESULT Status = WHvDeletePartition(this->hPartition);
            if (Status != S_OK)
            {
                throw DeletePartitionFailedException(GetWin32LastError());
            }
        }
    }
    catch (const DeletePartitionFailedException &e)
    {
        spdlog::debug(e.what());
    }
    catch (...)
    {
        spdlog::warn("Unidentified error got caught");
    }
}

void VirtualMachine::Initialize()
try
{
    const HRESULT Status = WHvCreatePartition(&hPartition);

    if (Status != S_OK)
        throw CreatePartitionFailedException(GetWin32LastError());

    spdlog::info("Create Partition successfully");
    return;
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