// Documents :
// https://docs.microsoft.com/en-us/virtualization/api/hypervisor-platform/funcs/whvgetcapability

#include <iostream>
#include <string>

#include <Windows.h>
#include <WinHvPlatform.h>

#pragma comment(lib, "WinHvPlatform.lib")

VOID CheckLastError()
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
    std::cerr << Msg << std::endl;
    LocalFree(MsgBuf);
}

BOOL CheckHyperVPresent()
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

HRESULT GetHyperVFeatures(WHV_CAPABILITY_FEATURES &Features)
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

HRESULT GetExtendedVMExits(WHV_EXTENDED_VM_EXITS &ExtendedVmExits)
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

HRESULT CheckHyperVCapability()
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

    if (IsHypervisorPresent)
    {
        GetHyperVFeatures(Features);
        GetExtendedVMExits(ExtendedVmExits);
    }

    return S_OK;
}

INT main(INT argc, PCHAR argv[], PCHAR envp[])
{
    PCSTR ProgramFilename = "test.exe";
    CheckHyperVCapability();
}