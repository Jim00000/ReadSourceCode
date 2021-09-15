#include "Common.hpp"
#include "VirtualMachine.hpp"

using namespace HyperV;

namespace
{
    using CreatePartitionFailedException = typename VMException<ExceptionIdentifier::CreatePartitionFailed>;
    using DeletePartitionFailedException = typename VMException<ExceptionIdentifier::DeletePartitionFailed>;
    using SetPartitionPropertyFailedExecption = typename VMException<ExceptionIdentifier::SetPartitionPropertyFailed>;
    using SetupPartitionFailedException = typename VMException<ExceptionIdentifier::SetupPartitionFailed>;
    using AllocateVMMemoryFailedException = typename VMException<ExceptionIdentifier::AllocateVMMemoryFailed>;
    using MapGpaRangeFailedException = typename VMException<ExceptionIdentifier::MapGpaRangeFailed>;
    using CreateVirtualProcessorFailedException = typename VMException<ExceptionIdentifier::CreateVirtualProcessorFailed>;
    using SetVirtualProcessorRegisterGeneralPurposeFailedException = typename VMException<ExceptionIdentifier::SetVirtualProcessorGeneralPurposeRegisterFailed>;
}

const uint32_t VirtualMachine::ProcessorCount = 1;
const SIZE_T VirtualMachine::GuestMemorySize = 0x2'000'000; // 2MB

VirtualMachine::VirtualMachine() noexcept : VirtualMemory{nullptr}, hPartition{nullptr}, Cap{}
{
    Cap.CheckAvailability();
}

VirtualMachine::VirtualMachine(Capability &capability) noexcept : VirtualMemory{nullptr}, hPartition{nullptr}, Cap{capability}
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

    Status = WHvSetupPartition(hPartition);

    if (Status != S_OK)
        throw SetupPartitionFailedException(GetWin32LastError());

    spdlog::info("Setup Partition successfully.");

    VirtualMemory = VirtualAlloc(NULL, GuestMemorySize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (VirtualMemory == NULL)
        throw AllocateVMMemoryFailedException(GetWin32LastError());

    spdlog::info("Allocate Memory for VM successfully.");

    SecureZeroMemory(VirtualMemory, GuestMemorySize);

    spdlog::info("Fill guest memory with zeros");

    // Host Physical Address (HPA): the native physical address space.
    // Guest Physical Address (GPA): the guest physical address space from a virtual
    // machine. GPA to HPA transition is usually based on an MMU-like hardware
    // module (EPT in X86), and is associated with a page table.
    //
    // Reference: https://projectacrn.github.io/latest/developer-guides/hld/hv-memmgt.html
    Status = WHvMapGpaRange(hPartition, VirtualMemory, 0, GuestMemorySize, WHvMapGpaRangeFlagRead | WHvMapGpaRangeFlagWrite | WHvMapGpaRangeFlagExecute);

    if (Status != S_OK)
        throw MapGpaRangeFailedException(GetWin32LastError());

    spdlog::info("Creating a memory mapping for VM successfully.");

    Status = WHvCreateVirtualProcessor(hPartition, 0, 0);

    if (Status != S_OK)
        throw CreateVirtualProcessorFailedException(GetWin32LastError());

    spdlog::info("Creating a virtual processor successfully");

    ConfigRegisters();

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
catch (const SetupPartitionFailedException &e)
{
    spdlog::warn(e.what());
    throw VirtualMachineException("Setup Hyper-V partition failed");
}
catch (const AllocateVMMemoryFailedException &e)
{
    spdlog::error(e.what());
    throw VirtualMachineException("Allocate memory for VM failed");
}
catch (const MapGpaRangeFailedException &e)
{
    spdlog::error(e.what());
    throw VirtualMachineException("Mapping memory for VM failed");
}
catch (const CreateVirtualProcessorFailedException &e)
{
    spdlog::error(e.what());
    throw VirtualMachineException("Creating virtual processor failed");
}
catch (...)
{
    spdlog::warn("Unidentified error got caught");
    throw VirtualMachineException("Unknown error");
}

void VirtualMachine::ConfigRegisters()
try
{
    HRESULT Status = WHvSetVirtualProcessorRegisters(hPartition, 0, Regs.GeneralPurpose().data(), Regs.GeneralPurpose().size(), Regs.GeneralPurposeInitValue().data());

    if (Status != S_OK)
        throw SetVirtualProcessorRegisterGeneralPurposeFailedException(GetWin32LastError());
}
catch (SetVirtualProcessorRegisterGeneralPurposeFailedException &e)
{
    spdlog::error(e.what());
    throw VirtualMachineException("Set virtual processor general purpose registers failed");
}
catch (...)
{
    spdlog::error("Unidentified error got caught");
    throw VirtualMachineException("Unknown error");
}