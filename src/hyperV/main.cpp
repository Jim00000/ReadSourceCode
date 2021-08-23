// Documents :
// https://docs.microsoft.com/en-us/virtualization/api/hypervisor-platform/funcs/whvgetcapability

#include "Common.hpp"
#include "HyperVCapability.hpp"

#pragma comment(lib, "WinHvPlatform.lib")

/*
    References :
    https://stackoverflow.com/questions/1113409/attribute-constructor-equivalent-in-vc
    https://boringssl.googlesource.com/boringssl/+/refs/tags/version_for_cocoapods_4.0/crypto/crypto.c
*/
#pragma section(".CRT$XCU", read)
static void __constructor(VOID)
{
    spdlog::set_level(spdlog::level::level_enum::debug);
}

__declspec(allocate(".CRT$XCU")) VOID (*__init_constructor)(VOID) = __constructor;

INT WINAPI
main(INT argc, PCHAR argv[], PCHAR envp[])
try
{
    spdlog::info("----- RSC Project : learn HyperVisor -----");

    PCSTR ProgramFilename = ".exe";

    HyperV::Capability Cap;

    // Check VM-Exits
    if(Cap.GetExtendedVmExits().X64CpuidExit)
    {
        
    }

    spdlog::info("----- Program terminates -----");
    return EXIT_SUCCESS;
}
catch (...)
{
    spdlog::critical("Unknown error occurs. Abort the program");
    return EXIT_FAILURE;
}