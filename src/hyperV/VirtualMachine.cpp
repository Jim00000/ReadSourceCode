#include "Common.hpp"
#include "VirtualMachine.hpp"

using namespace HyperV;

namespace
{
    using CreatePartitionFailedException = typename VMException<ExceptionIdentifier::CreatePartitionFailed>;
    using DeletePartitionFailedException = typename VMException<ExceptionIdentifier::DeletePartitionFailed>;
    using SetPartitionPropertyFailedExecption = typename VMException<ExceptionIdentifier::SetPartitionPropertyFailed>;
    using SetupPartitionFailedException = typename VMException<ExceptionIdentifier::SetupPartitionFailed>;
}

const uint32_t VirtualMachine::ProcessorCount = 1;

VirtualMachine::VirtualMachine() noexcept : hPartition{nullptr}, Cap{}
{
    Cap.CheckAvailability();
}

VirtualMachine::VirtualMachine(Capability &capability) noexcept : hPartition{nullptr}, Cap{capability}
{
    if (Cap.IsInitialized() == false)
    {
        Cap.CheckAvailability();
    }
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
    HRESULT Status = WHvCreatePartition(&hPartition);

    if (Status != S_OK)
        throw CreatePartitionFailedException(GetWin32LastError());

    spdlog::info("Create Partition successfully");

    Status = WHvSetPartitionProperty(hPartition, WHvPartitionPropertyCodeProcessorCount, &ProcessorCount, sizeof(ProcessorCount));

    if (Status != S_OK)
        throw SetPartitionPropertyFailedExecption(GetWin32LastError());

    spdlog::info("Set Partition Property successfully. ProcessorCount : {}.", ProcessorCount);

    return;
}
catch (const CreatePartitionFailedException &e)
{
    spdlog::warn(e.what());
    throw VirtualMachineException("Creating Hyper-V Partition failed");
}
catch (const SetPartitionPropertyFailedExecption &e)
{
    spdlog::warn(e.what());
    throw VirtualMachineException("Set Partition Property failed");
}
catch (...)
{
    spdlog::warn("Unidentified error got caught");
    throw VirtualMachineException("Unknown error");
}