#include "Common.hpp"
#include "HyperVCapability.hpp"
#include <Windows.h>

using namespace HyperV;

namespace
{
    template <enum WHV_CAPABILITY_CODE CODE, typename BUFFER> 
    inline void GetWin32HyperVCapability(_Out_ BUFFER &Buffer)
    {
        UINT32 WrittenSizeInBytes = 0;
        const HRESULT Status = WHvGetCapability(CODE, &Buffer, sizeof(Buffer),
                                          &WrittenSizeInBytes);
        if (Status != S_OK)
            throw CapabilityException(GetWin32LastError());
    }
}

Capability::Capability()
{
    this->IsHyperVPresent = GetHyperVPresent();
    if (this->IsHyperVPresent)
        try
        {
            spdlog::info("Hyper-V is present");
            FillFeatures(this->Features);
            FillExtendedVMExits(this->ExtendedVmExits);
            FillProcessorFeatures(this->ProcessorFeatures);
            FillProcessorXsaveFeatures(this->ProcessorXSaveFeature);
            FillGetProcessorVednors(this->Vendors);
        }
        catch (const CapabilityException &e)
        {
            spdlog::error(e.what());
        }
}

const BOOL Capability::GetHyperVPresent()
{
    BOOL HypervisorPresent = FALSE;
    UINT32 WrittenSizeInBytes = 0;
    HRESULT Status = WHvGetCapability(WHvCapabilityCodeHypervisorPresent,
                                      &HypervisorPresent, sizeof(BOOL), &WrittenSizeInBytes);
    if (Status != S_OK)
        throw CapabilityException(GetWin32LastError());

    return HypervisorPresent;
}

void Capability::FillFeatures(_Out_ WHV_CAPABILITY_FEATURES &Features)
{
    GetWin32HyperVCapability<WHvCapabilityCodeFeatures>(Features);
    spdlog::info("Fill features successfully");
}

void Capability::FillExtendedVMExits(_Out_ WHV_EXTENDED_VM_EXITS &ExtendedVmExits)
{
    GetWin32HyperVCapability<WHvCapabilityCodeExtendedVmExits>(ExtendedVmExits);
    spdlog::info("Fill extended VM exits successfully");
}

void Capability::FillProcessorFeatures(_Out_ WHV_PROCESSOR_FEATURES &ProcessorFeatures)
{
    GetWin32HyperVCapability<WHvCapabilityCodeProcessorFeatures>(ProcessorFeatures);
    spdlog::info("Fill processor's feature successfully");
}

void Capability::FillProcessorXsaveFeatures(_Out_ WHV_PROCESSOR_XSAVE_FEATURES &ProcessorXSaveFeature)
{
    GetWin32HyperVCapability<WHvCapabilityCodeProcessorXsaveFeatures>(ProcessorXSaveFeature);
    spdlog::info("Fill processor's xsave feature successfully");
}

void Capability::FillGetProcessorVednors(_Out_ WHV_PROCESSOR_VENDOR &Vendors)
{
    GetWin32HyperVCapability<WHvCapabilityCodeProcessorVendor>(Vendors);
    spdlog::info("Fill processor's vendor successfully");
}