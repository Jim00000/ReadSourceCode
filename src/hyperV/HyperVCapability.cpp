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

void Capability::CheckAvailability()
{
    if (this->mInitialized == false)
    {
        this->mIsHyperVPresent = GetHyperVPresent();
        if (this->mIsHyperVPresent)
            try
            {
                spdlog::info("Hyper-V is present");
                FillFeatures(this->mFeatures);
                FillExtendedVMExits(this->mExtendedVmExits);
                FillProcessorFeatures(this->mProcessorFeatures);
                FillProcessorXsaveFeatures(this->mProcessorXSaveFeature);
                FillGetProcessorVednors(this->mVendors);
            }
            catch (const CapabilityException &e)
            {
                spdlog::warn(e.what());
            }
        this->mInitialized = true;
    }
    else
    {
        spdlog::info("Availability of Hyper-V has been checked. Bypass duplicate check process");
    }
}

Capability::Capability() : mIsHyperVPresent{false}, mInitialized{false}
{
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