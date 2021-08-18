// Documents :
// https://docs.microsoft.com/en-us/virtualization/api/hypervisor-platform/funcs/whvgetcapability

#include <string>

#include <Windows.h>
#include <WinHvPlatform.h>

#include "spdlog/spdlog.h"

#pragma comment(lib, "WinHvPlatform.lib")

VOID WINAPI
CheckLastError()
{
    LPSTR MsgBuf;

    SIZE_T WrittenBufferSize = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&MsgBuf,
        0,
        NULL);

    std::string Msg(MsgBuf, WrittenBufferSize);
    spdlog::error(Msg);
    LocalFree(MsgBuf);
}

BOOL WINAPI
CheckHyperVPresent()
{
    BOOL HypervisorPresent = FALSE;
    UINT32 WrittenSizeInBytes = 0;

    HRESULT Status = WHvGetCapability(WHvCapabilityCodeHypervisorPresent,
                                      &HypervisorPresent, sizeof(BOOL), &WrittenSizeInBytes);
    if (Status != S_OK)
    {
        CheckLastError();
        return S_FALSE;
    }

    return HypervisorPresent;
}

HRESULT WINAPI
GetHyperVFeatures(_Out_ WHV_CAPABILITY_FEATURES &Features)
{
    UINT32 WrittenSizeInBytes = 0;
    HRESULT Status = WHvGetCapability(WHvCapabilityCodeFeatures,
                                      &Features, sizeof(Features), &WrittenSizeInBytes);
    if (Status != S_OK)
    {
        CheckLastError();
        return S_FALSE;
    }

    return S_OK;
}

HRESULT WINAPI
GetExtendedVMExits(_Out_ WHV_EXTENDED_VM_EXITS &ExtendedVmExits)
{
    UINT32 WrittenSizeInBytes = 0;
    HRESULT Status = WHvGetCapability(WHvCapabilityCodeExtendedVmExits,
                                      &ExtendedVmExits, sizeof(ExtendedVmExits), &WrittenSizeInBytes);
    if (Status != S_OK)
    {
        CheckLastError();
        return S_FALSE;
    }

    return S_OK;
}

HRESULT WINAPI
GetProcessorFeatures(_Out_ WHV_PROCESSOR_FEATURES &ProcessorFeatures)
{
    UINT32 WrittenSizeInBytes = 0;
    HRESULT Status = WHvGetCapability(WHvCapabilityCodeProcessorFeatures,
                                      &ProcessorFeatures, sizeof(ProcessorFeatures), &WrittenSizeInBytes);
    if (Status != S_OK)
    {
        CheckLastError();
        return S_FALSE;
    }

    return S_OK;
}

HRESULT WINAPI
GetProcessorXsaveFeatures(_Out_ WHV_PROCESSOR_XSAVE_FEATURES &ProcessorXSaveFeature)
{
    UINT32 WrittenSizeInBytes = 0;
    HRESULT Status = WHvGetCapability(WHvCapabilityCodeProcessorXsaveFeatures,
                                      &ProcessorXSaveFeature, sizeof(ProcessorXSaveFeature), &WrittenSizeInBytes);
    if (Status != S_OK)
    {
        CheckLastError();
        return S_FALSE;
    }

    return S_OK;
}

HRESULT WINAPI
GetProcessorVednors(_Out_ WHV_PROCESSOR_VENDOR &Vendors)
{
    UINT32 WrittenSizeInBytes = 0;
    HRESULT Status = WHvGetCapability(WHvCapabilityCodeProcessorVendor,
                                      &Vendors, sizeof(Vendors), &WrittenSizeInBytes);
    if (Status != S_OK)
    {
        CheckLastError();
        return S_FALSE;
    }

    return S_OK;
}

HRESULT WINAPI
CheckHyperVCapability()
{
    /*
        The WHvCapabilityCodeHypervisorPresent capability can be used to 
        determine whether the Windows Hypervisor is running on a host and 
        the functions of the platform APIs can be used to create VM 
        partitions.
    */
    CONST BOOL IsHypervisorPresent = CheckHyperVPresent();

    /*  
        The WHvCapabilityCodeFeatures capability is reserved for future 
        use, it returns 0. 
    */
    WHV_CAPABILITY_FEATURES Features;

    /*
        For the WHvCapabilityCodeExtendedVmExits capability, the buffer 
        contains a bit field that specifies which additional exit reasons 
        are available that can be configured to cause the execution of a 
        virtual processor to be halted (see WHvRunVirtualProcessor).

        The values returned for the processor properties are based on the 
        capabilities of the physical processor on the system (i.e., they are 
        retrieved by querying the corresponding properties of the root partition.
    */
    WHV_EXTENDED_VM_EXITS ExtendedVmExits;

    /* processor's vendor */
    WHV_PROCESSOR_VENDOR Vendors = WHV_PROCESSOR_VENDOR::WHvProcessorVendorHygon;

    /* processor's features */
    WHV_PROCESSOR_FEATURES ProcessorFeatures;

    /*  processor's XSAVE feature.
        XSAVE - Save Processor Extended States.

        Performs a full or partial save of processor state components to the XSAVE 
        area located at the memory address specified by the destination operand.
    */
    WHV_PROCESSOR_XSAVE_FEATURES ProcessorXSaveFeature;

    if (IsHypervisorPresent)
    {
        /* Capabilities of the API implementation */
        GetHyperVFeatures(Features);
        GetExtendedVMExits(ExtendedVmExits);

        /* Capabilities of the system's processor */
        GetProcessorVednors(Vendors);
        GetProcessorFeatures(ProcessorFeatures);
        GetProcessorXsaveFeatures(ProcessorXSaveFeature);
    }

    return S_OK;
}

INT WINAPI
main(INT argc, PCHAR argv[], PCHAR envp[])
{
    spdlog::set_level(spdlog::level::level_enum::debug);
    spdlog::debug("----- RSC Project : learn HyperVisor -----");

    PCSTR ProgramFilename = ".exe";

    if (CheckHyperVCapability() == S_FALSE)
    {
        spdlog::error("This machine does not support Hyper-V feature or \
         is disabled. The program is gonna be terminated");
        return EXIT_SUCCESS;
    };

    return EXIT_SUCCESS;
}